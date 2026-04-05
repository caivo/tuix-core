#include "types.h"
#include "tuix_registry.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static size_t scene_pixel_bytes(const TuixScene* scene);
static size_t compact_scene_pixels_locked(TuixScene* scene);

static void* safe_realloc(void* ptr, size_t size) {
    void* tmp = realloc(ptr, size);
    if (!tmp && size != 0) {
        printf("Memory reallocation failed!\n");
        exit(1);
    }
    return tmp;
}

static int find_scene_index(const char* name) {
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        if (strcmp(tuix_registry.scenes.names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

static int find_subcycles_index(const char* name) {
    for (int i = 0; i < tuix_registry.subcycles.count; i++) {
        if (strcmp(tuix_registry.subcycles.subcycles[i]->scene_name, name) == 0) {
            return i;
        }
    }
    return -1;
}

static void clear_scene_subcycles_content(TuixSceneSubcycles* scene_subcycles) {
    if (!scene_subcycles) return;

    for (int i = 0; i < scene_subcycles->count; i++) {
        TuixSubcycle* sub = scene_subcycles->subcycles[i];
        if (!sub) continue;

        if (sub->obj && sub->obj->state && sub->obj->builder && sub->obj->builder->destroy_state) {
            sub->obj->builder->destroy_state(sub->obj->state);
            sub->obj->state = NULL;
        }

        free(sub);
    }

    free(scene_subcycles->subcycles);
    scene_subcycles->subcycles = NULL;
    scene_subcycles->count = 0;
    scene_subcycles->capacity = 0;
}

static void clear_scene_buffers_content(TuixScene* scene) {
    if (!scene) return;

    for (int i = 0; i < scene->count; i++) {
        TuixBuffer* buf = scene->buffers[i];
        if (!buf) continue;
        free(buf->pixels);
        free(buf->obj);
        free(buf);
    }

    free(scene->buffers);
    scene->buffers = NULL;
    scene->count = 0;
    scene->capacity = 0;
    scene->current_focus = -1;
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
    sc->current_focus = -1;
    sc->last_active_frame = tuix_registry.frame_counter;
    sc->last_compacted_frame = 0;

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
    int scene_idx = find_scene_index(name);
    if (scene_idx == -1) {
        tuix_unlock();
        printf("Scene not found: %s\n", name);
        return;
    }

    int sub_idx = find_subcycles_index(name);
    if (sub_idx != -1) {
        TuixSceneSubcycles* scene_subcycles = tuix_registry.subcycles.subcycles[sub_idx];
        clear_scene_subcycles_content(scene_subcycles);
        free(scene_subcycles->scene_name);
        free(scene_subcycles);

        for (int j = sub_idx; j < tuix_registry.subcycles.count - 1; j++) {
            tuix_registry.subcycles.subcycles[j] = tuix_registry.subcycles.subcycles[j + 1];
        }
        tuix_registry.subcycles.count--;

        if (tuix_registry.subcycles.count == 0) {
            free(tuix_registry.subcycles.subcycles);
            tuix_registry.subcycles.subcycles = NULL;
            tuix_registry.subcycles.capacity = 0;
        }
    }

    TuixScene* scene = tuix_registry.scenes.scenes[scene_idx];
    clear_scene_buffers_content(scene);
    free(scene);
    free(tuix_registry.scenes.names[scene_idx]);

    for (int j = scene_idx; j < tuix_registry.scenes.count - 1; j++) {
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
    }

    if (tuix_registry.current_scene_name && strcmp(tuix_registry.current_scene_name, name) == 0) {
        tuix_registry.current_scene_name = NULL;
    }

    tuix_unlock();
}



void tuix_clear_scene(const char* name) {
    tuix_lock();

    int scene_idx = find_scene_index(name);
    if (scene_idx == -1) {
        tuix_unlock();
        printf("Scene not found: %s\n", name);
        return;
    }

    int sub_idx = find_subcycles_index(name);
    if (sub_idx != -1) {
        clear_scene_subcycles_content(tuix_registry.subcycles.subcycles[sub_idx]);
    }

    clear_scene_buffers_content(tuix_registry.scenes.scenes[scene_idx]);
    tuix_unlock();
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

int tuix_select_scene(const char* name) {
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        if (strcmp(tuix_registry.scenes.names[i], name) == 0) {
            tuix_registry.scenes.scenes[i]->active = 1;
            tuix_registry.scenes.scenes[i]->last_active_frame = tuix_registry.frame_counter;
            tuix_registry.current_scene_name = tuix_registry.scenes.names[i];
            return 0;
        } else {
            tuix_registry.scenes.scenes[i]->active = 0;
        }
    }
    return -1;
}

int tuix_scene_set_focus(const char* scene_name, int uid) {
    tuix_lock();
    TuixScene* scene = tuix_get_scene(scene_name);
    if (!scene) {
        tuix_unlock();
        printf("Scene not found: %s\n", scene_name);
        return -1;
    }
    for (int i = 0; i < scene->count; i++) {
        if (scene->buffers[i]->obj->uid == uid) {
            scene->current_focus = scene->buffers[i]->obj->uid;
            tuix_unlock();
            return 0;
        }
    }
    tuix_unlock();
    printf("Object not found in scene: %s\n", scene_name);
    return -1;
}

int tuix_scene_set_previous_focus(const char* scene_name) {
    tuix_lock();
    TuixScene* scene = tuix_get_scene(scene_name);
    if (!scene || scene->count == 0) {
        if (scene) scene->current_focus = -1;
        tuix_unlock();
        return -1;
    }

    int current_idx = -1;

    for (int i = 0; i < scene->count; i++) {
        if (scene->buffers[i]->obj->uid == scene->current_focus) {
            current_idx = i;
            break;
        }
    }

    if (current_idx == -1) {
        scene->current_focus = scene->buffers[0]->obj->uid;
        tuix_unlock();
        return 0;
    }

    int new_idx = (current_idx - 1 + scene->count) % scene->count;
    scene->current_focus = scene->buffers[new_idx]->obj->uid;
    tuix_unlock();
    return 0;
}

int tuix_scene_get_stats(const char* scene_name, TuixSceneStats* out_stats) {
    if (!scene_name || !out_stats) {
        return -1;
    }
    tuix_lock();
    int scene_idx = find_scene_index(scene_name);
    if (scene_idx == -1) {
        tuix_unlock();
        return -1;
    }
    TuixScene* scene = tuix_registry.scenes.scenes[scene_idx];
    memset(out_stats, 0, sizeof(*out_stats));
    out_stats->buffer_count = scene->count;
    out_stats->active = scene->active;
    out_stats->current_focus = scene->current_focus;
    out_stats->last_active_frame = scene->last_active_frame;
    out_stats->last_compacted_frame = scene->last_compacted_frame;
    out_stats->pixel_bytes = scene_pixel_bytes(scene);
    out_stats->approx_heap_bytes = out_stats->pixel_bytes +
                                   (size_t)scene->count * (sizeof(TuixBuffer) + sizeof(TuixObject));
    tuix_unlock();
    return 0;
}

size_t tuix_compact_scene_pixels(const char* scene_name) {
    if (!scene_name) {
        return 0;
    }
    tuix_lock();
    int scene_idx = find_scene_index(scene_name);
    if (scene_idx == -1) {
        tuix_unlock();
        return 0;
    }
    size_t freed = compact_scene_pixels_locked(tuix_registry.scenes.scenes[scene_idx]);
    tuix_unlock();
    return freed;
}

int tuix_compact_cold_scenes(unsigned long long cold_frames, size_t min_pixel_bytes, int keep_active_scene) {
    int compacted = 0;
    tuix_lock();
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        TuixScene* scene = tuix_registry.scenes.scenes[i];
        const char* scene_name = tuix_registry.scenes.names[i];
        if (!scene) {
            continue;
        }
        if (keep_active_scene && tuix_registry.current_scene_name && scene_name &&
            strcmp(scene_name, tuix_registry.current_scene_name) == 0) {
            continue;
        }

        unsigned long long age = 0;
        if (tuix_registry.frame_counter >= scene->last_active_frame) {
            age = tuix_registry.frame_counter - scene->last_active_frame;
        }
        if (age < cold_frames) {
            continue;
        }

        size_t px_bytes = scene_pixel_bytes(scene);
        if (px_bytes < min_pixel_bytes) {
            continue;
        }

        if (compact_scene_pixels_locked(scene) > 0) {
            compacted++;
        }
    }
    tuix_unlock();
    return compacted;
}

static size_t scene_pixel_bytes(const TuixScene* scene) {
    size_t bytes = 0;
    if (!scene) {
        return 0;
    }
    for (int i = 0; i < scene->count; i++) {
        TuixBuffer* buf = scene->buffers[i];
        if (!buf || !buf->pixels) {
            continue;
        }
        if (buf->width <= 0 || buf->height <= 0) {
            continue;
        }
        bytes += (size_t)buf->width * (size_t)buf->height * sizeof(TuixPixel);
    }
    return bytes;
}

static size_t compact_scene_pixels_locked(TuixScene* scene) {
    size_t freed = 0;
    if (!scene) {
        return 0;
    }
    for (int i = 0; i < scene->count; i++) {
        TuixBuffer* buf = scene->buffers[i];
        if (!buf || !buf->pixels) {
            continue;
        }
        if (buf->width > 0 && buf->height > 0) {
            freed += (size_t)buf->width * (size_t)buf->height * sizeof(TuixPixel);
        }
        free(buf->pixels);
        buf->pixels = NULL;
        buf->required_redraw = 1;
    }
    scene->last_compacted_frame = tuix_registry.frame_counter;
    return freed;
}