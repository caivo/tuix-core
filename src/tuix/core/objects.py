from . import _lib
from . import _structs
import ctypes


def _to_bytes(value):
    if isinstance(value, str):
        return value.encode("utf-8")
    return value


def _to_bytes_list(values):
    return [_to_bytes(v) for v in values]


def _obj_addr(obj_ptr):
    if isinstance(obj_ptr, int):
        return obj_ptr
    try:
        return ctypes.addressof(obj_ptr.contents)
    except Exception:
        pass
    try:
        return ctypes.addressof(obj_ptr)
    except Exception:
        return int(obj_ptr)


def create_object(
    builder_name: bytes,
    scene_name: bytes,
    width_mod: float,
    height_mod: float,
    margin_top_mod: float,
    margin_left_mod: float,
) -> int:
    builder_name = _to_bytes(builder_name)
    scene_name = _to_bytes(scene_name)
    try:
        return getattr(_lib._mod, "tuix_create_object")(
            builder_name,
            scene_name,
            width_mod,
            height_mod,
            margin_top_mod,
            margin_left_mod,
        )
    except Exception:
        fn = _lib.get_func(
            "tuix_create_object",
            restype=ctypes.c_int,
            argtypes=[
                ctypes.c_char_p,
                ctypes.c_char_p,
                ctypes.c_float,
                ctypes.c_float,
                ctypes.c_float,
                ctypes.c_float,
            ],
        )
        return fn(builder_name, scene_name, width_mod, height_mod, margin_top_mod, margin_left_mod)


def get_object_by_uid(uid: int):
    try:
        from . import _tuix_cy
        obj_addr = _tuix_cy._tuix_get_object_addr_by_uid(uid)
    except (ImportError, AttributeError):
        return None

    if not obj_addr:
        return None

    obj_ptr = ctypes.cast(ctypes.c_void_p(obj_addr), ctypes.POINTER(_structs.TuixObject))
    return obj_ptr.contents


def get_object_snapshot_by_uid(uid: int) -> dict:
    try:
        from . import _tuix_cy
        return _tuix_cy.tuix_get_object_snapshot_by_uid(uid)
    except (ImportError, AttributeError):
        return None


def _to_char(sym):
    if isinstance(sym, str):
        sym = sym.encode("utf-8")
    if isinstance(sym, (bytes, bytearray)):
        return sym[0]
    return sym


def tuix_progressbar_set_value(obj_ptr, value: float):
    return _lib._mod.tuix_progressbar_set_value(_obj_addr(obj_ptr), float(value))


def tuix_progressbar_get_value(obj_ptr):
    return _lib._mod.tuix_progressbar_get_value(_obj_addr(obj_ptr))


def tuix_progressbar_set_style(obj_ptr, fill, empty, fr, fg, fb, er, eg, eb):
    return _lib._mod.tuix_progressbar_set_style(_obj_addr(obj_ptr), _to_char(fill), _to_char(empty), fr, fg, fb, er, eg, eb)


def tuix_progressbar_show_percentage(obj_ptr, show=True):
    return _lib._mod.tuix_progressbar_show_percentage(_obj_addr(obj_ptr), 1 if show else 0)


def tuix_choice_set_options(obj_ptr, labels):
    return _lib._mod.tuix_choice_set_options(_obj_addr(obj_ptr), _to_bytes_list(labels))


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


def tuix_input_set_placeholder(obj_ptr, text):
    return _lib._mod.tuix_input_set_placeholder(_obj_addr(obj_ptr), _to_bytes(text))


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


def tuix_canvas_set_pixel(obj_ptr, x, y, sym, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_set_pixel(_obj_addr(obj_ptr), x, y, _to_char(sym), fgr, fgg, fgb, bgr, bgg, bgb)


def tuix_canvas_draw_line(obj_ptr, x0, y0, x1, y1, sym, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_draw_line(_obj_addr(obj_ptr), x0, y0, x1, y1, _to_char(sym), fgr, fgg, fgb, bgr, bgg, bgb)


def tuix_canvas_draw_rect(obj_ptr, x, y, w, h, sym, filled, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_draw_rect(_obj_addr(obj_ptr), x, y, w, h, _to_char(sym), filled, fgr, fgg, fgb, bgr, bgg, bgb)


def tuix_canvas_draw_circle(obj_ptr, cx, cy, radius, sym, filled, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_draw_circle(_obj_addr(obj_ptr), cx, cy, radius, _to_char(sym), filled, fgr, fgg, fgb, bgr, bgg, bgb)


def tuix_canvas_draw_text(obj_ptr, x, y, text, fgr, fgg, fgb, bgr, bgg, bgb):
    return _lib._mod.tuix_canvas_draw_text(_obj_addr(obj_ptr), x, y, _to_bytes(text), fgr, fgg, fgb, bgr, bgg, bgb)


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


def tuix_text_set_text(obj_ptr, text):
    return _lib._mod.tuix_text_set_text(_obj_addr(obj_ptr), _to_bytes(text))


def tuix_text_set_fg(obj_ptr, r, g, b):
    return _lib._mod.tuix_text_set_fg(_obj_addr(obj_ptr), r, g, b)


def tuix_text_set_bg(obj_ptr, r, g, b):
    return _lib._mod.tuix_text_set_bg(_obj_addr(obj_ptr), r, g, b)


def tuix_text_clear_bg(obj_ptr):
    return _lib._mod.tuix_text_clear_bg(_obj_addr(obj_ptr))


def tuix_box_set_title(obj_ptr, title):
    return _lib._mod.tuix_box_set_title(_obj_addr(obj_ptr), _to_bytes(title))


def tuix_box_set_colors(obj_ptr, border_r, border_g, border_b, bg_r, bg_g, bg_b):
    return _lib._mod.tuix_box_set_colors(_obj_addr(obj_ptr), border_r, border_g, border_b, bg_r, bg_g, bg_b)


def tuix_box_set_color(obj_ptr, r, g, b):
    return _lib._mod.tuix_box_set_color(_obj_addr(obj_ptr), r, g, b)


def tuix_box_set_border(obj_ptr, enabled):
    return _lib._mod.tuix_box_set_border(_obj_addr(obj_ptr), enabled)


def tuix_divider_set_orientation(obj_ptr, vertical):
    return _lib._mod.tuix_divider_set_orientation(_obj_addr(obj_ptr), vertical)


def tuix_divider_set_symbol(obj_ptr, sym):
    return _lib._mod.tuix_divider_set_symbol(_obj_addr(obj_ptr), _to_char(sym))


def tuix_divider_set_color(obj_ptr, r, g, b):
    return _lib._mod.tuix_divider_set_color(_obj_addr(obj_ptr), r, g, b)


def tuix_badge_set_text(obj_ptr, text):
    return _lib._mod.tuix_badge_set_text(_obj_addr(obj_ptr), _to_bytes(text))


def tuix_badge_set_colors(obj_ptr, fr, fg, fb, br, bg, bb):
    return _lib._mod.tuix_badge_set_colors(_obj_addr(obj_ptr), fr, fg, fb, br, bg, bb)


def tuix_button_set_label(obj_ptr, label):
    return _lib._mod.tuix_button_set_label(_obj_addr(obj_ptr), _to_bytes(label))


def tuix_button_take_pressed(obj_ptr):
    return _lib._mod.tuix_button_take_pressed(_obj_addr(obj_ptr))


def tuix_button_reset(obj_ptr):
    _lib._mod.tuix_button_reset(_obj_addr(obj_ptr))


def tuix_button_feed_input(obj_ptr, snap):
    return _lib._mod.tuix_button_feed_input(_obj_addr(obj_ptr), snap)


def tuix_button_is_pressed(obj_ptr):
    return _lib._mod.tuix_button_is_pressed(_obj_addr(obj_ptr))


def tuix_button_get_state(obj_ptr):
    return _lib._mod.tuix_button_get_state(_obj_addr(obj_ptr))


def tuix_tag_set_text(obj_ptr, text):
    return _lib._mod.tuix_tag_set_text(_obj_addr(obj_ptr), _to_bytes(text))


def tuix_tag_set_brackets(obj_ptr, left, right):
    return _lib._mod.tuix_tag_set_brackets(_obj_addr(obj_ptr), _to_char(left), _to_char(right))


def tuix_tag_set_colors(obj_ptr, fr, fg, fb, br, bg, bb):
    return _lib._mod.tuix_tag_set_colors(_obj_addr(obj_ptr), fr, fg, fb, br, bg, bb)


def tuix_status_set_text(obj_ptr, text):
    return _lib._mod.tuix_status_set_text(_obj_addr(obj_ptr), _to_bytes(text))


def tuix_status_set_level(obj_ptr, level):
    return _lib._mod.tuix_status_set_level(_obj_addr(obj_ptr), level)


def tuix_status_set_palette(obj_ptr, ok_r, ok_g, ok_b, warn_r, warn_g, warn_b, err_r, err_g, err_b, idle_r, idle_g, idle_b):
    return _lib._mod.tuix_status_set_palette(
        _obj_addr(obj_ptr),
        ok_r,
        ok_g,
        ok_b,
        warn_r,
        warn_g,
        warn_b,
        err_r,
        err_g,
        err_b,
        idle_r,
        idle_g,
        idle_b,
    )


def tuix_status_set_status(obj_ptr, status):
    return _lib._mod.tuix_status_set_status(_obj_addr(obj_ptr), status)


def tuix_menu_set_title(obj_ptr, title):
    return _lib._mod.tuix_menu_set_title(_obj_addr(obj_ptr), _to_bytes(title))


def tuix_menu_set_items(obj_ptr, labels):
    return _lib._mod.tuix_menu_set_items(_obj_addr(obj_ptr), _to_bytes_list(labels))


def tuix_menu_set_options(obj_ptr, labels):
    return _lib._mod.tuix_menu_set_options(_obj_addr(obj_ptr), _to_bytes_list(labels))


def tuix_menu_feed_input(obj_ptr, snap):
    return _lib._mod.tuix_menu_feed_input(_obj_addr(obj_ptr), snap)


def tuix_menu_get_selected(obj_ptr):
    return _lib._mod.tuix_menu_get_selected(_obj_addr(obj_ptr))


def tuix_menu_take_activated(obj_ptr):
    return _lib._mod.tuix_menu_take_activated(_obj_addr(obj_ptr))


def tuix_menu_is_activated(obj_ptr):
    return _lib._mod.tuix_menu_is_activated(_obj_addr(obj_ptr))


def tuix_menu_reset(obj_ptr):
    _lib._mod.tuix_menu_reset(_obj_addr(obj_ptr))


def tuix_scroll_container_is_viewport(obj_ptr):
    return _lib._mod.tuix_scroll_container_is_viewport(_obj_addr(obj_ptr))


def tuix_scroll_container_get_viewport_offset(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_viewport_offset(_obj_addr(obj_ptr))


def tuix_scroll_container_get_viewport_insets(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_viewport_insets(_obj_addr(obj_ptr))


def tuix_scroll_container_set_title(obj_ptr, title):
    return _lib._mod.tuix_scroll_container_set_title(_obj_addr(obj_ptr), _to_bytes(title))


def tuix_scroll_container_set_content_size(obj_ptr, width, height):
    return _lib._mod.tuix_scroll_container_set_content_size(_obj_addr(obj_ptr), width, height)


def tuix_scroll_container_set_offset(obj_ptr, offset_x, offset_y):
    return _lib._mod.tuix_scroll_container_set_offset(_obj_addr(obj_ptr), offset_x, offset_y)


def tuix_scroll_container_get_offset_x(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_offset_x(_obj_addr(obj_ptr))


def tuix_scroll_container_get_offset_y(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_offset_y(_obj_addr(obj_ptr))


def tuix_scroll_container_get_content_width(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_content_width(_obj_addr(obj_ptr))


def tuix_scroll_container_get_content_height(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_content_height(_obj_addr(obj_ptr))


def tuix_scroll_container_add_object(obj_ptr, scene_name, builder_name, width_mod, height_mod, margin_top_mod, margin_left_mod):
    return _lib._mod.tuix_scroll_container_add_object(
        _obj_addr(obj_ptr),
        _to_bytes(scene_name),
        _to_bytes(builder_name),
        width_mod,
        height_mod,
        margin_top_mod,
        margin_left_mod,
    )


def tuix_scroll_container_attach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_scroll_container_attach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_scroll_container_detach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_scroll_container_detach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_scroll_container_add_object_at(obj_ptr, scene_name, builder_name, content_x, content_y, content_w, content_h):
    return _lib._mod.tuix_scroll_container_add_object_at(
        _obj_addr(obj_ptr),
        _to_bytes(scene_name),
        _to_bytes(builder_name),
        content_x,
        content_y,
        content_w,
        content_h,
    )


def tuix_scroll_container_feed_input(obj_ptr, snap):
    return _lib._mod.tuix_scroll_container_feed_input(_obj_addr(obj_ptr), snap)


def tuix_scroll_container_get_scroll_pos(obj_ptr):
    return _lib._mod.tuix_scroll_container_get_scroll_pos(_obj_addr(obj_ptr))


def tuix_checkbox_set_label(obj_ptr, label):
    return _lib._mod.tuix_checkbox_set_label(_obj_addr(obj_ptr), _to_bytes(label))


def tuix_checkbox_set_checked(obj_ptr, checked):
    return _lib._mod.tuix_checkbox_set_checked(_obj_addr(obj_ptr), 1 if checked else 0)


def tuix_checkbox_get_checked(obj_ptr):
    return _lib._mod.tuix_checkbox_get_checked(_obj_addr(obj_ptr))


def tuix_checkbox_toggle(obj_ptr):
    return _lib._mod.tuix_checkbox_toggle(_obj_addr(obj_ptr))


def tuix_checkbox_take_changed(obj_ptr):
    return _lib._mod.tuix_checkbox_take_changed(_obj_addr(obj_ptr))


def tuix_checkbox_set_disabled(obj_ptr, disabled):
    return _lib._mod.tuix_checkbox_set_disabled(_obj_addr(obj_ptr), 1 if disabled else 0)


def tuix_listview_set_title(obj_ptr, title):
    return _lib._mod.tuix_listview_set_title(_obj_addr(obj_ptr), _to_bytes(title))


def tuix_listview_set_items(obj_ptr, items):
    return _lib._mod.tuix_listview_set_items(_obj_addr(obj_ptr), _to_bytes_list(items))


def tuix_listview_set_selected(obj_ptr, index):
    return _lib._mod.tuix_listview_set_selected(_obj_addr(obj_ptr), index)


def tuix_listview_get_selected(obj_ptr):
    return _lib._mod.tuix_listview_get_selected(_obj_addr(obj_ptr))


def tuix_listview_take_activated(obj_ptr):
    return _lib._mod.tuix_listview_take_activated(_obj_addr(obj_ptr))


def tuix_textarea_set_title(obj_ptr, title):
    return _lib._mod.tuix_textarea_set_title(_obj_addr(obj_ptr), _to_bytes(title))


def tuix_textarea_set_text(obj_ptr, text):
    return _lib._mod.tuix_textarea_set_text(_obj_addr(obj_ptr), _to_bytes(text))


def tuix_textarea_get_text(obj_ptr):
    return _lib._mod.tuix_textarea_get_text(_obj_addr(obj_ptr))


def tuix_textarea_set_placeholder(obj_ptr, text):
    return _lib._mod.tuix_textarea_set_placeholder(_obj_addr(obj_ptr), _to_bytes(text))


def tuix_textarea_set_readonly(obj_ptr, readonly):
    return _lib._mod.tuix_textarea_set_readonly(_obj_addr(obj_ptr), 1 if readonly else 0)


def tuix_dialog_attach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_dialog_attach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_dialog_detach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_dialog_detach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_dialog_add_object(obj_ptr, scene_name, builder_name, width_mod, height_mod):
    return _lib._mod.tuix_dialog_add_object(_obj_addr(obj_ptr), _to_bytes(scene_name), _to_bytes(builder_name), width_mod, height_mod)


def tuix_dialog_set_title(obj_ptr, title):
    return _lib._mod.tuix_dialog_set_title(_obj_addr(obj_ptr), _to_bytes(title))


def tuix_dialog_set_body_size(obj_ptr, width, height):
    return _lib._mod.tuix_dialog_set_body_size(_obj_addr(obj_ptr), width, height)


def tuix_dialog_set_padding(obj_ptr, left, top, right, bottom):
    return _lib._mod.tuix_dialog_set_padding(_obj_addr(obj_ptr), left, top, right, bottom)


def tuix_dialog_set_close_on_esc(obj_ptr, enabled):
    return _lib._mod.tuix_dialog_set_close_on_esc(_obj_addr(obj_ptr), 1 if enabled else 0)


def tuix_dialog_set_close_on_backdrop(obj_ptr, enabled):
    return _lib._mod.tuix_dialog_set_close_on_backdrop(_obj_addr(obj_ptr), 1 if enabled else 0)


def tuix_dialog_set_colors(obj_ptr, backdrop_r, backdrop_g, backdrop_b, border_r, border_g, border_b, body_r, body_g, body_b):
    return _lib._mod.tuix_dialog_set_colors(
        _obj_addr(obj_ptr),
        backdrop_r,
        backdrop_g,
        backdrop_b,
        border_r,
        border_g,
        border_b,
        body_r,
        body_g,
        body_b,
    )


def tuix_dialog_activate(obj_ptr, scene_name):
    return _lib._mod.tuix_dialog_activate(_obj_addr(obj_ptr), _to_bytes(scene_name))


def tuix_dialog_deactivate(obj_ptr, scene_name):
    return _lib._mod.tuix_dialog_deactivate(_obj_addr(obj_ptr), _to_bytes(scene_name))


def tuix_dialog_take_close_requested(obj_ptr):
    return _lib._mod.tuix_dialog_take_close_requested(_obj_addr(obj_ptr))


def tuix_stack_attach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_stack_attach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_stack_detach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_stack_detach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_stack_add_object(obj_ptr, scene_name, builder_name, width_mod, height_mod):
    return _lib._mod.tuix_stack_add_object(_obj_addr(obj_ptr), _to_bytes(scene_name), _to_bytes(builder_name), width_mod, height_mod)


def tuix_stack_set_gap(obj_ptr, gap):
    return _lib._mod.tuix_stack_set_gap(_obj_addr(obj_ptr), gap)


def tuix_stack_set_padding(obj_ptr, left, top, right, bottom):
    return _lib._mod.tuix_stack_set_padding(_obj_addr(obj_ptr), left, top, right, bottom)


def tuix_stack_set_justify(obj_ptr, justify):
    return _lib._mod.tuix_stack_set_justify(_obj_addr(obj_ptr), justify)


def tuix_stack_set_align(obj_ptr, align):
    return _lib._mod.tuix_stack_set_align(_obj_addr(obj_ptr), align)


def tuix_stack_set_bg(obj_ptr, r, g, b):
    return _lib._mod.tuix_stack_set_bg(_obj_addr(obj_ptr), r, g, b)


def tuix_stack_clear_bg(obj_ptr):
    return _lib._mod.tuix_stack_clear_bg(_obj_addr(obj_ptr))


def tuix_splitpane_attach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_splitpane_attach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_splitpane_detach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_splitpane_detach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_splitpane_add_object(obj_ptr, scene_name, builder_name, width_mod, height_mod):
    return _lib._mod.tuix_splitpane_add_object(_obj_addr(obj_ptr), _to_bytes(scene_name), _to_bytes(builder_name), width_mod, height_mod)


def tuix_splitpane_set_orientation(obj_ptr, orientation):
    return _lib._mod.tuix_splitpane_set_orientation(_obj_addr(obj_ptr), orientation)


def tuix_splitpane_set_split_ratio(obj_ptr, ratio):
    return _lib._mod.tuix_splitpane_set_split_ratio(_obj_addr(obj_ptr), float(ratio))


def tuix_splitpane_set_split_pixels(obj_ptr, pixels):
    return _lib._mod.tuix_splitpane_set_split_pixels(_obj_addr(obj_ptr), pixels)


def tuix_splitpane_set_divider_size(obj_ptr, divider_size):
    return _lib._mod.tuix_splitpane_set_divider_size(_obj_addr(obj_ptr), divider_size)


def tuix_splitpane_set_min_sizes(obj_ptr, min_first, min_second):
    return _lib._mod.tuix_splitpane_set_min_sizes(_obj_addr(obj_ptr), min_first, min_second)


def tuix_splitpane_set_colors(obj_ptr, divider_fg_r, divider_fg_g, divider_fg_b, divider_bg_r, divider_bg_g, divider_bg_b, bg_r, bg_g, bg_b):
    return _lib._mod.tuix_splitpane_set_colors(
        _obj_addr(obj_ptr),
        divider_fg_r,
        divider_fg_g,
        divider_fg_b,
        divider_bg_r,
        divider_bg_g,
        divider_bg_b,
        bg_r,
        bg_g,
        bg_b,
    )


def tuix_splitpane_clear_bg(obj_ptr):
    return _lib._mod.tuix_splitpane_clear_bg(_obj_addr(obj_ptr))


def tuix_grid_attach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_grid_attach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_grid_detach_child(obj_ptr, scene_name, child_uid):
    return _lib._mod.tuix_grid_detach_child(_obj_addr(obj_ptr), _to_bytes(scene_name), child_uid)


def tuix_grid_add_object(obj_ptr, scene_name, builder_name, width_mod, height_mod):
    return _lib._mod.tuix_grid_add_object(_obj_addr(obj_ptr), _to_bytes(scene_name), _to_bytes(builder_name), width_mod, height_mod)


def tuix_grid_set_columns(obj_ptr, tracks):
    return _lib._mod.tuix_grid_set_columns(_obj_addr(obj_ptr), list(tracks))


def tuix_grid_set_rows(obj_ptr, tracks):
    return _lib._mod.tuix_grid_set_rows(_obj_addr(obj_ptr), list(tracks))


def tuix_grid_set_padding(obj_ptr, left, top, right, bottom):
    return _lib._mod.tuix_grid_set_padding(_obj_addr(obj_ptr), left, top, right, bottom)


def tuix_grid_set_gaps(obj_ptr, gap_x, gap_y):
    return _lib._mod.tuix_grid_set_gaps(_obj_addr(obj_ptr), gap_x, gap_y)


def tuix_grid_set_bg(obj_ptr, r, g, b):
    return _lib._mod.tuix_grid_set_bg(_obj_addr(obj_ptr), r, g, b)


def tuix_grid_clear_bg(obj_ptr):
    return _lib._mod.tuix_grid_clear_bg(_obj_addr(obj_ptr))
