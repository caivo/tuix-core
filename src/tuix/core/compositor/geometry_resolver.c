#include "../tuix_registry.h"
#include "../types.h"
#include "../buffer_manager.h"
#include "geometry_resolver.h"

void tuix_resolve_geometry(TuixBuffer* buffer) {
    if (!buffer || !buffer->obj) {
        return;
    }

    int base_w = tuix_registry.terminal_width;
    int base_h = tuix_registry.terminal_height;
    int base_left = 0;
    int base_top = 0;

    if (buffer->parent_uid >= 0) {
        TuixBuffer parent;
        if (tuix_get_buffer_snapshot_by_uid(buffer->parent_uid, &parent) == 0) {
            if (parent.width > 0 && parent.height > 0) {
                base_w = parent.width;
                base_h = parent.height;
                base_left = parent.margin_left;
                base_top = parent.margin_top;
            }
        }
    }

    TuixObject *object = buffer->obj;
    buffer->width = (int)(base_w * object->width_mod);
    buffer->height = (int)(base_h * object->height_mod);
    buffer->margin_left = base_left + (int)(base_w * object->margin_left_mod);
    buffer->margin_top = base_top + (int)(base_h * object->margin_top_mod);

    if (buffer->width < 0) {
        buffer->width = 0;
    }
    if (buffer->height < 0) {
        buffer->height = 0;
    }
}