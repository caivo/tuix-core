#include "subcycle_registrant.h"
#include "../types.h"
#include "../tuix_registry.h"
#include "../buffers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int tuix_subcycle_init(char* scene_name, TuixObject *obj) {
    if (!obj || !obj->builder || !obj->builder->create_state || !obj->builder->destroy_state || !obj->builder->handler_func || !obj->builder->build_content) {
        return -1;
    }
    if (obj->state != NULL) {
        return -1;
    }

    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        TuixSceneSubcycles* scene_subcycles = tuix_registry.subcycles.subcycles[i];
        if (strcmp(scene_subcycles->scene_name, scene_name) == 0) {
            int new_count = scene_subcycles->count + 1;
            TuixSubcycle** tmp = realloc(scene_subcycles->subcycles, new_count * sizeof(TuixSubcycle*));
            if (!tmp) {
                printf("Memory allocation failed!\n");
                return -1;
            }
            scene_subcycles->subcycles = tmp;
            scene_subcycles->count++;
            TuixSubcycle* subcycle = malloc(sizeof(TuixSubcycle));
            if (!subcycle) {
                printf("Memory allocation failed!\n");
                return -1;
            }
            memset(subcycle, 0, sizeof(TuixSubcycle));

            subcycle->obj = obj;
            subcycle->handler = obj->builder->handler_func;
            subcycle->enabled = 1;
            scene_subcycles->subcycles[scene_subcycles->count - 1] = subcycle;
            subcycle->obj->state = subcycle->obj->builder->create_state(NULL);
            return 0;
        }
    }

    return -1;
}

void tuix_subcycle_free(char* scene_name, int uid) {
    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        TuixSceneSubcycles* scene_subcycles = tuix_registry.subcycles.subcycles[i];
        if (strcmp(scene_subcycles->scene_name, scene_name) == 0) {
            // Find and remove the subcycle with matching UID
            for (int j = 0; j < scene_subcycles->count; j++) {
                if (scene_subcycles->subcycles[j]->obj->uid == uid) {
                if (scene_subcycles->subcycles[j]->obj->state && 
                    scene_subcycles->subcycles[j]->obj->builder->destroy_state) {
                    scene_subcycles->subcycles[j]->obj->builder->destroy_state(scene_subcycles->subcycles[j]->obj->state);
                }

                free(scene_subcycles->subcycles[j]);

                for (int k = j; k < scene_subcycles->count - 1; k++) {
                        scene_subcycles->subcycles[k] = scene_subcycles->subcycles[k + 1];
                    }
                    
                    scene_subcycles->count--;
                    
                    if (scene_subcycles->count == 0) {
                        free(scene_subcycles->subcycles);
                        scene_subcycles->subcycles = NULL;
                    } else {
                        TuixSubcycle** tmp = realloc(scene_subcycles->subcycles, scene_subcycles->count * sizeof(TuixSubcycle*));
                        if (tmp) {
                            scene_subcycles->subcycles = tmp;
                        }
                    }
                    
                    return;
                }
            }
            return;
        }
    }
}