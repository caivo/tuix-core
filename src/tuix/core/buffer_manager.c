#include "types.h"
#include "buffers.h"
#include "tuix_registry.h"
#include "subcycles/subcycle_registrant.h"
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

void tuix_init_buffer(char* scene_name, TuixObject obj) {
    TuixScene* scene = tuix_get_scene(scene_name);
    if (!scene) {
        printf("Scene not found: %s\n", scene_name);
        return;
    }

    tuix_lock();
    /* Ensure scene->buffers has capacity (exponential growth). */
    if (scene->count + 1 > scene->capacity) {
        int new_cap = scene->capacity ? scene->capacity * 2 : 4;
        scene->buffers = safe_realloc(scene->buffers, sizeof(TuixBuffer*) * new_cap);
        scene->capacity = new_cap;
    }

    TuixBuffer* bf = calloc(1, sizeof(TuixBuffer));
    if (!bf) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    TuixObject* obj_copy = malloc(sizeof(TuixObject));
    if (!obj_copy) {
        printf("Memory allocation failed!\n");
        free(bf);
        exit(1);
    }
    *obj_copy = obj;
    
    bf->obj = obj_copy;
    bf->pixels = NULL;
    bf->height = 0;
    bf->width = 0;
    bf->required_redraw = 1;

    scene->buffers[scene->count] = bf;
    scene->count++;
    tuix_unlock();
}

TuixBuffer* tuix_get_buffer(char* scene_name, int uid) {
    TuixScene* scene = tuix_get_scene(scene_name);
    if (!scene) {
        return NULL;
    }
    tuix_lock();
    for (int i = 0; i < scene->count; i++) {
        if (scene->buffers[i]->obj->uid == uid) {
            TuixBuffer* found = scene->buffers[i];
            tuix_unlock();
            return found;
        }
    }
    tuix_unlock();
    return NULL;
}

TuixBuffer* tuix_get_buffer_by_uid(int uid) {
    tuix_lock();
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        TuixScene* scene = tuix_registry.scenes.scenes[i];
        for (int j = 0; j < scene->count; j++) {
            if (scene->buffers[j]->obj->uid == uid) {
                TuixBuffer* found = scene->buffers[j];
                tuix_unlock();
                return found;
            }
        }
    }
    tuix_unlock();
    return NULL;
}

void tuix_clear_buffer(char* scene_name, int uid) {
    TuixScene* scene = tuix_get_scene(scene_name);
    if (!scene) {
        printf("Scene not found: %s\n", scene_name);
        return;
    }
    tuix_lock();
    for (int i = 0; i < scene->count; i++) {
        if (scene->buffers[i]->obj->uid == uid) {
            if (scene->buffers[i]->pixels != NULL) {
                free(scene->buffers[i]->pixels);
                scene->buffers[i]->pixels = NULL;
            }
            scene->buffers[i]->height = 0;
            scene->buffers[i]->width = 0;
            scene->buffers[i]->required_redraw = 1;
            tuix_unlock();
            return;
        }
    }
    tuix_unlock();
    printf("Buffer not found: %d\n", uid);
}

void tuix_free_buffer(char* scene_name, int uid) {
    TuixScene* scene = tuix_get_scene(scene_name);
    if (!scene) {
        printf("Scene not found: %s\n", scene_name);
        return;
    }
    tuix_lock();
    if (scene->count == 0) {
        tuix_unlock();
        printf("There are no buffers available\n");
        return;
    }

    for (int i = 0; i < scene->count; i++) {

        if (scene->buffers[i]->obj->uid == uid) {
            tuix_subcycle_free(scene_name, uid);

            if (scene->buffers[i]->pixels)
                free(scene->buffers[i]->pixels);
            free(scene->buffers[i]->obj);
            free(scene->buffers[i]);

            for (int j = i; j < scene->count - 1; j++) {
                scene->buffers[j] = scene->buffers[j + 1];
            }

            scene->count--;

            if (scene->count == 0) {
                free(scene->buffers);
                scene->buffers = NULL;
                scene->capacity = 0;
                tuix_unlock();
                return;
            }
            /* Keep capacity unchanged to avoid frequent realloc/shrink cycles. */
            tuix_unlock();
            return;
        }
    }
    tuix_unlock();

    printf("Buffer not found: %d\n", uid);
}

int* tuix_get_buffer_size_by_uid(int uid) {
    TuixBuffer* buffer = tuix_get_buffer_by_uid(uid);
    if (!buffer) {
        printf("Buffer not found: %d\n", uid);
        return NULL;
    }
    int width = buffer->width;
    int height = buffer->height;
    int* data = (int*)malloc(2 * sizeof(int));
    if (!data) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    data[0] = width;
    data[1] = height;
    return data;
}

int* tuix_get_buffer_size_on_screen_by_uid(int uid) {
    int posx0 = 0, posy0 = 0, posx1 = 0, posy1 = 0;
    TuixBuffer* buffer = tuix_get_buffer_by_uid(uid);
    if (!buffer) {
        printf("Buffer not found: %d\n", uid);
        return NULL;
    }
    posx0 = buffer->margin_left;
    posy0 = buffer->margin_top;
    posx1 = posx0 + buffer->width;
    posy1 = posy0 + buffer->height;
    int* data = (int*)malloc(4 * sizeof(int));
    if (!data) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    data[0] = posx0;
    data[1] = posy0;
    data[2] = posx1;
    data[3] = posy1;
    return data;
}