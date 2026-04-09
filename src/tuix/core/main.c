#include "tuix_registry.h"
#include "types.h"
#include "buffers.h"
#include "buffer_manager.h"
#include "compositor/geometry_resolver.h"
#include "main.h"
#include "compositor/compositor.h"
#include "rendering.h"
#include "batch_executor.h"
#include "input/input.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static int tuix_loop_generate_buffers(TuixScene *scene, int size_changed);
static int is_mouse_focus_click_event(const TuixMouseKey *mouse);
static int scene_pick_uid_from_hitmap(TuixScene *scene, int col1, int row1);
static int scene_has_pending_visual_changes(TuixScene *scene);

static int tuix_debug_stats_enabled(void) {
    return tuix_registry.debug_config == TUIX_DEBUG_STATS ||
           tuix_registry.debug_config == TUIX_DEBUG_VERBOSE ||
           tuix_registry.debug_config == TUIX_DEBUG_PROFILE;
}

static double now_seconds(void) {
#ifdef _WIN32
    static LARGE_INTEGER freq = {0};
    LARGE_INTEGER counter;
    if (freq.QuadPart == 0) {
        QueryPerformanceFrequency(&freq);
    }
    QueryPerformanceCounter(&counter);
    if (freq.QuadPart <= 0) {
        return (double)GetTickCount64() / 1000.0;
    }
    return (double)counter.QuadPart / (double)freq.QuadPart;
#else
    struct timespec ts;
#if defined(CLOCK_MONOTONIC)
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
    }
#endif
    return (double)time(NULL);
#endif
}

static TuixCoreLoopStats s_last_core_loop_stats = {0};

void tuix_get_core_loop_stats(TuixCoreLoopStats* out_stats) {
    if (!out_stats) {
        return;
    }
    if (!tuix_debug_stats_enabled()) {
        memset(out_stats, 0, sizeof(*out_stats));
        return;
    }
    *out_stats = s_last_core_loop_stats;
}

void tuix_get_terminal_size(int* width, int* height) {
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(out, &csbi)) {
        *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        *width = 80;
        *height = 24;
    }
    #else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        *width = w.ws_col;
        *height = w.ws_row;
    } else {
        *width = 80;
        *height = 24;
    }
    #endif

    if (*width <= 0) *width = 80;
    if (*height <= 0) *height = 24;

    return;
}

/* Cache resolved scene and subcycle pointers to avoid strcmp on every frame.
   Invalidated when current_scene_name pointer changes. */
static const char       *s_cached_scene_name = NULL;
static TuixScene        *s_cached_scene      = NULL;
static TuixSceneSubcycles *s_cached_subcycles = NULL;

static void refresh_scene_cache(void) {
    tuix_lock();
    if (s_cached_scene_name == tuix_registry.current_scene_name && s_cached_scene && s_cached_subcycles)
        {
            tuix_unlock();
            return; /* pointer-equal: same interned string, nothing changed */
        }

    s_cached_scene = tuix_get_scene(tuix_registry.current_scene_name);
    s_cached_subcycles = NULL;
    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        if (strcmp(tuix_registry.subcycles.subcycles[i]->scene_name, tuix_registry.current_scene_name) == 0) {
            s_cached_subcycles = tuix_registry.subcycles.subcycles[i];
            break;
        }
    }
    s_cached_scene_name = tuix_registry.current_scene_name;
    tuix_unlock();
}

/* Fast scene-local buffer lookup - avoids the global all-scenes search. */
static inline TuixBuffer *scene_buffer_by_uid(TuixScene *scene, int uid) {
    TuixBuffer **bufs = scene->buffers;
    int n = scene->count;
    for (int i = 0; i < n; i++) {
        if (bufs[i]->obj->uid == uid)
            return bufs[i];
    }
    return NULL;
}

static void scene_cycle_focus(TuixScene *scene) {
    if (!scene || scene->count <= 0) return;
    if (scene->current_focus <= 0) {
        scene->current_focus = scene->buffers[0]->obj->uid;
        return;
    }
    int idx = -1;
    for (int i = 0; i < scene->count; i++) {
        if (scene->buffers[i]->obj->uid == scene->current_focus) {
            idx = i;
            break;
        }
    }
    if (idx < 0) {
        scene->current_focus = scene->buffers[0]->obj->uid;
        return;
    }
    scene->current_focus = scene->buffers[(idx + 1) % scene->count]->obj->uid;
}

static int is_mouse_focus_click_event(const TuixMouseKey *mouse) {
    if (!mouse || !mouse->has_event) {
        return 0;
    }
    return (mouse->event == TUIX_MOUSE_PRESS || mouse->event == TUIX_MOUSE_DOUBLE_CLICK);
}

static int scene_pick_uid_from_hitmap(TuixScene *scene, int col1, int row1) {
    if (!scene || col1 <= 0 || row1 <= 0) {
        return -1;
    }

    TuixHitmap *hitmap = tuix_get_last_hitmap();
    if (!hitmap || !hitmap->pixels || hitmap->width <= 0 || hitmap->height <= 0) {
        return -1;
    }

    int x = col1 - 1;
    int y = row1 - 1;
    if (x < 0 || y < 0 || x >= hitmap->width || y >= hitmap->height) {
        return -1;
    }

    int idx = y * hitmap->width + x;
    int uid = hitmap->pixels[idx].obj_uid;
    if (uid <= 0) {
        return -1;
    }

    TuixBuffer *buffer = scene_buffer_by_uid(scene, uid);
    if (!buffer || !buffer->obj) {
        return -1;
    }

    return uid;
}

void tuix_main_loop() {
    int debug_stats = tuix_debug_stats_enabled();
    double frame_start = 0.0;
    if (debug_stats) {
        frame_start = now_seconds();
        memset(&s_last_core_loop_stats, 0, sizeof(s_last_core_loop_stats));
    }
    tuix_registry.frame_counter++;
    if (debug_stats) {
        s_last_core_loop_stats.frame_counter = tuix_registry.frame_counter;
    }

    tuix_get_terminal_size(&tuix_registry.terminal_width, &tuix_registry.terminal_height);
    /* Commit any pending global batch commands before processing frame */
    extern void tuix_batch_global_commit(void);
    double stage_start = now_seconds();
    tuix_batch_global_commit();
    if (debug_stats) {
        s_last_core_loop_stats.batch_commit_ms = (now_seconds() - stage_start) * 1000.0;
    }
    
    if (!tuix_registry.current_scene_name) {
        if (debug_stats) {
            s_last_core_loop_stats.total_ms = (now_seconds() - frame_start) * 1000.0;
        }
        return;
    }

    stage_start = now_seconds();
    refresh_scene_cache();
    if (debug_stats) {
        s_last_core_loop_stats.cache_refresh_ms = (now_seconds() - stage_start) * 1000.0;
    }
    TuixScene *scene = s_cached_scene;
    TuixSceneSubcycles *scene_subcycles = s_cached_subcycles;
    if (debug_stats) {
        s_last_core_loop_stats.has_scene = scene != NULL;
        s_last_core_loop_stats.has_subcycles = scene_subcycles != NULL;
    }
    if (!scene) {
        if (debug_stats) {
            s_last_core_loop_stats.total_ms = (now_seconds() - frame_start) * 1000.0;
        }
        return;
    }
    scene->last_active_frame = tuix_registry.frame_counter;

    int size_changed = (tuix_registry.terminal_width != tuix_registry.terminal_width_old ||
                        tuix_registry.terminal_height != tuix_registry.terminal_height_old);
    if (size_changed) {
        tuix_registry.terminal_width_old  = tuix_registry.terminal_width;
        tuix_registry.terminal_height_old = tuix_registry.terminal_height;
    }

    stage_start = now_seconds();
    TuixInputSnapshot snap = get_input_snapshot();
    if (debug_stats) {
        s_last_core_loop_stats.input_ms = (now_seconds() - stage_start) * 1000.0;
    }
    bool has_keyboard_event = (snap.keyboard && snap.keyboard->has_event);
    bool has_mouse_event = (snap.mouse && snap.mouse->has_event);
    int focus_changed = 0;
    int frame_needs_compose = size_changed;
    int mouse_routed_uid = -1;
    if (has_mouse_event) {
        mouse_routed_uid = scene_pick_uid_from_hitmap(scene, snap.mouse->col, snap.mouse->row);
    }
    bool has_input_event = has_keyboard_event || has_mouse_event;
    if (has_input_event && snap.keyboard && snap.keyboard->has_event && snap.keyboard->code == 0x09) {
        int old_focus = scene->current_focus;
        scene_cycle_focus(scene);
        if (scene->current_focus != old_focus) {
            focus_changed = 1;
        }
        snap.consumed_keyboard = true;
    }

    if (has_mouse_event && is_mouse_focus_click_event(snap.mouse)) {
        int clicked_uid = mouse_routed_uid;
        if (clicked_uid > 0 && scene->current_focus != clicked_uid) {
            scene->current_focus = clicked_uid;
            focus_changed = 1;
        }
    }

    if (focus_changed) {
        for (int i = 0; i < scene->count; i++) {
            if (scene->buffers[i]) {
                scene->buffers[i]->required_redraw = 1;
            }
        }
    }

    bool has_event = has_input_event || size_changed;

    stage_start = now_seconds();

    if (scene_subcycles) {
        for (int j = 0; j < scene_subcycles->count; j++) {
            TuixSubcycle *subcycle = scene_subcycles->subcycles[j];
            if (!subcycle->on_event) continue;
            bool all_input_consumed = has_input_event &&
                                      (!has_keyboard_event || snap.consumed_keyboard) &&
                                      (!has_mouse_event || snap.consumed_mouse);
            if (!size_changed && all_input_consumed) break;

            TuixInputSnapshot routed_snap = snap;
            TuixMouseKey routed_mouse;
            if (has_mouse_event && snap.mouse) {
                routed_mouse = *snap.mouse;
                if (mouse_routed_uid <= 0 || subcycle->obj->uid != mouse_routed_uid) {
                    routed_mouse.has_event = 0;
                }
                routed_snap.mouse = &routed_mouse;
            }

            bool has_routed_input =
                (routed_snap.keyboard && routed_snap.keyboard->has_event) ||
                (routed_snap.mouse && routed_snap.mouse->has_event);

            bool is_focused = (scene->current_focus == subcycle->obj->uid);
            TuixHandlerResponse ans = subcycle->on_event(subcycle->obj, has_routed_input || size_changed, is_focused, &routed_snap);
            snap.consumed_keyboard = snap.consumed_keyboard || routed_snap.consumed_keyboard;
            snap.consumed_mouse = snap.consumed_mouse || routed_snap.consumed_mouse;
            if (size_changed) {
                ans.requires_redraw = 1;
            }
            if (ans.requires_redraw) {
                frame_needs_compose = 1;
            }

            TuixBuffer *buffer = scene_buffer_by_uid(scene, subcycle->obj->uid);
            if (buffer) {
                buffer->required_redraw |= ans.requires_redraw;
            }

        }
    }
    if (debug_stats) {
        s_last_core_loop_stats.routing_ms = (now_seconds() - stage_start) * 1000.0;
    }

    stage_start = now_seconds();
     /* Builder callbacks (on_resize/build_content) are intentionally run outside
         the global registry lock to reduce lock contention and callback latency. */
    frame_needs_compose |= tuix_loop_generate_buffers(scene, size_changed);
    if (debug_stats) {
        s_last_core_loop_stats.buffer_ms = (now_seconds() - stage_start) * 1000.0;
    }

    /* Composite only when visual state changed; fallback to full compose on uncertainty. */
    stage_start = now_seconds();
    tuix_lock();
    {
        int should_compose = frame_needs_compose || scene_has_pending_visual_changes(scene);
        if (should_compose) {
            int forced = 0;
            if (size_changed || scene->last_composited_topology_version == 0 ||
                scene->last_composited_topology_version != scene->topology_version) {
                forced = 1;
            }
            TuixFinalBuffer *final_ptr = tuix_composite_scene(scene);
            tuix_render_streaming(final_ptr, NULL, NULL);
            scene->last_composited_topology_version = scene->topology_version;
            if (debug_stats && forced) {
                s_last_core_loop_stats.composite_forced = 1;
            }
            if (debug_stats) {
                if (tuix_get_last_traversal_cache_hit()) {
                    s_last_core_loop_stats.traversal_cache_hit = 1;
                } else {
                    s_last_core_loop_stats.traversal_cache_miss = 1;
                }
            }
        } else {
            if (debug_stats) {
                s_last_core_loop_stats.composite_skipped = 1;
                if (scene->last_composited_topology_version != 0 &&
                    scene->last_composited_topology_version == scene->topology_version) {
                    s_last_core_loop_stats.traversal_cache_hit = 1;
                }
            }
        }
    }
    tuix_unlock();
    if (debug_stats) {
        s_last_core_loop_stats.composite_ms = (now_seconds() - stage_start) * 1000.0;
    }

    /* After rendering, commit any C->Python events as a single batched call */
    extern void tuix_event_commit_python(void);
    stage_start = now_seconds();
    tuix_event_commit_python();
    if (debug_stats) {
        s_last_core_loop_stats.python_commit_ms = (now_seconds() - stage_start) * 1000.0;
        s_last_core_loop_stats.size_changed = size_changed;
        s_last_core_loop_stats.has_input = has_input_event;
        s_last_core_loop_stats.total_ms = (now_seconds() - frame_start) * 1000.0;
    }
}

static int tuix_loop_generate_buffers(TuixScene *scene, int size_changed) {
    if (!scene) return 0;
    int rebuilt_any = 0;
    for (int i = 0; i < scene->count; i++) {
        TuixBuffer* buffer = scene->buffers[i];
        if (!buffer || !buffer->obj || !buffer->obj->builder || !buffer->obj->builder->build_content) continue;
        int needs_geometry = (size_changed || buffer->width == 0 || buffer->height == 0);
        int needs_build = (buffer->required_redraw != 0) || needs_geometry;
        if (!needs_build) continue;
        rebuilt_any = 1;
        /* Only re-resolve geometry when terminal size changed, or on first draw */
        if (needs_geometry) {
            tuix_resolve_geometry(buffer);
            /* Notify builder about resize so it can update internal state before building. */
            if (buffer->obj && buffer->obj->builder && buffer->obj->builder->on_resize) {
                buffer->obj->builder->on_resize(buffer->obj, buffer, buffer->width, buffer->height);
                /* Ensure a redraw is scheduled after resize-handling. */
                buffer->required_redraw = 1;
            }
        }
        TuixPixel* new_pixels = buffer->obj->builder->build_content(buffer->obj, buffer);
        if (!new_pixels) continue;
        /* If the builder returned a new allocation, free the old one.
         * Builders may return the same pointer for in-place updates. */
        TuixPixel* old_pixels = buffer->pixels;
        if (old_pixels && old_pixels != new_pixels) {
            free(old_pixels);
        }
        buffer->pixels = new_pixels;
        buffer->required_redraw = 0;
    }
    return rebuilt_any;
}

static int scene_has_pending_visual_changes(TuixScene *scene) {
    if (!scene) {
        return 1;
    }

    if (scene->last_composited_topology_version == 0) {
        return 1;
    }

    if (scene->last_composited_topology_version != scene->topology_version) {
        return 1;
    }

    for (int i = 0; i < scene->count; i++) {
        TuixBuffer *buffer = scene->buffers[i];
        if (!buffer) {
            continue;
        }
        if (buffer->required_redraw) {
            return 1;
        }
        if (buffer->width <= 0 || buffer->height <= 0 || !buffer->pixels) {
            return 1;
        }
    }
    return 0;
}