#include "subcycle_registrant.h"
#include "../types.h"
#include "../tuix_registry.h"
#include "../buffers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int is_registered_builder_ptr(const TuixBuilder *builder) {
    if (!builder) return 0;
    for (int i = 0; i < tuix_registry.builders.count; i++) {
        if (tuix_registry.builders.builders[i] == builder) {
            return 1;
        }
    }
    return 0;
}

int tuix_subcycle_init(char* scene_name, TuixObject *obj) {
    if (!obj || !is_registered_builder_ptr(obj->builder)) {
        return -1;
    }
    if (!obj->builder->create_state || !obj->builder->destroy_state || !obj->builder->on_event || !obj->builder->build_content) {
        return -1;
    }
    if (obj->state != NULL) {
        return -1;
    }

    tuix_lock();
    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        TuixSceneSubcycles* scene_subcycles = tuix_registry.subcycles.subcycles[i];
        if (strcmp(scene_subcycles->scene_name, scene_name) == 0) {
            int new_count = scene_subcycles->count + 1;
            TuixSubcycle** tmp = realloc(scene_subcycles->subcycles, new_count * sizeof(TuixSubcycle*));
            TuixSubcycle* subcycle;
            if (!tmp) {
                tuix_unlock();
                printf("Memory allocation failed!\n");
                return -1;
            }
            scene_subcycles->subcycles = tmp;
            scene_subcycles->count++;
            subcycle = malloc(sizeof(TuixSubcycle));
            if (!subcycle) {
                tuix_unlock();
                printf("Memory allocation failed!\n");
                return -1;
            }
            memset(subcycle, 0, sizeof(TuixSubcycle));

            subcycle->obj = obj;
            subcycle->on_event = obj->builder->on_event;
            subcycle->enabled = 1;
            scene_subcycles->subcycles[scene_subcycles->count - 1] = subcycle;
            subcycle->obj->state = subcycle->obj->builder->create_state(NULL);
            tuix_unlock();
            return 0;
        }
    }

    tuix_unlock();
    return -1;
}

void tuix_subcycle_free(char* scene_name, int uid) {
    tuix_lock();
    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        TuixSceneSubcycles* scene_subcycles = tuix_registry.subcycles.subcycles[i];
        if (strcmp(scene_subcycles->scene_name, scene_name) == 0) {
            // Find and remove the subcycle with matching UID
            for (int j = 0; j < scene_subcycles->count; j++) {
                TuixSubcycle* sub = scene_subcycles->subcycles[j];
                if (!sub || !sub->obj) {
                    continue;
                }
                if (sub->obj->uid == uid) {
                    if (sub->obj->state && sub->obj->builder && sub->obj->builder->destroy_state) {
                        sub->obj->builder->destroy_state(sub->obj->state);
                        sub->obj->state = NULL;
                    }

                    free(sub);

                    for (int k = j; k < scene_subcycles->count - 1; k++) {
                        scene_subcycles->subcycles[k] = scene_subcycles->subcycles[k + 1];
                    }
                    
                    scene_subcycles->count--;
                    
                    if (scene_subcycles->count == 0) {
                        free(scene_subcycles->subcycles);
                        scene_subcycles->subcycles = NULL;
                    } else {
                        TuixSubcycle** tmp;
                        tmp = realloc(scene_subcycles->subcycles, scene_subcycles->count * sizeof(TuixSubcycle*));
                        if (tmp) {
                            scene_subcycles->subcycles = tmp;
                        }
                    }

                    tuix_unlock();
                    return;
                }
            }
            tuix_unlock();
            return;
        }
    }
    tuix_unlock();
}