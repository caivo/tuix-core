#include "dialog_builder.h"
#include "../../buffer_manager.h"
#include "../../buffers.h"
#include "../../object_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TUIX_VK_ESCAPE 0x1B

typedef struct {
    char *title;
    int body_width;
    int body_height;
    int padding_left;
    int padding_top;
    int padding_right;
    int padding_bottom;
    int close_on_esc;
    int close_on_backdrop;
    int close_requested;
    int needs_redraw;
    TuixRGBTuple backdrop_bg;
    TuixRGBTuple border_fg;
    TuixRGBTuple body_bg;
    TuixRGBTuple title_fg;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixDialogState;

static void dialog_mark_redraw(TuixDialogState *state) {
    if (!state) return;
    state->needs_redraw = 1;
}

static void dialog_put(TuixPixel *pixel, char ch, TuixRGBTuple fg, TuixRGBTuple bg) {
    memset(pixel, 0, sizeof(*pixel));
    pixel->sym[0] = ch;
    pixel->sym[1] = '\0';
    pixel->styles.fg = fg;
    pixel->styles.bg = bg;
    pixel->styles.custom_fg = 1;
    pixel->styles.custom_bg = 1;
}

static int dialog_ensure_buffer(TuixDialogState *state, TuixBuffer *buffer) {
    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    size_t required = sizeof(TuixPixel) * n;
    if (state->inserted_buffer && (size_t)state->inserted_buffer_size == required) {
        return 0;
    }
    free(state->inserted_buffer);
    state->inserted_buffer = NULL;
    state->inserted_buffer_size = 0;
    state->inserted_buffer = (TuixPixel*)calloc(n, sizeof(TuixPixel));
    if (!state->inserted_buffer) return -1;
    state->inserted_buffer_size = (int)required;
    state->needs_redraw = 1;
    buffer->required_redraw = 1;
    return 0;
}

static void dialog_compute_body_rect(const TuixDialogState *state, TuixBuffer *buffer,
                                     int *out_left, int *out_top, int *out_width, int *out_height) {
    int width = state && state->body_width > 0 ? state->body_width : 56;
    int height = state && state->body_height > 0 ? state->body_height : 16;
    int max_width = buffer->width - 2;
    int max_height = buffer->height - 2;
    if (max_width < 8) max_width = buffer->width;
    if (max_height < 6) max_height = buffer->height;
    if (width > max_width) width = max_width;
    if (height > max_height) height = max_height;
    if (width < 8) width = buffer->width > 0 ? buffer->width : 1;
    if (height < 6) height = buffer->height > 0 ? buffer->height : 1;
    if (width > buffer->width) width = buffer->width;
    if (height > buffer->height) height = buffer->height;
    int left = (buffer->width - width) / 2;
    int top = (buffer->height - height) / 2;
    if (left < 0) left = 0;
    if (top < 0) top = 0;
    if (out_left) *out_left = left;
    if (out_top) *out_top = top;
    if (out_width) *out_width = width;
    if (out_height) *out_height = height;
}

static void* dialog_create_state(void* params) {
    (void)params;
    TuixDialogState *state = (TuixDialogState*)calloc(1, sizeof(TuixDialogState));
    if (!state) return NULL;
    state->title = strdup("Dialog");
    if (!state->title) {
        free(state);
        return NULL;
    }
    state->body_width = 56;
    state->body_height = 16;
    state->padding_left = 1;
    state->padding_top = 1;
    state->padding_right = 1;
    state->padding_bottom = 1;
    state->close_on_esc = 1;
    state->close_on_backdrop = 0;
    state->backdrop_bg = (TuixRGBTuple){8, 12, 18};
    state->border_fg = (TuixRGBTuple){220, 228, 244};
    state->body_bg = (TuixRGBTuple){24, 30, 44};
    state->title_fg = (TuixRGBTuple){240, 244, 252};
    state->needs_redraw = 1;
    return state;
}

static void dialog_destroy_state(void* state_ptr) {
    if (!state_ptr) return;
    TuixDialogState *state = (TuixDialogState*)state_ptr;
    free(state->title);
    free(state->inserted_buffer);
    free(state);
}

static void dialog_layout_children(TuixObject *obj, TuixBuffer *buffer) {
    if (!obj || !obj->state || !buffer) return;
    TuixDialogState *state = (TuixDialogState*)obj->state;
    int body_left = 0, body_top = 0, body_width = 0, body_height = 0;
    dialog_compute_body_rect(state, buffer, &body_left, &body_top, &body_width, &body_height);

    int content_left = body_left + 1 + state->padding_left;
    int content_top = body_top + 1 + state->padding_top;
    int content_width = body_width - 2 - state->padding_left - state->padding_right;
    int content_height = body_height - 2 - state->padding_top - state->padding_bottom;
    if (content_width < 1) content_width = 1;
    if (content_height < 1) content_height = 1;

    for (int i = 0; i < buffer->children_count; i++) {
        int child_uid = buffer->children_uids[i];
        tuix_set_buffer_layout_rect_by_uid(child_uid, content_left, content_top, content_width, content_height);
    }
}

static TuixPixel* dialog_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixDialogState *state = obj ? (TuixDialogState*)obj->state : NULL;
    if (!state || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (dialog_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

    int w = buffer->width;
    int h = buffer->height;
    int body_left = 0, body_top = 0, body_width = 0, body_height = 0;
    dialog_compute_body_rect(state, buffer, &body_left, &body_top, &body_width, &body_height);

    size_t total = (size_t)w * (size_t)h;
    for (size_t i = 0; i < total; i++) {
        dialog_put(&state->inserted_buffer[i], ' ', state->border_fg, state->backdrop_bg);
    }

    for (int y = 0; y < body_height; y++) {
        int py = body_top + y;
        if (py < 0 || py >= h) continue;
        for (int x = 0; x < body_width; x++) {
            int px = body_left + x;
            if (px < 0 || px >= w) continue;
            dialog_put(&state->inserted_buffer[(size_t)py * (size_t)w + (size_t)px], ' ', state->border_fg, state->body_bg);
        }
    }

    if (body_width >= 2 && body_height >= 2) {
        for (int x = 0; x < body_width; x++) {
            dialog_put(&state->inserted_buffer[(size_t)body_top * (size_t)w + (size_t)(body_left + x)], '-', state->border_fg, state->body_bg);
            dialog_put(&state->inserted_buffer[(size_t)(body_top + body_height - 1) * (size_t)w + (size_t)(body_left + x)], '-', state->border_fg, state->body_bg);
        }
        for (int y = 0; y < body_height; y++) {
            dialog_put(&state->inserted_buffer[(size_t)(body_top + y) * (size_t)w + (size_t)body_left], '|', state->border_fg, state->body_bg);
            dialog_put(&state->inserted_buffer[(size_t)(body_top + y) * (size_t)w + (size_t)(body_left + body_width - 1)], '|', state->border_fg, state->body_bg);
        }
        dialog_put(&state->inserted_buffer[(size_t)body_top * (size_t)w + (size_t)body_left], '+', state->border_fg, state->body_bg);
        dialog_put(&state->inserted_buffer[(size_t)body_top * (size_t)w + (size_t)(body_left + body_width - 1)], '+', state->border_fg, state->body_bg);
        dialog_put(&state->inserted_buffer[(size_t)(body_top + body_height - 1) * (size_t)w + (size_t)body_left], '+', state->border_fg, state->body_bg);
        dialog_put(&state->inserted_buffer[(size_t)(body_top + body_height - 1) * (size_t)w + (size_t)(body_left + body_width - 1)], '+', state->border_fg, state->body_bg);
    }

    if (state->title && state->title[0] && body_width > 4) {
        int title_x = body_left + 2;
        int title_y = body_top;
        for (int i = 0; state->title[i] && title_x + i < body_left + body_width - 2; i++) {
            TuixPixel *pixel = &state->inserted_buffer[(size_t)title_y * (size_t)w + (size_t)(title_x + i)];
            pixel->sym[0] = state->title[i];
            pixel->sym[1] = '\0';
            pixel->styles.fg = state->title_fg;
            pixel->styles.bg = state->body_bg;
        }
    }

    return state->inserted_buffer;
}

static TuixHandlerResponse dialog_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)is_focused;
    TuixHandlerResponse ans = {.requires_redraw = 0};
    if (!obj || !obj->state) return ans;
    TuixDialogState *state = (TuixDialogState*)obj->state;

    if (has_event && snap && !snap->consumed_mouse && snap->mouse && snap->mouse->has_event &&
        snap->mouse->btn == TUIX_BTN_LEFT && snap->mouse->event == TUIX_MOUSE_PRESS) {
        TuixBuffer buffer;
        if (tuix_get_buffer_snapshot_by_uid(obj->uid, &buffer) == 0) {
            int body_left = 0, body_top = 0, body_width = 0, body_height = 0;
            dialog_compute_body_rect(state, &buffer, &body_left, &body_top, &body_width, &body_height);
            int local_x = snap->mouse->col - 1 - buffer.margin_left;
            int local_y = snap->mouse->row - 1 - buffer.margin_top;
            int inside_body = local_x >= body_left && local_x < body_left + body_width &&
                              local_y >= body_top && local_y < body_top + body_height;
            if (!inside_body) {
                if (state->close_on_backdrop) {
                    state->close_requested = 1;
                }
                snap->consumed_mouse = true;
            }
        }
    }

    if (has_event && snap && !snap->consumed_keyboard && snap->keyboard && snap->keyboard->has_event) {
        if (state->close_on_esc && snap->keyboard->code == TUIX_VK_ESCAPE) {
            state->close_requested = 1;
            snap->consumed_keyboard = true;
        }
    }

    if (state->needs_redraw) {
        state->needs_redraw = 0;
        ans.requires_redraw = 1;
    }
    return ans;
}

static void dialog_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixDialogState *state = (TuixDialogState*)obj->state;
    if (dialog_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

const TuixBuilder tuix_dialog_builder = {
    .name = "DialogBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = dialog_create_state,
    .destroy_state = dialog_destroy_state,
    .on_event = dialog_handler,
    .on_resize = dialog_on_resize,
    .layout_children = dialog_layout_children,
    .build_content = dialog_build_content
};

const TuixBuilder* tuix_dialog_init(void) {
    return &tuix_dialog_builder;
}

int tuix_dialog_attach_child(const char* scene_name, TuixObject *dialog_obj, int child_uid) {
    if (!scene_name || !dialog_obj || child_uid <= 0) return -1;
    if (tuix_set_buffer_parent((char*)scene_name, child_uid, dialog_obj->uid) != 0) return -1;
    tuix_mark_buffer_children_geometry_dirty_by_uid(dialog_obj->uid);
    return 0;
}

int tuix_dialog_detach_child(const char* scene_name, TuixObject *dialog_obj, int child_uid) {
    (void)dialog_obj;
    if (!scene_name || child_uid <= 0) return -1;
    return tuix_set_buffer_parent((char*)scene_name, child_uid, -1);
}

int tuix_dialog_add_object(const char* scene_name, TuixObject *dialog_obj, const char* builder_name, float width_mod, float height_mod) {
    if (!scene_name || !dialog_obj || !builder_name) return -1;
    int uid = tuix_create_object_ex((char*)builder_name, (char*)scene_name, width_mod, height_mod, 0.0f, 0.0f, 0);
    if (uid <= 0) return -1;
    if (tuix_dialog_attach_child(scene_name, dialog_obj, uid) != 0) {
        tuix_free_buffer((char*)scene_name, uid);
        return -1;
    }
    return uid;
}

int tuix_dialog_set_title(TuixObject *obj, const char *title) {
    if (!obj || !obj->state) return -1;
    TuixDialogState *state = (TuixDialogState*)obj->state;
    char *copy = strdup(title ? title : "");
    if (!copy) return -1;
    free(state->title);
    state->title = copy;
    dialog_mark_redraw(state);
    return 0;
}

int tuix_dialog_set_body_size(TuixObject *obj, int width, int height) {
    if (!obj || !obj->state) return -1;
    TuixDialogState *state = (TuixDialogState*)obj->state;
    if (width < 8) width = 8;
    if (height < 6) height = 6;
    state->body_width = width;
    state->body_height = height;
    tuix_mark_buffer_children_geometry_dirty_by_uid(obj->uid);
    dialog_mark_redraw(state);
    return 0;
}

int tuix_dialog_set_padding(TuixObject *obj, int left, int top, int right, int bottom) {
    if (!obj || !obj->state) return -1;
    TuixDialogState *state = (TuixDialogState*)obj->state;
    state->padding_left = left < 0 ? 0 : left;
    state->padding_top = top < 0 ? 0 : top;
    state->padding_right = right < 0 ? 0 : right;
    state->padding_bottom = bottom < 0 ? 0 : bottom;
    tuix_mark_buffer_children_geometry_dirty_by_uid(obj->uid);
    dialog_mark_redraw(state);
    return 0;
}

int tuix_dialog_set_close_on_esc(TuixObject *obj, int enabled) {
    if (!obj || !obj->state) return -1;
    TuixDialogState *state = (TuixDialogState*)obj->state;
    state->close_on_esc = enabled ? 1 : 0;
    dialog_mark_redraw(state);
    return 0;
}

int tuix_dialog_set_close_on_backdrop(TuixObject *obj, int enabled) {
    if (!obj || !obj->state) return -1;
    TuixDialogState *state = (TuixDialogState*)obj->state;
    state->close_on_backdrop = enabled ? 1 : 0;
    dialog_mark_redraw(state);
    return 0;
}

int tuix_dialog_set_colors(TuixObject *obj,
                           uint8_t backdrop_r, uint8_t backdrop_g, uint8_t backdrop_b,
                           uint8_t border_r, uint8_t border_g, uint8_t border_b,
                           uint8_t body_r, uint8_t body_g, uint8_t body_b) {
    if (!obj || !obj->state) return -1;
    TuixDialogState *state = (TuixDialogState*)obj->state;
    state->backdrop_bg = (TuixRGBTuple){backdrop_r, backdrop_g, backdrop_b};
    state->border_fg = (TuixRGBTuple){border_r, border_g, border_b};
    state->title_fg = state->border_fg;
    state->body_bg = (TuixRGBTuple){body_r, body_g, body_b};
    dialog_mark_redraw(state);
    return 0;
}

int tuix_dialog_activate(const char* scene_name, TuixObject *obj) {
    if (!scene_name || !obj) return -1;
    if (tuix_set_buffer_z_index((char*)scene_name, obj->uid, 1000000) != 0) return -1;
    return tuix_scene_activate_modal(scene_name, obj->uid);
}

int tuix_dialog_deactivate(const char* scene_name, TuixObject *obj) {
    if (!scene_name || !obj) return -1;
    return tuix_scene_deactivate_modal(scene_name, obj->uid);
}

int tuix_dialog_take_close_requested(TuixObject *obj) {
    if (!obj || !obj->state) return 0;
    TuixDialogState *state = (TuixDialogState*)obj->state;
    int value = state->close_requested;
    state->close_requested = 0;
    return value;
}
