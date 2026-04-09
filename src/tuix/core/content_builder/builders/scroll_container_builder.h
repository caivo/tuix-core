#ifndef TUIX_scroll_container_builder_H
#define TUIX_scroll_container_builder_H

#include "../../types.h"

const TuixBuilder* tuix_scroll_container_init(void);

int tuix_scroll_container_set_title(TuixObject *obj, const char *title);
int tuix_scroll_container_set_content_size(TuixObject *obj, int content_w, int content_h);
int tuix_scroll_container_set_offset(TuixObject *obj, int offset_x, int offset_y);
int tuix_scroll_container_get_offset_x(TuixObject *obj);
int tuix_scroll_container_get_offset_y(TuixObject *obj);

#endif