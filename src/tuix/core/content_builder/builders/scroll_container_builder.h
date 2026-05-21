#ifndef TUIX_scroll_container_builder_H
#define TUIX_scroll_container_builder_H

#include "../../types.h"

const TuixBuilder* tuix_scroll_container_init(void);

int tuix_scroll_container_is_viewport(TuixObject *obj);
int tuix_scroll_container_get_viewport_offset(TuixObject *obj, int *offset_x, int *offset_y);
int tuix_scroll_container_get_viewport_insets(TuixObject *obj, int *left, int *top, int *right, int *bottom);
int tuix_scroll_container_set_title(TuixObject *obj, const char *title);
int tuix_scroll_container_set_content_size(TuixObject *obj, int content_w, int content_h);
int tuix_scroll_container_set_offset(TuixObject *obj, int offset_x, int offset_y);
int tuix_scroll_container_get_offset_x(TuixObject *obj);
int tuix_scroll_container_get_offset_y(TuixObject *obj);
int tuix_scroll_container_get_content_width(TuixObject *obj);
int tuix_scroll_container_get_content_height(TuixObject *obj);

/* Convenience helpers to add/attach/detach child objects into the scroll container.
 * - `tuix_scroll_container_add_object` creates an object with given builder and reparents it
 *   into the container. Returns the child UID (>0) on success or -1 on error.
 * - `tuix_scroll_container_attach_child` reparents an existing child UID into container.
 * - `tuix_scroll_container_detach_child` reparents an existing child UID to root (parent=-1).
 */
int tuix_scroll_container_add_object(const char* scene_name, TuixObject *container_obj, const char* builder_name, float width_mod, float height_mod, float margin_top_mod, float margin_left_mod);
int tuix_scroll_container_attach_child(const char* scene_name, TuixObject *container_obj, int child_uid);
int tuix_scroll_container_detach_child(const char* scene_name, TuixObject *container_obj, int child_uid);
/* Create an object positioned in content coordinates (columns/rows).
 * `content_x/content_y/content_w/content_h` are in the same units used
 * by `tuix_scroll_container_set_content_size()` (i.e. content columns/rows).
 * Returns child UID (>0) on success, -1 on error. */
int tuix_scroll_container_add_object_at(const char* scene_name, TuixObject *container_obj, const char* builder_name, int content_x, int content_y, int content_w, int content_h);

#endif
