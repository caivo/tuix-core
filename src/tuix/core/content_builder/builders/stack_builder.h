#ifndef TUIX_stack_builder_H
#define TUIX_stack_builder_H

#include "../../types.h"
#include <stdint.h>

const TuixBuilder* tuix_row_init(void);
const TuixBuilder* tuix_column_init(void);

int tuix_stack_attach_child(const char* scene_name, TuixObject *layout_obj, int child_uid);
int tuix_stack_detach_child(const char* scene_name, TuixObject *layout_obj, int child_uid);
int tuix_stack_add_object(const char* scene_name, TuixObject *layout_obj, const char* builder_name, float width_mod, float height_mod);

int tuix_stack_set_gap(TuixObject *obj, int gap);
int tuix_stack_set_padding(TuixObject *obj, int left, int top, int right, int bottom);
int tuix_stack_set_justify(TuixObject *obj, int justify);
int tuix_stack_set_align(TuixObject *obj, int align);
int tuix_stack_set_bg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b);
int tuix_stack_clear_bg(TuixObject *obj);

#endif
