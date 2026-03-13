"""
Progressbar example.

Shows two progress bars filling at different speeds.
Press ESC to quit early.
"""

import time
from tuix.core import engine, builders, scenes, registry, objects, buffers, input


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


def main():
    if engine.init() != 0:
        print("init failed")
        return 1

    builders.register_standard()
    scenes.init_scene(b"Main")
    registry.registry.current_scene_name = b"Main"
    input.listen()

    uid_a = objects.create_object(builders.PROGRESSBAR, b"Main", 0.7, 0.08, 0.3,  0.15)
    uid_b = objects.create_object(builders.PROGRESSBAR, b"Main", 0.7, 0.08, 0.55, 0.15)

    ptr_a = buffers.get_buffer_by_uid(uid_a)
    ptr_b = buffers.get_buffer_by_uid(uid_b)
    obj_a = ptr_a.contents.obj.contents
    obj_b = ptr_b.contents.obj.contents

    objects.tuix_progressbar_set_style(obj_a, ord('#'), ord('-'), 120, 220, 80,  50, 50, 50)
    objects.tuix_progressbar_set_style(obj_b, ord('='), ord('.'), 60,  160, 255, 50, 50, 50)

    step = 0
    while step <= 100:
        snap = input.get_snapshot()
        kb   = snap.keyboard
        if kb and kb.has_event and kb.code == 0x1B:
            break

        # bar A moves at full speed, bar B at half speed
        objects.tuix_progressbar_set_value(obj_a, step / 100.0)
        if step <= 50:
            objects.tuix_progressbar_set_value(obj_b, step / 50.0)

        engine.main_loop()
        sleep_ms(60)
        step += 1

    buffers.free_buffer(b"Main", uid_a)
    buffers.free_buffer(b"Main", uid_b)
    input.stop()
    engine.shutdown()
    return 0


if __name__ == "__main__":
    main()
