import ctypes

class TuixRGBTuple(ctypes.Structure):
    _fields_ = [('r', ctypes.c_uint8), ('g', ctypes.c_uint8), ('b', ctypes.c_uint8)]

class TuixPixelStyles(ctypes.Structure):
    _fields_ = [
        ('fg', TuixRGBTuple),
        ('bg', TuixRGBTuple),
        ('custom_bg', ctypes.c_uint8),
        ('custom_fg', ctypes.c_uint8),
        ('bold', ctypes.c_uint8),
        ('underlined', ctypes.c_uint8),
        ('italic', ctypes.c_uint8),
        ('dim', ctypes.c_uint8),
    ]

class TuixPixel(ctypes.Structure):
    _fields_ = [
        ('sym', ctypes.c_char * 8),
        ('styles', TuixPixelStyles),
        ('q_fg', TuixRGBTuple),
        ('q_bg', TuixRGBTuple),
        ('q_cached', ctypes.c_uint8),
    ]

class TuixObject(ctypes.Structure):
    pass

TuixObject._fields_ = [
    ('uid', ctypes.c_int),
    ('builder', ctypes.c_void_p),
    ('state', ctypes.c_void_p),
    ('width_mod', ctypes.c_float),
    ('height_mod', ctypes.c_float),
    ('margin_top_mod', ctypes.c_float),
    ('margin_left_mod', ctypes.c_float),
]

class TuixBuffer(ctypes.Structure):
    _fields_ = [
        ('obj', ctypes.POINTER(TuixObject)),
        ('pixels', ctypes.POINTER(TuixPixel)),
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
        ('required_redraw', ctypes.c_int),
        ('margin_left', ctypes.c_int),
        ('margin_top', ctypes.c_int),
    ]

class TuixFinalBuffer(ctypes.Structure):
    _fields_ = [
        ('pixels', ctypes.POINTER(TuixPixel)),
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
        ('full_redraw', ctypes.c_int),
    ]

class TuixInputSnapshot(ctypes.Structure):
    _fields_ = [
        ('term_x', ctypes.c_int),
        ('term_y', ctypes.c_int),
        ('mouse', ctypes.c_void_p),
        ('keyboard', ctypes.c_void_p),
    ]


class TuixScenes(ctypes.Structure):
    _fields_ = [
        ('scenes', ctypes.c_void_p),
        ('count', ctypes.c_int),
        ('capacity', ctypes.c_int),
        ('names', ctypes.POINTER(ctypes.c_char_p)),
    ]


class TuixSubcycles(ctypes.Structure):
    _fields_ = [
        ('subcycles', ctypes.c_void_p),
        ('count', ctypes.c_int),
        ('capacity', ctypes.c_int),
    ]


class TuixBuilders(ctypes.Structure):
    _fields_ = [
        ('builders', ctypes.c_void_p),
        ('count', ctypes.c_int),
        ('capacity', ctypes.c_int),
    ]


class TuixRegistry(ctypes.Structure):
    _fields_ = [
        ('scenes', TuixScenes),
        ('subcycles', TuixSubcycles),
        ('builders', TuixBuilders),
        ('current_scene_name', ctypes.c_char_p),
        ('next_uid', ctypes.c_int),
        ('terminal_width', ctypes.c_int),
        ('terminal_height', ctypes.c_int),
        ('terminal_height_old', ctypes.c_int),
        ('terminal_width_old', ctypes.c_int),
    ]
