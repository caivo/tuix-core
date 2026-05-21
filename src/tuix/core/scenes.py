from . import _lib
import ctypes


def _to_bytes(name):
    if isinstance(name, str):
        return name.encode("utf-8")
    return name


def init_scene(name: bytes):
    name = _to_bytes(name)
    fn = _lib.get_func("tuix_init_scene", restype=ctypes.c_int, argtypes=[ctypes.c_char_p])
    return fn(name)


def free_scene(name: bytes):
    name = _to_bytes(name)
    fn = _lib.get_func("tuix_free_scene", restype=None, argtypes=[ctypes.c_char_p])
    return fn(name)


def clear_scene(name: bytes):
    name = _to_bytes(name)
    fn = _lib.get_func("tuix_clear_scene", restype=None, argtypes=[ctypes.c_char_p])
    return fn(name)


def get_scene(name: bytes) -> int:
    return int(_lib._mod.tuix_get_scene(_to_bytes(name)))


def get_scenes():
    return _lib._mod.tuix_get_scenes()


def select_scene(name: bytes) -> int:
    return int(_lib._mod.tuix_select_scene(_to_bytes(name)))


def set_focus(scene_name: bytes, uid: int) -> int:
    return int(_lib._mod.tuix_scene_set_focus(_to_bytes(scene_name), int(uid)))


def set_previous_focus(scene_name: bytes) -> int:
    return int(_lib._mod.tuix_scene_set_previous_focus(_to_bytes(scene_name)))


def activate_modal(scene_name: bytes, uid: int) -> int:
    return int(_lib._mod.tuix_scene_activate_modal(_to_bytes(scene_name), int(uid)))


def deactivate_modal(scene_name: bytes, uid: int) -> int:
    return int(_lib._mod.tuix_scene_deactivate_modal(_to_bytes(scene_name), int(uid)))


def get_active_modal(scene_name: bytes) -> int:
    return int(_lib._mod.tuix_scene_get_active_modal(_to_bytes(scene_name)))


def begin_transaction(scene_name: bytes) -> int:
    return int(_lib._mod.tuix_scene_begin_transaction(_to_bytes(scene_name)))


def commit_transaction(scene_name: bytes) -> int:
    return int(_lib._mod.tuix_scene_commit_transaction(_to_bytes(scene_name)))


def get_scene_stats(scene_name: bytes) -> dict:
    scene_name = _to_bytes(scene_name)
    try:
        from . import _tuix_cy
        return _tuix_cy.tuix_scene_get_stats(scene_name)
    except (ImportError, AttributeError):
        return None


def compact_scene_pixels(scene_name: bytes) -> int:
    scene_name = _to_bytes(scene_name)
    try:
        from . import _tuix_cy
        return _tuix_cy.tuix_compact_scene_pixels(scene_name)
    except (ImportError, AttributeError):
        fn = _lib.get_func("tuix_compact_scene_pixels", restype=ctypes.c_size_t, argtypes=[ctypes.c_char_p])
        return fn(scene_name) if fn else 0


def compact_cold_scenes(cold_frames: int, min_pixel_bytes: int, keep_active_scene: bool = True) -> int:
    try:
        from . import _tuix_cy
        return _tuix_cy.tuix_compact_cold_scenes(cold_frames, min_pixel_bytes, 1 if keep_active_scene else 0)
    except (ImportError, AttributeError):
        fn = _lib.get_func(
            "tuix_compact_cold_scenes",
            restype=ctypes.c_int,
            argtypes=[ctypes.c_ulonglong, ctypes.c_size_t, ctypes.c_int],
        )
        return fn(cold_frames, min_pixel_bytes, 1 if keep_active_scene else 0) if fn else 0
