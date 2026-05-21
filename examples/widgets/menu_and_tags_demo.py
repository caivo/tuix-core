"""Menu + Tags demo.

UP/DOWN changes selection in focused menu.
ENTER activates selected item.
ESC exits.
"""

import time

from tuix.core import builders, engine, input, objects, scenes


SCENE = b"Main"


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


MENU_OPTIONS = [b"New Session", b"Load Session", b"Settings", b"About", b"Exit"]
TAGS = [b"CLI", b"Widgets", b"Demo"]


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

        for i, text in enumerate(TAGS):
            tag = must_get_obj(
                objects.create_object(builders.TAG, SCENE, 0.14, 0.08, 0.06, 0.06 + i * 0.16),
                f"tag[{i}]",
            )
            objects.tuix_tag_set_text(tag, text)
            objects.tuix_tag_set_brackets(tag, b"[", b"]")
            objects.tuix_tag_set_colors(tag, 250, 220, 120, 45, 45, 70)

        menu = must_get_obj(
            objects.create_object(builders.MENU, SCENE, 0.44, 0.6, 0.2, 0.28),
            "menu",
        )
        objects.tuix_menu_set_title(menu, b"MenuBuilder")
        objects.tuix_menu_set_items(menu, MENU_OPTIONS)

        selected_text = must_get_obj(
            objects.create_object(builders.TEXT, SCENE, 0.35, 0.1, 0.95, 0.06),
            "selected_text",
        )
        objects.tuix_text_set_text(selected_text, b"Selected: -")
        scenes.set_focus(SCENE, selected_text.uid)
        scenes.set_focus(SCENE, menu.uid)

        while True:
            snap = input.peek_snapshot()
            kb = snap.keyboard
            if kb and kb.has_event and kb.code == 0x1B:
                break

            engine.main_loop()

            if objects.tuix_menu_take_activated(menu):
                idx = objects.tuix_menu_get_selected(menu)
                if 0 <= idx < len(MENU_OPTIONS):
                    display_text = f"Selected: {MENU_OPTIONS[idx].decode('utf-8')}".encode()
                    objects.tuix_text_set_text(selected_text, display_text)
                if idx == len(MENU_OPTIONS) - 1:
                    break

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
