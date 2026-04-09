from . import _lib

def init():
    # call tuix_init and init_registry if available
    try:
        fn = _lib.get_func('tuix_init', restype=int, argtypes=None)
        r = fn()
    except AttributeError:
        # fallback to Cython wrapper
        r = getattr(_lib._mod, 'init', lambda: 0)()
    # init registry
    try:
        fn2 = _lib.get_func('tuix_init_registry', restype=int, argtypes=None)
        fn2()
    except AttributeError:
        try:
            getattr(_lib._mod, 'init_registry', lambda: 0)()
        except Exception:
            pass
    return r

def shutdown():
    try:
        fn = _lib.get_func('tuix_shutdown', restype=int, argtypes=None)
        return fn()
    except AttributeError:
        return getattr(_lib._mod, 'shutdown', lambda: 0)()

def main_loop():
    # prefer Cython-exposed fast main_loop
    if hasattr(_lib._mod, 'main_loop'):
        return _lib._mod.main_loop()
    fn = _lib.get_func('tuix_main_loop', restype=None, argtypes=None)
    return fn()


def get_core_loop_stats():
    getter = getattr(_lib._mod, 'get_core_loop_stats', None)
    if getter is not None:
        return getter()
    return {}

