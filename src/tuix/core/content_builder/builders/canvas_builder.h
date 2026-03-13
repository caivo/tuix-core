#ifndef TUIX_canvas_builder_H
#define TUIX_canvas_builder_H

#include "../../types.h"
#include <stdint.h>

/* Returns a pointer to the static canvas builder descriptor */
const TuixBuilder* tuix_canvas_init(void);

/* Per-object pixel operations */
int tuix_canvas_set_pixel(TuixObject *obj, int x, int y, char sym,
						  uint8_t fgr, uint8_t fgg, uint8_t fgb,
						  uint8_t bgr, uint8_t bgg, uint8_t bgb);

/* Insert a full pixel buffer (ownership copied) */
int tuix_canvas_insert_buffer(TuixObject *obj, TuixPixel *pixels, int size);

/* ── Drawing primitives ──────────────────────────────────────── */

/* Bresenham line from (x0,y0) to (x1,y1) */
int tuix_canvas_draw_line(TuixObject *obj, int x0, int y0, int x1, int y1,
                          char sym,
                          uint8_t fgr, uint8_t fgg, uint8_t fgb,
                          uint8_t bgr, uint8_t bgg, uint8_t bgb);

/* Axis-aligned rectangle at (x,y) with size w*h. filled=1 for solid. */
int tuix_canvas_draw_rect(TuixObject *obj, int x, int y, int w, int h,
                          char sym, int filled,
                          uint8_t fgr, uint8_t fgg, uint8_t fgb,
                          uint8_t bgr, uint8_t bgg, uint8_t bgb);

/* Circle centred at (cx,cy) with given radius. filled=1 for solid disc. */
int tuix_canvas_draw_circle(TuixObject *obj, int cx, int cy, int radius,
                            char sym, int filled,
                            uint8_t fgr, uint8_t fgg, uint8_t fgb,
                            uint8_t bgr, uint8_t bgg, uint8_t bgb);

/* Write a null-terminated string starting at (x,y). Supports \n for newlines. */
int tuix_canvas_draw_text(TuixObject *obj, int x, int y, const char *text,
                          uint8_t fgr, uint8_t fgg, uint8_t fgb,
                          uint8_t bgr, uint8_t bgg, uint8_t bgb);

/* Blit a sprite (sub-buffer) of size sprite_w*sprite_h at position (dst_x,dst_y).
 * Each sprite pixel is queued as a TuixCanvasChange - only the covered region is
 * updated, unlike insert_buffer which replaces the entire buffer. */
int tuix_canvas_draw_sprite(TuixObject *obj, int dst_x, int dst_y,
                            int sprite_w, int sprite_h,
                            const TuixPixel *sprite);

/* ── Sprite cache API ────────────────────────────────────────── */

/* Cache a sprite (deep copy). Returns a non-negative cache ID on success, -1 on error. */
int tuix_canvas_cache_sprite(TuixObject *obj, int sprite_w, int sprite_h,
                             const TuixPixel *sprite);

/* Free a previously cached sprite by its ID. */
void tuix_canvas_free_cached_sprite(TuixObject *obj, int id);

/* Draw a cached sprite at (dst_x, dst_y). Returns 0 on success, -1 on error. */
int tuix_canvas_draw_cached_sprite(TuixObject *obj, int id, int dst_x, int dst_y);

#endif
