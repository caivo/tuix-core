from . import _lib
from . import _structs
import ctypes
def listen():
    try:
        fn = _lib.get_func('listen_input', restype=None, argtypes=None)
        return fn()
    except AttributeError:
        # fallback: try Cython wrapper
        try:
            return getattr(_lib._mod, 'listen_input', lambda: None)()
        except Exception:
            return None


def stop():
    try:
        fn = _lib.get_func('stop_input_listening', restype=None, argtypes=None)
        return fn()
    except AttributeError:
        try:
            return getattr(_lib._mod, 'stop_input_listening', lambda: None)()
        except Exception:
            return None


def get_snapshot():
    return _lib._mod.get_input_snapshot()


