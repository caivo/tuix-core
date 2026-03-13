#ifndef TUIX_progressbar_builder_H
#define TUIX_progressbar_builder_H

#include "../../types.h"
#include <stdint.h>

/* Returns a pointer to the static progressbar builder descriptor */
const TuixBuilder* tuix_progressbar_init(void);

/* ── Control ──────────────────────────────────────────────────── */

/* Set progress value (clamped to 0.0 .. 1.0). Returns 0 on success. */
int tuix_progressbar_set_value(TuixObject *obj, float value);

/* Get current progress value. */
float tuix_progressbar_get_value(TuixObject *obj);

/* ── Styling ──────────────────────────────────────────────────── */

/* Set fill/empty characters and their foreground colours. */
int tuix_progressbar_set_style(TuixObject *obj, char fill, char empty,
                               uint8_t fill_r, uint8_t fill_g, uint8_t fill_b,
                               uint8_t empty_r, uint8_t empty_g, uint8_t empty_b);

/* Show or hide the " XX%" percentage label (1 = show, 0 = hide). */
void tuix_progressbar_show_percentage(TuixObject *obj, int show);

#endif
