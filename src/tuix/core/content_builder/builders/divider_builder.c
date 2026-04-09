#include "divider_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int orientation;
    char sym;
    TuixRGBTuple color;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixDividerState;

static void* divider_create_state(void* params) {
    (void)params;
    TuixDividerState *s = calloc(1, sizeof(TuixDividerState));
    if (!s) return NULL;
    s->orientation = TUIX_DIVIDER_HORIZONTAL;
    s->sym = '-';
    s->color = (TuixRGBTuple){170, 170, 170};
    s->needs_redraw = 1;
    return s;
}

static void divider_destroy_state(void* state) {
    free(state);
}

static int divider_ensure_buffer(TuixDividerState *s, TuixBuffer *buffer) {
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

static void divider_put(TuixPixel *p, char ch, TuixRGBTuple fg) {
    memset(p, 0, sizeof(*p));
    p->sym[0] = ch;
    p->sym[1] = '\0';
    p->styles.fg = fg;
    p->styles.bg = (TuixRGBTuple){0, 0, 0};
    p->styles.custom_fg = 1;
    p->styles.custom_bg = 0;
}

static TuixPixel* divider_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixDividerState *s = obj ? (TuixDividerState*)obj->state : NULL;
    if (!s || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (divider_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }

    int w = buffer->width;
    int h = buffer->height;
    size_t n = (size_t)w * (size_t)h;

    for (size_t i = 0; i < n; i++) {
        divider_put(&s->inserted_buffer[i], ' ', s->color);
    }

    if (s->orientation == TUIX_DIVIDER_VERTICAL) {
        int x = w / 2;
        for (int y = 0; y < h; y++) {
            divider_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)x], s->sym, s->color);
        }
    } else {
        int y = h / 2;
        for (int x = 0; x < w; x++) {
            divider_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)x], s->sym, s->color);
        }
    }

    return s->inserted_buffer;
}

static TuixHandlerResponse divider_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)has_event;
    (void)is_focused;
    (void)snap;
    if (!obj || !obj->state) return (TuixHandlerResponse){.requires_redraw = 0};
    TuixDividerState *s = (TuixDividerState*)obj->state;
    if (s->needs_redraw) {
        s->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}

static void divider_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixDividerState *s = (TuixDividerState*)obj->state;
    if (divider_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }
}

const TuixBuilder tuix_divider_builder = {
    .name = "DividerBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = divider_create_state,
    .destroy_state = divider_destroy_state,
    .on_event = divider_handler,
    .on_resize = divider_on_resize,
    .build_content = divider_build_content
};

const TuixBuilder* tuix_divider_init(void) {
    return &tuix_divider_builder;
}

int tuix_divider_set_orientation(TuixObject *obj, int orientation) {
    if (!obj || !obj->state) return -1;
    TuixDividerState *s = (TuixDividerState*)obj->state;
    s->orientation = (orientation == TUIX_DIVIDER_VERTICAL) ? TUIX_DIVIDER_VERTICAL : TUIX_DIVIDER_HORIZONTAL;
    s->needs_redraw = 1;
    return 0;
}

int tuix_divider_set_symbol(TuixObject *obj, char sym) {
    if (!obj || !obj->state) return -1;
    TuixDividerState *s = (TuixDividerState*)obj->state;
    s->sym = sym ? sym : '-';
    s->needs_redraw = 1;
    return 0;
}

int tuix_divider_set_color(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b) {
    if (!obj || !obj->state) return -1;
    TuixDividerState *s = (TuixDividerState*)obj->state;
    s->color = (TuixRGBTuple){r, g, b};
    s->needs_redraw = 1;
    return 0;
}
