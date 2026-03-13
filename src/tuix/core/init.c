#include "init.h"
#include "content_builder/builders_registrant.h"
#include <stdio.h>
#include "tuix_registry.h"
#include <stdlib.h>

#ifdef _WIN32
CRITICAL_SECTION tuix_registry_lock;
#else
#include <pthread.h>
pthread_mutex_t tuix_registry_lock;
#endif

// Define single shared registry
TuixRegistry tuix_registry = {0};

int tuix_init() {
    int response = tuix_init_registry();
    if (response != 0) {
        printf("Failed to initialize TUIX registry\n");
        return response;
    }
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
    // Free scenes registry
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        for (int j = 0; j < tuix_registry.scenes.scenes[i]->count; j++) {
            TuixBuffer* buf = tuix_registry.scenes.scenes[i]->buffers[j];
            free(buf->pixels);
            free(buf->obj);
            free(buf);
        }
        free(tuix_registry.scenes.scenes[i]->buffers);
        free(tuix_registry.scenes.scenes[i]);
        free(tuix_registry.scenes.names[i]);
    }
    free(tuix_registry.scenes.scenes);
    free(tuix_registry.scenes.names);
    tuix_registry.scenes.count = 0;

    // Free subcycles registry
    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        TuixSceneSubcycles* scene_subs = tuix_registry.subcycles.subcycles[i];

        for (int j = 0; j < scene_subs->count; j++) {
            free(scene_subs->subcycles[j]);
        }

        free(scene_subs->subcycles);
        free(scene_subs->scene_name);
        free(scene_subs);
    }

    free(tuix_registry.subcycles.subcycles);
    tuix_registry.subcycles.count = 0;

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
    int response = tuix_destroy_registry();
    if (response != 0) {
        printf("Failed to destroy TUIX registry\n");
        return response;
    }
    return 0;
}