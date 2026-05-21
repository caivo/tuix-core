"""Nested layout demo for Tuix v0.5.

Shows Row, Column, SplitPane, Grid, layout slots, and grid placement.
TAB cycles focus between the ListView, TextArea, and Checkbox. ESC exits.
"""

import time

from tuix.core import builders, buffers, engine, input, objects, scenes


SCENE = b"LayoutDemo"
ESC_KEY = 0x1B
TAB_KEY = 0x09


def sleep_ms(ms):
    time.sleep(ms / 1000.0)


def must_get_obj(uid, name):
    obj = objects.get_object_by_uid(uid)
    if obj is None:
        raise RuntimeError(f"failed to create {name}")
    return obj


def set_text(uid, text, fg=(220, 230, 255), bg=None):
    obj = must_get_obj(uid, text.decode(errors="replace"))
    objects.tuix_text_set_text(obj, text)
    objects.tuix_text_set_fg(obj, *fg)
    if bg is None:
        objects.tuix_text_clear_bg(obj)
    else:
        objects.tuix_text_set_bg(obj, *bg)
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

    try:
        builders.register_standard()
        scenes.init_scene(SCENE)
        scenes.select_scene(SCENE)
        input.listen()

        header_uid = objects.create_object(builders.ROW, SCENE, 0.9, 0.13, 0.04, 0.05)
        uids.append(header_uid)
        header = must_get_obj(header_uid, "header row")
        objects.tuix_stack_set_gap(header, 2)
        objects.tuix_stack_set_padding(header, 1, 1, 1, 1)
        objects.tuix_stack_set_align(header, builders.ALIGN_STRETCH)
        objects.tuix_stack_set_bg(header, 18, 24, 34)

        title_uid = objects.tuix_stack_add_object(header, SCENE, builders.TEXT, 1.0, 1.0)
        status_uid = objects.tuix_stack_add_object(header, SCENE, builders.TEXT, 1.0, 1.0)
        uids.extend([title_uid, status_uid])
        buffers.set_buffer_layout_slot_by_uid(title_uid, grow=1.0, min_w=24)
        buffers.set_buffer_layout_slot_by_uid(status_uid, basis=30, min_w=18, align_self=builders.ALIGN_CENTER)
        set_text(title_uid, b"RowBuilder: title grows, status keeps a fixed basis")
        status = set_text(status_uid, b"focus: ListView", fg=(255, 220, 120))

        split_uid = objects.create_object(builders.SPLITPANE, SCENE, 0.9, 0.48, 0.2, 0.05)
        uids.append(split_uid)
        split = must_get_obj(split_uid, "splitpane")
        objects.tuix_splitpane_set_orientation(split, builders.AXIS_HORIZONTAL)
        objects.tuix_splitpane_set_split_ratio(split, 0.42)
        objects.tuix_splitpane_set_divider_size(split, 1)
        objects.tuix_splitpane_set_min_sizes(split, 18, 24)
        objects.tuix_splitpane_set_colors(split, 120, 200, 240, 30, 38, 52, 8, 10, 14)

        list_uid = objects.tuix_splitpane_add_object(split, SCENE, builders.LISTVIEW, 1.0, 1.0)
        notes_uid = objects.tuix_splitpane_add_object(split, SCENE, builders.TEXTAREA, 1.0, 1.0)
        uids.extend([list_uid, notes_uid])

        listview = must_get_obj(list_uid, "listview")
        objects.tuix_listview_set_title(listview, b"ListView")
        objects.tuix_listview_set_items(
            listview,
            [
                b"LayoutSlot grow/shrink",
                b"Row + Column stack",
                b"SplitPane ratio",
                b"Grid fixed/weighted tracks",
                b"Viewport widgets",
            ],
        )

        notes = must_get_obj(notes_uid, "textarea")
        objects.tuix_textarea_set_title(notes, b"TextArea")
        objects.tuix_textarea_set_text(
            notes,
            b"SplitPaneBuilder owns two children here.\n"
            b"The left child is a ListView, the right child is a TextArea.\n"
            b"TAB cycles focus through viewport-aware widgets.",
        )
        objects.tuix_textarea_set_placeholder(notes, b"Type notes...")

        grid_uid = objects.create_object(builders.GRID, SCENE, 0.9, 0.22, 0.72, 0.05)
        uids.append(grid_uid)
        grid = must_get_obj(grid_uid, "grid")
        objects.tuix_grid_set_columns(
            grid,
            [
                (builders.GRID_TRACK_FIXED, 24),
                (builders.GRID_TRACK_WEIGHT, 1),
                (builders.GRID_TRACK_FIXED, 22),
            ],
        )
        objects.tuix_grid_set_rows(grid, [(builders.GRID_TRACK_FIXED, 3), (builders.GRID_TRACK_WEIGHT, 1)])
        objects.tuix_grid_set_padding(grid, 1, 1, 1, 1)
        objects.tuix_grid_set_gaps(grid, 2, 1)
        objects.tuix_grid_set_bg(grid, 12, 16, 22)

        check_uid = objects.tuix_grid_add_object(grid, SCENE, builders.CHECKBOX, 1.0, 1.0)
        progress_uid = objects.tuix_grid_add_object(grid, SCENE, builders.PROGRESSBAR, 1.0, 1.0)
        badge_uid = objects.tuix_grid_add_object(grid, SCENE, builders.BADGE, 1.0, 1.0)
        help_uid = objects.tuix_grid_add_object(grid, SCENE, builders.TEXT, 1.0, 1.0)
        uids.extend([check_uid, progress_uid, badge_uid, help_uid])

        buffers.set_buffer_grid_placement_by_uid(check_uid, row=0, col=0)
        buffers.set_buffer_grid_placement_by_uid(progress_uid, row=0, col=1)
        buffers.set_buffer_grid_placement_by_uid(badge_uid, row=0, col=2)
        buffers.set_buffer_grid_placement_by_uid(help_uid, row=1, col=0, row_span=1, col_span=3)

        checkbox = must_get_obj(check_uid, "checkbox")
        progress = must_get_obj(progress_uid, "progressbar")
        badge = must_get_obj(badge_uid, "badge")
        objects.tuix_checkbox_set_label(checkbox, b"Grid child")
        objects.tuix_checkbox_set_checked(checkbox, True)
        objects.tuix_progressbar_set_style(progress, b"=", b".", 80, 210, 140, 55, 60, 70)
        objects.tuix_progressbar_show_percentage(progress, True)
        objects.tuix_badge_set_text(badge, b"v0.5 layout")
        objects.tuix_badge_set_colors(badge, 18, 22, 30, 255, 220, 110)
        set_text(help_uid, b"GridBuilder: fixed + weighted columns, child placement spans this row.")

        focus_order = [list_uid, notes_uid, check_uid]
        active_uid = list_uid
        scenes.set_focus(SCENE, active_uid)

        frame = 0
        while True:
            snap = input.peek_snapshot()
            kb = snap.keyboard
            if kb and kb.has_event:
                if kb.code == ESC_KEY:
                    break
                if kb.code == TAB_KEY:
                    active_uid = next_focus(active_uid, focus_order)
                    scenes.set_focus(SCENE, active_uid)
                    label = {
                        list_uid: b"focus: ListView",
                        notes_uid: b"focus: TextArea",
                        check_uid: b"focus: Checkbox",
                    }[active_uid]
                    objects.tuix_text_set_text(status, label)

            if objects.tuix_listview_take_activated(listview):
                idx = objects.tuix_listview_get_selected(listview)
                objects.tuix_textarea_set_text(notes, f"Activated list item #{idx}".encode())

            if objects.tuix_checkbox_take_changed(checkbox):
                checked = objects.tuix_checkbox_get_checked(checkbox)
                objects.tuix_badge_set_text(badge, b"checked" if checked else b"unchecked")

            objects.tuix_progressbar_set_value(progress, (frame % 180) / 179.0)
            engine.main_loop()
            sleep_ms(16)
            frame += 1

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
