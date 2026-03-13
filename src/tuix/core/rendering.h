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

#ifdef __cplusplus
}
#endif

#endif
