#include "scroll_container_builder.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../buffer_manager.h"

#define TUIX_VK_LEFT  0x25
#define TUIX_VK_UP    0x26
#define TUIX_VK_RIGHT 0x27
#define TUIX_VK_DOWN  0x28
#define TUIX_VK_HOME  0x24
#define TUIX_VK_END   0x23

typedef struct {
    char *title;
    int content_w;
    int content_h;
    int offset_x;
    int offset_y;
    int drag_active;
    int drag_last_col;
    int drag_last_row;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixScrollContainerState;

static void scroll_mark_redraw(TuixScrollContainerState *s) {
    if (!s) return;
    s->needs_redraw = 1;
}

static void scroll_put(TuixPixel *p, char ch, TuixRGBTuple fg, TuixRGBTuple bg) {
    memset(p, 0, sizeof(*p));
    p->sym[0] = ch;
    p->sym[1] = '\0';
    p->styles.fg = fg;
    p->styles.bg = bg;
    p->styles.custom_fg = 1;
    p->styles.custom_bg = 1;
}

static int scroll_ensure_buffer(TuixScrollContainerState *s, TuixBuffer *buffer) {
    if (!s || !buffer || buffer->width <= 0 || buffer->height <= 0) {
        return -1;
    }
    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    size_t required = sizeof(TuixPixel) * n;
    if (s->inserted_buffer && (size_t)s->inserted_buffer_size == required) {
        return 0;
    }
    if (s->inserted_buffer) {
        free(s->inserted_buffer);
        s->inserted_buffer = NULL;
        s->inserted_buffer_size = 0;
    }
    s->inserted_buffer = calloc(n, sizeof(TuixPixel));
    if (!s->inserted_buffer) {
        return -1;
    }
    s->inserted_buffer_size = (int)required;
    s->needs_redraw = 1;
    buffer->required_redraw = 1;
    return 0;
}

static void scroll_clamp_offset(TuixScrollContainerState *s, int view_w, int view_h) {
    if (!s) return;
    int max_x = s->content_w - view_w;
    int max_y = s->content_h - view_h;
    if (max_x < 0) max_x = 0;
    if (max_y < 0) max_y = 0;
    if (s->offset_x < 0) s->offset_x = 0;
    if (s->offset_y < 0) s->offset_y = 0;
    if (s->offset_x > max_x) s->offset_x = max_x;
    if (s->offset_y > max_y) s->offset_y = max_y;
}

static void* scroll_create_state(void* params) {
    (void)params;
    TuixScrollContainerState *s = calloc(1, sizeof(TuixScrollContainerState));
    if (!s) return NULL;
    s->title = strdup("ScrollContainer");
    s->content_w = 240;
    s->content_h = 120;
    s->offset_x = 0;
    s->offset_y = 0;
    s->drag_active = 0;
    s->drag_last_col = 0;
    s->drag_last_row = 0;
    s->needs_redraw = 1;
    return s;
}

static void scroll_destroy_state(void* state) {
    if (!state) return;
    TuixScrollContainerState *s = (TuixScrollContainerState*)state;
    free(s->title);
    free(s);
}

static TuixPixel* scroll_build_content(TuixObject *obj, TuixBuffer *buffer) {
    TuixScrollContainerState *s = obj ? (TuixScrollContainerState*)obj->state : NULL;
    if (!buffer || buffer->width <= 0 || buffer->height <= 0) {
        return NULL;
    }

    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    if (!s) {
        TuixPixel *px = calloc(n, sizeof(TuixPixel));
        if (!px) { printf("OOM\n"); exit(1); }
        for (size_t i = 0; i < n; i++) {
            px[i].sym[0] = ' ';
            px[i].sym[1] = '\0';
            px[i].styles.fg = (TuixRGBTuple){255, 255, 255};
            px[i].styles.bg = (TuixRGBTuple){0, 0, 0};
            px[i].styles.custom_bg = 1;
            px[i].styles.custom_fg = 1;
        }
        return px;
    }

    if (scroll_ensure_buffer(s, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

    int w = buffer->width;
    int h = buffer->height;
    int view_w = w >= 2 ? (w - 2) : w;
    /* Body rows are [1, h-2], so viewport height is h-2 (top/bottom borders excluded). */
    int view_h = h >= 2 ? (h - 2) : h;
    if (view_w < 1) view_w = 1;
    if (view_h < 1) view_h = 1;
    scroll_clamp_offset(s, view_w, view_h);

    TuixRGBTuple fg_border = {210, 220, 245};
    TuixRGBTuple fg_text = {225, 230, 240};
    TuixRGBTuple fg_hint = {150, 160, 180};
    TuixRGBTuple bg_border = {28, 34, 48};
    TuixRGBTuple bg_body = {18, 22, 32};
    TuixRGBTuple bg_alt = {26, 30, 44};

    for (size_t i = 0; i < n; i++) {
        scroll_put(&s->inserted_buffer[i], ' ', fg_text, bg_body);
    }

    for (int x = 0; x < w; x++) {
        scroll_put(&s->inserted_buffer[x], '-', fg_border, bg_border);
        scroll_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)x], '-', fg_hint, bg_border);
    }
    for (int y = 0; y < h; y++) {
        scroll_put(&s->inserted_buffer[(size_t)y * (size_t)w], '|', fg_border, bg_border);
        scroll_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)(w - 1)], '|', fg_border, bg_border);
    }
    scroll_put(&s->inserted_buffer[0], '+', fg_border, bg_border);
    scroll_put(&s->inserted_buffer[(size_t)(w - 1)], '+', fg_border, bg_border);
    scroll_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w], '+', fg_hint, bg_border);
    scroll_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)(w - 1)], '+', fg_hint, bg_border);

    const char *title = s->title ? s->title : "ScrollContainer";
    int title_len = (int)strlen(title);
    int title_x = 2;
    for (int i = 0; i < title_len && (title_x + i) < w - 1; i++) {
        TuixPixel *p = &s->inserted_buffer[(size_t)(title_x + i)];
        p->sym[0] = title[i];
        p->sym[1] = '\0';
        p->styles.fg = fg_border;
        p->styles.bg = bg_border;
    }

    int body_y0 = 1;
    int body_y1 = h - 1;
    static const char *lorem =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. "
        "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. ";
    int lorem_len = (int)strlen(lorem);

    for (int y = body_y0; y < body_y1; y++) {
        for (int x = 1; x < w - 1; x++) {
            int virtual_x = s->offset_x + (x - 1);
            int virtual_y = s->offset_y + (y - 1);
            int row_seed = (virtual_y * 53) % lorem_len;
            if (row_seed < 0) row_seed += lorem_len;
            int idx = (row_seed + virtual_x) % lorem_len;
            if (idx < 0) idx += lorem_len;
            char ch = lorem[idx];
            TuixRGBTuple bg = ((virtual_y / 2) % 2 == 0) ? bg_body : bg_alt;
            scroll_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)x], ch, fg_text, bg);
        }
    }

    char info[96];
    snprintf(info, sizeof(info), "ofs(%d,%d) view(%d,%d) content(%d,%d)",
             s->offset_x, s->offset_y, view_w, view_h, s->content_w, s->content_h);
    int info_len = (int)strlen(info);
    int info_x = 2;
    for (int i = 0; i < info_len && (info_x + i) < w - 1; i++) {
        TuixPixel *p = &s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)(info_x + i)];
        p->sym[0] = info[i];
        p->sym[1] = '\0';
        p->styles.fg = fg_hint;
        p->styles.bg = bg_border;
    }

    return s->inserted_buffer;
}

static TuixHandlerResponse scroll_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    TuixHandlerResponse ans = {.requires_redraw = 0};
    if (!obj || !obj->state) return ans;

    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;

    if (is_focused && has_event && snap && !snap->consumed_keyboard && snap->keyboard && snap->keyboard->has_event) {
        int code = snap->keyboard->code;
        int changed = 0;
        if (code == TUIX_VK_LEFT || code == 'h') {
            s->offset_x -= 2;
            changed = 1;
        } else if (code == TUIX_VK_RIGHT || code == 'l') {
            s->offset_x += 2;
            changed = 1;
        } else if (code == TUIX_VK_UP || code == 'k') {
            s->offset_y -= 1;
            changed = 1;
        } else if (code == TUIX_VK_DOWN || code == 'j') {
            s->offset_y += 1;
            changed = 1;
        } else if (code == TUIX_VK_HOME) {
            s->offset_x = 0;
            s->offset_y = 0;
            changed = 1;
        } else if (code == TUIX_VK_END) {
            s->offset_x = s->content_w;
            s->offset_y = s->content_h;
            changed = 1;
        }
        if (changed) {
            scroll_mark_redraw(s);
            snap->consumed_keyboard = true;
        }
    }

    if (has_event && snap && !snap->consumed_mouse && snap->mouse && snap->mouse->has_event) {
        int changed = 0;
        if (snap->mouse->event == TUIX_MOUSE_PRESS && snap->mouse->btn == TUIX_BTN_LEFT) {
            s->drag_active = 1;
            s->drag_last_col = snap->mouse->col;
            s->drag_last_row = snap->mouse->row;
            snap->consumed_mouse = true;
        } else if (snap->mouse->event == TUIX_MOUSE_RELEASE && snap->mouse->btn == TUIX_BTN_LEFT) {
            if (s->drag_active) {
                s->drag_active = 0;
                snap->consumed_mouse = true;
            }
        } else if (snap->mouse->event == TUIX_MOUSE_DRAG && s->drag_active) {
            int dx = snap->mouse->col - s->drag_last_col;
            int dy = snap->mouse->row - s->drag_last_row;
            if (dx != 0 || dy != 0) {
                /* Dragging the viewport follows pointer movement direction. */
                s->offset_x -= dx;
                s->offset_y -= dy;
                changed = 1;
            }
            s->drag_last_col = snap->mouse->col;
            s->drag_last_row = snap->mouse->row;
            snap->consumed_mouse = true;
        } else if (snap->mouse->event == TUIX_MOUSE_SCROLL_UP) {
            s->offset_y -= 2;
            changed = 1;
        } else if (snap->mouse->event == TUIX_MOUSE_SCROLL_DOWN) {
            s->offset_y += 2;
            changed = 1;
        } else if (snap->mouse->event == TUIX_MOUSE_HSCROLL_LEFT) {
            s->offset_x -= 3;
            changed = 1;
        } else if (snap->mouse->event == TUIX_MOUSE_HSCROLL_RIGHT) {
            s->offset_x += 3;
            changed = 1;
        }
        if (changed) {
            scroll_mark_redraw(s);
            snap->consumed_mouse = true;
        }
    }

    if (s->needs_redraw) {
        s->needs_redraw = 0;
        ans.requires_redraw = 1;
    }
    return ans;
}

static void scroll_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    if (scroll_ensure_buffer(s, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

const TuixBuilder tuix_scroll_container_builder = {
    .name = "ScrollContainerBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = scroll_create_state,
    .destroy_state = scroll_destroy_state,
    .on_event = scroll_handler,
    .on_resize = scroll_on_resize,
    .build_content = scroll_build_content
};

const TuixBuilder* tuix_scroll_container_init(void) {
    return &tuix_scroll_container_builder;
}

int tuix_scroll_container_set_title(TuixObject *obj, const char *title) {
    if (!obj || !obj->state) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    char *copy = strdup(title ? title : "");
    if (!copy) return -1;
    free(s->title);
    s->title = copy;
    scroll_mark_redraw(s);
    return 0;
}

int tuix_scroll_container_set_content_size(TuixObject *obj, int content_w, int content_h) {
    if (!obj || !obj->state) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    if (content_w < 1) content_w = 1;
    if (content_h < 1) content_h = 1;
    s->content_w = content_w;
    s->content_h = content_h;
    scroll_mark_redraw(s);
    return 0;
}

int tuix_scroll_container_set_offset(TuixObject *obj, int offset_x, int offset_y) {
    if (!obj || !obj->state) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    s->offset_x = offset_x;
    s->offset_y = offset_y;
    scroll_mark_redraw(s);
    return 0;
}

int tuix_scroll_container_get_offset_x(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    return s->offset_x;
}

int tuix_scroll_container_get_offset_y(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    return s->offset_y;
}