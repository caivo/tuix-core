from . import _lib
from . import _structs
import ctypes


def get_buffer_by_uid(uid: int):
    fn = _lib.get_func('tuix_get_buffer_by_uid', restype=None, argtypes=[ctypes.c_int])
    res = fn(uid)
    # If CDLL returned a ctypes pointer, return it directly
    if isinstance(res, ctypes._CFuncPtr) or hasattr(res, 'contents'):
        return res
    # If we got an integer address (from Cython wrapper), cast to pointer
    try:
        addr = int(res)
    except Exception:
        return None
    if addr == 0:
        return None
    return ctypes.cast(ctypes.c_void_p(addr), ctypes.POINTER(_structs.TuixBuffer))


def free_buffer(scene_name: bytes, uid: int):
    try:
        fn = _lib.get_func('tuix_free_buffer', restype=None, argtypes=[ctypes.c_char_p, ctypes.c_int])
        return fn(scene_name, uid)
    except AttributeError:
        # fallback: no-op
        return None


