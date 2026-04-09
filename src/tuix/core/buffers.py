from . import _lib
import ctypes


def free_buffer(scene_name: bytes, uid: int):
    try:
        fn = _lib.get_func('tuix_free_buffer', restype=None, argtypes=[ctypes.c_char_p, ctypes.c_int])
        return fn(scene_name, uid)
    except AttributeError:
        # fallback: no-op
        return None


def set_buffer_parent(scene_name: bytes, uid: int, parent_uid: int) -> int:
    """Set the parent buffer for a given buffer (for hierarchy)."""
    try:
        # Try Cython wrapper first
        from . import _tuix_cy
        return _tuix_cy.tuix_set_buffer_parent(scene_name, uid, parent_uid)
    except (ImportError, AttributeError):
        # Fallback to ctypes
        fn = _lib.get_func('tuix_set_buffer_parent', restype=ctypes.c_int, 
                           argtypes=[ctypes.c_char_p, ctypes.c_int, ctypes.c_int])
        return fn(scene_name, uid, parent_uid) if fn else -1


def get_buffer_z_index(scene_name: bytes, uid: int) -> int:
    """Get the z-index of a buffer."""
    try:
        # Try Cython wrapper first
        from . import _tuix_cy
        return _tuix_cy.tuix_get_buffer_z_index(scene_name, uid)
    except (ImportError, AttributeError):
        # Fallback to ctypes
        fn = _lib.get_func('tuix_get_buffer_z_index', restype=ctypes.c_int,
                           argtypes=[ctypes.c_char_p, ctypes.c_int])
        return fn(scene_name, uid) if fn else -1


def set_buffer_z_index(scene_name: bytes, uid: int, z_index: int) -> int:
    """Set the z-index of a buffer."""
    try:
        # Try Cython wrapper first
        from . import _tuix_cy
        return _tuix_cy.tuix_set_buffer_z_index(scene_name, uid, z_index)
    except (ImportError, AttributeError):
        # Fallback to ctypes
        fn = _lib.get_func('tuix_set_buffer_z_index', restype=ctypes.c_int,
                           argtypes=[ctypes.c_char_p, ctypes.c_int, ctypes.c_int])
        return fn(scene_name, uid, z_index) if fn else -1


def get_buffer_snapshot(scene_name: bytes, uid: int) -> dict:
    """Get a snapshot (read-only copy) of a buffer."""
    try:
        # Try Cython wrapper first
        from . import _tuix_cy
        return _tuix_cy.tuix_get_buffer_snapshot(scene_name, uid)
    except (ImportError, AttributeError):
        # Fallback to ctypes - not easily doable for struct returns, return None
        return None


def get_buffer_snapshot_by_uid(uid: int) -> dict:
    """Get a snapshot (read-only copy) of a buffer by UID."""
    try:
        # Try Cython wrapper first
        from . import _tuix_cy
        return _tuix_cy.tuix_get_buffer_snapshot_by_uid(uid)
    except (ImportError, AttributeError):
        # Fallback to ctypes - not easily doable for struct returns, return None
        return None

