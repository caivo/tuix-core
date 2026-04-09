#ifndef TUIX_badge_builder_H
#define TUIX_badge_builder_H

#include "../../types.h"
#include <stdint.h>

const TuixBuilder* tuix_badge_init(void);

int tuix_badge_set_text(TuixObject *obj, const char *text);
int tuix_badge_set_colors(TuixObject *obj,
                          uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                          uint8_t bg_r, uint8_t bg_g, uint8_t bg_b);

#endif
