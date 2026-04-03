"""
Choice menu example.

Presents a colour palette menu and prints the chosen colour to stdout.
Press ESC to cancel.
"""

import time
from tuix.core import engine, builders, scenes, registry, objects, buffers, input


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


COLOURS = [
    (b"Red", 255, 60, 60),
    (b"Green", 60, 220, 80),
    (b"Blue", 60, 120, 255),
    (b"Yellow", 255, 220, 40),
    (b"Cyan", 40, 220, 220),
    (b"Magenta", 200, 60, 200),
]


def main():
    if engine.init() != 0:
        print("init failed")
        return 1

    builders.register_standard()
    scenes.init_scene(b"Main")
    registry.registry.current_scene_name = b"Main"
    input.listen()

    uid = objects.create_object(builders.CHOICE, b"Main", 0.35, 0.55, 0.2, 0.32)
    ptr = buffers.get_buffer_by_uid(uid)
    obj = ptr.contents.obj.contents

    options = [name for name, *_ in COLOURS]
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
        idx = int(objects.tuix_choice_get_result(obj))
        name, r, g, b = COLOURS[idx]
        engine.main_loop()
        time.sleep(0.3)
        print(f"\nChosen: {name.decode()}  RGB({r}, {g}, {b})")

    buffers.free_buffer(b"Main", uid)
    input.stop()
    engine.shutdown()
    return 0


if __name__ == "__main__":
    main()
