#ifndef TUIX_compositor_H
#define TUIX_compositor_H

#include "../types.h"

/* Fast path: accepts pre-resolved scene pointer, returns pointer to
   persistent static buffer (no allocation per frame). */
TuixFinalBuffer *tuix_composite_scene(TuixScene *scene);

#endif
