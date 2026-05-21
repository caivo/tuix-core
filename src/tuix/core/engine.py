from . import _lib


def init():
    try:
        fn = _lib.get_func("tuix_init", restype=int, argtypes=None)
        rc = fn()
    except AttributeError:
        rc = getattr(_lib._mod, "init", lambda: 0)()

    try:
        fn = _lib.get_func("tuix_init_registry", restype=int, argtypes=None)
        fn()
    except AttributeError:
        try:
            getattr(_lib._mod, "init_registry", lambda: 0)()
        except Exception:
            pass
    return rc


def shutdown():
    try:
        fn = _lib.get_func("tuix_shutdown", restype=int, argtypes=None)
        return fn()
    except AttributeError:
        return getattr(_lib._mod, "shutdown", lambda: 0)()


def main_loop():
    if hasattr(_lib._mod, "main_loop"):
        return _lib._mod.main_loop()
    fn = _lib.get_func("tuix_main_loop", restype=None, argtypes=None)
    return fn()


def get_core_loop_stats():
    getter = getattr(_lib._mod, "get_core_loop_stats", None)
    if getter is not None:
        return getter()
    return {}


def mouse_capture_begin(uid: int) -> int:
    return int(_lib._mod.tuix_mouse_capture_begin(int(uid)))


def mouse_capture_end(uid: int) -> int:
    return int(_lib._mod.tuix_mouse_capture_end(int(uid)))


def get_mouse_capture_uid() -> int:
    return int(_lib._mod.tuix_get_mouse_capture_uid())
