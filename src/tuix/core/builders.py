from . import _lib

# Builder name constants (match C builder `.name` fields)
CANVAS = b"CanvasBuilder"
CHOICE = b"ChoiceBuilder"
INPUT = b"InputBuilder"
PROGRESSBAR = b"ProgressBarBuilder"
TEXT = b"TextBuilder"
BOX = b"BoxBuilder"
DIVIDER = b"DividerBuilder"
BADGE = b"BadgeBuilder"
BUTTON = b"ButtonBuilder"
ICON = b"IconBuilder"
TAG = b"TagBuilder"
STATUS = b"StatusBuilder"
MENU = b"MenuBuilder"
SCROLL_CONTAINER = b"ScrollContainerBuilder"


def register_standard():
    # Calls the native registrant to install built-in builders
    try:
        return _lib._mod.tuix_register_standart_builders()
    except Exception:
        try:
            fn = _lib.get_func('tuix_register_standart_builders', restype=int, argtypes=None)
            return fn()
        except Exception:
            return 0
