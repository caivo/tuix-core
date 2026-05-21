#ifndef TUIX_types_H
#define TUIX_types_H

#include <stddef.h>
#include <stdbool.h>

#ifdef TUIX_DEBUG_HEAP
#include <stdlib.h>
#include "debug_heap.h"
#define malloc(S) debug_malloc((S), __FILE__, __LINE__)
#define calloc(N,S) debug_calloc((N),(S), __FILE__, __LINE__)
#define realloc(P,S) debug_realloc((P),(S), __FILE__, __LINE__)
#define free(P) debug_free((P), __FILE__, __LINE__)
#endif

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
typedef struct TuixInputSnapshot TuixInputSnapshot;
typedef struct TuixMouseKey TuixMouseKey;
typedef struct TuixKeyboardKey TuixKeyboardKey;

typedef struct TuixHandlerResponse {
    int requires_redraw;
} TuixHandlerResponse;

typedef enum TuixLayoutAxis {
    TUIX_LAYOUT_AXIS_NONE = 0,
    TUIX_LAYOUT_AXIS_HORIZONTAL = 1,
    TUIX_LAYOUT_AXIS_VERTICAL = 2
} TuixLayoutAxis;

typedef enum TuixLayoutJustify {
    TUIX_LAYOUT_JUSTIFY_START = 0,
    TUIX_LAYOUT_JUSTIFY_CENTER = 1,
    TUIX_LAYOUT_JUSTIFY_END = 2,
    TUIX_LAYOUT_JUSTIFY_SPACE_BETWEEN = 3
} TuixLayoutJustify;

typedef enum TuixLayoutAlign {
    TUIX_LAYOUT_ALIGN_START = 0,
    TUIX_LAYOUT_ALIGN_CENTER = 1,
    TUIX_LAYOUT_ALIGN_END = 2,
    TUIX_LAYOUT_ALIGN_STRETCH = 3
} TuixLayoutAlign;

#define TUIX_LAYOUT_ALIGN_AUTO (-1)
#define TUIX_LAYOUT_BASIS_AUTO (-1)
#define TUIX_LAYOUT_UNBOUNDED (-1)

typedef struct TuixLayoutSlot {
    float grow;
    float shrink;
    int basis;
    int min_w;
    int min_h;
    int max_w;
    int max_h;
    int align_self;
    int grid_row;
    int grid_col;
    int row_span;
    int col_span;
} TuixLayoutSlot;

typedef struct TuixLayoutRect {
    int active;
    int offset_left;
    int offset_top;
    int width;
    int height;
} TuixLayoutRect;

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
    int pixels_owned; /* 1 if core owns and may free `pixels`, 0 if builder-owned */
    int width;
    int height;
    int required_redraw;
    int margin_left;
    int margin_top;
    int parent_uid;
    int z_index;
    int flat_index;
    int* children_uids;
    int children_count;
    int children_capacity;
    TuixLayoutSlot layout_slot;
    TuixLayoutRect layout_rect;
} TuixBuffer;

typedef struct TuixFinalBuffer {
    TuixPixel* pixels;
    int width;
    int height;
    int full_redraw;
} TuixFinalBuffer;

typedef void (*TuixRowDoneCallback)(TuixFinalBuffer *buffer, int y, void *user_data);

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

struct TuixMouseKey {
    int event;         /* TUIX_MOUSE_* constant                     */
    int btn;           /* TUIX_BTN_* - which button (for press/release/drag/dblclick) */
    int buttons_held;  /* bitmask: bit 0=left, 1=middle, 2=right, 3=x1, 4=x2 */
    int col;           /* 1-based column                            */
    int row;           /* 1-based row                               */
    double timestamp;
    int has_event;
};

struct TuixKeyboardKey {
    int btn;
    int code;
    int scancode;
    int modifiers;
    int pressed;
    int repeat;
    double timestamp;
    int has_event;
    char text[8];
};

struct TuixInputSnapshot {
    int term_x;
    int term_y;
    TuixMouseKey *mouse;
    TuixKeyboardKey *keyboard;
    bool consumed_keyboard;
    bool consumed_mouse;
};

typedef struct TuixScene {
    TuixBuffer** buffers;
    TuixBuffer** buffer_by_uid;
    int* root_uids;
    int count;
    int active;
    int capacity;
    int max_uid_capacity;
    int root_count;
    int root_capacity;
    int current_focus;
    int active_modal_uid;
    int modal_restore_focus_uid;
    int transaction_depth;
    int topology_dirty;
    unsigned long long last_active_frame;
    unsigned long long last_compacted_frame;
    unsigned long long topology_version;
    unsigned long long last_composited_topology_version;
} TuixScene;

typedef struct TuixSceneStats {
    int buffer_count;
    int active;
    int current_focus;
    unsigned long long last_active_frame;
    unsigned long long last_compacted_frame;
    size_t pixel_bytes;
    size_t approx_heap_bytes;
} TuixSceneStats;

typedef struct TuixScenes {
    TuixScene** scenes;
    int count;
    int capacity;
    char** names;
} TuixScenes;

typedef struct TuixSubcycle {
    TuixObject* obj;
    TuixHandlerResponse (*on_event)(struct TuixObject* obj, bool has_event, bool is_focused, TuixInputSnapshot* snap);
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
typedef void (*layout_children_fn)(struct TuixObject* obj, TuixBuffer* buffer);
typedef int (*viewport_offset_fn)(struct TuixObject* obj, int *offset_x, int *offset_y);
typedef int (*viewport_insets_fn)(struct TuixObject* obj, int *left, int *top, int *right, int *bottom);
typedef int (*viewport_content_size_fn)(struct TuixObject* obj, int *content_w, int *content_h);

struct TuixBuilder {
    const char* name;
    const char* version;
    const char* author;
    const char* namespace;
    create_state_fn create_state;
    destroy_state_fn destroy_state;
    TuixHandlerResponse (*on_event)(struct TuixObject* obj, bool has_event, bool is_focused, TuixInputSnapshot* snap);
    /* Optional callback invoked when buffer geometry changes due to terminal resize.
       Called after `tuix_resolve_geometry` and before `build_content`. */
    resize_fn on_resize;
    /* Optional callback for parent-controlled child layout. Invoked after geometry
       is resolved and resize handling is complete, before `build_content`. */
    layout_children_fn layout_children;
    viewport_offset_fn get_viewport_offset;
    viewport_insets_fn get_viewport_insets;
    viewport_content_size_fn get_viewport_content_size;
    build_content_fn build_content;
    /* When set, `build_content` returns a short-lived heap buffer that the core
       must free after copying. Default builders return persistent state-owned
       scratch buffers and leave cleanup to `destroy_state`. */
    unsigned char returns_temporary_pixels;
};

typedef struct TuixBuilders {
    TuixBuilder** builders;
    int count;
    int capacity;
} TuixBuilders;

typedef enum TuixDebugConfig {
    TUIX_DEBUG_NONE = 0,
    TUIX_DEBUG_STATS = 1,
    TUIX_DEBUG_VERBOSE = 2,
    TUIX_DEBUG_PROFILE = 3,
} TuixDebugConfig;

typedef struct TuixRegistry {
    TuixScenes scenes;
    TuixSubcycles subcycles;
    TuixBuilders builders;
    char* current_scene_name;
    unsigned long long frame_counter;
    int next_uid;
    int terminal_width;
    int terminal_height;
    int terminal_height_old;
    int terminal_width_old;
    TuixDebugConfig debug_config;
} TuixRegistry;

typedef struct TuixHitmapPixel {
    int obj_uid;
    int viewport_uid;
    int idx; /* index of the pixel in the hitmap */
} TuixHitmapPixel;

typedef struct TuixHitmap {
    TuixHitmapPixel* pixels;
    int width;
    int height;
    int count; /* number of valid entries in pixels */
    int capacity; /* allocated capacity of the pixels array */
} TuixHitmap;

#endif
