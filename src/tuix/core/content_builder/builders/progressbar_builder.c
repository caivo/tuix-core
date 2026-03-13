#include "progressbar_builder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../../types.h"
#include "../../buffer_manager.h"

typedef struct {
    float       value;
    char        fill_sym;
    char        empty_sym;
    TuixRGBTuple fg_fill;
    TuixRGBTuple bg_fill;
    TuixRGBTuple fg_empty;
    TuixRGBTuple bg_empty;
    int         show_percentage;
    int         needs_redraw;
    TuixPixel  *inserted_buffer;
    int         inserted_buffer_size;
} TuixProgressBarState;



static void* progressbar_create_state(void* params) {
    (void)params;
    TuixProgressBarState *s = calloc(1, sizeof(TuixProgressBarState));
    if (!s) return NULL;
    s->value     = 0.0f;
    s->fill_sym  = '#';
    s->empty_sym = '-';
    s->fg_fill   = (TuixRGBTuple){80, 200, 120};
    s->bg_fill   = (TuixRGBTuple){30, 30, 30};
    s->fg_empty  = (TuixRGBTuple){80, 80, 80};
    s->bg_empty  = (TuixRGBTuple){30, 30, 30};
    s->show_percentage = 1;
    s->needs_redraw    = 1;
    return s;
}

static void progressbar_destroy_state(void* state) {
    if (!state) return;
    TuixProgressBarState *s = (TuixProgressBarState*)state;
    /* Do NOT free inserted_buffer here - tuix_free_buffer() already frees buffer->pixels,
       which points to the same allocation. */
    free(s);
}



static TuixPixel* progressbar_build_content(TuixObject *obj, TuixBuffer *buffer) {
    TuixProgressBarState *s = obj ? (TuixProgressBarState*)obj->state : NULL;
    int w = buffer->width, h = buffer->height;
    size_t n = (size_t)w * h;

    if (!s) {
        TuixPixel *px = calloc(n, sizeof(TuixPixel));
        if (!px) { printf("OOM\n"); exit(1); }
        for (size_t i = 0; i < n; i++) {
            px[i].sym[0] = ' '; px[i].sym[1] = '\0';
            px[i].styles.fg = (TuixRGBTuple){255,255,255};
            px[i].styles.bg = (TuixRGBTuple){0,0,0};
            px[i].styles.custom_bg = 1;
            px[i].styles.custom_fg = 1;
        }
        return px;
    }

    /* Ensure inserted_buffer matches current buffer size; reallocate on resize. */
    size_t required_bytes = sizeof(TuixPixel) * n;
    if (!s->inserted_buffer || (size_t)s->inserted_buffer_size != required_bytes) {
        if (s->inserted_buffer) {
            free(s->inserted_buffer);
            s->inserted_buffer = NULL;
            s->inserted_buffer_size = 0;
        }
        s->inserted_buffer = calloc(n, sizeof(TuixPixel));
        if (!s->inserted_buffer) { printf("OOM\n"); exit(1); }
        s->inserted_buffer_size = (int)required_bytes;
        s->needs_redraw = 1;
        if (buffer) buffer->required_redraw = 1;
    }

    for (size_t i = 0; i < n; i++) {
        TuixPixel *p = &s->inserted_buffer[i];
        p->sym[0] = ' '; p->sym[1] = '\0';
        p->styles.fg = s->fg_empty;
        p->styles.bg = s->bg_empty;
        p->styles.custom_bg = 1;
        p->styles.custom_fg = 1;
    }

    int row = h / 2;
    if (row >= h) row = 0;

    /* Layout: [####----] 100%
       brackets = 2 chars, label = 5 chars " 100%" if percentage is shown */
    int pct_space = s->show_percentage ? 5 : 0;
    int bar_w = w - 2 - pct_space;
    if (bar_w < 1) { bar_w = w; }

    float val = s->value < 0.0f ? 0.0f : (s->value > 1.0f ? 1.0f : s->value);
    int filled = (int)(val * (float)bar_w + 0.5f);
    if (filled > bar_w) filled = bar_w;

    int col = 0;

    if (col < w && row < h) {
        TuixPixel *p = &s->inserted_buffer[(size_t)row * w + col];
        p->sym[0] = '['; p->sym[1] = '\0';
        p->styles.fg = (TuixRGBTuple){180, 180, 180};
        p->styles.bg = s->bg_empty;
    }
    col++;

    for (int x = 0; x < bar_w && col < w; x++, col++) {
        TuixPixel *p = &s->inserted_buffer[(size_t)row * w + col];
        if (x < filled) {
            p->sym[0] = s->fill_sym; p->sym[1] = '\0';
            p->styles.fg = s->fg_fill;
            p->styles.bg = s->bg_fill;
        } else {
            p->sym[0] = s->empty_sym; p->sym[1] = '\0';
            p->styles.fg = s->fg_empty;
            p->styles.bg = s->bg_empty;
        }
    }

    if (col < w && row < h) {
        TuixPixel *p = &s->inserted_buffer[(size_t)row * w + col];
        p->sym[0] = ']'; p->sym[1] = '\0';
        p->styles.fg = (TuixRGBTuple){180, 180, 180};
        p->styles.bg = s->bg_empty;
        col++;
    }

    if (s->show_percentage && col < w && row < h) {
        int pct = (int)(val * 100.0f + 0.5f);
        char label[8];
        int lablen = snprintf(label, sizeof(label), "%3d%%", pct);
        for (int i = 0; i < lablen && col < w; i++, col++) {
            TuixPixel *p = &s->inserted_buffer[(size_t)row * w + col];
            p->sym[0] = label[i]; p->sym[1] = '\0';
            p->styles.fg = (TuixRGBTuple){200, 200, 200};
            p->styles.bg = s->bg_empty;
        }
    }

    return s->inserted_buffer;
}


static TuixHandlerResponse progressbar_handler(TuixObject *obj) {
    if (!obj || !obj->state)
        return (TuixHandlerResponse){.requires_redraw = 0};
    TuixProgressBarState *s = (TuixProgressBarState*)obj->state;
    if (s->needs_redraw) {
        s->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}


/* forward declaration so initializer can take address */
static void progressbar_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height);

const TuixBuilder tuix_progressbar_builder = {
    .name       = "ProgressBarBuilder",
    .version    = "1.0",
    .author     = "custosh",
    .namespace  = "tuix",
    .create_state  = progressbar_create_state,
    .destroy_state = progressbar_destroy_state,
    .handler_func  = progressbar_handler,
    .on_resize     = progressbar_on_resize,
    .build_content = progressbar_build_content
};

const TuixBuilder* tuix_progressbar_init(void) {
    return &tuix_progressbar_builder;
}


int tuix_progressbar_set_value(TuixObject *obj, float value) {
    if (!obj || !obj->state) return -1;
    TuixProgressBarState *s = (TuixProgressBarState*)obj->state;
    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;
    s->value = value;
    s->needs_redraw = 1;
    return 0;
}

float tuix_progressbar_get_value(TuixObject *obj) {
    if (!obj || !obj->state) return 0.0f;
    return ((TuixProgressBarState*)obj->state)->value;
}

int tuix_progressbar_set_style(TuixObject *obj, char fill, char empty,
                               uint8_t fill_r, uint8_t fill_g, uint8_t fill_b,
                               uint8_t empty_r, uint8_t empty_g, uint8_t empty_b) {
    if (!obj || !obj->state) return -1;
    TuixProgressBarState *s = (TuixProgressBarState*)obj->state;
    s->fill_sym  = fill;
    s->empty_sym = empty;
    s->fg_fill   = (TuixRGBTuple){fill_r, fill_g, fill_b};
    s->fg_empty  = (TuixRGBTuple){empty_r, empty_g, empty_b};
    s->needs_redraw = 1;
    return 0;
}

void tuix_progressbar_show_percentage(TuixObject *obj, int show) {
    if (!obj || !obj->state) return;
    TuixProgressBarState *s = (TuixProgressBarState*)obj->state;
    s->show_percentage = show ? 1 : 0;
    s->needs_redraw = 1;
}

static void progressbar_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    if (!obj || !obj->state) return;
    TuixProgressBarState *s = (TuixProgressBarState*)obj->state;
    size_t n = (size_t)width * (size_t)height;
    size_t required_bytes = sizeof(TuixPixel) * n;
    if (s->inserted_buffer && (size_t)s->inserted_buffer_size == required_bytes) return;
    if (s->inserted_buffer) { free(s->inserted_buffer); s->inserted_buffer = NULL; s->inserted_buffer_size = 0; }
    s->inserted_buffer = calloc(n, sizeof(TuixPixel));
    if (!s->inserted_buffer) { printf("OOM\n"); exit(1); }
    s->inserted_buffer_size = (int)required_bytes;
    s->needs_redraw = 1;
    if (buffer) buffer->required_redraw = 1;
}