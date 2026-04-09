#include "status_builder.h"
#include "../../buffer_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *text;
    int level;
    TuixRGBTuple ok;
    TuixRGBTuple warn;
    TuixRGBTuple err;
    TuixRGBTuple idle;
    int needs_redraw;
    TuixPixel *inserted_buffer;
    int inserted_buffer_size;
} TuixStatusState;

static void* status_create_state(void* params) {
    (void)params;
    TuixStatusState *s = calloc(1, sizeof(TuixStatusState));
    if (!s) return NULL;
    s->text = strdup("idle");
    s->level = TUIX_STATUS_IDLE;
    s->ok = (TuixRGBTuple){74, 196, 126};
    s->warn = (TuixRGBTuple){255, 193, 7};
    s->err = (TuixRGBTuple){230, 86, 72};
    s->idle = (TuixRGBTuple){160, 167, 176};
    s->needs_redraw = 1;
    return s;
}

static void status_destroy_state(void* state) {
    if (!state) return;
    TuixStatusState *s = (TuixStatusState*)state;
    free(s->text);
    free(s);
}

static int status_ensure_buffer(TuixStatusState *s, TuixBuffer *buffer) {
    size_t n = (size_t)buffer->width * (size_t)buffer->height;
    size_t required = sizeof(TuixPixel) * n;
    if (s->inserted_buffer && (size_t)s->inserted_buffer_size == required) return 0;

    if (s->inserted_buffer) {
        free(s->inserted_buffer);
        s->inserted_buffer = NULL;
        s->inserted_buffer_size = 0;
    }

    s->inserted_buffer = calloc(n, sizeof(TuixPixel));
    if (!s->inserted_buffer) return -1;
    s->inserted_buffer_size = (int)required;
    s->needs_redraw = 1;
    buffer->required_redraw = 1;
    return 0;
}

static TuixRGBTuple status_color(const TuixStatusState *s) {
    if (s->level == TUIX_STATUS_OK) return s->ok;
    if (s->level == TUIX_STATUS_WARN) return s->warn;
    if (s->level == TUIX_STATUS_ERROR) return s->err;
    return s->idle;
}

static const char* status_prefix(const TuixStatusState *s) {
    if (s->level == TUIX_STATUS_OK) return "OK";
    if (s->level == TUIX_STATUS_WARN) return "WARN";
    if (s->level == TUIX_STATUS_ERROR) return "ERROR";
    return "IDLE";
}

static TuixPixel* status_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixStatusState *s = obj ? (TuixStatusState*)obj->state : NULL;
    if (!s || !buffer || buffer->width <= 0 || buffer->height <= 0) return NULL;
    if (status_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }

    int w = buffer->width;
    int h = buffer->height;
    size_t n = (size_t)w * (size_t)h;
    TuixRGBTuple c = status_color(s);

    for (size_t i = 0; i < n; i++) {
        TuixPixel *p = &s->inserted_buffer[i];
        memset(p, 0, sizeof(*p));
        p->sym[0] = ' ';
        p->sym[1] = '\0';
        p->styles.fg = c;
        p->styles.custom_fg = 1;
        p->styles.custom_bg = 0;
    }

    int row = h / 2;
    if (row < 0 || row >= h) return s->inserted_buffer;

    char line[256];
    snprintf(line, sizeof(line), "[%s] %s", status_prefix(s), s->text ? s->text : "");
    int len = (int)strlen(line);
    int start = (w - len) / 2;
    if (start < 0) start = 0;

    for (int i = 0; i < len && (start + i) < w; i++) {
        TuixPixel *p = &s->inserted_buffer[(size_t)row * (size_t)w + (size_t)(start + i)];
        p->sym[0] = line[i];
        p->sym[1] = '\0';
    }

    return s->inserted_buffer;
}

static TuixHandlerResponse status_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    (void)has_event;
    (void)is_focused;
    (void)snap;
    if (!obj || !obj->state) return (TuixHandlerResponse){.requires_redraw = 0};
    TuixStatusState *s = (TuixStatusState*)obj->state;
    if (s->needs_redraw) {
        s->needs_redraw = 0;
        return (TuixHandlerResponse){.requires_redraw = 1};
    }
    return (TuixHandlerResponse){.requires_redraw = 0};
}

static void status_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    (void)width;
    (void)height;
    if (!obj || !obj->state || !buffer) return;
    TuixStatusState *s = (TuixStatusState*)obj->state;
    if (status_ensure_buffer(s, buffer) != 0) { printf("OOM\n"); exit(1); }
}

const TuixBuilder tuix_status_builder = {
    .name = "StatusBuilder",
    .version = "1.0",
    .author = "custosh",
    .namespace = "tuix",
    .create_state = status_create_state,
    .destroy_state = status_destroy_state,
    .on_event = status_handler,
    .on_resize = status_on_resize,
    .build_content = status_build_content
};

const TuixBuilder* tuix_status_init(void) {
    return &tuix_status_builder;
}

int tuix_status_set_text(TuixObject *obj, const char *text) {
    if (!obj || !obj->state) return -1;
    TuixStatusState *s = (TuixStatusState*)obj->state;
    char *copy = strdup(text ? text : "");
    if (!copy) return -1;
    free(s->text);
    s->text = copy;
    s->needs_redraw = 1;
    return 0;
}

int tuix_status_set_level(TuixObject *obj, int level) {
    if (!obj || !obj->state) return -1;
    if (level < TUIX_STATUS_IDLE || level > TUIX_STATUS_ERROR) return -1;
    TuixStatusState *s = (TuixStatusState*)obj->state;
    s->level = level;
    s->needs_redraw = 1;
    return 0;
}

int tuix_status_set_palette(TuixObject *obj,
                            uint8_t ok_r, uint8_t ok_g, uint8_t ok_b,
                            uint8_t warn_r, uint8_t warn_g, uint8_t warn_b,
                            uint8_t err_r, uint8_t err_g, uint8_t err_b,
                            uint8_t idle_r, uint8_t idle_g, uint8_t idle_b) {
    if (!obj || !obj->state) return -1;
    TuixStatusState *s = (TuixStatusState*)obj->state;
    s->ok = (TuixRGBTuple){ok_r, ok_g, ok_b};
    s->warn = (TuixRGBTuple){warn_r, warn_g, warn_b};
    s->err = (TuixRGBTuple){err_r, err_g, err_b};
    s->idle = (TuixRGBTuple){idle_r, idle_g, idle_b};
    s->needs_redraw = 1;
    return 0;
}
