#include "menu_builder.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../buffer_manager.h"

#define TUIX_VK_UP    0x26
#define TUIX_VK_DOWN  0x28
#define TUIX_VK_HOME  0x24
#define TUIX_VK_END   0x23
#define TUIX_VK_ENTER 0x0D

typedef struct {
    char **items;
    int item_count;
    int selected;
    int activated;
    char *title;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixMenuState;

static void menu_mark_redraw(TuixMenuState *s) {
    if (!s) return;
    s->needs_redraw = 1;
}

static void menu_put(TuixPixel *p, char ch, TuixRGBTuple fg, TuixRGBTuple bg) {
    memset(p, 0, sizeof(*p));
    p->sym[0] = ch;
    p->sym[1] = '\0';
    p->styles.fg = fg;
    p->styles.bg = bg;
    p->styles.custom_fg = 1;
    p->styles.custom_bg = 1;
}

static int menu_ensure_buffer(TuixMenuState *s, TuixBuffer *buffer) {
    if (!s || !buffer || buffer->width <= 0 || buffer->height <= 0) {
        return -1;
    }

    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    size_t required_bytes = sizeof(TuixPixel) * n;
    if (s->inserted_buffer && (size_t)s->inserted_buffer_size == required_bytes) {
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
    s->inserted_buffer_size = (int)required_bytes;
    menu_mark_redraw(s);
    buffer->required_redraw = 1;
    return 0;
}

static void* menu_create_state(void* params) {
    (void)params;
    TuixMenuState *s = calloc(1, sizeof(TuixMenuState));
    if (!s) return NULL;
    s->title = strdup("Menu");
    s->selected = 0;
    s->activated = -1;
    s->needs_redraw = 1;
    return s;
}

static void menu_destroy_state(void* state) {
    if (!state) return;
    TuixMenuState *s = (TuixMenuState*)state;
    for (int i = 0; i < s->item_count; i++) {
        free(s->items[i]);
    }
    free(s->items);
    free(s->title);
    free(s->inserted_buffer);
    free(s);
}

static TuixPixel* menu_build_content(TuixObject *obj, TuixBuffer *buffer) {
    TuixMenuState *s = obj ? (TuixMenuState*)obj->state : NULL;
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

    if (menu_ensure_buffer(s, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }

    int w = buffer->width;
    int h = buffer->height;
    TuixRGBTuple bg_panel = {26, 28, 36};
    TuixRGBTuple bg_border = {18, 20, 28};
    TuixRGBTuple bg_selected = {70, 112, 210};
    TuixRGBTuple bg_footer = {34, 38, 50};
    TuixRGBTuple fg_panel = {230, 232, 238};
    TuixRGBTuple fg_selected = {255, 255, 255};
    TuixRGBTuple fg_title = {255, 220, 140};
    TuixRGBTuple fg_hint = {145, 155, 175};

    for (size_t i = 0; i < n; i++) {
        menu_put(&s->inserted_buffer[i], ' ', fg_panel, bg_panel);
    }

    for (int x = 0; x < w; x++) {
        menu_put(&s->inserted_buffer[x], '-', fg_title, bg_border);
        menu_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)x], '-', fg_hint, bg_footer);
    }
    for (int y = 0; y < h; y++) {
        menu_put(&s->inserted_buffer[(size_t)y * (size_t)w], '|', fg_title, bg_border);
        menu_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)(w - 1)], '|', fg_title, bg_border);
    }
    menu_put(&s->inserted_buffer[0], '+', fg_title, bg_border);
    menu_put(&s->inserted_buffer[(size_t)(w - 1)], '+', fg_title, bg_border);
    menu_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w], '+', fg_hint, bg_footer);
    menu_put(&s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)(w - 1)], '+', fg_hint, bg_footer);

    const char *title = s->title ? s->title : "Menu";
    int title_len = (int)strlen(title);
    int title_x = (w - title_len) / 2;
    if (title_x < 1) title_x = 1;
    if (title_x + title_len > w - 1) {
        title_len = (w - 1) - title_x;
    }
    for (int i = 0; i < title_len; i++) {
        TuixPixel *p = &s->inserted_buffer[(size_t)title_x + (size_t)i];
        p->sym[0] = title[i];
        p->sym[1] = '\0';
        p->styles.fg = fg_title;
        p->styles.bg = bg_border;
        p->styles.custom_fg = 1;
        p->styles.custom_bg = 1;
    }

    int body_top = (h >= 3) ? 2 : 1;
    int body_bottom = (h >= 2) ? h - 1 : h;
    int visible_rows = body_bottom - body_top;
    if (visible_rows < 0) visible_rows = 0;

    for (int row = 0; row < visible_rows; row++) {
        int y = body_top + row;
        if (y >= body_bottom) break;
        int item_index = row;
        if (item_index >= s->item_count) break;

        int is_selected = (item_index == s->selected);
        TuixRGBTuple fg = is_selected ? fg_selected : fg_panel;
        TuixRGBTuple bg = is_selected ? bg_selected : bg_panel;

        for (int x = 1; x < w - 1; x++) {
            menu_put(&s->inserted_buffer[(size_t)y * (size_t)w + (size_t)x], ' ', fg, bg);
        }

        const char *prefix = is_selected ? "> " : "  ";
        for (int i = 0; i < 2 && (1 + i) < w - 1; i++) {
            TuixPixel *p = &s->inserted_buffer[(size_t)y * (size_t)w + (size_t)(1 + i)];
            p->sym[0] = prefix[i];
            p->sym[1] = '\0';
            p->styles.fg = fg;
            p->styles.bg = bg;
            p->styles.custom_fg = 1;
            p->styles.custom_bg = 1;
        }

        const char *label = s->items[item_index] ? s->items[item_index] : "";
        int max_text = w - 4;
        for (int x = 0; label[x] && x < max_text; x++) {
            TuixPixel *p = &s->inserted_buffer[(size_t)y * (size_t)w + (size_t)(3 + x)];
            p->sym[0] = label[x];
            p->sym[1] = '\0';
            p->styles.fg = fg;
            p->styles.bg = bg;
            p->styles.custom_fg = 1;
            p->styles.custom_bg = 1;
        }
    }

    if (h >= 2) {
        const char *hint = "Up/Down + Enter";
        int hint_len = (int)strlen(hint);
        int hint_x = (w - hint_len) / 2;
        if (hint_x < 1) hint_x = 1;
        for (int i = 0; i < hint_len && (hint_x + i) < w - 1; i++) {
            TuixPixel *p = &s->inserted_buffer[(size_t)(h - 1) * (size_t)w + (size_t)(hint_x + i)];
            p->sym[0] = hint[i];
            p->sym[1] = '\0';
            p->styles.fg = fg_hint;
            p->styles.bg = bg_footer;
            p->styles.custom_fg = 1;
            p->styles.custom_bg = 1;
        }
    }

    return s->inserted_buffer;
}

static TuixHandlerResponse menu_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    TuixHandlerResponse ans = {.requires_redraw = 0};
    if (!obj || !obj->state) {
        return ans;
    }

    TuixMenuState *s = (TuixMenuState*)obj->state;

    if (is_focused && has_event && snap && !snap->consumed_keyboard && snap->keyboard && snap->keyboard->has_event && s->item_count > 0) {
        int code = snap->keyboard->code;
        if (code == TUIX_VK_UP || code == 'k') {
            if (s->selected > 0) {
                s->selected--;
                menu_mark_redraw(s);
            }
            snap->consumed_keyboard = true;
        } else if (code == TUIX_VK_DOWN || code == 'j') {
            if (s->selected < s->item_count - 1) {
                s->selected++;
                menu_mark_redraw(s);
            }
            snap->consumed_keyboard = true;
        } else if (code == TUIX_VK_HOME) {
            s->selected = 0;
            menu_mark_redraw(s);
            snap->consumed_keyboard = true;
        } else if (code == TUIX_VK_END) {
            s->selected = s->item_count - 1;
            menu_mark_redraw(s);
            snap->consumed_keyboard = true;
        } else if (code == TUIX_VK_ENTER || code == '\n' || code == ' ') {
            s->activated = s->selected;
            menu_mark_redraw(s);
            snap->consumed_keyboard = true;
        }
    }

    if (has_event && snap && !snap->consumed_mouse && snap->mouse && snap->mouse->has_event && s->item_count > 0) {
        TuixBuffer b;
        if (tuix_get_buffer_snapshot_by_uid(obj->uid, &b) == 0) {
            int mx = snap->mouse->col - 1;
            int my = snap->mouse->row - 1;
            int inside = (mx >= b.margin_left && mx < b.margin_left + b.width &&
                          my >= b.margin_top && my < b.margin_top + b.height);
            if (inside) {
                int item_row = my - b.margin_top - 2;
                if (item_row >= 0 && item_row < s->item_count) {
                    if (s->selected != item_row) {
                        s->selected = item_row;
                        menu_mark_redraw(s);
                    }
                    if (snap->mouse->event == TUIX_MOUSE_PRESS || snap->mouse->event == TUIX_MOUSE_RELEASE) {
                        s->activated = item_row;
                        menu_mark_redraw(s);
                        snap->consumed_mouse = true;
                    }
                }
            }
        }
    }

    if (s->needs_redraw) {
        s->needs_redraw = 0;
        ans.requires_redraw = 1;
    }
    return ans;
}

static void menu_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixMenuState *s = (TuixMenuState*)obj->state;
    if (menu_ensure_buffer(s, buffer) != 0) {
        printf("OOM\n");
        exit(1);
    }
}

const TuixBuilder tuix_menu_builder = {
    .name = "MenuBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = menu_create_state,
    .destroy_state = menu_destroy_state,
    .on_event = menu_handler,
    .on_resize = menu_on_resize,
    .build_content = menu_build_content
};

const TuixBuilder* tuix_menu_init(void) {
    return &tuix_menu_builder;
}

int tuix_menu_set_title(TuixObject *obj, const char *title) {
    if (!obj || !obj->state) return -1;
    TuixMenuState *s = (TuixMenuState*)obj->state;
    char *copy = strdup(title ? title : "");
    if (!copy) return -1;
    free(s->title);
    s->title = copy;
    menu_mark_redraw(s);
    return 0;
}

int tuix_menu_set_items(TuixObject *obj, const char **labels, int count) {
    if (!obj || !obj->state || count < 0) return -1;
    TuixMenuState *s = (TuixMenuState*)obj->state;

    for (int i = 0; i < s->item_count; i++) {
        free(s->items[i]);
    }
    free(s->items);
    s->items = NULL;
    s->item_count = 0;
    s->selected = 0;
    s->activated = -1;

    if (count == 0) {
        menu_mark_redraw(s);
        return 0;
    }

    if (!labels) return -1;

    s->items = malloc(sizeof(char*) * (size_t)count);
    if (!s->items) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        s->items[i] = strdup(labels[i] ? labels[i] : "");
        if (!s->items[i]) {
            for (int j = 0; j < i; j++) {
                free(s->items[j]);
            }
            free(s->items);
            s->items = NULL;
            return -1;
        }
    }

    s->item_count = count;
    if (s->selected >= s->item_count) {
        s->selected = 0;
    }
    menu_mark_redraw(s);
    return 0;
}

int tuix_menu_get_selected(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    return ((TuixMenuState*)obj->state)->selected;
}

int tuix_menu_take_activated(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixMenuState *s = (TuixMenuState*)obj->state;
    int v = s->activated;
    s->activated = -1;
    return v;
}

void tuix_menu_reset(TuixObject *obj) {
    if (!obj || !obj->state) return;
    TuixMenuState *s = (TuixMenuState*)obj->state;
    s->selected = 0;
    s->activated = -1;
    menu_mark_redraw(s);
}
