#include "tuix_registry.h"
#include "types.h"
#include "buffers.h"
#include "buffer_manager.h"
#include "compositor/geometry_resolver.h"
#include "main.h"
#include "compositor/compositor.h"
#include "viewport.h"
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
static int tuix_rebuild_buffer_if_needed(TuixBuffer *buffer, int size_changed);
static void tuix_rebuild_buffer_subtree(TuixScene *scene, int uid, int size_changed, unsigned char *visited, int visited_cap, int *rebuilt_any);
static int is_mouse_focus_click_event(const TuixMouseKey *mouse);
static int scene_pick_uid_from_hitmap(TuixScene *scene, int col1, int row1);
static int scene_pick_viewport_uid_from_hitmap(TuixScene *scene, int col1, int row1);
static int scene_has_pending_visual_changes(TuixScene *scene);
static int buffer_is_viewport(const TuixBuffer *buffer);
static int dispatch_mouse_via_hitmap(TuixScene *scene, TuixInputSnapshot *snap, int mouse_routed_uid, int mouse_viewport_uid);
static inline TuixBuffer *scene_buffer_by_uid(TuixScene *scene, int uid);
static int scene_uid_in_subtree(TuixScene *scene, int uid, int ancestor_uid);
static int scene_get_active_modal_uid(TuixScene *scene);

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
static int s_mouse_capture_uid = -1;

static int buffer_is_viewport(const TuixBuffer *buffer) {
    return buffer && buffer->obj && tuix_object_is_viewport(buffer->obj);
}

static int mouse_event_is_wheel_like(const TuixMouseKey *mouse) {
    if (!mouse || !mouse->has_event) {
        return 0;
    }
    return mouse->event == TUIX_MOUSE_SCROLL_UP ||
           mouse->event == TUIX_MOUSE_SCROLL_DOWN ||
           mouse->event == TUIX_MOUSE_HSCROLL_LEFT ||
           mouse->event == TUIX_MOUSE_HSCROLL_RIGHT;
}

static int mouse_event_is_left_drag_sequence(const TuixMouseKey *mouse) {
    if (!mouse || !mouse->has_event) {
        return 0;
    }
    if (mouse->event == TUIX_MOUSE_PRESS || mouse->event == TUIX_MOUSE_RELEASE) {
        return mouse->btn == TUIX_BTN_LEFT;
    }
    if (mouse->event == TUIX_MOUSE_DRAG) {
        return mouse->btn == TUIX_BTN_LEFT || (mouse->buttons_held & (1 << TUIX_BTN_LEFT)) != 0;
    }
    return 0;
}

static int append_route_uid(int *uids, int *count, int capacity, int uid) {
    if (!uids || !count || uid <= 0 || *count >= capacity) {
        return 0;
    }
    for (int i = 0; i < *count; i++) {
        if (uids[i] == uid) {
            return 0;
        }
    }
    uids[(*count)++] = uid;
    return 1;
}

static int build_mouse_route_chain(TuixScene *scene, const TuixMouseKey *mouse, int mouse_routed_uid, int mouse_viewport_uid, int *uids, int capacity) {
    if (!scene || !mouse || !uids || capacity <= 0) {
        return 0;
    }

    if (s_mouse_capture_uid > 0 && !scene_buffer_by_uid(scene, s_mouse_capture_uid)) {
        s_mouse_capture_uid = -1;
    }

    int count = 0;
    if (mouse_event_is_left_drag_sequence(mouse) &&
        s_mouse_capture_uid > 0 &&
        (mouse->event == TUIX_MOUSE_DRAG || mouse->event == TUIX_MOUSE_RELEASE)) {
        append_route_uid(uids, &count, capacity, s_mouse_capture_uid);
        return count;
    }

    if (mouse_routed_uid <= 0) {
        return 0;
    }

    append_route_uid(uids, &count, capacity, mouse_routed_uid);

    int current = mouse_viewport_uid;
    while (current > 0 && count < capacity) {
        TuixBuffer *viewport = scene_buffer_by_uid(scene, current);
        if (!viewport) {
            break;
        }
        append_route_uid(uids, &count, capacity, current);

        current = viewport->parent_uid;
        while (current > 0) {
            TuixBuffer *ancestor = scene_buffer_by_uid(scene, current);
            if (!ancestor) {
                current = -1;
                break;
            }
            if (buffer_is_viewport(ancestor)) {
                break;
            }
            current = ancestor->parent_uid;
        }
    }

    return count;
}

static int dispatch_mouse_via_hitmap(TuixScene *scene, TuixInputSnapshot *snap, int mouse_routed_uid, int mouse_viewport_uid) {
    if (!scene || !snap || snap->consumed_mouse || !snap->mouse || !snap->mouse->has_event) {
        return 0;
    }

    int route_uids[64];
    int route_count = build_mouse_route_chain(scene, snap->mouse, mouse_routed_uid, mouse_viewport_uid, route_uids, 64);
    int requires_redraw = 0;

    for (int i = 0; i < route_count && !snap->consumed_mouse; i++) {
        TuixBuffer *buffer = scene_buffer_by_uid(scene, route_uids[i]);
        if (!buffer || !buffer->obj || !buffer->obj->builder || !buffer->obj->builder->on_event) {
            continue;
        }

        TuixInputSnapshot routed_snap = *snap;
        TuixMouseKey routed_mouse = *snap->mouse;
        routed_snap.mouse = &routed_mouse;

        TuixKeyboardKey muted_keyboard;
        if (routed_snap.keyboard) {
            muted_keyboard = *routed_snap.keyboard;
            muted_keyboard.has_event = 0;
            routed_snap.keyboard = &muted_keyboard;
        }

        bool is_focused = (scene->current_focus == buffer->obj->uid);
        TuixHandlerResponse ans = buffer->obj->builder->on_event(buffer->obj, true, is_focused, &routed_snap);
        if (ans.requires_redraw) {
            buffer->required_redraw |= ans.requires_redraw;
            requires_redraw = 1;
        }

        if (routed_snap.consumed_mouse) {
            snap->consumed_mouse = true;
        }
    }

    if (mouse_event_is_left_drag_sequence(snap->mouse) &&
        snap->mouse->event == TUIX_MOUSE_RELEASE) {
        s_mouse_capture_uid = -1;
    }

    return requires_redraw;
}

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

int tuix_mouse_capture_begin(int uid) {
    if (uid <= 0) {
        return -1;
    }
    s_mouse_capture_uid = uid;
    return 0;
}

int tuix_mouse_capture_end(int uid) {
    if (uid <= 0 || s_mouse_capture_uid == uid) {
        s_mouse_capture_uid = -1;
        return 0;
    }
    return -1;
}

int tuix_get_mouse_capture_uid(void) {
    return s_mouse_capture_uid;
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

/* Per-frame list of buffers updated by builders (populated by
    `tuix_loop_generate_buffers`). Used to enable single-buffer blit
    optimizations when possible. */
static int s_last_changed_count = 0;
static int s_last_changed_uids[64];

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
    if (!scene || uid <= 0 || !scene->buffer_by_uid || uid >= scene->max_uid_capacity) {
        return NULL;
    }
    return scene->buffer_by_uid[uid];
}

static int scene_uid_in_subtree(TuixScene *scene, int uid, int ancestor_uid) {
    if (!scene || uid <= 0 || ancestor_uid <= 0) {
        return 0;
    }
    int guard = 0;
    int current = uid;
    while (current > 0 && guard < 1024) {
        if (current == ancestor_uid) {
            return 1;
        }
        TuixBuffer *buffer = scene_buffer_by_uid(scene, current);
        if (!buffer) {
            break;
        }
        current = buffer->parent_uid;
        guard++;
    }
    return 0;
}

static int scene_get_active_modal_uid(TuixScene *scene) {
    if (!scene || scene->active_modal_uid <= 0) {
        return -1;
    }
    if (!scene_buffer_by_uid(scene, scene->active_modal_uid)) {
        scene->active_modal_uid = -1;
        scene->modal_restore_focus_uid = -1;
        return -1;
    }
    return scene->active_modal_uid;
}

static void scene_cycle_focus(TuixScene *scene) {
    if (!scene || scene->count <= 0) return;
    int active_modal_uid = scene_get_active_modal_uid(scene);
    if (scene->current_focus <= 0) {
        if (active_modal_uid > 0) {
            scene->current_focus = active_modal_uid;
        } else {
            scene->current_focus = scene->buffers[0]->obj->uid;
        }
        return;
    }
    int idx = -1;
    for (int i = 0; i < scene->count; i++) {
        if (scene->buffers[i]->obj->uid == scene->current_focus) {
            idx = i;
            break;
        }
    }
    if (active_modal_uid > 0 && !scene_uid_in_subtree(scene, scene->current_focus, active_modal_uid)) {
        scene->current_focus = active_modal_uid;
        return;
    }
    if (idx < 0) {
        scene->current_focus = active_modal_uid > 0 ? active_modal_uid : scene->buffers[0]->obj->uid;
        return;
    }
    for (int step = 1; step <= scene->count; step++) {
        int candidate_uid = scene->buffers[(idx + step) % scene->count]->obj->uid;
        if (active_modal_uid > 0 && !scene_uid_in_subtree(scene, candidate_uid, active_modal_uid)) {
            continue;
        }
        scene->current_focus = candidate_uid;
        return;
    }
    scene->current_focus = active_modal_uid > 0 ? active_modal_uid : scene->current_focus;
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

static int scene_pick_viewport_uid_from_hitmap(TuixScene *scene, int col1, int row1) {
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
    int uid = hitmap->pixels[idx].viewport_uid;
    if (uid <= 0) {
        return -1;
    }

    TuixBuffer *buffer = scene_buffer_by_uid(scene, uid);
    if (!buffer || !buffer->obj || !buffer_is_viewport(buffer)) {
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
    int active_modal_uid = scene_get_active_modal_uid(scene);
    if (active_modal_uid > 0 &&
        !scene_uid_in_subtree(scene, scene->current_focus, active_modal_uid)) {
        scene->current_focus = active_modal_uid;
    }

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
    int mouse_viewport_uid = -1;
    if (has_mouse_event) {
        mouse_routed_uid = scene_pick_uid_from_hitmap(scene, snap.mouse->col, snap.mouse->row);
        mouse_viewport_uid = scene_pick_viewport_uid_from_hitmap(scene, snap.mouse->col, snap.mouse->row);
        if (active_modal_uid > 0) {
            if (mouse_routed_uid <= 0 || !scene_uid_in_subtree(scene, mouse_routed_uid, active_modal_uid)) {
                mouse_routed_uid = active_modal_uid;
            }
            if (mouse_viewport_uid > 0 && !scene_uid_in_subtree(scene, mouse_viewport_uid, active_modal_uid)) {
                mouse_viewport_uid = -1;
            }
        }
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
        if (active_modal_uid > 0 &&
            clicked_uid > 0 &&
            !scene_uid_in_subtree(scene, clicked_uid, active_modal_uid)) {
            clicked_uid = active_modal_uid;
        }
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

    if (dispatch_mouse_via_hitmap(scene, &snap, mouse_routed_uid, mouse_viewport_uid)) {
        frame_needs_compose = 1;
    }

    bool has_event = has_input_event || size_changed;

    stage_start = now_seconds();

    if (scene_subcycles) {
        for (int j = 0; j < scene_subcycles->count; j++) {
            TuixSubcycle *subcycle = scene_subcycles->subcycles[j];
            if (!subcycle->on_event) continue;
            if (active_modal_uid > 0 &&
                (!subcycle->obj || !scene_uid_in_subtree(scene, subcycle->obj->uid, active_modal_uid))) {
                continue;
            }
            bool all_input_consumed = has_input_event &&
                                      (!has_keyboard_event || snap.consumed_keyboard) &&
                                      (!has_mouse_event || snap.consumed_mouse);
            if (!size_changed && all_input_consumed && !has_mouse_event) break;

            TuixInputSnapshot routed_snap = snap;
            TuixMouseKey routed_mouse;
            if (has_mouse_event && snap.mouse) {
                routed_mouse = *snap.mouse;
                routed_mouse.has_event = 0;
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
            if (size_changed || scene->topology_dirty || scene->last_composited_topology_version == 0 ||
                scene->last_composited_topology_version != scene->topology_version) {
                forced = 1;
            }

            /* Fast-path: if exactly one buffer changed and the scene topology
               and size are unchanged, perform a targeted blit into the
               persistent final buffer instead of recompositing the whole
               scene. */
            if (!forced && s_last_changed_count == 1) {
                int changed_uid = s_last_changed_uids[0];
                TuixBuffer *changed_buf = scene_buffer_by_uid(scene, changed_uid);
                if (changed_buf && changed_buf->pixels && !buffer_is_viewport(changed_buf)) {
                    TuixFinalBuffer *final_ptr = tuix_blit_buffer(scene, changed_buf);
                    tuix_render_streaming(final_ptr, NULL, NULL);
                    scene->last_composited_topology_version = scene->topology_version;
                    if (debug_stats) {
                        s_last_core_loop_stats.traversal_cache_hit = 1;
                    }
                    goto COMPOSE_DONE;
                }
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
    COMPOSE_DONE: ;
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
    s_last_changed_count = 0;

    int visited_cap = scene->max_uid_capacity > 0 ? scene->max_uid_capacity : 1;
    unsigned char *visited = (unsigned char*)calloc((size_t)visited_cap, sizeof(unsigned char));
    if (!visited) {
        return 1;
    }

    int rebuilt_any = 0;
    for (int i = 0; i < scene->root_count; i++) {
        tuix_rebuild_buffer_subtree(scene, scene->root_uids[i], size_changed, visited, visited_cap, &rebuilt_any);
    }

    for (int i = 0; i < scene->count; i++) {
        TuixBuffer *buffer = scene->buffers[i];
        if (!buffer || !buffer->obj || buffer->obj->uid <= 0 || buffer->obj->uid >= visited_cap) {
            continue;
        }
        if (visited[buffer->obj->uid]) {
            continue;
        }
        tuix_rebuild_buffer_subtree(scene, buffer->obj->uid, size_changed, visited, visited_cap, &rebuilt_any);
    }

    free(visited);
    return rebuilt_any;
}

static void tuix_rebuild_buffer_subtree(TuixScene *scene, int uid, int size_changed, unsigned char *visited, int visited_cap, int *rebuilt_any) {
    if (!scene || !visited || uid <= 0 || uid >= visited_cap || visited[uid]) {
        return;
    }

    visited[uid] = 1;
    TuixBuffer *buffer = scene_buffer_by_uid(scene, uid);
    if (!buffer || !buffer->obj) {
        return;
    }

    if (tuix_rebuild_buffer_if_needed(buffer, size_changed)) {
        if (rebuilt_any) {
            *rebuilt_any = 1;
        }
    }

    for (int i = 0; i < buffer->children_count; i++) {
        tuix_rebuild_buffer_subtree(scene, buffer->children_uids[i], size_changed, visited, visited_cap, rebuilt_any);
    }
}

static int tuix_rebuild_buffer_if_needed(TuixBuffer *buffer, int size_changed) {
    if (!buffer || !buffer->obj || !buffer->obj->builder || !buffer->obj->builder->build_content) {
        return 0;
    }

    int needs_geometry = (size_changed || buffer->width == 0 || buffer->height == 0);
    int needs_build = (buffer->required_redraw != 0) || needs_geometry;
    if (!needs_build) {
        return 0;
    }

    if (needs_geometry) {
        tuix_resolve_geometry(buffer);
        if (buffer->obj->builder->on_resize) {
            buffer->obj->builder->on_resize(buffer->obj, buffer, buffer->width, buffer->height);
            buffer->required_redraw = 1;
        }
    }

    if (buffer->obj->builder->layout_children) {
        buffer->obj->builder->layout_children(buffer->obj, buffer);
    }

    TuixPixel* new_pixels = buffer->obj->builder->build_content(buffer->obj, buffer);
    if (!new_pixels) {
        return 1;
    }

    TuixPixel* old_pixels = buffer->pixels;
    if (old_pixels && old_pixels == new_pixels) {
        buffer->required_redraw = 0;
        return 1;
    }

    int w = buffer->width;
    int h = buffer->height;
    size_t n = (size_t)w * (size_t)h;
    int returns_temporary_pixels =
        buffer->obj->builder->returns_temporary_pixels ||
        buffer->obj->state == NULL;
    TuixPixel* core_pixels = NULL;
    if (n > 0) {
        core_pixels = (TuixPixel*)calloc(n, sizeof(TuixPixel));
        if (core_pixels) memcpy(core_pixels, new_pixels, sizeof(TuixPixel) * n);
    }

    if (returns_temporary_pixels) {
        free(new_pixels);
    }

    if (old_pixels && buffer->pixels_owned) {
        free(old_pixels);
    }

    buffer->pixels = core_pixels;
    buffer->pixels_owned = core_pixels ? 1 : 0;
    buffer->required_redraw = 0;

    if (core_pixels != old_pixels && buffer->obj && s_last_changed_count < (int)(sizeof(s_last_changed_uids)/sizeof(s_last_changed_uids[0]))) {
        s_last_changed_uids[s_last_changed_count++] = buffer->obj->uid;
    }

    return 1;
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
