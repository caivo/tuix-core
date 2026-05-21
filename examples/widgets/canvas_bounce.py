"""
Canvas drawing example.

Draws an animated bouncing ball inside a bordered canvas.
Press any key to quit.
"""

import math
import time
from tuix.core import engine, builders, scenes, objects, buffers, input


SCENE = b"Main"


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


def draw_border(obj, w, h):
    c = (100, 180, 220)
    bg = (0, 0, 0)
    objects.tuix_canvas_draw_rect(obj, 0, 0, w, h, b'-', 0, *c, *bg)
    for y in range(1, h - 1):
        objects.tuix_canvas_set_pixel(obj, 0, y, b'|', *c, *bg)
        objects.tuix_canvas_set_pixel(obj, w - 1, y, b'|', *c, *bg)
    objects.tuix_canvas_set_pixel(obj, 0, 0, b'+', *c, *bg)
    objects.tuix_canvas_set_pixel(obj, w - 1, 0, b'+', *c, *bg)
    objects.tuix_canvas_set_pixel(obj, 0, h - 1, b'+', *c, *bg)
    objects.tuix_canvas_set_pixel(obj, w - 1, h - 1, b'+', *c, *bg)


def main():
    if engine.init() != 0:
        print("init failed")
        return 1

    builders.register_standard()
    scenes.init_scene(SCENE)
    scenes.select_scene(SCENE)
    input.listen()

    uid = objects.create_object(builders.CANVAS, SCENE, 0.85, 0.75, 0.1, 0.075)
    engine.main_loop()
    sleep_ms(40)

    snap = buffers.get_buffer_snapshot_by_uid(uid)
    obj = objects.get_object_by_uid(uid)
    if not snap or obj is None:
        print("failed to get canvas snapshot/object")
        return 1
    w = int(snap['width'])
    h = int(snap['height'])

    iw, ih = w - 2, h - 2

    bx, by = iw / 2.0, ih / 2.0
    dx, dy = 0.6, 0.35
    frame = 0
    t0 = time.time()

    while True:
        snap = input.peek_snapshot()
        kb = snap.keyboard
        if kb and kb.has_event:
            break

        objects.tuix_canvas_draw_rect(obj, 1, 1, iw, ih, b' ', 1, 0, 0, 0, 0, 0, 0)

        hue = (frame * 3) % 360
        r = int(128 + 127 * math.sin(math.radians(hue)))
        g = int(128 + 127 * math.sin(math.radians(hue + 120)))
        b = int(128 + 127 * math.sin(math.radians(hue + 240)))

        ix, iy = int(round(bx)) + 1, int(round(by)) + 1
        objects.tuix_canvas_set_pixel(obj, ix, iy, b'O', r, g, b, 0, 0, 0)

        draw_border(obj, w, h)

        elapsed = time.time() - t0
        fps = frame / elapsed if elapsed > 0 else 0
        label = f"{fps:.0f}fps".encode()
        lx = w - len(label) - 1
        if lx > 0:
            objects.tuix_canvas_draw_text(obj, lx, 0, label, 200, 200, 50, 0, 0, 0)

        engine.main_loop()
        sleep_ms(16)
        frame += 1

        bx += dx
        by += dy
        if bx <= 0 or bx >= iw - 1:
            dx = -dx
            bx = max(0.0, min(float(iw - 1), bx))
        if by <= 0 or by >= ih - 1:
            dy = -dy
            by = max(0.0, min(float(ih - 1), by))

    buffers.free_buffer(SCENE, uid)
    input.stop()
    engine.shutdown()
    return 0


if __name__ == "__main__":
    main()
