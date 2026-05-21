#ifndef TUIX_listview_builder_H
#define TUIX_listview_builder_H

#include "../../types.h"

const TuixBuilder* tuix_listview_init(void);

int tuix_listview_set_title(TuixObject *obj, const char *title);
int tuix_listview_set_items(TuixObject *obj, const char **items, int count);
int tuix_listview_set_selected(TuixObject *obj, int index);
int tuix_listview_get_selected(TuixObject *obj);
int tuix_listview_take_activated(TuixObject *obj);

#endif
