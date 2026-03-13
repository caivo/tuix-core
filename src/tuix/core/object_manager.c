#include "object_manager.h"
#include "tuix_registry.h"
#include "types.h"
#include "buffer_manager.h"
#include "buffers.h"
#include "content_builder/builders_router.h"
#include "subcycles/subcycle_registrant.h"

#include <stdlib.h>

TuixObject tuix_objects_new_object(char* builder_name, char* scene_name, float width_mod, float height_mod, float margin_top_mod, float margin_left_mod) {
    TuixBuilder *builder = tuix_get_builder_by_name(builder_name);
    if (builder == NULL) {
        return (TuixObject){0};
    }
    TuixScene *scene = tuix_get_scene(scene_name);
    if (scene == NULL) {
        return (TuixObject){0};
    }
    TuixObject obj;
    obj.uid = tuix_registry.next_uid++;
    obj.builder = builder;
    obj.state = NULL;
    obj.width_mod = width_mod;
    obj.height_mod = height_mod;
    obj.margin_top_mod = margin_top_mod;
    obj.margin_left_mod = margin_left_mod;
    return obj;
}

int tuix_create_object(char* builder_name, char* scene_name, float width_mod, float height_mod, float margin_top_mod, float margin_left_mod) {
    TuixObject obj = tuix_objects_new_object(builder_name, scene_name, width_mod, height_mod, margin_top_mod, margin_left_mod);
    if (obj.uid == 0) {
        return -1;
    }

    tuix_init_buffer(scene_name, obj);
    TuixBuffer* buffer = tuix_get_buffer(scene_name, obj.uid);
    if (buffer == NULL) {
        return -1;
    }

    /* Use the heap-allocated obj copy stored in the buffer, not the stack copy. */
    tuix_subcycle_init(scene_name, buffer->obj);
    
    return buffer->obj->uid;
}