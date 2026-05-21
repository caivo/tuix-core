#include "text_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *text;
    TuixRGBTuple fg;
    TuixRGBTuple bg;
    int use_bg;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixTextState;

static void* text_create_state(void* params) {
    (void)params;
    TuixTextState *s = calloc(1, sizeof(TuixTextState));
    if (!s) return NULL;
    s->text = strdup("");
    s->fg = (TuixRGBTuple){230, 230, 230};
    s->bg = (TuixRGBTuple){0, 0, 0};
    s->use_bg = 0;
    s->needs_redraw = 1;
    return s;
}

static void text_destroy_state(void* state) {
    if (!state) return;
    TuixTextState *s = (TuixTextState*)state;
    free(s->text);
    free(s->inserted_buffer);
    free(s);
}

static int text_ensure_buffer(TuixTextState *s, TuixBuffer *buffer) {
    int w = buffer->width;
    int h = buffer->height;
    size_t n = (size_t)w * (size_t)h;
    size_t required = sizeof(TuixPixel) * n;

    if (s->inserted_buffer && (size_t)s->inserted_buffer_size == required) {
        return 0;
    }

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

static TuixPixel* text_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixTextState *s = obj ? (TuixTextState*)obj->state : NULL;
    if (!s || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (text_ensure_buffer(s, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

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

    int x = 0;
    int y = 0;
    const char *t = s->text ? s->text : "";
    for (const char *c = t; *c && y < h; c++) {
        if (*c == '\n') {
            y++;
            x = 0;
            continue;
        }
        if (x >= w) {
            y++;
            x = 0;
            if (y >= h) break;
        }
        TuixPixel *p = &s->inserted_buffer[(size_t)y * (size_t)w + (size_t)x];
        p->sym[0] = *c;
        p->sym[1] = '\0';
        x++;
    }

    return s->inserted_buffer;
}

static TuixHandlerResponse text_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)has_event;
    (void)is_focused;
    (void)snap;
    if (!obj || !obj->state) return (TuixHandlerResponse){.requires_redraw = 0};
    TuixTextState *s = (TuixTextState*)obj->state;
    if (s->needs_redraw) {
        s->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}

static void text_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixTextState *s = (TuixTextState*)obj->state;
    if (text_ensure_buffer(s, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

const TuixBuilder tuix_text_builder = {
    .name = "TextBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = text_create_state,
    .destroy_state = text_destroy_state,
    .on_event = text_handler,
    .on_resize = text_on_resize,
    .build_content = text_build_content
};

const TuixBuilder* tuix_text_init(void) {
    return &tuix_text_builder;
}

int tuix_text_set_text(TuixObject *obj, const char *text) {
    if (!obj || !obj->state) return -1;
    TuixTextState *s = (TuixTextState*)obj->state;
    char *copy = strdup(text ? text : "");
    if (!copy) return -1;
    free(s->text);
    s->text = copy;
    s->needs_redraw = 1;
    return 0;
}

int tuix_text_set_fg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b) {
    if (!obj || !obj->state) return -1;
    TuixTextState *s = (TuixTextState*)obj->state;
    s->fg = (TuixRGBTuple){r, g, b};
    s->needs_redraw = 1;
    return 0;
}

int tuix_text_set_bg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b) {
    if (!obj || !obj->state) return -1;
    TuixTextState *s = (TuixTextState*)obj->state;
    s->bg = (TuixRGBTuple){r, g, b};
    s->use_bg = 1;
    s->needs_redraw = 1;
    return 0;
}

int tuix_text_clear_bg(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixTextState *s = (TuixTextState*)obj->state;
    s->use_bg = 0;
    s->needs_redraw = 1;
    return 0;
}
