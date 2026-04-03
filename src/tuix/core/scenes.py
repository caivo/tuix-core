from . import _lib
import ctypes


def _to_bytes(name):
    if isinstance(name, str):
        return name.encode("utf-8")
    return name


def init_scene(name: bytes):
    name = _to_bytes(name)
    fn = _lib.get_func('tuix_init_scene', restype=ctypes.c_int, argtypes=[ctypes.c_char_p])
    return fn(name)


def free_scene(name: bytes):
    name = _to_bytes(name)
    fn = _lib.get_func('tuix_free_scene', restype=ctypes.c_int, argtypes=[ctypes.c_char_p])
    return fn(name)


def clear_scene(name: bytes):
    name = _to_bytes(name)
    fn = _lib.get_func('tuix_clear_scene', restype=ctypes.c_int, argtypes=[ctypes.c_char_p])
    return fn(name)


def get_scene(name: bytes) -> int:
    name = _to_bytes(name)
    return int(_lib._mod.tuix_get_scene(name))


def get_scenes():
    return _lib._mod.tuix_get_scenes()


def set_focus(scene_name: bytes, uid: int) -> int:
    scene_name = _to_bytes(scene_name)
    return int(_lib._mod.tuix_scene_set_focus(scene_name, int(uid)))


def set_previous_focus(scene_name: bytes) -> int:
    scene_name = _to_bytes(scene_name)
    return int(_lib._mod.tuix_scene_set_previous_focus(scene_name))
