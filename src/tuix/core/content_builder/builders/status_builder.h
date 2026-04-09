#ifndef TUIX_status_builder_H
#define TUIX_status_builder_H

#include "../../types.h"
#include <stdint.h>

#define TUIX_STATUS_IDLE  0
#define TUIX_STATUS_OK    1
#define TUIX_STATUS_WARN  2
#define TUIX_STATUS_ERROR 3

const TuixBuilder* tuix_status_init(void);

int tuix_status_set_text(TuixObject *obj, const char *text);
int tuix_status_set_level(TuixObject *obj, int level);
int tuix_status_set_palette(TuixObject *obj,
                            uint8_t ok_r, uint8_t ok_g, uint8_t ok_b,
                            uint8_t warn_r, uint8_t warn_g, uint8_t warn_b,
                            uint8_t err_r, uint8_t err_g, uint8_t err_b,
                            uint8_t idle_r, uint8_t idle_g, uint8_t idle_b);

#endif
