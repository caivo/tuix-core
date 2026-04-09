from . import _lib
from . import _structs
import ctypes

_registry = None
if _lib.lib is not None:
    try:
        _registry = _structs.TuixRegistry.in_dll(_lib.lib, 'tuix_registry')
    except Exception:
        pass

class RegistryProxy:
    def __init__(self, native_registry=None):
        self._native = native_registry
        self._name = None

    def __getattr__(self, name):
        if self._native is not None:
            return getattr(self._native, name)
        raise AttributeError(name)

    def __setattr__(self, name, value):
        if name in ('_native', '_name'):
            object.__setattr__(self, name, value)
            return
        if name == 'current_scene_name':
            RegistryProxy.current_scene_name.fset(self, value)
            return
        if self._native is not None:
            setattr(self._native, name, value)
            return
        object.__setattr__(self, name, value)

    @property
    def current_scene_name(self):
        if self._native is not None:
            return self._native.current_scene_name
        return self._name

    @current_scene_name.setter
    def current_scene_name(self, val):
        if isinstance(val, str):
            val = val.encode('utf-8')

        # Keep a Python-owned copy to avoid dangling pointers in fallback paths.
        self._name = val

        # Prefer tuix_select_scene which stores the engine-owned interned pointer.
        fn = getattr(_lib._mod, 'tuix_select_scene', None)
        if fn is not None and val is not None:
            try:
                rc = fn(val)
                if rc == 0 and self._native is not None:
                    self._name = self._native.current_scene_name
                    return
            except Exception:
                pass

        # Fall back to direct setter exposed by Cython.
        set_fn = getattr(_lib._mod, 'set_current_scene_name', None)
        if set_fn is not None:
            try:
                set_fn(val)
            except Exception:
                pass

        if self._native is not None:
            self._native.current_scene_name = val


registry = RegistryProxy(_registry)
