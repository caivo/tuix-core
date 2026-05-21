#ifndef TUIX_splitpane_builder_H
#define TUIX_splitpane_builder_H

#include "../../types.h"
#include <stdint.h>

#define TUIX_SPLIT_MODE_RATIO  0
#define TUIX_SPLIT_MODE_PIXELS 1

const TuixBuilder* tuix_splitpane_init(void);

int tuix_splitpane_attach_child(const char* scene_name, TuixObject *split_obj, int child_uid);
int tuix_splitpane_detach_child(const char* scene_name, TuixObject *split_obj, int child_uid);
int tuix_splitpane_add_object(const char* scene_name, TuixObject *split_obj, const char* builder_name, float width_mod, float height_mod);

int tuix_splitpane_set_orientation(TuixObject *obj, int orientation);
int tuix_splitpane_set_split_ratio(TuixObject *obj, float ratio);
int tuix_splitpane_set_split_pixels(TuixObject *obj, int pixels);
int tuix_splitpane_set_divider_size(TuixObject *obj, int divider_size);
int tuix_splitpane_set_min_sizes(TuixObject *obj, int min_first, int min_second);
int tuix_splitpane_set_colors(TuixObject *obj,
                              uint8_t divider_fg_r, uint8_t divider_fg_g, uint8_t divider_fg_b,
                              uint8_t divider_bg_r, uint8_t divider_bg_g, uint8_t divider_bg_b,
                              uint8_t bg_r, uint8_t bg_g, uint8_t bg_b);
int tuix_splitpane_clear_bg(TuixObject *obj);

#endif
