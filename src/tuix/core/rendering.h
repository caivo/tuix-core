#include "types.h"


// Quantize RGB to 16-bit terminal color (returns nearest TuixRGBTuple)
static inline TuixRGBTuple tuix_rgb16(const TuixRGBTuple c) {
	int r5 = (c.r * 31 + 127) / 255;
	int g6 = (c.g * 63 + 127) / 255;
	int b5 = (c.b * 31 + 127) / 255;
	TuixRGBTuple out;
	out.r = (r5 * 255 + 15) / 31;
	out.g = (g6 * 255 + 31) / 63;
	out.b = (b5 * 255 + 15) / 31;
	return out;
}
#ifndef TUIX_rendering_H
#define TUIX_rendering_H

#include "types.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void tuix_render_streaming(TuixFinalBuffer *buffer, TuixRowDoneCallback on_row_done, void *user_data);

/* Force the next `tuix_render_streaming()` call to redraw the whole frame.
   Use this after manual terminal writes like clear-screen sequences, because
   they invalidate the renderer's diff cache. */
void tuix_render_invalidate_all(void);

/* Mark a range of rows [y0,y1) as dirty for the next render. If called
	before `tuix_render_streaming`, only those rows will have their hash
	recomputed and be considered changed, reducing per-frame CPU when only
	a small region changed. Safe to call multiple times per frame. */
void tuix_render_mark_rows_dirty(int y0, int y1);

/* Return the number of bytes written to stdout by the most recent
	`tuix_render_streaming()` invocation. Use this to compute "physical"
	frames-per-second (frames that actually emitted bytes to the terminal). */
size_t tuix_render_get_last_frame_bytes(void);

#ifdef __cplusplus
}
#endif

#endif
