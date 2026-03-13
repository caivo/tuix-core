#ifndef TUIX_object_manager_H
#define TUIX_object_manager_H

#include "types.h"

TuixObject tuix_objects_new_object(char* builder_name, char* scene_name, float width_mod, float height_mod, float margin_top_mod, float margin_left_mod);

int tuix_create_object(char* builder_name, char* scene_name, float width_mod, float height_mod, float margin_top_mod, float margin_left_mod);

#endif