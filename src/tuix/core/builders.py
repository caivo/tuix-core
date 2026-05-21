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
TAG = b"TagBuilder"
STATUS = b"StatusBuilder"
MENU = b"MenuBuilder"
SCROLL_CONTAINER = b"ScrollContainerBuilder"
ROW = b"RowBuilder"
COLUMN = b"ColumnBuilder"
SPLITPANE = b"SplitPaneBuilder"
GRID = b"GridBuilder"
CHECKBOX = b"CheckboxBuilder"
LISTVIEW = b"ListViewBuilder"
TEXTAREA = b"TextAreaBuilder"
DIALOG = b"DialogBuilder"

# Aliases for ergonomic Python naming.
SPLIT_PANE = SPLITPANE
LIST_VIEW = LISTVIEW
TEXT_AREA = TEXTAREA

# Layout / track constants used by row/column/grid/splitpane/divider APIs.
AXIS_NONE = 0
AXIS_HORIZONTAL = 1
AXIS_VERTICAL = 2

JUSTIFY_START = 0
JUSTIFY_CENTER = 1
JUSTIFY_END = 2
JUSTIFY_SPACE_BETWEEN = 3

ALIGN_START = 0
ALIGN_CENTER = 1
ALIGN_END = 2
ALIGN_STRETCH = 3
ALIGN_AUTO = -1

GRID_TRACK_FIXED = 0
GRID_TRACK_WEIGHT = 1


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
