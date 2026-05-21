#ifndef TUIX_compositor_H
#define TUIX_compositor_H

#include "../types.h"

/* Fast path: accepts pre-resolved scene pointer, returns pointer to
   persistent static buffer (no allocation per frame). */
TuixFinalBuffer *tuix_composite_scene(TuixScene *scene);

/* Fast partial update: copy a single buffer's pixels into the persistent
   final buffer and update the hitmap for that region. Returns the final
   buffer pointer (same as `tuix_composite_scene`). If sizes or state don't
   match a full composite is done instead and the returned pointer is still
   valid. */
TuixFinalBuffer *tuix_blit_buffer(TuixScene *scene, TuixBuffer *buffer);

/* Returns the most recently generated frame hitmap. Valid after
   tuix_composite_scene() has run for the current frame. */
TuixHitmap *tuix_get_last_hitmap(void);

/* Returns 1 if traversal order cache was reused on the most recent compose call. */
int tuix_get_last_traversal_cache_hit(void);

#endif
