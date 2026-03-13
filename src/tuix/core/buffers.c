#include "types.h"
#include "tuix_registry.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void* safe_realloc(void* ptr, size_t size) {
    void* tmp = realloc(ptr, size);
    if (!tmp && size != 0) {
        printf("Memory reallocation failed!\n");
        exit(1);
    }
    return tmp;
}


int tuix_init_scene(const char* name) {
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        if (strcmp(tuix_registry.scenes.names[i], name) == 0) {
            printf("Scene with this name already exists\n");
            return 1;
        }
    }

    tuix_lock();
    /* Ensure capacity using exponential growth to avoid frequent reallocs. */
    if (tuix_registry.scenes.count + 1 > tuix_registry.scenes.capacity) {
        int new_cap = tuix_registry.scenes.capacity ? tuix_registry.scenes.capacity * 2 : 4;
        tuix_registry.scenes.scenes = safe_realloc(tuix_registry.scenes.scenes, sizeof(TuixScene*) * new_cap);
        tuix_registry.scenes.names  = safe_realloc(tuix_registry.scenes.names,  sizeof(char*)      * new_cap);
        tuix_registry.scenes.capacity = new_cap;
    }

    TuixScene* sc = calloc(1, sizeof(TuixScene));
    if (!sc) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    TuixSceneSubcycles* scene_subcycles = calloc(1, sizeof(TuixSceneSubcycles));
    if (!scene_subcycles) {
        printf("Memory allocation failed!\n");
        free(sc);
        exit(1);
    }
    scene_subcycles->scene_name = strdup(name);
    scene_subcycles->subcycles = NULL;
    scene_subcycles->count = 0;

    tuix_registry.scenes.scenes[tuix_registry.scenes.count] = sc;
    tuix_registry.scenes.names[tuix_registry.scenes.count]  = strdup(name);
    tuix_registry.scenes.count++;

    if (tuix_registry.subcycles.count + 1 > tuix_registry.subcycles.capacity) {
        int new_cap = tuix_registry.subcycles.capacity ? tuix_registry.subcycles.capacity * 2 : 4;
        tuix_registry.subcycles.subcycles = safe_realloc(tuix_registry.subcycles.subcycles, sizeof(TuixSceneSubcycles*) * new_cap);
        tuix_registry.subcycles.capacity = new_cap;
    }
    tuix_registry.subcycles.subcycles[tuix_registry.subcycles.count] = scene_subcycles;
    tuix_registry.subcycles.count++;
    tuix_unlock();
    return 0;
}



void tuix_free_scene(const char* name) {
    if (tuix_registry.scenes.count == 0) {
        printf("There are no scenes available\n");
        return;
    }
    tuix_lock();
    for (int i = 0; i < tuix_registry.scenes.count; i++) {

        if (strcmp(tuix_registry.scenes.names[i], name) == 0) {
            // Free all buffers in the scene
            for (int j = 0; j < tuix_registry.scenes.scenes[i]->count; j++) {
                TuixBuffer* buf = tuix_registry.scenes.scenes[i]->buffers[j];
                if (buf) {
                    // Free pixel data
                    free(buf->pixels);
                    free(buf->obj);
                    free(buf);
                }
            }

            free(tuix_registry.scenes.scenes[i]->buffers);
            free(tuix_registry.scenes.scenes[i]);
            free(tuix_registry.scenes.names[i]);

            for (int j = i; j < tuix_registry.scenes.count - 1; j++) {
                tuix_registry.scenes.scenes[j] = tuix_registry.scenes.scenes[j + 1];
                tuix_registry.scenes.names[j]  = tuix_registry.scenes.names[j + 1];
            }

            tuix_registry.scenes.count--;

            if (tuix_registry.scenes.count == 0) {
                free(tuix_registry.scenes.scenes);
                free(tuix_registry.scenes.names);
                tuix_registry.scenes.scenes = NULL;
                tuix_registry.scenes.names = NULL;
                tuix_registry.scenes.capacity = 0;
                tuix_unlock();
                return;
            }
            /* Keep capacity as-is to avoid frequent realloc/shrink cycles. */
            tuix_unlock();
            return;
        }
    }
    tuix_unlock();
    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        if (strcmp(tuix_registry.subcycles.subcycles[i]->scene_name, name) == 0) {
            for (int j = 0; j < tuix_registry.subcycles.subcycles[i]->count; j++) {
                free(tuix_registry.subcycles.subcycles[i]->subcycles[j]);
            }
            free(tuix_registry.subcycles.subcycles[i]->subcycles);
            free(tuix_registry.subcycles.subcycles[i]->scene_name);
            free(tuix_registry.subcycles.subcycles[i]);

            for (int j = i; j < tuix_registry.subcycles.count - 1; j++) {
                tuix_registry.subcycles.subcycles[j] = tuix_registry.subcycles.subcycles[j + 1];
            }

            tuix_registry.subcycles.count--;

            if (tuix_registry.subcycles.count == 0) {
                free(tuix_registry.subcycles.subcycles);
                tuix_registry.subcycles.subcycles = NULL;
                tuix_registry.subcycles.capacity = 0;
                return;
            }
            /* Keep capacity as-is to avoid frequent realloc/shrink cycles. */
            return;
        }
    }

    printf("Scene not found: %s\n", name);
}



void tuix_clear_scene(const char* name) {
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        if (strcmp(tuix_registry.scenes.names[i], name) == 0) {
            for (int j = 0; j < tuix_registry.scenes.scenes[i]->count; j++) {
                TuixBuffer* buf = tuix_registry.scenes.scenes[i]->buffers[j];
                if (buf) {
                    free(buf->pixels);
                    free(buf->obj);
                    free(buf);
                }
            }

            free(tuix_registry.scenes.scenes[i]->buffers);
            tuix_registry.scenes.scenes[i]->buffers = NULL;
            tuix_registry.scenes.scenes[i]->count = 0;

            return;
        }
    }

    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        if (strcmp(tuix_registry.subcycles.subcycles[i]->scene_name, name) == 0) {
            for (int j = 0; j < tuix_registry.subcycles.subcycles[i]->count; j++) {
                free(tuix_registry.subcycles.subcycles[i]->subcycles[j]);
            }

            free(tuix_registry.subcycles.subcycles[i]->subcycles);
            tuix_registry.subcycles.subcycles[i]->subcycles = NULL;
            tuix_registry.subcycles.subcycles[i]->count = 0;

            return;
        }
    }

    printf("Scene not found: %s\n", name);
}



char** tuix_get_scenes() {
    if (tuix_registry.scenes.count == 0) {
        return NULL;
    }

    char** list = malloc(sizeof(char*) * tuix_registry.scenes.count);
    if (!list) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        list[i] = strdup(tuix_registry.scenes.names[i]);
    }

    return list;
}



TuixScene* tuix_get_scene(const char* name) {
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        if (strcmp(tuix_registry.scenes.names[i], name) == 0) {
            return tuix_registry.scenes.scenes[i];
        }
    }
    return NULL;
}

int tuix_select_scene(char* name) {
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        if (strcmp(tuix_registry.scenes.names[i], name) == 0) {
            tuix_registry.scenes.scenes[i]->active = 1;
            tuix_registry.current_scene_name = tuix_registry.scenes.names[i];
            return 0;
        } else {
            tuix_registry.scenes.scenes[i]->active = 0;
        }
    }
    return -1;
}