"""
Multimodal focus-routing example.

Demonstrates keyboard + mouse driven focus switching between interactive widgets:
  - Choice widget (left)
  - Input widget (right)
  - Progressbar timeout (top)

Controls:
  - TAB: switch focus between Choice/Input
  - Left mouse click: switch focus between Choice/Input
  - ESC: exit

Only the focused interactive widget receives the input snapshot.
"""

import time
from tuix.core import engine, builders, scenes, registry, objects, buffers, input


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


SCENE = b"Main"
TAB_KEY = 0x09
ESC_KEY = 0x1B
TIMEOUT_FRAMES = 450


def _switch_focus(current_uid, uid_choice, uid_input):
    return uid_input if current_uid == uid_choice else uid_choice


def main():
    if engine.init() != 0:
        print("init failed")
        return 1

    builders.register_standard()
    scenes.init_scene(SCENE)
    registry.registry.current_scene_name = SCENE
    input.listen()

    uid_pb = objects.create_object(builders.PROGRESSBAR, SCENE, 0.9, 0.07, 0.05, 0.05)
    uid_choice = objects.create_object(builders.CHOICE, SCENE, 0.4, 0.45, 0.2, 0.05)
    uid_input = objects.create_object(builders.INPUT, SCENE, 0.4, 0.1, 0.4, 0.55)

    pb_ptr = buffers.get_buffer_by_uid(uid_pb)
    ch_ptr = buffers.get_buffer_by_uid(uid_choice)
    in_ptr = buffers.get_buffer_by_uid(uid_input)

    pb = pb_ptr.contents.obj.contents
    ch = ch_ptr.contents.obj.contents
    inp = in_ptr.contents.obj.contents

    objects.tuix_progressbar_set_style(pb, ord('>'), ord('-'), 80, 200, 80, 40, 40, 40)
    objects.tuix_choice_set_options(ch, [b"Option A", b"Option B", b"Option C", b"Option D"])
    objects.tuix_input_set_placeholder(inp, b"Type text and press Enter...")

    active_uid = uid_choice
    scenes.set_focus(SCENE, active_uid)

    chosen = None
    entered = None

    for frame in range(TIMEOUT_FRAMES + 1):
        snap = input.get_snapshot()
        kb = snap.keyboard
        ms = snap.mouse

        if kb and kb.has_event:
            if kb.code == ESC_KEY:
                break
            if kb.code == TAB_KEY:
                active_uid = _switch_focus(active_uid, uid_choice, uid_input)
                scenes.set_focus(SCENE, active_uid)

        if ms and ms.has_event and ms.btn == 1:
            active_uid = _switch_focus(active_uid, uid_choice, uid_input)
            scenes.set_focus(SCENE, active_uid)

        objects.tuix_progressbar_set_value(pb, frame / TIMEOUT_FRAMES)

        if chosen is None and active_uid == uid_choice and not objects.tuix_choice_is_confirmed(ch):
            objects.tuix_choice_feed_input(ch, snap)
        if chosen is None and objects.tuix_choice_is_confirmed(ch):
            chosen = int(objects.tuix_choice_get_result(ch))
            active_uid = uid_input
            scenes.set_focus(SCENE, active_uid)

        if entered is None and active_uid == uid_input and not objects.tuix_input_is_submitted(inp):
            objects.tuix_input_feed_input(inp, snap)
        if entered is None and objects.tuix_input_is_submitted(inp):
            raw = objects.tuix_input_get_result(inp)
            entered = raw.decode(errors="replace") if raw else ""

        engine.main_loop()
        sleep_ms(16)

        if chosen is not None and entered is not None:
            break

    print()
    if chosen is None:
        print("Choice: cancelled / timed out")
    else:
        print(f"Choice: option index {chosen}")

    if entered is None:
        print("Input: cancelled / timed out")
    else:
        print(f"Input: {entered!r}")

    buffers.free_buffer(SCENE, uid_pb)
    buffers.free_buffer(SCENE, uid_choice)
    buffers.free_buffer(SCENE, uid_input)
    input.stop()
    engine.shutdown()
    return 0


if __name__ == "__main__":
    main()
