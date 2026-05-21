#include "../tuix_registry.h"
#include "../types.h"
#include "../buffer_manager.h"
#include "../object_manager.h"
#include "../viewport.h"
#include "geometry_resolver.h"

#include <string.h>

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

                TuixObject parent_obj;
                if (tuix_get_object_snapshot_by_uid(buffer->parent_uid, &parent_obj) == 0 &&
                    tuix_object_is_viewport(&parent_obj)) {
                    int content_w = 0;
                    int content_h = 0;
                    int inset_left = 1, inset_top = 1;
                    tuix_object_get_viewport_content_size(&parent_obj, &content_w, &content_h);
                    tuix_object_get_viewport_insets(&parent_obj, &inset_left, &inset_top, NULL, NULL);
                    if (content_w > 0) base_w = content_w;
                    if (content_h > 0) base_h = content_h;
                    base_left = parent.margin_left + inset_left;
                    base_top = parent.margin_top + inset_top;
                }
            }
        }
    }

    TuixObject *object = buffer->obj;
    if (buffer->layout_rect.active) {
        buffer->width = buffer->layout_rect.width;
        buffer->height = buffer->layout_rect.height;
        buffer->margin_left = base_left + buffer->layout_rect.offset_left;
        buffer->margin_top = base_top + buffer->layout_rect.offset_top;
    } else {
        buffer->width = (int)(base_w * object->width_mod);
        buffer->height = (int)(base_h * object->height_mod);
        buffer->margin_left = base_left + (int)(base_w * object->margin_left_mod);
        buffer->margin_top = base_top + (int)(base_h * object->margin_top_mod);
    }

    if (buffer->width < 0) {
        buffer->width = 0;
    }
    if (buffer->height < 0) {
        buffer->height = 0;
    }
}
