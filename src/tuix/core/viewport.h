#ifndef TUIX_viewport_H
#define TUIX_viewport_H

#include "types.h"

static inline int tuix_object_is_viewport(const TuixObject *obj) {
    return obj &&
           obj->builder &&
           obj->builder->get_viewport_offset &&
           obj->builder->get_viewport_insets;
}

static inline int tuix_object_get_viewport_offset(TuixObject *obj, int *offset_x, int *offset_y) {
    if (!tuix_object_is_viewport(obj) || !obj->builder->get_viewport_offset) {
        return -1;
    }
    return obj->builder->get_viewport_offset(obj, offset_x, offset_y);
}

static inline int tuix_object_get_viewport_insets(TuixObject *obj, int *left, int *top, int *right, int *bottom) {
    if (!tuix_object_is_viewport(obj) || !obj->builder->get_viewport_insets) {
        return -1;
    }
    return obj->builder->get_viewport_insets(obj, left, top, right, bottom);
}

static inline int tuix_object_get_viewport_content_size(TuixObject *obj, int *content_w, int *content_h) {
    if (!tuix_object_is_viewport(obj) || !obj->builder->get_viewport_content_size) {
        return -1;
    }
    return obj->builder->get_viewport_content_size(obj, content_w, content_h);
}

#endif
