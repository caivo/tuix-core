#ifndef TUIX_input_builder_H
#define TUIX_input_builder_H

#include "../../types.h"

/* Returns a pointer to the static input builder descriptor */
const TuixBuilder* tuix_input_init(void);

/* ── Configuration ────────────────────────────────────────────── */

/* Set placeholder text shown when the field is empty. */
int tuix_input_set_placeholder(TuixObject *obj, const char *text);

/* ── Input handling (feed + poll pattern) ─────────────────────── */

/* Feed an input snapshot. Processes typing, Backspace, Delete,
 * Left/Right/Home/End navigation, and Enter to submit.
 * Call once per frame for the focused input object. */
int tuix_input_feed_input(TuixObject *obj, TuixInputSnapshot snap);

/* ── Result retrieval ─────────────────────────────────────────── */

/* Current text content (never NULL - returns "" on error). */
const char* tuix_input_get_text(TuixObject *obj);

/* 1 if the user pressed Enter, 0 otherwise. */
int tuix_input_is_submitted(TuixObject *obj);

/* Returns the text if submitted, NULL otherwise. */
const char* tuix_input_get_result(TuixObject *obj);

/* Clear text and reset submitted state. */
void tuix_input_reset(TuixObject *obj);

#endif
