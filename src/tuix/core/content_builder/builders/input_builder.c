#include "input_builder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../../types.h"
#include "../../buffer_manager.h"

#define TUIX_VK_LEFT   0x25
#define TUIX_VK_RIGHT  0x27
#define TUIX_VK_HOME   0x24
#define TUIX_VK_END    0x23
#define TUIX_VK_BACK   0x08
#define TUIX_VK_DELETE 0x2E
#define TUIX_VK_ENTER  0x0D
#define TUIX_VK_ESCAPE 0x1B

typedef struct {
    char  *text;
    int    text_len;
    int    text_cap;
    int    cursor;
    int    scroll_offset;
    int    submitted;
    char  *placeholder;
    int    needs_redraw;
    TuixPixel *inserted_buffer;
    int    inserted_buffer_size;
} TuixInputState;

static int input_ensure_cap(TuixInputState *s, int extra);



static void* input_create_state(void* params) {
    (void)params;
    TuixInputState *s = calloc(1, sizeof(TuixInputState));
    if (!s) return NULL;
    s->text_cap = 64;
    s->text = calloc((size_t)s->text_cap, 1);
    if (!s->text) { free(s); return NULL; }
    s->needs_redraw = 1;
    return s;
}

static void input_destroy_state(void* state) {
    if (!state) return;
    TuixInputState *s = (TuixInputState*)state;
    free(s->text);
    free(s->placeholder);
    /* Do NOT free inserted_buffer here - tuix_free_buffer() already frees buffer->pixels,
       which points to the same allocation. */
    free(s);
}



static TuixPixel* input_build_content(TuixObject *obj, TuixBuffer *buffer) {
    TuixInputState *s = obj ? (TuixInputState*)obj->state : NULL;
    int w = buffer->width, h = buffer->height;
    size_t n = (size_t)w * h;

    if (!s) {
        TuixPixel *px = calloc(n, sizeof(TuixPixel));
        if (!px) { printf("OOM\n"); exit(1); }
        for (size_t i = 0; i < n; i++) {
            px[i].sym[0] = ' '; px[i].sym[1] = '\0';
            px[i].styles.fg = (TuixRGBTuple){255,255,255};
            px[i].styles.bg = (TuixRGBTuple){0,0,0};
            px[i].styles.custom_bg = 1;
            px[i].styles.custom_fg = 1;
        }
        return px;
    }

    /* Ensure inserted_buffer matches current buffer size; reallocate on resize. */
    size_t required_bytes = sizeof(TuixPixel) * n;
    if (!s->inserted_buffer || (size_t)s->inserted_buffer_size != required_bytes) {
        if (s->inserted_buffer) {
            free(s->inserted_buffer);
            s->inserted_buffer = NULL;
            s->inserted_buffer_size = 0;
        }
        s->inserted_buffer = calloc(n, sizeof(TuixPixel));
        if (!s->inserted_buffer) { printf("OOM\n"); exit(1); }
        s->inserted_buffer_size = (int)required_bytes;
        s->needs_redraw = 1;
        if (buffer) buffer->required_redraw = 1;
    }

    TuixRGBTuple fg_text        = {230, 230, 230};
    TuixRGBTuple fg_placeholder = {100, 100, 100};
    TuixRGBTuple fg_cursor      = {0, 0, 0};
    TuixRGBTuple bg_field       = {40, 40, 50};
    TuixRGBTuple bg_cursor      = {230, 230, 230};
    TuixRGBTuple fg_border      = {100, 140, 200};
    TuixRGBTuple bg_border      = {25, 25, 35};

    for (size_t i = 0; i < n; i++) {
        TuixPixel *p = &s->inserted_buffer[i];
        p->sym[0] = ' '; p->sym[1] = '\0';
        p->styles.fg = fg_border;
        p->styles.bg = bg_border;
        p->styles.custom_bg = 1;
        p->styles.custom_fg = 1;
    }

    /* Draw a single-line input field on the vertically centred row */
    int row = h > 2 ? 1 : 0;
    int field_x0 = 1;
    int field_w  = w - 2;
    if (field_w < 1) { field_w = w; field_x0 = 0; }

    if (w >= 2 && row < h) {
        TuixPixel *lb = &s->inserted_buffer[(size_t)row * w];
        lb->sym[0] = '['; lb->sym[1] = '\0';
        lb->styles.fg = fg_border; lb->styles.bg = bg_border;

        TuixPixel *rb = &s->inserted_buffer[(size_t)row * w + (w - 1)];
        rb->sym[0] = ']'; rb->sym[1] = '\0';
        rb->styles.fg = fg_border; rb->styles.bg = bg_border;
    }

    /* Adjust horizontal scroll so cursor is always visible */
    if (s->cursor - s->scroll_offset >= field_w)
        s->scroll_offset = s->cursor - field_w + 1;
    if (s->cursor < s->scroll_offset)
        s->scroll_offset = s->cursor;
    if (s->scroll_offset < 0)
        s->scroll_offset = 0;

    int show_placeholder = (s->text_len == 0 && s->placeholder && !s->submitted);
    const char *display;
    TuixRGBTuple fg_active;
    if (show_placeholder) {
        display   = s->placeholder;
        fg_active = fg_placeholder;
    } else {
        display   = s->text + s->scroll_offset;
        fg_active = fg_text;
    }

    int dlen = (int)strlen(display);
    for (int x = 0; x < field_w && row < h; x++) {
        int px = field_x0 + x;
        if (px >= w) break;
        TuixPixel *p = &s->inserted_buffer[(size_t)row * w + px];
        p->styles.bg = bg_field;
        p->styles.fg = fg_active;
        p->sym[0] = (x < dlen) ? display[x] : ' ';
        p->sym[1] = '\0';
    }

    /* Draw cursor (not during placeholder or after submit) */
    if (!show_placeholder && !s->submitted) {
        int cx = field_x0 + (s->cursor - s->scroll_offset);
        if (cx >= field_x0 && cx < field_x0 + field_w && cx < w && row < h) {
            TuixPixel *cp = &s->inserted_buffer[(size_t)row * w + cx];
            cp->styles.bg = bg_cursor;
            cp->styles.fg = fg_cursor;
            cp->sym[0] = (s->cursor < s->text_len) ? s->text[s->cursor] : ' ';
            cp->sym[1] = '\0';
        }
    }

    return s->inserted_buffer;
}


static TuixHandlerResponse input_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    if (!obj || !obj->state)
        return (TuixHandlerResponse){.requires_redraw = 0};

    TuixInputState *s = (TuixInputState*)obj->state;

    if (is_focused && has_event && snap && !snap->consumed_keyboard && snap->keyboard && snap->keyboard->has_event && !s->submitted) {
        int code = snap->keyboard->code;

        if (code == TUIX_VK_LEFT) {
            if (s->cursor > 0) { s->cursor--; s->needs_redraw = 1; }
        } else if (code == TUIX_VK_RIGHT) {
            if (s->cursor < s->text_len) { s->cursor++; s->needs_redraw = 1; }
        } else if (code == TUIX_VK_HOME) {
            s->cursor = 0; s->needs_redraw = 1;
        } else if (code == TUIX_VK_END) {
            s->cursor = s->text_len; s->needs_redraw = 1;
        } else if (code == TUIX_VK_BACK || code == 127) {
            if (s->cursor > 0) {
                memmove(s->text + s->cursor - 1, s->text + s->cursor,
                        (size_t)(s->text_len - s->cursor));
                s->text_len--;
                s->cursor--;
                s->text[s->text_len] = '\0';
                s->needs_redraw = 1;
            }
        } else if (code == TUIX_VK_DELETE) {
            if (s->cursor < s->text_len) {
                memmove(s->text + s->cursor, s->text + s->cursor + 1,
                        (size_t)(s->text_len - s->cursor - 1));
                s->text_len--;
                s->text[s->text_len] = '\0';
                s->needs_redraw = 1;
            }
        } else if (code == TUIX_VK_ENTER || code == '\n') {
            s->submitted = 1;
            s->needs_redraw = 1;
        } else if (code >= 32 && code < 127) {
            if (input_ensure_cap(s, 1) == 0) {
                memmove(s->text + s->cursor + 1, s->text + s->cursor,
                        (size_t)(s->text_len - s->cursor));
                s->text[s->cursor] = (char)code;
                s->text_len++;
                s->cursor++;
                s->text[s->text_len] = '\0';
                s->needs_redraw = 1;
            }
        }

        if (code == TUIX_VK_LEFT || code == TUIX_VK_RIGHT || code == TUIX_VK_HOME || code == TUIX_VK_END ||
            code == TUIX_VK_BACK || code == 127 || code == TUIX_VK_DELETE || code == TUIX_VK_ENTER ||
            code == '\n' || (code >= 32 && code < 127)) {
            snap->consumed_keyboard = true;
        }
    }

    if (s->needs_redraw) {
        s->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}


/* forward declaration so initializer can take address */
static void input_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height);

const TuixBuilder tuix_input_builder = {
    .name       = "InputBuilder",
    .version    = "1.0",
    .author     = "custosh",
    .namespace  = "tuix",
    .create_state  = input_create_state,
    .destroy_state = input_destroy_state,
    .on_event  = input_handler,
    .on_resize     = input_on_resize,
    .build_content = input_build_content
};

const TuixBuilder* tuix_input_init(void) {
    return &tuix_input_builder;
}


static int input_ensure_cap(TuixInputState *s, int extra) {
    if (s->text_len + extra < s->text_cap) return 0;
    int newcap = s->text_cap < 64 ? 64 : s->text_cap;
    while (newcap <= s->text_len + extra) newcap *= 2;
    char *tmp = realloc(s->text, (size_t)newcap);
    if (!tmp) return -1;
    s->text = tmp;
    s->text_cap = newcap;
    return 0;
}

int tuix_input_set_placeholder(TuixObject *obj, const char *text) {
    if (!obj || !obj->state) return -1;
    TuixInputState *s = (TuixInputState*)obj->state;
    free(s->placeholder);
    s->placeholder = text ? strdup(text) : NULL;
    s->needs_redraw = 1;
    return 0;
}

const char* tuix_input_get_text(TuixObject *obj) {
    if (!obj || !obj->state) return "";
    return ((TuixInputState*)obj->state)->text;
}

int tuix_input_is_submitted(TuixObject *obj) {
    if (!obj || !obj->state) return 0;
    return ((TuixInputState*)obj->state)->submitted;
}

const char* tuix_input_get_result(TuixObject *obj) {
    if (!obj || !obj->state) return NULL;
    TuixInputState *s = (TuixInputState*)obj->state;
    return s->submitted ? s->text : NULL;
}

void tuix_input_reset(TuixObject *obj) {
    if (!obj || !obj->state) return;
    TuixInputState *s = (TuixInputState*)obj->state;
    s->text_len = 0;
    s->cursor = 0;
    s->scroll_offset = 0;
    s->submitted = 0;
    s->text[0] = '\0';
    s->needs_redraw = 1;
}

static void input_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    if (!obj || !obj->state) return;
    TuixInputState *s = (TuixInputState*)obj->state;
    size_t n = (size_t)width * (size_t)height;
    size_t required_bytes = sizeof(TuixPixel) * n;
    if (s->inserted_buffer && (size_t)s->inserted_buffer_size == required_bytes) return;
    if (s->inserted_buffer) { free(s->inserted_buffer); s->inserted_buffer = NULL; s->inserted_buffer_size = 0; }
    s->inserted_buffer = calloc(n, sizeof(TuixPixel));
    if (!s->inserted_buffer) { printf("OOM\n"); exit(1); }
    s->inserted_buffer_size = (int)required_bytes;
    s->needs_redraw = 1;
    if (buffer) buffer->required_redraw = 1;
}