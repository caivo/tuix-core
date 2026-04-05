#include "types.h"
#include "buffers.h"
#include "buffer_manager.h"
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

static TuixBuffer* find_buffer_in_scene_unlocked(TuixScene* scene, int uid) {
    if (!scene) {
        return NULL;
    }
    for (int i = 0; i < scene->count; i++) {
        TuixBuffer* b = scene->buffers[i];
        if (b && b->obj && b->obj->uid == uid) {
            return b;
        }
    }
    return NULL;
}

static TuixBuffer* find_buffer_unlocked(const char* scene_name, int uid) {
    TuixScene* scene = tuix_get_scene(scene_name);
    return find_buffer_in_scene_unlocked(scene, uid);
}

static TuixBuffer* find_buffer_by_uid_unlocked(int uid) {
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        TuixScene* scene = tuix_registry.scenes.scenes[i];
        TuixBuffer* b = find_buffer_in_scene_unlocked(scene, uid);
        if (b) {
            return b;
        }
    }
    return NULL;
}

static int parent_chain_contains_uid_locked(char* scene_name, int start_uid, int target_uid) {
    int guard = 0;
    int current = start_uid;
    while (current >= 0 && guard < 1024) {
        if (current == target_uid) {
            return 1;
        }
        TuixBuffer* node = find_buffer_unlocked(scene_name, current);
        if (!node) {
            break;
        }
        current = node->parent_uid;
        guard++;
    }
    return 0;
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
    bf->parent_uid = -1;
    bf->z_index = 0;

    scene->buffers[scene->count] = bf;
    scene->count++;
    tuix_unlock();
}

int tuix_get_buffer_snapshot(char* scene_name, int uid, TuixBuffer* out_buffer) {
    if (!out_buffer) {
        return -1;
    }
    tuix_lock();
    TuixBuffer* found = find_buffer_unlocked(scene_name, uid);
    if (!found) {
        tuix_unlock();
        return -1;
    }
    *out_buffer = *found;
    out_buffer->obj = NULL;
    out_buffer->pixels = NULL;
    tuix_unlock();
    return 0;
}

int tuix_get_buffer_snapshot_by_uid(int uid, TuixBuffer* out_buffer) {
    if (!out_buffer) {
        return -1;
    }
    tuix_lock();
    TuixBuffer* found = find_buffer_by_uid_unlocked(uid);
    if (!found) {
        tuix_unlock();
        return -1;
    }
    *out_buffer = *found;
    out_buffer->obj = NULL;
    out_buffer->pixels = NULL;
    tuix_unlock();
    return 0;
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

            int was_focused = (scene->current_focus == uid);

            int new_focus_uid = -1;

            if (was_focused && scene->count > 1) {
                int new_idx = (i - 1 + scene->count) % scene->count;
                if (new_idx == i) new_idx = (i + 1) % scene->count;

                new_focus_uid = scene->buffers[new_idx]->obj->uid;
            }

            int removed_parent_uid = scene->buffers[i]->parent_uid;
            for (int c = 0; c < scene->count; c++) {
                if (scene->buffers[c]->obj->uid == uid) {
                    continue;
                }
                if (scene->buffers[c]->parent_uid == uid) {
                    scene->buffers[c]->parent_uid = removed_parent_uid;
                    scene->buffers[c]->required_redraw = 1;
                }
            }

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
                scene->current_focus = -1;

                free(scene->buffers);
                scene->buffers = NULL;
                scene->capacity = 0;

                tuix_unlock();
                return;
            }

            if (was_focused) {
                scene->current_focus = new_focus_uid;
            }
            else {
                int found = 0;
                for (int k = 0; k < scene->count; k++) {
                    if (scene->buffers[k]->obj->uid == scene->current_focus) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    scene->current_focus = scene->buffers[0]->obj->uid;
                    printf("Focused object UID: %d\n", scene->current_focus);
                }
            }

            tuix_unlock();
            return;
        }
    }

    tuix_unlock();
    printf("Buffer not found: %d\n", uid);
}

int* tuix_get_buffer_size_by_uid(int uid) {
    TuixBuffer snap;
    if (tuix_get_buffer_snapshot_by_uid(uid, &snap) != 0) {
        printf("Buffer not found: %d\n", uid);
        return NULL;
    }
    int width = snap.width;
    int height = snap.height;
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
    TuixBuffer snap;
    if (tuix_get_buffer_snapshot_by_uid(uid, &snap) != 0) {
        printf("Buffer not found: %d\n", uid);
        return NULL;
    }
    posx0 = snap.margin_left;
    posy0 = snap.margin_top;
    posx1 = posx0 + snap.width;
    posy1 = posy0 + snap.height;
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

int tuix_set_buffer_parent(char* scene_name, int uid, int parent_uid) {
    tuix_lock();
    TuixBuffer* buffer = find_buffer_unlocked(scene_name, uid);
    if (!buffer) {
        tuix_unlock();
        printf("Buffer not found: %d\n", uid);
        return -1;
    }

    if (parent_uid < 0) {
        buffer->parent_uid = -1;
        buffer->required_redraw = 1;
        tuix_unlock();
        return 0;
    }

    if (uid == parent_uid) {
        tuix_unlock();
        printf("Invalid parent assignment: buffer %d cannot be its own parent\n", uid);
        return -1;
    }

    TuixBuffer* parent_buffer = find_buffer_unlocked(scene_name, parent_uid);
    if (!parent_buffer) {
        tuix_unlock();
        printf("Parent buffer not found: %d\n", parent_uid);
        return -1;
    }

    if (parent_chain_contains_uid_locked(scene_name, parent_uid, uid)) {
        tuix_unlock();
        printf("Invalid parent assignment: cycle detected (%d -> %d)\n", uid, parent_uid);
        return -1;
    }

    buffer->parent_uid = parent_uid;
    buffer->required_redraw = 1;
    tuix_unlock();
    return 0;
}

int tuix_get_buffer_parent(char* scene_name, int uid) {
    tuix_lock();
    TuixBuffer* buffer = find_buffer_unlocked(scene_name, uid);
    if (!buffer) {
        tuix_unlock();
        printf("Buffer not found: %d\n", uid);
        return -1;
    }
    int parent_uid = buffer->parent_uid;
    tuix_unlock();
    return parent_uid;
}

int tuix_set_buffer_z_index(char* scene_name, int uid, int z_index) {
    tuix_lock();
    TuixBuffer* buffer = find_buffer_unlocked(scene_name, uid);
    if (!buffer) {
        tuix_unlock();
        printf("Buffer not found: %d\n", uid);
        return -1;
    }
    buffer->z_index = z_index;
    buffer->required_redraw = 1;
    tuix_unlock();
    return 0;
}

int tuix_get_buffer_z_index(char* scene_name, int uid) {
    tuix_lock();
    TuixBuffer* buffer = find_buffer_unlocked(scene_name, uid);
    if (!buffer) {
        tuix_unlock();
        printf("Buffer not found: %d\n", uid);
        return 0;
    }
    int z = buffer->z_index;
    tuix_unlock();
    return z;
}

/* Test-only: Get the raw object by UID. Used for testing object methods. */
TuixObject* _tuix_get_object_by_uid(int uid) {
    tuix_lock();
    TuixBuffer* buffer = find_buffer_by_uid_unlocked(uid);
    if (!buffer || !buffer->obj) {
        tuix_unlock();
        return NULL;
    }
    TuixObject* obj = buffer->obj;
    tuix_unlock();
    return obj;
}