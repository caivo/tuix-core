#include "canvas_builder.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../../buffer_manager.h"

typedef struct {
    TuixPixel pixel;
    int x, y;
} TuixCanvasChange;


typedef struct {
    TuixPixel *pixels;
    int w, h;
} TuixCachedSprite;

static int canvas_get_dims(TuixObject *obj, int *out_w, int *out_h) {
    if (!obj || !out_w || !out_h) {
        return -1;
    }
    TuixBuffer snap;
    if (tuix_get_buffer_snapshot_by_uid(obj->uid, &snap) != 0) {
        return -1;
    }
    *out_w = snap.width;
    *out_h = snap.height;
    return 0;
}

typedef struct {
    TuixCanvasChange *changes;
    int changes_count;
    int changes_capacity;
    TuixPixel* inserted_buffer;
    int inserted_buffer_size;
    int     needs_redraw;
    /* Sprite cache */
    TuixCachedSprite *sprite_cache;
    int sprite_cache_count;
    int sprite_cache_capacity;
} TuixCanvasState;

static void* tuix_canvas_create_state(void* params) {
    TuixCanvasState *s = calloc(1, sizeof(TuixCanvasState));
    if (!s) return NULL;
    s->changes = NULL;
    s->changes_count = 0;
    s->changes_capacity = 0;
    s->inserted_buffer = NULL;
    s->inserted_buffer_size = 0;
    s->needs_redraw = 1;
    s->sprite_cache = NULL;
    s->sprite_cache_count = 0;
    s->sprite_cache_capacity = 0;
    return s;
}

static void tuix_canvas_destroy_state(void* state) {
    if (!state) return;
    TuixCanvasState *s = (TuixCanvasState*)state;
    if (s->changes) free(s->changes);
    /* Do NOT free inserted_buffer here - tuix_free_buffer() already frees buffer->pixels,
       which points to the same allocation. */
    for (int i = 0; i < s->sprite_cache_count; i++)
        free(s->sprite_cache[i].pixels);
    free(s->sprite_cache);
    free(s);
}

int tuix_canvas_set_pixel(TuixObject *obj, int x, int y, char sym, uint8_t fgr, uint8_t fgg, uint8_t fgb, uint8_t bgr, uint8_t bgg, uint8_t bgb) {
    if (!obj || !obj->state) return -1;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    int width = 0, height = 0;
    if (canvas_get_dims(obj, &width, &height) != 0) return -1;
    if (x < 0 || x >= width || y < 0 || y >= height) return -1;
    if (s->changes_count + 1 > s->changes_capacity) {
        int newcap = s->changes_capacity < 64 ? 64 : s->changes_capacity * 2;
        TuixCanvasChange *tmp = realloc(s->changes, sizeof(TuixCanvasChange) * (size_t)newcap);
        if (!tmp) return -1;
        s->changes = tmp;
        s->changes_capacity = newcap;
    }
    TuixCanvasChange *change = &s->changes[s->changes_count];
    change->x = x;
    change->y = y;
    memset(&change->pixel, 0, sizeof(change->pixel));
    change->pixel.sym[0] = sym;
    change->pixel.sym[1] = '\0';
    change->pixel.styles.fg = (TuixRGBTuple){fgr, fgg, fgb};
    change->pixel.styles.bg = (TuixRGBTuple){bgr, bgg, bgb};
    change->pixel.styles.custom_bg = 1;
    change->pixel.styles.custom_fg = 1;
    s->changes_count++;
    s->needs_redraw = 1;
    return 0;
}

int tuix_canvas_insert_buffer(TuixObject *obj, TuixPixel *pixels, int size) {
    if (!obj || !obj->state) return -1;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    /* Copy incoming buffer to avoid ownership/aliasing issues. */
    if (s->inserted_buffer) free(s->inserted_buffer);
    if (pixels && size > 0) {
        s->inserted_buffer = malloc((size_t)size);
        if (!s->inserted_buffer) return -1;
        memcpy(s->inserted_buffer, pixels, (size_t)size);
        s->inserted_buffer_size = size;
    } else {
        s->inserted_buffer = NULL;
        s->inserted_buffer_size = 0;
    }
    s->changes_count = 0;
    if (s->changes) { free(s->changes); s->changes = NULL; }
    s->needs_redraw = 1;
    return 0;
}


static int canvas_ensure_capacity(TuixCanvasState *s, int extra) {
    if (s->changes_count + extra <= s->changes_capacity) return 0;
    int newcap = s->changes_capacity < 64 ? 64 : s->changes_capacity;
    while (newcap < s->changes_count + extra) newcap *= 2;
    TuixCanvasChange *tmp = realloc(s->changes, sizeof(TuixCanvasChange) * (size_t)newcap);
    if (!tmp) return -1;
    s->changes = tmp;
    s->changes_capacity = newcap;
    return 0;
}

static int canvas_queue_pixel(TuixCanvasState *s, int x, int y, char sym,
                              uint8_t fgr, uint8_t fgg, uint8_t fgb,
                              uint8_t bgr, uint8_t bgg, uint8_t bgb) {
    if (canvas_ensure_capacity(s, 1)) return -1;
    TuixCanvasChange *c = &s->changes[s->changes_count];
    c->x = x;
    c->y = y;
    memset(&c->pixel, 0, sizeof(c->pixel));
    c->pixel.sym[0] = sym;
    c->pixel.sym[1] = '\0';
    c->pixel.styles.fg  = (TuixRGBTuple){fgr, fgg, fgb};
    c->pixel.styles.bg  = (TuixRGBTuple){bgr, bgg, bgb};
    c->pixel.styles.custom_bg = 1;
    c->pixel.styles.custom_fg = 1;
    s->changes_count++;
    return 0;
}


int tuix_canvas_draw_line(TuixObject *obj, int x0, int y0, int x1, int y1,
                          char sym,
                          uint8_t fgr, uint8_t fgg, uint8_t fgb,
                          uint8_t bgr, uint8_t bgg, uint8_t bgb) {
    if (!obj || !obj->state) return -1;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    int w = 0, h = 0;
    if (canvas_get_dims(obj, &w, &h) != 0) return -1;

    /* Bresenham's line algorithm */
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    for (;;) {
        if (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h)
            canvas_queue_pixel(s, x0, y0, sym, fgr, fgg, fgb, bgr, bgg, bgb);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
    s->needs_redraw = 1;
    return 0;
}

int tuix_canvas_draw_rect(TuixObject *obj, int x, int y, int w, int h,
                          char sym, int filled,
                          uint8_t fgr, uint8_t fgg, uint8_t fgb,
                          uint8_t bgr, uint8_t bgg, uint8_t bgb) {
    if (!obj || !obj->state) return -1;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    int bw = 0, bh = 0;
    if (canvas_get_dims(obj, &bw, &bh) != 0) return -1;

    for (int ry = y; ry < y + h; ry++) {
        for (int rx = x; rx < x + w; rx++) {
            if (rx < 0 || rx >= bw || ry < 0 || ry >= bh) continue;
            if (filled || ry == y || ry == y + h - 1 || rx == x || rx == x + w - 1)
                canvas_queue_pixel(s, rx, ry, sym, fgr, fgg, fgb, bgr, bgg, bgb);
        }
    }
    s->needs_redraw = 1;
    return 0;
}

int tuix_canvas_draw_circle(TuixObject *obj, int cx, int cy, int radius,
                            char sym, int filled,
                            uint8_t fgr, uint8_t fgg, uint8_t fgb,
                            uint8_t bgr, uint8_t bgg, uint8_t bgb) {
    if (!obj || !obj->state) return -1;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    int bw = 0, bh = 0;
    if (canvas_get_dims(obj, &bw, &bh) != 0) return -1;

    /* Midpoint ellipse with semi-axes a=2*radius (wide) and b=radius (tall).
       Produces exact outlines with no double pixels or gaps. */
    int a = radius * 2;
    int b = radius;
    long long a2 = (long long)a * a;
    long long b2 = (long long)b * b;

    /* Helper: queue a clipped pixel */
#define QPIX(px,py) do { \
    int _x = (px), _y = (py); \
    if (_x >= 0 && _x < bw && _y >= 0 && _y < bh) \
        canvas_queue_pixel(s, _x, _y, sym, fgr, fgg, fgb, bgr, bgg, bgb); \
} while(0)

    if (filled) {
        for (int dy = -b; dy <= b; dy++) {
            long long dy2 = (long long)dy * dy;
            long long num = a2 * (b2 - dy2);
            int dx_max = (b2 > 0) ? (int)(num / b2) : 0;
            int dx = (int)dx_max;
            while ((long long)dx * dx > dx_max) dx--;
            while ((long long)(dx+1) * (dx+1) <= dx_max) dx++;
            int y = cy + dy;
            for (int x = cx - dx; x <= cx + dx; x++)
                QPIX(x, y);
        }
    } else {
        /* Outline: Bresenham midpoint ellipse, plots 4-way symmetric points. */
        int x = 0, y = b;
        long long dx = 2 * b2 * x;
        long long dy = 2 * a2 * y;

        /* Region 1: |slope| < 1  (move x, decide y) */
        long long d1 = b2 - a2 * b + a2 / 4;
        while (dx < dy) {
            QPIX(cx + x, cy + y);
            QPIX(cx - x, cy + y);
            QPIX(cx + x, cy - y);
            QPIX(cx - x, cy - y);
            if (d1 < 0) {
                x++;
                dx += 2 * b2;
                d1 += dx + b2;
            } else {
                x++;
                y--;
                dx += 2 * b2;
                dy -= 2 * a2;
                d1 += dx - dy + b2;
            }
        }

        /* Region 2: |slope| >= 1  (move y, decide x) */
        long long d2 = b2 * (long long)(2 * x + 1) * (2 * x + 1) / 4
                      + a2 * (long long)(y - 1) * (y - 1) - a2 * b2;
        while (y >= 0) {
            QPIX(cx + x, cy + y);
            QPIX(cx - x, cy + y);
            QPIX(cx + x, cy - y);
            QPIX(cx - x, cy - y);
            if (d2 > 0) {
                y--;
                dy -= 2 * a2;
                d2 += a2 - dy;
            } else {
                y--;
                x++;
                dx += 2 * b2;
                dy -= 2 * a2;
                d2 += dx - dy + a2;
            }
        }
    }
#undef QPIX
    s->needs_redraw = 1;
    return 0;
}

int tuix_canvas_draw_text(TuixObject *obj, int x, int y, const char *text,
                          uint8_t fgr, uint8_t fgg, uint8_t fgb,
                          uint8_t bgr, uint8_t bgg, uint8_t bgb) {
    if (!obj || !obj->state || !text) return -1;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    int bw = 0, bh = 0;
    if (canvas_get_dims(obj, &bw, &bh) != 0) return -1;

    int col = x;
    for (const char *p = text; *p; p++) {
        if (*p == '\n') { y++; col = x; continue; }
        if (col >= 0 && col < bw && y >= 0 && y < bh)
            canvas_queue_pixel(s, col, y, *p, fgr, fgg, fgb, bgr, bgg, bgb);
        col++;
    }
    s->needs_redraw = 1;
    return 0;
}

int tuix_canvas_draw_sprite(TuixObject *obj, int dst_x, int dst_y,
                            int sprite_w, int sprite_h,
                            const TuixPixel *sprite) {
    if (!obj || !obj->state || !sprite) return -1;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    int bw = 0, bh = 0;
    if (canvas_get_dims(obj, &bw, &bh) != 0) return -1;

    for (int sy = 0; sy < sprite_h; sy++) {
        int py = dst_y + sy;
        if (py < 0 || py >= bh) continue;
        for (int sx = 0; sx < sprite_w; sx++) {
            int px = dst_x + sx;
            if (px < 0 || px >= bw) continue;
            const TuixPixel *sp = &sprite[(size_t)sy * sprite_w + sx];
            /* Queue each sprite pixel as a change */
            if (canvas_ensure_capacity(s, 1)) return -1;
            TuixCanvasChange *c = &s->changes[s->changes_count];
            c->x = px;
            c->y = py;
            c->pixel = *sp;
            s->changes_count++;
        }
    }
    s->needs_redraw = 1;
    return 0;
}


int tuix_canvas_cache_sprite(TuixObject *obj, int sprite_w, int sprite_h,
                             const TuixPixel *sprite) {
    if (!obj || !obj->state || !sprite || sprite_w <= 0 || sprite_h <= 0) return -1;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;

    /* Grow cache array if needed (2x policy) */
    if (s->sprite_cache_count >= s->sprite_cache_capacity) {
        int newcap = s->sprite_cache_capacity < 4 ? 4 : s->sprite_cache_capacity * 2;
        TuixCachedSprite *tmp = realloc(s->sprite_cache, sizeof(TuixCachedSprite) * (size_t)newcap);
        if (!tmp) return -1;
        s->sprite_cache = tmp;
        s->sprite_cache_capacity = newcap;
    }

    size_t npx = (size_t)sprite_w * sprite_h;
    TuixPixel *copy = malloc(sizeof(TuixPixel) * npx);
    if (!copy) return -1;
    memcpy(copy, sprite, sizeof(TuixPixel) * npx);

    int id = s->sprite_cache_count;
    s->sprite_cache[id].pixels = copy;
    s->sprite_cache[id].w = sprite_w;
    s->sprite_cache[id].h = sprite_h;
    s->sprite_cache_count++;
    return id;
}

void tuix_canvas_free_cached_sprite(TuixObject *obj, int id) {
    if (!obj || !obj->state) return;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    if (id < 0 || id >= s->sprite_cache_count) return;
    if (s->sprite_cache[id].pixels) {
        free(s->sprite_cache[id].pixels);
        s->sprite_cache[id].pixels = NULL;
        s->sprite_cache[id].w = 0;
        s->sprite_cache[id].h = 0;
    }
}

int tuix_canvas_draw_cached_sprite(TuixObject *obj, int id, int dst_x, int dst_y) {
    if (!obj || !obj->state) return -1;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    if (id < 0 || id >= s->sprite_cache_count) return -1;
    TuixCachedSprite *cs = &s->sprite_cache[id];
    if (!cs->pixels) return -1;
    return tuix_canvas_draw_sprite(obj, dst_x, dst_y, cs->w, cs->h, cs->pixels);
}


static TuixPixel* tuix_canvas_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixCanvasState *s = obj ? (TuixCanvasState*)obj->state : NULL;

    if (s) {
        size_t n = (size_t)buffer->height * (size_t)buffer->width;
        size_t required_bytes = sizeof(TuixPixel) * n;
        /* Reallocate inserted_buffer when buffer size changes to avoid OOB writes. */
        if (!s->inserted_buffer || (size_t)s->inserted_buffer_size != required_bytes) {
            if (s->inserted_buffer) {
                free(s->inserted_buffer);
                s->inserted_buffer = NULL;
                s->inserted_buffer_size = 0;
            }
            /* Allocate fresh buffer initialized to spaces */
            s->inserted_buffer = calloc(n, sizeof(TuixPixel));
            if (!s->inserted_buffer) { printf("OOM\n"); exit(1); }
            for (size_t i = 0; i < n; i++) {
                s->inserted_buffer[i].sym[0] = ' ';
                s->inserted_buffer[i].sym[1] = '\0';
                s->inserted_buffer[i].styles.fg = (TuixRGBTuple){255,255,255};
                s->inserted_buffer[i].styles.bg = (TuixRGBTuple){0,0,0};
                s->inserted_buffer[i].styles.custom_bg = 1;
                s->inserted_buffer[i].styles.custom_fg = 1;
            }
            s->inserted_buffer_size = (int)required_bytes;
            /* Ensure the handler requests a redraw after buffer size change */
            s->needs_redraw = 1;
            if (buffer) buffer->required_redraw = 1;
        }

        /* Apply changes in-place */
        if (s->changes_count > 0) {
            for (int i = 0; i < s->changes_count; i++) {
                TuixCanvasChange *c = &s->changes[i];
                if (c->x < 0 || c->x >= buffer->width || c->y < 0 || c->y >= buffer->height) continue;
                s->inserted_buffer[(size_t)c->y * buffer->width + c->x] = c->pixel;
            }
            free(s->changes);
            s->changes = NULL;
            s->changes_count = 0;
            s->changes_capacity = 0;
        }

        return s->inserted_buffer;
    }

    /* No per-object state: fallback to one-shot allocation (engine will own this) */
    TuixPixel* pixels = calloc((size_t)buffer->height * buffer->width, sizeof(TuixPixel));
    if (!pixels) { printf("Memory allocation failed!\n"); exit(1); }
    for (int y = 0; y < buffer->height; y++) {
        for (int x = 0; x < buffer->width; x++) {
            TuixPixel* px = &pixels[(size_t)y * buffer->width + x];
            px->sym[0] = '#';
            px->sym[1] = '\0';
            px->styles.fg = (TuixRGBTuple){255,255,255};
            px->styles.bg = (TuixRGBTuple){0,0,0};
            px->styles.custom_bg = 1;
            px->styles.custom_fg = 1;
            px->styles.bold = 0;
            px->styles.underlined = 0;
            px->styles.italic = 0;
            px->styles.dim = 0;
        }
    }
    return pixels;
}

static TuixHandlerResponse tuix_canvas_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)has_event;
    (void)is_focused;
    (void)snap;
    if (!obj || !obj->state)
        return (TuixHandlerResponse){.requires_redraw = 0};
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    if (s->needs_redraw) {
        s->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}

/* forward declaration so initializer can take address */
static void tuix_canvas_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height);

const TuixBuilder tuix_canvas_builder = {
    .name = "CanvasBuilder",
    .version = "1.1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = tuix_canvas_create_state,
    .destroy_state = tuix_canvas_destroy_state,
    .on_event = tuix_canvas_handler,
    .on_resize    = tuix_canvas_on_resize,
    .build_content = tuix_canvas_build_content
};

const TuixBuilder* tuix_canvas_init(void) {
    return &tuix_canvas_builder;
}

static void tuix_canvas_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    if (!obj || !obj->state) return;
    TuixCanvasState *s = (TuixCanvasState*)obj->state;
    size_t n = (size_t)width * (size_t)height;
    size_t required_bytes = sizeof(TuixPixel) * n;
    if (s->inserted_buffer && (size_t)s->inserted_buffer_size == required_bytes) return;
    if (s->inserted_buffer) { free(s->inserted_buffer); s->inserted_buffer = NULL; s->inserted_buffer_size = 0; }
    s->inserted_buffer = calloc(n, sizeof(TuixPixel));
    if (!s->inserted_buffer) { printf("OOM\n"); exit(1); }
    for (size_t i = 0; i < n; i++) {
        s->inserted_buffer[i].sym[0] = ' ';
        s->inserted_buffer[i].sym[1] = '\0';
        s->inserted_buffer[i].styles.fg = (TuixRGBTuple){255,255,255};
        s->inserted_buffer[i].styles.bg = (TuixRGBTuple){0,0,0};
        s->inserted_buffer[i].styles.custom_bg = 1;
        s->inserted_buffer[i].styles.custom_fg = 1;
    }
    s->inserted_buffer_size = (int)required_bytes;
    s->needs_redraw = 1;
    s->changes_count = 0;
    if (s->changes) { free(s->changes); s->changes = NULL; s->changes_capacity = 0; }
    if (buffer) buffer->required_redraw = 1;
}