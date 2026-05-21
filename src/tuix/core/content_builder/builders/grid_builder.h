#ifndef TUIX_grid_builder_H
#define TUIX_grid_builder_H

#include "../../types.h"
#include <stdint.h>

#define TUIX_GRID_TRACK_FIXED  0
#define TUIX_GRID_TRACK_WEIGHT 1

typedef struct TuixGridTrack {
    int kind;
    int value;
} TuixGridTrack;

const TuixBuilder* tuix_grid_init(void);

int tuix_grid_attach_child(const char* scene_name, TuixObject *grid_obj, int child_uid);
int tuix_grid_detach_child(const char* scene_name, TuixObject *grid_obj, int child_uid);
int tuix_grid_add_object(const char* scene_name, TuixObject *grid_obj, const char* builder_name, float width_mod, float height_mod);

int tuix_grid_set_columns(TuixObject *obj, const TuixGridTrack *tracks, int count);
int tuix_grid_set_rows(TuixObject *obj, const TuixGridTrack *tracks, int count);
int tuix_grid_set_padding(TuixObject *obj, int left, int top, int right, int bottom);
int tuix_grid_set_gaps(TuixObject *obj, int gap_x, int gap_y);
int tuix_grid_set_bg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b);
int tuix_grid_clear_bg(TuixObject *obj);

#endif
