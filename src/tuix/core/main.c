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

static void tuix_loop_generate_buffers(TuixScene *scene, int size_changed);

void tuix_get_terminal_size(int* width, int* height) {
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *width = w.ws_col;
    *height = w.ws_row;
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
    if (s_cached_scene_name == tuix_registry.current_scene_name && s_cached_scene)
        return; /* pointer-equal: same interned string, nothing changed */

    s_cached_scene = tuix_get_scene(tuix_registry.current_scene_name);
    s_cached_subcycles = NULL;
    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        if (strcmp(tuix_registry.subcycles.subcycles[i]->scene_name, tuix_registry.current_scene_name) == 0) {
            s_cached_subcycles = tuix_registry.subcycles.subcycles[i];
            break;
        }
    }
    s_cached_scene_name = tuix_registry.current_scene_name;
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

void tuix_main_loop() {
    tuix_registry.frame_counter++;
    tuix_get_terminal_size(&tuix_registry.terminal_width, &tuix_registry.terminal_height);
    /* Commit any pending global batch commands before processing frame */
    extern void tuix_batch_global_commit(void);
    tuix_batch_global_commit();
    
    if (!tuix_registry.current_scene_name) {
        return;
    }

    refresh_scene_cache();
    TuixScene *scene = s_cached_scene;
    TuixSceneSubcycles *scene_subcycles = s_cached_subcycles;
    if (!scene || !scene_subcycles) return;
    scene->last_active_frame = tuix_registry.frame_counter;

    int size_changed = (tuix_registry.terminal_width != tuix_registry.terminal_width_old ||
                        tuix_registry.terminal_height != tuix_registry.terminal_height_old);
    if (size_changed) {
        tuix_registry.terminal_width_old  = tuix_registry.terminal_width;
        tuix_registry.terminal_height_old = tuix_registry.terminal_height;
    }

    TuixInputSnapshot snap = get_input_snapshot();
    bool has_keyboard_event = (snap.keyboard && snap.keyboard->has_event);
    bool has_mouse_event = (snap.mouse && snap.mouse->has_event);
    bool has_input_event = has_keyboard_event || has_mouse_event;
    if (has_input_event && snap.keyboard && snap.keyboard->has_event && snap.keyboard->code == 0x09) {
        scene_cycle_focus(scene);
        snap.consumed_keyboard = true;
    }
    bool has_event = has_input_event || size_changed;

    for (int j = 0; j < scene_subcycles->count; j++) {
        TuixSubcycle *subcycle = scene_subcycles->subcycles[j];
        if (!subcycle->on_event) continue;
        bool all_input_consumed = has_input_event &&
                                  (!has_keyboard_event || snap.consumed_keyboard) &&
                                  (!has_mouse_event || snap.consumed_mouse);
        if (!size_changed && all_input_consumed) break;

        bool is_focused = (scene->current_focus == subcycle->obj->uid);
        TuixHandlerResponse ans = subcycle->on_event(subcycle->obj, has_event, is_focused, &snap);
        if (size_changed) {
            ans.requires_redraw = 1;
        }

        TuixBuffer *buffer = scene_buffer_by_uid(scene, subcycle->obj->uid);
        if (buffer) {
            buffer->required_redraw |= ans.requires_redraw;
        }

    }

     /* Builder callbacks (on_resize/build_content) are intentionally run outside
         the global registry lock to reduce lock contention and callback latency. */
    tuix_loop_generate_buffers(scene, size_changed);

    /* Composite and render under lock to avoid concurrent mutations during resize. */
    tuix_lock();
    TuixFinalBuffer *final_ptr = tuix_composite_scene(scene);
    tuix_render_streaming(final_ptr, NULL, NULL);
    tuix_unlock();

    /* After rendering, commit any C->Python events as a single batched call */
    extern void tuix_event_commit_python(void);
    tuix_event_commit_python();
}

static void tuix_loop_generate_buffers(TuixScene *scene, int size_changed) {
    if (!scene) return;
    for (int i = 0; i < scene->count; i++) {
        TuixBuffer* buffer = scene->buffers[i];
        if (!buffer || !buffer->obj || !buffer->obj->builder || !buffer->obj->builder->build_content) continue;
        if (buffer->required_redraw == 0) continue;
        /* Only re-resolve geometry when terminal size changed, or on first draw */
        if (size_changed || buffer->width == 0 || buffer->height == 0) {
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
}