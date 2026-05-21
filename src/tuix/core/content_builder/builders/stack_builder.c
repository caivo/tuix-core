#include "stack_builder.h"
#include "../../buffer_manager.h"
#include "../../object_manager.h"

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int axis;
    int gap;
    int padding_left;
    int padding_top;
    int padding_right;
    int padding_bottom;
    int justify;
    int align;
    int use_bg;
    TuixRGBTuple bg;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixStackState;

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static int min_int(int a, int b) {
    return a < b ? a : b;
}

static int clamp_min_max(int value, int min_value, int max_value) {
    if (value < min_value) value = min_value;
    if (max_value >= 0 && value > max_value) value = max_value;
    return value;
}

static void stack_mark_layout_changed(TuixObject *obj, TuixStackState *state) {
    if (!obj || !state) return;
    state->needs_redraw = 1;
    tuix_mark_buffer_children_geometry_dirty_by_uid(obj->uid);
}

static void* stack_create_state_common(int axis) {
    TuixStackState *state = (TuixStackState*)calloc(1, sizeof(TuixStackState));
    if (!state) return NULL;
    state->axis = axis;
    state->gap = 0;
    state->padding_left = 0;
    state->padding_top = 0;
    state->padding_right = 0;
    state->padding_bottom = 0;
    state->justify = TUIX_LAYOUT_JUSTIFY_START;
    state->align = TUIX_LAYOUT_ALIGN_STRETCH;
    state->use_bg = 0;
    state->bg = (TuixRGBTuple){0, 0, 0};
    state->needs_redraw = 1;
    return state;
}

static void* row_create_state(void* params) {
    (void)params;
    return stack_create_state_common(TUIX_LAYOUT_AXIS_HORIZONTAL);
}

static void* column_create_state(void* params) {
    (void)params;
    return stack_create_state_common(TUIX_LAYOUT_AXIS_VERTICAL);
}

static void stack_destroy_state(void* state) {
    if (!state) return;
    TuixStackState *stack = (TuixStackState*)state;
    free(stack->inserted_buffer);
    free(stack);
}

static int stack_ensure_buffer(TuixStackState *state, TuixBuffer *buffer) {
    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    size_t required = sizeof(TuixPixel) * n;
    if (state->inserted_buffer && (size_t)state->inserted_buffer_size == required) {
        return 0;
    }
    if (state->inserted_buffer) {
        free(state->inserted_buffer);
        state->inserted_buffer = NULL;
        state->inserted_buffer_size = 0;
    }
    state->inserted_buffer = (TuixPixel*)calloc(n, sizeof(TuixPixel));
    if (!state->inserted_buffer) return -1;
    state->inserted_buffer_size = (int)required;
    state->needs_redraw = 1;
    buffer->required_redraw = 1;
    return 0;
}

static void stack_put_blank(TuixPixel *pixel, const TuixStackState *state) {
    memset(pixel, 0, sizeof(*pixel));
    pixel->sym[0] = ' ';
    pixel->sym[1] = '\0';
    if (state->use_bg) {
        pixel->styles.bg = state->bg;
        pixel->styles.custom_bg = 1;
    }
}

static int stack_effective_align(const TuixStackState *state, const TuixLayoutSlot *slot) {
    if (slot && slot->align_self != TUIX_LAYOUT_ALIGN_AUTO) {
        return slot->align_self;
    }
    return state ? state->align : TUIX_LAYOUT_ALIGN_STRETCH;
}

static int stack_main_basis(const TuixLayoutSlot *slot, const TuixObject *child_obj, int axis, int available_main) {
    if (slot && slot->basis >= 0) {
        return slot->basis;
    }

    if (child_obj) {
        float mod = axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? child_obj->width_mod : child_obj->height_mod;
        if (mod > 0.0f) {
            return (int)((float)available_main * mod);
        }
    }

    return 0;
}

static int stack_cross_basis(const TuixLayoutSlot *slot, const TuixObject *child_obj, int axis, int available_cross, int effective_align) {
    int cross = available_cross;

    if (effective_align != TUIX_LAYOUT_ALIGN_STRETCH && child_obj) {
        float mod = axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? child_obj->height_mod : child_obj->width_mod;
        if (mod > 0.0f) {
            cross = (int)((float)available_cross * mod);
        }
    }

    if (slot) {
        int min_cross = axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? slot->min_h : slot->min_w;
        int max_cross = axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? slot->max_h : slot->max_w;
        cross = clamp_min_max(cross, max_int(0, min_cross), max_cross);
    }

    return cross;
}

static void distribute_weighted_units(
    int *sizes,
    int count,
    const float *weights,
    const int *mins,
    const int *maxs,
    int units,
    int grow_direction) {
    if (!sizes || !weights || !mins || !maxs || count <= 0 || units <= 0) {
        return;
    }

    float total_weight = 0.0f;
    for (int i = 0; i < count; i++) {
        if (weights[i] > 0.0f) {
            total_weight += weights[i];
        }
    }
    if (total_weight <= 0.0f) {
        return;
    }

    float *accum = (float*)calloc((size_t)count, sizeof(float));
    if (!accum) {
        return;
    }

    while (units > 0) {
        int best_idx = -1;
        float best_score = -1.0f;

        for (int i = 0; i < count; i++) {
            if (weights[i] <= 0.0f) {
                continue;
            }
            if (grow_direction > 0) {
                if (maxs[i] >= 0 && sizes[i] >= maxs[i]) {
                    continue;
                }
            } else if (sizes[i] <= mins[i]) {
                continue;
            }

            accum[i] += weights[i];
            if (accum[i] > best_score) {
                best_score = accum[i];
                best_idx = i;
            }
        }

        if (best_idx < 0) {
            break;
        }

        sizes[best_idx] += grow_direction > 0 ? 1 : -1;
        accum[best_idx] -= total_weight;
        units--;
    }

    free(accum);
}

static void stack_layout_children(TuixObject *obj, TuixBuffer *buffer) {
    if (!obj || !obj->state || !buffer || buffer->children_count <= 0) {
        return;
    }

    TuixStackState *state = (TuixStackState*)obj->state;
    int child_count = buffer->children_count;
    int gap = state->gap > 0 ? state->gap : 0;
    int inner_w = buffer->width - state->padding_left - state->padding_right;
    int inner_h = buffer->height - state->padding_top - state->padding_bottom;
    if (inner_w < 0) inner_w = 0;
    if (inner_h < 0) inner_h = 0;

    int available_main = state->axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? inner_w : inner_h;
    int available_cross = state->axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? inner_h : inner_w;
    int gaps_total = child_count > 1 ? gap * (child_count - 1) : 0;
    int main_for_items = available_main - gaps_total;
    if (main_for_items < 0) main_for_items = 0;

    int *sizes = (int*)calloc((size_t)child_count, sizeof(int));
    int *mins = (int*)calloc((size_t)child_count, sizeof(int));
    int *maxs = (int*)calloc((size_t)child_count, sizeof(int));
    float *grow_weights = (float*)calloc((size_t)child_count, sizeof(float));
    float *shrink_weights = (float*)calloc((size_t)child_count, sizeof(float));
    TuixLayoutSlot *slots = (TuixLayoutSlot*)calloc((size_t)child_count, sizeof(TuixLayoutSlot));
    TuixObject *child_objects = (TuixObject*)calloc((size_t)child_count, sizeof(TuixObject));
    if (!sizes || !mins || !maxs || !grow_weights || !shrink_weights || !slots || !child_objects) {
        free(sizes);
        free(mins);
        free(maxs);
        free(grow_weights);
        free(shrink_weights);
        free(slots);
        free(child_objects);
        return;
    }

    int used_main = 0;
    for (int i = 0; i < child_count; i++) {
        int child_uid = buffer->children_uids[i];
        slots[i] = (TuixLayoutSlot){
            .grow = 0.0f,
            .shrink = 1.0f,
            .basis = TUIX_LAYOUT_BASIS_AUTO,
            .min_w = 0,
            .min_h = 0,
            .max_w = TUIX_LAYOUT_UNBOUNDED,
            .max_h = TUIX_LAYOUT_UNBOUNDED,
            .align_self = TUIX_LAYOUT_ALIGN_AUTO,
            .grid_row = 0,
            .grid_col = 0,
            .row_span = 1,
            .col_span = 1
        };
        memset(&child_objects[i], 0, sizeof(child_objects[i]));
        tuix_get_buffer_layout_slot_by_uid(child_uid, &slots[i]);
        tuix_get_object_snapshot_by_uid(child_uid, &child_objects[i]);

        mins[i] = state->axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? max_int(0, slots[i].min_w) : max_int(0, slots[i].min_h);
        maxs[i] = state->axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? slots[i].max_w : slots[i].max_h;
        sizes[i] = stack_main_basis(&slots[i], &child_objects[i], state->axis, main_for_items);
        sizes[i] = clamp_min_max(sizes[i], mins[i], maxs[i]);
        used_main += sizes[i];
        grow_weights[i] = slots[i].grow > 0.0f ? slots[i].grow : 0.0f;
        shrink_weights[i] = slots[i].shrink > 0.0f ? slots[i].shrink : 0.0f;
    }

    if (used_main < main_for_items) {
        distribute_weighted_units(sizes, child_count, grow_weights, mins, maxs, main_for_items - used_main, 1);
    } else if (used_main > main_for_items) {
        distribute_weighted_units(sizes, child_count, shrink_weights, mins, maxs, used_main - main_for_items, -1);
    }

    used_main = 0;
    for (int i = 0; i < child_count; i++) {
        used_main += sizes[i];
    }
    int used_with_gaps = used_main + gaps_total;
    int slack = available_main - used_with_gaps;
    if (slack < 0) slack = 0;

    int leading = 0;
    int dynamic_gap = gap;
    int gap_remainder = 0;
    switch (state->justify) {
        case TUIX_LAYOUT_JUSTIFY_CENTER:
            leading = slack / 2;
            break;
        case TUIX_LAYOUT_JUSTIFY_END:
            leading = slack;
            break;
        case TUIX_LAYOUT_JUSTIFY_SPACE_BETWEEN:
            if (child_count > 1) {
                dynamic_gap += slack / (child_count - 1);
                gap_remainder = slack % (child_count - 1);
            }
            break;
        case TUIX_LAYOUT_JUSTIFY_START:
        default:
            break;
    }

    int cursor = leading;
    for (int i = 0; i < child_count; i++) {
        int effective_align = stack_effective_align(state, &slots[i]);
        int cross_size = stack_cross_basis(&slots[i], &child_objects[i], state->axis, available_cross, effective_align);
        int cross_offset = 0;
        switch (effective_align) {
            case TUIX_LAYOUT_ALIGN_CENTER:
                cross_offset = (available_cross - cross_size) / 2;
                break;
            case TUIX_LAYOUT_ALIGN_END:
                cross_offset = available_cross - cross_size;
                break;
            case TUIX_LAYOUT_ALIGN_START:
            case TUIX_LAYOUT_ALIGN_STRETCH:
            default:
                cross_offset = 0;
                break;
        }
        if (cross_offset < 0) cross_offset = 0;

        int child_left = state->axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? state->padding_left + cursor : state->padding_left + cross_offset;
        int child_top = state->axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? state->padding_top + cross_offset : state->padding_top + cursor;
        int child_width = state->axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? sizes[i] : cross_size;
        int child_height = state->axis == TUIX_LAYOUT_AXIS_HORIZONTAL ? cross_size : sizes[i];

        tuix_set_buffer_layout_rect_by_uid(buffer->children_uids[i], child_left, child_top, child_width, child_height);

        cursor += sizes[i];
        if (i + 1 < child_count) {
            cursor += dynamic_gap;
            if (gap_remainder > 0) {
                cursor++;
                gap_remainder--;
            }
        }
    }

    free(sizes);
    free(mins);
    free(maxs);
    free(grow_weights);
    free(shrink_weights);
    free(slots);
    free(child_objects);
}

static TuixPixel* stack_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixStackState *state = obj ? (TuixStackState*)obj->state : NULL;
    if (!state || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (stack_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    for (size_t i = 0; i < n; i++) {
        stack_put_blank(&state->inserted_buffer[i], state);
    }

    return state->inserted_buffer;
}

static TuixHandlerResponse stack_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)has_event;
    (void)is_focused;
    (void)snap;
    if (!obj || !obj->state) return (TuixHandlerResponse){.requires_redraw = 0};
    TuixStackState *state = (TuixStackState*)obj->state;
    if (state->needs_redraw) {
        state->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}

static void stack_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixStackState *state = (TuixStackState*)obj->state;
    if (stack_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

static const TuixBuilder tuix_row_builder = {
    .name = "RowBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = row_create_state,
    .destroy_state = stack_destroy_state,
    .on_event = stack_handler,
    .on_resize = stack_on_resize,
    .layout_children = stack_layout_children,
    .build_content = stack_build_content
};

static const TuixBuilder tuix_column_builder = {
    .name = "ColumnBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = column_create_state,
    .destroy_state = stack_destroy_state,
    .on_event = stack_handler,
    .on_resize = stack_on_resize,
    .layout_children = stack_layout_children,
    .build_content = stack_build_content
};

const TuixBuilder* tuix_row_init(void) {
    return &tuix_row_builder;
}

const TuixBuilder* tuix_column_init(void) {
    return &tuix_column_builder;
}

int tuix_stack_attach_child(const char* scene_name, TuixObject *layout_obj, int child_uid) {
    if (!scene_name || !layout_obj || child_uid <= 0) return -1;
    return tuix_set_buffer_parent((char*)scene_name, child_uid, layout_obj->uid);
}

int tuix_stack_detach_child(const char* scene_name, TuixObject *layout_obj, int child_uid) {
    (void)layout_obj;
    if (!scene_name || child_uid <= 0) return -1;
    return tuix_set_buffer_parent((char*)scene_name, child_uid, -1);
}

int tuix_stack_add_object(const char* scene_name, TuixObject *layout_obj, const char* builder_name, float width_mod, float height_mod) {
    if (!scene_name || !layout_obj || !builder_name) return -1;
    int child_uid = tuix_create_object_ex((char*)builder_name, (char*)scene_name, width_mod, height_mod, 0.0f, 0.0f, 0);
    if (child_uid <= 0) {
        return -1;
    }
    if (tuix_set_buffer_parent((char*)scene_name, child_uid, layout_obj->uid) != 0) {
        tuix_free_buffer((char*)scene_name, child_uid);
        return -1;
    }
    return child_uid;
}

int tuix_stack_set_gap(TuixObject *obj, int gap) {
    if (!obj || !obj->state) return -1;
    TuixStackState *state = (TuixStackState*)obj->state;
    if (gap < 0) gap = 0;
    if (state->gap == gap) return 0;
    state->gap = gap;
    stack_mark_layout_changed(obj, state);
    return 0;
}

int tuix_stack_set_padding(TuixObject *obj, int left, int top, int right, int bottom) {
    if (!obj || !obj->state) return -1;
    TuixStackState *state = (TuixStackState*)obj->state;
    left = max_int(0, left);
    top = max_int(0, top);
    right = max_int(0, right);
    bottom = max_int(0, bottom);
    if (state->padding_left == left &&
        state->padding_top == top &&
        state->padding_right == right &&
        state->padding_bottom == bottom) {
        return 0;
    }
    state->padding_left = left;
    state->padding_top = top;
    state->padding_right = right;
    state->padding_bottom = bottom;
    stack_mark_layout_changed(obj, state);
    return 0;
}

int tuix_stack_set_justify(TuixObject *obj, int justify) {
    if (!obj || !obj->state) return -1;
    if (justify < TUIX_LAYOUT_JUSTIFY_START || justify > TUIX_LAYOUT_JUSTIFY_SPACE_BETWEEN) {
        return -1;
    }
    TuixStackState *state = (TuixStackState*)obj->state;
    if (state->justify == justify) return 0;
    state->justify = justify;
    stack_mark_layout_changed(obj, state);
    return 0;
}

int tuix_stack_set_align(TuixObject *obj, int align) {
    if (!obj || !obj->state) return -1;
    if (align < TUIX_LAYOUT_ALIGN_START || align > TUIX_LAYOUT_ALIGN_STRETCH) {
        return -1;
    }
    TuixStackState *state = (TuixStackState*)obj->state;
    if (state->align == align) return 0;
    state->align = align;
    stack_mark_layout_changed(obj, state);
    return 0;
}

int tuix_stack_set_bg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b) {
    if (!obj || !obj->state) return -1;
    TuixStackState *state = (TuixStackState*)obj->state;
    state->bg = (TuixRGBTuple){r, g, b};
    state->use_bg = 1;
    state->needs_redraw = 1;
    return 0;
}

int tuix_stack_clear_bg(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixStackState *state = (TuixStackState*)obj->state;
    if (!state->use_bg) return 0;
    state->use_bg = 0;
    state->needs_redraw = 1;
    return 0;
}
