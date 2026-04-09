#include "compositor.h"
#include "../tuix_registry.h"
#include "../types.h"
#include "../buffers.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Persistent final buffer - only reallocated when the terminal dimensions change. */
static TuixFinalBuffer s_final = { NULL, 0, 0, 0 };
static TuixHitmap s_hitmap = { NULL, 0, 0, 0, 0 };
static TuixPixel *s_blank_row  = NULL;
static int        s_blank_w    = 0;
static int       *s_draw_order = NULL;
static unsigned char *s_draw_visited = NULL;
static int        s_draw_cap   = 0;
static TuixScene *s_cached_order_scene = NULL;
static unsigned long long s_cached_order_topology_version = 0;
static int s_cached_order_scene_count = -1;
static int s_cached_order_count = 0;
static int s_last_traversal_cache_hit = 0;

static int ensure_draw_capacity(int n) {
    if (n <= s_draw_cap) return 1;
    int *new_order = malloc(sizeof(int) * (size_t)n);
    if (!new_order) return 0;
    unsigned char *new_visited = malloc(sizeof(unsigned char) * (size_t)n);
    if (!new_visited) {
        free(new_order);
        return 0;
    }
    if (s_draw_order && s_draw_cap > 0) {
        memcpy(new_order, s_draw_order, sizeof(int) * (size_t)s_draw_cap);
    }
    if (s_draw_visited && s_draw_cap > 0) {
        memcpy(new_visited, s_draw_visited, sizeof(unsigned char) * (size_t)s_draw_cap);
    }
    free(s_draw_order);
    free(s_draw_visited);
    s_draw_order = new_order;
    s_draw_visited = new_visited;
    s_draw_cap = n;
    return 1;
}

static int find_index_by_uid(TuixScene *scene, int uid) {
    for (int i = 0; i < scene->count; i++) {
        TuixBuffer *b = scene->buffers[i];
        if (b && b->obj && b->obj->uid == uid) return i;
    }
    return -1;
}

static int is_root_buffer(TuixScene *scene, int idx) {
    TuixBuffer *b = scene->buffers[idx];
    if (!b || !b->obj) return 1;
    if (b->parent_uid < 0) return 1;
    if (b->parent_uid == b->obj->uid) return 1;
    return find_index_by_uid(scene, b->parent_uid) < 0;
}

static void append_subtree(TuixScene *scene, int idx, int *out_count) {
    if (idx < 0 || idx >= scene->count) return;
    if (s_draw_visited[idx]) return;
    TuixBuffer *node = scene->buffers[idx];
    if (!node || !node->obj) {
        s_draw_visited[idx] = 1;
        return;
    }

    s_draw_visited[idx] = 1;
    s_draw_order[(*out_count)++] = idx;
    int uid = node->obj->uid;

    for (int j = 0; j < scene->count; j++) {
        TuixBuffer *child = scene->buffers[j];
        if (!child || !child->obj) continue;
        if (child->parent_uid == uid) {
            append_subtree(scene, j, out_count);
        }
    }
}

static int build_traversal_order(TuixScene *scene) {
    if (!scene || scene->count <= 0) return 0;

    if (s_cached_order_scene == scene &&
        s_cached_order_topology_version == scene->topology_version &&
        s_cached_order_scene_count == scene->count &&
        s_cached_order_count > 0) {
        s_last_traversal_cache_hit = 1;
        return s_cached_order_count;
    }

    if (!ensure_draw_capacity(scene->count)) return -1;
    s_last_traversal_cache_hit = 0;

    memset(s_draw_visited, 0, sizeof(unsigned char) * (size_t)scene->count);
    int out_count = 0;

    int root_count = 0;
    for (int i = 0; i < scene->count; i++) {
        if (is_root_buffer(scene, i)) {
            s_draw_order[root_count++] = i;
        }
    }

    /* Stable insertion sort: low z first (back), high z last (front). */
    for (int i = 1; i < root_count; i++) {
        int key_idx = s_draw_order[i];
        int key_z = 0;
        if (scene->buffers[key_idx]) {
            key_z = scene->buffers[key_idx]->z_index;
        }
        int j = i - 1;
        while (j >= 0) {
            int cur_idx = s_draw_order[j];
            int cur_z = 0;
            if (scene->buffers[cur_idx]) {
                cur_z = scene->buffers[cur_idx]->z_index;
            }
            if (cur_z > key_z) {
                s_draw_order[j + 1] = s_draw_order[j];
                j--;
            } else {
                break;
            }
        }
        s_draw_order[j + 1] = key_idx;
    }

    for (int i = 0; i < root_count; i++) {
        append_subtree(scene, s_draw_order[i], &out_count);
    }

    /* Fallback for cyclic or malformed parent chains: keep deterministic draw order. */
    for (int i = 0; i < scene->count; i++) {
        if (!s_draw_visited[i]) {
            append_subtree(scene, i, &out_count);
        }
    }

    s_cached_order_scene = scene;
    s_cached_order_topology_version = scene->topology_version;
    s_cached_order_scene_count = scene->count;
    s_cached_order_count = out_count;

    return out_count;
}

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

static void rebuild_hitmap(int width, int height) {
    size_t total = (size_t)width * (size_t)height;
    if (s_hitmap.width != width || s_hitmap.height != height) {
        free(s_hitmap.pixels);
        s_hitmap.pixels = NULL;
        s_hitmap.width = width;
        s_hitmap.height = height;
        s_hitmap.capacity = 0;
        s_hitmap.count = 0;
    }

    if (total == 0) {
        return;
    }

    if (!s_hitmap.pixels) {
        s_hitmap.pixels = malloc(sizeof(TuixHitmapPixel) * total);
        if (!s_hitmap.pixels) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        s_hitmap.capacity = (int)total;
    } else if (s_hitmap.capacity < (int)total) {
        TuixHitmapPixel *new_pixels = realloc(s_hitmap.pixels, sizeof(TuixHitmapPixel) * total);
        if (!new_pixels) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        s_hitmap.pixels = new_pixels;
        s_hitmap.capacity = (int)total;
    }

    TuixHitmapPixel blank = { .obj_uid = 0, .idx = -1 };
    for (size_t i = 0; i < total; i++) {
        s_hitmap.pixels[i] = blank;
    }
    s_hitmap.count = (int)total;
}

TuixFinalBuffer *tuix_composite_scene(TuixScene *scene) {
    int W = tuix_registry.terminal_width;
    /* The bottom row is reserved for log output, so we subtract one. */
    int H = tuix_registry.terminal_height > 1 ? (tuix_registry.terminal_height - 1) : 1;

    rebuild_hitmap(W, H);

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

    int ordered_count = build_traversal_order(scene);
    if (ordered_count < 0) return &s_final;

    for (int oi = 0; oi < ordered_count; oi++) {
        int i = s_draw_order[oi];
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

            if (buffer->obj && buffer->obj->uid > 0) {
                for (int rx = 0; rx < run; rx++) {
                    int hit_idx = (int)((size_t)fy * (size_t)W + (size_t)(x0 + ml + rx));
                    s_hitmap.pixels[hit_idx].obj_uid = buffer->obj->uid;
                    s_hitmap.pixels[hit_idx].idx = hit_idx;
                }
            }

            /* Ensure custom_fg is set on every composited pixel. */
            for (int rx = 0; rx < run; rx++) {
                if (!dst[rx].styles.custom_fg)
                    dst[rx].styles.custom_fg = 1;
            }
        }
    }

    return &s_final;
}

TuixHitmap *tuix_get_last_hitmap(void) {
    return &s_hitmap;
}

int tuix_get_last_traversal_cache_hit(void) {
    return s_last_traversal_cache_hit;
}
