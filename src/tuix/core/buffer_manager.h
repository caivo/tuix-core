#ifndef TUIX_buffer_manager_H
#define TUIX_buffer_manager_H

#include "types.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void tuix_init_buffer(char* scene_name, TuixObject obj);

int tuix_get_buffer_snapshot(char* scene_name, int uid, TuixBuffer* out_buffer);
int tuix_get_buffer_snapshot_by_uid(int uid, TuixBuffer* out_buffer);

void tuix_clear_buffer(char* scene_name, int uid);
void tuix_free_buffer(char* scene_name, int uid);
int tuix_set_buffer_parent(char* scene_name, int uid, int parent_uid);
int tuix_get_buffer_parent(char* scene_name, int uid);
int tuix_set_buffer_z_index(char* scene_name, int uid, int z_index);
int tuix_get_buffer_z_index(char* scene_name, int uid);
void tuix_mark_buffer_children_geometry_dirty_by_uid(int uid);
int tuix_set_buffer_layout_slot_by_uid(int uid, const TuixLayoutSlot* slot);
int tuix_get_buffer_layout_slot_by_uid(int uid, TuixLayoutSlot* out_slot);
int tuix_set_buffer_layout_rect_by_uid(int uid, int offset_left, int offset_top, int width, int height);
int tuix_clear_buffer_layout_rect_by_uid(int uid);
int tuix_set_buffer_grid_placement_by_uid(int uid, int row, int col, int row_span, int col_span);

/* Test-only: get the raw object by UID for testing purposes */
TuixObject* _tuix_get_object_by_uid(int uid);

#ifdef __cplusplus
}
#endif

#endif
