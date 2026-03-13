#include "../tuix_registry.h"
#include "../types.h"
#include "geometry_resolver.h"

void tuix_resolve_geometry(TuixBuffer* buffer) {
    TuixObject *object = buffer->obj;
    buffer->width = (int)(tuix_registry.terminal_width * object->width_mod);
    buffer->height = (int)(tuix_registry.terminal_height * object->height_mod);
    buffer->margin_left = (int)(tuix_registry.terminal_width * object->margin_left_mod);
    buffer->margin_top = (int)(tuix_registry.terminal_height * object->margin_top_mod);
}