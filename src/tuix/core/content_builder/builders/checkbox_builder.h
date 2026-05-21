#ifndef TUIX_checkbox_builder_H
#define TUIX_checkbox_builder_H

#include "../../types.h"

const TuixBuilder* tuix_checkbox_init(void);

int tuix_checkbox_set_label(TuixObject *obj, const char *label);
int tuix_checkbox_set_checked(TuixObject *obj, int checked);
int tuix_checkbox_get_checked(TuixObject *obj);
int tuix_checkbox_toggle(TuixObject *obj);
int tuix_checkbox_take_changed(TuixObject *obj);
int tuix_checkbox_set_disabled(TuixObject *obj, int disabled);

#endif
