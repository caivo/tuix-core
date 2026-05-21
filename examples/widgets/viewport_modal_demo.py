"""Viewport and modal widgets demo for Tuix v0.5.

Shows ListView, TextArea, Checkbox, Button, and Dialog APIs together.
TAB cycles focus, ENTER activates list/buttons, D opens the dialog, ESC closes or exits.
"""

import time

from tuix.core import builders, buffers, engine, input, objects, scenes


SCENE = b"ViewportModalDemo"
ESC_KEY = 0x1B
TAB_KEY = 0x09


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


def must_get_obj(uid, name):
    obj = objects.get_object_by_uid(uid)
    if obj is None:
        raise RuntimeError(f"failed to create {name}")
    return obj


def next_focus(current_uid, focus_order):
    try:
        index = focus_order.index(current_uid)
    except ValueError:
        return focus_order[0]
    return focus_order[(index + 1) % len(focus_order)]


def main():
    if engine.init() != 0:
        print("init failed")
        return 1

    uids = []
    dialog_open = False

    try:
        builders.register_standard()
        scenes.init_scene(SCENE)
        scenes.select_scene(SCENE)
        input.listen()

        title_uid = objects.create_object(builders.TEXT, SCENE, 0.86, 0.08, 0.05, 0.07)
        uids.append(title_uid)
        title = must_get_obj(title_uid, "title")
        objects.tuix_text_set_text(title, b"ListView + TextArea + Checkbox + Dialog")
        objects.tuix_text_set_fg(title, 230, 235, 255)

        list_uid = objects.create_object(builders.LISTVIEW, SCENE, 0.34, 0.58, 0.16, 0.07)
        notes_uid = objects.create_object(builders.TEXTAREA, SCENE, 0.5, 0.58, 0.16, 0.43)
        check_uid = objects.create_object(builders.CHECKBOX, SCENE, 0.34, 0.08, 0.78, 0.07)
        open_uid = objects.create_object(builders.BUTTON, SCENE, 0.22, 0.09, 0.78, 0.43)
        state_uid = objects.create_object(builders.BADGE, SCENE, 0.25, 0.09, 0.78, 0.68)
        uids.extend([list_uid, notes_uid, check_uid, open_uid, state_uid])

        listview = must_get_obj(list_uid, "listview")
        notes = must_get_obj(notes_uid, "textarea")
        checkbox = must_get_obj(check_uid, "checkbox")
        open_button = must_get_obj(open_uid, "open button")
        state = must_get_obj(state_uid, "state badge")

        objects.tuix_listview_set_title(listview, b"Tasks")
        objects.tuix_listview_set_items(
            listview,
            [
                b"Write README snippet",
                b"Check layout demo",
                b"Open modal dialog",
                b"Review viewport routing",
                b"Ship package",
            ],
        )
        objects.tuix_textarea_set_title(notes, b"Notes")
        objects.tuix_textarea_set_placeholder(notes, b"Type details here...")
        objects.tuix_textarea_set_text(notes, b"Select an item or press D to open a modal dialog.")
        objects.tuix_checkbox_set_label(checkbox, b"Close dialog on ESC")
        objects.tuix_checkbox_set_checked(checkbox, True)
        objects.tuix_button_set_label(open_button, b"Open Dialog")
        objects.tuix_badge_set_text(state, b"ready")
        objects.tuix_badge_set_colors(state, 25, 30, 42, 130, 220, 170)

        dialog_uid = objects.create_object(builders.DIALOG, SCENE, 0.68, 0.46, 0.25, 0.16)
        uids.append(dialog_uid)
        dialog = must_get_obj(dialog_uid, "dialog")
        objects.tuix_dialog_set_title(dialog, b"DialogBuilder")
        objects.tuix_dialog_set_body_size(dialog, 48, 10)
        objects.tuix_dialog_set_padding(dialog, 2, 1, 2, 1)
        objects.tuix_dialog_set_colors(dialog, 8, 10, 16, 120, 200, 240, 18, 24, 34)
        objects.tuix_dialog_set_close_on_backdrop(dialog, True)

        dialog_text_uid = objects.tuix_dialog_add_object(dialog, SCENE, builders.TEXT, 1.0, 1.0)
        uids.append(dialog_text_uid)
        dialog_text = must_get_obj(dialog_text_uid, "dialog text")
        objects.tuix_text_set_text(
            dialog_text,
            b"Modal input is owned by the active dialog.\nESC/backdrop can request close.",
        )
        objects.tuix_text_set_fg(dialog_text, 230, 235, 255)

        focus_order = [list_uid, notes_uid, check_uid, open_uid]
        active_uid = list_uid
        scenes.set_focus(SCENE, active_uid)

        while True:
            snap = input.peek_snapshot()
            kb = snap.keyboard
            if kb and kb.has_event:
                if kb.code == ESC_KEY and not dialog_open:
                    break
                if kb.code == TAB_KEY and not dialog_open:
                    active_uid = next_focus(active_uid, focus_order)
                    scenes.set_focus(SCENE, active_uid)
                if kb.code in (ord("d"), ord("D")) and not dialog_open:
                    objects.tuix_dialog_activate(dialog, SCENE)
                    dialog_open = True
                    objects.tuix_badge_set_text(state, b"modal")

            objects.tuix_dialog_set_close_on_esc(dialog, bool(objects.tuix_checkbox_get_checked(checkbox)))

            if objects.tuix_listview_take_activated(listview):
                idx = objects.tuix_listview_get_selected(listview)
                objects.tuix_textarea_set_text(notes, f"Activated task #{idx}".encode())

            if objects.tuix_button_take_pressed(open_button) and not dialog_open:
                objects.tuix_dialog_activate(dialog, SCENE)
                dialog_open = True
                objects.tuix_badge_set_text(state, b"modal")

            if dialog_open and objects.tuix_dialog_take_close_requested(dialog):
                objects.tuix_dialog_deactivate(dialog, SCENE)
                dialog_open = False
                scenes.set_focus(SCENE, active_uid)
                objects.tuix_badge_set_text(state, b"ready")

            engine.main_loop()
            sleep_ms(16)

        return 0
    except Exception as exc:
        print(f"demo error: {exc}")
        return 1
    finally:
        for uid in reversed(uids):
            buffers.free_buffer(SCENE, uid)
        input.stop()
        engine.shutdown()


if __name__ == "__main__":
    raise SystemExit(main())
