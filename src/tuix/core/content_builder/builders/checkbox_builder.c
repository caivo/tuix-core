#include "checkbox_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TUIX_VK_ENTER 0x0D

typedef struct {
    char *label;
    int checked;
    int changed;
    int disabled;
    int focused;
    int hovered;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixCheckboxState;

static void* checkbox_create_state(void* params) {
    (void)params;
    TuixCheckboxState *state = (TuixCheckboxState*)calloc(1, sizeof(TuixCheckboxState));
    if (!state) return NULL;
    state->label = strdup("Checkbox");
    if (!state->label) {
        free(state);
        return NULL;
    }
    state->needs_redraw = 1;
    return state;
}

static void checkbox_destroy_state(void* state_ptr) {
    if (!state_ptr) return;
    TuixCheckboxState *state = (TuixCheckboxState*)state_ptr;
    free(state->label);
    free(state->inserted_buffer);
    free(state);
}

static int checkbox_ensure_buffer(TuixCheckboxState *state, TuixBuffer *buffer) {
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

static void checkbox_put(TuixPixel *pixel, char ch, TuixRGBTuple fg, TuixRGBTuple bg) {
    memset(pixel, 0, sizeof(*pixel));
    pixel->sym[0] = ch;
    pixel->sym[1] = '\0';
    pixel->styles.fg = fg;
    pixel->styles.bg = bg;
    pixel->styles.custom_fg = 1;
    pixel->styles.custom_bg = 1;
}

static void checkbox_toggle_internal(TuixCheckboxState *state) {
    if (!state || state->disabled) return;
    state->checked = state->checked ? 0 : 1;
    state->changed = 1;
    state->needs_redraw = 1;
}

static TuixPixel* checkbox_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixCheckboxState *state = obj ? (TuixCheckboxState*)obj->state : NULL;
    if (!state || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (checkbox_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

    int w = buffer->width;
    int h = buffer->height;
    size_t n = (size_t)w * (size_t)h;

    TuixRGBTuple fg = state->disabled ? (TuixRGBTuple){120, 126, 138} : (TuixRGBTuple){222, 228, 236};
    TuixRGBTuple bg = (TuixRGBTuple){22, 26, 36};
    if (state->hovered && !state->disabled) {
        bg = (TuixRGBTuple){32, 38, 52};
    }
    if (state->focused && !state->disabled) {
        bg = (TuixRGBTuple){46, 64, 108};
    }

    for (size_t i = 0; i < n; i++) {
        checkbox_put(&state->inserted_buffer[i], ' ', fg, bg);
    }

    int row = h / 2;
    if (row < 0 || row >= h) {
        return state->inserted_buffer;
    }

    const char *mark = state->checked ? "[x] " : "[ ] ";
    int x = 1;
    for (int i = 0; mark[i] && x < w; i++, x++) {
        TuixPixel *pixel = &state->inserted_buffer[(size_t)row * (size_t)w + (size_t)x];
        pixel->sym[0] = mark[i];
        pixel->sym[1] = '\0';
    }

    const char *label = state->label ? state->label : "";
    for (int i = 0; label[i] && x < w; i++, x++) {
        TuixPixel *pixel = &state->inserted_buffer[(size_t)row * (size_t)w + (size_t)x];
        pixel->sym[0] = label[i];
        pixel->sym[1] = '\0';
    }

    return state->inserted_buffer;
}

static TuixHandlerResponse checkbox_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    TuixHandlerResponse ans = {.requires_redraw = 0};
    if (!obj || !obj->state) return ans;

    TuixCheckboxState *state = (TuixCheckboxState*)obj->state;
    if (state->focused != (is_focused ? 1 : 0)) {
        state->focused = is_focused ? 1 : 0;
        state->needs_redraw = 1;
    }

    if (snap && snap->mouse) {
        TuixBuffer buffer;
        if (tuix_get_buffer_snapshot_by_uid(obj->uid, &buffer) == 0) {
            int mx = snap->mouse->col - 1;
            int my = snap->mouse->row - 1;
            int inside = (mx >= buffer.margin_left && mx < buffer.margin_left + buffer.width &&
                          my >= buffer.margin_top && my < buffer.margin_top + buffer.height);
            int hovered_now = inside ? 1 : 0;
            if (hovered_now != state->hovered) {
                state->hovered = hovered_now;
                state->needs_redraw = 1;
            }

            if (!state->disabled && has_event && !snap->consumed_mouse && snap->mouse->has_event &&
                snap->mouse->event == TUIX_MOUSE_RELEASE && snap->mouse->btn == TUIX_BTN_LEFT && inside) {
                checkbox_toggle_internal(state);
                snap->consumed_mouse = true;
            }
        }
    }

    if (!state->disabled && is_focused && has_event && snap && !snap->consumed_keyboard &&
        snap->keyboard && snap->keyboard->has_event) {
        int code = snap->keyboard->code;
        if (code == TUIX_VK_ENTER || code == '\n' || code == ' ') {
            checkbox_toggle_internal(state);
            snap->consumed_keyboard = true;
        }
    }

    if (state->needs_redraw) {
        state->needs_redraw = 0;
        ans.requires_redraw = 1;
    }
    return ans;
}

static void checkbox_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixCheckboxState *state = (TuixCheckboxState*)obj->state;
    if (checkbox_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

const TuixBuilder tuix_checkbox_builder = {
    .name = "CheckboxBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = checkbox_create_state,
    .destroy_state = checkbox_destroy_state,
    .on_event = checkbox_handler,
    .on_resize = checkbox_on_resize,
    .build_content = checkbox_build_content
};

const TuixBuilder* tuix_checkbox_init(void) {
    return &tuix_checkbox_builder;
}

int tuix_checkbox_set_label(TuixObject *obj, const char *label) {
    if (!obj || !obj->state) return -1;
    TuixCheckboxState *state = (TuixCheckboxState*)obj->state;
    char *copy = strdup(label ? label : "");
    if (!copy) return -1;
    free(state->label);
    state->label = copy;
    state->needs_redraw = 1;
    return 0;
}

int tuix_checkbox_set_checked(TuixObject *obj, int checked) {
    if (!obj || !obj->state) return -1;
    TuixCheckboxState *state = (TuixCheckboxState*)obj->state;
    checked = checked ? 1 : 0;
    if (state->checked == checked) return 0;
    state->checked = checked;
    state->needs_redraw = 1;
    return 0;
}

int tuix_checkbox_get_checked(TuixObject *obj) {
    if (!obj || !obj->state) return 0;
    TuixCheckboxState *state = (TuixCheckboxState*)obj->state;
    return state->checked;
}

int tuix_checkbox_toggle(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixCheckboxState *state = (TuixCheckboxState*)obj->state;
    checkbox_toggle_internal(state);
    return 0;
}

int tuix_checkbox_take_changed(TuixObject *obj) {
    if (!obj || !obj->state) return 0;
    TuixCheckboxState *state = (TuixCheckboxState*)obj->state;
    int changed = state->changed;
    state->changed = 0;
    return changed;
}

int tuix_checkbox_set_disabled(TuixObject *obj, int disabled) {
    if (!obj || !obj->state) return -1;
    TuixCheckboxState *state = (TuixCheckboxState*)obj->state;
    disabled = disabled ? 1 : 0;
    if (state->disabled == disabled) return 0;
    state->disabled = disabled;
    state->needs_redraw = 1;
    return 0;
}
