from . import _lib
from . import _structs
import ctypes


def _to_bytes(name):
    if isinstance(name, str):
        return name.encode("utf-8")
    return name


def free_buffer(scene_name: bytes, uid: int):
    scene_name = _to_bytes(scene_name)
    try:
        fn = _lib.get_func("tuix_free_buffer", restype=None, argtypes=[ctypes.c_char_p, ctypes.c_int])
        return fn(scene_name, uid)
    except AttributeError:
        return None


def set_buffer_parent(scene_name: bytes, uid: int, parent_uid: int) -> int:
    scene_name = _to_bytes(scene_name)
    try:
        from . import _tuix_cy
        return _tuix_cy.tuix_set_buffer_parent(scene_name, uid, parent_uid)
    except (ImportError, AttributeError):
        fn = _lib.get_func(
            "tuix_set_buffer_parent",
            restype=ctypes.c_int,
            argtypes=[ctypes.c_char_p, ctypes.c_int, ctypes.c_int],
        )
        return fn(scene_name, uid, parent_uid) if fn else -1


def get_buffer_parent(scene_name: bytes, uid: int) -> int:
    scene_name = _to_bytes(scene_name)
    return int(_lib._mod.tuix_get_buffer_parent(scene_name, uid))


def get_buffer_z_index(scene_name: bytes, uid: int) -> int:
    scene_name = _to_bytes(scene_name)
    try:
        from . import _tuix_cy
        return _tuix_cy.tuix_get_buffer_z_index(scene_name, uid)
    except (ImportError, AttributeError):
        fn = _lib.get_func(
            "tuix_get_buffer_z_index",
            restype=ctypes.c_int,
            argtypes=[ctypes.c_char_p, ctypes.c_int],
        )
        return fn(scene_name, uid) if fn else -1


def set_buffer_z_index(scene_name: bytes, uid: int, z_index: int) -> int:
    scene_name = _to_bytes(scene_name)
    try:
        from . import _tuix_cy
        return _tuix_cy.tuix_set_buffer_z_index(scene_name, uid, z_index)
    except (ImportError, AttributeError):
        fn = _lib.get_func(
            "tuix_set_buffer_z_index",
            restype=ctypes.c_int,
            argtypes=[ctypes.c_char_p, ctypes.c_int, ctypes.c_int],
        )
        return fn(scene_name, uid, z_index) if fn else -1


def set_buffer_layout_slot_by_uid(
    uid: int,
    *,
    grow: float = 0.0,
    shrink: float = 1.0,
    basis: int = -1,
    min_w: int = 0,
    min_h: int = 0,
    max_w: int = -1,
    max_h: int = -1,
    align_self: int = -1,
    grid_row: int = 0,
    grid_col: int = 0,
    row_span: int = 1,
    col_span: int = 1,
) -> int:
    return int(
        _lib._mod.tuix_set_buffer_layout_slot_by_uid(
            uid,
            float(grow),
            float(shrink),
            basis,
            min_w,
            min_h,
            max_w,
            max_h,
            align_self,
            grid_row,
            grid_col,
            row_span,
            col_span,
        )
    )


def get_buffer_layout_slot_by_uid(uid: int) -> dict:
    return _lib._mod.tuix_get_buffer_layout_slot_by_uid(uid)


def set_buffer_layout_rect_by_uid(uid: int, offset_left: int, offset_top: int, width: int, height: int) -> int:
    return int(_lib._mod.tuix_set_buffer_layout_rect_by_uid(uid, offset_left, offset_top, width, height))


def clear_buffer_layout_rect_by_uid(uid: int) -> int:
    return int(_lib._mod.tuix_clear_buffer_layout_rect_by_uid(uid))


def set_buffer_grid_placement_by_uid(uid: int, row: int, col: int, row_span: int = 1, col_span: int = 1) -> int:
    return int(_lib._mod.tuix_set_buffer_grid_placement_by_uid(uid, row, col, row_span, col_span))


def get_buffer_snapshot(scene_name: bytes, uid: int) -> dict:
    scene_name = _to_bytes(scene_name)
    try:
        from . import _tuix_cy
        return _tuix_cy.tuix_get_buffer_snapshot(scene_name, uid)
    except (ImportError, AttributeError):
        return None


def get_buffer_snapshot_by_uid(uid: int) -> dict:
    try:
        from . import _tuix_cy
        return _tuix_cy.tuix_get_buffer_snapshot_by_uid(uid)
    except (ImportError, AttributeError):
        return None
