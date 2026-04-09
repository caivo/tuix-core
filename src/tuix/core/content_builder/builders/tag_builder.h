#ifndef TUIX_tag_builder_H
#define TUIX_tag_builder_H

#include "../../types.h"
#include <stdint.h>

const TuixBuilder* tuix_tag_init(void);

int tuix_tag_set_text(TuixObject *obj, const char *text);
int tuix_tag_set_brackets(TuixObject *obj, char left, char right);
int tuix_tag_set_colors(TuixObject *obj,
                        uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                        uint8_t bg_r, uint8_t bg_g, uint8_t bg_b);

#endif
