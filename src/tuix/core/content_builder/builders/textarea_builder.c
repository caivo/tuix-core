#include "textarea_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TUIX_VK_LEFT   0x25
#define TUIX_VK_UP     0x26
#define TUIX_VK_RIGHT  0x27
#define TUIX_VK_DOWN   0x28
#define TUIX_VK_HOME   0x24
#define TUIX_VK_END    0x23
#define TUIX_VK_PRIOR  0x21
#define TUIX_VK_NEXT   0x22
#define TUIX_VK_BACK   0x08
#define TUIX_VK_DELETE 0x2E
#define TUIX_VK_ENTER  0x0D

typedef struct {
    char *title;
    char *text;
    int text_len;
    int text_cap;
    int cursor;
    int preferred_column;
    int scroll_x;
    int scroll_y;
    int follow_cursor;
    int readonly;
    int focused;
    int needs_redraw;
    char *placeholder;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixTextAreaState;

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static int min_int(int a, int b) {
    return a < b ? a : b;
}

static void textarea_mark_redraw(TuixTextAreaState *state) {
    if (!state) return;
    state->needs_redraw = 1;
}

static int textarea_ensure_cap(TuixTextAreaState *state, int extra) {
    if (!state) return -1;
    if (state->text_len + extra + 1 < state->text_cap) {
        return 0;
    }
    int new_cap = state->text_cap > 0 ? state->text_cap : 128;
    while (new_cap <= state->text_len + extra + 1) {
        new_cap *= 2;
    }
    char *tmp = (char*)realloc(state->text, (size_t)new_cap);
    if (!tmp) return -1;
    state->text = tmp;
    state->text_cap = new_cap;
    return 0;
}

static int textarea_line_count(const TuixTextAreaState *state) {
    if (!state || state->text_len <= 0) return 1;
    int count = 1;
    for (int i = 0; i < state->text_len; i++) {
        if (state->text[i] == '\n') count++;
    }
    return count;
}

static int textarea_max_line_width(const TuixTextAreaState *state) {
    if (!state || !state->text || state->text_len <= 0) return 1;
    int max_width = 1;
    int current = 0;
    for (int i = 0; i < state->text_len; i++) {
        if (state->text[i] == '\n') {
            if (current > max_width) max_width = current;
            current = 0;
        } else {
            current++;
        }
    }
    if (current > max_width) max_width = current;
    return max_width;
}

static int textarea_find_line_start(const TuixTextAreaState *state, int offset) {
    if (!state || !state->text) return 0;
    if (offset < 0) offset = 0;
    if (offset > state->text_len) offset = state->text_len;
    while (offset > 0 && state->text[offset - 1] != '\n') {
        offset--;
    }
    return offset;
}

static int textarea_find_line_end(const TuixTextAreaState *state, int offset) {
    if (!state || !state->text) return 0;
    if (offset < 0) offset = 0;
    if (offset > state->text_len) offset = state->text_len;
    while (offset < state->text_len && state->text[offset] != '\n') {
        offset++;
    }
    return offset;
}

static void textarea_offset_to_line_col(const TuixTextAreaState *state, int offset, int *out_line, int *out_col) {
    int line = 0;
    int col = 0;
    if (!state || !state->text) {
        if (out_line) *out_line = 0;
        if (out_col) *out_col = 0;
        return;
    }

    if (offset < 0) offset = 0;
    if (offset > state->text_len) offset = state->text_len;

    for (int i = 0; i < offset; i++) {
        if (state->text[i] == '\n') {
            line++;
            col = 0;
        } else {
            col++;
        }
    }

    if (out_line) *out_line = line;
    if (out_col) *out_col = col;
}

static int textarea_line_col_to_offset(const TuixTextAreaState *state, int target_line, int target_col) {
    if (!state || !state->text) return 0;
    if (target_line < 0) target_line = 0;
    if (target_col < 0) target_col = 0;

    int line = 0;
    int i = 0;
    while (line < target_line && i < state->text_len) {
        if (state->text[i] == '\n') {
            line++;
        }
        i++;
    }

    int line_start = i;
    int line_end = textarea_find_line_end(state, line_start);
    int line_len = line_end - line_start;
    if (target_col > line_len) target_col = line_len;
    return line_start + target_col;
}

static void textarea_update_preferred_column(TuixTextAreaState *state) {
    if (!state) return;
    int col = 0;
    textarea_offset_to_line_col(state, state->cursor, NULL, &col);
    state->preferred_column = col;
}

static void textarea_ensure_cursor_visible(TuixTextAreaState *state, int view_w, int view_h) {
    if (!state) return;
    if (view_w < 1) view_w = 1;
    if (view_h < 1) view_h = 1;
    int line = 0, col = 0;
    textarea_offset_to_line_col(state, state->cursor, &line, &col);

    if (col < state->scroll_x) {
        state->scroll_x = col;
    } else if (col >= state->scroll_x + view_w) {
        state->scroll_x = col - view_w + 1;
    }

    if (line < state->scroll_y) {
        state->scroll_y = line;
    } else if (line >= state->scroll_y + view_h) {
        state->scroll_y = line - view_h + 1;
    }

    if (state->scroll_x < 0) state->scroll_x = 0;
    if (state->scroll_y < 0) state->scroll_y = 0;
}

static void textarea_clamp_scroll(TuixTextAreaState *state, int view_w, int view_h) {
    if (!state) return;
    if (view_w < 1) view_w = 1;
    if (view_h < 1) view_h = 1;

    int max_x = textarea_max_line_width(state) - view_w;
    int max_y = textarea_line_count(state) - view_h;
    if (max_x < 0) max_x = 0;
    if (max_y < 0) max_y = 0;

    if (state->scroll_x < 0) state->scroll_x = 0;
    if (state->scroll_y < 0) state->scroll_y = 0;
    if (state->scroll_x > max_x) state->scroll_x = max_x;
    if (state->scroll_y > max_y) state->scroll_y = max_y;
}

static int textarea_move_vertical(TuixTextAreaState *state, int delta_lines) {
    if (!state || delta_lines == 0) return 0;
    int line = 0, col = 0;
    textarea_offset_to_line_col(state, state->cursor, &line, &col);
    int target_line = line + delta_lines;
    if (target_line < 0) target_line = 0;
    int max_line = textarea_line_count(state) - 1;
    if (target_line > max_line) target_line = max_line;

    int target_col = state->preferred_column >= 0 ? state->preferred_column : col;
    int new_cursor = textarea_line_col_to_offset(state, target_line, target_col);
    if (new_cursor == state->cursor) return 0;
    state->cursor = new_cursor;
    return 1;
}

static int textarea_insert_char(TuixTextAreaState *state, char ch) {
    if (!state) return -1;
    if (textarea_ensure_cap(state, 1) != 0) return -1;
    memmove(state->text + state->cursor + 1,
            state->text + state->cursor,
            (size_t)(state->text_len - state->cursor + 1));
    state->text[state->cursor] = ch;
    state->text_len++;
    state->cursor++;
    return 0;
}

static int textarea_backspace(TuixTextAreaState *state) {
    if (!state || state->cursor <= 0) return 0;
    memmove(state->text + state->cursor - 1,
            state->text + state->cursor,
            (size_t)(state->text_len - state->cursor + 1));
    state->text_len--;
    state->cursor--;
    return 1;
}

static int textarea_delete_forward(TuixTextAreaState *state) {
    if (!state || state->cursor >= state->text_len) return 0;
    memmove(state->text + state->cursor,
            state->text + state->cursor + 1,
            (size_t)(state->text_len - state->cursor));
    state->text_len--;
    return 1;
}

static void* textarea_create_state(void* params) {
    (void)params;
    TuixTextAreaState *state = (TuixTextAreaState*)calloc(1, sizeof(TuixTextAreaState));
    if (!state) return NULL;
    state->title = strdup("TextArea");
    state->text_cap = 256;
    state->text = (char*)calloc((size_t)state->text_cap, 1);
    if (!state->title || !state->text) {
        free(state->title);
        free(state->text);
        free(state);
        return NULL;
    }
    state->preferred_column = 0;
    state->follow_cursor = 1;
    state->needs_redraw = 1;
    return state;
}

static void textarea_destroy_state(void* state_ptr) {
    if (!state_ptr) return;
    TuixTextAreaState *state = (TuixTextAreaState*)state_ptr;
    free(state->title);
    free(state->text);
    free(state->placeholder);
    free(state->inserted_buffer);
    free(state);
}

static int textarea_get_viewport_offset(TuixObject *obj, int *offset_x, int *offset_y) {
    if (!obj || !obj->state) return -1;
    TuixTextAreaState *state = (TuixTextAreaState*)obj->state;
    if (offset_x) *offset_x = state->scroll_x;
    if (offset_y) *offset_y = state->scroll_y;
    return 0;
}

static int textarea_get_viewport_insets(TuixObject *obj, int *left, int *top, int *right, int *bottom) {
    (void)obj;
    if (left) *left = 1;
    if (top) *top = 1;
    if (right) *right = 1;
    if (bottom) *bottom = 1;
    return 0;
}

static int textarea_get_viewport_content_size(TuixObject *obj, int *content_w, int *content_h) {
    if (!obj || !obj->state) return -1;
    TuixTextAreaState *state = (TuixTextAreaState*)obj->state;
    if (content_w) *content_w = textarea_max_line_width(state);
    if (content_h) *content_h = textarea_line_count(state);
    return 0;
}

static int textarea_ensure_buffer(TuixTextAreaState *state, TuixBuffer *buffer) {
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

static void textarea_put(TuixPixel *pixel, char ch, TuixRGBTuple fg, TuixRGBTuple bg) {
    memset(pixel, 0, sizeof(*pixel));
    pixel->sym[0] = ch;
    pixel->sym[1] = '\0';
    pixel->styles.fg = fg;
    pixel->styles.bg = bg;
    pixel->styles.custom_fg = 1;
    pixel->styles.custom_bg = 1;
}

static const char* textarea_get_line_text(const TuixTextAreaState *state, int line_index, int *out_len) {
    static const char empty[] = "";
    if (!state || !state->text || line_index < 0) {
        if (out_len) *out_len = 0;
        return empty;
    }
    int current_line = 0;
    int start = 0;
    for (int i = 0; i < state->text_len; i++) {
        if (current_line == line_index) break;
        if (state->text[i] == '\n') {
            current_line++;
            start = i + 1;
        }
    }
    if (current_line != line_index) {
        if (out_len) *out_len = 0;
        return empty;
    }
    int end = textarea_find_line_end(state, start);
    if (out_len) *out_len = end - start;
    return state->text + start;
}

static TuixPixel* textarea_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixTextAreaState *state = obj ? (TuixTextAreaState*)obj->state : NULL;
    if (!state || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (textarea_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

    int w = buffer->width;
    int h = buffer->height;
    int view_w = w >= 2 ? w - 2 : w;
    int view_h = h >= 2 ? h - 2 : h;
    if (view_w < 1) view_w = 1;
    if (view_h < 1) view_h = 1;
    if (state->follow_cursor) {
        textarea_ensure_cursor_visible(state, view_w, view_h);
    } else {
        textarea_clamp_scroll(state, view_w, view_h);
    }

    TuixRGBTuple fg_border = {210, 220, 245};
    TuixRGBTuple fg_text = {225, 230, 240};
    TuixRGBTuple fg_placeholder = {118, 126, 142};
    TuixRGBTuple fg_cursor = {0, 0, 0};
    TuixRGBTuple bg_border = {26, 32, 46};
    TuixRGBTuple bg_body = {18, 22, 32};
    TuixRGBTuple bg_cursor = {234, 236, 242};

    size_t n = (size_t)w * (size_t)h;
    for (size_t i = 0; i < n; i++) {
        textarea_put(&state->inserted_buffer[i], ' ', fg_text, bg_body);
    }

    for (int x = 0; x < w; x++) {
        textarea_put(&state->inserted_buffer[(size_t)x], '-', fg_border, bg_border);
        textarea_put(&state->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)x], '-', fg_border, bg_border);
    }
    for (int y = 0; y < h; y++) {
        textarea_put(&state->inserted_buffer[(size_t)y * (size_t)w], '|', fg_border, bg_border);
        textarea_put(&state->inserted_buffer[(size_t)y * (size_t)w + (size_t)(w - 1)], '|', fg_border, bg_border);
    }
    textarea_put(&state->inserted_buffer[0], '+', fg_border, bg_border);
    textarea_put(&state->inserted_buffer[(size_t)(w - 1)], '+', fg_border, bg_border);
    textarea_put(&state->inserted_buffer[(size_t)(h - 1) * (size_t)w], '+', fg_border, bg_border);
    textarea_put(&state->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)(w - 1)], '+', fg_border, bg_border);

    const char *title = state->title ? state->title : "TextArea";
    for (int i = 0; title[i] && (2 + i) < w - 1; i++) {
        TuixPixel *pixel = &state->inserted_buffer[(size_t)(2 + i)];
        pixel->sym[0] = title[i];
        pixel->sym[1] = '\0';
        pixel->styles.fg = fg_border;
        pixel->styles.bg = bg_border;
    }

    int total_lines = textarea_line_count(state);
    int show_placeholder = state->text_len == 0 && state->placeholder && state->placeholder[0];

    for (int row = 0; row < view_h; row++) {
        int line_index = state->scroll_y + row;
        int screen_y = row + 1;
        if (screen_y >= h - 1) break;

        if (show_placeholder && row == 0) {
            for (int x = 0; state->placeholder[x] && x < view_w; x++) {
                TuixPixel *pixel = &state->inserted_buffer[(size_t)screen_y * (size_t)w + (size_t)(x + 1)];
                pixel->sym[0] = state->placeholder[x];
                pixel->sym[1] = '\0';
                pixel->styles.fg = fg_placeholder;
            }
            continue;
        }

        if (line_index >= total_lines) {
            continue;
        }

        int line_len = 0;
        const char *line_text = textarea_get_line_text(state, line_index, &line_len);
        int start_col = state->scroll_x;
        if (start_col > line_len) start_col = line_len;
        for (int x = 0; x < view_w && (start_col + x) < line_len; x++) {
            TuixPixel *pixel = &state->inserted_buffer[(size_t)screen_y * (size_t)w + (size_t)(x + 1)];
            pixel->sym[0] = line_text[start_col + x];
            pixel->sym[1] = '\0';
            pixel->styles.fg = fg_text;
        }
    }

    if (state->focused) {
        int cursor_line = 0;
        int cursor_col = 0;
        textarea_offset_to_line_col(state, state->cursor, &cursor_line, &cursor_col);
        int local_y = cursor_line - state->scroll_y;
        int local_x = cursor_col - state->scroll_x;
        if (local_x >= 0 && local_x < view_w && local_y >= 0 && local_y < view_h) {
            int screen_x = local_x + 1;
            int screen_y = local_y + 1;
            TuixPixel *pixel = &state->inserted_buffer[(size_t)screen_y * (size_t)w + (size_t)screen_x];
            pixel->styles.bg = bg_cursor;
            pixel->styles.fg = fg_cursor;
            if (pixel->sym[0] == '\0') {
                pixel->sym[0] = ' ';
                pixel->sym[1] = '\0';
            }
        }
    }

    return state->inserted_buffer;
}

static int textarea_pick_cursor(TuixTextAreaState *state, TuixBuffer *buffer, int col1, int row1) {
    if (!state || !buffer) return -1;
    int local_x = col1 - 1 - buffer->margin_left;
    int local_y = row1 - 1 - buffer->margin_top;
    if (local_x < 1 || local_x >= buffer->width - 1) return -1;
    if (local_y < 1 || local_y >= buffer->height - 1) return -1;
    int target_line = state->scroll_y + (local_y - 1);
    int target_col = state->scroll_x + (local_x - 1);
    return textarea_line_col_to_offset(state, target_line, target_col);
}

static TuixHandlerResponse textarea_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    TuixHandlerResponse ans = {.requires_redraw = 0};
    if (!obj || !obj->state) return ans;

    TuixTextAreaState *state = (TuixTextAreaState*)obj->state;
    if (state->focused != (is_focused ? 1 : 0)) {
        state->focused = is_focused ? 1 : 0;
        state->needs_redraw = 1;
    }

    TuixBuffer buffer;
    if (tuix_get_buffer_snapshot_by_uid(obj->uid, &buffer) != 0) {
        return ans;
    }

    int view_w = buffer.width >= 2 ? buffer.width - 2 : buffer.width;
    int view_h = buffer.height >= 2 ? buffer.height - 2 : buffer.height;
    if (view_w < 1) view_w = 1;
    if (view_h < 1) view_h = 1;

    if (has_event && snap && !snap->consumed_mouse && snap->mouse && snap->mouse->has_event) {
        int changed = 0;
        if (snap->mouse->event == TUIX_MOUSE_SCROLL_UP) {
            state->scroll_y -= 2;
            state->follow_cursor = 0;
            if (state->scroll_y < 0) state->scroll_y = 0;
            changed = 1;
            snap->consumed_mouse = true;
        } else if (snap->mouse->event == TUIX_MOUSE_SCROLL_DOWN) {
            state->scroll_y += 2;
            state->follow_cursor = 0;
            changed = 1;
            snap->consumed_mouse = true;
        } else if ((snap->mouse->event == TUIX_MOUSE_PRESS || snap->mouse->event == TUIX_MOUSE_RELEASE) &&
                   snap->mouse->btn == TUIX_BTN_LEFT) {
            int picked = textarea_pick_cursor(state, &buffer, snap->mouse->col, snap->mouse->row);
            if (picked >= 0) {
                state->cursor = picked;
                state->follow_cursor = 1;
                textarea_update_preferred_column(state);
                changed = 1;
                snap->consumed_mouse = true;
            }
        }
        if (changed) {
            if (state->follow_cursor) {
                textarea_ensure_cursor_visible(state, view_w, view_h);
            } else {
                textarea_clamp_scroll(state, view_w, view_h);
            }
            state->needs_redraw = 1;
        }
    }

    if (is_focused && has_event && snap && !snap->consumed_keyboard &&
        snap->keyboard && snap->keyboard->has_event) {
        int code = snap->keyboard->code;
        int changed = 0;
        int handled = 0;

        if (code == TUIX_VK_LEFT) {
            if (state->cursor > 0) {
                state->cursor--;
                changed = 1;
            }
            state->follow_cursor = 1;
            textarea_update_preferred_column(state);
            handled = 1;
        } else if (code == TUIX_VK_RIGHT) {
            if (state->cursor < state->text_len) {
                state->cursor++;
                changed = 1;
            }
            state->follow_cursor = 1;
            textarea_update_preferred_column(state);
            handled = 1;
        } else if (code == TUIX_VK_UP) {
            state->follow_cursor = 1;
            changed = textarea_move_vertical(state, -1);
            handled = 1;
        } else if (code == TUIX_VK_DOWN) {
            state->follow_cursor = 1;
            changed = textarea_move_vertical(state, 1);
            handled = 1;
        } else if (code == TUIX_VK_PRIOR) {
            state->follow_cursor = 1;
            changed = textarea_move_vertical(state, -view_h);
            handled = 1;
        } else if (code == TUIX_VK_NEXT) {
            state->follow_cursor = 1;
            changed = textarea_move_vertical(state, view_h);
            handled = 1;
        } else if (code == TUIX_VK_HOME) {
            state->cursor = textarea_find_line_start(state, state->cursor);
            state->follow_cursor = 1;
            textarea_update_preferred_column(state);
            changed = 1;
            handled = 1;
        } else if (code == TUIX_VK_END) {
            state->cursor = textarea_find_line_end(state, state->cursor);
            state->follow_cursor = 1;
            textarea_update_preferred_column(state);
            changed = 1;
            handled = 1;
        } else if (!state->readonly && (code == TUIX_VK_BACK || code == 127)) {
            state->follow_cursor = 1;
            changed = textarea_backspace(state);
            textarea_update_preferred_column(state);
            handled = 1;
        } else if (!state->readonly && code == TUIX_VK_DELETE) {
            state->follow_cursor = 1;
            changed = textarea_delete_forward(state);
            handled = 1;
        } else if (!state->readonly && (code == TUIX_VK_ENTER || code == '\n')) {
            state->follow_cursor = 1;
            changed = (textarea_insert_char(state, '\n') == 0);
            textarea_update_preferred_column(state);
            handled = 1;
        } else if (!state->readonly) {
            char ch = snap->keyboard->text[0];
            if (ch && ch != '\n' && ch != '\r' && isprint((unsigned char)ch)) {
                state->follow_cursor = 1;
                changed = (textarea_insert_char(state, ch) == 0);
                textarea_update_preferred_column(state);
                handled = 1;
            } else if (code >= 32 && code < 127) {
                state->follow_cursor = 1;
                changed = (textarea_insert_char(state, (char)code) == 0);
                textarea_update_preferred_column(state);
                handled = 1;
            }
        }

        if (handled) {
            snap->consumed_keyboard = true;
        }
        if (changed) {
            textarea_ensure_cursor_visible(state, view_w, view_h);
            state->needs_redraw = 1;
        }
    }

    if (state->needs_redraw) {
        state->needs_redraw = 0;
        ans.requires_redraw = 1;
    }
    return ans;
}

static void textarea_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixTextAreaState *state = (TuixTextAreaState*)obj->state;
    if (textarea_ensure_buffer(state, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

const TuixBuilder tuix_textarea_builder = {
    .name = "TextAreaBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = textarea_create_state,
    .destroy_state = textarea_destroy_state,
    .on_event = textarea_handler,
    .on_resize = textarea_on_resize,
    .get_viewport_offset = textarea_get_viewport_offset,
    .get_viewport_insets = textarea_get_viewport_insets,
    .get_viewport_content_size = textarea_get_viewport_content_size,
    .build_content = textarea_build_content
};

const TuixBuilder* tuix_textarea_init(void) {
    return &tuix_textarea_builder;
}

int tuix_textarea_set_title(TuixObject *obj, const char *title) {
    if (!obj || !obj->state) return -1;
    TuixTextAreaState *state = (TuixTextAreaState*)obj->state;
    char *copy = strdup(title ? title : "");
    if (!copy) return -1;
    free(state->title);
    state->title = copy;
    textarea_mark_redraw(state);
    return 0;
}

int tuix_textarea_set_text(TuixObject *obj, const char *text) {
    if (!obj || !obj->state) return -1;
    TuixTextAreaState *state = (TuixTextAreaState*)obj->state;
    const char *src = text ? text : "";
    int len = (int)strlen(src);
    if (textarea_ensure_cap(state, len) != 0) return -1;
    memcpy(state->text, src, (size_t)len + 1);
    state->text_len = len;
    state->cursor = len;
    state->preferred_column = 0;
    state->scroll_x = 0;
    state->scroll_y = 0;
    state->follow_cursor = 1;
    textarea_update_preferred_column(state);
    textarea_mark_redraw(state);
    return 0;
}

const char* tuix_textarea_get_text(TuixObject *obj) {
    if (!obj || !obj->state) return "";
    TuixTextAreaState *state = (TuixTextAreaState*)obj->state;
    return state->text ? state->text : "";
}

int tuix_textarea_set_placeholder(TuixObject *obj, const char *text) {
    if (!obj || !obj->state) return -1;
    TuixTextAreaState *state = (TuixTextAreaState*)obj->state;
    char *copy = text ? strdup(text) : NULL;
    if (text && !copy) return -1;
    free(state->placeholder);
    state->placeholder = copy;
    textarea_mark_redraw(state);
    return 0;
}

int tuix_textarea_set_readonly(TuixObject *obj, int readonly) {
    if (!obj || !obj->state) return -1;
    TuixTextAreaState *state = (TuixTextAreaState*)obj->state;
    readonly = readonly ? 1 : 0;
    if (state->readonly == readonly) return 0;
    state->readonly = readonly;
    textarea_mark_redraw(state);
    return 0;
}
