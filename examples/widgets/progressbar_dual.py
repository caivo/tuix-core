"""
Progressbar example.

Shows two progress bars filling at different speeds.
Press ESC to quit early.
"""

import time
from tuix.core import engine, builders, scenes, objects, buffers, input


SCENE = b"Main"


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


def main():
    if engine.init() != 0:
        print("init failed")
        return 1

    builders.register_standard()
    scenes.init_scene(SCENE)
    scenes.select_scene(SCENE)
    input.listen()

    uid_a = objects.create_object(builders.PROGRESSBAR, SCENE, 0.7, 0.08, 0.3, 0.15)
    uid_b = objects.create_object(builders.PROGRESSBAR, SCENE, 0.7, 0.08, 0.55, 0.15)

    obj_a = objects.get_object_by_uid(uid_a)
    obj_b = objects.get_object_by_uid(uid_b)
    if obj_a is None or obj_b is None:
        print("failed to get progressbar objects")
        return 1

    objects.tuix_progressbar_set_style(obj_a, ord('#'), ord('-'), 120, 220, 80, 50, 50, 50)
    objects.tuix_progressbar_set_style(obj_b, ord('='), ord('.'), 60, 160, 255, 50, 50, 50)
    objects.tuix_progressbar_show_percentage(obj_a, True)
    objects.tuix_progressbar_show_percentage(obj_b, True)

    step = 0
    while step <= 100:
        snap = input.peek_snapshot()
        kb = snap.keyboard
        if kb and kb.has_event and kb.code == 0x1B:
            break

        objects.tuix_progressbar_set_value(obj_a, step / 100.0)
        if step <= 50:
            objects.tuix_progressbar_set_value(obj_b, step / 50.0)

        engine.main_loop()
        sleep_ms(60)
        step += 1

    buffers.free_buffer(SCENE, uid_a)
    buffers.free_buffer(SCENE, uid_b)
    input.stop()
    engine.shutdown()
    return 0


if __name__ == "__main__":
    main()
