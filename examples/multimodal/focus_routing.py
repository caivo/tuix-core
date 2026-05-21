"""
Multimodal focus-routing example.

Demonstrates keyboard + mouse driven focus switching between interactive widgets:
  - Choice widget (left)
  - ListView widget (center)
  - TextArea widget (right)
  - Progressbar timeout (top)

Controls:
  - TAB: switch focus between Choice/ListView/TextArea
  - Left mouse click: let the viewport-aware hitmap update focus
  - ESC: exit

Only the focused interactive widget receives the input snapshot.
"""

import time
from tuix.core import engine, builders, scenes, objects, buffers, input


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


SCENE = b"Main"
TAB_KEY = 0x09
ESC_KEY = 0x1B
TIMEOUT_FRAMES = 450


def _next_focus(current_uid, focus_order):
    try:
        index = focus_order.index(current_uid)
    except ValueError:
        return focus_order[0]
    return focus_order[(index + 1) % len(focus_order)]


def main():
    if engine.init() != 0:
        print("init failed")
        return 1

    builders.register_standard()
    scenes.init_scene(SCENE)
    scenes.select_scene(SCENE)
    input.listen()

    uid_pb = objects.create_object(builders.PROGRESSBAR, SCENE, 0.9, 0.07, 0.05, 0.05)
    uid_choice = objects.create_object(builders.CHOICE, SCENE, 0.28, 0.45, 0.2, 0.05)
    uid_list = objects.create_object(builders.LISTVIEW, SCENE, 0.28, 0.45, 0.2, 0.36)
    uid_textarea = objects.create_object(builders.TEXTAREA, SCENE, 0.28, 0.45, 0.2, 0.67)

    pb = objects.get_object_by_uid(uid_pb)
    ch = objects.get_object_by_uid(uid_choice)
    listview = objects.get_object_by_uid(uid_list)
    textarea = objects.get_object_by_uid(uid_textarea)
    if pb is None or ch is None or listview is None or textarea is None:
        print("failed to get one or more objects")
        return 1

    objects.tuix_progressbar_set_style(pb, ord('>'), ord('-'), 80, 200, 80, 40, 40, 40)
    objects.tuix_progressbar_show_percentage(pb, True)
    objects.tuix_choice_set_options(ch, [b"Mouse", b"Keyboard", b"Viewport", b"Modal"])
    objects.tuix_listview_set_title(listview, b"ListView")
    objects.tuix_listview_set_items(
        listview,
        [b"Focus by TAB", b"Focus by click", b"Enter activates", b"Wheel scrolls", b"Viewport owns hitmap"],
    )
    objects.tuix_textarea_set_title(textarea, b"TextArea")
    objects.tuix_textarea_set_placeholder(textarea, b"Type here...")
    objects.tuix_textarea_set_text(textarea, b"Multimodal input is routed by the active focus target.")

    focus_order = [uid_choice, uid_list, uid_textarea]
    active_uid = uid_choice
    scenes.set_focus(SCENE, active_uid)

    chosen = None
    activated = None

    for frame in range(TIMEOUT_FRAMES + 1):
        snap = input.peek_snapshot()
        kb = snap.keyboard

        if kb and kb.has_event:
            if kb.code == ESC_KEY:
                break
            if kb.code == TAB_KEY:
                active_uid = _next_focus(active_uid, focus_order)
                scenes.set_focus(SCENE, active_uid)

        objects.tuix_progressbar_set_value(pb, frame / TIMEOUT_FRAMES)

        if chosen is None and objects.tuix_choice_is_confirmed(ch):
            chosen = int(objects.tuix_choice_get_result(ch))
            active_uid = uid_list
            scenes.set_focus(SCENE, active_uid)

        if activated is None and objects.tuix_listview_take_activated(listview):
            activated = objects.tuix_listview_get_selected(listview)
            active_uid = uid_textarea
            scenes.set_focus(SCENE, active_uid)

        engine.main_loop()
        sleep_ms(16)

        if chosen is not None and activated is not None:
            break

    print()
    if chosen is None:
        print("Choice: cancelled / timed out")
    else:
        print(f"Choice: option index {chosen}")

    if activated is None:
        print("ListView: cancelled / timed out")
    else:
        print(f"ListView: item index {activated}")

    raw_text = objects.tuix_textarea_get_text(textarea)
    if raw_text:
        print(f"TextArea: {raw_text.decode(errors='replace')!r}")

    buffers.free_buffer(SCENE, uid_pb)
    buffers.free_buffer(SCENE, uid_choice)
    buffers.free_buffer(SCENE, uid_list)
    buffers.free_buffer(SCENE, uid_textarea)
    input.stop()
    engine.shutdown()
    return 0


if __name__ == "__main__":
    main()
