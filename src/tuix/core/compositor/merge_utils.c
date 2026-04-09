#include "../types.h"
#include "../tuix_registry.h"
#include "../buffers.h"
#include "compositor.h"

#include <stdio.h>

TuixHitmap tuix_composite_hitmap(const char* scene_name) {
    TuixHitmap empty = {0};
    if (!scene_name) {
        return empty;
    }

    tuix_lock();
    TuixScene* scene = tuix_get_scene(scene_name);
    if (!scene) {
        tuix_unlock();
        printf("Scene not found: %s\n", scene_name);
        return empty;
    }

    TuixHitmap *hitmap = tuix_get_last_hitmap();
    if (!hitmap || !hitmap->pixels) {
        tuix_unlock();
        return empty;
    }

    TuixHitmap copy = *hitmap;
    tuix_unlock();
    return copy;
}