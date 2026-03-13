from . import _lib
import ctypes


def create_object(builder_name: bytes, scene_name: bytes, width_mod: float, height_mod: float, margin_top_mod: float, margin_left_mod: float) -> int:
    # Prefer Cython-exposed wrapper for lower overhead
    try:
        return getattr(_lib._mod, 'tuix_create_object')(builder_name, scene_name, width_mod, height_mod, margin_top_mod, margin_left_mod)
    except Exception:
        fn = _lib.get_func('tuix_create_object', restype=ctypes.c_int, argtypes=[ctypes.c_char_p, ctypes.c_char_p, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float])
        return fn(builder_name, scene_name, width_mod, height_mod, margin_top_mod, margin_left_mod)


def _obj_addr(obj_ptr):
    """Return the integer address of a TuixObject (ctypes pointer, struct instance, or int)."""
    if isinstance(obj_ptr, int):
        return obj_ptr
    try:
        return ctypes.addressof(obj_ptr.contents)  # ctypes POINTER(TuixObject)
    except Exception:
        pass
    try:
        return ctypes.addressof(obj_ptr)  # ctypes struct instance
    except Exception:
        return int(obj_ptr)


# ── Progressbar builder ───────────────────────────────────────────

def tuix_progressbar_set_value(obj_ptr, value: float):
    return _lib._mod.tuix_progressbar_set_value(_obj_addr(obj_ptr), float(value))

def tuix_progressbar_set_style(obj_ptr, fill, empty, fr, fg, fb, er, eg, eb):
    return _lib._mod.tuix_progressbar_set_style(_obj_addr(obj_ptr), fill, empty, fr, fg, fb, er, eg, eb)


# ── Choice builder ────────────────────────────────────────────────

def tuix_choice_set_options(obj_ptr, labels):
    return _lib._mod.tuix_choice_set_options(_obj_addr(obj_ptr), list(labels))

def tuix_choice_feed_input(obj_ptr, snap):
    return _lib._mod.tuix_choice_feed_input(_obj_addr(obj_ptr), snap)

def tuix_choice_is_confirmed(obj_ptr):
    return _lib._mod.tuix_choice_is_confirmed(_obj_addr(obj_ptr))

def tuix_choice_get_result(obj_ptr):
    return _lib._mod.tuix_choice_get_result(_obj_addr(obj_ptr))

def tuix_choice_get_selected(obj_ptr):
    return _lib._mod.tuix_choice_get_selected(_obj_addr(obj_ptr))

def tuix_choice_reset(obj_ptr):
    _lib._mod.tuix_choice_reset(_obj_addr(obj_ptr))


# ── Input builder ─────────────────────────────────────────────────

def tuix_input_set_placeholder(obj_ptr, text):
    return _lib._mod.tuix_input_set_placeholder(_obj_addr(obj_ptr), text)

def tuix_input_feed_input(obj_ptr, snap):
    return _lib._mod.tuix_input_feed_input(_obj_addr(obj_ptr), snap)

def tuix_input_is_submitted(obj_ptr):
    return _lib._mod.tuix_input_is_submitted(_obj_addr(obj_ptr))

def tuix_input_get_result(obj_ptr):
    return _lib._mod.tuix_input_get_result(_obj_addr(obj_ptr))

def tuix_input_get_text(obj_ptr):
    return _lib._mod.tuix_input_get_text(_obj_addr(obj_ptr))

def tuix_input_reset(obj_ptr):
    _lib._mod.tuix_input_reset(_obj_addr(obj_ptr))


# ── Canvas builder ────────────────────────────────────────────────

def _to_char(sym):
    """Convert a length-1 bytes object to its integer value for Cython char params."""
    if isinstance(sym, (bytes, bytearray)):
        return sym[0]
    return sym

def tuix_canvas_set_pixel(obj_ptr, x, y, sym, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_set_pixel(_obj_addr(obj_ptr), x, y, _to_char(sym), fgr, fgg, fgb, bgr, bgg, bgb)

def tuix_canvas_draw_line(obj_ptr, x0, y0, x1, y1, sym, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_draw_line(_obj_addr(obj_ptr), x0, y0, x1, y1, _to_char(sym), fgr, fgg, fgb, bgr, bgg, bgb)

def tuix_canvas_draw_rect(obj_ptr, x, y, w, h, sym, filled, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_draw_rect(_obj_addr(obj_ptr), x, y, w, h, _to_char(sym), filled, fgr, fgg, fgb, bgr, bgg, bgb)

def tuix_canvas_draw_circle(obj_ptr, cx, cy, radius, sym, filled, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_draw_circle(_obj_addr(obj_ptr), cx, cy, radius, _to_char(sym), filled, fgr, fgg, fgb, bgr, bgg, bgb)

def tuix_canvas_draw_text(obj_ptr, x, y, text, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_draw_text(_obj_addr(obj_ptr), x, y, text, fgr, fgg, fgb, bgr, bgg, bgb)

def tuix_canvas_draw_sprite(obj_ptr, dst_x, dst_y, sprite_w, sprite_h, sprite_ptr):
    return _lib._mod.tuix_canvas_draw_sprite(_obj_addr(obj_ptr), dst_x, dst_y, sprite_w, sprite_h, sprite_ptr)

def tuix_canvas_insert_buffer(obj_ptr, pixels_ptr, size):
    return _lib._mod.tuix_canvas_insert_buffer(_obj_addr(obj_ptr), pixels_ptr, size)

def tuix_canvas_cache_sprite(obj_ptr, sprite_w, sprite_h, sprite_ptr):
    return _lib._mod.tuix_canvas_cache_sprite(_obj_addr(obj_ptr), sprite_w, sprite_h, sprite_ptr)

def tuix_canvas_free_cached_sprite(obj_ptr, sprite_id):
    _lib._mod.tuix_canvas_free_cached_sprite(_obj_addr(obj_ptr), sprite_id)

def tuix_canvas_draw_cached_sprite(obj_ptr, sprite_id, dst_x, dst_y):
    return _lib._mod.tuix_canvas_draw_cached_sprite(_obj_addr(obj_ptr), sprite_id, dst_x, dst_y)
