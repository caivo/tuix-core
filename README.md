# 🧱 TUIX Core 0.2.1

---

## 🧩 Overview

**TUIX** is a modular terminal UI engine inspired by web technologies.  
It introduces a **DOM-like component system**, a **layout engine**, and a **buffer-based rendering pipeline** for building structured and styled terminal interfaces.

v0.2.1 ships a compiled **C + Cython** core for low-latency rendering, four built-in widget builders (progressbar, choice, input, canvas), and a clean Python API layered on top.

## 🆕 What's New In 0.2.1

- Packaging/build consistency improvements for Windows and editable installs.
- New scene focus primitives in Python API: `scenes.set_focus(...)` and `scenes.set_previous_focus(...)`.
- New multimodal example: `examples/multimodal/focus_routing.py`.
- Examples reorganized by domain: `widgets/`, `multimodal/`, `showcase/`.
- Extended test coverage for focus APIs and examples layout integrity.

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
ptr = buffers.get_buffer_by_uid(uid)
obj = ptr.contents.obj.contents

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
objects.tuix_choice_feed_input(obj, snap)       # snap = input.get_snapshot()

if objects.tuix_choice_is_confirmed(obj):
    idx = int(objects.tuix_choice_get_result(obj))
```

### Input

```python
objects.tuix_input_set_placeholder(obj, b"Type here...")
objects.tuix_input_feed_input(obj, snap)

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

39 tests covering engine lifecycle, scene management, registry, multimodal focus APIs, all four widget builders, canvas draw calls, and examples layout integrity.

---

## 📊 Benchmarks

Benchmarked against **blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React, TUIX.Renderer, Rich, Urwid, PromptToolkit** on Windows 10 (12th Gen i5-12450HX, 24 GB RAM).

Note to Rezi: Hope you don't mind that I took your benchmarks and reproduced them here - thanks for the reference and inspiration.

Expanded benchmark runs now include CI95 intervals and two byte metrics:
- `Bytes(local)`: bytes reported by each framework's own counters.
- `Bytes(pty)`: observed PTY bytes for cross-framework comparison.

### Startup

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| **TUIX.Core** | 176µs | 5.7K | 4.0 MB |
| **TUIX.Python** | 177µs | 5.7K | 18.2 MB |
| Ratatui | 258µs | 3.9K | 27.0 KB |
| ReziTUI | 201µs | 5.0K | 78.4 MB |
| Ink | 3.74ms | 268 | 62.3 MB |
| Blessed | 31.80ms | 31 | 149.5 MB |

### Tree Construction (1 000 items)

| Framework | Mean | ops/s |
|---|---:|---:|
| **TUIX.Core** | **49µs** | **20.2K** |
| OpenTUI.Core | 103µs | 9.7K |
| **TUIX.Python** | 140µs | 7.1K |
| Ratatui | 1.18ms | 847 |
| Bubble Tea | 1.23ms | 811 |
| Ink | 79.91ms | 13 |

### Re-render

| Framework | Mean | ops/s |
|---|---:|---:|
| **TUIX.Python** | **11µs** | **88.6K** |
| **TUIX.Core** | 13µs | 75.6K |
| Blessed | 30µs | 33.6K |
| Ratatui | 66µs | 15.1K |
| Bubble Tea | 121µs | 8.3K |
| Ink | 1.03ms | 966 |

### Virtual List (100 000 items, viewport 40)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| **TUIX.Core** | **13µs** | **80.0K** | **4.1 MB** |
| OpenTUI.React | 42µs | 24.0K | 38.9 MB |
| **TUIX.Python** | 62µs | 16.0K | 18.5 MB |
| Ratatui | 82µs | 12.2K | 27.0 KB |
| Ink | 1.54ms | 651 | 347.4 MB |

### Terminal Full UI (120x40, 24 services)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| **TUIX.Core** | **33µs** | **30.5K** | **4.6 MB** |
| **TUIX.Python** | 82µs | 12.1K | 19.0 MB |
| Ratatui | 393µs | 2.5K | 27.0 KB |
| Bubble Tea | 608µs | 1.6K | 24.8 MB |
| OpenTUI.Core | 689µs | 1.5K | 36.7 MB |
| Ink | 2.60ms | 385 | 348.7 MB |

Across the expanded suite, TUIX.Core leads key throughput-heavy scenarios (tree build, virtual list, and full UI composition), while TUIX.Python and TUIX.Renderer stay highly competitive in low-latency rerender paths. Peak RSS remains low for TUIX variants compared with most JavaScript/Python UI stacks.  
Full results with CI95 and byte-output columns are in [`BENCHMARKS.md`](BENCHMARKS.md).

---

## ⚙️ Architecture

```
tuix.core
├── engine.py       init / shutdown / main_loop
├── scenes.py       scene create / free / clear
├── registry.py     active scene pointer
├── builders.py     builder name constants + register_standard()
├── objects.py      widget API (progressbar / choice / input / canvas)
├── buffers.py      buffer lookups
├── input.py        input listener + snapshot
├── _structs.py     ctypes mirror of C structs
└── _tuix_cy.pyx    Cython extension - all C calls go through here
```

The C core handles layout geometry, compositing, and rendering to the terminal. Python only drives the loop and feeds input.

---

## ⚙️ Future Plans

- Scrollable containers and nested layouts
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
