#include "button_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TUIX_VK_ENTER 0x0D

typedef struct {
    char *label;
    int pressed;
    int focused;
    int hovered;
    int mouse_armed;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixButtonState;

static void* button_create_state(void* params) {
    (void)params;
    TuixButtonState *s = calloc(1, sizeof(TuixButtonState));
    if (!s) return NULL;
    s->label = strdup("Button");
    s->pressed = 0;
    s->focused = 0;
    s->hovered = 0;
    s->mouse_armed = 0;
    s->needs_redraw = 1;
    return s;
}

static void button_destroy_state(void* state) {
    if (!state) return;
    TuixButtonState *s = (TuixButtonState*)state;
    free(s->label);
    free(s->inserted_buffer);
    free(s);
}

static int button_ensure_buffer(TuixButtonState *s, TuixBuffer *buffer) {
    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    size_t required = sizeof(TuixPixel) * n;
    if (s->inserted_buffer && (size_t)s->inserted_buffer_size == required) return 0;
    if (s->inserted_buffer) {
        free(s->inserted_buffer);
        s->inserted_buffer = NULL;
        s->inserted_buffer_size = 0;
    }
    s->inserted_buffer = calloc(n, sizeof(TuixPixel));
    if (!s->inserted_buffer) return -1;
    s->inserted_buffer_size = (int)required;
    s->needs_redraw = 1;
    buffer->required_redraw = 1;
    return 0;
}

static void button_put(TuixPixel *p, char ch, TuixRGBTuple fg, TuixRGBTuple bg) {
    memset(p, 0, sizeof(*p));
    p->sym[0] = ch;
    p->sym[1] = '\0';
    p->styles.fg = fg;
    p->styles.bg = bg;
    p->styles.custom_fg = 1;
    p->styles.custom_bg = 1;
}

static TuixPixel* button_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixButtonState *s = obj ? (TuixButtonState*)obj->state : NULL;
    if (!s || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (button_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }

    int w = buffer->width;
    int h = buffer->height;

    TuixRGBTuple fg = s->focused ? (TuixRGBTuple){255, 255, 255} : (TuixRGBTuple){220, 220, 220};
    TuixRGBTuple bg = s->focused ? (TuixRGBTuple){70, 95, 180} : (TuixRGBTuple){60, 60, 70};
    if (s->hovered) {
        bg = s->focused ? (TuixRGBTuple){84, 110, 205} : (TuixRGBTuple){72, 72, 86};
    }
    if (s->mouse_armed) {
        bg = (TuixRGBTuple){52, 76, 150};
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            button_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)x], ' ', fg, bg);
        }
    }

    if (w >= 2 && h >= 2) {
        for (int x = 0; x < w; x++) {
            button_put(&s->inserted_buffer[(size_t)x], '-', fg, bg);
            button_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)x], '-', fg, bg);
        }
        for (int y = 0; y < h; y++) {
            button_put(&s->inserted_buffer[(size_t)y * (size_t)w], '|', fg, bg);
            button_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)(w - 1)], '|', fg, bg);
        }
        button_put(&s->inserted_buffer[0], '+', fg, bg);
        button_put(&s->inserted_buffer[(size_t)(w - 1)], '+', fg, bg);
        button_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w], '+', fg, bg);
        button_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)(w - 1)], '+', fg, bg);
    }

    const char *label = s->label ? s->label : "";
    int len = (int)strlen(label);
    int y = h / 2;
    int x = (w - len) / 2;
    if (x < 1) x = 1;

    for (int i = 0; i < len && (x + i) < (w - 1); i++) {
        TuixPixel *p = &s->inserted_buffer[(size_t)y * (size_t)w + (size_t)(x + i)];
        p->sym[0] = label[i];
        p->sym[1] = '\0';
    }

    return s->inserted_buffer;
}

static TuixHandlerResponse button_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    TuixHandlerResponse ans = {.requires_redraw = 0};
    if (!obj || !obj->state) return ans;

    TuixButtonState *s = (TuixButtonState*)obj->state;
    if (s->focused != (is_focused ? 1 : 0)) {
        s->focused = is_focused ? 1 : 0;
        s->needs_redraw = 1;
    }

    if (snap && snap->mouse) {
        TuixBuffer b;
        if (tuix_get_buffer_snapshot_by_uid(obj->uid, &b) == 0) {
            int mx = snap->mouse->col - 1;
            int my = snap->mouse->row - 1;
            int inside = (mx >= b.margin_left && mx < b.margin_left + b.width &&
                          my >= b.margin_top && my < b.margin_top + b.height);
            int hovered_now = inside ? 1 : 0;
            if (hovered_now != s->hovered) {
                s->hovered = hovered_now;
                s->needs_redraw = 1;
            }
        }
    }

    if (is_focused && has_event && snap) {
        if (snap->keyboard && snap->keyboard->has_event) {
            int code = snap->keyboard->code;
            if (code == TUIX_VK_ENTER || code == '\n' || code == ' ') {
                s->pressed = 1;
                s->needs_redraw = 1;
                snap->consumed_keyboard = true;
            }
        }
    }

    if (has_event && snap) {
        if (snap->mouse && snap->mouse->has_event &&
            (snap->mouse->event == TUIX_MOUSE_PRESS || snap->mouse->event == TUIX_MOUSE_RELEASE) &&
            snap->mouse->btn == TUIX_BTN_LEFT) {
            TuixBuffer b;
            if (tuix_get_buffer_snapshot_by_uid(obj->uid, &b) == 0) {
                int mx = snap->mouse->col - 1;
                int my = snap->mouse->row - 1;
                int inside = (mx >= b.margin_left && mx < b.margin_left + b.width &&
                              my >= b.margin_top && my < b.margin_top + b.height);

                if (snap->mouse->event == TUIX_MOUSE_PRESS && inside) {
                    s->mouse_armed = 1;
                    s->needs_redraw = 1;
                    snap->consumed_mouse = true;
                } else if (snap->mouse->event == TUIX_MOUSE_RELEASE) {
                    if (s->mouse_armed && inside) {
                        s->pressed = 1;
                    }
                    if (s->mouse_armed) {
                        s->mouse_armed = 0;
                    }
                    s->needs_redraw = 1;
                    snap->consumed_mouse = true;
                }
            }
        }
    }

    if (s->needs_redraw) {
        s->needs_redraw = 0;
        ans.requires_redraw = 1;
    }
    return ans;
}

static void button_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixButtonState *s = (TuixButtonState*)obj->state;
    if (button_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }
}

const TuixBuilder tuix_button_builder = {
    .name = "ButtonBuilder",
    .version = "1.1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = button_create_state,
    .destroy_state = button_destroy_state,
    .on_event = button_handler,
    .on_resize = button_on_resize,
    .build_content = button_build_content
};

const TuixBuilder* tuix_button_init(void) {
    return &tuix_button_builder;
}

int tuix_button_set_label(TuixObject *obj, const char *label) {
    if (!obj || !obj->state) return -1;
    TuixButtonState *s = (TuixButtonState*)obj->state;
    char *copy = strdup(label ? label : "");
    if (!copy) return -1;
    free(s->label);
    s->label = copy;
    s->needs_redraw = 1;
    return 0;
}

int tuix_button_take_pressed(TuixObject *obj) {
    if (!obj || !obj->state) return 0;
    TuixButtonState *s = (TuixButtonState*)obj->state;
    int v = s->pressed;
    s->pressed = 0;
    return v;
}

void tuix_button_reset(TuixObject *obj) {
    if (!obj || !obj->state) return;
    TuixButtonState *s = (TuixButtonState*)obj->state;
    s->pressed = 0;
    s->needs_redraw = 1;
}
