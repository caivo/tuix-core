#ifndef TUIX_buffer_manager_H
#define TUIX_buffer_manager_H

#include "types.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void tuix_init_buffer(char* scene_name, TuixObject obj);

TuixBuffer* tuix_get_buffer(char* scene_name, int uid);
TuixBuffer* tuix_get_buffer_by_uid(int uid);

void tuix_clear_buffer(char* scene_name, int uid);
void tuix_free_buffer(char* scene_name, int uid);

#ifdef __cplusplus
}
#endif

#endif