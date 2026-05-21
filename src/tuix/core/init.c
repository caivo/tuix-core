#include "init.h"
#include "content_builder/builders_registrant.h"
#include "input/input.h"
#include <stdio.h>
#include "tuix_registry.h"
#include <stdlib.h>
#ifdef TUIX_ENABLE_SOUND
#include "tuix_sound.h"
#endif

#ifndef TUIX_DEFAULT_DEBUG_MODE
#define TUIX_DEFAULT_DEBUG_MODE TUIX_DEBUG_NONE
#endif

#ifdef _WIN32
TUIX_REGISTRY_API CRITICAL_SECTION tuix_registry_lock;
#else
#include <pthread.h>
TUIX_REGISTRY_API pthread_mutex_t tuix_registry_lock;
#endif

// Define single shared registry
TUIX_REGISTRY_API TuixRegistry tuix_registry = {0};

int tuix_init() {
    int response = tuix_init_registry();
    if (response != 0) {
        printf("Failed to initialize TUIX registry\n");
        return response;
    }
    listen_input();
    /* Optional prototype sound subsystem. */
#ifdef TUIX_ENABLE_SOUND
    tuix_sound_init();
#endif
    return 0;
}

int tuix_init_registry() {
    tuix_registry.scenes.scenes = NULL;
    tuix_registry.scenes.names = NULL;
    tuix_registry.scenes.count = 0;
    tuix_registry.scenes.capacity = 0;

    tuix_registry.next_uid = 1;

    tuix_registry.subcycles.subcycles = NULL;
    tuix_registry.subcycles.count = 0;
    tuix_registry.subcycles.capacity = 0;

    tuix_registry.builders.builders = NULL;
    tuix_registry.builders.count = 0;
    tuix_registry.builders.capacity = 0;

    tuix_registry.frame_counter = 0;
    tuix_registry.debug_config = (TuixDebugConfig)TUIX_DEBUG_VERBOSE;

    /* initialize lock */
#ifdef _WIN32
    InitializeCriticalSection(&tuix_registry_lock);
#else
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&tuix_registry_lock, &attr);
    pthread_mutexattr_destroy(&attr);
#endif

    return 0;
}

int tuix_destroy_registry() {
    // Free subcycles registry
    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        TuixSceneSubcycles* scene_subs = tuix_registry.subcycles.subcycles[i];

        for (int j = 0; j < scene_subs->count; j++) {
            TuixSubcycle* sub = scene_subs->subcycles[j];
            if (!sub) {
                continue;
            }
            if (sub->obj && sub->obj->state && sub->obj->builder && sub->obj->builder->destroy_state) {
                sub->obj->builder->destroy_state(sub->obj->state);
                sub->obj->state = NULL;
            }
            free(sub);
        }

        free(scene_subs->subcycles);
        free(scene_subs->scene_name);
        free(scene_subs);
    }

    free(tuix_registry.subcycles.subcycles);
    tuix_registry.subcycles.count = 0;

    // Free scenes registry
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        for (int j = 0; j < tuix_registry.scenes.scenes[i]->count; j++) {
            TuixBuffer* buf = tuix_registry.scenes.scenes[i]->buffers[j];
            if (!buf) {
                continue;
            }
            if (buf->pixels) {
                if (buf->pixels_owned) free(buf->pixels);
                buf->pixels = NULL;
                buf->pixels_owned = 0;
            }
            free(buf->children_uids);
            buf->children_uids = NULL;
            free(buf->obj);
            free(buf);
        }
        free(tuix_registry.scenes.scenes[i]->buffers);
        free(tuix_registry.scenes.scenes[i]->buffer_by_uid);
        free(tuix_registry.scenes.scenes[i]->root_uids);
        free(tuix_registry.scenes.scenes[i]);
        free(tuix_registry.scenes.names[i]);
    }
    free(tuix_registry.scenes.scenes);
    free(tuix_registry.scenes.names);
    tuix_registry.scenes.count = 0;

    free(tuix_registry.builders.builders);
    tuix_registry.builders.count = 0;

    /* destroy lock */
#ifdef _WIN32
    DeleteCriticalSection(&tuix_registry_lock);
#else
    pthread_mutex_destroy(&tuix_registry_lock);
#endif

    return 0;
}

int tuix_shutdown() {
    stop_input_listening();
    int response = tuix_destroy_registry();
    if (response != 0) {
        printf("Failed to destroy TUIX registry\n");
        return response;
    }
    /* Optional prototype sound subsystem. */
#ifdef TUIX_ENABLE_SOUND
    tuix_sound_shutdown();
#endif
    return 0;
}
