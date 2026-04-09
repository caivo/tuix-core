#ifndef TUIX_icon_builder_H
#define TUIX_icon_builder_H

#include "../../types.h"
#include <stdint.h>

const TuixBuilder* tuix_icon_init(void);

int tuix_icon_set_symbol(TuixObject *obj, const char *symbol);
int tuix_icon_set_fg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b);
int tuix_icon_set_bg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b);
int tuix_icon_clear_bg(TuixObject *obj);

#endif
