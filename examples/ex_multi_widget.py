"""
Multi-widget layout example.

Three widgets are live on screen at the same time:
  - a progressbar ticking up automatically (top)
  - a choice menu (centre-left)
  - a text input (centre-right)

The progressbar acts as a countdown — when it fills the session ends.
Confirming the choice or submitting the input also ends that widget early.
Press ESC to quit at any time.

NOTE (v0.2 known limitation): both the choice and input widgets receive the
same input snapshot on every frame, so keystrokes are processed by both
simultaneously. Proper focus/routing will be addressed in v0.2.1.
"""

import time
from tuix.core import engine, builders, scenes, registry, objects, buffers, input


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


TIMEOUT_FRAMES = 300   # ~5 s at 60 fps


def main():
    if engine.init() != 0:
        print("init failed")
        return 1

    builders.register_standard()
    scenes.init_scene(b"Main")
    registry.registry.current_scene_name = b"Main"
    input.listen()

    # progressbar — thin strip at the top
    uid_pb = objects.create_object(builders.PROGRESSBAR, b"Main", 0.9, 0.07, 0.05, 0.05)
    # choice — left half, middle
    uid_ch = objects.create_object(builders.CHOICE,      b"Main", 0.4, 0.45, 0.2,  0.05)
    # input — right half, middle
    uid_in = objects.create_object(builders.INPUT,       b"Main", 0.4, 0.1,  0.4,  0.55)

    pb_ptr = buffers.get_buffer_by_uid(uid_pb)
    ch_ptr = buffers.get_buffer_by_uid(uid_ch)
    in_ptr = buffers.get_buffer_by_uid(uid_in)

    pb = pb_ptr.contents.obj.contents
    ch = ch_ptr.contents.obj.contents
    inp = in_ptr.contents.obj.contents

    objects.tuix_progressbar_set_style(pb, ord('>'), ord('-'), 80, 200, 80, 40, 40, 40)
    objects.tuix_choice_set_options(ch, [b"Option A", b"Option B", b"Option C", b"Option D"])
    objects.tuix_input_set_placeholder(inp, b"Enter a value...")

    chosen  = None
    entered = None
    frame   = 0

    while frame <= TIMEOUT_FRAMES:
        snap = input.get_snapshot()
        kb   = snap.keyboard
        if kb and kb.has_event and kb.code == 0x1B:
            break

        objects.tuix_progressbar_set_value(pb, frame / TIMEOUT_FRAMES)

        if chosen is None and not objects.tuix_choice_is_confirmed(ch):
            objects.tuix_choice_feed_input(ch, snap)
        elif chosen is None and objects.tuix_choice_is_confirmed(ch):
            chosen = int(objects.tuix_choice_get_result(ch))

        if entered is None and not objects.tuix_input_is_submitted(inp):
            objects.tuix_input_feed_input(inp, snap)
        elif entered is None and objects.tuix_input_is_submitted(inp):
            result = objects.tuix_input_get_result(inp)
            entered = result.decode(errors="replace") if result else ""

        engine.main_loop()
        sleep_ms(16)
        frame += 1

        # stop early once both widgets are done
        if chosen is not None and entered is not None:
            engine.main_loop()
            sleep_ms(400)
            break

    # print a clean summary below the TUI output
    print()
    if chosen is not None:
        print(f"Choice: option index {chosen}")
    else:
        print("Choice: cancelled / timed out")
    if entered is not None:
        print(f"Input:  {entered!r}")
    else:
        print("Input:  cancelled / timed out")

    buffers.free_buffer(b"Main", uid_pb)
    buffers.free_buffer(b"Main", uid_ch)
    buffers.free_buffer(b"Main", uid_in)
    input.stop()
    engine.shutdown()
    return 0


if __name__ == "__main__":
    main()
