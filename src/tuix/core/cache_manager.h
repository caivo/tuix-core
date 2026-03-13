#ifndef TUIX_cache_manager_H
#define TUIX_cache_manager_H

#include "types.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void tuix_cache_scenes();

static void tuix_append_scene(TuixScene* scene, const char* name);

void tuix_restore_scenes();

#ifdef __cplusplus
}
#endif

#endif
