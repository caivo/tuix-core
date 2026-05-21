"""Button + Badge + Checkbox demo.

Uses ButtonBuilder widgets, a BadgeBuilder header, and CheckboxBuilder state.
Press ESC to exit. Clicking buttons prints events to stdout.
"""

import time

from tuix.core import builders, engine, input, objects, scenes


SCENE = b"Main"


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
        scenes.init_scene(SCENE)
        scenes.select_scene(SCENE)
        input.listen()

        badge = must_get_obj(
            objects.create_object(builders.BADGE, SCENE, 0.32, 0.12, 0.07, 0.34),
            "badge",
        )
        objects.tuix_badge_set_text(badge, b"Buttons")
        objects.tuix_badge_set_colors(badge, 255, 230, 120, 35, 45, 80)

        checkbox_uid = objects.create_object(builders.CHECKBOX, SCENE, 0.34, 0.08, 0.19, 0.34)
        checkbox = must_get_obj(checkbox_uid, "checkbox")
        objects.tuix_checkbox_set_label(checkbox, b"Enable events")
        objects.tuix_checkbox_set_checked(checkbox, True)
        scenes.set_focus(SCENE, checkbox_uid)

        labels = [b"Primary", b"Secondary", b"Danger"]
        btn_objs = []
        for i, label in enumerate(labels):
            uid = objects.create_object(builders.BUTTON, SCENE, 0.28, 0.11, 0.31 + i * 0.14, 0.36)
            btn = must_get_obj(uid, f"button[{i}]")
            objects.tuix_button_set_label(btn, label)
            btn_objs.append((label.decode("utf-8"), btn))

        while True:
            snap = input.peek_snapshot()
            kb = snap.keyboard
            if kb and kb.has_event and kb.code == 0x1B:
                break

            engine.main_loop()

            if objects.tuix_checkbox_take_changed(checkbox):
                if objects.tuix_checkbox_get_checked(checkbox):
                    objects.tuix_badge_set_text(badge, b"Events on")
                else:
                    objects.tuix_badge_set_text(badge, b"Events off")

            for label, btn in btn_objs:
                if objects.tuix_button_take_pressed(btn) and objects.tuix_checkbox_get_checked(checkbox):
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
