#include "icon_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char symbol[8];
    TuixRGBTuple fg;
    TuixRGBTuple bg;
    int use_bg;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixIconState;

static void* icon_create_state(void* params) {
    (void)params;
    TuixIconState *s = calloc(1, sizeof(TuixIconState));
    if (!s) return NULL;
    strcpy(s->symbol, "*");
    s->fg = (TuixRGBTuple){255, 214, 102};
    s->bg = (TuixRGBTuple){0, 0, 0};
    s->use_bg = 0;
    s->needs_redraw = 1;
    return s;
}

static void icon_destroy_state(void* state) {
    if (!state) return;
    free(state);
}

static int icon_ensure_buffer(TuixIconState *s, TuixBuffer *buffer) {
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

static TuixPixel* icon_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixIconState *s = obj ? (TuixIconState*)obj->state : NULL;
    if (!s || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (icon_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }

    int w = buffer->width;
    int h = buffer->height;
    size_t n = (size_t)w * (size_t)h;

    for (size_t i = 0; i < n; i++) {
        TuixPixel *p = &s->inserted_buffer[i];
        memset(p, 0, sizeof(*p));
        p->sym[0] = ' ';
        p->sym[1] = '\0';
        p->styles.fg = s->fg;
        p->styles.bg = s->bg;
        p->styles.custom_fg = 1;
        p->styles.custom_bg = s->use_bg ? 1 : 0;
    }

    int x = w / 2;
    int y = h / 2;
    if (x >= 0 && x < w && y >= 0 && y < h) {
        TuixPixel *p = &s->inserted_buffer[(size_t)y * (size_t)w + (size_t)x];
        snprintf(p->sym, sizeof(p->sym), "%s", s->symbol[0] ? s->symbol : "*");
    }

    return s->inserted_buffer;
}

static TuixHandlerResponse icon_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)has_event;
    (void)is_focused;
    (void)snap;
    if (!obj || !obj->state) return (TuixHandlerResponse){.requires_redraw = 0};
    TuixIconState *s = (TuixIconState*)obj->state;
    if (s->needs_redraw) {
        s->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}

static void icon_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixIconState *s = (TuixIconState*)obj->state;
    if (icon_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }
}

const TuixBuilder tuix_icon_builder = {
    .name = "IconBuilder",
    .version = "1.1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = icon_create_state,
    .destroy_state = icon_destroy_state,
    .on_event = icon_handler,
    .on_resize = icon_on_resize,
    .build_content = icon_build_content
};

const TuixBuilder* tuix_icon_init(void) {
    return &tuix_icon_builder;
}

int tuix_icon_set_symbol(TuixObject *obj, const char *symbol) {
    if (!obj || !obj->state) return -1;
    TuixIconState *s = (TuixIconState*)obj->state;
    if (!symbol || !symbol[0]) return -1;
    snprintf(s->symbol, sizeof(s->symbol), "%s", symbol);
    s->needs_redraw = 1;
    return 0;
}

int tuix_icon_set_fg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b) {
    if (!obj || !obj->state) return -1;
    TuixIconState *s = (TuixIconState*)obj->state;
    s->fg = (TuixRGBTuple){r, g, b};
    s->needs_redraw = 1;
    return 0;
}

int tuix_icon_set_bg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b) {
    if (!obj || !obj->state) return -1;
    TuixIconState *s = (TuixIconState*)obj->state;
    s->bg = (TuixRGBTuple){r, g, b};
    s->use_bg = 1;
    s->needs_redraw = 1;
    return 0;
}

int tuix_icon_clear_bg(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixIconState *s = (TuixIconState*)obj->state;
    s->use_bg = 0;
    s->needs_redraw = 1;
    return 0;
}
