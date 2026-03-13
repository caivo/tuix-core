# 🧱 TUIX Core 0.2.0

> ⚠️ **ATTENTION:**  
> This version is a **major rewrite** of the 0.1 MVP.  
> The Python API, layout engine, and rendering pipeline have all been overhauled.  
> Some edge cases and multi-widget input routing are still being ironed out for **v0.2.1**.  
> P.S. Still in active development!

---

## 🧩 Overview

**TUIX** is a modular terminal UI engine inspired by web technologies.  
It introduces a **DOM-like component system**, a **layout engine**, and a **buffer-based rendering pipeline** for building structured and styled terminal interfaces.

v0.2 ships a compiled **C + Cython** core for low-latency rendering, four built-in widget builders (progressbar, choice, input, canvas), and a clean Python API layered on top.

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
| `examples/ex_progressbar.py` | Two bars filling at different speeds |
| `examples/ex_choice.py` | Colour palette menu, prints chosen RGB to stdout |
| `examples/ex_canvas.py` | Animated bouncing ball with colour cycling and FPS counter |
| `examples/ex_multi_widget.py` | Progressbar + choice + input all live simultaneously |
| `example.py` | Full sequential demo (progressbar → choice → input → canvas) |

```bash
python examples/ex_canvas.py
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

29 tests covering engine lifecycle, scene management, registry, all four widget builders, and canvas draw calls.

---

## 📊 Benchmarks

Benchmarked against **blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React** on Windows 10 (12th Gen i5-12450HX, 24 GB RAM).

Note to Rezi: Hope you don't mind that I took your benchmarks and reproduced them here - thanks for the reference and inspiration.

### Startup

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| **TUIX.Core** | 302µs | 3.3K | **3.9 MB** |
| **TUIX.Python** | 361µs | 2.8K | 18.1 MB |
| Ratatui | 152µs | 6.6K | 27.0 KB |
| ReziTUI | 165µs | 6.1K | 76.1 MB |
| Ink | 2.49ms | 401 | 64.7 MB |
| Blessed | 41.14ms | 24 | 152.1 MB |

### Tree Construction (1 000 items)

| Framework | Mean | ops/s |
|---|---:|---:|
| **TUIX.Core** | **97µs** | **10.3K** |
| **TUIX.Python** | 167µs | 6.0K |
| OpenTUI.Core | 123µs | 8.1K |
| Ratatui | 962µs | 1.0K |
| Ink | 42.13ms | 24 |
| Bubble Tea | 562.95ms | 2 |

### Re-render

| Framework | Mean | ops/s |
|---|---:|---:|
| **TUIX.Python** | **10µs** | **96.1K** |
| Blessed | 31µs | 32.7K |
| **TUIX.Core** | 37µs | 26.7K |
| Ratatui | 50µs | 20.1K |
| Ink | 552µs | 1.8K |
| Bubble Tea | 10.98ms | 91 |

### Virtual List (100 000 items, viewport 40)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| **TUIX.Python** | 70µs | 14.4K | 18.3 MB |
| **TUIX.Core** | 89µs | 11.2K | **4.4 MB** |
| Ratatui | 100µs | 10.0K | 27.0 KB |
| OpenTUI.React | 197µs | 5.1K | 39.0 MB |
| Ink | 683µs | 1.5K | 359.2 MB |

TUIX.Core consistently **tops tree-construction regardless of item count** (97–110µs flat), and TUIX.Python leads the re-render benchmark at **96K ops/s** - all while keeping peak RSS well under 20 MB.  
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

## ⚠️ Known Limitations (v0.2)

- **Multi-widget input routing** - when multiple interactive widgets (e.g. choice + input) are on screen, both receive every keystroke. Proper focus management is planned for **v0.2.1**.
- `tuix_registry` is not exported from the `.pyd` on Windows; the Python layer uses a proxy that calls `tuix_select_scene` instead of writing the pointer directly.

---

## ⚙️ Future Plans

- Focus/routing system for multi-widget layouts (v0.2.1)
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
