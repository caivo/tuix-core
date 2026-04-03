# cython: boundscheck=False
# cython: wraparound=False
# cython: cdivision=True
# cython: initializedcheck=False
# cython: nonecheck=False
# cython: language_level=3

"""
Thin, maximally-optimized Cython bridge between the tuix C engine and Python.

Design goals:
  * Every C call goes through a typed `cdef`/`cpdef` with `nogil` where possible.
  * ANSI rendering uses C-level snprintf into a pre-allocated char buffer — zero
    Python string formatting on the hot path.
  * The Batch class stores actions in a flat C array from the start, not Python
    tuples, and dispatches them in a single nogil loop.
  * All pixel conversion from Python objects is done once into a contiguous
    C array, then processed entirely at C speed.
"""

from libc.stdint cimport uintptr_t, uint32_t, uint64_t, uint8_t, int64_t
from libc.stddef cimport size_t
from libc.stdlib cimport malloc, realloc, free
from libc.string cimport memcpy, memset, memmove, strlen, strcmp
from libc.math cimport sin
from libc.stdio cimport snprintf
from cpython.bytes cimport PyBytes_AS_STRING, PyBytes_FromStringAndSize
from cpython.bytearray cimport PyByteArray_AS_STRING

# ═══════════════════════════════════════════════════════════════════
#  C type declarations — mirrors types.h exactly
# ═══════════════════════════════════════════════════════════════════
cdef extern from "types.h":
    ctypedef struct TuixRGBTuple:
        unsigned char r
        unsigned char g
        unsigned char b

    ctypedef struct TuixPixelStyles:
        TuixRGBTuple fg
        TuixRGBTuple bg
        unsigned char custom_bg
        unsigned char custom_fg
        unsigned char bold
        unsigned char underlined
        unsigned char italic
        unsigned char dim

    ctypedef struct TuixPixel:
        char sym[8]
        TuixPixelStyles styles
        TuixRGBTuple q_fg
        TuixRGBTuple q_bg
        unsigned char q_cached

    ctypedef struct TuixHandlerResponse:
        int requires_redraw

    ctypedef struct TuixObject:
        int uid
        const TuixBuilder* builder
        void* state
        float width_mod
        float height_mod
        float margin_top_mod
        float margin_left_mod

    ctypedef struct TuixBuffer:
        TuixObject* obj
        TuixPixel* pixels
        int width
        int height
        int required_redraw
        int margin_left
        int margin_top

    ctypedef struct TuixFinalBuffer:
        TuixPixel* pixels
        int width
        int height
        int full_redraw

    ctypedef struct TuixMouseKey:
        int event
        int btn
        int buttons_held
        int col
        int row
        double timestamp
        int has_event

    ctypedef struct TuixKeyboardKey:
        int btn
        int code
        int scancode
        int modifiers
        int pressed
        int repeat
        double timestamp
        int has_event
        char text[8]

    ctypedef struct TuixInputSnapshot:
        int term_x
        int term_y
        TuixMouseKey* mouse
        TuixKeyboardKey* keyboard

    ctypedef struct TuixScene:
        TuixBuffer** buffers
        int count
        int active
        int capacity
        int current_focus

    ctypedef struct TuixScenes:
        TuixScene** scenes
        int count
        int capacity
        char** names

    ctypedef TuixPixel* (*build_content_fn)(TuixObject* obj, TuixBuffer* buffer) nogil
    ctypedef void* (*create_state_fn)(void* params) nogil
    ctypedef void (*destroy_state_fn)(void* state) nogil
    ctypedef void (*resize_fn)(TuixObject* obj, TuixBuffer* buffer, int width, int height) nogil

    ctypedef struct TuixBuilder:
        const char* name
        const char* version
        const char* author
        const char* namespace_ "namespace"
        create_state_fn create_state
        destroy_state_fn destroy_state
        TuixHandlerResponse (*handler_func)(TuixObject* obj) nogil
        resize_fn on_resize
        build_content_fn build_content

    ctypedef struct TuixSubcycle:
        TuixObject* obj
        TuixHandlerResponse (*handler)(TuixObject* obj) nogil
        int enabled

    ctypedef struct TuixSceneSubcycles:
        char* scene_name
        TuixSubcycle** subcycles
        int count
        int capacity

    ctypedef struct TuixSubcycles:
        TuixSceneSubcycles** subcycles
        int count
        int capacity

    ctypedef struct TuixBuilders:
        TuixBuilder** builders
        int count
        int capacity

    ctypedef struct TuixRegistry:
        TuixScenes scenes
        TuixSubcycles subcycles
        TuixBuilders builders
        char* current_scene_name
        int next_uid
        int terminal_width
        int terminal_height
        int terminal_height_old
        int terminal_width_old

# ═══════════════════════════════════════════════════════════════════
#  Core lifecycle
# ═══════════════════════════════════════════════════════════════════
cdef extern from "init.h":
    int c_tuix_init "tuix_init"() nogil
    int c_tuix_shutdown "tuix_shutdown"() nogil
    int c_tuix_init_registry "tuix_init_registry"() nogil
    int c_tuix_destroy_registry "tuix_destroy_registry"() nogil

cdef extern from "main.h":
    void c_tuix_main_loop "tuix_main_loop"() nogil
    # tuix_get_terminal_size lives in main.c but is linked into the extension
    void c_tuix_get_terminal_size "tuix_get_terminal_size"(int* width, int* height) nogil

# ═══════════════════════════════════════════════════════════════════
#  Scenes & buffers
# ═══════════════════════════════════════════════════════════════════
cdef extern from "buffers.h":
    int c_tuix_init_scene "tuix_init_scene"(const char* name) nogil
    void c_tuix_free_scene "tuix_free_scene"(const char* name) nogil
    void c_tuix_clear_scene "tuix_clear_scene"(const char* name) nogil
    TuixScene* c_tuix_get_scene "tuix_get_scene"(const char* name) nogil
    char** c_tuix_get_scenes "tuix_get_scenes"() nogil
    int c_tuix_select_scene "tuix_select_scene"(char* name) nogil
    int c_tuix_scene_set_focus "tuix_scene_set_focus"(const char* scene_name, int uid) nogil
    int c_tuix_scene_set_previous_focus "tuix_scene_set_previous_focus"(const char* scene_name) nogil

cdef extern from "buffer_manager.h":
    void c_tuix_init_buffer "tuix_init_buffer"(char* scene_name, TuixObject obj) nogil
    TuixBuffer* c_tuix_get_buffer "tuix_get_buffer"(char* scene_name, int uid) nogil
    TuixBuffer* c_tuix_get_buffer_by_uid "tuix_get_buffer_by_uid"(int uid) nogil
    void c_tuix_clear_buffer "tuix_clear_buffer"(char* scene_name, int uid) nogil
    void c_tuix_free_buffer "tuix_free_buffer"(char* scene_name, int uid) nogil

# ═══════════════════════════════════════════════════════════════════
#  Object management
# ═══════════════════════════════════════════════════════════════════
cdef extern from "object_manager.h":
    TuixObject c_tuix_objects_new_object "tuix_objects_new_object"(char* builder_name, char* scene_name, float width_mod, float height_mod, float margin_top_mod, float margin_left_mod) nogil
    int c_tuix_create_object "tuix_create_object"(char* builder_name, char* scene_name, float width_mod, float height_mod, float margin_top_mod, float margin_left_mod) nogil

# ═══════════════════════════════════════════════════════════════════
#  Subcycles
# ═══════════════════════════════════════════════════════════════════
cdef extern from "subcycles/subcycle_registrant.h":
    int c_tuix_subcycle_init "tuix_subcycle_init"(char* scene_name, TuixObject* obj) nogil
    void c_tuix_subcycle_free "tuix_subcycle_free"(char* scene_name, int uid) nogil

# ═══════════════════════════════════════════════════════════════════
#  Cache manager
# ═══════════════════════════════════════════════════════════════════
cdef extern from "cache_manager.h":
    void c_tuix_cache_scenes "tuix_cache_scenes"() nogil
    void c_tuix_restore_scenes "tuix_restore_scenes"() nogil

# ═══════════════════════════════════════════════════════════════════
#  Builders — registration
# ═══════════════════════════════════════════════════════════════════
cdef extern from "content_builder/builders_registrant.h":
    int c_tuix_register_standart_builders "tuix_register_standart_builders"() nogil

# ═══════════════════════════════════════════════════════════════════
#  Progressbar builder
# ═══════════════════════════════════════════════════════════════════
cdef extern from "content_builder/builders/progressbar_builder.h":
    int c_tuix_progressbar_set_value "tuix_progressbar_set_value"(TuixObject* obj, float value) nogil
    float c_tuix_progressbar_get_value "tuix_progressbar_get_value"(TuixObject* obj) nogil
    int c_tuix_progressbar_set_style "tuix_progressbar_set_style"(TuixObject* obj, char fill, char empty, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t er, uint8_t eg, uint8_t eb) nogil
    void c_tuix_progressbar_show_percentage "tuix_progressbar_show_percentage"(TuixObject* obj, int show) nogil

# ═══════════════════════════════════════════════════════════════════
#  Canvas builder
# ═══════════════════════════════════════════════════════════════════
cdef extern from "content_builder/builders/canvas_builder.h":
    int c_tuix_canvas_set_pixel "tuix_canvas_set_pixel"(TuixObject* obj, int x, int y, char sym, uint8_t fgr, uint8_t fgg, uint8_t fgb, uint8_t bgr, uint8_t bgg, uint8_t bgb) nogil
    int c_tuix_canvas_insert_buffer "tuix_canvas_insert_buffer"(TuixObject* obj, TuixPixel* pixels, int size) nogil
    int c_tuix_canvas_draw_line "tuix_canvas_draw_line"(TuixObject* obj, int x0, int y0, int x1, int y1, char sym, uint8_t fgr, uint8_t fgg, uint8_t fgb, uint8_t bgr, uint8_t bgg, uint8_t bgb) nogil
    int c_tuix_canvas_draw_rect "tuix_canvas_draw_rect"(TuixObject* obj, int x, int y, int w, int h, char sym, int filled, uint8_t fgr, uint8_t fgg, uint8_t fgb, uint8_t bgr, uint8_t bgg, uint8_t bgb) nogil
    int c_tuix_canvas_draw_circle "tuix_canvas_draw_circle"(TuixObject* obj, int cx, int cy, int radius, char sym, int filled, uint8_t fgr, uint8_t fgg, uint8_t fgb, uint8_t bgr, uint8_t bgg, uint8_t bgb) nogil
    int c_tuix_canvas_draw_text "tuix_canvas_draw_text"(TuixObject* obj, int x, int y, const char* text, uint8_t fgr, uint8_t fgg, uint8_t fgb, uint8_t bgr, uint8_t bgg, uint8_t bgb) nogil
    int c_tuix_canvas_draw_sprite "tuix_canvas_draw_sprite"(TuixObject* obj, int dst_x, int dst_y, int sprite_w, int sprite_h, const TuixPixel* sprite) nogil
    int c_tuix_canvas_cache_sprite "tuix_canvas_cache_sprite"(TuixObject* obj, int sprite_w, int sprite_h, const TuixPixel* sprite) nogil
    void c_tuix_canvas_free_cached_sprite "tuix_canvas_free_cached_sprite"(TuixObject* obj, int id) nogil
    int c_tuix_canvas_draw_cached_sprite "tuix_canvas_draw_cached_sprite"(TuixObject* obj, int id, int dst_x, int dst_y) nogil

# ═══════════════════════════════════════════════════════════════════
#  Choice builder
# ═══════════════════════════════════════════════════════════════════
cdef extern from "content_builder/builders/choice_builder.h":
    int c_tuix_choice_set_options "tuix_choice_set_options"(TuixObject* obj, const char** labels, int count) nogil
    int c_tuix_choice_feed_input "tuix_choice_feed_input"(TuixObject* obj, TuixInputSnapshot snap) nogil
    int c_tuix_choice_get_selected "tuix_choice_get_selected"(TuixObject* obj) nogil
    int c_tuix_choice_is_confirmed "tuix_choice_is_confirmed"(TuixObject* obj) nogil
    int c_tuix_choice_get_result "tuix_choice_get_result"(TuixObject* obj) nogil
    void c_tuix_choice_reset "tuix_choice_reset"(TuixObject* obj) nogil

# ═══════════════════════════════════════════════════════════════════
#  Input builder
# ═══════════════════════════════════════════════════════════════════
cdef extern from "content_builder/builders/input_builder.h":
    int c_tuix_input_set_placeholder "tuix_input_set_placeholder"(TuixObject* obj, const char* text) nogil
    int c_tuix_input_feed_input "tuix_input_feed_input"(TuixObject* obj, TuixInputSnapshot snap) nogil
    const char* c_tuix_input_get_text "tuix_input_get_text"(TuixObject* obj) nogil
    int c_tuix_input_is_submitted "tuix_input_is_submitted"(TuixObject* obj) nogil
    const char* c_tuix_input_get_result "tuix_input_get_result"(TuixObject* obj) nogil
    void c_tuix_input_reset "tuix_input_reset"(TuixObject* obj) nogil

# ═══════════════════════════════════════════════════════════════════
#  Input system (listener thread)
# ═══════════════════════════════════════════════════════════════════
cdef extern from "input/input.h":
    int tuix_input_keyboard_enabled
    int tuix_input_mouse_enabled
    void c_listen_input "listen_input"() nogil
    void c_stop_input_listening "stop_input_listening"() nogil
    TuixInputSnapshot c_get_input_snapshot "get_input_snapshot"() nogil

# ═══════════════════════════════════════════════════════════════════
#  Rendering
# ═══════════════════════════════════════════════════════════════════
cdef extern from "rendering.h":
    ctypedef void (*TuixRowDoneCallback)(TuixFinalBuffer*, int, void*)
    TuixRGBTuple c_tuix_rgb16 "tuix_rgb16"(TuixRGBTuple c) nogil
    void c_tuix_render_streaming "tuix_render_streaming"(TuixFinalBuffer* buffer, TuixRowDoneCallback on_row_done, void* user_data) nogil

# ═══════════════════════════════════════════════════════════════════
#  Compositor & geometry resolver
# ═══════════════════════════════════════════════════════════════════
cdef extern from "compositor/compositor.h":
    TuixFinalBuffer* c_tuix_composite_scene "tuix_composite_scene"(TuixScene* scene) nogil

cdef extern from "compositor/geometry_resolver.h":
    void c_tuix_resolve_geometry "tuix_resolve_geometry"(TuixBuffer* buf) nogil

# ═══════════════════════════════════════════════════════════════════
#  Batch executor & timer
# ═══════════════════════════════════════════════════════════════════
cdef extern from "batch_executor.h":
    int c_tuix_execute_command_buffer "tuix_execute_command_buffer"(const unsigned char* buf, size_t length) nogil
    uint64_t c_tuix_time_now_us "tuix_time_now_us"() nogil

# ═══════════════════════════════════════════════════════════════════
#  Registry (global)
# ═══════════════════════════════════════════════════════════════════
cdef extern from "tuix_registry.h":
    void c_tuix_lock "tuix_lock"() nogil
    void c_tuix_unlock "tuix_unlock"() nogil
    TuixRegistry tuix_registry


# ═══════════════════════════════════════════════════════════════════
#  INLINE HELPERS (pure C speed)
# ═══════════════════════════════════════════════════════════════════

cdef inline uint32_t _read_u32_le(const unsigned char* buf, size_t off) noexcept nogil:
    return ((<uint32_t>buf[off])
          | ((<uint32_t>buf[off+1]) << 8)
          | ((<uint32_t>buf[off+2]) << 16)
          | ((<uint32_t>buf[off+3]) << 24))

cdef inline void _write_u32_le(unsigned char* buf, uint32_t v) noexcept nogil:
    buf[0] = <unsigned char>(v & 0xFF)
    buf[1] = <unsigned char>((v >> 8) & 0xFF)
    buf[2] = <unsigned char>((v >> 16) & 0xFF)
    buf[3] = <unsigned char>((v >> 24) & 0xFF)

# Max bytes for one ANSI truecolor escape: "\x1b[38;2;RRR;GGG;BBBm" = 20 chars max
DEF ANSI_ESC_MAX = 24
# Pre-sized output buffer: worst case per pixel = 2*ANSI_ESC_MAX + 8 (sym)
DEF PIXEL_OUT_MAX = 56

cdef inline int _emit_fg(char* buf, int r, int g, int b) noexcept nogil:
    """Write fg ANSI escape to buf, return bytes written."""
    return snprintf(buf, ANSI_ESC_MAX, "\x1b[38;2;%d;%d;%dm", r, g, b)

cdef inline int _emit_bg(char* buf, int r, int g, int b) noexcept nogil:
    """Write bg ANSI escape to buf, return bytes written."""
    return snprintf(buf, ANSI_ESC_MAX, "\x1b[48;2;%d;%d;%dm", r, g, b)


# ═══════════════════════════════════════════════════════════════════
#  MODULE-LEVEL PYTHON API — lifecycle
# ═══════════════════════════════════════════════════════════════════

cpdef int init():
    return c_tuix_init()

cpdef int shutdown():
    return c_tuix_shutdown()

cpdef int init_registry():
    return c_tuix_init_registry()

cpdef int destroy_registry():
    return c_tuix_destroy_registry()

cpdef void main_loop():
    with nogil:
        c_tuix_main_loop()

cpdef tuple get_terminal_size():
    cdef int w = 0, h = 0
    with nogil:
        c_tuix_get_terminal_size(&w, &h)
    return (w, h)


cpdef int tuix_init():
    return init()

cpdef int tuix_shutdown():
    return shutdown()

cpdef int tuix_init_registry():
    return init_registry()

cpdef int tuix_destroy_registry():
    return destroy_registry()

cpdef void tuix_main_loop():
    main_loop()

cpdef tuple tuix_get_terminal_size():
    return get_terminal_size()

cpdef void tuix_lock():
    with nogil:
        c_tuix_lock()

cpdef void tuix_unlock():
    with nogil:
        c_tuix_unlock()

cpdef void tuix_cache_scenes():
    with nogil:
        c_tuix_cache_scenes()

cpdef void tuix_restore_scenes():
    with nogil:
        c_tuix_restore_scenes()


# ═══════════════════════════════════════════════════════════════════
#  Scenes
# ═══════════════════════════════════════════════════════════════════

cpdef int tuix_init_scene(bytes name):
    cdef const char* c = name
    cdef int r
    with nogil:
        r = c_tuix_init_scene(c)
    return r

cpdef void tuix_free_scene(bytes name):
    cdef const char* c = name
    with nogil:
        c_tuix_free_scene(c)

cpdef void tuix_clear_scene(bytes name):
    cdef const char* c = name
    with nogil:
        c_tuix_clear_scene(c)

cpdef uintptr_t tuix_get_scene(bytes name):
    cdef const char* c = name
    cdef TuixScene* scene
    with nogil:
        scene = c_tuix_get_scene(c)
    return <uintptr_t>scene

cpdef list tuix_get_scenes():
    cdef char** names = NULL
    cdef list out = []
    cdef int i
    cdef int count = tuix_registry.scenes.count
    with nogil:
        names = c_tuix_get_scenes()
    if names == NULL:
        return out
    for i in range(count):
        if names[i] != NULL:
            out.append(names[i])
            free(names[i])
    free(names)
    return out

cpdef int tuix_scene_set_focus(bytes scene_name, int uid):
    cdef const char* sn = scene_name
    cdef int r
    with nogil:
        r = c_tuix_scene_set_focus(sn, uid)
    return r

cpdef int tuix_scene_set_previous_focus(bytes scene_name):
    cdef const char* sn = scene_name
    cdef int r
    with nogil:
        r = c_tuix_scene_set_previous_focus(sn)
    return r


# ═══════════════════════════════════════════════════════════════════
#  Object management
# ═══════════════════════════════════════════════════════════════════

cpdef uintptr_t tuix_objects_new_object(bytes builder_name, bytes scene_name,
                                        float w, float h, float mt, float ml):
    cdef char* bn = builder_name
    cdef char* sn = scene_name
    cdef TuixObject obj
    cdef TuixObject* heap_obj = NULL
    with nogil:
        obj = c_tuix_objects_new_object(bn, sn, w, h, mt, ml)
    heap_obj = <TuixObject*>malloc(sizeof(TuixObject))
    if heap_obj == NULL:
        raise MemoryError()
    heap_obj[0] = obj
    return <uintptr_t>heap_obj

cpdef void tuix_free_object_ptr(uintptr_t obj_addr):
    if obj_addr != 0:
        free(<void*>obj_addr)

cpdef int tuix_create_object(bytes builder_name, bytes scene_name,
                             float w, float h, float mt, float ml):
    cdef char* bn = builder_name
    cdef char* sn = scene_name
    cdef int r
    with nogil:
        r = c_tuix_create_object(bn, sn, w, h, mt, ml)
    return r


# ═══════════════════════════════════════════════════════════════════
#  Buffer access
# ═══════════════════════════════════════════════════════════════════

cpdef void tuix_init_buffer(bytes scene_name, uintptr_t obj_addr):
    cdef char* sn = scene_name
    cdef TuixObject obj
    if obj_addr == 0:
        raise ValueError("obj_addr must be non-zero")
    obj = (<TuixObject*>obj_addr)[0]
    with nogil:
        c_tuix_init_buffer(sn, obj)

cpdef uintptr_t tuix_get_buffer_by_uid(int uid):
    """Return raw pointer to TuixBuffer (as uintptr_t) for the given UID."""
    cdef TuixBuffer* buf
    with nogil:
        buf = c_tuix_get_buffer_by_uid(uid)
    return <uintptr_t>buf

cpdef uintptr_t tuix_get_buffer(bytes scene_name, int uid):
    cdef char* sn = scene_name
    cdef TuixBuffer* buf
    with nogil:
        buf = c_tuix_get_buffer(sn, uid)
    return <uintptr_t>buf

cpdef void tuix_free_buffer(bytes scene_name, int uid):
    cdef char* sn = scene_name
    with nogil:
        c_tuix_free_buffer(sn, uid)

cpdef void tuix_clear_buffer(bytes scene_name, int uid):
    cdef char* sn = scene_name
    with nogil:
        c_tuix_clear_buffer(sn, uid)

cpdef int buffer_width(uintptr_t buf_ptr):
    if buf_ptr == 0:
        return 0
    return (<TuixBuffer*>buf_ptr).width

cpdef int buffer_height(uintptr_t buf_ptr):
    if buf_ptr == 0:
        return 0
    return (<TuixBuffer*>buf_ptr).height

cpdef uintptr_t buffer_obj(uintptr_t buf_ptr):
    """Return the TuixObject* from a TuixBuffer*."""
    if buf_ptr == 0:
        return 0
    return <uintptr_t>((<TuixBuffer*>buf_ptr).obj)


# ═══════════════════════════════════════════════════════════════════
#  Builders registration
# ═══════════════════════════════════════════════════════════════════

cpdef int tuix_register_standart_builders():
    cdef int r
    with nogil:
        r = c_tuix_register_standart_builders()
    return r


# ═══════════════════════════════════════════════════════════════════
#  Progressbar — direct C calls, zero overhead
# ═══════════════════════════════════════════════════════════════════

cpdef int tuix_progressbar_set_value(uintptr_t obj_addr, float value):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_progressbar_set_value(p, value)
    return r

cpdef float tuix_progressbar_get_value(uintptr_t obj_addr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef float r
    with nogil:
        r = c_tuix_progressbar_get_value(p)
    return r

cpdef int tuix_progressbar_set_style(uintptr_t obj_addr, char fill, char empty,
                                     unsigned char fr, unsigned char fg, unsigned char fb,
                                     unsigned char er, unsigned char eg, unsigned char eb):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_progressbar_set_style(p, fill, empty, fr, fg, fb, er, eg, eb)
    return r

cpdef void tuix_progressbar_show_percentage(uintptr_t obj_addr, int show):
    cdef TuixObject* p = <TuixObject*>obj_addr
    with nogil:
        c_tuix_progressbar_show_percentage(p, show)


# ═══════════════════════════════════════════════════════════════════
#  Canvas builder — all drawing primitives
# ═══════════════════════════════════════════════════════════════════

cpdef int tuix_canvas_set_pixel(uintptr_t obj_addr, int x, int y, char sym,
                                unsigned char fgr, unsigned char fgg, unsigned char fgb,
                                unsigned char bgr, unsigned char bgg, unsigned char bgb):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_canvas_set_pixel(p, x, y, sym, fgr, fgg, fgb, bgr, bgg, bgb)
    return r

cpdef int tuix_canvas_draw_line(uintptr_t obj_addr, int x0, int y0, int x1, int y1,
                                char sym,
                                unsigned char fgr, unsigned char fgg, unsigned char fgb,
                                unsigned char bgr, unsigned char bgg, unsigned char bgb):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_canvas_draw_line(p, x0, y0, x1, y1, sym, fgr, fgg, fgb, bgr, bgg, bgb)
    return r

cpdef int tuix_canvas_draw_rect(uintptr_t obj_addr, int x, int y, int w, int h,
                                char sym, int filled,
                                unsigned char fgr, unsigned char fgg, unsigned char fgb,
                                unsigned char bgr, unsigned char bgg, unsigned char bgb):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_canvas_draw_rect(p, x, y, w, h, sym, filled, fgr, fgg, fgb, bgr, bgg, bgb)
    return r

cpdef int tuix_canvas_draw_circle(uintptr_t obj_addr, int cx, int cy, int radius,
                                  char sym, int filled,
                                  unsigned char fgr, unsigned char fgg, unsigned char fgb,
                                  unsigned char bgr, unsigned char bgg, unsigned char bgb):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_canvas_draw_circle(p, cx, cy, radius, sym, filled, fgr, fgg, fgb, bgr, bgg, bgb)
    return r

cpdef int tuix_canvas_draw_text(uintptr_t obj_addr, int x, int y, bytes text,
                                unsigned char fgr, unsigned char fgg, unsigned char fgb,
                                unsigned char bgr, unsigned char bgg, unsigned char bgb):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef const char* t = text
    cdef int r
    with nogil:
        r = c_tuix_canvas_draw_text(p, x, y, t, fgr, fgg, fgb, bgr, bgg, bgb)
    return r

cpdef int tuix_canvas_draw_sprite(uintptr_t obj_addr, int dst_x, int dst_y,
                                  int sprite_w, int sprite_h, uintptr_t sprite_ptr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef const TuixPixel* sp = <const TuixPixel*>sprite_ptr
    cdef int r
    with nogil:
        r = c_tuix_canvas_draw_sprite(p, dst_x, dst_y, sprite_w, sprite_h, sp)
    return r

cpdef int tuix_canvas_insert_buffer(uintptr_t obj_addr, uintptr_t pixels_ptr, int size):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef TuixPixel* px = <TuixPixel*>pixels_ptr
    cdef int r
    with nogil:
        r = c_tuix_canvas_insert_buffer(p, px, size)
    return r

cpdef int tuix_canvas_cache_sprite(uintptr_t obj_addr, int sprite_w, int sprite_h,
                                   uintptr_t sprite_ptr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef const TuixPixel* sp = <const TuixPixel*>sprite_ptr
    cdef int r
    with nogil:
        r = c_tuix_canvas_cache_sprite(p, sprite_w, sprite_h, sp)
    return r

cpdef void tuix_canvas_free_cached_sprite(uintptr_t obj_addr, int sprite_id):
    cdef TuixObject* p = <TuixObject*>obj_addr
    with nogil:
        c_tuix_canvas_free_cached_sprite(p, sprite_id)

cpdef int tuix_canvas_draw_cached_sprite(uintptr_t obj_addr, int sprite_id,
                                         int dst_x, int dst_y):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_canvas_draw_cached_sprite(p, sprite_id, dst_x, dst_y)
    return r


# ═══════════════════════════════════════════════════════════════════
#  Choice builder
# ═══════════════════════════════════════════════════════════════════

cpdef int tuix_choice_set_options(uintptr_t obj_addr, list labels):
    """Set choice options from a list of bytes strings."""
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int count = <int>len(labels)
    cdef const char** c_labels = <const char**>malloc(count * sizeof(const char*))
    if c_labels == NULL:
        raise MemoryError()
    cdef int i
    for i in range(count):
        c_labels[i] = <const char*>(<bytes>labels[i])
    cdef int r
    with nogil:
        r = c_tuix_choice_set_options(p, c_labels, count)
    free(c_labels)
    return r

cpdef int tuix_choice_get_selected(uintptr_t obj_addr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_choice_get_selected(p)
    return r

cpdef int tuix_choice_is_confirmed(uintptr_t obj_addr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_choice_is_confirmed(p)
    return r

cpdef int tuix_choice_get_result(uintptr_t obj_addr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_choice_get_result(p)
    return r

cpdef void tuix_choice_reset(uintptr_t obj_addr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    with nogil:
        c_tuix_choice_reset(p)

cpdef int tuix_choice_feed_input(uintptr_t obj_addr, object snap):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef TuixKeyboardKey kb_val
    cdef TuixMouseKey ms_val
    cdef TuixInputSnapshot c_snap
    cdef int r
    c_snap.term_x = snap.term_x
    c_snap.term_y = snap.term_y
    c_snap.keyboard = NULL
    c_snap.mouse = NULL
    kb = getattr(snap, '_keyboard', None)
    if kb is not None:
        kb_val.btn = kb.btn
        kb_val.code = kb.code
        kb_val.scancode = kb.scancode
        kb_val.modifiers = kb.modifiers
        kb_val.pressed = kb.pressed
        kb_val.repeat = kb.repeat
        kb_val.has_event = kb.has_event
        kb_val.timestamp = 0.0
        kb_val.text[0] = 0
        c_snap.keyboard = &kb_val
    ms = getattr(snap, '_mouse', None)
    if ms is not None:
        ms_val.event = ms.event
        ms_val.btn = ms.btn
        ms_val.buttons_held = ms.buttons_held
        ms_val.col = ms.col
        ms_val.row = ms.row
        ms_val.has_event = ms.has_event
        ms_val.timestamp = 0.0
        c_snap.mouse = &ms_val
    with nogil:
        r = c_tuix_choice_feed_input(p, c_snap)
    return r


# ═══════════════════════════════════════════════════════════════════
#  Input builder
# ═══════════════════════════════════════════════════════════════════

cpdef int tuix_input_set_placeholder(uintptr_t obj_addr, bytes text):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef const char* t = text
    cdef int r
    with nogil:
        r = c_tuix_input_set_placeholder(p, t)
    return r

cpdef bytes tuix_input_get_text(uintptr_t obj_addr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef const char* r
    with nogil:
        r = c_tuix_input_get_text(p)
    if r == NULL:
        return b""
    return r

cpdef int tuix_input_is_submitted(uintptr_t obj_addr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef int r
    with nogil:
        r = c_tuix_input_is_submitted(p)
    return r

cpdef bytes tuix_input_get_result(uintptr_t obj_addr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef const char* r
    with nogil:
        r = c_tuix_input_get_result(p)
    if r == NULL:
        return None
    return r

cpdef int tuix_input_feed_input(uintptr_t obj_addr, object snap):
    cdef TuixObject* p = <TuixObject*>obj_addr
    cdef TuixKeyboardKey kb_val
    cdef TuixMouseKey ms_val
    cdef TuixInputSnapshot c_snap
    cdef int r
    c_snap.term_x = snap.term_x
    c_snap.term_y = snap.term_y
    c_snap.keyboard = NULL
    c_snap.mouse = NULL
    kb = getattr(snap, '_keyboard', None)
    if kb is not None:
        kb_val.btn = kb.btn
        kb_val.code = kb.code
        kb_val.scancode = kb.scancode
        kb_val.modifiers = kb.modifiers
        kb_val.pressed = kb.pressed
        kb_val.repeat = kb.repeat
        kb_val.has_event = kb.has_event
        kb_val.timestamp = 0.0
        kb_val.text[0] = 0
        c_snap.keyboard = &kb_val
    ms = getattr(snap, '_mouse', None)
    if ms is not None:
        ms_val.event = ms.event
        ms_val.btn = ms.btn
        ms_val.buttons_held = ms.buttons_held
        ms_val.col = ms.col
        ms_val.row = ms.row
        ms_val.has_event = ms.has_event
        ms_val.timestamp = 0.0
        c_snap.mouse = &ms_val
    with nogil:
        r = c_tuix_input_feed_input(p, c_snap)
    return r

cpdef void tuix_input_reset(uintptr_t obj_addr):
    cdef TuixObject* p = <TuixObject*>obj_addr
    with nogil:
        c_tuix_input_reset(p)


# ═══════════════════════════════════════════════════════════════════
#  Input system (listener thread)
# ═══════════════════════════════════════════════════════════════════

cdef bint _input_active = 0

cpdef void listen_input():
    global _input_active
    with nogil:
        c_listen_input()
    _input_active = 1

cpdef void stop_input_listening():
    global _input_active
    _input_active = 0
    with nogil:
        c_stop_input_listening()

def get_input_snapshot():
    """Return input snapshot as a lightweight Python object.
    Returns empty snapshot if listen_input() has not been called."""
    if not _input_active:
        return _empty_snapshot()
    cdef TuixInputSnapshot snap
    with nogil:
        snap = c_get_input_snapshot()
    return _wrap_input_snapshot(snap)

cdef object _empty_snapshot():
    cdef InputSnapshot out = InputSnapshot.__new__(InputSnapshot)
    out.term_x = 0
    out.term_y = 0
    out._keyboard = None
    out._mouse = None
    return out

cdef object _wrap_input_snapshot(TuixInputSnapshot snap):
    """Convert C snapshot to a Cython extension type — fast, no dict overhead."""
    cdef InputSnapshot out = InputSnapshot.__new__(InputSnapshot)
    out.term_x = snap.term_x
    out.term_y = snap.term_y
    if snap.keyboard != NULL:
        kb = KeyboardKey.__new__(KeyboardKey)
        kb.btn = snap.keyboard.btn
        kb.code = snap.keyboard.code
        kb.scancode = snap.keyboard.scancode
        kb.modifiers = snap.keyboard.modifiers
        kb.pressed = snap.keyboard.pressed
        kb.repeat = snap.keyboard.repeat
        kb.has_event = snap.keyboard.has_event
        out._keyboard = kb
    else:
        out._keyboard = None
    if snap.mouse != NULL:
        ms = MouseKey.__new__(MouseKey)
        ms.event = snap.mouse.event
        ms.btn = snap.mouse.btn
        ms.buttons_held = snap.mouse.buttons_held
        ms.col = snap.mouse.col
        ms.row = snap.mouse.row
        ms.has_event = snap.mouse.has_event
        out._mouse = ms
    else:
        out._mouse = None
    return out


# Lightweight extension types for input events — no __dict__, slots only
cdef class KeyboardKey:
    cdef public int btn, code, scancode, modifiers, pressed, repeat, has_event

cdef class MouseKey:
    cdef public int event, btn, buttons_held, col, row, has_event

cdef class InputSnapshot:
    cdef public int term_x, term_y
    cdef public object _keyboard
    cdef public object _mouse

    @property
    def keyboard(self):
        return self._keyboard

    @property
    def mouse(self):
        return self._mouse


# ═══════════════════════════════════════════════════════════════════
#  Batch executor & timer
# ═══════════════════════════════════════════════════════════════════

cpdef int execute_command_buffer(bytes data):
    cdef const unsigned char* p = <const unsigned char*>PyBytes_AS_STRING(data)
    cdef size_t length = <size_t>len(data)
    cdef int r
    with nogil:
        r = c_tuix_execute_command_buffer(p, length)
    return r

cpdef int tuix_execute_command_buffer(bytes data):
    return execute_command_buffer(data)

cpdef uint64_t time_now_us():
    cdef uint64_t r
    with nogil:
        r = c_tuix_time_now_us()
    return r

cpdef uint64_t tuix_time_now_us():
    return time_now_us()


# ═══════════════════════════════════════════════════════════════════
#  ANSI rendering — C-level buffer, zero Python string ops
# ═══════════════════════════════════════════════════════════════════

cdef bytes _render_ansi_from_pixels(TuixPixel* pix, int W, int H):
    """Build ANSI output entirely in C char buffer — no Python format strings."""
    cdef size_t n = <size_t>W * H
    cdef size_t alloc = n * PIXEL_OUT_MAX + 8  # generous upper bound
    cdef char* out = <char*>malloc(alloc)
    if out == NULL:
        return b''
    cdef size_t pos = 0
    cdef int prev_fr = -1, prev_fg = -1, prev_fb = -1
    cdef int prev_br = -1, prev_bg = -1, prev_bb = -1
    cdef int fr, fg, fb, br, bg, bb
    cdef size_t idx
    cdef TuixPixel* pp
    cdef int wrote

    for idx in range(n):
        pp = &pix[idx]
        if pp.styles.custom_fg or pp.styles.custom_bg:
            fr = pp.styles.fg.r; fg = pp.styles.fg.g; fb = pp.styles.fg.b
            br = pp.styles.bg.r; bg = pp.styles.bg.g; bb = pp.styles.bg.b
            if fr != prev_fr or fg != prev_fg or fb != prev_fb:
                wrote = _emit_fg(out + pos, fr, fg, fb)
                if wrote > 0:
                    pos += wrote
                prev_fr = fr; prev_fg = fg; prev_fb = fb
            if br != prev_br or bg != prev_bg or bb != prev_bb:
                wrote = _emit_bg(out + pos, br, bg, bb)
                if wrote > 0:
                    pos += wrote
                prev_br = br; prev_bg = bg; prev_bb = bb
        out[pos] = pp.sym[0]
        pos += 1

    # Reset: "\x1b[0m"
    out[pos] = 0x1b; pos += 1
    out[pos] = 0x5b; pos += 1  # '['
    out[pos] = 0x30; pos += 1  # '0'
    out[pos] = 0x6d; pos += 1  # 'm'

    cdef bytes result = PyBytes_FromStringAndSize(out, <Py_ssize_t>pos)
    free(out)
    return result


# ═══════════════════════════════════════════════════════════════════
#  Pixel conversion: Python buffer → C TuixPixel array
# ═══════════════════════════════════════════════════════════════════

cdef TuixPixel* _py_buf_to_pixels(object py_buf, int* out_w, int* out_h) except NULL:
    """Convert a Python TuixFinalBuffer-like object to a malloc'd TuixPixel array.
    Sets out_w / out_h. Caller must free() the returned pointer."""
    cdef int W = int(py_buf.width)
    cdef int H = int(py_buf.height)
    out_w[0] = W
    out_h[0] = H
    cdef size_t n = <size_t>W * H
    if n == 0:
        return <TuixPixel*>malloc(1)  # non-NULL sentinel

    cdef TuixPixel* pix = <TuixPixel*>malloc(n * sizeof(TuixPixel))
    if pix == NULL:
        raise MemoryError()

    cdef Py_ssize_t i
    cdef object p, s_obj
    cdef bytes s_bytes
    cdef const char* s_ptr
    cdef Py_ssize_t slen, copy_len

    for i in range(<Py_ssize_t>n):
        p = py_buf.pixels[i]
        # Symbol
        s_obj = getattr(p, 'sym', b' ')
        if not isinstance(s_obj, bytes):
            if isinstance(s_obj, bytearray):
                s_bytes = bytes(s_obj)
            else:
                s_bytes = str(s_obj).encode('utf-8')
        else:
            s_bytes = <bytes>s_obj
        slen = len(s_bytes)
        copy_len = slen if slen < 7 else 7
        s_ptr = PyBytes_AS_STRING(s_bytes)
        memcpy(&pix[i].sym[0], s_ptr, copy_len)
        pix[i].sym[copy_len] = 0

        # Styles — inline, no try/except per field
        pix[i].styles.fg.r = <unsigned char>int(getattr(getattr(p, 'fg', None), 'r', 0) or 0)
        pix[i].styles.fg.g = <unsigned char>int(getattr(getattr(p, 'fg', None), 'g', 0) or 0)
        pix[i].styles.fg.b = <unsigned char>int(getattr(getattr(p, 'fg', None), 'b', 0) or 0)
        pix[i].styles.bg.r = <unsigned char>int(getattr(getattr(p, 'bg', None), 'r', 0) or 0)
        pix[i].styles.bg.g = <unsigned char>int(getattr(getattr(p, 'bg', None), 'g', 0) or 0)
        pix[i].styles.bg.b = <unsigned char>int(getattr(getattr(p, 'bg', None), 'b', 0) or 0)
        pix[i].styles.custom_fg = 1 if getattr(p, 'custom_fg', 0) else 0
        pix[i].styles.custom_bg = 1 if getattr(p, 'custom_bg', 0) else 0
        pix[i].styles.bold = 0
        pix[i].styles.underlined = 0
        pix[i].styles.italic = 0
        pix[i].styles.dim = 0
        pix[i].q_cached = <unsigned char>(getattr(p, 'q_cached', 0) or 0)

    return pix


# ═══════════════════════════════════════════════════════════════════
#  Public render & patch API
# ═══════════════════════════════════════════════════════════════════

cpdef bytes py_tuix_render_streaming(object py_buf):
    """Convert Python buffer → native pixels, call C renderer, return ANSI bytes."""
    cdef int W = 0, H = 0
    cdef TuixPixel* pix = _py_buf_to_pixels(py_buf, &W, &H)
    cdef size_t n = <size_t>W * H
    if n == 0:
        free(pix)
        return b''

    cdef TuixFinalBuffer fb
    fb.pixels = pix
    fb.width = W
    fb.height = H
    fb.full_redraw = int(getattr(py_buf, 'full_redraw', 1))

    with nogil:
        c_tuix_render_streaming(&fb, NULL, NULL)

    cdef bytes result = _render_ansi_from_pixels(pix, W, H)
    free(pix)
    return result


cpdef bytes tuix_render_streaming(object py_buf):
    return py_tuix_render_streaming(py_buf)


cpdef bytes py_tuix_commit_patch_and_render(object py_buf, bytes data):
    """Apply 20-byte patch records to a copy of `py_buf`, render, return ANSI bytes."""
    cdef Py_ssize_t dlen = <Py_ssize_t>len(data)
    if dlen == 0:
        return b''
    if dlen % 20 != 0:
        raise ValueError('patch length must be a multiple of 20')

    cdef int W = 0, H = 0
    cdef TuixPixel* pix = _py_buf_to_pixels(py_buf, &W, &H)
    cdef size_t n = <size_t>W * H
    if n == 0:
        free(pix)
        return b''

    # Apply patch without GIL
    cdef const unsigned char* pd = <const unsigned char*>PyBytes_AS_STRING(data)
    cdef Py_ssize_t nrec = dlen // 20
    cdef Py_ssize_t k
    cdef size_t off
    cdef uint32_t idx
    cdef TuixPixel* pp
    cdef unsigned char flags

    with nogil:
        for k in range(nrec):
            off = <size_t>(k * 20)
            idx = _read_u32_le(pd, off)
            if idx >= n:
                continue
            pp = &pix[idx]
            memcpy(&pp.sym[0], pd + off + 4, 7)
            pp.sym[7] = 0
            pp.styles.fg.r = pd[off + 12]
            pp.styles.fg.g = pd[off + 13]
            pp.styles.fg.b = pd[off + 14]
            pp.styles.bg.r = pd[off + 15]
            pp.styles.bg.g = pd[off + 16]
            pp.styles.bg.b = pd[off + 17]
            flags = pd[off + 18]
            pp.styles.custom_fg = 1 if (flags & 1) else 0
            pp.styles.custom_bg = 1 if (flags & 2) else 0

    cdef TuixFinalBuffer fb
    fb.pixels = pix
    fb.width = W
    fb.height = H
    fb.full_redraw = int(getattr(py_buf, 'full_redraw', 1))

    with nogil:
        c_tuix_render_streaming(&fb, NULL, NULL)

    cdef bytes result = _render_ansi_from_pixels(pix, W, H)
    free(pix)
    return result


# ═══════════════════════════════════════════════════════════════════
#  Persistent native buffer API
# ═══════════════════════════════════════════════════════════════════

cpdef uintptr_t py_tuix_create_native_buffer(object py_buf):
    """Create a persistent native TuixFinalBuffer. Returns handle (0 on failure)."""
    cdef int W = 0, H = 0
    cdef TuixPixel* pix = _py_buf_to_pixels(py_buf, &W, &H)
    cdef size_t n = <size_t>W * H
    if n == 0:
        free(pix)
        return 0

    cdef TuixFinalBuffer* fb = <TuixFinalBuffer*>malloc(sizeof(TuixFinalBuffer))
    if fb == NULL:
        free(pix)
        return 0
    fb.pixels = pix
    fb.width = W
    fb.height = H
    fb.full_redraw = int(getattr(py_buf, 'full_redraw', 1))
    return <uintptr_t>fb


cpdef int py_tuix_free_native_buffer(uintptr_t handle):
    cdef TuixFinalBuffer* fb = <TuixFinalBuffer*>handle
    if fb == NULL:
        return -1
    if fb.pixels != NULL:
        free(fb.pixels)
    free(fb)
    return 0


cpdef bytes py_tuix_apply_patch_and_render(uintptr_t handle, bytes data):
    """Apply patch in-place to persistent native buffer, render, return ANSI bytes."""
    if handle == 0:
        raise ValueError('invalid handle')
    cdef TuixFinalBuffer* fb = <TuixFinalBuffer*>handle

    cdef Py_ssize_t dlen = <Py_ssize_t>len(data)
    cdef const unsigned char* pd = <const unsigned char*>PyBytes_AS_STRING(data)
    cdef size_t n = <size_t>fb.width * fb.height
    cdef Py_ssize_t nrec = dlen // 20
    cdef Py_ssize_t k
    cdef size_t off
    cdef uint32_t idx
    cdef TuixPixel* pp
    cdef unsigned char flags

    with nogil:
        for k in range(nrec):
            off = <size_t>(k * 20)
            idx = _read_u32_le(pd, off)
            if idx >= n:
                continue
            pp = &fb.pixels[idx]
            memcpy(&pp.sym[0], pd + off + 4, 7)
            pp.sym[7] = 0
            pp.styles.fg.r = pd[off + 12]
            pp.styles.fg.g = pd[off + 13]
            pp.styles.fg.b = pd[off + 14]
            pp.styles.bg.r = pd[off + 15]
            pp.styles.bg.g = pd[off + 16]
            pp.styles.bg.b = pd[off + 17]
            flags = pd[off + 18]
            pp.styles.custom_fg = 1 if (flags & 1) else 0
            pp.styles.custom_bg = 1 if (flags & 2) else 0

        c_tuix_render_streaming(fb, NULL, NULL)

    return _render_ansi_from_pixels(fb.pixels, fb.width, fb.height)


# ═══════════════════════════════════════════════════════════════════
#  Fast fill (benchmark utility)
# ═══════════════════════════════════════════════════════════════════

cpdef bytes fast_fill_symbols(int W, int H, int seed):
    cdef Py_ssize_t n = <Py_ssize_t>W * H
    if n <= 0:
        return b''
    cdef char* buf = <char*>malloc(n)
    if buf == NULL:
        raise MemoryError()
    cdef int s = seed
    cdef Py_ssize_t i
    with nogil:
        for i in range(n):
            s = (s * 1103515245 + 12345) & 0x7FFFFFFF
            buf[i] = <char>(32 + ((s >> 16) % 95))
    cdef bytes result = PyBytes_FromStringAndSize(buf, n)
    free(buf)
    return result


# ═══════════════════════════════════════════════════════════════════
#  Batch API — C-array-backed, minimal Python overhead
# ═══════════════════════════════════════════════════════════════════

cdef enum _ActionType:
    ACT_CREATE = 1
    ACT_PROGRESS_VALUE = 2
    ACT_PROGRESS_STYLE = 3
    ACT_INIT_SCENE = 4
    ACT_FREE_SCENE = 5
    ACT_CLEAR_SCENE = 6

cdef struct _Action:
    int type
    int s1
    int s2
    uintptr_t ptr    # object pointer for progress ops
    float f1, f2, f3, f4
    char c1, c2
    unsigned char uc1, uc2, uc3, uc4, uc5, uc6  # color bytes

DEF BATCH_INIT_CAP = 64
DEF STRPOOL_INIT_CAP = 4096

cdef class Batch:
    """Accumulate actions in flat C arrays, execute in a tight nogil loop."""
    cdef _Action* _acts
    cdef int _count
    cdef int _cap
    # String pool: contiguous char buffer + offsets
    cdef char* _strpool
    cdef int _sp_len
    cdef int _sp_cap
    # Keep Python bytes refs alive during execution
    cdef list _refs

    def __cinit__(self):
        self._cap = BATCH_INIT_CAP
        self._acts = <_Action*>malloc(self._cap * sizeof(_Action))
        self._count = 0
        self._sp_cap = STRPOOL_INIT_CAP
        self._strpool = <char*>malloc(self._sp_cap)
        self._sp_len = 0
        self._refs = []

    def __dealloc__(self):
        if self._acts != NULL:
            free(self._acts)
        if self._strpool != NULL:
            free(self._strpool)

    cdef int _push_string(self, bytes s) except -1:
        """Add a string to the pool, return its offset."""
        cdef int slen = <int>len(s)
        cdef int needed = self._sp_len + slen + 1
        if needed > self._sp_cap:
            while self._sp_cap < needed:
                self._sp_cap *= 2
            self._strpool = <char*>realloc(self._strpool, self._sp_cap)
            if self._strpool == NULL:
                raise MemoryError()
        cdef int off = self._sp_len
        memcpy(self._strpool + off, PyBytes_AS_STRING(s), slen)
        self._strpool[off + slen] = 0
        self._sp_len = off + slen + 1
        self._refs.append(s)
        return off

    cdef void _grow_acts(self) noexcept:
        self._cap *= 2
        self._acts = <_Action*>realloc(self._acts, self._cap * sizeof(_Action))

    def clear(self):
        self._count = 0
        self._sp_len = 0
        self._refs.clear()

    def add_create_object(self, builder, scene, float w, float h, float mt, float ml):
        if not isinstance(builder, bytes):
            builder = builder.encode('utf-8') if isinstance(builder, str) else bytes(builder)
        if not isinstance(scene, bytes):
            scene = scene.encode('utf-8') if isinstance(scene, str) else bytes(scene)
        if self._count >= self._cap:
            self._grow_acts()
        cdef int idx = self._count
        self._acts[idx].type = ACT_CREATE
        self._acts[idx].s1 = self._push_string(builder)
        self._acts[idx].s2 = self._push_string(scene)
        self._acts[idx].f1 = w; self._acts[idx].f2 = h
        self._acts[idx].f3 = mt; self._acts[idx].f4 = ml
        self._count += 1

    def add_progress_value(self, uintptr_t obj_addr, float value):
        if self._count >= self._cap:
            self._grow_acts()
        cdef int idx = self._count
        self._acts[idx].type = ACT_PROGRESS_VALUE
        self._acts[idx].ptr = obj_addr
        self._acts[idx].f1 = value
        self._count += 1

    def add_progress_style(self, uintptr_t obj_addr, char fill, char empty,
                           unsigned char fr, unsigned char fg, unsigned char fb,
                           unsigned char er, unsigned char eg, unsigned char eb):
        if self._count >= self._cap:
            self._grow_acts()
        cdef int idx = self._count
        self._acts[idx].type = ACT_PROGRESS_STYLE
        self._acts[idx].ptr = obj_addr
        self._acts[idx].c1 = fill; self._acts[idx].c2 = empty
        self._acts[idx].uc1 = fr; self._acts[idx].uc2 = fg; self._acts[idx].uc3 = fb
        self._acts[idx].uc4 = er; self._acts[idx].uc5 = eg; self._acts[idx].uc6 = eb
        self._count += 1

    def add_init_scene(self, name):
        if not isinstance(name, bytes):
            name = name.encode('utf-8') if isinstance(name, str) else bytes(name)
        if self._count >= self._cap:
            self._grow_acts()
        cdef int idx = self._count
        self._acts[idx].type = ACT_INIT_SCENE
        self._acts[idx].s1 = self._push_string(name)
        self._count += 1

    def add_free_scene(self, name):
        if not isinstance(name, bytes):
            name = name.encode('utf-8') if isinstance(name, str) else bytes(name)
        if self._count >= self._cap:
            self._grow_acts()
        cdef int idx = self._count
        self._acts[idx].type = ACT_FREE_SCENE
        self._acts[idx].s1 = self._push_string(name)
        self._count += 1

    def add_clear_scene(self, name):
        if not isinstance(name, bytes):
            name = name.encode('utf-8') if isinstance(name, str) else bytes(name)
        if self._count >= self._cap:
            self._grow_acts()
        cdef int idx = self._count
        self._acts[idx].type = ACT_CLEAR_SCENE
        self._acts[idx].s1 = self._push_string(name)
        self._count += 1

    cpdef int execute_native(self):
        """Execute all accumulated actions in a tight nogil C loop."""
        cdef int n = self._count
        if n == 0:
            return 0
        cdef _Action* acts = self._acts
        cdef char* sp = self._strpool
        cdef int i, t
        cdef int processed = 0
        with nogil:
            for i in range(n):
                t = acts[i].type
                if t == ACT_CREATE:
                    c_tuix_create_object(sp + acts[i].s1, sp + acts[i].s2,
                                         acts[i].f1, acts[i].f2, acts[i].f3, acts[i].f4)
                elif t == ACT_PROGRESS_VALUE:
                    c_tuix_progressbar_set_value(<TuixObject*>acts[i].ptr, acts[i].f1)
                elif t == ACT_PROGRESS_STYLE:
                    c_tuix_progressbar_set_style(<TuixObject*>acts[i].ptr,
                                                  acts[i].c1, acts[i].c2,
                                                  acts[i].uc1, acts[i].uc2, acts[i].uc3,
                                                  acts[i].uc4, acts[i].uc5, acts[i].uc6)
                elif t == ACT_INIT_SCENE:
                    c_tuix_init_scene(sp + acts[i].s1)
                elif t == ACT_FREE_SCENE:
                    c_tuix_free_scene(sp + acts[i].s1)
                elif t == ACT_CLEAR_SCENE:
                    c_tuix_clear_scene(sp + acts[i].s1)
                processed += 1
        return processed

    cpdef int execute(self):
        """Serialize to command buffer and execute via the batch executor."""
        cdef bytes data = self.serialize()
        if not data:
            return 0
        cdef const unsigned char* p = <const unsigned char*>PyBytes_AS_STRING(data)
        cdef size_t length = <size_t>len(data)
        cdef int r
        with nogil:
            r = c_tuix_execute_command_buffer(p, length)
        return r

    cpdef bytes serialize(self):
        """Serialize accumulated actions into the compact binary command format."""
        cdef int n = self._count
        if n == 0:
            return b''
        cdef _Action* acts = self._acts
        cdef char* sp = self._strpool
        cdef int sp_len = self._sp_len

        # Compute total buffer size
        cdef size_t total = 1 + 4 + <size_t>sp_len  # string table header
        cdef int i, t
        for i in range(n):
            t = acts[i].type
            if t == ACT_CREATE:
                total += 1 + 4 + 24  # cmd + plen + 4+4+16
            elif t == ACT_INIT_SCENE or t == ACT_FREE_SCENE or t == ACT_CLEAR_SCENE:
                total += 1 + 4 + 4
            elif t == ACT_PROGRESS_VALUE:
                total += 1 + 4 + 8
            elif t == ACT_PROGRESS_STYLE:
                total += 1 + 4 + 12
            else:
                total += 5  # unknown: cmd + 0-length

        cdef bytearray pybuf = bytearray(total)
        cdef unsigned char* out = <unsigned char*>PyByteArray_AS_STRING(pybuf)
        cdef size_t off = 0
        cdef uint32_t plen, v32

        # String table (cmd=255)
        out[off] = 255; off += 1
        v32 = <uint32_t>sp_len
        memcpy(out + off, &v32, 4); off += 4
        memcpy(out + off, sp, sp_len); off += sp_len

        # Action records
        cdef uint32_t s_off
        cdef float fv
        for i in range(n):
            t = acts[i].type
            out[off] = <unsigned char>t; off += 1

            if t == ACT_CREATE:
                plen = 24
                memcpy(out + off, &plen, 4); off += 4
                s_off = <uint32_t>acts[i].s1
                memcpy(out + off, &s_off, 4); off += 4
                s_off = <uint32_t>acts[i].s2
                memcpy(out + off, &s_off, 4); off += 4
                fv = acts[i].f1; memcpy(out + off, &fv, 4); off += 4
                fv = acts[i].f2; memcpy(out + off, &fv, 4); off += 4
                fv = acts[i].f3; memcpy(out + off, &fv, 4); off += 4
                fv = acts[i].f4; memcpy(out + off, &fv, 4); off += 4
            elif t == ACT_INIT_SCENE or t == ACT_FREE_SCENE or t == ACT_CLEAR_SCENE:
                plen = 4
                memcpy(out + off, &plen, 4); off += 4
                s_off = <uint32_t>acts[i].s1
                memcpy(out + off, &s_off, 4); off += 4
            elif t == ACT_PROGRESS_VALUE:
                plen = 8
                memcpy(out + off, &plen, 4); off += 4
                v32 = <uint32_t>acts[i].ptr
                memcpy(out + off, &v32, 4); off += 4
                fv = acts[i].f1
                memcpy(out + off, &fv, 4); off += 4
            elif t == ACT_PROGRESS_STYLE:
                plen = 12
                memcpy(out + off, &plen, 4); off += 4
                v32 = <uint32_t>acts[i].ptr
                memcpy(out + off, &v32, 4); off += 4
                out[off] = <unsigned char>acts[i].c1; off += 1
                out[off] = <unsigned char>acts[i].c2; off += 1
                out[off] = acts[i].uc1; off += 1
                out[off] = acts[i].uc2; off += 1
                out[off] = acts[i].uc3; off += 1
                out[off] = acts[i].uc4; off += 1
                out[off] = acts[i].uc5; off += 1
                out[off] = acts[i].uc6; off += 1
            else:
                plen = 0
                memcpy(out + off, &plen, 4); off += 4

        return bytes(pybuf)

    cpdef int execute_serialized(self, bytes data):
        cdef const unsigned char* p = <const unsigned char*>PyBytes_AS_STRING(data)
        cdef size_t length = <size_t>len(data)
        cdef int r
        with nogil:
            r = c_tuix_execute_command_buffer(p, length)
        return r


# ═══════════════════════════════════════════════════════════════════
#  Engine helpers — terminal size, scene name, pipeline tick
# ═══════════════════════════════════════════════════════════════════

cpdef void set_terminal_size(int w, int h):
    """Set the engine's terminal dimensions (for geometry/compositor)."""
    tuix_registry.terminal_width = w
    tuix_registry.terminal_height = h
    tuix_registry.terminal_width_old = w
    tuix_registry.terminal_height_old = h

cpdef void set_current_scene_name(bytes name):
    """Set the engine's current scene name pointer."""
    tuix_registry.current_scene_name = name

cpdef int tuix_select_scene(bytes name):
    """Select the active scene by name (sets current_scene_name to the
    scene-registry's own interned copy and marks the scene active)."""
    cdef char* n = name
    cdef int r
    with nogil:
        r = c_tuix_select_scene(n)
    return r

cpdef int tuix_subcycle_init(bytes scene_name, uintptr_t obj_addr):
    cdef char* sn = scene_name
    cdef int r
    if obj_addr == 0:
        return -1
    with nogil:
        r = c_tuix_subcycle_init(sn, <TuixObject*>obj_addr)
    return r

cpdef void tuix_subcycle_free(bytes scene_name, int uid):
    cdef char* sn = scene_name
    with nogil:
        c_tuix_subcycle_free(sn, uid)

cpdef uintptr_t tuix_composite_scene(uintptr_t scene_addr):
    cdef TuixFinalBuffer* fb = NULL
    if scene_addr == 0:
        return 0
    with nogil:
        fb = c_tuix_composite_scene(<TuixScene*>scene_addr)
    return <uintptr_t>fb

cpdef void tuix_resolve_geometry(uintptr_t buf_addr):
    if buf_addr == 0:
        return
    with nogil:
        c_tuix_resolve_geometry(<TuixBuffer*>buf_addr)

cpdef tuple tuix_rgb16(int r, int g, int b):
    cdef TuixRGBTuple src
    cdef TuixRGBTuple out
    if r < 0:
        r = 0
    elif r > 255:
        r = 255
    if g < 0:
        g = 0
    elif g > 255:
        g = 255
    if b < 0:
        b = 0
    elif b > 255:
        b = 255
    src.r = <unsigned char>r
    src.g = <unsigned char>g
    src.b = <unsigned char>b
    with nogil:
        out = c_tuix_rgb16(src)
    return (out.r, out.g, out.b)

cpdef void pipeline_tick(bytes scene_name):
    """Run one full engine pipeline iteration entirely in C:
    subcycles → geometry → build_content → composite → render.
    Mirrors the C benchmark's pipeline_tick()."""
    cdef const char* sn = scene_name
    cdef TuixScene* scene
    cdef TuixSceneSubcycles* ss
    cdef TuixSubcycle* sc
    cdef TuixHandlerResponse ans
    cdef TuixBuffer* buf
    cdef TuixFinalBuffer* fb
    cdef TuixPixel* new_px
    cdef int i, j

    with nogil:
        # 1. Subcycle handlers
        for i in range(tuix_registry.subcycles.count):
            ss = tuix_registry.subcycles.subcycles[i]
            if strcmp(ss.scene_name, sn) != 0:
                continue
            for j in range(ss.count):
                sc = ss.subcycles[j]
                if sc.handler != NULL:
                    ans = sc.handler(sc.obj)
                    buf = c_tuix_get_buffer_by_uid(sc.obj.uid)
                    if buf != NULL:
                        buf.required_redraw = buf.required_redraw | ans.requires_redraw
            break

        # 2. Geometry + build_content
        scene = c_tuix_get_scene(sn)
        if scene == NULL:
            return
        for i in range(scene.count):
            buf = scene.buffers[i]
            if buf == NULL or buf.obj == NULL or buf.obj.builder == NULL:
                continue
            if buf.obj.builder.build_content == NULL:
                continue
            if buf.required_redraw == 0:
                continue
            c_tuix_resolve_geometry(buf)
            new_px = buf.obj.builder.build_content(buf.obj, buf)
            if new_px == NULL:
                continue
            free(buf.pixels)
            buf.pixels = new_px
            buf.required_redraw = 0

        # 3. Composite
        fb = c_tuix_composite_scene(scene)
        if fb == NULL:
            return

        # 4. Render
        c_tuix_render_streaming(fb, NULL, NULL)

cpdef void force_scene_redraw(bytes scene_name):
    """Mark all buffers in a scene for redraw."""
    cdef const char* sn = scene_name
    cdef TuixScene* scene
    cdef int i
    with nogil:
        scene = c_tuix_get_scene(sn)
        if scene != NULL:
            for i in range(scene.count):
                if scene.buffers[i] != NULL:
                    scene.buffers[i].required_redraw = 1


# ═══════════════════════════════════════════════════════════════════
#  NativeBuffer — zero-copy C-level buffer for benchmarking
# ═══════════════════════════════════════════════════════════════════

cdef class NativeBuffer:
    """Owns a C TuixFinalBuffer. All fills and renders at C speed, no Python per-pixel ops."""
    cdef TuixFinalBuffer fb

    def __cinit__(self, int w, int h):
        cdef size_t n = <size_t>w * h
        cdef size_t i
        self.fb.width = w
        self.fb.height = h
        self.fb.full_redraw = 1
        self.fb.pixels = <TuixPixel*>malloc(n * sizeof(TuixPixel))
        if self.fb.pixels == NULL and n > 0:
            raise MemoryError()
        memset(self.fb.pixels, 0, n * sizeof(TuixPixel))
        # Match C alloc_buf: default every pixel to space
        for i in range(n):
            self.fb.pixels[i].sym[0] = b' '
            self.fb.pixels[i].sym[1] = 0

    def __dealloc__(self):
        if self.fb.pixels != NULL:
            free(self.fb.pixels)

    @property
    def width(self):
        return self.fb.width

    @property
    def height(self):
        return self.fb.height

    @property
    def full_redraw(self):
        return self.fb.full_redraw

    @full_redraw.setter
    def full_redraw(self, int v):
        self.fb.full_redraw = v

    cpdef void render(self):
        """Call C tuix_render_streaming directly on the native buffer."""
        with nogil:
            c_tuix_render_streaming(&self.fb, NULL, NULL)

    cpdef bytes render_to_bytes(self):
        """Render and return ANSI escape bytes."""
        with nogil:
            c_tuix_render_streaming(&self.fb, NULL, NULL)
        return _render_ansi_from_pixels(self.fb.pixels, self.fb.width, self.fb.height)

    # ── C-level fills — match C benchmark exactly, all run nogil ──

    cpdef void fill_noop(self):
        pass

    cpdef void fill_random(self, int seed):
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef unsigned int s = <unsigned int>seed
        cdef int i, n = W * H
        with nogil:
            for i in range(n):
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[i].sym[0] = <char>(32 + ((s >> 16) % 95))
                pix[i].sym[1] = 0
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[i].styles.fg.r = <unsigned char>((s >> 16) & 0xFF)
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[i].styles.fg.g = <unsigned char>((s >> 16) & 0xFF)
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[i].styles.fg.b = <unsigned char>((s >> 16) & 0xFF)
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[i].styles.bg.r = <unsigned char>((s >> 16) & 0xFF)
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[i].styles.bg.g = <unsigned char>((s >> 16) & 0xFF)
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[i].styles.bg.b = <unsigned char>((s >> 16) & 0xFF)
                pix[i].styles.custom_fg = 1
                pix[i].styles.custom_bg = 1
                pix[i].q_cached = 0

    cpdef void fill_gradient(self, int off):
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef int x, y, v, idx
        with nogil:
            for y in range(H):
                for x in range(W):
                    idx = y * W + x
                    v = (x + y + off) & 0xFF
                    pix[idx].sym[0] = <char>(32 + ((x + y + off) % 95))
                    pix[idx].sym[1] = 0
                    pix[idx].styles.fg.r = <unsigned char>v
                    pix[idx].styles.fg.g = <unsigned char>(255 - v)
                    pix[idx].styles.fg.b = 128
                    pix[idx].styles.bg.r = <unsigned char>(255 - v)
                    pix[idx].styles.bg.g = <unsigned char>v
                    pix[idx].styles.bg.b = 128
                    pix[idx].styles.custom_fg = 1
                    pix[idx].styles.custom_bg = 1
                    pix[idx].q_cached = 0

    cpdef void fill_content_update(self, int frame):
        """~25% pixel update (skip 75%) with random bg — matches C fill_content_update."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef unsigned int s = <unsigned int>frame * 7919
        cdef int x, y, idx
        with nogil:
            for y in range(H):
                for x in range(W):
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    if ((s >> 16) & 3) != 0:
                        continue
                    idx = y * W + x
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].sym[0] = <char>(32 + ((s >> 16) % 95))
                    pix[idx].sym[1] = 0
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.bg.r = <unsigned char>((s >> 16) & 0xFF)
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.bg.g = <unsigned char>((s >> 16) & 0xFF)
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.bg.b = <unsigned char>((s >> 16) & 0xFF)
                    pix[idx].styles.fg.r = 255
                    pix[idx].styles.fg.g = 255
                    pix[idx].styles.fg.b = 255
                    pix[idx].styles.custom_fg = 1
                    pix[idx].styles.custom_bg = 1
                    pix[idx].q_cached = 0

    cpdef void fill_tree(self, int items, int frame):
        """Fill items rows of colored text — matches C w_fill_tree."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef unsigned int s = <unsigned int>frame
        cdef int i, x, idx
        with nogil:
            for i in range(items):
                if i >= H:
                    break
                for x in range(W):
                    idx = i * W + x
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].sym[0] = <char>(65 + ((s >> 16) % 26))
                    pix[idx].sym[1] = 0
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.fg.r = <unsigned char>((s >> 16) & 0xFF)
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.fg.g = <unsigned char>((s >> 16) & 0xFF)
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.fg.b = <unsigned char>((s >> 16) & 0xFF)
                    pix[idx].styles.bg.r = 0
                    pix[idx].styles.bg.g = 0
                    pix[idx].styles.bg.b = 0
                    pix[idx].styles.custom_fg = 1
                    pix[idx].styles.custom_bg = 0
                    pix[idx].q_cached = 0

    cpdef void fill_layout_grid(self, int rows, int cols, int frame):
        """Dense coloured grid — matches C fill_layout_grid."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef int x, y, idx, rh, cw, ry, cx, hue
        with nogil:
            rh = H // rows
            if rh < 1:
                rh = 1
            cw = W // cols
            if cw < 1:
                cw = 1
            for y in range(H):
                for x in range(W):
                    idx = y * W + x
                    ry = y // rh
                    cx = x // cw
                    hue = ((ry * cols + cx) * 37 + frame * 13) & 0xFF
                    pix[idx].sym[0] = <char>(65 + ((ry + cx + frame) % 26))
                    pix[idx].sym[1] = 0
                    pix[idx].styles.fg.r = <unsigned char>hue
                    pix[idx].styles.fg.g = <unsigned char>(255 - hue)
                    pix[idx].styles.fg.b = 128
                    pix[idx].styles.bg.r = 30
                    pix[idx].styles.bg.g = 30
                    pix[idx].styles.bg.b = 30
                    pix[idx].styles.custom_fg = 1
                    pix[idx].styles.custom_bg = 1
                    pix[idx].q_cached = 0

    cpdef void fill_scroll(self, int frame):
        """Shift rows up by 1, fill bottom row — matches C fill_scroll."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef unsigned int s = <unsigned int>frame * 9973
        cdef int x, base
        cdef size_t i, n
        with nogil:
            if H > 1:
                memmove(pix, pix + W, sizeof(TuixPixel) * W * (H - 1))
            base = (H - 1) * W
            for x in range(W):
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[base + x].sym[0] = <char>(32 + ((s >> 16) % 95))
                pix[base + x].sym[1] = 0
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[base + x].styles.fg.r = 200
                pix[base + x].styles.fg.g = 200
                pix[base + x].styles.fg.b = 200
                pix[base + x].styles.bg.r = <unsigned char>((s >> 16) & 0xFF)
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[base + x].styles.bg.g = <unsigned char>((s >> 16) & 0xFF)
                s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                pix[base + x].styles.bg.b = <unsigned char>((s >> 16) & 0xFF)
                pix[base + x].styles.custom_fg = 1
                pix[base + x].styles.custom_bg = 1
                pix[base + x].q_cached = 0
            n = <size_t>H * W
            for i in range(n):
                pix[i].q_cached = 0

    cpdef void fill_virtual_list(self, int frame, int total):
        """Virtual-list viewport with row labels — matches C fill_virtual_list."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef int x, y, idx, item, llen, bg_v, offset, denom
        cdef char label[128]
        with nogil:
            denom = total - H
            if denom <= 0:
                denom = 1
            offset = (frame * 3) % denom
            for y in range(H):
                item = offset + y
                llen = snprintf(label, 128, " Row %d ", item)
                for x in range(W):
                    idx = y * W + x
                    if x < llen:
                        pix[idx].sym[0] = label[x]
                    else:
                        pix[idx].sym[0] = b' '
                    pix[idx].sym[1] = 0
                    bg_v = 40 if (item & 1) else 50
                    pix[idx].styles.fg.r = 200
                    pix[idx].styles.fg.g = 200
                    pix[idx].styles.fg.b = 200
                    pix[idx].styles.bg.r = <unsigned char>bg_v
                    pix[idx].styles.bg.g = <unsigned char>bg_v
                    pix[idx].styles.bg.b = <unsigned char>bg_v
                    pix[idx].styles.custom_fg = 1
                    pix[idx].styles.custom_bg = 1
                    pix[idx].q_cached = 0

    cpdef void fill_table(self, int cols, int frame):
        """Table with borders — matches C fill_table."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef int x, y, idx, cw, is_border, col_idx, hue
        with nogil:
            cw = W // cols
            if cw < 1:
                cw = 1
            for y in range(H):
                for x in range(W):
                    idx = y * W + x
                    is_border = 1 if (y == 0 or (x % cw == 0)) else 0
                    if is_border:
                        if y == 0 and x % cw == 0:
                            pix[idx].sym[0] = b'+'
                        elif x % cw == 0:
                            pix[idx].sym[0] = b'|'
                        else:
                            pix[idx].sym[0] = b'-'
                        pix[idx].styles.fg.r = 200
                        pix[idx].styles.fg.g = 200
                        pix[idx].styles.fg.b = 200
                        pix[idx].styles.bg.r = 20
                        pix[idx].styles.bg.g = 20
                        pix[idx].styles.bg.b = 20
                    else:
                        col_idx = x // cw
                        pix[idx].sym[0] = <char>(65 + ((col_idx + y + frame) % 26))
                        hue = (col_idx * 23 + y * 7 + frame) & 0xFF
                        pix[idx].styles.fg.r = <unsigned char>hue
                        pix[idx].styles.fg.g = <unsigned char>(255 - hue)
                        pix[idx].styles.fg.b = 128
                        pix[idx].styles.bg.r = 30
                        pix[idx].styles.bg.g = 30
                        pix[idx].styles.bg.b = 30
                    pix[idx].sym[1] = 0
                    pix[idx].styles.custom_fg = 1
                    pix[idx].styles.custom_bg = 1
                    pix[idx].q_cached = 0

    cpdef void fill_dirty_rows(self, int n_dirty, int frame):
        """Change only n_dirty rows — matches C fill_dirty_rows."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef unsigned int s = <unsigned int>frame * 6151
        cdef int d, x, y, idx
        with nogil:
            for d in range(n_dirty):
                if d >= H:
                    break
                y = (frame + d * 3) % H
                for x in range(W):
                    idx = y * W + x
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].sym[0] = <char>(32 + ((s >> 16) % 95))
                    pix[idx].sym[1] = 0
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.bg.r = <unsigned char>((s >> 16) & 0xFF)
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.bg.g = <unsigned char>((s >> 16) & 0xFF)
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.bg.b = <unsigned char>((s >> 16) & 0xFF)
                    pix[idx].styles.fg.r = 255
                    pix[idx].styles.fg.g = 255
                    pix[idx].styles.fg.b = 255
                    pix[idx].styles.custom_fg = 1
                    pix[idx].styles.custom_bg = 1
                    pix[idx].q_cached = 0

    cpdef void fill_dashboard(self, int services, int frame):
        """Structured dashboard UI — matches C fill_dashboard."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef int x, y, idx, rps, s_idx, dy, hue, bg_v
        cdef char* hdr = " TUIX Service Dashboard "
        cdef int hlen = 24
        with nogil:
            # header row
            for x in range(W):
                if x < hlen:
                    pix[x].sym[0] = hdr[x]
                else:
                    pix[x].sym[0] = b' '
                pix[x].sym[1] = 0
                pix[x].styles.fg.r = 255
                pix[x].styles.fg.g = 255
                pix[x].styles.fg.b = 255
                pix[x].styles.bg.r = 30
                pix[x].styles.bg.g = 80
                pix[x].styles.bg.b = 160
                pix[x].styles.custom_fg = 1
                pix[x].styles.custom_bg = 1
                pix[x].q_cached = 0
            # service rows
            rps = (H - 2) // services
            if rps < 1:
                rps = 1
            for s_idx in range(services):
                for dy in range(rps):
                    y = 1 + s_idx * rps + dy
                    if y >= H - 1:
                        break
                    for x in range(W):
                        idx = y * W + x
                        hue = ((s_idx * 37 + frame * 3) & 0xFF)
                        bg_v = 30 if (s_idx & 1) else 40
                        if x < 10:
                            pix[idx].sym[0] = <char>(65 + (s_idx % 26))
                        else:
                            pix[idx].sym[0] = <char>(48 + ((s_idx + x + frame) % 10))
                        pix[idx].sym[1] = 0
                        pix[idx].styles.fg.r = <unsigned char>hue
                        pix[idx].styles.fg.g = <unsigned char>(255 - hue)
                        pix[idx].styles.fg.b = 128
                        pix[idx].styles.bg.r = <unsigned char>bg_v
                        pix[idx].styles.bg.g = <unsigned char>bg_v
                        pix[idx].styles.bg.b = <unsigned char>bg_v
                        pix[idx].styles.custom_fg = 1
                        pix[idx].styles.custom_bg = 1
                        pix[idx].q_cached = 0
            # status bar
            if H > 1:
                for x in range(W):
                    idx = (H - 1) * W + x
                    pix[idx].sym[0] = b' '
                    pix[idx].sym[1] = 0
                    pix[idx].styles.fg.r = 200
                    pix[idx].styles.fg.g = 200
                    pix[idx].styles.fg.b = 200
                    pix[idx].styles.bg.r = 20
                    pix[idx].styles.bg.g = 60
                    pix[idx].styles.bg.b = 120
                    pix[idx].styles.custom_fg = 1
                    pix[idx].styles.custom_bg = 1
                    pix[idx].q_cached = 0

    cpdef void fill_fps_region(self, int frame, int services):
        """Update only top-left 12 x services area — matches C fill_fps_region."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef unsigned int s = <unsigned int>frame * 8111
        cdef int rw = 12, rh = services
        cdef int x, y, idx
        with nogil:
            if rw > W:
                rw = W
            if rh > H:
                rh = H
            for y in range(rh):
                for x in range(rw):
                    idx = y * W + x
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].sym[0] = <char>(48 + ((s >> 16) % 10))
                    pix[idx].sym[1] = 0
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.fg.r = <unsigned char>((s >> 16) & 0xFF)
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.fg.g = <unsigned char>((s >> 16) & 0xFF)
                    s = (s * 1103515245 + 12345) & 0x7FFFFFFF
                    pix[idx].styles.fg.b = <unsigned char>((s >> 16) & 0xFF)
                    pix[idx].styles.bg.r = 0
                    pix[idx].styles.bg.g = 0
                    pix[idx].styles.bg.b = 0
                    pix[idx].styles.custom_fg = 1
                    pix[idx].styles.custom_bg = 0
                    pix[idx].q_cached = 0

    cpdef void fill_input_cell(self, int frame):
        """Update single cell — matches C fill_input_cell."""
        cdef int W = self.fb.width, H = self.fb.height
        cdef TuixPixel* pix = self.fb.pixels
        cdef int x, y, idx
        with nogil:
            x = frame % W
            y = (frame // W) % H
            idx = y * W + x
            pix[idx].sym[0] = b'_'
            pix[idx].sym[1] = 0
            pix[idx].styles.fg.r = 255
            pix[idx].styles.fg.g = 255
            pix[idx].styles.fg.b = 255
            pix[idx].styles.bg.r = 60
            pix[idx].styles.bg.g = 60
            pix[idx].styles.bg.b = 60
            pix[idx].styles.custom_fg = 1
            pix[idx].styles.custom_bg = 1
            pix[idx].q_cached = 0

    cpdef void fill_transition(self, int frame):
        """Alternate between fill_random and fill_gradient — matches C fill_transition."""
        if frame & 1:
            self.fill_gradient(frame)
        else:
            self.fill_random(frame)

    cpdef void fill_dash_nav(self, int services, int dwell, int frame):
        """Dashboard + navigation: every dwell frames, full rebuild — matches C w_fill_dash_nav."""
        if frame % dwell == 0:
            self.fill_random(frame)
        self.fill_dashboard(services, frame)



# ═══════════════════════════════════════════════════════════════════
#  Legacy compatibility shim
# ═══════════════════════════════════════════════════════════════════

def tuix_inject_key(key, is_down):
    raise NotImplementedError()

