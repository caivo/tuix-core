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

class TuixLayoutSlot(ctypes.Structure):
    _fields_ = [
        ('grow', ctypes.c_float),
        ('shrink', ctypes.c_float),
        ('basis', ctypes.c_int),
        ('min_w', ctypes.c_int),
        ('min_h', ctypes.c_int),
        ('max_w', ctypes.c_int),
        ('max_h', ctypes.c_int),
        ('align_self', ctypes.c_int),
        ('grid_row', ctypes.c_int),
        ('grid_col', ctypes.c_int),
        ('row_span', ctypes.c_int),
        ('col_span', ctypes.c_int),
    ]

class TuixLayoutRect(ctypes.Structure):
    _fields_ = [
        ('active', ctypes.c_int),
        ('offset_left', ctypes.c_int),
        ('offset_top', ctypes.c_int),
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
    ]

class TuixGridTrack(ctypes.Structure):
    _fields_ = [('kind', ctypes.c_int), ('value', ctypes.c_int)]

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
        ('pixels_owned', ctypes.c_int),
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
        ('required_redraw', ctypes.c_int),
        ('margin_left', ctypes.c_int),
        ('margin_top', ctypes.c_int),
        ('parent_uid', ctypes.c_int),
        ('z_index', ctypes.c_int),
        ('flat_index', ctypes.c_int),
        ('children_uids', ctypes.POINTER(ctypes.c_int)),
        ('children_count', ctypes.c_int),
        ('children_capacity', ctypes.c_int),
        ('layout_slot', TuixLayoutSlot),
        ('layout_rect', TuixLayoutRect),
    ]

class TuixFinalBuffer(ctypes.Structure):
    _fields_ = [
        ('pixels', ctypes.POINTER(TuixPixel)),
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
        ('full_redraw', ctypes.c_int),
    ]

class TuixMouseKey(ctypes.Structure):
    _fields_ = [
        ('event', ctypes.c_int),
        ('btn', ctypes.c_int),
        ('buttons_held', ctypes.c_int),
        ('col', ctypes.c_int),
        ('row', ctypes.c_int),
        ('timestamp', ctypes.c_double),
        ('has_event', ctypes.c_int),
    ]

class TuixKeyboardKey(ctypes.Structure):
    _fields_ = [
        ('btn', ctypes.c_int),
        ('code', ctypes.c_int),
        ('scancode', ctypes.c_int),
        ('modifiers', ctypes.c_int),
        ('pressed', ctypes.c_int),
        ('repeat', ctypes.c_int),
        ('timestamp', ctypes.c_double),
        ('has_event', ctypes.c_int),
        ('text', ctypes.c_char * 8),
    ]

class TuixInputSnapshot(ctypes.Structure):
    _fields_ = [
        ('term_x', ctypes.c_int),
        ('term_y', ctypes.c_int),
        ('mouse', ctypes.POINTER(TuixMouseKey)),
        ('keyboard', ctypes.POINTER(TuixKeyboardKey)),
        ('consumed_keyboard', ctypes.c_bool),
        ('consumed_mouse', ctypes.c_bool),
    ]


class TuixScene(ctypes.Structure):
    _fields_ = [
        ('buffers', ctypes.c_void_p),
        ('buffer_by_uid', ctypes.c_void_p),
        ('root_uids', ctypes.POINTER(ctypes.c_int)),
        ('count', ctypes.c_int),
        ('active', ctypes.c_int),
        ('capacity', ctypes.c_int),
        ('max_uid_capacity', ctypes.c_int),
        ('root_count', ctypes.c_int),
        ('root_capacity', ctypes.c_int),
        ('current_focus', ctypes.c_int),
        ('active_modal_uid', ctypes.c_int),
        ('modal_restore_focus_uid', ctypes.c_int),
        ('transaction_depth', ctypes.c_int),
        ('topology_dirty', ctypes.c_int),
        ('last_active_frame', ctypes.c_ulonglong),
        ('last_compacted_frame', ctypes.c_ulonglong),
        ('topology_version', ctypes.c_ulonglong),
        ('last_composited_topology_version', ctypes.c_ulonglong),
    ]


class TuixSceneStats(ctypes.Structure):
    _fields_ = [
        ('buffer_count', ctypes.c_int),
        ('active', ctypes.c_int),
        ('current_focus', ctypes.c_int),
        ('last_active_frame', ctypes.c_ulonglong),
        ('last_compacted_frame', ctypes.c_ulonglong),
        ('pixel_bytes', ctypes.c_size_t),
        ('approx_heap_bytes', ctypes.c_size_t),
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
        ('frame_counter', ctypes.c_ulonglong),
        ('next_uid', ctypes.c_int),
        ('terminal_width', ctypes.c_int),
        ('terminal_height', ctypes.c_int),
        ('terminal_height_old', ctypes.c_int),
        ('terminal_width_old', ctypes.c_int),
        ('debug_config', ctypes.c_int),
    ]
