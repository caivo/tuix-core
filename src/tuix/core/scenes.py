from . import _lib
import ctypes


def init_scene(name: bytes):
    fn = _lib.get_func('tuix_init_scene', restype=ctypes.c_int, argtypes=[ctypes.c_char_p])
    return fn(name)


def free_scene(name: bytes):
    fn = _lib.get_func('tuix_free_scene', restype=ctypes.c_int, argtypes=[ctypes.c_char_p])
    return fn(name)


def clear_scene(name: bytes):
    fn = _lib.get_func('tuix_clear_scene', restype=ctypes.c_int, argtypes=[ctypes.c_char_p])
    return fn(name)
