#ifndef TUIX_menu_builder_H
#define TUIX_menu_builder_H

#include <stdint.h>
#include "../../types.h"

const TuixBuilder* tuix_menu_init(void);

int tuix_menu_set_title(TuixObject *obj, const char *title);
int tuix_menu_set_items(TuixObject *obj, const char **labels, int count);
int tuix_menu_get_selected(TuixObject *obj);
int tuix_menu_take_activated(TuixObject *obj);
void tuix_menu_reset(TuixObject *obj);

#endif