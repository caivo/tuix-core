# 🧱 TUIX Core 0.4

---

## 🧩 Overview

**TUIX** is a modular terminal UI engine inspired by web technologies.  
It introduces a **DOM-like component system**, a **layout engine**, and a **buffer-based rendering pipeline** for building structured and styled terminal interfaces.

v0.4 ships a compiled **C + Cython** core for low-level latency. Now with a larger builder set, concurrency-safety fixes, hitmap-based mouse routing, and expanded benchmarking coverage.

## 🆕 What's New In 0.4

- New builders: Text, Box, Divider, Badge, Button, Icon, Tag, Status, Menu, ScrollContainer (plus existing ProgressBar, Choice, Input, Canvas).
- New low-level routing capabilities: frame-local compositor hitmap generation + hitmap-based mouse click picking.
- New non-consuming input snapshot API: `input.peek_snapshot()`.
- Concurrency and lifecycle hardening across registry/scene/subcycle/object paths.
- Expanded benchmark suite and reporting (`BENCHMARKS.md`).

**Core vs Framework:** TUIX provides a low-level engine core - not a high-level application framework. The public API intentionally exposes low-level primitives and is not designed to be "simple" by itself; higher-level, user-friendly frameworks will be built on top of this core once it has matured and stabilised.

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
from tuix.core import engine, builders, scenes, registry, objects, buffers, input

engine.init()
builders.register_standard()

scenes.init_scene(b"Main")
registry.registry.current_scene_name = b"Main"
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
| `builders.ICON` | Centered symbol/icon with configurable colors |
| `builders.TAG` | Chip-style label with configurable brackets |
| `builders.STATUS` | IDLE/OK/WARN/ERROR status display widget |
| `builders.MENU` | Interactive menu with keyboard/mouse selection |
| `builders.SCROLL_CONTAINER` | Scrollable viewport over virtual content |

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

---

## 📁 Examples

Runnable examples are in the `examples/` directory:

| File | Description |
|---|---|
| `examples/widgets/progressbar_dual.py` | Two bars filling at different speeds |
| `examples/widgets/choice_palette.py` | Colour palette menu, prints chosen RGB to stdout |
| `examples/widgets/canvas_bounce.py` | Animated bouncing ball with colour cycling and FPS counter |
| `examples/multimodal/focus_routing.py` | Keyboard + mouse focus routing between choice and input |
| `examples/showcase/sequential_journey.py` | Full sequential demo (progressbar → choice → input → canvas) |
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

60 tests covering engine lifecycle, scene management, registry, multimodal routing APIs, widget builders, snapshots/hierarchy APIs, scene compaction/stats, canvas draw calls, and examples layout integrity.

---

## 📊 Benchmarks

Benchmarked against **blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React, TUIX.Renderer, TUIX.Core, TUIX.Python, Rich, Urwid, PromptToolkit**.

Latest run metadata (from `BENCHMARKS.md`):
- Date: 2026-04-09T07:26:08.098Z
- Runtime/OS: Node v22.19.0 on Windows_NT 10.0.26200 (win32 x64)
- CPU/RAM: 12th Gen Intel(R) Core(TM) i5-12450HX (12 cores), RAM 24352MB

Runs include CI95 intervals and two byte metrics:
- `Bytes(local)`: bytes reported by each framework counter.
- `Bytes(pty)`: observed PTY bytes (cross-framework comparable).

### Startup

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| Ratatui | 262µs | 3.8K | 27.0 KB |
| TUIX.Renderer | 277µs | 3.6K | 3.9 MB |
| TUIX.Core | 311µs | 3.2K | 4.0 MB |
| ReziTUI | 346µs | 2.9K | 81.8 MB |
| TUIX.Python | 371µs | 2.7K | 18.4 MB |
| Ink | 3.26ms | 306 | 64.9 MB |
| Blessed | 34.93ms | 29 | 152.6 MB |

### Tree Construction (1,000 items)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Renderer | 83µs | 12.1K | 3.9 MB |
| TUIX.Core | 126µs | 7.9K | 4.0 MB |
| OpenTUI.Core | 134µs | 7.5K | 34.9 MB |
| TUIX.Python | 219µs | 4.6K | 18.6 MB |
| Ratatui | 1.30ms | 770 | 27.0 KB |
| Bubble Tea | 1.26ms | 791 | 16.0 MB |
| Ink | 54.72ms | 18 | 364.8 MB |

### Re-render

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Renderer | 18µs | 57.0K | 3.9 MB |
| TUIX.Core | 19µs | 54.0K | 4.1 MB |
| TUIX.Python | 24µs | 41.0K | 18.6 MB |
| Blessed | 35µs | 28.2K | 526.0 MB |
| Ratatui | 67µs | 14.9K | 27.0 KB |
| Bubble Tea | 162µs | 6.2K | 16.0 MB |
| Ink | 812µs | 1.2K | 364.8 MB |

### Virtual List (100,000 items, viewport 40)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Core | 31µs | 32.2K | 4.1 MB |
| OpenTUI.React | 49µs | 20.4K | 38.9 MB |
| TUIX.Python | 82µs | 12.2K | 18.8 MB |
| TUIX.Renderer | 96µs | 10.4K | 4.2 MB |
| Ratatui | 128µs | 7.8K | 27.0 KB |
| Ink | 820µs | 1.2K | 365.7 MB |

### Terminal Full UI (120x40, 24 services)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Core | 61µs | 16.4K | 4.6 MB |
| TUIX.Renderer | 99µs | 10.1K | 4.6 MB |
| TUIX.Python | 129µs | 7.7K | 19.4 MB |
| Ratatui | 260µs | 3.8K | 27.0 KB |
| OpenTUI.Core | 436µs | 2.3K | 36.3 MB |
| Bubble Tea | 645µs | 1.6K | 24.5 MB |
| Ink | 1.94ms | 517 | 366.7 MB |

Across the expanded suite, TUIX.Core leads many throughput-heavy scenarios (for example virtual-list and terminal-full-ui), while TUIX.Renderer and TUIX.Python are especially strong in low-latency rerender and input-oriented terminal workloads. Peak RSS for TUIX variants remains low versus most JavaScript/Python UI stacks.

Full tables (including CI95, CV, wall time, RSS growth, `Bytes(local)`, and `Bytes(pty)`) are in [BENCHMARKS.md](BENCHMARKS.md).

---

## ⚙️ Architecture

```
tuix.core
├── engine.py       init / shutdown / main_loop
├── scenes.py       scene create / free / clear
├── registry.py     active scene pointer
├── builders.py     builder name constants + register_standard()
├── objects.py      widget API (progressbar / choice / input / canvas)
├── buffers.py      buffer snapshots + hierarchy + z-index
├── input.py        input listener + snapshot
├── _structs.py     ctypes mirror of C structs
└── _tuix_cy.pyx    Cython extension - all C calls go through here
```

The C core handles layout geometry, compositing, and rendering to the terminal. Python drives scene/object lifecycle and can read state safely through snapshots.

---

## ⚙️ Future Plans

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
