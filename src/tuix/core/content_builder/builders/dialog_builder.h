#ifndef TUIX_dialog_builder_H
#define TUIX_dialog_builder_H

#include "../../types.h"
#include <stdint.h>

const TuixBuilder* tuix_dialog_init(void);

int tuix_dialog_attach_child(const char* scene_name, TuixObject *dialog_obj, int child_uid);
int tuix_dialog_detach_child(const char* scene_name, TuixObject *dialog_obj, int child_uid);
int tuix_dialog_add_object(const char* scene_name, TuixObject *dialog_obj, const char* builder_name, float width_mod, float height_mod);

int tuix_dialog_set_title(TuixObject *obj, const char *title);
int tuix_dialog_set_body_size(TuixObject *obj, int width, int height);
int tuix_dialog_set_padding(TuixObject *obj, int left, int top, int right, int bottom);
int tuix_dialog_set_close_on_esc(TuixObject *obj, int enabled);
int tuix_dialog_set_close_on_backdrop(TuixObject *obj, int enabled);
int tuix_dialog_set_colors(TuixObject *obj,
                           uint8_t backdrop_r, uint8_t backdrop_g, uint8_t backdrop_b,
                           uint8_t border_r, uint8_t border_g, uint8_t border_b,
                           uint8_t body_r, uint8_t body_g, uint8_t body_b);
int tuix_dialog_activate(const char* scene_name, TuixObject *obj);
int tuix_dialog_deactivate(const char* scene_name, TuixObject *obj);
int tuix_dialog_take_close_requested(TuixObject *obj);

#endif
