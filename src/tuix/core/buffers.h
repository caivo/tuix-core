#ifndef TUIX_buffers_H
#define TUIX_buffers_H

#include "types.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a new scene with the given name.
 * If the name already exists, does nothing.
 * Returns 0 on success, 1 if the name already exists.
 */
int tuix_init_scene(const char* name);

/**
 * Frees the scene and removes it from the registry.
 */
void tuix_free_scene(const char* name);

/**
 * Clears all buffers inside the scene (but keeps the scene allocated).
 */
void tuix_clear_scene(const char* name);

/**
 * Returns an allocated array of scene names.
 * The caller must free the returned array AND each string.
 *
 * Returns NULL if no scenes exist.
 */
char** tuix_get_scenes(void);

/**
 * Returns a pointer to a scene structure by name.
 * Returns NULL if not found.
 */
TuixScene* tuix_get_scene(const char* name);

/**
 * Select the active scene by name. Sets current_scene_name to the
 * registry's own interned string and marks the scene active.
 * Returns 0 on success, -1 if the scene does not exist.
 */
int tuix_select_scene(char* name);

#ifdef __cplusplus
}
#endif

#endif
