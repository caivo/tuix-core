"""ScrollContainer demo.

Use arrows/mouse wheel/drag to scroll via ScrollContainerBuilder.
ESC exits.
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

        title = must_get_obj(
            objects.create_object(builders.TEXT, SCENE, 0.7, 0.08, 0.05, 0.15),
            "title",
        )
        objects.tuix_text_set_text(title, b"ScrollContainerBuilder + content-space children")
        objects.tuix_text_set_fg(title, 220, 230, 255)

        checkbox = must_get_obj(
            objects.create_object(builders.CHECKBOX, SCENE, 0.35, 0.08, 0.09, 0.15),
            "checkbox",
        )
        objects.tuix_checkbox_set_label(checkbox, b"Viewport input owner")
        objects.tuix_checkbox_set_checked(checkbox, True)

        scroll = must_get_obj(
            objects.create_object(builders.SCROLL_CONTAINER, SCENE, 0.7, 0.72, 0.22, 0.15),
            "scroll container",
        )

        objects.tuix_scroll_container_set_content_size(scroll, 120, 80)
        objects.tuix_scroll_container_set_title(scroll, b"Scrollable Grid")
        scenes.set_focus(SCENE, scroll.uid)

        for row in range(18):
            uid = objects.tuix_scroll_container_add_object_at(
                scroll,
                SCENE,
                builders.TEXT,
                3,
                2 + row * 4,
                80,
                1,
            )
            line = must_get_obj(uid, f"row[{row}]")
            objects.tuix_text_set_text(line, f"content row {row:02d}  offset-aware child".encode())
            objects.tuix_text_set_fg(line, 140 + (row * 7) % 90, 220, 180)

        marker_uid = objects.tuix_scroll_container_add_object_at(
            scroll,
            SCENE,
            builders.BADGE,
            42,
            42,
            20,
            1,
        )
        marker = must_get_obj(marker_uid, "marker")
        objects.tuix_badge_set_text(marker, b"deep content")
        objects.tuix_badge_set_colors(marker, 20, 20, 30, 245, 210, 90)

        while True:
            snap = input.peek_snapshot()
            kb = snap.keyboard
            if kb and kb.has_event:
                if kb.code == 0x1B:
                    break

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
