# 🧱 TUIX Core 0.5

---

## 🧩 Overview

**TUIX** is a modular terminal UI engine inspired by web technologies.
It introduces a **DOM-like component system**, a **nested layout model**, a **viewport-aware input/rendering pipeline**, and a **buffer-based renderer** for building structured and styled terminal interfaces.

v0.5 ships a compiled **C + Cython** core focused on stability and composition. It hardens resize/teardown memory ownership, lowers render-path lock contention, promotes scrollable viewports to first-class layout/input owners, and expands the Python API around layout, modal UI, scene state, and widget control.

## 🆕 What's New In 0.5

- Core buffer ownership hardening: `TuixBuffer.pixels_owned`, safer core/builder pixel lifetime rules, and core-owned copies for builder-returned temporary buffers.
- Resize and teardown stability fixes across scene clear, registry destroy, buffer allocation, compaction, blank-row scratch rebuilds, and builder cached-pixel cleanup.
- Lower lock contention: builder callbacks run outside the global registry lock while snapshots/copies keep resize and build steps safe.
- Rendering diff stability improvements in text and halfblock renderers by comparing terminal-visible ANSI color output instead of unstable intermediate color values.
- Input routing fixes for heavy hover/drag traffic, hitmap target picking, viewport ownership, and generic mouse capture.
- Scrollable containers are now integrated through a viewport-aware compositor, geometry resolver, hitmap, and input-routing pipeline.
- First nested-layout contract: `TuixLayoutSlot`, `TuixLayoutRect`, parent-first subtree rebuilds, and per-child slot/layout/grid override APIs.
- New layout builders: Row, Column, SplitPane, and Grid.
- New viewport/UI widgets: Checkbox, ListView, TextArea, and Dialog.
- Python bindings now cover scene selection, modal activation, transactions, scene stats/compaction, mouse capture, layout/grid overrides, richer snapshots, and the new widget/layout APIs.
- Compatibility aliases remain for older high-level names such as `menu_set_options`, `status_set_status`, `button_is_pressed`, `scroll_container_get_scroll_pos`, and `box_set_color`.
- `VideoPlayerBuilder` remains internal-only for now due to stability concerns and low practical demand.

**Core vs Framework:** TUIX provides a low-level engine core - not a high-level application framework. The public API intentionally exposes low-level primitives and is not designed to be "simple" by itself. A higher-level framework API is now available at https://github.com/custosh/tuixpy.

---

## 🚀 Installation

```bash
pip install tuix-core
```

> Requires Python ≥ 3.10. Pre-built wheels are provided for Windows/Linux/macOS (AMD64, x86, ARM).  
> Building from source also requires **Cython ≥ 0.29** and a C compiler.

---

## ⚡ Quick Start

```python
from tuix.core import engine, builders, scenes, objects, buffers, input

engine.init()
builders.register_standard()

scenes.init_scene(b"Main")
scenes.select_scene(b"Main")
input.listen()

# create a progressbar and animate it
uid = objects.create_object(builders.PROGRESSBAR, b"Main", 0.7, 0.08, 0.45, 0.15)
obj = objects.get_object_by_uid(uid)
snap = buffers.get_buffer_snapshot(b"Main", uid)

if obj and snap:
    for i in range(101):
        objects.tuix_progressbar_set_value(obj, i / 100.0)
        engine.main_loop()

buffers.free_buffer(b"Main", uid)
input.stop()
engine.shutdown()
```

---

## 🧩 Widgets

| Builder constant | What it renders |
|---|---|
| `builders.PROGRESSBAR` | Horizontal fill bar with customisable chars and colours |
| `builders.CHOICE` | Keyboard-navigable list menu |
| `builders.INPUT` | Single-line text input with placeholder support |
| `builders.CANVAS` | Free-draw surface - pixels, lines, rects, circles, text, sprites |
| `builders.TEXT` | Inline text content with runtime fg/bg updates |
| `builders.BOX` | Framed container with title and color controls |
| `builders.DIVIDER` | Horizontal/vertical divider with custom symbol/color |
| `builders.BADGE` | Compact label with fg/bg palette |
| `builders.BUTTON` | Clickable/keyboard-activatable button state |
| `builders.TAG` | Chip-style label with configurable brackets |
| `builders.STATUS` | IDLE/OK/WARN/ERROR status display widget |
| `builders.MENU` | Interactive menu with keyboard/mouse selection |
| `builders.SCROLL_CONTAINER` | Viewport-backed scroll container with content-space children |
| `builders.ROW` | Horizontal stack/flex-style layout parent |
| `builders.COLUMN` | Vertical stack/flex-style layout parent |
| `builders.SPLITPANE` | Two-pane layout with draggable/keyboard-resizable divider |
| `builders.GRID` | Grid layout with fixed and weighted row/column tracks |
| `builders.CHECKBOX` | Toggle with label, checked/disabled state, mouse and keyboard input |
| `builders.LISTVIEW` | Viewport-backed virtual list with selection and activation |
| `builders.TEXTAREA` | Multiline viewport-backed editor with cursor/scroll/editing APIs |
| `builders.DIALOG` | Modal dialog with backdrop, focus trapping, and close handling |

### Progressbar

```python
objects.tuix_progressbar_set_value(obj, 0.75)          # 0.0 – 1.0
objects.tuix_progressbar_set_style(obj,
    ord('#'), ord('-'),          # fill char, empty char
    120, 220, 80,                # fill RGB
    50,  50,  50)                # empty RGB
```

### Choice

```python
objects.tuix_choice_set_options(obj, [b"Yes", b"No", b"Maybe"])
# no manual feed_input needed (handled by engine/main loop)

if objects.tuix_choice_is_confirmed(obj):
    idx = int(objects.tuix_choice_get_result(obj))
```

### Input

```python
objects.tuix_input_set_placeholder(obj, b"Type here...")
# no manual feed_input needed (handled by engine/main loop)

if objects.tuix_input_is_submitted(obj):
    text = objects.tuix_input_get_result(obj)   # bytes
```

### Canvas

```python
objects.tuix_canvas_draw_rect  (obj, x, y, w, h, b'#', filled=1, r, g, b, br, bg, bb)
objects.tuix_canvas_draw_circle(obj, cx, cy, radius, b'O', filled=0, r, g, b, br, bg, bb)
objects.tuix_canvas_draw_line  (obj, x0, y0, x1, y1, b'/', r, g, b, br, bg, bb)
objects.tuix_canvas_draw_text  (obj, x, y, b"hello", r, g, b, br, bg, bb)
objects.tuix_canvas_set_pixel  (obj, x, y, b'*', r, g, b, br, bg, bb)
```

### Layout Builders

```python
row_uid = objects.create_object(builders.ROW, b"Main", 0.9, 0.25, 0.1, 0.05)
row = objects.get_object_by_uid(row_uid)

objects.tuix_stack_set_gap(row, 2)
objects.tuix_stack_set_padding(row, 1, 1, 1, 1)
objects.tuix_stack_set_justify(row, builders.JUSTIFY_SPACE_BETWEEN)
objects.tuix_stack_set_align(row, builders.ALIGN_STRETCH)

label_uid = objects.tuix_stack_add_object(row, b"Main", builders.TEXT, 1.0, 1.0)
buffers.set_buffer_layout_slot_by_uid(label_uid, grow=1.0, min_w=12)
```

```python
grid_uid = objects.create_object(builders.GRID, b"Main", 0.9, 0.5, 0.35, 0.05)
grid = objects.get_object_by_uid(grid_uid)

objects.tuix_grid_set_columns(grid, [
    (builders.GRID_TRACK_WEIGHT, 1),
    (builders.GRID_TRACK_FIXED, 24),
])
objects.tuix_grid_set_rows(grid, [
    (builders.GRID_TRACK_FIXED, 3),
    (builders.GRID_TRACK_WEIGHT, 1),
])
objects.tuix_grid_set_gaps(grid, 1, 1)

child_uid = objects.tuix_grid_add_object(grid, b"Main", builders.TEXT, 1.0, 1.0)
buffers.set_buffer_grid_placement_by_uid(child_uid, row=1, col=0, row_span=1, col_span=2)
```

### Viewport And Modal Widgets

```python
list_uid = objects.create_object(builders.LISTVIEW, b"Main", 0.4, 0.4, 0.1, 0.05)
listview = objects.get_object_by_uid(list_uid)
objects.tuix_listview_set_title(listview, b"Items")
objects.tuix_listview_set_items(listview, [b"Alpha", b"Beta", b"Gamma"])

textarea_uid = objects.create_object(builders.TEXTAREA, b"Main", 0.5, 0.4, 0.1, 0.5)
textarea = objects.get_object_by_uid(textarea_uid)
objects.tuix_textarea_set_title(textarea, b"Notes")
objects.tuix_textarea_set_placeholder(textarea, b"Type here...")

dialog_uid = objects.create_object(builders.DIALOG, b"Main", 0.9, 0.7, 0.05, 0.05)
dialog = objects.get_object_by_uid(dialog_uid)
objects.tuix_dialog_set_title(dialog, b"Confirm")
objects.tuix_dialog_activate(dialog, b"Main")
```

---

## 📁 Examples

Runnable examples are in the `examples/` directory:

| File | Description |
|---|---|
| `examples/widgets/progressbar_dual.py` | Two bars filling at different speeds |
| `examples/widgets/choice_palette.py` | Colour palette menu, prints chosen RGB to stdout |
| `examples/widgets/canvas_bounce.py` | Animated bouncing ball with colour cycling and FPS counter |
| `examples/widgets/text_and_box_demo.py` | Text and box builder styling demo |
| `examples/widgets/button_and_badge_demo.py` | Button and badge interaction demo |
| `examples/widgets/menu_and_tags_demo.py` | Menu and tag widgets together |
| `examples/widgets/scroll_container_demo.py` | Scroll container viewport demo |
| `examples/widgets/viewport_modal_demo.py` | ListView, TextArea, Checkbox, Button, and Dialog demo |
| `examples/multimodal/focus_routing.py` | Focus routing between Choice, ListView, and TextArea |
| `examples/showcase/sequential_journey.py` | Full sequential demo (progressbar → choice → input → canvas) |
| `examples/showcase/layout_demo.py` | Row, SplitPane, Grid, layout slots, and grid placement demo |
| `examples/showcase/buffer_hierarchy_demo.py` | Parent-child buffer hierarchy and z-index layering (v0.3) |
| `examples/showcase/scene_stats_demo.py` | Scene stats snapshots and compaction APIs (v0.3) |

```bash
python examples/widgets/canvas_bounce.py
```

---

## 📚 Documentation

Full documentation is available at: https://docs.custosh.dev/docs/tuix-core

---

## 🧪 Tests

```bash
pip install pytest
pytest
```

65 tests covering engine lifecycle, scene management, registry, multimodal routing APIs, widget builders, nested layout helpers, viewport widgets, modal state, snapshots/hierarchy APIs, scene compaction/stats, canvas draw calls, and examples layout integrity.

---

## 📊 Benchmarks

Benchmarked against **blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React, TUIX.Renderer, TUIX.Core, TUIX.Python, Rich, Urwid, PromptToolkit**.

Latest run metadata (from `benchmark_report.md`):
- Date: 2026-05-20T05:03:14.509Z
- Runtime/OS: Node v22.19.0 on Windows_NT 10.0.26200 (win32 x64)
- CPU/RAM: 12th Gen Intel(R) Core(TM) i5-12450HX (12 cores), RAM 24352MB

The README summary keeps `Mean`, `ops/s`, and `Peak RSS`. The full report also includes `Runs`, `Run CV`, `Mean CI95`, `Wall`, `RSS Growth`, and two byte metrics:
- `Bytes(local)`: bytes reported by each framework counter.
- `Bytes(pty)`: observed PTY bytes (cross-framework comparable).

### Startup

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| Ratatui | 152µs | 6.6K | 27.0 KB |
| ReziTUI | 256µs | 3.9K | 81.4 MB |
| TUIX.Python | 286µs | 3.5K | 18.9 MB |
| TUIX.Core | 315µs | 3.2K | 4.1 MB |
| Bubble Tea | 315µs | 3.2K | 11.9 MB |
| TUIX.Renderer | 324µs | 3.1K | 3.9 MB |
| OpenTUI.React | 368µs | 2.7K | 35.9 MB |

### Tree Construction (1,000 items)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Core | 108µs | 9.3K | 4.2 MB |
| TUIX.Renderer | 109µs | 9.2K | 4.0 MB |
| OpenTUI.Core | 110µs | 9.1K | 35.1 MB |
| TUIX.Python | 241µs | 4.1K | 19.1 MB |
| OpenTUI.React | 367µs | 2.7K | 38.2 MB |
| ReziTUI | 424µs | 2.4K | 188.5 MB |
| Rich | 1.00ms | 998 | 31.9 MB |

### Re-render

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Python | 9µs | 114.3K | 19.2 MB |
| TUIX.Renderer | 10µs | 98.0K | 4.0 MB |
| TUIX.Core | 15µs | 68.2K | 4.3 MB |
| Blessed | 27µs | 36.5K | 526.7 MB |
| Ratatui | 46µs | 21.7K | 27.0 KB |
| OpenTUI.React | 50µs | 19.8K | 38.2 MB |
| ReziTUI | 60µs | 16.7K | 196.6 MB |

### Layout Stress (4 cols x 10 rows)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Renderer | 72µs | 13.9K | 4.0 MB |
| OpenTUI.Core | 80µs | 12.5K | 35.1 MB |
| OpenTUI.React | 80µs | 12.5K | 39.1 MB |
| TUIX.Core | 81µs | 12.3K | 4.2 MB |
| Ratatui | 89µs | 11.2K | 27.0 KB |
| TUIX.Python | 109µs | 9.2K | 19.3 MB |
| Bubble Tea | 172µs | 5.8K | 16.2 MB |

### Scroll Stress (2,000 items)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| OpenTUI.React | 46µs | 21.5K | 39.1 MB |
| ReziTUI | 73µs | 13.7K | 191.7 MB |
| OpenTUI.Core | 91µs | 11.0K | 35.3 MB |
| Bubble Tea | 102µs | 9.8K | 16.2 MB |
| Ratatui | 111µs | 9.0K | 27.0 KB |
| TUIX.Core | 247µs | 4.0K | 4.2 MB |
| TUIX.Python | 311µs | 3.2K | 19.3 MB |
| TUIX.Renderer | 314µs | 3.2K | 4.0 MB |

### Virtual List (100,000 items, viewport 40)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| OpenTUI.React | 62µs | 16.2K | 39.1 MB |
| Ratatui | 71µs | 14.0K | 27.0 KB |
| Blessed | 98µs | 10.2K | 529.4 MB |
| OpenTUI.Core | 104µs | 9.6K | 35.3 MB |
| ReziTUI | 111µs | 9.0K | 193.6 MB |
| Bubble Tea | 113µs | 8.9K | 20.2 MB |
| TUIX.Renderer | 120µs | 8.3K | 4.0 MB |
| TUIX.Core | 136µs | 7.4K | 4.3 MB |
| TUIX.Python | 156µs | 6.4K | 19.3 MB |

### Terminal Full UI (120x40, 24 services)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| Ratatui | 193µs | 5.2K | 27.0 KB |
| OpenTUI.Core | 293µs | 3.4K | 37.3 MB |
| TUIX.Renderer | 352µs | 2.8K | 4.3 MB |
| OpenTUI.React | 365µs | 2.7K | 39.3 MB |
| TUIX.Python | 383µs | 2.6K | 19.8 MB |
| TUIX.Core | 413µs | 2.4K | 4.7 MB |
| Bubble Tea | 466µs | 2.1K | 20.5 MB |

In this run, TUIX.Core is effectively tied for the fastest 1,000-item tree construction, TUIX.Python leads the low-latency re-render scenario, and TUIX.Renderer leads layout stress. Terminal full-UI and scroll/virtual-list scenarios are more mixed, but TUIX variants keep Peak RSS around 4-20 MB and the full report shows low observed PTY byte output for several full-screen workloads.

Full tables (including CI95, CV, wall time, RSS growth, `Bytes(local)`, and `Bytes(pty)`) are in [BENCHMARKS.md](BENCHMARKS.md).

---

## ⚙️ Architecture

```
tuix.core
├── engine.py       init / shutdown / main_loop / stats / mouse capture
├── scenes.py       scene lifecycle / focus / modal / transactions / compaction
├── registry.py     active scene pointer
├── builders.py     builder constants + layout/grid constants + register_standard()
├── objects.py      widget, layout, viewport, and modal runtime APIs
├── buffers.py      snapshots + hierarchy + z-index + layout/grid overrides
├── input.py        input listener + consuming/non-consuming snapshots
├── _structs.py     ctypes mirror of C structs
└── _tuix_cy.pyx    Cython extension - all C calls go through here
```

The C core handles layout geometry, viewport clipping, hitmap generation, compositing, diffing, and terminal rendering. Python drives scene/object lifecycle, widget state, nested layout placement, and safe state inspection through snapshots.

---

## ⚙️ Future Plans

- Optional runtime assertions around buffer bounds and ownership
- Debug mode for buffer allocation/free and ownership-transition logs
- Theme / style system
- Async-friendly loop integration

---

## Support / Donate

If you enjoy using TUIX, you can support its development:

- [💖 GitHub Sponsors](https://github.com/sponsors/custosh)
- [💰 PayPal](https://www.paypal.com/donate/?hosted_button_id=R7W3Z65XMVLBN)

---

## 📜 License

MIT License © 2026 custosh

```text
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
