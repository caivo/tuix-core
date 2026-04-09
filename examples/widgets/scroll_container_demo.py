"""ScrollContainer demo.

Use arrows/mouse wheel/drag to scroll via ScrollContainerBuilder.
ESC exits.
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

        scroll = must_get_obj(
            objects.create_object(builders.SCROLL_CONTAINER, b"Main", 0.7, 0.75, 0.14, 0.15),
            "scroll container",
        )

        objects.tuix_scroll_container_set_content_size(scroll, 240, 120)
        objects.tuix_scroll_container_set_title(scroll, b"Scrollable Grid")

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
