"""Button + Badge demo.

Uses three ButtonBuilder widgets and one BadgeBuilder header.
Press ESC to exit. Clicking buttons prints events to stdout.
"""

import time

from tuix.core import builders, engine, input, objects, registry, scenes


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


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

        badge = must_get_obj(
            objects.create_object(builders.BADGE, b"Main", 0.32, 0.12, 0.07, 0.34),
            "badge",
        )
        objects.tuix_badge_set_text(badge, b"Buttons")
        objects.tuix_badge_set_colors(badge, 255, 230, 120, 35, 45, 80)

        labels = [b"Primary", b"Secondary", b"Danger"]
        btn_objs = []
        for i, label in enumerate(labels):
            uid = objects.create_object(builders.BUTTON, b"Main", 0.28, 0.11, 0.22 + i * 0.14, 0.36)
            btn = must_get_obj(uid, f"button[{i}]")
            objects.tuix_button_set_label(btn, label)
            btn_objs.append((label.decode("utf-8"), btn))

        while True:
            snap = input.peek_snapshot()
            kb = snap.keyboard
            if kb and kb.has_event and kb.code == 0x1B:
                break

            engine.main_loop()

            for label, btn in btn_objs:
                if objects.tuix_button_is_pressed(btn):
                    print(f"pressed: {label}")

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
