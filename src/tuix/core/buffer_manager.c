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
    if (!scene || !scene->buffer_by_uid || uid <= 0 || uid >= scene->max_uid_capacity) {
        return NULL;
    }
    return scene->buffer_by_uid[uid];
}

static TuixBuffer* find_buffer_unlocked(const char* scene_name, int uid) {
    TuixScene* scene = tuix_get_scene(scene_name);
    return find_buffer_in_scene_unlocked(scene, uid);
}

static TuixScene* find_scene_by_uid_unlocked(int uid) {
    if (uid <= 0) {
        return NULL;
    }
    for (int i = 0; i < tuix_registry.scenes.count; i++) {
        TuixScene* scene = tuix_registry.scenes.scenes[i];
        if (!scene || !scene->buffer_by_uid || uid >= scene->max_uid_capacity) {
            continue;
        }
        if (scene->buffer_by_uid[uid]) {
            return scene;
        }
    }
    return NULL;
}

static TuixBuffer* find_buffer_by_uid_unlocked(int uid) {
    TuixScene* scene = find_scene_by_uid_unlocked(uid);
    return find_buffer_in_scene_unlocked(scene, uid);
}

static int ensure_scene_uid_capacity_unlocked(TuixScene* scene, int uid) {
    if (!scene || uid < 0) {
        return -1;
    }
    if (uid < scene->max_uid_capacity) {
        return 0;
    }

    int old_cap = scene->max_uid_capacity;
    int new_cap = old_cap > 0 ? old_cap : 8;
    while (new_cap <= uid) {
        if (new_cap > uid / 2) {
            new_cap = uid + 1;
            break;
        }
        new_cap *= 2;
    }

    scene->buffer_by_uid = safe_realloc(scene->buffer_by_uid, sizeof(TuixBuffer*) * (size_t)new_cap);
    memset(scene->buffer_by_uid + old_cap, 0, sizeof(TuixBuffer*) * (size_t)(new_cap - old_cap));
    scene->max_uid_capacity = new_cap;
    return 0;
}

static int ensure_uid_list_capacity_unlocked(int** items, int* capacity, int needed) {
    if (needed <= *capacity) {
        return 0;
    }

    int new_cap = *capacity > 0 ? *capacity : 4;
    while (new_cap < needed) {
        new_cap *= 2;
    }

    *items = safe_realloc(*items, sizeof(int) * (size_t)new_cap);
    *capacity = new_cap;
    return 0;
}

static int remove_uid_from_list_unlocked(int* items, int* count, int uid) {
    if (!items || !count || *count <= 0) {
        return 0;
    }

    for (int i = 0; i < *count; i++) {
        if (items[i] != uid) {
            continue;
        }
        for (int j = i; j < *count - 1; j++) {
            items[j] = items[j + 1];
        }
        (*count)--;
        return 1;
    }
    return 0;
}

static int insert_uid_sorted_unlocked(int** items, int* count, int* capacity, TuixScene* scene, int uid) {
    if (!items || !count || !capacity || !scene || uid <= 0) {
        return -1;
    }

    for (int i = 0; i < *count; i++) {
        if ((*items)[i] == uid) {
            return 0;
        }
    }

    ensure_uid_list_capacity_unlocked(items, capacity, *count + 1);

    int z_index = 0;
    TuixBuffer* target = find_buffer_in_scene_unlocked(scene, uid);
    if (target) {
        z_index = target->z_index;
    }

    int pos = *count;
    while (pos > 0) {
        int current_uid = (*items)[pos - 1];
        TuixBuffer* current = find_buffer_in_scene_unlocked(scene, current_uid);
        int current_z = current ? current->z_index : 0;
        if (current_z > z_index) {
            (*items)[pos] = (*items)[pos - 1];
            pos--;
            continue;
        }
        break;
    }

    (*items)[pos] = uid;
    (*count)++;
    return 0;
}

static void detach_uid_from_parent_or_root_unlocked(TuixScene* scene, int parent_uid, int uid) {
    if (!scene || uid <= 0) {
        return;
    }

    if (parent_uid < 0) {
        remove_uid_from_list_unlocked(scene->root_uids, &scene->root_count, uid);
        return;
    }

    TuixBuffer* parent = find_buffer_in_scene_unlocked(scene, parent_uid);
    if (!parent) {
        remove_uid_from_list_unlocked(scene->root_uids, &scene->root_count, uid);
        return;
    }

    remove_uid_from_list_unlocked(parent->children_uids, &parent->children_count, uid);
}

static int attach_uid_to_parent_or_root_unlocked(TuixScene* scene, int parent_uid, int uid) {
    if (!scene || uid <= 0) {
        return -1;
    }

    if (parent_uid < 0) {
        return insert_uid_sorted_unlocked(&scene->root_uids, &scene->root_count, &scene->root_capacity, scene, uid);
    }

    TuixBuffer* parent = find_buffer_in_scene_unlocked(scene, parent_uid);
    if (!parent) {
        return insert_uid_sorted_unlocked(&scene->root_uids, &scene->root_count, &scene->root_capacity, scene, uid);
    }

    return insert_uid_sorted_unlocked(&parent->children_uids, &parent->children_count, &parent->children_capacity, scene, uid);
}

static void mark_buffer_geometry_dirty_unlocked(TuixBuffer* buffer) {
    if (!buffer) {
        return;
    }
    buffer->required_redraw = 1;
    buffer->width = 0;
    buffer->height = 0;
}

static void mark_subtree_geometry_dirty_unlocked(TuixScene* scene, int uid) {
    TuixBuffer* buffer = find_buffer_in_scene_unlocked(scene, uid);
    if (!buffer) {
        return;
    }

    mark_buffer_geometry_dirty_unlocked(buffer);
    for (int i = 0; i < buffer->children_count; i++) {
        mark_subtree_geometry_dirty_unlocked(scene, buffer->children_uids[i]);
    }
}

static void mark_scene_topology_changed_unlocked(TuixScene* scene) {
    if (!scene) {
        return;
    }
    if (scene->transaction_depth > 0) {
        scene->topology_dirty = 1;
        return;
    }
    scene->topology_dirty = 0;
    scene->topology_version++;
    scene->last_composited_topology_version = 0;
}

static void mark_parent_layout_redraw_unlocked(TuixScene* scene, int parent_uid) {
    if (!scene || parent_uid < 0) {
        return;
    }
    TuixBuffer* parent = find_buffer_in_scene_unlocked(scene, parent_uid);
    if (!parent) {
        return;
    }
    parent->required_redraw = 1;
}

static TuixLayoutSlot default_layout_slot(void) {
    TuixLayoutSlot slot;
    slot.grow = 0.0f;
    slot.shrink = 1.0f;
    slot.basis = TUIX_LAYOUT_BASIS_AUTO;
    slot.min_w = 0;
    slot.min_h = 0;
    slot.max_w = TUIX_LAYOUT_UNBOUNDED;
    slot.max_h = TUIX_LAYOUT_UNBOUNDED;
    slot.align_self = TUIX_LAYOUT_ALIGN_AUTO;
    slot.grid_row = 0;
    slot.grid_col = 0;
    slot.row_span = 1;
    slot.col_span = 1;
    return slot;
}

static int layout_slot_equals(const TuixLayoutSlot* a, const TuixLayoutSlot* b) {
    if (!a || !b) {
        return 0;
    }
    return a->grow == b->grow &&
           a->shrink == b->shrink &&
           a->basis == b->basis &&
           a->min_w == b->min_w &&
           a->min_h == b->min_h &&
           a->max_w == b->max_w &&
           a->max_h == b->max_h &&
           a->align_self == b->align_self &&
           a->grid_row == b->grid_row &&
           a->grid_col == b->grid_col &&
           a->row_span == b->row_span &&
           a->col_span == b->col_span;
}

static int layout_rect_equals(const TuixLayoutRect* rect, int active, int offset_left, int offset_top, int width, int height) {
    if (!rect) {
        return 0;
    }
    return rect->active == active &&
           rect->offset_left == offset_left &&
           rect->offset_top == offset_top &&
           rect->width == width &&
           rect->height == height;
}

static int parent_chain_contains_uid_locked(TuixScene* scene, int start_uid, int target_uid) {
    int guard = 0;
    int current = start_uid;
    while (current >= 0 && guard < 1024) {
        if (current == target_uid) {
            return 1;
        }
        TuixBuffer* node = find_buffer_in_scene_unlocked(scene, current);
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
    if (scene->count + 1 > scene->capacity) {
        int new_cap = scene->capacity ? scene->capacity * 2 : 4;
        scene->buffers = safe_realloc(scene->buffers, sizeof(TuixBuffer*) * (size_t)new_cap);
        scene->capacity = new_cap;
    }
    ensure_scene_uid_capacity_unlocked(scene, obj.uid);

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
    bf->pixels_owned = 0;
    bf->height = 0;
    bf->width = 0;
    bf->required_redraw = 1;
    bf->parent_uid = -1;
    bf->z_index = 0;
    bf->flat_index = scene->count;
    bf->children_uids = NULL;
    bf->children_count = 0;
    bf->children_capacity = 0;
    bf->layout_slot = default_layout_slot();
    memset(&bf->layout_rect, 0, sizeof(bf->layout_rect));

    scene->buffers[bf->flat_index] = bf;
    scene->buffer_by_uid[obj.uid] = bf;
    attach_uid_to_parent_or_root_unlocked(scene, -1, obj.uid);
    scene->count++;
    mark_scene_topology_changed_unlocked(scene);
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
    out_buffer->children_uids = NULL;
    out_buffer->children_count = 0;
    out_buffer->children_capacity = 0;
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
    out_buffer->children_uids = NULL;
    out_buffer->children_count = 0;
    out_buffer->children_capacity = 0;
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
    TuixBuffer* buffer = find_buffer_in_scene_unlocked(scene, uid);
    if (buffer) {
        if (buffer->pixels != NULL) {
            if (buffer->pixels_owned) {
                free(buffer->pixels);
            }
            buffer->pixels = NULL;
            buffer->pixels_owned = 0;
        }
        buffer->height = 0;
        buffer->width = 0;
        buffer->required_redraw = 1;
        tuix_unlock();
        return;
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

    TuixBuffer* removed = find_buffer_in_scene_unlocked(scene, uid);
    if (removed && removed->obj) {
        int idx = removed->flat_index;
        int was_focused = (scene->current_focus == uid);
        int was_modal = (scene->active_modal_uid == uid);
        int restore_focus_uid = scene->modal_restore_focus_uid;
        int removed_parent_uid = removed->parent_uid;

        detach_uid_from_parent_or_root_unlocked(scene, removed_parent_uid, uid);
        mark_parent_layout_redraw_unlocked(scene, removed_parent_uid);

        for (int c = 0; c < removed->children_count; c++) {
            int child_uid = removed->children_uids[c];
            TuixBuffer* child = find_buffer_in_scene_unlocked(scene, child_uid);
            if (!child) {
                continue;
            }
            child->parent_uid = removed_parent_uid;
            memset(&child->layout_rect, 0, sizeof(child->layout_rect));
            attach_uid_to_parent_or_root_unlocked(scene, removed_parent_uid, child_uid);
            mark_subtree_geometry_dirty_unlocked(scene, child_uid);
        }

        tuix_subcycle_free(scene_name, uid);

        if (scene->buffer_by_uid && uid > 0 && uid < scene->max_uid_capacity) {
            scene->buffer_by_uid[uid] = NULL;
        }

        scene->count--;
        if (idx >= 0 && idx <= scene->count) {
            TuixBuffer* moved = scene->buffers[scene->count];
            scene->buffers[idx] = moved;
            if (moved) {
                moved->flat_index = idx;
            }
        }
        if (scene->capacity > 0) {
            scene->buffers[scene->count] = NULL;
        }

        if (removed->pixels) {
            if (removed->pixels_owned) {
                free(removed->pixels);
            }
            removed->pixels = NULL;
            removed->pixels_owned = 0;
        }
        free(removed->children_uids);
        free(removed->obj);
        free(removed);

        mark_scene_topology_changed_unlocked(scene);

        if (scene->count == 0) {
            scene->current_focus = -1;
            scene->active_modal_uid = -1;
            scene->modal_restore_focus_uid = -1;
            tuix_unlock();
            return;
        }

        if (was_focused || !find_buffer_in_scene_unlocked(scene, scene->current_focus)) {
            int focus_idx = idx < scene->count ? idx : 0;
            scene->current_focus = scene->buffers[focus_idx]->obj->uid;
        }
        if (was_modal) {
            scene->active_modal_uid = -1;
            scene->modal_restore_focus_uid = -1;
            if (restore_focus_uid > 0 && find_buffer_in_scene_unlocked(scene, restore_focus_uid)) {
                scene->current_focus = restore_focus_uid;
            }
        } else if (scene->modal_restore_focus_uid == uid) {
            scene->modal_restore_focus_uid = -1;
        }

        tuix_unlock();
        return;
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
    TuixScene* scene = tuix_get_scene(scene_name);
    TuixBuffer* buffer = find_buffer_in_scene_unlocked(scene, uid);
    if (!buffer) {
        tuix_unlock();
        printf("Buffer not found: %d\n", uid);
        return -1;
    }

    if (parent_uid < 0) {
        int old_parent_uid = buffer->parent_uid;
        if (buffer->parent_uid == -1) {
            tuix_unlock();
            return 0;
        }
        detach_uid_from_parent_or_root_unlocked(scene, old_parent_uid, uid);
        buffer->parent_uid = -1;
        memset(&buffer->layout_rect, 0, sizeof(buffer->layout_rect));
        attach_uid_to_parent_or_root_unlocked(scene, -1, uid);
        mark_subtree_geometry_dirty_unlocked(scene, uid);
        mark_parent_layout_redraw_unlocked(scene, old_parent_uid);
        mark_scene_topology_changed_unlocked(scene);
        tuix_unlock();
        return 0;
    }

    if (uid == parent_uid) {
        tuix_unlock();
        printf("Invalid parent assignment: buffer %d cannot be its own parent\n", uid);
        return -1;
    }

    TuixBuffer* parent_buffer = find_buffer_in_scene_unlocked(scene, parent_uid);
    if (!parent_buffer) {
        tuix_unlock();
        printf("Parent buffer not found: %d\n", parent_uid);
        return -1;
    }

    if (parent_chain_contains_uid_locked(scene, parent_uid, uid)) {
        tuix_unlock();
        printf("Invalid parent assignment: cycle detected (%d -> %d)\n", uid, parent_uid);
        return -1;
    }

    if (buffer->parent_uid == parent_uid) {
        tuix_unlock();
        return 0;
    }

    int old_parent_uid = buffer->parent_uid;
    detach_uid_from_parent_or_root_unlocked(scene, old_parent_uid, uid);
    buffer->parent_uid = parent_uid;
    memset(&buffer->layout_rect, 0, sizeof(buffer->layout_rect));
    attach_uid_to_parent_or_root_unlocked(scene, parent_uid, uid);
    mark_subtree_geometry_dirty_unlocked(scene, uid);
    mark_parent_layout_redraw_unlocked(scene, old_parent_uid);
    mark_parent_layout_redraw_unlocked(scene, parent_uid);
    mark_scene_topology_changed_unlocked(scene);
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
    TuixScene* scene = tuix_get_scene(scene_name);
    TuixBuffer* buffer = find_buffer_in_scene_unlocked(scene, uid);
    if (!buffer) {
        tuix_unlock();
        printf("Buffer not found: %d\n", uid);
        return -1;
    }
    if (buffer->z_index == z_index) {
        tuix_unlock();
        return 0;
    }

    detach_uid_from_parent_or_root_unlocked(scene, buffer->parent_uid, uid);
    buffer->z_index = z_index;
    attach_uid_to_parent_or_root_unlocked(scene, buffer->parent_uid, uid);
    mark_scene_topology_changed_unlocked(scene);
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

void tuix_mark_buffer_children_geometry_dirty_by_uid(int uid) {
    tuix_lock();
    TuixScene* scene = find_scene_by_uid_unlocked(uid);
    TuixBuffer* buffer = find_buffer_in_scene_unlocked(scene, uid);
    if (buffer) {
        for (int i = 0; i < buffer->children_count; i++) {
            mark_subtree_geometry_dirty_unlocked(scene, buffer->children_uids[i]);
        }
    }
    tuix_unlock();
}

int tuix_set_buffer_layout_slot_by_uid(int uid, const TuixLayoutSlot* slot) {
    if (uid <= 0 || !slot) {
        return -1;
    }

    tuix_lock();
    TuixScene* scene = find_scene_by_uid_unlocked(uid);
    TuixBuffer* buffer = find_buffer_in_scene_unlocked(scene, uid);
    if (!buffer) {
        tuix_unlock();
        return -1;
    }

    if (layout_slot_equals(&buffer->layout_slot, slot)) {
        tuix_unlock();
        return 0;
    }

    buffer->layout_slot = *slot;
    mark_subtree_geometry_dirty_unlocked(scene, uid);
    if (buffer->parent_uid >= 0) {
        TuixBuffer* parent = find_buffer_in_scene_unlocked(scene, buffer->parent_uid);
        if (parent) {
            parent->required_redraw = 1;
        }
    }
    tuix_unlock();
    return 0;
}

int tuix_get_buffer_layout_slot_by_uid(int uid, TuixLayoutSlot* out_slot) {
    if (uid <= 0 || !out_slot) {
        return -1;
    }

    tuix_lock();
    TuixBuffer* buffer = find_buffer_by_uid_unlocked(uid);
    if (!buffer) {
        tuix_unlock();
        return -1;
    }
    *out_slot = buffer->layout_slot;
    tuix_unlock();
    return 0;
}

int tuix_set_buffer_layout_rect_by_uid(int uid, int offset_left, int offset_top, int width, int height) {
    if (uid <= 0) {
        return -1;
    }

    if (width < 0) width = 0;
    if (height < 0) height = 0;

    tuix_lock();
    TuixScene* scene = find_scene_by_uid_unlocked(uid);
    TuixBuffer* buffer = find_buffer_in_scene_unlocked(scene, uid);
    if (!buffer) {
        tuix_unlock();
        return -1;
    }

    if (layout_rect_equals(&buffer->layout_rect, 1, offset_left, offset_top, width, height)) {
        tuix_unlock();
        return 0;
    }

    buffer->layout_rect.active = 1;
    buffer->layout_rect.offset_left = offset_left;
    buffer->layout_rect.offset_top = offset_top;
    buffer->layout_rect.width = width;
    buffer->layout_rect.height = height;
    mark_subtree_geometry_dirty_unlocked(scene, uid);
    tuix_unlock();
    return 0;
}

int tuix_clear_buffer_layout_rect_by_uid(int uid) {
    if (uid <= 0) {
        return -1;
    }

    tuix_lock();
    TuixScene* scene = find_scene_by_uid_unlocked(uid);
    TuixBuffer* buffer = find_buffer_in_scene_unlocked(scene, uid);
    if (!buffer) {
        tuix_unlock();
        return -1;
    }

    if (!buffer->layout_rect.active) {
        tuix_unlock();
        return 0;
    }

    memset(&buffer->layout_rect, 0, sizeof(buffer->layout_rect));
    mark_subtree_geometry_dirty_unlocked(scene, uid);
    tuix_unlock();
    return 0;
}

int tuix_set_buffer_grid_placement_by_uid(int uid, int row, int col, int row_span, int col_span) {
    if (uid <= 0) {
        return -1;
    }

    TuixLayoutSlot slot;
    if (tuix_get_buffer_layout_slot_by_uid(uid, &slot) != 0) {
        return -1;
    }

    if (row < 0) row = 0;
    if (col < 0) col = 0;
    if (row_span < 1) row_span = 1;
    if (col_span < 1) col_span = 1;

    slot.grid_row = row;
    slot.grid_col = col;
    slot.row_span = row_span;
    slot.col_span = col_span;
    return tuix_set_buffer_layout_slot_by_uid(uid, &slot);
}

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
