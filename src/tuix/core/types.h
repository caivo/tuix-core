#ifndef TUIX_types_H
#define TUIX_types_H

#include <stddef.h>

typedef struct TuixRGBTuple {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} TuixRGBTuple;

typedef struct TuixPixelStyles {
    TuixRGBTuple fg;
    TuixRGBTuple bg;
    unsigned char custom_bg;
    unsigned char custom_fg;
    unsigned char bold;
    unsigned char underlined;
    unsigned char italic;
    unsigned char dim;
} TuixPixelStyles;

typedef struct TuixPixel {
    char sym[8];
    TuixPixelStyles styles;
    /* Quantized fg/bg cached per-frame so the renderer doesn't re-quantize unchanged pixels. */
    TuixRGBTuple q_fg;
    TuixRGBTuple q_bg;
    unsigned char q_cached;
} TuixPixel;

struct TuixBuffer;
typedef struct TuixBuilder TuixBuilder;

typedef struct TuixHandlerResponse {
    int requires_redraw;
} TuixHandlerResponse;

typedef struct TuixObject {
    int uid;
    const TuixBuilder* builder;
    void* state;
    float width_mod;
    float height_mod;
    float margin_top_mod;
    float margin_left_mod;
} TuixObject;

typedef struct TuixBuffer {
    TuixObject* obj;
    TuixPixel* pixels;
    int width;
    int height;
    int required_redraw;
    int margin_left;
    int margin_top;
} TuixBuffer;

typedef struct TuixFinalBuffer {
    TuixPixel* pixels;
    int width;
    int height;
    int full_redraw;
} TuixFinalBuffer;

typedef void (*TuixRowDoneCallback)(TuixFinalBuffer *buffer, int y, void *user_data);

typedef struct TuixScene {
    TuixBuffer** buffers;
    int count;
    int active;
    int capacity;
} TuixScene;

typedef struct TuixScenes {
    TuixScene** scenes;
    int count;
    int capacity;
    char** names;
} TuixScenes;

typedef struct TuixSubcycle {
    TuixObject* obj;
    TuixHandlerResponse (*handler)(struct TuixObject* obj);
    int enabled;
} TuixSubcycle;

typedef struct TuixSceneSubcycles {
    char* scene_name;
    TuixSubcycle** subcycles;
    int count;
    int capacity;
} TuixSceneSubcycles;

typedef struct TuixSubcycles {
    TuixSceneSubcycles** subcycles;
    int count;
    int capacity;
} TuixSubcycles;

typedef TuixPixel* (*build_content_fn)(struct TuixObject* obj, TuixBuffer* buffer);
typedef void* (*create_state_fn)(void* params);
typedef void (*destroy_state_fn)(void* state);
typedef void (*resize_fn)(struct TuixObject* obj, TuixBuffer* buffer, int width, int height);

struct TuixBuilder {
    const char* name;
    const char* version;
    const char* author;
    const char* namespace;
    create_state_fn create_state;
    destroy_state_fn destroy_state;
    TuixHandlerResponse (*handler_func)(struct TuixObject* obj);
    /* Optional callback invoked when buffer geometry changes due to terminal resize.
       Called after `tuix_resolve_geometry` and before `build_content`. */
    resize_fn on_resize;
    build_content_fn build_content;
};

typedef struct TuixBuilders {
    TuixBuilder** builders;
    int count;
    int capacity;
} TuixBuilders;

typedef struct TuixRegistry {
    TuixScenes scenes;
    TuixSubcycles subcycles;
    TuixBuilders builders;
    char* current_scene_name;
    int next_uid;
    int terminal_width;
    int terminal_height;
    int terminal_height_old;
    int terminal_width_old;
} TuixRegistry;

// Input - mouse event types
#define TUIX_MOUSE_NONE         0
#define TUIX_MOUSE_PRESS        1
#define TUIX_MOUSE_RELEASE      2
#define TUIX_MOUSE_HOVER        3   /* move, no button held         */
#define TUIX_MOUSE_DRAG         4   /* move with button(s) held     */
#define TUIX_MOUSE_SCROLL_UP    5
#define TUIX_MOUSE_SCROLL_DOWN  6
#define TUIX_MOUSE_DOUBLE_CLICK 7
#define TUIX_MOUSE_HSCROLL_LEFT  8  /* horizontal scroll            */
#define TUIX_MOUSE_HSCROLL_RIGHT 9

// Mouse button identifiers
#define TUIX_BTN_LEFT   0
#define TUIX_BTN_MIDDLE 1
#define TUIX_BTN_RIGHT  2
#define TUIX_BTN_X1     3
#define TUIX_BTN_X2     4

typedef struct {
    int event;         /* TUIX_MOUSE_* constant                     */
    int btn;           /* TUIX_BTN_* - which button (for press/release/drag/dblclick) */
    int buttons_held;  /* bitmask: bit 0=left, 1=middle, 2=right, 3=x1, 4=x2 */
    int col;           /* 1-based column                            */
    int row;           /* 1-based row                               */
    double timestamp;
    int has_event;
} TuixMouseKey;

typedef struct {
    int btn;
    int code;
    int scancode;
    int modifiers;
    int pressed;
    int repeat;
    double timestamp;
    int has_event;
    char text[8];
} TuixKeyboardKey;

typedef struct {
    int term_x;
    int term_y;
    TuixMouseKey *mouse;
    TuixKeyboardKey *keyboard;
} TuixInputSnapshot;

#endif