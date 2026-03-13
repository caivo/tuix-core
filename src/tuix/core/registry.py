from . import _lib
from . import _structs
import ctypes

_registry = None
if _lib.lib is not None:
    try:
        _registry = _structs.TuixRegistry.in_dll(_lib.lib, 'tuix_registry')
    except Exception:
        pass

if _registry is None:
    class RegistryProxy:
        def __init__(self):
            self._name = None

        @property
        def current_scene_name(self):
            return self._name

        @current_scene_name.setter
        def current_scene_name(self, val):
            if isinstance(val, str):
                val = val.encode('utf-8')
            # Prefer tuix_select_scene which sets the scene active and stores
            # the engine's own interned pointer (safe lifetime).
            # Fall back to set_current_scene_name which at least sets the pointer.
            fn = getattr(_lib._mod, 'tuix_select_scene', None)
            if fn is not None:
                fn(val)
            else:
                getattr(_lib._mod, 'set_current_scene_name', lambda x: None)(val)
            self._name = val

    registry = RegistryProxy()
else:
    registry = _registry

