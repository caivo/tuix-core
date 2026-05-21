#include "listview_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TUIX_VK_UP       0x26
#define TUIX_VK_DOWN     0x28
#define TUIX_VK_HOME     0x24
#define TUIX_VK_END      0x23
#define TUIX_VK_PRIOR    0x21
#define TUIX_VK_NEXT     0x22
#define TUIX_VK_ENTER    0x0D

typedef struct {
    char *title;
    char **items;
    int item_count;
    int selected_index;
    int hovered_index;
    int scroll_y;
    int follow_selection;
    int activated_index;
    int focused;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixListViewState;

static void listview_mark_redraw(TuixListViewState *state) {
    if (!state) return;
    state->needs_redraw = 1;
}

static void listview_free_items(TuixListViewState *state) {
    if (!state) return;
    for (int i = 0; i < state->item_count; i++) {
        free(state->items[i]);
    }
    free(state->items);
    state->items = NULL;
    state->item_count = 0;
}

static void listview_clamp_scroll(TuixListViewState *state, int viewport_rows) {
    if (!state) return;
    if (viewport_rows < 1) viewport_rows = 1;
    int max_scroll = state->item_count - viewport_rows;
    if (max_scroll < 0) max_scroll = 0;
    if (state->scroll_y < 0) state->scroll_y = 0;
    if (state->scroll_y > max_scroll) state->scroll_y = max_scroll;
}

static void listview_ensure_visible(TuixListViewState *state, int viewport_rows) {
    if (!state || state->item_count <= 0) return;
    if (state->selected_index < 0) state->selected_index = 0;
    if (state->selected_index >= state->item_count) state->selected_index = state->item_count - 1;
    if (viewport_rows < 1) viewport_rows = 1;

    if (state->selected_index < state->scroll_y) {
        state->scroll_y = state->selected_index;
    } else if (state->selected_index >= state->scroll_y + viewport_rows) {
        state->scroll_y = state->selected_index - viewport_rows + 1;
    }
    listview_clamp_scroll(state, viewport_rows);
}

static void* listview_create_state(void* params) {
    (void)params;
    TuixListViewState *state = (TuixListViewState*)calloc(1, sizeof(TuixListViewState));
    if (!state) return NULL;
    state->title = strdup("ListView");
    if (!state->title) {
        free(state);
        return NULL;
    }
    state->selected_index = -1;
    state->hovered_index = -1;
    state->follow_selection = 1;
    state->activated_index = -1;
    state->needs_redraw = 1;
    return state;
}

static void listview_destroy_state(void* state_ptr) {
    if (!state_ptr) return;
    TuixListViewState *state = (TuixListViewState*)state_ptr;
    free(state->title);
    listview_free_items(state);
    free(state->inserted_buffer);
    free(state);
}

static int listview_get_viewport_offset(TuixObject *obj, int *offset_x, int *offset_y) {
    if (!obj || !obj->state) return -1;
    TuixListViewState *state = (TuixListViewState*)obj->state;
    if (offset_x) *offset_x = 0;
    if (offset_y) *offset_y = state->scroll_y;
    return 0;
}

static int listview_get_viewport_insets(TuixObject *obj, int *left, int *top, int *right, int *bottom) {
    (void)obj;
    if (left) *left = 1;
    if (top) *top = 1;
    if (right) *right = 1;
    if (bottom) *bottom = 1;
    return 0;
}

static int listview_get_viewport_content_size(TuixObject *obj, int *content_w, int *content_h) {
    if (!obj || !obj->state) return -1;
    TuixListViewState *state = (TuixListViewState*)obj->state;
    if (content_w) *content_w = 1;
    if (content_h) *content_h = state->item_count > 0 ? state->item_count : 1;
    return 0;
}

static int listview_ensure_buffer(TuixListViewState *state, TuixBuffer *buffer) {
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

static void listview_put(TuixPixel *pixel, char ch, TuixRGBTuple fg, TuixRGBTuple bg) {
    memset(pixel, 0, sizeof(*pixel));
    pixel->sym[0] = ch;
    pixel->sym[1] = '\0';
    pixel->styles.fg = fg;
    pixel->styles.bg = bg;
    pixel->styles.custom_fg = 1;
    pixel->styles.custom_bg = 1;
}

static TuixPixel* listview_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixListViewState *state = obj ? (TuixListViewState*)obj->state : NULL;
    if (!state || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (listview_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

    int w = buffer->width;
    int h = buffer->height;
    int view_w = w >= 2 ? w - 2 : w;
    int view_h = h >= 2 ? h - 2 : h;
    if (view_w < 1) view_w = 1;
    if (view_h < 1) view_h = 1;
    if (state->follow_selection) {
        listview_ensure_visible(state, view_h);
    } else {
        listview_clamp_scroll(state, view_h);
    }

    TuixRGBTuple fg_border = {210, 220, 245};
    TuixRGBTuple fg_text = {225, 230, 240};
    TuixRGBTuple fg_muted = {145, 156, 176};
    TuixRGBTuple bg_border = {26, 32, 46};
    TuixRGBTuple bg_body = {18, 22, 32};
    TuixRGBTuple bg_hover = {34, 42, 58};
    TuixRGBTuple bg_selected = state->focused ? (TuixRGBTuple){66, 92, 164} : (TuixRGBTuple){46, 58, 90};

    size_t n = (size_t)w * (size_t)h;
    for (size_t i = 0; i < n; i++) {
        listview_put(&state->inserted_buffer[i], ' ', fg_text, bg_body);
    }

    for (int x = 0; x < w; x++) {
        listview_put(&state->inserted_buffer[(size_t)x], '-', fg_border, bg_border);
        listview_put(&state->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)x], '-', fg_border, bg_border);
    }
    for (int y = 0; y < h; y++) {
        listview_put(&state->inserted_buffer[(size_t)y * (size_t)w], '|', fg_border, bg_border);
        listview_put(&state->inserted_buffer[(size_t)y * (size_t)w + (size_t)(w - 1)], '|', fg_border, bg_border);
    }
    listview_put(&state->inserted_buffer[0], '+', fg_border, bg_border);
    listview_put(&state->inserted_buffer[(size_t)(w - 1)], '+', fg_border, bg_border);
    listview_put(&state->inserted_buffer[(size_t)(h - 1) * (size_t)w], '+', fg_border, bg_border);
    listview_put(&state->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)(w - 1)], '+', fg_border, bg_border);

    const char *title = state->title ? state->title : "ListView";
    int title_x = 2;
    for (int i = 0; title[i] && (title_x + i) < (w - 1); i++) {
        TuixPixel *pixel = &state->inserted_buffer[(size_t)(title_x + i)];
        pixel->sym[0] = title[i];
        pixel->sym[1] = '\0';
        pixel->styles.fg = fg_border;
        pixel->styles.bg = bg_border;
    }

    for (int row = 0; row < view_h; row++) {
        int item_index = state->scroll_y + row;
        int screen_y = row + 1;
        if (screen_y >= h - 1) break;

        TuixRGBTuple row_fg = fg_text;
        TuixRGBTuple row_bg = bg_body;
        if (item_index == state->selected_index) {
            row_bg = bg_selected;
        } else if (item_index == state->hovered_index) {
            row_bg = bg_hover;
        }

        for (int x = 1; x < w - 1; x++) {
            TuixPixel *pixel = &state->inserted_buffer[(size_t)screen_y * (size_t)w + (size_t)x];
            pixel->styles.fg = row_fg;
            pixel->styles.bg = row_bg;
            pixel->styles.custom_fg = 1;
            pixel->styles.custom_bg = 1;
        }

        if (item_index >= state->item_count) {
            continue;
        }

        const char *prefix = item_index == state->selected_index ? "> " : "  ";
        int x = 1;
        for (int i = 0; prefix[i] && x < w - 1; i++, x++) {
            TuixPixel *pixel = &state->inserted_buffer[(size_t)screen_y * (size_t)w + (size_t)x];
            pixel->sym[0] = prefix[i];
            pixel->sym[1] = '\0';
        }

        const char *label = state->items[item_index] ? state->items[item_index] : "";
        for (int i = 0; label[i] && x < w - 1; i++, x++) {
            TuixPixel *pixel = &state->inserted_buffer[(size_t)screen_y * (size_t)w + (size_t)x];
            pixel->sym[0] = label[i];
            pixel->sym[1] = '\0';
        }
    }

    if (state->item_count > view_h && w > 4) {
        char footer[64];
        snprintf(footer, sizeof(footer), "%d-%d / %d",
                 state->item_count > 0 ? state->scroll_y + 1 : 0,
                 state->scroll_y + view_h < state->item_count ? state->scroll_y + view_h : state->item_count,
                 state->item_count);
        int len = (int)strlen(footer);
        int start_x = w - 2 - len;
        if (start_x < 2) start_x = 2;
        for (int i = 0; footer[i] && (start_x + i) < w - 1; i++) {
            TuixPixel *pixel = &state->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)(start_x + i)];
            pixel->sym[0] = footer[i];
            pixel->sym[1] = '\0';
            pixel->styles.fg = fg_muted;
            pixel->styles.bg = bg_border;
        }
    }

    return state->inserted_buffer;
}

static int listview_pick_row(TuixBuffer *buffer, TuixListViewState *state, int col1, int row1) {
    if (!buffer || !state) return -1;
    int local_x = col1 - 1 - buffer->margin_left;
    int local_y = row1 - 1 - buffer->margin_top;
    if (local_x < 1 || local_x >= buffer->width - 1) return -1;
    if (local_y < 1 || local_y >= buffer->height - 1) return -1;
    int item_index = state->scroll_y + (local_y - 1);
    if (item_index < 0 || item_index >= state->item_count) return -1;
    return item_index;
}

static TuixHandlerResponse listview_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    TuixHandlerResponse ans = {.requires_redraw = 0};
    if (!obj || !obj->state) return ans;

    TuixListViewState *state = (TuixListViewState*)obj->state;
    if (state->focused != (is_focused ? 1 : 0)) {
        state->focused = is_focused ? 1 : 0;
        state->needs_redraw = 1;
    }

    TuixBuffer buffer;
    if (tuix_get_buffer_snapshot_by_uid(obj->uid, &buffer) != 0) {
        return ans;
    }
    int view_h = buffer.height >= 2 ? buffer.height - 2 : buffer.height;
    if (view_h < 1) view_h = 1;

    if (snap && snap->mouse) {
        int hovered_index = -1;
        if (snap->mouse->col > 0 && snap->mouse->row > 0) {
            hovered_index = listview_pick_row(&buffer, state, snap->mouse->col, snap->mouse->row);
        }
        if (hovered_index != state->hovered_index) {
            state->hovered_index = hovered_index;
            state->needs_redraw = 1;
        }
    }

    if (is_focused && has_event && snap && !snap->consumed_keyboard && snap->keyboard && snap->keyboard->has_event && state->item_count > 0) {
        int code = snap->keyboard->code;
        int changed = 0;
        if (state->selected_index < 0) state->selected_index = 0;
        if (code == TUIX_VK_UP || code == 'k') {
            if (state->selected_index > 0) {
                state->selected_index--;
                changed = 1;
            }
        } else if (code == TUIX_VK_DOWN || code == 'j') {
            if (state->selected_index < state->item_count - 1) {
                state->selected_index++;
                changed = 1;
            }
        } else if (code == TUIX_VK_PRIOR) {
            state->selected_index -= view_h;
            if (state->selected_index < 0) state->selected_index = 0;
            changed = 1;
        } else if (code == TUIX_VK_NEXT) {
            state->selected_index += view_h;
            if (state->selected_index >= state->item_count) state->selected_index = state->item_count - 1;
            changed = 1;
        } else if (code == TUIX_VK_HOME) {
            state->selected_index = 0;
            changed = 1;
        } else if (code == TUIX_VK_END) {
            state->selected_index = state->item_count - 1;
            changed = 1;
        } else if (code == TUIX_VK_ENTER || code == '\n') {
            state->activated_index = state->selected_index;
            state->follow_selection = 1;
            state->needs_redraw = 1;
            snap->consumed_keyboard = true;
        }

        if (changed) {
            state->follow_selection = 1;
            listview_ensure_visible(state, view_h);
            state->needs_redraw = 1;
            snap->consumed_keyboard = true;
        }
    }

    if (has_event && snap && !snap->consumed_mouse && snap->mouse && snap->mouse->has_event) {
        int changed = 0;
        if (snap->mouse->event == TUIX_MOUSE_SCROLL_UP) {
            state->scroll_y -= 2;
            state->follow_selection = 0;
            listview_clamp_scroll(state, view_h);
            changed = 1;
            snap->consumed_mouse = true;
        } else if (snap->mouse->event == TUIX_MOUSE_SCROLL_DOWN) {
            state->scroll_y += 2;
            state->follow_selection = 0;
            listview_clamp_scroll(state, view_h);
            changed = 1;
            snap->consumed_mouse = true;
        } else if ((snap->mouse->event == TUIX_MOUSE_RELEASE || snap->mouse->event == TUIX_MOUSE_DOUBLE_CLICK) &&
                   snap->mouse->btn == TUIX_BTN_LEFT) {
            int picked = listview_pick_row(&buffer, state, snap->mouse->col, snap->mouse->row);
            if (picked >= 0) {
                state->selected_index = picked;
                state->activated_index = picked;
                state->follow_selection = 1;
                listview_ensure_visible(state, view_h);
                changed = 1;
                snap->consumed_mouse = true;
            }
        }
        if (changed) {
            state->needs_redraw = 1;
        }
    }

    if (state->needs_redraw) {
        state->needs_redraw = 0;
        ans.requires_redraw = 1;
    }
    return ans;
}

static void listview_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixListViewState *state = (TuixListViewState*)obj->state;
    if (listview_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

const TuixBuilder tuix_listview_builder = {
    .name = "ListViewBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = listview_create_state,
    .destroy_state = listview_destroy_state,
    .on_event = listview_handler,
    .on_resize = listview_on_resize,
    .get_viewport_offset = listview_get_viewport_offset,
    .get_viewport_insets = listview_get_viewport_insets,
    .get_viewport_content_size = listview_get_viewport_content_size,
    .build_content = listview_build_content
};

const TuixBuilder* tuix_listview_init(void) {
    return &tuix_listview_builder;
}

int tuix_listview_set_title(TuixObject *obj, const char *title) {
    if (!obj || !obj->state) return -1;
    TuixListViewState *state = (TuixListViewState*)obj->state;
    char *copy = strdup(title ? title : "");
    if (!copy) return -1;
    free(state->title);
    state->title = copy;
    listview_mark_redraw(state);
    return 0;
}

int tuix_listview_set_items(TuixObject *obj, const char **items, int count) {
    if (!obj || !obj->state || !items || count < 0) return -1;
    TuixListViewState *state = (TuixListViewState*)obj->state;

    listview_free_items(state);
    if (count == 0) {
        state->selected_index = -1;
        state->hovered_index = -1;
        state->scroll_y = 0;
        state->follow_selection = 1;
        listview_mark_redraw(state);
        return 0;
    }

    state->items = (char**)calloc((size_t)count, sizeof(char*));
    if (!state->items) return -1;
    for (int i = 0; i < count; i++) {
        state->items[i] = strdup(items[i] ? items[i] : "");
        if (!state->items[i]) {
            for (int j = 0; j < i; j++) free(state->items[j]);
            free(state->items);
            state->items = NULL;
            return -1;
        }
    }

    state->item_count = count;
    state->selected_index = count > 0 ? 0 : -1;
    state->hovered_index = -1;
    state->scroll_y = 0;
    state->follow_selection = 1;
    state->activated_index = -1;
    listview_mark_redraw(state);
    return 0;
}

int tuix_listview_set_selected(TuixObject *obj, int index) {
    if (!obj || !obj->state) return -1;
    TuixListViewState *state = (TuixListViewState*)obj->state;
    if (state->item_count <= 0) {
        state->selected_index = -1;
        return 0;
    }
    if (index < 0) index = 0;
    if (index >= state->item_count) index = state->item_count - 1;
    if (state->selected_index == index) return 0;
    state->selected_index = index;
    state->follow_selection = 1;
    listview_mark_redraw(state);
    return 0;
}

int tuix_listview_get_selected(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixListViewState *state = (TuixListViewState*)obj->state;
    return state->selected_index;
}

int tuix_listview_take_activated(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixListViewState *state = (TuixListViewState*)obj->state;
    int activated = state->activated_index;
    state->activated_index = -1;
    return activated;
}
