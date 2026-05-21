#ifndef TUIX_textarea_builder_H
#define TUIX_textarea_builder_H

#include "../../types.h"

const TuixBuilder* tuix_textarea_init(void);

int tuix_textarea_set_title(TuixObject *obj, const char *title);
int tuix_textarea_set_text(TuixObject *obj, const char *text);
const char* tuix_textarea_get_text(TuixObject *obj);
int tuix_textarea_set_placeholder(TuixObject *obj, const char *text);
int tuix_textarea_set_readonly(TuixObject *obj, int readonly);

#endif
