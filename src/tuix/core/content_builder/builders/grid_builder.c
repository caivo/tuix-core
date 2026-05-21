#include "grid_builder.h"

#include "../../buffer_manager.h"
#include "../../object_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    TuixGridTrack *cols;
    int col_count;
    TuixGridTrack *rows;
    int row_count;
    int padding_left;
    int padding_top;
    int padding_right;
    int padding_bottom;
    int gap_x;
    int gap_y;
    int use_bg;
    TuixRGBTuple bg;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixGridState;

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static int min_int(int a, int b) {
    return a < b ? a : b;
}

static int clamp_non_negative(int value) {
    return value < 0 ? 0 : value;
}

static void grid_mark_layout_changed(TuixObject *obj, TuixGridState *state) {
    if (!obj || !state) return;
    state->needs_redraw = 1;
    tuix_mark_buffer_children_geometry_dirty_by_uid(obj->uid);
}

static int grid_copy_tracks(TuixGridTrack **dst_tracks, int *dst_count, const TuixGridTrack *src_tracks, int count) {
    if (!dst_tracks || !dst_count || !src_tracks || count <= 0) {
        return -1;
    }

    TuixGridTrack *copy = (TuixGridTrack*)calloc((size_t)count, sizeof(TuixGridTrack));
    if (!copy) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        copy[i] = src_tracks[i];
        if (copy[i].kind != TUIX_GRID_TRACK_FIXED && copy[i].kind != TUIX_GRID_TRACK_WEIGHT) {
            copy[i].kind = TUIX_GRID_TRACK_WEIGHT;
        }
        if (copy[i].value < 1) {
            copy[i].value = 1;
        }
    }

    free(*dst_tracks);
    *dst_tracks = copy;
    *dst_count = count;
    return 0;
}

static void* grid_create_state(void* params) {
    (void)params;
    TuixGridState *state = (TuixGridState*)calloc(1, sizeof(TuixGridState));
    if (!state) return NULL;

    state->cols = (TuixGridTrack*)calloc(1, sizeof(TuixGridTrack));
    state->rows = (TuixGridTrack*)calloc(1, sizeof(TuixGridTrack));
    if (!state->cols || !state->rows) {
        free(state->cols);
        free(state->rows);
        free(state);
        return NULL;
    }
    state->cols[0] = (TuixGridTrack){.kind = TUIX_GRID_TRACK_WEIGHT, .value = 1};
    state->rows[0] = (TuixGridTrack){.kind = TUIX_GRID_TRACK_WEIGHT, .value = 1};
    state->col_count = 1;
    state->row_count = 1;
    state->padding_left = 0;
    state->padding_top = 0;
    state->padding_right = 0;
    state->padding_bottom = 0;
    state->gap_x = 0;
    state->gap_y = 0;
    state->use_bg = 1;
    state->bg = (TuixRGBTuple){18, 22, 32};
    state->needs_redraw = 1;
    return state;
}

static void grid_destroy_state(void* state) {
    if (!state) return;
    TuixGridState *grid = (TuixGridState*)state;
    free(grid->cols);
    free(grid->rows);
    free(grid->inserted_buffer);
    free(grid);
}

static int grid_ensure_buffer(TuixGridState *state, TuixBuffer *buffer) {
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

static void grid_put_blank(TuixPixel *pixel, const TuixGridState *state) {
    memset(pixel, 0, sizeof(*pixel));
    pixel->sym[0] = ' ';
    pixel->sym[1] = '\0';
    if (state->use_bg) {
        pixel->styles.bg = state->bg;
        pixel->styles.custom_bg = 1;
    }
}

static void distribute_weight_remainder(int *sizes, int count, const TuixGridTrack *tracks, int remainder) {
    if (!sizes || !tracks || count <= 0 || remainder <= 0) {
        return;
    }

    for (int i = 0; i < count && remainder > 0; i++) {
        if (tracks[i].kind != TUIX_GRID_TRACK_WEIGHT) {
            continue;
        }
        sizes[i]++;
        remainder--;
        if (i == count - 1 && remainder > 0) {
            i = -1;
        }
    }
}

static void grid_compute_track_layout(
    const TuixGridTrack *tracks,
    int count,
    int available,
    int gap,
    int *out_sizes,
    int *out_offsets) {
    if (!tracks || count <= 0 || !out_sizes || !out_offsets) {
        return;
    }

    if (gap < 0) gap = 0;
    int gaps_total = count > 1 ? gap * (count - 1) : 0;
    int track_space = available - gaps_total;
    if (track_space < 0) track_space = 0;

    int fixed_total = 0;
    int weight_total = 0;
    for (int i = 0; i < count; i++) {
        out_sizes[i] = 0;
        if (tracks[i].kind == TUIX_GRID_TRACK_FIXED) {
            fixed_total += max_int(0, tracks[i].value);
        } else {
            weight_total += max_int(1, tracks[i].value);
        }
    }

    int remaining = track_space - fixed_total;
    if (remaining < 0) remaining = 0;

    for (int i = 0; i < count; i++) {
        if (tracks[i].kind == TUIX_GRID_TRACK_FIXED) {
            out_sizes[i] = max_int(0, tracks[i].value);
        } else if (weight_total > 0) {
            out_sizes[i] = (remaining * max_int(1, tracks[i].value)) / weight_total;
        }
    }

    int used = 0;
    for (int i = 0; i < count; i++) {
        used += out_sizes[i];
    }
    if (used < track_space) {
        distribute_weight_remainder(out_sizes, count, tracks, track_space - used);
    }

    int cursor = 0;
    for (int i = 0; i < count; i++) {
        out_offsets[i] = cursor;
        cursor += out_sizes[i];
        if (i + 1 < count) {
            cursor += gap;
        }
    }
}

static int grid_span_size(const int *sizes, int start, int span, int gap) {
    int total = 0;
    for (int i = 0; i < span; i++) {
        total += sizes[start + i];
    }
    if (span > 1) {
        total += gap * (span - 1);
    }
    return total;
}

static void grid_layout_children(TuixObject *obj, TuixBuffer *buffer) {
    if (!obj || !obj->state || !buffer) {
        return;
    }

    TuixGridState *state = (TuixGridState*)obj->state;
    if (!state->cols || state->col_count <= 0 || !state->rows || state->row_count <= 0) {
        return;
    }

    int inner_w = buffer->width - state->padding_left - state->padding_right;
    int inner_h = buffer->height - state->padding_top - state->padding_bottom;
    if (inner_w < 0) inner_w = 0;
    if (inner_h < 0) inner_h = 0;

    int *col_sizes = (int*)calloc((size_t)state->col_count, sizeof(int));
    int *col_offsets = (int*)calloc((size_t)state->col_count, sizeof(int));
    int *row_sizes = (int*)calloc((size_t)state->row_count, sizeof(int));
    int *row_offsets = (int*)calloc((size_t)state->row_count, sizeof(int));
    if (!col_sizes || !col_offsets || !row_sizes || !row_offsets) {
        free(col_sizes);
        free(col_offsets);
        free(row_sizes);
        free(row_offsets);
        return;
    }

    grid_compute_track_layout(state->cols, state->col_count, inner_w, state->gap_x, col_sizes, col_offsets);
    grid_compute_track_layout(state->rows, state->row_count, inner_h, state->gap_y, row_sizes, row_offsets);

    for (int i = 0; i < buffer->children_count; i++) {
        int child_uid = buffer->children_uids[i];
        TuixLayoutSlot slot;
        if (tuix_get_buffer_layout_slot_by_uid(child_uid, &slot) != 0) {
            continue;
        }

        int col = clamp_non_negative(slot.grid_col);
        int row = clamp_non_negative(slot.grid_row);
        if (col >= state->col_count) col = state->col_count - 1;
        if (row >= state->row_count) row = state->row_count - 1;

        int col_span = slot.col_span < 1 ? 1 : slot.col_span;
        int row_span = slot.row_span < 1 ? 1 : slot.row_span;
        if (col + col_span > state->col_count) col_span = state->col_count - col;
        if (row + row_span > state->row_count) row_span = state->row_count - row;
        if (col_span < 1) col_span = 1;
        if (row_span < 1) row_span = 1;

        int child_left = state->padding_left + col_offsets[col];
        int child_top = state->padding_top + row_offsets[row];
        int child_width = grid_span_size(col_sizes, col, col_span, state->gap_x);
        int child_height = grid_span_size(row_sizes, row, row_span, state->gap_y);
        tuix_set_buffer_layout_rect_by_uid(child_uid, child_left, child_top, child_width, child_height);
    }

    free(col_sizes);
    free(col_offsets);
    free(row_sizes);
    free(row_offsets);
}

static TuixPixel* grid_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixGridState *state = obj ? (TuixGridState*)obj->state : NULL;
    if (!state || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (grid_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    for (size_t i = 0; i < n; i++) {
        grid_put_blank(&state->inserted_buffer[i], state);
    }

    return state->inserted_buffer;
}

static TuixHandlerResponse grid_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)has_event;
    (void)is_focused;
    (void)snap;
    if (!obj || !obj->state) return (TuixHandlerResponse){.requires_redraw = 0};
    TuixGridState *state = (TuixGridState*)obj->state;
    if (state->needs_redraw) {
        state->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}

static void grid_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixGridState *state = (TuixGridState*)obj->state;
    if (grid_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

static const TuixBuilder tuix_grid_builder = {
    .name = "GridBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = grid_create_state,
    .destroy_state = grid_destroy_state,
    .on_event = grid_handler,
    .on_resize = grid_on_resize,
    .layout_children = grid_layout_children,
    .build_content = grid_build_content
};

const TuixBuilder* tuix_grid_init(void) {
    return &tuix_grid_builder;
}

int tuix_grid_attach_child(const char* scene_name, TuixObject *grid_obj, int child_uid) {
    if (!scene_name || !grid_obj || child_uid <= 0) return -1;
    return tuix_set_buffer_parent((char*)scene_name, child_uid, grid_obj->uid);
}

int tuix_grid_detach_child(const char* scene_name, TuixObject *grid_obj, int child_uid) {
    (void)grid_obj;
    if (!scene_name || child_uid <= 0) return -1;
    return tuix_set_buffer_parent((char*)scene_name, child_uid, -1);
}

int tuix_grid_add_object(const char* scene_name, TuixObject *grid_obj, const char* builder_name, float width_mod, float height_mod) {
    if (!scene_name || !grid_obj || !builder_name) return -1;
    int child_uid = tuix_create_object_ex((char*)builder_name, (char*)scene_name, width_mod, height_mod, 0.0f, 0.0f, 0);
    if (child_uid <= 0) {
        return -1;
    }
    if (tuix_set_buffer_parent((char*)scene_name, child_uid, grid_obj->uid) != 0) {
        tuix_free_buffer((char*)scene_name, child_uid);
        return -1;
    }
    return child_uid;
}

int tuix_grid_set_columns(TuixObject *obj, const TuixGridTrack *tracks, int count) {
    if (!obj || !obj->state) return -1;
    TuixGridState *state = (TuixGridState*)obj->state;
    if (grid_copy_tracks(&state->cols, &state->col_count, tracks, count) != 0) {
        return -1;
    }
    grid_mark_layout_changed(obj, state);
    return 0;
}

int tuix_grid_set_rows(TuixObject *obj, const TuixGridTrack *tracks, int count) {
    if (!obj || !obj->state) return -1;
    TuixGridState *state = (TuixGridState*)obj->state;
    if (grid_copy_tracks(&state->rows, &state->row_count, tracks, count) != 0) {
        return -1;
    }
    grid_mark_layout_changed(obj, state);
    return 0;
}

int tuix_grid_set_padding(TuixObject *obj, int left, int top, int right, int bottom) {
    if (!obj || !obj->state) return -1;
    TuixGridState *state = (TuixGridState*)obj->state;
    left = clamp_non_negative(left);
    top = clamp_non_negative(top);
    right = clamp_non_negative(right);
    bottom = clamp_non_negative(bottom);
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
    grid_mark_layout_changed(obj, state);
    return 0;
}

int tuix_grid_set_gaps(TuixObject *obj, int gap_x, int gap_y) {
    if (!obj || !obj->state) return -1;
    TuixGridState *state = (TuixGridState*)obj->state;
    gap_x = clamp_non_negative(gap_x);
    gap_y = clamp_non_negative(gap_y);
    if (state->gap_x == gap_x && state->gap_y == gap_y) {
        return 0;
    }
    state->gap_x = gap_x;
    state->gap_y = gap_y;
    grid_mark_layout_changed(obj, state);
    return 0;
}

int tuix_grid_set_bg(TuixObject *obj, uint8_t r, uint8_t g, uint8_t b) {
    if (!obj || !obj->state) return -1;
    TuixGridState *state = (TuixGridState*)obj->state;
    state->bg = (TuixRGBTuple){r, g, b};
    state->use_bg = 1;
    state->needs_redraw = 1;
    return 0;
}

int tuix_grid_clear_bg(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixGridState *state = (TuixGridState*)obj->state;
    state->use_bg = 0;
    state->needs_redraw = 1;
    return 0;
}
