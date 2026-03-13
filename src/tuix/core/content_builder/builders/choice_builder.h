#ifndef TUIX_choice_builder_H
#define TUIX_choice_builder_H

#include "../../types.h"

/* Returns a pointer to the static choice builder descriptor */
const TuixBuilder* tuix_choice_init(void);

/* ── Configuration ────────────────────────────────────────────── */

/* Set the list of options (deep-copied). Returns 0 on success, -1 on error. */
int tuix_choice_set_options(TuixObject *obj, const char **labels, int count);

/* ── Input handling (feed + poll pattern) ─────────────────────── */

/* Feed an input snapshot. The builder processes Up/Down/Enter keys.
 * Call this once per frame for the focused choice object. */
int tuix_choice_feed_input(TuixObject *obj, TuixInputSnapshot snap);

/* ── Result retrieval ─────────────────────────────────────────── */

/* Currently highlighted index (0-based), or -1 on error. */
int tuix_choice_get_selected(TuixObject *obj);

/* 1 if the user pressed Enter to confirm, 0 otherwise. */
int tuix_choice_is_confirmed(TuixObject *obj);

/* Returns the confirmed index (0-based) if confirmed, -1 otherwise. */
int tuix_choice_get_result(TuixObject *obj);

/* Reset confirmed state and selection back to 0. */
void tuix_choice_reset(TuixObject *obj);

#endif
