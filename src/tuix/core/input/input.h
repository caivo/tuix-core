/* input.h - Unified input module
 * Single-thread reader for keyboard and mouse events.
 * Enable/disable keyboard or mouse handling via flags.
 */
#ifndef TUIX_INPUT_H
#define TUIX_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../types.h"

/* Control which inputs are handled (set before listen_input). */
extern int tuix_input_keyboard_enabled;
extern int tuix_input_mouse_enabled;

/* Start the unified input listener thread. Returns 0 on success. */
void listen_input(void);

/* Stop the listener thread and clean up. */
void stop_input_listening(void);

/* Get the next input snapshot.
 * Pops one keyboard event and one mouse event from their queues.
 * has_event == 0 means no event in that queue. */
TuixInputSnapshot get_input_snapshot(void);

/* Peek the latest input snapshot without consuming queued events. */
TuixInputSnapshot peek_input_snapshot(void);

#ifdef __cplusplus
}
#endif

#endif /* TUIX_INPUT_H */
