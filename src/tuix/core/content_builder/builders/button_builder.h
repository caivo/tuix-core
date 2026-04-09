#ifndef TUIX_button_builder_H
#define TUIX_button_builder_H

#include "../../types.h"

const TuixBuilder* tuix_button_init(void);

int tuix_button_set_label(TuixObject *obj, const char *label);
int tuix_button_take_pressed(TuixObject *obj);
void tuix_button_reset(TuixObject *obj);

#endif
