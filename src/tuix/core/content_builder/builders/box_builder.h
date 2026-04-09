#ifndef TUIX_box_builder_H
#define TUIX_box_builder_H

#include "../../types.h"
#include <stdint.h>

const TuixBuilder* tuix_box_init(void);

int tuix_box_set_title(TuixObject *obj, const char *title);
int tuix_box_set_colors(TuixObject *obj,
                        uint8_t border_r, uint8_t border_g, uint8_t border_b,
                        uint8_t bg_r, uint8_t bg_g, uint8_t bg_b);

#endif
