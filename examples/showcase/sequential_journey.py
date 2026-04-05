import time
import random
from tuix.core import engine, builders, scenes, registry, objects, buffers, input, _structs
def sleep_ms(ms):
    time.sleep(ms / 1000.0)

# --- Drawing helpers (kept simple) ---
def draw_border(obj, w, h, title=None):
    fr, fg, fb = 100, 200, 220
    br, bg, bb = 0, 0, 0
    objects.tuix_canvas_set_pixel(obj, 0, 0, b'+', fr, fg, fb, br, bg, bb)
    objects.tuix_canvas_set_pixel(obj, w - 1, 0, b'+', fr, fg, fb, br, bg, bb)
    objects.tuix_canvas_set_pixel(obj, 0, h - 1, b'+', fr, fg, fb, br, bg, bb)
    objects.tuix_canvas_set_pixel(obj, w - 1, h - 1, b'+', fr, fg, fb, br, bg, bb)
    for x in range(1, w - 1):
        objects.tuix_canvas_set_pixel(obj, x, 0, b'-', fr, fg, fb, br, bg, bb)
        objects.tuix_canvas_set_pixel(obj, x, h - 1, b'-', fr, fg, fb, br, bg, bb)
    for y in range(1, h - 1):
        objects.tuix_canvas_set_pixel(obj, 0, y, b'|', fr, fg, fb, br, bg, bb)
        objects.tuix_canvas_set_pixel(obj, w - 1, y, b'|', fr, fg, fb, br, bg, bb)
    if title:
        start = max(1, (w - len(title)) // 2)
        objects.tuix_canvas_draw_text(obj, start, 0, title.encode(), 255, 255, 255, br, bg, bb)

def build_smiley():
    smiley = (_structs.TuixPixel * 9)()
    for i in range(9):
        smiley[i].sym = b'.' + (b'\x00' * 7)
        smiley[i].styles.fg = _structs.TuixRGBTuple(80, 80, 80)
        smiley[i].styles.bg = _structs.TuixRGBTuple(0, 0, 0)
        smiley[i].styles.custom_fg = 1
        smiley[i].styles.custom_bg = 1
    smiley[1].sym = b'O' + (b'\x00' * 7)
    smiley[1].styles.fg = _structs.TuixRGBTuple(255, 255, 0)
    smiley[6].sym = b'\\' + (b'\x00' * 7)
    smiley[6].styles.fg = _structs.TuixRGBTuple(255, 100, 100)
    smiley[7].sym = b'_' + (b'\x00' * 7)
    smiley[7].styles.fg = _structs.TuixRGBTuple(255, 100, 100)
    smiley[8].sym = b'/' + (b'\x00' * 7)
    smiley[8].styles.fg = _structs.TuixRGBTuple(255, 100, 100)
    return smiley


# --- Sequential demo ported from usage_example.c ---
def main():
    random.seed(int(time.time()))
    if engine.init() != 0:
        print("tuix_init() failed")
        return 1

    builders.register_standard()
    scenes.init_scene(b"MainScene")
    registry.registry.current_scene_name = b"MainScene"
    input.listen()

    # Colour LUT (matches choice order)
    colour_r = [255, 0, 60, 255, 0]
    colour_g = [60, 200, 120, 220, 220]
    colour_b = [60, 60, 255, 40, 220]
    chosen_colour = 2
    user_name = "TUIX Demo"

    # Stage 1: ProgressBar
    uid = objects.create_object(builders.PROGRESSBAR, b"MainScene", 0.8, 0.1, 0.45, 0.1)
    obj = objects.get_object_by_uid(uid)
    if not obj:
        print("progressbar object retrieval failed")
        engine.shutdown()
        return 1

    # Progressbar demo with animated value
    objects.tuix_progressbar_set_value(obj, 0.0)
    objects.tuix_progressbar_set_style(obj, b'#'[0], b'-'[0], 80, 200, 120, 60, 60, 60)

    for f in range(51):
        snap = input.get_snapshot()
        kb = snap.keyboard
        if kb and kb.has_event and kb.code == 0x1B:
            break
        val = float(f) / 50.0
        objects.tuix_progressbar_set_value(obj, val)
        engine.main_loop()
        sleep_ms(40)

    buffers.free_buffer(b"MainScene", uid)

    # Stage 2: Choice
    uid = objects.create_object(builders.CHOICE, b"MainScene", 0.4, 0.25, 0.35, 0.3)
    obj = objects.get_object_by_uid(uid)
    if not obj:
        print("choice object retrieval failed")
        engine.shutdown()
        return 1

    options = [b"Red", b"Green", b"Blue", b"Yellow", b"Cyan"]
    objects.tuix_choice_set_options(obj, options)

    while not objects.tuix_choice_is_confirmed(obj):
        snap = input.get_snapshot()
        kb = snap.keyboard
        if kb and kb.has_event and kb.code == 0x1B:
            break
        objects.tuix_choice_feed_input(obj, snap)
        engine.main_loop()
        sleep_ms(16)

    if objects.tuix_choice_is_confirmed(obj):
        res = objects.tuix_choice_get_result(obj)
        try:
            chosen_colour = int(res)
        except Exception:
            chosen_colour = 2
        engine.main_loop()
        sleep_ms(400)

    buffers.free_buffer(b"MainScene", uid)

    # Stage 3: Input
    uid = objects.create_object(builders.INPUT, b"MainScene", 0.6, 0.1, 0.45, 0.2)
    obj = objects.get_object_by_uid(uid)
    if not obj:
        print("input object retrieval failed")
        engine.shutdown()
        return 1
    objects.tuix_input_set_placeholder(obj, b"Type your name...")

    while not objects.tuix_input_is_submitted(obj):
        snap = input.get_snapshot()
        kb = snap.keyboard
        if kb and kb.has_event and kb.code == 0x1B:
            break
        objects.tuix_input_feed_input(obj, snap)
        engine.main_loop()
        sleep_ms(16)

    if objects.tuix_input_is_submitted(obj):
        result = objects.tuix_input_get_result(obj)
        if result and result != b"":
            try:
                user_name = result.decode()
            except Exception:
                pass
        engine.main_loop()
        sleep_ms(400)

    buffers.free_buffer(b"MainScene", uid)

    # Stage 4: Canvas
    uid = objects.create_object(builders.CANVAS, b"MainScene", 0.9, 0.85, 0.05, 0.05)
    # allow one frame to resolve geometry
    engine.main_loop()
    sleep_ms(50)

    snap = buffers.get_buffer_snapshot(b"MainScene", uid)
    if snap and snap['width'] > 0 and snap['height'] > 0:
        cw = snap['width']
        ch = snap['height']
        r = colour_r[chosen_colour]
        g = colour_g[chosen_colour]
        b = colour_b[chosen_colour]

        # Border
        tuix_obj = objects.get_object_by_uid(uid)
        if tuix_obj:
            objects.tuix_canvas_draw_rect(tuix_obj, 0, 0, cw, ch, b'.', 0, 80, 80, 80, 0, 0, 0)
            # Title
            title = user_name.encode() if isinstance(user_name, str) else b""
            tlen = len(title)
            tx = (cw - tlen) // 2
            if tx < 1: tx = 1
            objects.tuix_canvas_draw_text(tuix_obj, tx, 1, title, 255, 255, 255, 0, 0, 0)
            # Circle & shapes
            radius = (ch // 2) - 2
            if radius < 2: radius = 2
            objects.tuix_canvas_draw_circle(tuix_obj, cw // 4, ch // 2, radius, b'O', 1, r, g, b, 0, 0, 0)
            objects.tuix_canvas_draw_circle(tuix_obj, cw * 3 // 4, ch // 2, radius, b'o', 0, r, g, b, 0, 0, 0)
            x0, y0 = cw // 2 + 2, 3
            x1, y1 = cw - 3, ch - 2
            objects.tuix_canvas_draw_line(tuix_obj, x0, y0, x1, y1, b'/', r, g, b, 0, 0, 0)
            objects.tuix_canvas_draw_line(tuix_obj, x1, y0, x0, y1, b'\\', r, g, b, 0, 0, 0)
            objects.tuix_canvas_draw_rect(tuix_obj, 2, ch - 4, 10, 3, b'#', 1, r, g, b, 0, 0, 0)

    # Show canvas until any key pressed
    while True:
        snap = input.get_snapshot()
        kb = snap.keyboard
        if kb and kb.has_event:
            break
        engine.main_loop()
        sleep_ms(16)

    buffers.free_buffer(b"MainScene", uid)

    # Cleanup
    input.stop()
    engine.shutdown()
    return 0


if __name__ == "__main__":
    main()
