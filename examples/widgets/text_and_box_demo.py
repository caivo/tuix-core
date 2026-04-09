"""Text + Box demo.

Press SPACE to cycle text color.
Press T to rotate message text.
Press ESC to exit.
"""

import time

from tuix.core import builders, engine, input, objects, registry, scenes


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


COLORS = [
    (255, 120, 120),
    (140, 255, 140),
    (120, 170, 255),
    (255, 235, 120),
]

MESSAGES = [
    b"Hello from TextBuilder",
    b"BoxBuilder renders frame",
    b"SPACE changes color",
    b"T changes content",
]


def must_get_obj(uid, name):
    obj = objects.get_object_by_uid(uid)
    if obj is None:
        raise RuntimeError(f"failed to create {name}")
    return obj


def main():
    if engine.init() != 0:
        print("init failed")
        return 1

    try:
        builders.register_standard()
        scenes.init_scene(b"Main")
        registry.registry.current_scene_name = b"Main"
        input.listen()

        box = must_get_obj(
            objects.create_object(builders.BOX, b"Main", 0.7, 0.5, 0.2, 0.15),
            "box",
        )
        text = must_get_obj(
            objects.create_object(builders.TEXT, b"Main", 0.62, 0.35, 0.28, 0.19),
            "text",
        )

        objects.tuix_box_set_title(box, b"Text + Box")
        objects.tuix_box_set_color(box, 220, 190, 120)

        color_idx = 0
        msg_idx = 0
        objects.tuix_text_set_text(text, MESSAGES[msg_idx])
        objects.tuix_text_set_bg(text, 0, 0, 0)

        while True:
            snap = input.peek_snapshot()
            kb = snap.keyboard
            if kb and kb.has_event:
                if kb.code == 0x1B:
                    break
                if kb.code == 0x20:
                    color_idx = (color_idx + 1) % len(COLORS)
                    r, g, b = COLORS[color_idx]
                    objects.tuix_text_set_fg(text, r, g, b)
                if kb.code in (ord("t"), ord("T")):
                    msg_idx = (msg_idx + 1) % len(MESSAGES)
                    objects.tuix_text_set_text(text, MESSAGES[msg_idx])

            engine.main_loop()
            sleep_ms(16)

        return 0
    except Exception as exc:
        print(f"demo error: {exc}")
        return 1
    finally:
        input.stop()
        engine.shutdown()


if __name__ == "__main__":
    raise SystemExit(main())
