#include "compositor.h"
#include "../tuix_registry.h"
#include "../types.h"
#include "../buffers.h"

#include <stdlib.h>
#include <string.h>

/* Persistent final buffer - only reallocated when the terminal dimensions change. */
static TuixFinalBuffer s_final = { NULL, 0, 0, 0 };
static TuixPixel *s_blank_row  = NULL;
static int        s_blank_w    = 0;

static void rebuild_blank_row(int width) {
    if (s_blank_w == width && s_blank_row) return;

    free(s_blank_row);
    s_blank_row = malloc(sizeof(TuixPixel) * (size_t)width);
    if (!s_blank_row) return;
    s_blank_w = width;

    TuixPixel blank;
    memset(&blank, 0, sizeof(blank));
    blank.sym[0] = ' ';
    blank.styles.fg = (TuixRGBTuple){255, 255, 255};
    blank.styles.bg = (TuixRGBTuple){0, 0, 0};
    blank.styles.custom_fg = 1;
    blank.styles.custom_bg = 1;

    for (int x = 0; x < width; x++)
        s_blank_row[x] = blank;
}

TuixFinalBuffer *tuix_composite_scene(TuixScene *scene) {
    int W = tuix_registry.terminal_width;
    /* The bottom row is reserved for log output, so we subtract one. */
    int H = tuix_registry.terminal_height > 1 ? (tuix_registry.terminal_height - 1) : 1;

    if (s_final.width != W || s_final.height != H) {
        free(s_final.pixels);
        s_final.pixels = malloc(sizeof(TuixPixel) * (size_t)H * (size_t)W);
        if (!s_final.pixels) { s_final.width = s_final.height = 0; return &s_final; }
        s_final.width  = W;
        s_final.height = H;
        s_final.full_redraw = 1;
        rebuild_blank_row(W);
    } else {
        s_final.full_redraw = 0;
    }

    rebuild_blank_row(W);
    for (int y = 0; y < H; y++)
        memcpy(&s_final.pixels[(size_t)y * W], s_blank_row, sizeof(TuixPixel) * (size_t)W);

    for (int i = 0; i < scene->count; i++) {
        TuixBuffer *buffer = scene->buffers[i];
        if (!buffer || !buffer->pixels) continue;

        int ml = buffer->margin_left;
        int mt = buffer->margin_top;
        int bw = buffer->width;
        int bh = buffer->height;

        int y0 = (mt < 0) ? -mt : 0;
        int y1 = (mt + bh > H) ? (H - mt) : bh;

        int x0 = (ml < 0) ? -ml : 0;
        int x1 = (ml + bw > W) ? (W - ml) : bw;
        int run = x1 - x0;
        if (run <= 0) continue;

        for (int by = y0; by < y1; by++) {
            int fy = by + mt;
            TuixPixel *dst = &s_final.pixels[(size_t)fy * W + (x0 + ml)];
            TuixPixel *src = &buffer->pixels[(size_t)by * bw + x0];

            memcpy(dst, src, sizeof(TuixPixel) * (size_t)run);

            /* Ensure custom_fg is set on every composited pixel. */
            for (int rx = 0; rx < run; rx++) {
                if (!dst[rx].styles.custom_fg)
                    dst[rx].styles.custom_fg = 1;
            }
        }
    }

    return &s_final;
}