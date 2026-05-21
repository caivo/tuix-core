#include "badge_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *text;
    TuixRGBTuple fg;
    TuixRGBTuple bg;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixBadgeState;

static void* badge_create_state(void* params) {
    (void)params;
    TuixBadgeState *s = calloc(1, sizeof(TuixBadgeState));
    if (!s) return NULL;
    s->text = strdup("Badge");
    s->fg = (TuixRGBTuple){255, 255, 255};
    s->bg = (TuixRGBTuple){72, 100, 190};
    s->needs_redraw = 1;
    return s;
}

static void badge_destroy_state(void* state) {
    if (!state) return;
    TuixBadgeState *s = (TuixBadgeState*)state;
    free(s->text);
    free(s->inserted_buffer);
    free(s);
}

static int badge_ensure_buffer(TuixBadgeState *s, TuixBuffer *buffer) {
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

static void badge_put(TuixPixel *p, char ch, TuixRGBTuple fg, TuixRGBTuple bg) {
    memset(p, 0, sizeof(*p));
    p->sym[0] = ch;
    p->sym[1] = '\0';
    p->styles.fg = fg;
    p->styles.bg = bg;
    p->styles.custom_fg = 1;
    p->styles.custom_bg = 1;
}

static TuixPixel* badge_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixBadgeState *s = obj ? (TuixBadgeState*)obj->state : NULL;
    if (!s || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (badge_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }

    int w = buffer->width;
    int h = buffer->height;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            badge_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)x], ' ', s->fg, s->bg);
        }
    }

    const char *txt = s->text ? s->text : "";
    int len = (int)strlen(txt);
    int content_len = len + 2;
    int start = (w - content_len) / 2;
    if (start < 0) start = 0;
    int row = h / 2;

    if (row >= 0 && row < h) {
        if (start < w) badge_put(&s->inserted_buffer[(size_t)row * (size_t)w + (size_t)start], '[', s->fg, s->bg);
        int x = start + 1;
        for (int i = 0; i < len && x < w; i++, x++) {
            badge_put(&s->inserted_buffer[(size_t)row * (size_t)w + (size_t)x], txt[i], s->fg, s->bg);
        }
        if (x < w) badge_put(&s->inserted_buffer[(size_t)row * (size_t)w + (size_t)x], ']', s->fg, s->bg);
    }

    return s->inserted_buffer;
}

static TuixHandlerResponse badge_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)has_event;
    (void)is_focused;
    (void)snap;
    if (!obj || !obj->state) return (TuixHandlerResponse){.requires_redraw = 0};
    TuixBadgeState *s = (TuixBadgeState*)obj->state;
    if (s->needs_redraw) {
        s->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}

static void badge_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixBadgeState *s = (TuixBadgeState*)obj->state;
    if (badge_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }
}

const TuixBuilder tuix_badge_builder = {
    .name = "BadgeBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = badge_create_state,
    .destroy_state = badge_destroy_state,
    .on_event = badge_handler,
    .on_resize = badge_on_resize,
    .build_content = badge_build_content
};

const TuixBuilder* tuix_badge_init(void) {
    return &tuix_badge_builder;
}

int tuix_badge_set_text(TuixObject *obj, const char *text) {
    if (!obj || !obj->state) return -1;
    TuixBadgeState *s = (TuixBadgeState*)obj->state;
    char *copy = strdup(text ? text : "");
    if (!copy) return -1;
    free(s->text);
    s->text = copy;
    s->needs_redraw = 1;
    return 0;
}

int tuix_badge_set_colors(TuixObject *obj,
                          uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                          uint8_t bg_r, uint8_t bg_g, uint8_t bg_b) {
    if (!obj || !obj->state) return -1;
    TuixBadgeState *s = (TuixBadgeState*)obj->state;
    s->fg = (TuixRGBTuple){fg_r, fg_g, fg_b};
    s->bg = (TuixRGBTuple){bg_r, bg_g, bg_b};
    s->needs_redraw = 1;
    return 0;
}
