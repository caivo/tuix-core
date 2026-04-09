#ifndef TUIX_text_builder_H
#define TUIX_text_builder_H

#include "../../types.h"
#include <stdint.h>

const TuixBuilder* tuix_text_init(void);

int tuix_text_set_text(TuixObject *obj, const char *text);
int tuix_text_set_fg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b);
int tuix_text_set_bg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b);
int tuix_text_clear_bg(TuixObject *obj);

#endif
