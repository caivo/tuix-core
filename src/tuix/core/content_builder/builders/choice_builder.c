#include "choice_builder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../types.h"
#include "../../buffer_manager.h"


#define TUIX_VK_UP    0x26
#define TUIX_VK_DOWN  0x28
#define TUIX_VK_ENTER 0x0D

typedef struct {
    char  **options;
    int     option_count;
    int     selected;
    int     confirmed;
    int     needs_redraw;
    TuixPixel *inserted_buffer;
    int        inserted_buffer_size;
} TuixChoiceState;



static void* choice_create_state(void* params) {
    (void)params;
    TuixChoiceState *s = calloc(1, sizeof(TuixChoiceState));
    if (!s) return NULL;
    s->needs_redraw = 1;
    return s;
}

static void choice_destroy_state(void* state) {
    if (!state) return;
    TuixChoiceState *s = (TuixChoiceState*)state;
    for (int i = 0; i < s->option_count; i++)
        free(s->options[i]);
    free(s->options);
    /* Do NOT free inserted_buffer here - tuix_free_buffer() already frees buffer->pixels,
       which points to the same allocation. */
    free(s);
}



static TuixPixel* choice_build_content(TuixObject *obj, TuixBuffer* buffer) {
    TuixChoiceState *s = obj ? (TuixChoiceState*)obj->state : NULL;
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

    TuixRGBTuple bg_normal    = {30, 30, 30};
    TuixRGBTuple fg_normal    = {200, 200, 200};
    TuixRGBTuple bg_highlight = {60, 120, 220};
    TuixRGBTuple fg_highlight = {255, 255, 255};
    TuixRGBTuple bg_confirmed = {40, 180, 80};
    TuixRGBTuple fg_confirmed = {255, 255, 255};

    for (size_t i = 0; i < n; i++) {
        TuixPixel *p = &s->inserted_buffer[i];
        p->sym[0] = ' '; p->sym[1] = '\0';
        p->styles.fg = fg_normal;
        p->styles.bg = bg_normal;
        p->styles.custom_bg = 1;
        p->styles.custom_fg = 1;
    }

    for (int i = 0; i < s->option_count && i < h; i++) {
        int is_sel = (i == s->selected);
        TuixRGBTuple fg = fg_normal, bg = bg_normal;
        if (is_sel && s->confirmed) { fg = fg_confirmed; bg = bg_confirmed; }
        else if (is_sel)            { fg = fg_highlight; bg = bg_highlight; }

        /* Paint row background + colours */
        for (int x = 0; x < w; x++) {
            TuixPixel *p = &s->inserted_buffer[(size_t)i * w + x];
            p->styles.fg = fg;
            p->styles.bg = bg;
        }

        const char *pfx = is_sel ? " > " : "   ";
        for (int x = 0; x < 3 && x < w; x++) {
            s->inserted_buffer[(size_t)i * w + x].sym[0] = pfx[x];
            s->inserted_buffer[(size_t)i * w + x].sym[1] = '\0';
        }

        const char *label = s->options[i];
        for (int x = 0; label[x] && (x + 3) < w; x++) {
            TuixPixel *p = &s->inserted_buffer[(size_t)i * w + (x + 3)];
            p->sym[0] = label[x];
            p->sym[1] = '\0';
        }
    }

    return s->inserted_buffer;
}


static TuixHandlerResponse choice_handler(TuixObject *obj, bool has_event, bool is_focused, TuixInputSnapshot* snap) {
    if (!obj || !obj->state)
        return (TuixHandlerResponse){.requires_redraw = 0};

    TuixChoiceState *s = (TuixChoiceState*)obj->state;
    if (is_focused && has_event && snap && !snap->consumed_keyboard && snap->keyboard && snap->keyboard->has_event && s->option_count > 0) {
        int code = snap->keyboard->code;
        if (code == TUIX_VK_UP || code == 'k') {
            if (s->selected > 0) { s->selected--; s->needs_redraw = 1; }
        } else if (code == TUIX_VK_DOWN || code == 'j') {
            if (s->selected < s->option_count - 1) { s->selected++; s->needs_redraw = 1; }
        } else if (code == TUIX_VK_ENTER || code == '\n') {
            s->confirmed = 1;
            s->needs_redraw = 1;
        }
        if (code == TUIX_VK_UP || code == 'k' || code == TUIX_VK_DOWN || code == 'j' || code == TUIX_VK_ENTER || code == '\n') {
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
static void choice_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height);

const TuixBuilder tuix_choice_builder = {
    .name       = "ChoiceBuilder",
    .version    = "1.0",
    .author     = "custosh",
    .namespace  = "tuix",
    .create_state  = choice_create_state,
    .destroy_state = choice_destroy_state,
    .on_event  = choice_handler,
    .on_resize     = choice_on_resize,
    .build_content = choice_build_content
};

const TuixBuilder* tuix_choice_init(void) {
    return &tuix_choice_builder;
}


int tuix_choice_set_options(TuixObject *obj, const char **labels, int count) {
    if (!obj || !obj->state || !labels || count <= 0) return -1;
    TuixChoiceState *s = (TuixChoiceState*)obj->state;

    for (int i = 0; i < s->option_count; i++)
        free(s->options[i]);
    free(s->options);

    s->options = malloc(sizeof(char*) * (size_t)count);
    if (!s->options) { s->option_count = 0; return -1; }
    for (int i = 0; i < count; i++) {
        s->options[i] = strdup(labels[i]);
        if (!s->options[i]) {
            for (int j = 0; j < i; j++) free(s->options[j]);
            free(s->options);
            s->options = NULL;
            s->option_count = 0;
            return -1;
        }
    }
    s->option_count = count;
    s->selected     = 0;
    s->confirmed    = 0;
    s->needs_redraw = 1;
    return 0;
}

int tuix_choice_get_selected(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    return ((TuixChoiceState*)obj->state)->selected;
}

int tuix_choice_is_confirmed(TuixObject *obj) {
    if (!obj || !obj->state) return 0;
    return ((TuixChoiceState*)obj->state)->confirmed;
}

int tuix_choice_get_result(TuixObject *obj) {
    if (!obj || !obj->state) return -1;
    TuixChoiceState *s = (TuixChoiceState*)obj->state;
    return s->confirmed ? s->selected : -1;
}

void tuix_choice_reset(TuixObject *obj) {
    if (!obj || !obj->state) return;
    TuixChoiceState *s = (TuixChoiceState*)obj->state;
    s->confirmed = 0;
    s->selected  = 0;
    s->needs_redraw = 1;
}

static void choice_on_resize(TuixObject *obj, TuixBuffer *buffer, int width, int height) {
    if (!obj || !obj->state) return;
    TuixChoiceState *s = (TuixChoiceState*)obj->state;
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