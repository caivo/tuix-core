#include "box_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *title;
    TuixRGBTuple border;
    TuixRGBTuple bg;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixBoxState;

static void* box_create_state(void* params) {
    (void)params;
    TuixBoxState *s = calloc(1, sizeof(TuixBoxState));
    if (!s) return NULL;
    s->title = strdup("");
    s->border = (TuixRGBTuple){180, 180, 180};
    s->bg = (TuixRGBTuple){25, 25, 30};
    s->needs_redraw = 1;
    return s;
}

static void box_destroy_state(void* state) {
    if (!state) return;
    TuixBoxState *s = (TuixBoxState*)state;
    free(s->title);
    free(s->inserted_buffer);
    free(s);
}

static int box_ensure_buffer(TuixBoxState *s, TuixBuffer *buffer) {
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

static void box_put(TuixPixel *p, char ch, TuixRGBTuple fg, TuixRGBTuple bg) {
    memset(p, 0, sizeof(*p));
    p->sym[0] = ch;
    p->sym[1] = '\0';
    p->styles.fg = fg;
    p->styles.bg = bg;
    p->styles.custom_fg = 1;
    p->styles.custom_bg = 1;
}

static TuixPixel* box_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixBoxState *s = obj ? (TuixBoxState*)obj->state : NULL;
    if (!s || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (box_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }

    int w = buffer->width;
    int h = buffer->height;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            box_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)x], ' ', s->border, s->bg);
        }
    }

    if (w >= 2 && h >= 2) {
        for (int x = 0; x < w; x++) {
            box_put(&s->inserted_buffer[(size_t)x], '-', s->border, s->bg);
            box_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)x], '-', s->border, s->bg);
        }
        for (int y = 0; y < h; y++) {
            box_put(&s->inserted_buffer[(size_t)y * (size_t)w], '|', s->border, s->bg);
            box_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)(w - 1)], '|', s->border, s->bg);
        }
        box_put(&s->inserted_buffer[0], '+', s->border, s->bg);
        box_put(&s->inserted_buffer[(size_t)(w - 1)], '+', s->border, s->bg);
        box_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w], '+', s->border, s->bg);
        box_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)(w - 1)], '+', s->border, s->bg);
    }

    if (s->title && s->title[0] && w > 4) {
        int x = 2;
        for (const char *c = s->title; *c && x < w - 2; c++, x++) {
            TuixPixel *p = &s->inserted_buffer[(size_t)x];
            p->sym[0] = *c;
            p->sym[1] = '\0';
        }
    }

    return s->inserted_buffer;
}

static TuixHandlerResponse box_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)has_event;
    (void)is_focused;
    (void)snap;
    if (!obj || !obj->state) return (TuixHandlerResponse){.requires_redraw = 0};
    TuixBoxState *s = (TuixBoxState*)obj->state;
    if (s->needs_redraw) {
        s->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}

static void box_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixBoxState *s = (TuixBoxState*)obj->state;
    if (box_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }
}

const TuixBuilder tuix_box_builder = {
    .name = "BoxBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = box_create_state,
    .destroy_state = box_destroy_state,
    .on_event = box_handler,
    .on_resize = box_on_resize,
    .build_content = box_build_content
};

const TuixBuilder* tuix_box_init(void) {
    return &tuix_box_builder;
}

int tuix_box_set_title(TuixObject *obj, const char *title) {
    if (!obj || !obj->state) return -1;
    TuixBoxState *s = (TuixBoxState*)obj->state;
    char *copy = strdup(title ? title : "");
    if (!copy) return -1;
    free(s->title);
    s->title = copy;
    s->needs_redraw = 1;
    return 0;
}

int tuix_box_set_colors(TuixObject *obj,
                        uint8_t border_r, uint8_t border_g, uint8_t border_b,
                        uint8_t bg_r, uint8_t bg_g, uint8_t bg_b) {
    if (!obj || !obj->state) return -1;
    TuixBoxState *s = (TuixBoxState*)obj->state;
    s->border = (TuixRGBTuple){border_r, border_g, border_b};
    s->bg = (TuixRGBTuple){bg_r, bg_g, bg_b};
    s->needs_redraw = 1;
    return 0;
}
