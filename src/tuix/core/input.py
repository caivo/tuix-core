from . import _lib
from . import _structs
import ctypes

_LISTEN_INPUT = getattr(_lib._mod, 'listen_input', None)
_STOP_INPUT_LISTENING = getattr(_lib._mod, 'stop_input_listening', None)
_GET_INPUT_SNAPSHOT = getattr(_lib._mod, 'get_input_snapshot', None)
_PEEK_INPUT_SNAPSHOT = getattr(_lib._mod, 'peek_input_snapshot', None)

def listen():
    if _LISTEN_INPUT is not None:
        return _LISTEN_INPUT()
    try:
        fn = _lib.get_func('listen_input', restype=None, argtypes=None)
        return fn()
    except AttributeError:
        return None


def stop():
    if _STOP_INPUT_LISTENING is not None:
        return _STOP_INPUT_LISTENING()
    try:
        fn = _lib.get_func('stop_input_listening', restype=None, argtypes=None)
        return fn()
    except AttributeError:
        return None


def get_snapshot():
    if _GET_INPUT_SNAPSHOT is not None:
        return _GET_INPUT_SNAPSHOT()
    return _lib._mod.get_input_snapshot()


def peek_snapshot():
    if _PEEK_INPUT_SNAPSHOT is not None:
        return _PEEK_INPUT_SNAPSHOT()
    getter = getattr(_lib._mod, 'peek_input_snapshot', None)
    if getter is not None:
        return getter()
    return get_snapshot()


