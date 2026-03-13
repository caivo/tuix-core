#ifndef TUIX_subcycle_registrant_H
#define TUIX_subcycle_registrant_H
#include "../types.h"

int tuix_subcycle_init(char* scene_name, TuixObject *obj);
void tuix_subcycle_free(char* scene_name, int uid);

#endif
