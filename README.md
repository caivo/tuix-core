# 🧱 TUIX Core 0.3

---

## 🧩 Overview

**TUIX** is a modular terminal UI engine inspired by web technologies.  
It introduces a **DOM-like component system**, a **layout engine**, and a **buffer-based rendering pipeline** for building structured and styled terminal interfaces.

v0.3 ships a compiled **C + Cython** core for low-latency rendering, four built-in widget builders (progressbar, choice, input, canvas), and a Python API that now includes buffer snapshots, hierarchy controls, and scene memory tooling.

## 🆕 What's New In 0.3

- Buffer snapshots for safe reads: `buffers.get_buffer_snapshot(...)`, `buffers.get_buffer_snapshot_by_uid(...)`, `objects.get_object_snapshot_by_uid(...)`.
- Buffer hierarchy and ordering controls: `buffers.set_buffer_parent(...)`, `buffers.set_buffer_z_index(...)`, `buffers.get_buffer_z_index(...)`.
- Scene instrumentation and memory APIs: `scenes.get_scene_stats(...)`, `scenes.compact_scene_pixels(...)`, `scenes.compact_cold_scenes(...)`.
- Input handling now runs automatically inside builders during the main loop.
- New showcase demos: `examples/showcase/buffer_hierarchy_demo.py` and `examples/showcase/scene_stats_demo.py`.

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
# v0.3: no manual feed_input needed (handled by engine/main loop)

if objects.tuix_choice_is_confirmed(obj):
    idx = int(objects.tuix_choice_get_result(obj))
```

### Input

```python
objects.tuix_input_set_placeholder(obj, b"Type here...")
# v0.3: no manual feed_input needed (handled by engine/main loop)

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

50 tests covering engine lifecycle, scene management, registry, multimodal focus APIs, all four widget builders, v0.3 buffer hierarchy/snapshot APIs, scene compaction/stats, canvas draw calls, and examples layout integrity.

---

## 📊 Benchmarks

Benchmarked against **blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React, TUIX.Renderer, TUIX.Core, TUIX.Python, Rich, Urwid, PromptToolkit**.

Latest run metadata (from `BENCHMARKS.md`):
- Date: 2026-04-05
- Runtime/OS: Node v22.19.0 on Windows_NT 10.0.26200 (win32 x64)
- CPU/RAM: 12th Gen Intel Core i5-12450HX (12 cores), 24 GB RAM

Expanded benchmark runs include CI95 intervals and two byte metrics:
- `Bytes(local)`: bytes reported by each framework's own counters.
- `Bytes(pty)`: observed PTY bytes for cross-framework comparison.

### Startup

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Python | 229µs | 4.4K | 18.4 MB |
| TUIX.Core | 257µs | 3.9K | 4.0 MB |
| TUIX.Renderer | 258µs | 3.9K | 3.9 MB |
| Ratatui | 195µs | 5.1K | 27.0 KB |
| ReziTUI | 197µs | 5.1K | 81.7 MB |
| Ink | 2.51ms | 398 | 65.0 MB |
| Blessed | 25.64ms | 39 | 152.1 MB |

### Tree Construction (1,000 items)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Core | 72µs | 13.9K | 4.0 MB |
| TUIX.Renderer | 84µs | 12.0K | 3.9 MB |
| OpenTUI.Core | 97µs | 10.4K | 35.0 MB |
| TUIX.Python | 125µs | 8.0K | 18.7 MB |
| Ratatui | 986µs | 1.0K | 27.0 KB |
| Bubble Tea | 1.05ms | 955 | 16.2 MB |
| Ink | 42.86ms | 23 | 355.0 MB |

### Re-render

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Renderer | 10µs | 95.9K | 3.9 MB |
| TUIX.Python | 13µs | 79.9K | 18.7 MB |
| TUIX.Core | 23µs | 44.0K | 4.1 MB |
| Blessed | 26µs | 39.1K | 520.5 MB |
| Ratatui | 57µs | 17.6K | 27.0 KB |
| Bubble Tea | 91µs | 11.0K | 16.2 MB |
| Ink | 647µs | 1.5K | 354.9 MB |

### Virtual List (100,000 items, viewport 40)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Core | 19µs | 53.1K | 4.1 MB |
| OpenTUI.React | 41µs | 24.7K | 38.9 MB |
| TUIX.Renderer | 62µs | 16.1K | 4.0 MB |
| TUIX.Python | 88µs | 11.4K | 18.7 MB |
| Ratatui | 97µs | 10.3K | 27.0 KB |
| Ink | 704µs | 1.4K | 356.7 MB |

### Terminal Full UI (120x40, 24 services)

| Framework | Mean | ops/s | Peak RSS |
|---|---:|---:|---:|
| TUIX.Core | 41µs | 24.2K | 4.5 MB |
| TUIX.Renderer | 93µs | 10.8K | 4.3 MB |
| TUIX.Python | 118µs | 8.5K | 19.3 MB |
| Ratatui | 170µs | 5.9K | 27.0 KB |
| OpenTUI.Core | 278µs | 3.6K | 36.9 MB |
| Bubble Tea | 497µs | 2.0K | 20.7 MB |
| Ink | 1.61ms | 622 | 357.1 MB |

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
