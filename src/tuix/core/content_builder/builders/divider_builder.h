#ifndef TUIX_divider_builder_H
#define TUIX_divider_builder_H

#include "../../types.h"
#include <stdint.h>

#define TUIX_DIVIDER_HORIZONTAL 0
#define TUIX_DIVIDER_VERTICAL   1

const TuixBuilder* tuix_divider_init(void);

int tuix_divider_set_orientation(TuixObject *obj, int orientation);
int tuix_divider_set_symbol(TuixObject *obj, char sym);
int tuix_divider_set_color(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b);

#endif
