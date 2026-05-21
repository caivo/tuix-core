#include "splitpane_builder.h"

#include "../../buffer_manager.h"
#include "../../main.h"
#include "../../object_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TUIX_VK_LEFT  0x25
#define TUIX_VK_UP    0x26
#define TUIX_VK_RIGHT 0x27
#define TUIX_VK_DOWN  0x28

typedef struct {
    int orientation;
    int split_mode;
    float split_ratio;
    int split_pixels;
    int divider_size;
    int min_first;
    int min_second;
    int dragging;
    int hovered_divider;
    int focused;
    int resolved_first_size;
    int resolved_second_size;
    int resolved_divider_offset;
    int resolved_divider_size;
    int use_bg;
    TuixRGBTuple divider_fg;
    TuixRGBTuple divider_bg;
    TuixRGBTuple bg;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixSplitPaneState;

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static int min_int(int a, int b) {
    return a < b ? a : b;
}

static int clamp_int(int value, int min_value, int max_value) {
    if (value < min_value) value = min_value;
    if (value > max_value) value = max_value;
    return value;
}

static void splitpane_mark_layout_changed(TuixObject *obj, TuixSplitPaneState *state) {
    if (!obj || !state) return;
    state->needs_redraw = 1;
    tuix_mark_buffer_children_geometry_dirty_by_uid(obj->uid);
}

static void* splitpane_create_state(void* params) {
    (void)params;
    TuixSplitPaneState *state = (TuixSplitPaneState*)calloc(1, sizeof(TuixSplitPaneState));
    if (!state) return NULL;
    state->orientation = TUIX_LAYOUT_AXIS_HORIZONTAL;
    state->split_mode = TUIX_SPLIT_MODE_RATIO;
    state->split_ratio = 0.35f;
    state->split_pixels = 0;
    state->divider_size = 1;
    state->min_first = 8;
    state->min_second = 8;
    state->dragging = 0;
    state->hovered_divider = 0;
    state->focused = 0;
    state->use_bg = 1;
    state->divider_fg = (TuixRGBTuple){210, 220, 235};
    state->divider_bg = (TuixRGBTuple){54, 62, 82};
    state->bg = (TuixRGBTuple){20, 24, 34};
    state->needs_redraw = 1;
    return state;
}

static void splitpane_destroy_state(void* state) {
    if (!state) return;
    TuixSplitPaneState *s = (TuixSplitPaneState*)state;
    free(s->inserted_buffer);
    free(s);
}

static int splitpane_ensure_buffer(TuixSplitPaneState *state, TuixBuffer *buffer) {
    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    size_t required = sizeof(TuixPixel) * n;
    if (state->inserted_buffer && (size_t)state->inserted_buffer_size == required) {
        return 0;
    }
    if (state->inserted_buffer) {
        free(state->inserted_buffer);
        state->inserted_buffer = NULL;
        state->inserted_buffer_size = 0;
    }
    state->inserted_buffer = (TuixPixel*)calloc(n, sizeof(TuixPixel));
    if (!state->inserted_buffer) return -1;
    state->inserted_buffer_size = (int)required;
    state->needs_redraw = 1;
    buffer->required_redraw = 1;
    return 0;
}

static void splitpane_put(TuixPixel *pixel, char ch, TuixRGBTuple fg, TuixRGBTuple bg, int use_bg) {
    memset(pixel, 0, sizeof(*pixel));
    pixel->sym[0] = ch;
    pixel->sym[1] = '\0';
    pixel->styles.fg = fg;
    pixel->styles.custom_fg = 1;
    if (use_bg) {
        pixel->styles.bg = bg;
        pixel->styles.custom_bg = 1;
    }
}

static void splitpane_compute_metrics(TuixSplitPaneState *state, TuixBuffer *buffer) {
    if (!state || !buffer) return;

    int child_count = buffer->children_count;
    int total_main = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? buffer->width : buffer->height;
    int max_divider = total_main > 1 ? (total_main - 1) : 0;
    int divider = child_count >= 2 ? clamp_int(state->divider_size, 1, max_divider) : 0;
    int content_main = total_main - divider;
    if (content_main < 0) content_main = 0;

    if (child_count <= 0) {
        state->resolved_first_size = 0;
        state->resolved_second_size = 0;
        state->resolved_divider_offset = 0;
        state->resolved_divider_size = 0;
        return;
    }

    if (child_count == 1) {
        state->resolved_first_size = total_main;
        state->resolved_second_size = 0;
        state->resolved_divider_offset = total_main;
        state->resolved_divider_size = 0;
        return;
    }

    int first_size = 0;
    if (state->split_mode == TUIX_SPLIT_MODE_PIXELS) {
        first_size = state->split_pixels;
    } else {
        if (state->split_ratio < 0.0f) state->split_ratio = 0.0f;
        if (state->split_ratio > 1.0f) state->split_ratio = 1.0f;
        first_size = (int)((float)content_main * state->split_ratio + 0.5f);
    }

    int min_first = max_int(0, state->min_first);
    int min_second = max_int(0, state->min_second);
    int max_first = content_main - min_second;
    if (max_first < 0) max_first = 0;
    if (min_first > max_first) {
        min_first = max_first;
    }
    first_size = clamp_int(first_size, min_first, max_first);

    state->resolved_first_size = first_size;
    state->resolved_second_size = content_main - first_size;
    state->resolved_divider_offset = first_size;
    state->resolved_divider_size = divider;
}

static void splitpane_set_split_from_local_main(TuixObject *obj, TuixSplitPaneState *state, TuixBuffer *buffer, int local_main) {
    if (!obj || !state || !buffer) return;

    splitpane_compute_metrics(state, buffer);
    int total_main = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? buffer->width : buffer->height;
    int divider = state->resolved_divider_size;
    int content_main = total_main - divider;
    if (content_main < 0) content_main = 0;

    int desired_first = local_main - divider / 2;
    int min_first = max_int(0, state->min_first);
    int min_second = max_int(0, state->min_second);
    int max_first = content_main - min_second;
    if (max_first < 0) max_first = 0;
    if (min_first > max_first) min_first = max_first;
    desired_first = clamp_int(desired_first, min_first, max_first);

    int changed = 0;
    if (state->split_mode == TUIX_SPLIT_MODE_PIXELS) {
        if (state->split_pixels != desired_first) {
            state->split_pixels = desired_first;
            changed = 1;
        }
    } else {
        float ratio = content_main > 0 ? ((float)desired_first / (float)content_main) : 0.0f;
        if (ratio < 0.0f) ratio = 0.0f;
        if (ratio > 1.0f) ratio = 1.0f;
        if (state->split_ratio != ratio) {
            state->split_ratio = ratio;
            changed = 1;
        }
    }

    if (changed) {
        splitpane_mark_layout_changed(obj, state);
    }
}

static void splitpane_layout_children(TuixObject *obj, TuixBuffer *buffer) {
    if (!obj || !obj->state || !buffer) {
        return;
    }

    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    splitpane_compute_metrics(state, buffer);

    int cross = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? buffer->height : buffer->width;
    if (cross < 0) cross = 0;

    for (int i = 0; i < buffer->children_count; i++) {
        int uid = buffer->children_uids[i];
        if (i == 0) {
            int width = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? state->resolved_first_size : cross;
            int height = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? cross : state->resolved_first_size;
            tuix_set_buffer_layout_rect_by_uid(uid, 0, 0, width, height);
        } else if (i == 1) {
            int offset = state->resolved_divider_offset + state->resolved_divider_size;
            int width = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? state->resolved_second_size : cross;
            int height = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? cross : state->resolved_second_size;
            int left = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? offset : 0;
            int top = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? 0 : offset;
            tuix_set_buffer_layout_rect_by_uid(uid, left, top, width, height);
        } else {
            tuix_set_buffer_layout_rect_by_uid(uid, 0, 0, 0, 0);
        }
    }
}

static TuixPixel* splitpane_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixSplitPaneState *state = obj ? (TuixSplitPaneState*)obj->state : NULL;
    if (!state || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (splitpane_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

    splitpane_compute_metrics(state, buffer);

    int w = buffer->width;
    int h = buffer->height;
    size_t n = (size_t)w * (size_t)h;

    for (size_t i = 0; i < n; i++) {
        splitpane_put(&state->inserted_buffer[i], ' ', state->divider_fg, state->bg, state->use_bg);
    }

    if (buffer->children_count >= 2 && state->resolved_divider_size > 0) {
        TuixRGBTuple divider_bg = state->divider_bg;
        TuixRGBTuple divider_fg = state->divider_fg;
        char divider_char = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? '|' : '-';

        if (state->dragging) {
            divider_bg = (TuixRGBTuple){86, 102, 148};
            divider_fg = (TuixRGBTuple){255, 255, 255};
        } else if (state->hovered_divider || state->focused) {
            divider_bg = (TuixRGBTuple){72, 86, 120};
            divider_fg = (TuixRGBTuple){240, 245, 255};
        }

        if (state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL) {
            int x0 = state->resolved_divider_offset;
            int x1 = x0 + state->resolved_divider_size;
            int center = x0 + state->resolved_divider_size / 2;
            for (int y = 0; y < h; y++) {
                for (int x = x0; x < x1 && x < w; x++) {
                    char ch = (x == center) ? divider_char : ' ';
                    splitpane_put(&state->inserted_buffer[(size_t)y * (size_t)w + (size_t)x], ch, divider_fg, divider_bg, 1);
                }
            }
        } else {
            int y0 = state->resolved_divider_offset;
            int y1 = y0 + state->resolved_divider_size;
            int center = y0 + state->resolved_divider_size / 2;
            for (int y = y0; y < y1 && y < h; y++) {
                for (int x = 0; x < w; x++) {
                    char ch = (y == center) ? divider_char : ' ';
                    splitpane_put(&state->inserted_buffer[(size_t)y * (size_t)w + (size_t)x], ch, divider_fg, divider_bg, 1);
                }
            }
        }
    }

    return state->inserted_buffer;
}

static TuixHandlerResponse splitpane_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    TuixHandlerResponse ans = {.requires_redraw = 0};
    if (!obj || !obj->state) return ans;

    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    if (state->focused != (is_focused ? 1 : 0)) {
        state->focused = is_focused ? 1 : 0;
        state->needs_redraw = 1;
    }

    TuixBuffer buffer;
    if (tuix_get_buffer_snapshot_by_uid(obj->uid, &buffer) != 0) {
        return ans;
    }
    splitpane_compute_metrics(state, &buffer);

    if (is_focused && has_event && snap && !snap->consumed_keyboard && snap->keyboard && snap->keyboard->has_event) {
        int code = snap->keyboard->code;
        int delta = 0;
        if (state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL) {
            if (code == TUIX_VK_LEFT) delta = -1;
            else if (code == TUIX_VK_RIGHT) delta = 1;
        } else {
            if (code == TUIX_VK_UP) delta = -1;
            else if (code == TUIX_VK_DOWN) delta = 1;
        }
        if (delta != 0) {
            int local_main = state->resolved_divider_offset + delta + state->resolved_divider_size / 2;
            splitpane_set_split_from_local_main(obj, state, &buffer, local_main);
            state->needs_redraw = 1;
            snap->consumed_keyboard = true;
        }
    }

    if (snap && snap->mouse) {
        int local_x = snap->mouse->col - 1 - buffer.margin_left;
        int local_y = snap->mouse->row - 1 - buffer.margin_top;
        int inside = local_x >= 0 && local_y >= 0 && local_x < buffer.width && local_y < buffer.height;
        int divider_hit = 0;
        if (inside && buffer.children_count >= 2 && state->resolved_divider_size > 0) {
            if (state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL) {
                divider_hit = local_x >= state->resolved_divider_offset &&
                              local_x < state->resolved_divider_offset + state->resolved_divider_size;
            } else {
                divider_hit = local_y >= state->resolved_divider_offset &&
                              local_y < state->resolved_divider_offset + state->resolved_divider_size;
            }
        }

        if (divider_hit != state->hovered_divider) {
            state->hovered_divider = divider_hit;
            state->needs_redraw = 1;
        }

        if (has_event && !snap->consumed_mouse && snap->mouse->has_event) {
            if (snap->mouse->event == TUIX_MOUSE_PRESS && snap->mouse->btn == TUIX_BTN_LEFT && divider_hit) {
                state->dragging = 1;
                tuix_mouse_capture_begin(obj->uid);
                state->needs_redraw = 1;
                snap->consumed_mouse = true;
            } else if (snap->mouse->event == TUIX_MOUSE_DRAG && state->dragging) {
                int local_main = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? local_x : local_y;
                splitpane_set_split_from_local_main(obj, state, &buffer, local_main);
                state->needs_redraw = 1;
                snap->consumed_mouse = true;
            } else if (snap->mouse->event == TUIX_MOUSE_RELEASE && snap->mouse->btn == TUIX_BTN_LEFT && state->dragging) {
                int local_main = state->orientation == TUIX_LAYOUT_AXIS_HORIZONTAL ? local_x : local_y;
                splitpane_set_split_from_local_main(obj, state, &buffer, local_main);
                state->dragging = 0;
                tuix_mouse_capture_end(obj->uid);
                state->needs_redraw = 1;
                snap->consumed_mouse = true;
            }
        }
    }

    if (state->needs_redraw) {
        state->needs_redraw = 0;
        ans.requires_redraw = 1;
    }
    return ans;
}

static void splitpane_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    if (splitpane_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

static const TuixBuilder tuix_splitpane_builder = {
    .name = "SplitPaneBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = splitpane_create_state,
    .destroy_state = splitpane_destroy_state,
    .on_event = splitpane_handler,
    .on_resize = splitpane_on_resize,
    .layout_children = splitpane_layout_children,
    .build_content = splitpane_build_content
};

const TuixBuilder* tuix_splitpane_init(void) {
    return &tuix_splitpane_builder;
}

int tuix_splitpane_attach_child(const char* scene_name, TuixObject *split_obj, int child_uid) {
    if (!scene_name || !split_obj || child_uid <= 0) return -1;
    return tuix_set_buffer_parent((char*)scene_name, child_uid, split_obj->uid);
}

int tuix_splitpane_detach_child(const char* scene_name, TuixObject *split_obj, int child_uid) {
    (void)split_obj;
    if (!scene_name || child_uid <= 0) return -1;
    return tuix_set_buffer_parent((char*)scene_name, child_uid, -1);
}

int tuix_splitpane_add_object(const char* scene_name, TuixObject *split_obj, const char* builder_name, float width_mod, float height_mod) {
    if (!scene_name || !split_obj || !builder_name) return -1;
    int child_uid = tuix_create_object_ex((char*)builder_name, (char*)scene_name, width_mod, height_mod, 0.0f, 0.0f, 0);
    if (child_uid <= 0) {
        return -1;
    }
    if (tuix_set_buffer_parent((char*)scene_name, child_uid, split_obj->uid) != 0) {
        tuix_free_buffer((char*)scene_name, child_uid);
        return -1;
    }
    return child_uid;
}

int tuix_splitpane_set_orientation(TuixObject *obj, int orientation) {
    if (!obj || !obj->state) return -1;
    if (orientation != TUIX_LAYOUT_AXIS_HORIZONTAL && orientation != TUIX_LAYOUT_AXIS_VERTICAL) {
        return -1;
    }
    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    if (state->orientation == orientation) return 0;
    state->orientation = orientation;
    splitpane_mark_layout_changed(obj, state);
    return 0;
}

int tuix_splitpane_set_split_ratio(TuixObject *obj, float ratio) {
    if (!obj || !obj->state) return -1;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    if (state->split_mode == TUIX_SPLIT_MODE_RATIO && state->split_ratio == ratio) return 0;
    state->split_mode = TUIX_SPLIT_MODE_RATIO;
    state->split_ratio = ratio;
    splitpane_mark_layout_changed(obj, state);
    return 0;
}

int tuix_splitpane_set_split_pixels(TuixObject *obj, int pixels) {
    if (!obj || !obj->state) return -1;
    if (pixels < 0) pixels = 0;
    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    if (state->split_mode == TUIX_SPLIT_MODE_PIXELS && state->split_pixels == pixels) return 0;
    state->split_mode = TUIX_SPLIT_MODE_PIXELS;
    state->split_pixels = pixels;
    splitpane_mark_layout_changed(obj, state);
    return 0;
}

int tuix_splitpane_set_divider_size(TuixObject *obj, int divider_size) {
    if (!obj || !obj->state) return -1;
    if (divider_size < 1) divider_size = 1;
    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    if (state->divider_size == divider_size) return 0;
    state->divider_size = divider_size;
    splitpane_mark_layout_changed(obj, state);
    return 0;
}

int tuix_splitpane_set_min_sizes(TuixObject *obj, int min_first, int min_second) {
    if (!obj || !obj->state) return -1;
    if (min_first < 0) min_first = 0;
    if (min_second < 0) min_second = 0;
    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    if (state->min_first == min_first && state->min_second == min_second) return 0;
    state->min_first = min_first;
    state->min_second = min_second;
    splitpane_mark_layout_changed(obj, state);
    return 0;
}

int tuix_splitpane_set_colors(TuixObject *obj,
                              uint8_t divider_fg_r, uint8_t divider_fg_g, uint8_t divider_fg_b,
                              uint8_t divider_bg_r, uint8_t divider_bg_g, uint8_t divider_bg_b,
                              uint8_t bg_r, uint8_t bg_g, uint8_t bg_b) {
    if (!obj || !obj->state) return -1;
    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    state->divider_fg = (TuixRGBTuple){divider_fg_r, divider_fg_g, divider_fg_b};
    state->divider_bg = (TuixRGBTuple){divider_bg_r, divider_bg_g, divider_bg_b};
    state->bg = (TuixRGBTuple){bg_r, bg_g, bg_b};
    state->use_bg = 1;
    state->needs_redraw = 1;
    return 0;
}

int tuix_splitpane_clear_bg(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixSplitPaneState *state = (TuixSplitPaneState*)obj->state;
    state->use_bg = 0;
    state->needs_redraw = 1;
    return 0;
}
