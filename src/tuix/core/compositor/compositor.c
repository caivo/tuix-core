#include "compositor.h"
#include "../tuix_registry.h"
#include "../types.h"
#include "../buffers.h"
#include "../rendering.h"
#include "../viewport.h"

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
static int        s_draw_visited_cap = 0;
static TuixScene *s_cached_order_scene = NULL;
static unsigned long long s_cached_order_topology_version = 0;
static int s_cached_order_scene_count = -1;
static int s_cached_order_count = 0;
static int s_last_traversal_cache_hit = 0;

static int ensure_draw_capacity(int n) {
    if (n <= s_draw_cap) return 1;
    int *new_order = malloc(sizeof(int) * (size_t)n);
    if (!new_order) return 0;
    if (s_draw_order && s_draw_cap > 0) {
        memcpy(new_order, s_draw_order, sizeof(int) * (size_t)s_draw_cap);
    }
    free(s_draw_order);
    s_draw_order = new_order;
    s_draw_cap = n;
    return 1;
}

static int ensure_draw_visited_capacity(int n) {
    if (n <= s_draw_visited_cap) return 1;
    unsigned char *new_visited = malloc(sizeof(unsigned char) * (size_t)n);
    if (!new_visited) return 0;
    if (s_draw_visited && s_draw_visited_cap > 0) {
        memcpy(new_visited, s_draw_visited, sizeof(unsigned char) * (size_t)s_draw_visited_cap);
    }
    free(s_draw_visited);
    s_draw_visited = new_visited;
    s_draw_visited_cap = n;
    return 1;
}

static TuixBuffer* scene_buffer_by_uid(TuixScene* scene, int uid) {
    if (!scene || !scene->buffer_by_uid || uid <= 0 || uid >= scene->max_uid_capacity) {
        return NULL;
    }
    return scene->buffer_by_uid[uid];
}

static int buffer_is_viewport(const TuixBuffer *buffer) {
    return buffer && buffer->obj && tuix_object_is_viewport(buffer->obj);
}

static void resolve_buffer_viewport_context(
    TuixScene *scene,
    TuixBuffer *buffer,
    int W,
    int H,
    int *out_eff_ml,
    int *out_eff_mt,
    int *out_clip_l,
    int *out_clip_t,
    int *out_clip_r,
    int *out_clip_b,
    int *out_viewport_uid) {
    int eff_ml = buffer ? buffer->margin_left : 0;
    int eff_mt = buffer ? buffer->margin_top : 0;
    int clip_l = 0, clip_t = 0, clip_r = W, clip_b = H;
    int viewport_uid = (buffer_is_viewport(buffer) && buffer->obj) ? buffer->obj->uid : 0;

    int anc_uid = buffer ? buffer->parent_uid : -1;
    while (anc_uid >= 0) {
        TuixBuffer *parent = scene_buffer_by_uid(scene, anc_uid);
        if (!parent || !parent->obj) break;
        if (buffer_is_viewport(parent)) {
            int offx = 0, offy = 0;
            int inset_left = 1, inset_top = 1, inset_right = 1, inset_bottom = 1;
            tuix_object_get_viewport_offset(parent->obj, &offx, &offy);
            tuix_object_get_viewport_insets(parent->obj, &inset_left, &inset_top, &inset_right, &inset_bottom);
            eff_ml -= offx;
            eff_mt -= offy;

            if (!viewport_uid) viewport_uid = parent->obj->uid;

            int p_left = parent->margin_left + inset_left;
            int p_top = parent->margin_top + inset_top;
            int p_right = parent->margin_left + parent->width - inset_right;
            int p_bottom = parent->margin_top + parent->height - inset_bottom;
            if (p_left > clip_l) clip_l = p_left;
            if (p_top > clip_t) clip_t = p_top;
            if (p_right < clip_r) clip_r = p_right;
            if (p_bottom < clip_b) clip_b = p_bottom;
        }
        anc_uid = parent->parent_uid;
    }

    if (out_eff_ml) *out_eff_ml = eff_ml;
    if (out_eff_mt) *out_eff_mt = eff_mt;
    if (out_clip_l) *out_clip_l = clip_l;
    if (out_clip_t) *out_clip_t = clip_t;
    if (out_clip_r) *out_clip_r = clip_r;
    if (out_clip_b) *out_clip_b = clip_b;
    if (out_viewport_uid) *out_viewport_uid = viewport_uid;
}

static void append_subtree(TuixScene *scene, int uid, int *out_count) {
    if (!scene || uid <= 0 || uid >= s_draw_visited_cap) return;
    if (s_draw_visited[uid]) return;
    TuixBuffer *node = scene_buffer_by_uid(scene, uid);
    if (!node || !node->obj) {
        s_draw_visited[uid] = 1;
        return;
    }

    s_draw_visited[uid] = 1;
    s_draw_order[(*out_count)++] = uid;

    for (int j = 0; j < node->children_count; j++) {
        append_subtree(scene, node->children_uids[j], out_count);
    }
}

static int build_traversal_order(TuixScene *scene) {
    if (!scene || scene->count <= 0) return 0;

    if (!scene->topology_dirty &&
        s_cached_order_scene == scene &&
        s_cached_order_topology_version == scene->topology_version &&
        s_cached_order_scene_count == scene->count &&
        s_cached_order_count > 0) {
        s_last_traversal_cache_hit = 1;
        return s_cached_order_count;
    }

    if (!ensure_draw_capacity(scene->count)) return -1;
    if (!ensure_draw_visited_capacity(scene->max_uid_capacity > 0 ? scene->max_uid_capacity : 1)) return -1;
    s_last_traversal_cache_hit = 0;

    memset(s_draw_visited, 0, sizeof(unsigned char) * (size_t)s_draw_visited_cap);
    int out_count = 0;

    for (int i = 0; i < scene->root_count; i++) {
        append_subtree(scene, scene->root_uids[i], &out_count);
    }

    /* Fallback for cyclic or malformed parent chains: keep deterministic draw order. */
    for (int i = 0; i < scene->count; i++) {
        TuixBuffer *buffer = scene->buffers[i];
        if (!buffer || !buffer->obj) {
            continue;
        }
        if (!s_draw_visited[buffer->obj->uid]) {
            append_subtree(scene, buffer->obj->uid, &out_count);
        }
    }

    s_cached_order_scene = scene;
    s_cached_order_topology_version = scene->topology_version;
    s_cached_order_scene_count = scene->count;
    s_cached_order_count = out_count;

    return out_count;
}

static int rebuild_blank_row(int width) {
    if (width <= 0) {
        free(s_blank_row);
        s_blank_row = NULL;
        s_blank_w = 0;
        return 1;
    }

    if (s_blank_w == width && s_blank_row) return 1;

    free(s_blank_row);
    s_blank_row = NULL;
    s_blank_w = 0;
    s_blank_row = malloc(sizeof(TuixPixel) * (size_t)width);
    if (!s_blank_row) return 0;
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
    return 1;
}

static void rebuild_hitmap(int width, int height) {
    size_t total = (size_t)width * (size_t)height;
#ifdef TUIX_DEBUG_HEAP
    if (tuix_registry.debug_config != TUIX_DEBUG_NONE) debug_heap_check();
#endif
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

    TuixHitmapPixel blank = { .obj_uid = 0, .viewport_uid = 0, .idx = -1 };
    for (size_t i = 0; i < total; i++) {
        s_hitmap.pixels[i] = blank;
    }
    s_hitmap.count = (int)total;
}

TuixFinalBuffer *tuix_composite_scene(TuixScene *scene) {
    int W = tuix_registry.terminal_width;
    /* The bottom row is reserved for log output, so we subtract one. */
    int H = tuix_registry.terminal_height > 1 ? (tuix_registry.terminal_height - 1) : 1;

#ifdef TUIX_DEBUG_HEAP
    if (tuix_registry.debug_config != TUIX_DEBUG_NONE) debug_heap_check();
#endif
    rebuild_hitmap(W, H);

    if (s_final.width != W || s_final.height != H) {
#ifdef TUIX_DEBUG_HEAP
    if (tuix_registry.debug_config != TUIX_DEBUG_NONE) debug_heap_check();
#endif
        free(s_final.pixels);
        s_final.pixels = malloc(sizeof(TuixPixel) * (size_t)H * (size_t)W);
        if (!s_final.pixels) { s_final.width = s_final.height = 0; return &s_final; }
        s_final.width  = W;
        s_final.height = H;
        s_final.full_redraw = 1;
        if (!rebuild_blank_row(W)) {
            free(s_final.pixels);
            s_final.pixels = NULL;
            s_final.width = s_final.height = 0;
            return &s_final;
        }
    } else {
        s_final.full_redraw = 0;
    }

    if (!rebuild_blank_row(W)) {
        free(s_final.pixels);
        s_final.pixels = NULL;
        s_final.width = s_final.height = 0;
        return &s_final;
    }
    for (int y = 0; y < H; y++)
        memcpy(&s_final.pixels[(size_t)y * W], s_blank_row, sizeof(TuixPixel) * (size_t)W);

    int ordered_count = build_traversal_order(scene);
    if (ordered_count < 0) return &s_final;

    for (int oi = 0; oi < ordered_count; oi++) {
        int uid = s_draw_order[oi];
        TuixBuffer *buffer = scene_buffer_by_uid(scene, uid);
        if (!buffer || !buffer->pixels) continue;
        int ml = buffer->margin_left;
        int mt = buffer->margin_top;
        int bw = buffer->width;
        int bh = buffer->height;

        int eff_ml = ml;
        int eff_mt = mt;
        int clip_l = 0, clip_t = 0, clip_r = W, clip_b = H;
        int viewport_uid = 0;
        resolve_buffer_viewport_context(scene, buffer, W, H, &eff_ml, &eff_mt, &clip_l, &clip_t, &clip_r, &clip_b, &viewport_uid);

        int left = eff_ml;
        int top = eff_mt;
        int right = eff_ml + bw;
        int bottom = eff_mt + bh;

        if (left < clip_l) left = clip_l;
        if (top < clip_t) top = clip_t;
        if (right > clip_r) right = clip_r;
        if (bottom > clip_b) bottom = clip_b;

        if (left >= right || top >= bottom) continue;

        int x0 = left - eff_ml;
        int x1 = right - eff_ml;
        int y0 = top - eff_mt;
        int y1 = bottom - eff_mt;
        if (x0 < 0) x0 = 0;
        if (y0 < 0) y0 = 0;
        if (x1 > bw) x1 = bw;
        if (y1 > bh) y1 = bh;

        int run = x1 - x0;
        if (run <= 0) continue;

        for (int by = y0; by < y1; by++) {
            int fy = by + eff_mt;
            TuixPixel *dst = &s_final.pixels[(size_t)fy * W + (size_t)(eff_ml + x0)];
            TuixPixel *src = &buffer->pixels[(size_t)by * bw + x0];
            memcpy(dst, src, sizeof(TuixPixel) * (size_t)run);

            int base_hit = (int)((size_t)fy * (size_t)W + (size_t)(eff_ml + x0));
            int hit_uid = (buffer->obj && buffer->obj->uid > 0) ? buffer->obj->uid : 0;
            for (int rx = 0; rx < run; rx++) {
                if (hit_uid) {
                    s_hitmap.pixels[base_hit + rx].obj_uid = hit_uid;
                    s_hitmap.pixels[base_hit + rx].viewport_uid = viewport_uid;
                    s_hitmap.pixels[base_hit + rx].idx = base_hit + rx;
                }
                if (!dst[rx].styles.custom_fg) dst[rx].styles.custom_fg = 1;
            }
        }
    }

    return &s_final;
}

TuixFinalBuffer *tuix_blit_buffer(TuixScene *scene, TuixBuffer *buffer) {
    if (!scene || !buffer || !buffer->pixels) return tuix_composite_scene(scene);

    int W = tuix_registry.terminal_width;
    int H = tuix_registry.terminal_height > 1 ? (tuix_registry.terminal_height - 1) : 1;

    /* If final buffer isn't allocated or size changed, fall back to full compose. */
    if (s_final.width != W || s_final.height != H || !s_final.pixels) {
        return tuix_composite_scene(scene);
    }

    /* Ensure hitmap exists and is sized; if not, fall back. */
    if (s_hitmap.width != W || s_hitmap.height != H || !s_hitmap.pixels) {
        /* Attempt to allocate hitmap without clearing existing final pixels. */
        size_t total = (size_t)W * (size_t)H;
        s_hitmap.pixels = malloc(sizeof(TuixHitmapPixel) * total);
        if (!s_hitmap.pixels) return tuix_composite_scene(scene);
        s_hitmap.width = W; s_hitmap.height = H; s_hitmap.capacity = (int)total; s_hitmap.count = (int)total;
        TuixHitmapPixel blank = { .obj_uid = 0, .viewport_uid = 0, .idx = -1 };
        for (size_t i = 0; i < total; i++) s_hitmap.pixels[i] = blank;
    }

    int ml = buffer->margin_left;
    int mt = buffer->margin_top;
    int bw = buffer->width;
    int bh = buffer->height;

    if (bw <= 0 || bh <= 0) return &s_final;

    int eff_ml = ml;
    int eff_mt = mt;
    int clip_l = 0, clip_t = 0, clip_r = W, clip_b = H;
    int viewport_uid = 0;
    resolve_buffer_viewport_context(scene, buffer, W, H, &eff_ml, &eff_mt, &clip_l, &clip_t, &clip_r, &clip_b, &viewport_uid);

    int left = eff_ml;
    int top = eff_mt;
    int right = eff_ml + bw;
    int bottom = eff_mt + bh;

    if (left < clip_l) left = clip_l;
    if (top < clip_t) top = clip_t;
    if (right > clip_r) right = clip_r;
    if (bottom > clip_b) bottom = clip_b;

    if (left >= right || top >= bottom) return &s_final;

    int x0 = left - eff_ml;
    int x1 = right - eff_ml;
    int y0 = top - eff_mt;
    int y1 = bottom - eff_mt;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > bw) x1 = bw;
    if (y1 > bh) y1 = bh;
    int run = x1 - x0;
    if (run <= 0) return &s_final;

    for (int by = y0; by < y1; by++) {
        int fy = by + eff_mt;
        TuixPixel *dst = &s_final.pixels[(size_t)fy * W + (size_t)(eff_ml + x0)];
        TuixPixel *src = &buffer->pixels[(size_t)by * bw + x0];

        int base_hit = (int)((size_t)fy * (size_t)W + (size_t)(eff_ml + x0));
        int uid = (buffer->obj && buffer->obj->uid > 0) ? buffer->obj->uid : 0;
        int any_changed = 0;
        for (int rx = 0; rx < run; rx++) {
            int hit_idx = base_hit + rx;
            if (uid && s_hitmap.pixels[hit_idx].obj_uid == uid) {
                dst[rx] = src[rx];
                s_hitmap.pixels[hit_idx].viewport_uid = viewport_uid;
                if (!dst[rx].styles.custom_fg) dst[rx].styles.custom_fg = 1;
                any_changed = 1;
            }
        }

        if (any_changed) {
            tuix_render_mark_rows_dirty(fy, fy + 1);
        }
    }

    tuix_render_mark_rows_dirty(eff_mt + y0, eff_mt + y1);

    return &s_final;
}

TuixHitmap *tuix_get_last_hitmap(void) {
    return &s_hitmap;
}

int tuix_get_last_traversal_cache_hit(void) {
    return s_last_traversal_cache_hit;
}
