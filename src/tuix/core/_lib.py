"""Enhanced loader for the compiled Cython extension.

Exposes:
- `_mod`: the imported Cython module (tuix.core._tuix_cy)
- `lib`: a ctypes.CDLL handle to the compiled `.pyd` for raw symbol access
- `get_func(name, restype, argtypes)`: helper to get a ctypes callable
"""
import ctypes
import importlib
import os
import sys

try:
	_mod = importlib.import_module("tuix.core._tuix_cy")
except Exception as _err:
	raise ImportError(
		"Cython wrapper 'tuix.core._tuix_cy' not available; build the extension (python -m pip install -e .)"
	) from _err

# Attempt to load the compiled module as a CDLL for raw symbol access
_lib_path = getattr(_mod, "__file__", None)
_cdll = None
if _lib_path and os.path.exists(_lib_path):
	try:
		_cdll = ctypes.CDLL(_lib_path)
	except Exception:
		_cdll = None

def get_func(name, restype=ctypes.c_int, argtypes=None):
	"""Return a ctypes callable for `name` looking first on the Cython module,
	then on the loaded CDLL. Set `restype` and `argtypes` if found on CDLL."""
	# Prefer attribute on the Cython module (fast, no ctypes), then CDLL symbol
	if hasattr(_mod, name):
		return getattr(_mod, name)
	if _cdll is not None and hasattr(_cdll, name):
		fn = getattr(_cdll, name)
		try:
			fn.restype = restype
			if argtypes is not None:
				fn.argtypes = argtypes
		except Exception:
			pass
		return fn
	raise AttributeError(f"native symbol not found: {name}")

# Exported handles
lib = _cdll
__all__ = ["_mod", "lib", "get_func"]


def tuix_render_streaming(buf):
	return _mod.py_tuix_render_streaming(buf)


def tuix_create_native_buffer(py_buf) -> int:
	"""Create a native TuixFinalBuffer from `py_buf` and return an integer handle."""
	return int(_mod.py_tuix_create_native_buffer(py_buf))


def tuix_free_native_buffer(handle: int):
	"""Free a native buffer previously created by `tuix_create_native_buffer`."""
	return _mod.py_tuix_free_native_buffer(int(handle))


def tuix_apply_patch_and_render(handle: int, patch: bytes) -> bytes:
	"""Apply a binary patch (20-byte records) to the native buffer and render.
	Returns the rendered ANSI bytes.
	"""
	return _mod.py_tuix_apply_patch_and_render(int(handle), patch)


def make_patch_bytes(updates) -> bytes:
	"""Serialize an iterable of (index, sym, fg, bg) updates into the compact
	20-byte-per-record binary patch format used by the native bridge.

	Record layout (20 bytes):
	  0..3   uint32 le index
	  4..11  8-byte symbol (utf-8, zero-padded)
	  12..14 fg r,g,b
	  15..17 bg r,g,b
	  18     flags (bit0: custom_fg, bit1: custom_bg)
	  19     reserved
	"""
	import struct
	rec_fmt = "<I8s3B3BB"
	out = bytearray()
	for idx, sym, fg, bg in updates:
		if sym is None:
			symb = b"\x00" * 8
		else:
			bs = str(sym).encode("utf-8")[:8]
			symb = bs.ljust(8, b"\x00")
		if fg is None:
			fg_b = (0, 0, 0)
			fg_flag = 0
		else:
			fg_b = tuple(int(x) & 0xFF for x in fg)
			fg_flag = 1
		if bg is None:
			bg_b = (0, 0, 0)
			bg_flag = 0
		else:
			bg_b = tuple(int(x) & 0xFF for x in bg)
			bg_flag = 1
		flags = (fg_flag & 1) | ((bg_flag & 1) << 1)
		out.extend(struct.pack(rec_fmt, int(idx) & 0xFFFFFFFF, symb,
			fg_b[0], fg_b[1], fg_b[2], bg_b[0], bg_b[1], bg_b[2], flags, 0))
	return bytes(out)


__all__ += ["tuix_render_streaming", "tuix_create_native_buffer", "tuix_free_native_buffer",
			"tuix_apply_patch_and_render", "make_patch_bytes"]
