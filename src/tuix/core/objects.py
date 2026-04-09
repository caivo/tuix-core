from . import _lib
from . import _structs
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


def get_object_by_uid(uid: int):
    """Return a ctypes TuixObject for a UID, or None if not found."""
    try:
        from . import _tuix_cy
        obj_addr = _tuix_cy._tuix_get_object_addr_by_uid(uid)
    except (ImportError, AttributeError):
        return None

    if not obj_addr:
        return None

    obj_ptr = ctypes.cast(ctypes.c_void_p(obj_addr), ctypes.POINTER(_structs.TuixObject))
    return obj_ptr.contents


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


# ── Text builder ──────────────────────────────────────────────────

def tuix_text_set_text(obj_ptr, text):
    return _lib._mod.tuix_text_set_text(_obj_addr(obj_ptr), text)

def tuix_text_set_fg(obj_ptr, r, g, b):
    return _lib._mod.tuix_text_set_fg(_obj_addr(obj_ptr), r, g, b)

def tuix_text_set_bg(obj_ptr, r, g, b):
    return _lib._mod.tuix_text_set_bg(_obj_addr(obj_ptr), r, g, b)


# ── Box builder ───────────────────────────────────────────────────

def tuix_box_set_border(obj_ptr, enabled):
    return _lib._mod.tuix_box_set_border(_obj_addr(obj_ptr), enabled)

def tuix_box_set_title(obj_ptr, title):
    return _lib._mod.tuix_box_set_title(_obj_addr(obj_ptr), title)

def tuix_box_set_color(obj_ptr, r, g, b):
    return _lib._mod.tuix_box_set_color(_obj_addr(obj_ptr), r, g, b)


# ── Divider builder ───────────────────────────────────────────────

def tuix_divider_set_orientation(obj_ptr, vertical):
    return _lib._mod.tuix_divider_set_orientation(_obj_addr(obj_ptr), vertical)

def tuix_divider_set_symbol(obj_ptr, sym):
    return _lib._mod.tuix_divider_set_symbol(_obj_addr(obj_ptr), _to_char(sym))

def tuix_divider_set_color(obj_ptr, r, g, b):
    return _lib._mod.tuix_divider_set_color(_obj_addr(obj_ptr), r, g, b)


# ── Badge builder ─────────────────────────────────────────────────

def tuix_badge_set_text(obj_ptr, text):
    return _lib._mod.tuix_badge_set_text(_obj_addr(obj_ptr), text)

def tuix_badge_set_colors(obj_ptr, fr, fg, fb, br, bg, bb):
    return _lib._mod.tuix_badge_set_colors(_obj_addr(obj_ptr), fr, fg, fb, br, bg, bb)


# ── Button builder ────────────────────────────────────────────────

def tuix_button_set_label(obj_ptr, label):
    return _lib._mod.tuix_button_set_label(_obj_addr(obj_ptr), label)

def tuix_button_feed_input(obj_ptr, snap):
    return _lib._mod.tuix_button_feed_input(_obj_addr(obj_ptr), snap)

def tuix_button_is_pressed(obj_ptr):
    return _lib._mod.tuix_button_is_pressed(_obj_addr(obj_ptr))

def tuix_button_get_state(obj_ptr):
    return _lib._mod.tuix_button_get_state(_obj_addr(obj_ptr))


# ── Icon builder ──────────────────────────────────────────────────

def tuix_icon_set_symbol(obj_ptr, sym):
    if isinstance(sym, int):
        sym = bytes([sym & 0xFF])
    return _lib._mod.tuix_icon_set_symbol(_obj_addr(obj_ptr), sym)

def tuix_icon_set_colors(obj_ptr, fr, fg, fb, br, bg, bb):
    return _lib._mod.tuix_icon_set_colors(_obj_addr(obj_ptr), fr, fg, fb, br, bg, bb)


# ── Tag builder ───────────────────────────────────────────────────

def tuix_tag_set_text(obj_ptr, text):
    return _lib._mod.tuix_tag_set_text(_obj_addr(obj_ptr), text)

def tuix_tag_set_brackets(obj_ptr, left, right):
    return _lib._mod.tuix_tag_set_brackets(_obj_addr(obj_ptr), _to_char(left), _to_char(right))

def tuix_tag_set_colors(obj_ptr, fr, fg, fb, br, bg, bb):
    return _lib._mod.tuix_tag_set_colors(_obj_addr(obj_ptr), fr, fg, fb, br, bg, bb)


# ── Status builder ────────────────────────────────────────────────

def tuix_status_set_status(obj_ptr, status):
    return _lib._mod.tuix_status_set_status(_obj_addr(obj_ptr), status)


# ── Menu builder ──────────────────────────────────────────────────

def tuix_menu_set_title(obj_ptr, title):
    return _lib._mod.tuix_menu_set_title(_obj_addr(obj_ptr), title)

def tuix_menu_set_options(obj_ptr, labels):
    return _lib._mod.tuix_menu_set_options(_obj_addr(obj_ptr), list(labels))

def tuix_menu_feed_input(obj_ptr, snap):
    return _lib._mod.tuix_menu_feed_input(_obj_addr(obj_ptr), snap)

def tuix_menu_get_selected(obj_ptr):
    return _lib._mod.tuix_menu_get_selected(_obj_addr(obj_ptr))

def tuix_menu_is_activated(obj_ptr):
    return _lib._mod.tuix_menu_is_activated(_obj_addr(obj_ptr))


# ── ScrollContainer builder ───────────────────────────────────────

def tuix_scroll_container_set_title(obj_ptr, title):
    return _lib._mod.tuix_scroll_container_set_title(_obj_addr(obj_ptr), title)

def tuix_scroll_container_set_content_size(obj_ptr, width, height):
    return _lib._mod.tuix_scroll_container_set_content_size(_obj_addr(obj_ptr), width, height)

def tuix_scroll_container_set_offset(obj_ptr, offset_x, offset_y):
    return _lib._mod.tuix_scroll_container_set_offset(_obj_addr(obj_ptr), offset_x, offset_y)

def tuix_scroll_container_get_offset_x(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_offset_x(_obj_addr(obj_ptr))

def tuix_scroll_container_get_offset_y(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_offset_y(_obj_addr(obj_ptr))

def tuix_scroll_container_feed_input(obj_ptr, snap):
    return _lib._mod.tuix_scroll_container_feed_input(_obj_addr(obj_ptr), snap)

def tuix_scroll_container_get_scroll_pos(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_scroll_pos(_obj_addr(obj_ptr))


def get_object_snapshot_by_uid(uid: int) -> dict:
    """Get a snapshot (read-only copy) of an object by UID."""
    try:
        # Try Cython wrapper first
        from . import _tuix_cy
        return _tuix_cy.tuix_get_object_snapshot_by_uid(uid)
    except (ImportError, AttributeError):
        # Fallback to ctypes - not easily doable for struct returns, return None
        return None

