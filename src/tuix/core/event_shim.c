#include <stdio.h>

/* Minimal shim implementations for functions referenced from main.c
 * These are placeholders to allow building the combined extension.
 * Replace with real implementations if runtime behavior is required.
 */

void tuix_batch_global_commit(void) {
    /* no-op: commit pending global batches (placeholder) */
}

void tuix_event_commit_python(void) {
    /* no-op: flush C->Python events (placeholder) */
}
