#include "scroll_container_builder.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../buffer_manager.h"
#include "../../main.h"
#include "../../object_manager.h"
#include "../../tuix_registry.h"

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

int tuix_scroll_container_is_viewport(TuixObject *obj) {
    return obj && obj->builder && obj->builder->name &&
           strcmp(obj->builder->name, "ScrollContainerBuilder") == 0;
}

int tuix_scroll_container_get_viewport_offset(TuixObject *obj, int *offset_x, int *offset_y) {
    if (!tuix_scroll_container_is_viewport(obj) || !obj->state) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    if (offset_x) *offset_x = s->offset_x;
    if (offset_y) *offset_y = s->offset_y;
    return 0;
}

int tuix_scroll_container_get_viewport_insets(TuixObject *obj, int *left, int *top, int *right, int *bottom) {
    if (!tuix_scroll_container_is_viewport(obj)) return -1;
    if (left) *left = 1;
    if (top) *top = 1;
    if (right) *right = 1;
    if (bottom) *bottom = 1;
    return 0;
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
    free(s->inserted_buffer);
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

    /* Body left intentionally blank — content is provided by child objects
       attached to the scroll container. Builders should not draw persistent
       filler content here to allow full customization. */

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
            tuix_mouse_capture_begin(obj->uid);
            snap->consumed_mouse = true;
        } else if (snap->mouse->event == TUIX_MOUSE_RELEASE && snap->mouse->btn == TUIX_BTN_LEFT) {
            if (s->drag_active) {
                s->drag_active = 0;
                tuix_mouse_capture_end(obj->uid);
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

static int scroll_viewport_content_size(TuixObject *obj, int *content_w, int *content_h) {
    if (!tuix_scroll_container_is_viewport(obj) || !obj->state) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    if (content_w) *content_w = s->content_w;
    if (content_h) *content_h = s->content_h;
    return 0;
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
    .get_viewport_offset = tuix_scroll_container_get_viewport_offset,
    .get_viewport_insets = tuix_scroll_container_get_viewport_insets,
    .get_viewport_content_size = scroll_viewport_content_size,
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
    if (s->content_w == content_w && s->content_h == content_h) {
        return 0;
    }
    s->content_w = content_w;
    s->content_h = content_h;
    tuix_mark_buffer_children_geometry_dirty_by_uid(obj->uid);
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
    int offset_x = -1;
    tuix_scroll_container_get_viewport_offset(obj, &offset_x, NULL);
    return offset_x;
}

int tuix_scroll_container_get_offset_y(TuixObject *obj) {
    int offset_y = -1;
    tuix_scroll_container_get_viewport_offset(obj, NULL, &offset_y);
    return offset_y;
}

int tuix_scroll_container_get_content_width(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    return s->content_w;
}

int tuix_scroll_container_get_content_height(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)obj->state;
    return s->content_h;
}

int tuix_scroll_container_add_object(const char* scene_name, TuixObject *container_obj, const char* builder_name, float width_mod, float height_mod, float margin_top_mod, float margin_left_mod) {
    if (!scene_name || !container_obj || !builder_name) return -1;
    int child_uid = tuix_create_object_ex((char*)builder_name, (char*)scene_name, width_mod, height_mod, margin_top_mod, margin_left_mod, 0);
    if (child_uid <= 0) return -1;
    if (tuix_set_buffer_parent((char*)scene_name, child_uid, container_obj->uid) != 0) {
        tuix_free_buffer((char*)scene_name, child_uid);
        return -1;
    }
    return child_uid;
}

int tuix_scroll_container_attach_child(const char* scene_name, TuixObject *container_obj, int child_uid) {
    if (!scene_name || !container_obj || child_uid <= 0) return -1;
    return tuix_set_buffer_parent((char*)scene_name, child_uid, container_obj->uid);
}

int tuix_scroll_container_detach_child(const char* scene_name, TuixObject *container_obj, int child_uid) {
    if (!scene_name || !container_obj || child_uid <= 0) return -1;
    return tuix_set_buffer_parent((char*)scene_name, child_uid, -1);
}

int tuix_scroll_container_add_object_at(const char* scene_name, TuixObject *container_obj, const char* builder_name, int content_x, int content_y, int content_w, int content_h) {
    if (!scene_name || !container_obj || !builder_name) return -1;
    TuixScrollContainerState *s = (TuixScrollContainerState*)container_obj->state;
    int base_content_w = (s && s->content_w > 0) ? s->content_w : 1;
    int base_content_h = (s && s->content_h > 0) ? s->content_h : 1;

    if (content_w < 1) content_w = 1;
    if (content_h < 1) content_h = 1;

    /* Child mods are expressed in scroll-content coordinates so the child can
       be created before the container has completed its first layout pass. */
    float margin_left_mod = (float)content_x / (float)base_content_w;
    float margin_top_mod = (float)content_y / (float)base_content_h;
    float width_mod = (float)content_w / (float)base_content_w;
    float height_mod = (float)content_h / (float)base_content_h;

    int child_uid = tuix_create_object_ex((char*)builder_name, (char*)scene_name, width_mod, height_mod, margin_top_mod, margin_left_mod, 0);
    if (child_uid <= 0) return -1;

    if (tuix_set_buffer_parent((char*)scene_name, child_uid, container_obj->uid) != 0) {
        tuix_free_buffer((char*)scene_name, child_uid);
        return -1;
    }

    return child_uid;
}
