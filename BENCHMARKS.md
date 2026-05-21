# Benchmark Results

> 2026-05-20T05:03:14.509Z | Node v22.19.0 | Windows_NT 10.0.26200 | win32 x64 | 12th Gen Intel(R) Core(TM) i5-12450HX (12 cores) | RAM 24352MB
> Libraries tested: blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React, TUIX.Renderer, TUIX.Core, TUIX.Python, Rich, Urwid, PromptToolkit
> Byte columns: "Bytes(local)" = bytes produced per frame (framework counter); "Bytes(pty)" = observed PTY bytes (cross-framework comparable).

## startup

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 152µs | 37.9% | 141µs–164µs | 6.6K ops/s | 17.22ms | 27.0KB | 0.00KB | 1.6MB | 16.0KB |
| rezitui | 1 | 256µs | 260.5% | 125µs–386µs | 3.9K ops/s | 25.74ms | 81.4MB | 13.4MB | 133.7KB | 1.3KB |
| TUIX.Py | 1 | 286µs | 20.8% | 274µs–298µs | 3.5K ops/s | 28.68ms | 18.9MB | 64.0KB | 6.7MB | 68.3KB |
| TUIX.Core | 1 | 315µs | 14.1% | 306µs–323µs | 3.2K ops/s | 31.47ms | 4.1MB | 4.0KB | 6.0MB | 61.6KB |
| bubbletea | 1 | 315µs | 51.2% | 284µs–347µs | 3.2K ops/s | 31.58ms | 11.9MB | 904.0KB | 1.6MB | 16.0KB |
| TUIX.Renderer | 1 | 324µs | 13.5% | 315µs–332µs | 3.1K ops/s | 32.38ms | 3.9MB | 0.00KB | 6.3MB | 64.3KB |
| opentui-react | 1 | 368µs | 46.5% | 334µs–401µs | 2.7K ops/s | 36.81ms | 35.9MB | 416.0KB | 7.4MB | 76.0KB |
| opentui-core | 1 | 431µs | 41.9% | 395µs–466µs | 2.3K ops/s | 43.12ms | 33.4MB | 412.0KB | 6.7MB | 68.5KB |
| terminal-kit | 1 | 626µs | 46.7% | 569µs–684µs | 1.6K ops/s | 62.92ms | 46.8MB | 0.00KB | 190.0KB | 1.9KB |
| urwid | 1 | 1.28ms | 13.0% | 1.25ms–1.32ms | 779 ops/s | 128.46ms | 42.1MB | 36.0KB | 187.5KB | 1.9KB |
| rich | 1 | 1.39ms | 12.6% | 1.35ms–1.42ms | 722 ops/s | 138.60ms | 31.8MB | 476.0KB | 189.7KB | 1.9KB |
| prompt-toolkit | 1 | 1.45ms | 38.1% | 1.34ms–1.56ms | 689 ops/s | 145.27ms | 36.0MB | 20.0KB | 189.7KB | 1.9KB |
| ink | 1 | 2.52ms | 25.9% | 2.39ms–2.65ms | 397 ops/s | 252.28ms | 65.2MB | 2.6MB | 189.7KB | 1.9KB |
| blessed | 1 | 36.92ms | 14.7% | 35.86ms–37.99ms | 27 ops/s | 3.69s | 152.2MB | 74.0MB | 229.2KB | 2.3KB |

## tree-construction (items=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| opentui-react | 1 | 52µs | 31.7% | 51µs–54µs | 19.2K ops/s | 26.73ms | 36.1MB | 112.0KB | 3.6MB | 7.5KB |
| TUIX.Renderer | 1 | 60µs | 17.7% | 59µs–61µs | 16.7K ops/s | 29.91ms | 3.9MB | 0.00KB | 593.8KB | 1.2KB |
| blessed | 1 | 81µs | 152.7% | 70µs–91µs | 12.4K ops/s | 40.65ms | 160.3MB | 6.5MB | 126.0KB | 258B |
| ratatui | 1 | 93µs | 20.5% | 91µs–94µs | 10.8K ops/s | 46.45ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| rezitui | 1 | 102µs | 211.3% | 83µs–120µs | 9.8K ops/s | 51.06ms | 102.3MB | 19.2MB | 668.5KB | 1.3KB |
| TUIX.Core | 1 | 105µs | 20.2% | 103µs–107µs | 9.6K ops/s | 52.35ms | 4.0MB | 16.0KB | 589.8KB | 1.2KB |
| TUIX.Py | 1 | 114µs | 49.9% | 109µs–119µs | 8.8K ops/s | 57.37ms | 19.0MB | 16.0KB | 7.7MB | 15.7KB |
| bubbletea | 1 | 189µs | 48.7% | 181µs–197µs | 5.3K ops/s | 94.71ms | 16.2MB | 4.3MB | 7.8MB | 16.0KB |
| terminal-kit | 1 | 316µs | 25.7% | 309µs–323µs | 3.2K ops/s | 158.37ms | 46.9MB | 4.0KB | 950.2KB | 1.9KB |
| opentui-core | 1 | 399µs | 69.1% | 375µs–423µs | 2.5K ops/s | 204.57ms | 34.1MB | 724.0KB | 33.4MB | 68.3KB |
| ink | 1 | 503µs | 41.7% | 484µs–521µs | 2.0K ops/s | 251.72ms | 69.2MB | 3.4MB | 23.9KB | 49B |
| prompt-toolkit | 1 | 851µs | 24.4% | 833µs–869µs | 1.2K ops/s | 425.76ms | 36.0MB | 16.0KB | 948.7KB | 1.9KB |
| rich | 1 | 915µs | 32.9% | 888µs–941µs | 1.1K ops/s | 457.62ms | 31.8MB | 32.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.32ms | 28.4% | 1.28ms–1.35ms | 759 ops/s | 658.69ms | 42.2MB | 76.0KB | 937.5KB | 1.9KB |

## tree-construction (items=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 125µs | 45.0% | 120µs–130µs | 8.0K ops/s | 62.38ms | 3.9MB | 0.00KB | 1.2MB | 2.5KB |
| rezitui | 1 | 128µs | 157.3% | 111µs–146µs | 7.8K ops/s | 64.53ms | 110.1MB | 11.2MB | 668.5KB | 1.3KB |
| TUIX.Core | 1 | 129µs | 35.1% | 125µs–133µs | 7.7K ops/s | 64.55ms | 4.1MB | 12.0KB | 1.2MB | 2.4KB |
| opentui-react | 1 | 137µs | 36.0% | 133µs–141µs | 7.3K ops/s | 75.13ms | 36.2MB | 104.0KB | 8.9MB | 18.3KB |
| opentui-core | 1 | 169µs | 259.8% | 131µs–208µs | 5.9K ops/s | 122.97ms | 34.8MB | 732.0KB | 33.8MB | 69.2KB |
| ratatui | 1 | 175µs | 43.4% | 168µs–181µs | 5.7K ops/s | 87.42ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| TUIX.Py | 1 | 201µs | 46.4% | 193µs–209µs | 5.0K ops/s | 100.78ms | 19.1MB | 32.0KB | 17.2MB | 35.2KB |
| bubbletea | 1 | 238µs | 39.0% | 230µs–246µs | 4.2K ops/s | 119.13ms | 16.2MB | 0.00KB | 7.8MB | 16.0KB |
| blessed | 1 | 570µs | 114.0% | 513µs–627µs | 1.8K ops/s | 285.42ms | 242.6MB | 78.9MB | 490.2KB | 1004B |
| rich | 1 | 832µs | 28.7% | 811µs–853µs | 1.2K ops/s | 416.16ms | 31.8MB | 16.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 1.05ms | 32.5% | 1.02ms–1.07ms | 957 ops/s | 522.74ms | 36.0MB | 20.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.25ms | 29.9% | 1.22ms–1.29ms | 798 ops/s | 626.43ms | 42.2MB | 16.0KB | 937.5KB | 1.9KB |
| terminal-kit | 1 | 1.43ms | 29.5% | 1.40ms–1.47ms | 697 ops/s | 718.28ms | 46.8MB | 32.0KB | 950.2KB | 1.9KB |
| ink | 1 | 5.20ms | 29.1% | 5.06ms–5.33ms | 192 ops/s | 2.60s | 107.3MB | 22.4MB | 287.6KB | 589B |

## tree-construction (items=500)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 106µs | 37.5% | 102µs–109µs | 9.4K ops/s | 53.01ms | 3.9MB | 0.00KB | 1.2MB | 2.5KB |
| TUIX.Core | 1 | 135µs | 59.2% | 128µs–142µs | 7.4K ops/s | 67.30ms | 4.1MB | 0.00KB | 1.2MB | 2.4KB |
| opentui-core | 1 | 175µs | 75.1% | 163µs–186µs | 5.7K ops/s | 298.23ms | 35.1MB | 348.0KB | 33.7MB | 68.9KB |
| TUIX.Py | 1 | 215µs | 26.1% | 210µs–220µs | 4.7K ops/s | 107.78ms | 19.1MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-react | 1 | 236µs | 33.7% | 229µs–243µs | 4.2K ops/s | 159.92ms | 36.5MB | 344.0KB | 8.9MB | 18.2KB |
| rezitui | 1 | 277µs | 300.3% | 204µs–350µs | 3.6K ops/s | 138.79ms | 168.6MB | 55.3MB | 668.5KB | 1.3KB |
| ratatui | 1 | 507µs | 9.8% | 502µs–511µs | 2.0K ops/s | 253.36ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| bubbletea | 1 | 689µs | 35.5% | 668µs–710µs | 1.5K ops/s | 344.66ms | 16.2MB | 16.0KB | 7.8MB | 16.0KB |
| rich | 1 | 985µs | 29.0% | 960µs–1.01ms | 1.0K ops/s | 493.09ms | 31.8MB | 16.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 1.02ms | 27.4% | 999µs–1.05ms | 977 ops/s | 511.94ms | 36.0MB | 16.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.26ms | 29.4% | 1.23ms–1.29ms | 795 ops/s | 629.55ms | 42.2MB | 28.0KB | 937.5KB | 1.9KB |
| blessed | 1 | 2.25ms | 68.1% | 2.12ms–2.39ms | 444 ops/s | 1.13s | 451.5MB | 191.7MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 7.32ms | 27.6% | 7.14ms–7.49ms | 137 ops/s | 3.66s | 47.3MB | 564.0KB | 950.2KB | 1.9KB |
| ink | 1 | 22.79ms | 22.4% | 22.34ms–23.24ms | 44 ops/s | 11.39s | 188.9MB | 81.1MB | 1.6MB | 3.3KB |

## tree-construction (items=1000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 108µs | 26.3% | 105µs–110µs | 9.3K ops/s | 53.91ms | 4.2MB | 124.0KB | 1.2MB | 2.4KB |
| TUIX.Renderer | 1 | 109µs | 65.4% | 103µs–115µs | 9.2K ops/s | 54.61ms | 4.0MB | 48.0KB | 1.2MB | 2.5KB |
| opentui-core | 1 | 110µs | 50.6% | 105µs–115µs | 9.1K ops/s | 437.77ms | 35.1MB | 0.00KB | 33.8MB | 69.3KB |
| TUIX.Py | 1 | 241µs | 31.7% | 234µs–248µs | 4.1K ops/s | 120.98ms | 19.1MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-react | 1 | 367µs | 144.8% | 320µs–414µs | 2.7K ops/s | 236.88ms | 38.2MB | 1.6MB | 8.9MB | 18.2KB |
| rezitui | 1 | 424µs | 226.2% | 340µs–508µs | 2.4K ops/s | 212.17ms | 188.5MB | 19.8MB | 668.5KB | 1.3KB |
| rich | 1 | 1.00ms | 27.0% | 978µs–1.03ms | 998 ops/s | 501.29ms | 31.9MB | 16.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 1.01ms | 27.6% | 981µs–1.03ms | 994 ops/s | 503.03ms | 36.0MB | 16.0KB | 948.7KB | 1.9KB |
| ratatui | 1 | 1.10ms | 26.5% | 1.07ms–1.12ms | 913 ops/s | 547.59ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| bubbletea | 1 | 1.17ms | 26.4% | 1.14ms–1.19ms | 858 ops/s | 583.26ms | 16.2MB | 0.00KB | 7.8MB | 16.0KB |
| urwid | 1 | 1.29ms | 25.7% | 1.27ms–1.32ms | 772 ops/s | 647.66ms | 42.3MB | 24.0KB | 937.5KB | 1.9KB |
| blessed | 1 | 4.44ms | 46.3% | 4.26ms–4.62ms | 225 ops/s | 2.22s | 527.6MB | 75.8MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 13.22ms | 24.4% | 12.94ms–13.51ms | 76 ops/s | 6.61s | 55.5MB | 8.1MB | 950.2KB | 1.9KB |
| ink | 1 | 45.27ms | 20.6% | 44.45ms–46.08ms | 22 ops/s | 22.63s | 364.8MB | 172.5MB | 3.3MB | 6.7KB |

## rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 9µs | 29.2% | 9µs–9µs | 114.3K ops/s | 9.42ms | 19.2MB | 32.0KB | 1.8MB | 1.9KB |
| TUIX.Renderer | 1 | 10µs | 35.9% | 10µs–10µs | 98.0K ops/s | 10.25ms | 4.0MB | 0.00KB | 15.6KB | 16B |
| TUIX.Core | 1 | 15µs | 31.5% | 14µs–15µs | 68.2K ops/s | 14.71ms | 4.3MB | 0.00KB | 15.6KB | 16B |
| blessed | 1 | 27µs | 29.9% | 27µs–28µs | 36.5K ops/s | 27.51ms | 526.7MB | 60.0KB | 0.00KB | 0.00KB |
| ratatui | 1 | 46µs | 55.4% | 45µs–48µs | 21.7K ops/s | 46.23ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| opentui-react | 1 | 50µs | 67.6% | 48µs–53µs | 19.8K ops/s | 50.54ms | 38.2MB | 0.00KB | 3.9KB | 4B |
| rezitui | 1 | 60µs | 404.9% | 45µs–75µs | 16.7K ops/s | 60.22ms | 196.6MB | 8.1MB | 1.3MB | 1.3KB |
| bubbletea | 1 | 106µs | 60.7% | 102µs–110µs | 9.4K ops/s | 106.31ms | 16.2MB | 0.00KB | 15.6MB | 16.0KB |
| opentui-core | 1 | 112µs | 64.0% | 107µs–116µs | 9.0K ops/s | 111.82ms | 35.1MB | 0.00KB | 66.9MB | 68.5KB |
| terminal-kit | 1 | 275µs | 31.6% | 270µs–281µs | 3.6K ops/s | 275.53ms | 55.4MB | 0.00KB | 1.9MB | 1.9KB |
| ink | 1 | 642µs | 36.3% | 628µs–657µs | 1.6K ops/s | 642.37ms | 364.7MB | 44.0KB | 1.9MB | 1.9KB |
| rich | 1 | 1.25ms | 32.2% | 1.23ms–1.28ms | 799 ops/s | 1.25s | 31.9MB | 28.0KB | 1.9MB | 1.9KB |
| prompt-toolkit | 1 | 1.31ms | 30.9% | 1.28ms–1.33ms | 764 ops/s | 1.31s | 36.1MB | 104.0KB | 1.9MB | 1.9KB |
| urwid | 1 | 1.54ms | 28.1% | 1.51ms–1.57ms | 649 ops/s | 1.54s | 42.3MB | 84.0KB | 1.8MB | 1.9KB |

## content-update

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 68µs | 67.3% | 64µs–72µs | 14.7K ops/s | 34.03ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| rezitui | 1 | 71µs | 376.9% | 47µs–94µs | 14.1K ops/s | 35.70ms | 190.5MB | 0.00KB | 668.5KB | 1.3KB |
| TUIX.Renderer | 1 | 138µs | 15.8% | 136µs–139µs | 7.3K ops/s | 74.77ms | 4.0MB | 0.00KB | 5.1MB | 10.5KB |
| TUIX.Py | 1 | 153µs | 44.7% | 147µs–159µs | 6.5K ops/s | 84.50ms | 19.3MB | 72.0KB | 17.1MB | 35.0KB |
| opentui-react | 1 | 181µs | 49.2% | 173µs–189µs | 5.5K ops/s | 98.42ms | 39.1MB | 892.0KB | 15.3MB | 31.3KB |
| TUIX.Core | 1 | 182µs | 27.9% | 178µs–187µs | 5.5K ops/s | 91.07ms | 4.2MB | 0.00KB | 5.0MB | 10.2KB |
| opentui-core | 1 | 186µs | 52.9% | 178µs–195µs | 5.4K ops/s | 98.33ms | 35.1MB | 0.00KB | 33.4MB | 68.5KB |
| bubbletea | 1 | 252µs | 37.9% | 244µs–260µs | 4.0K ops/s | 126.07ms | 16.2MB | 8.0KB | 7.8MB | 16.0KB |
| blessed | 1 | 295µs | 53.4% | 281µs–309µs | 3.4K ops/s | 147.57ms | 527.4MB | 0.00KB | 1.1MB | 2.3KB |
| terminal-kit | 1 | 426µs | 26.5% | 416µs–436µs | 2.3K ops/s | 213.11ms | 55.7MB | 0.00KB | 950.2KB | 1.9KB |
| prompt-toolkit | 1 | 1.25ms | 28.8% | 1.21ms–1.28ms | 803 ops/s | 622.82ms | 36.1MB | 0.00KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.35ms | 24.8% | 1.32ms–1.38ms | 742 ops/s | 673.91ms | 42.3MB | 0.00KB | 937.5KB | 1.9KB |
| rich | 1 | 1.49ms | 30.2% | 1.45ms–1.53ms | 669 ops/s | 747.54ms | 32.3MB | 332.0KB | 948.7KB | 1.9KB |
| ink | 1 | 3.13ms | 29.4% | 3.05ms–3.21ms | 319 ops/s | 1.57s | 365.5MB | 564.0KB | 953.1KB | 1.9KB |

## layout-stress (cols=4,rows=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 72µs | 12.6% | 71µs–73µs | 13.9K ops/s | 23.49ms | 4.0MB | 0.00KB | 697.6KB | 2.3KB |
| opentui-core | 1 | 80µs | 41.0% | 76µs–84µs | 12.5K ops/s | 31.96ms | 35.1MB | 0.00KB | 19.8MB | 67.7KB |
| opentui-react | 1 | 80µs | 40.8% | 76µs–84µs | 12.5K ops/s | 27.19ms | 39.1MB | 0.00KB | 1.2KB | 4B |
| TUIX.Core | 1 | 81µs | 16.1% | 80µs–83µs | 12.3K ops/s | 24.44ms | 4.2MB | 0.00KB | 668.6KB | 2.2KB |
| ratatui | 1 | 89µs | 58.8% | 83µs–95µs | 11.2K ops/s | 26.82ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| TUIX.Py | 1 | 109µs | 38.5% | 104µs–114µs | 9.2K ops/s | 36.24ms | 19.3MB | 0.00KB | 1.1MB | 3.6KB |
| bubbletea | 1 | 172µs | 46.9% | 163µs–181µs | 5.8K ops/s | 51.63ms | 16.2MB | 0.00KB | 4.7MB | 16.0KB |
| blessed | 1 | 189µs | 155.0% | 156µs–222µs | 5.3K ops/s | 56.77ms | 527.7MB | 0.00KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 656µs | 27.2% | 636µs–676µs | 1.5K ops/s | 196.91ms | 55.5MB | 0.00KB | 570.1KB | 1.9KB |
| rich | 1 | 964µs | 32.5% | 929µs–1000µs | 1.0K ops/s | 289.47ms | 32.3MB | 48.0KB | 569.2KB | 1.9KB |
| prompt-toolkit | 1 | 1.12ms | 30.6% | 1.08ms–1.16ms | 892 ops/s | 336.28ms | 36.2MB | 52.0KB | 569.2KB | 1.9KB |
| urwid | 1 | 1.26ms | 25.8% | 1.22ms–1.30ms | 794 ops/s | 378.06ms | 42.3MB | 0.00KB | 562.5KB | 1.9KB |
| rezitui | 1 | 1.39ms | 47.1% | 1.32ms–1.46ms | 719 ops/s | 417.28ms | 191.7MB | 2.0MB | 1.6MB | 5.6KB |
| ink | 1 | 5.88ms | 28.4% | 5.69ms–6.07ms | 170 ops/s | 1.76s | 365.9MB | 712.0KB | 1.2MB | 4.2KB |

## scroll-stress (items=2000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| opentui-react | 1 | 46µs | 60.1% | 39µs–54µs | 21.5K ops/s | 2.41ms | 39.1MB | 0.00KB | 135.6KB | 2.7KB |
| rezitui | 1 | 73µs | 77.9% | 57µs–89µs | 13.7K ops/s | 3.67ms | 191.7MB | 16.0KB | 66.8KB | 1.3KB |
| opentui-core | 1 | 91µs | 49.4% | 79µs–104µs | 11.0K ops/s | 5.14ms | 35.3MB | 124.0KB | 3.1MB | 62.8KB |
| bubbletea | 1 | 102µs | 35.7% | 92µs–112µs | 9.8K ops/s | 5.12ms | 16.2MB | 0.00KB | 798.0KB | 16.0KB |
| ratatui | 1 | 111µs | 11.5% | 107µs–114µs | 9.0K ops/s | 5.54ms | 27.0KB | 0.00KB | 798.0KB | 16.0KB |
| TUIX.Core | 1 | 247µs | 37.0% | 222µs–273µs | 4.0K ops/s | 12.37ms | 4.2MB | 0.00KB | 1.4MB | 29.3KB |
| TUIX.Py | 1 | 311µs | 25.0% | 289µs–333µs | 3.2K ops/s | 15.83ms | 19.3MB | 0.00KB | 1.6MB | 32.4KB |
| TUIX.Renderer | 1 | 314µs | 21.6% | 295µs–333µs | 3.2K ops/s | 15.86ms | 4.0MB | 0.00KB | 1.5MB | 30.7KB |
| rich | 1 | 545µs | 18.0% | 517µs–572µs | 1.8K ops/s | 27.25ms | 32.4MB | 84.0KB | 94.9KB | 1.9KB |
| prompt-toolkit | 1 | 745µs | 13.6% | 717µs–773µs | 1.3K ops/s | 37.27ms | 36.2MB | 0.00KB | 94.9KB | 1.9KB |
| terminal-kit | 1 | 931µs | 26.4% | 863µs–999µs | 1.1K ops/s | 46.58ms | 55.6MB | 0.00KB | 95.0KB | 1.9KB |
| urwid | 1 | 1.20ms | 24.1% | 1.12ms–1.28ms | 832 ops/s | 60.13ms | 42.3MB | 0.00KB | 93.8KB | 1.9KB |
| ink | 1 | 1.76ms | 46.2% | 1.53ms–1.98ms | 569 ops/s | 87.88ms | 365.9MB | 4.0KB | 93.2KB | 1.9KB |
| blessed | 1 | 4.58ms | 36.5% | 4.11ms–5.04ms | 218 ops/s | 228.93ms | 527.3MB | 0.00KB | 115.5KB | 2.3KB |

## virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| opentui-react | 1 | 62µs | 39.6% | 60µs–63µs | 16.2K ops/s | 64.01ms | 39.1MB | 0.00KB | 1.8MB | 1.8KB |
| ratatui | 1 | 71µs | 45.9% | 69µs–73µs | 14.0K ops/s | 71.48ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| blessed | 1 | 98µs | 279.6% | 81µs–115µs | 10.2K ops/s | 98.14ms | 529.4MB | 0.00KB | 616.2KB | 631B |
| opentui-core | 1 | 104µs | 54.3% | 101µs–108µs | 9.6K ops/s | 127.17ms | 35.3MB | 0.00KB | 66.1MB | 67.7KB |
| rezitui | 1 | 111µs | 209.7% | 96µs–125µs | 9.0K ops/s | 110.82ms | 193.6MB | 1.7MB | 2.4MB | 2.5KB |
| bubbletea | 1 | 113µs | 72.5% | 107µs–118µs | 8.9K ops/s | 112.77ms | 20.2MB | 0.00KB | 15.6MB | 16.0KB |
| TUIX.Renderer | 1 | 120µs | 43.4% | 117µs–123µs | 8.3K ops/s | 126.94ms | 4.0MB | 0.00KB | 627.0KB | 642B |
| TUIX.Core | 1 | 136µs | 42.0% | 132µs–139µs | 7.4K ops/s | 135.85ms | 4.3MB | 0.00KB | 601.6KB | 616B |
| TUIX.Py | 1 | 156µs | 41.6% | 152µs–160µs | 6.4K ops/s | 165.65ms | 19.3MB | 0.00KB | 2.2MB | 2.3KB |
| rich | 1 | 534µs | 31.2% | 524µs–545µs | 1.9K ops/s | 534.93ms | 32.0MB | 0.00KB | 1.9MB | 1.9KB |
| terminal-kit | 1 | 540µs | 34.1% | 528µs–551µs | 1.9K ops/s | 540.11ms | 96.7MB | 5.4MB | 1.9MB | 1.9KB |
| prompt-toolkit | 1 | 622µs | 28.5% | 611µs–633µs | 1.6K ops/s | 622.73ms | 36.3MB | 88.0KB | 1.9MB | 1.9KB |
| ink | 1 | 745µs | 45.3% | 724µs–766µs | 1.3K ops/s | 745.48ms | 366.8MB | 0.00KB | 153.3KB | 157B |
| urwid | 1 | 843µs | 23.9% | 830µs–855µs | 1.2K ops/s | 843.41ms | 42.4MB | 64.0KB | 1.8MB | 1.9KB |

## tables (cols=8,rows=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 86µs | 25.5% | 84µs–89µs | 11.6K ops/s | 25.89ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| TUIX.Py | 1 | 105µs | 46.6% | 99µs–110µs | 9.6K ops/s | 35.81ms | 19.3MB | 0.00KB | 4.2MB | 14.3KB |
| TUIX.Renderer | 1 | 107µs | 29.8% | 103µs–111µs | 9.3K ops/s | 34.74ms | 4.0MB | 0.00KB | 1.2MB | 3.9KB |
| TUIX.Core | 1 | 116µs | 35.7% | 111µs–121µs | 8.6K ops/s | 34.86ms | 4.3MB | 0.00KB | 1.1MB | 3.8KB |
| opentui-core | 1 | 141µs | 73.4% | 129µs–153µs | 7.1K ops/s | 53.27ms | 35.3MB | 0.00KB | 19.8MB | 67.7KB |
| bubbletea | 1 | 146µs | 46.0% | 138µs–154µs | 6.9K ops/s | 43.86ms | 20.2MB | 0.00KB | 4.7MB | 16.0KB |
| opentui-react | 1 | 254µs | 49.4% | 239µs–268µs | 3.9K ops/s | 84.03ms | 39.1MB | 0.00KB | 20.6MB | 70.4KB |
| blessed | 1 | 350µs | 43.9% | 333µs–368µs | 2.9K ops/s | 105.12ms | 529.3MB | 0.00KB | 821.5KB | 2.7KB |
| terminal-kit | 1 | 598µs | 29.9% | 578µs–618µs | 1.7K ops/s | 179.50ms | 96.3MB | 0.00KB | 570.1KB | 1.9KB |
| rezitui | 1 | 716µs | 61.7% | 666µs–766µs | 1.4K ops/s | 214.94ms | 192.2MB | 100.0KB | 2.4MB | 8.0KB |
| ink | 1 | 785µs | 33.3% | 755µs–814µs | 1.3K ops/s | 235.55ms | 366.8MB | 0.00KB | 569.2KB | 1.9KB |
| rich | 1 | 914µs | 30.2% | 883µs–945µs | 1.1K ops/s | 274.44ms | 32.1MB | 52.0KB | 569.2KB | 1.9KB |
| prompt-toolkit | 1 | 1.21ms | 21.5% | 1.18ms–1.24ms | 829 ops/s | 361.92ms | 36.3MB | 8.0KB | 569.2KB | 1.9KB |
| urwid | 1 | 1.30ms | 30.2% | 1.26ms–1.35ms | 768 ops/s | 390.56ms | 42.4MB | 0.00KB | 562.5KB | 1.9KB |

## memory-profile

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 85µs | 46.5% | 83µs–87µs | 11.8K ops/s | 169.75ms | 27.0KB | 0.00KB | 31.2MB | 16.0KB |
| rezitui | 1 | 116µs | 397.0% | 95µs–136µs | 8.7K ops/s | 231.41ms | 193.2MB | 752.0KB | 2.6MB | 1.3KB |
| bubbletea | 1 | 143µs | 58.4% | 139µs–146µs | 7.0K ops/s | 285.65ms | 20.2MB | 8.0KB | 31.2MB | 16.0KB |
| opentui-react | 1 | 178µs | 41.0% | 175µs–182µs | 5.6K ops/s | 373.74ms | 39.3MB | 276.0KB | 147.0MB | 75.3KB |
| opentui-core | 1 | 182µs | 49.2% | 179µs–186µs | 5.5K ops/s | 423.61ms | 35.3MB | 48.0KB | 133.8MB | 68.5KB |
| TUIX.Py | 1 | 299µs | 34.6% | 295µs–304µs | 3.3K ops/s | 661.34ms | 19.4MB | 64.0KB | 133.5MB | 68.3KB |
| TUIX.Renderer | 1 | 332µs | 34.6% | 327µs–337µs | 3.0K ops/s | 723.50ms | 4.0MB | 0.00KB | 125.6MB | 64.3KB |
| TUIX.Core | 1 | 367µs | 28.6% | 362µs–371µs | 2.7K ops/s | 733.15ms | 4.3MB | 0.00KB | 120.4MB | 61.6KB |
| blessed | 1 | 382µs | 46.1% | 375µs–390µs | 2.6K ops/s | 765.13ms | 528.8MB | 188.0KB | 4.6MB | 2.3KB |
| terminal-kit | 1 | 659µs | 30.2% | 650µs–667µs | 1.5K ops/s | 1.32s | 96.3MB | 12.0KB | 3.7MB | 1.9KB |
| ink | 1 | 934µs | 30.2% | 921µs–946µs | 1.1K ops/s | 1.87s | 366.8MB | 12.0KB | 3.7MB | 1.9KB |
| prompt-toolkit | 1 | 1.14ms | 32.3% | 1.13ms–1.16ms | 874 ops/s | 2.29s | 36.5MB | 268.0KB | 3.7MB | 1.9KB |
| urwid | 1 | 1.57ms | 29.1% | 1.55ms–1.59ms | 636 ops/s | 3.15s | 42.5MB | 64.0KB | 3.7MB | 1.9KB |
| rich | 1 | 1.84ms | 31.0% | 1.81ms–1.86ms | 544 ops/s | 3.68s | 32.5MB | 448.0KB | 3.7MB | 1.9KB |

## terminal-rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 20µs | 53.2% | 19µs–20µs | 51.1K ops/s | 20.23ms | 19.6MB | 64.0KB | 4.6MB | 4.7KB |
| TUIX.Renderer | 1 | 20µs | 55.1% | 19µs–20µs | 50.9K ops/s | 19.70ms | 4.2MB | 0.00KB | 15.6KB | 16B |
| rezitui | 1 | 36µs | 287.5% | 30µs–43µs | 27.6K ops/s | 36.34ms | 193.2MB | 8.0KB | 2.2MB | 2.2KB |
| TUIX.Core | 1 | 48µs | 68.2% | 46µs–50µs | 20.7K ops/s | 48.27ms | 4.7MB | 0.00KB | 15.6KB | 16B |
| ratatui | 1 | 116µs | 35.3% | 114µs–119µs | 8.6K ops/s | 116.27ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| opentui-react | 1 | 119µs | 45.0% | 115µs–122µs | 8.4K ops/s | 118.85ms | 39.3MB | 0.00KB | 3.9KB | 4B |
| bubbletea | 1 | 131µs | 70.6% | 125µs–136µs | 7.7K ops/s | 130.88ms | 20.2MB | 0.00KB | 40.6MB | 41.6KB |
| blessed | 1 | 163µs | 20.3% | 161µs–165µs | 6.1K ops/s | 163.46ms | 528.9MB | 0.00KB | 0.00KB | 0.00KB |
| opentui-core | 1 | 395µs | 42.9% | 384µs–405µs | 2.5K ops/s | 394.99ms | 36.1MB | 24.0KB | 167.1MB | 171.1KB |
| terminal-kit | 1 | 585µs | 33.5% | 572µs–597µs | 1.7K ops/s | 584.85ms | 96.4MB | 4.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.70ms | 30.9% | 1.67ms–1.73ms | 588 ops/s | 1.70s | 366.8MB | 12.0KB | 4.6MB | 4.7KB |
| rich | 1 | 2.77ms | 27.8% | 2.72ms–2.81ms | 361 ops/s | 2.77s | 32.6MB | 24.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 3.20ms | 27.6% | 3.15ms–3.26ms | 312 ops/s | 3.20s | 37.0MB | 208.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 3.50ms | 26.7% | 3.44ms–3.55ms | 286 ops/s | 3.50s | 42.5MB | 0.00KB | 4.6MB | 4.7KB |

## terminal-frame-fill (cols=120,dirtyLines=1,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 58µs | 219.1% | 47µs–69µs | 17.2K ops/s | 29.17ms | 191.8MB | 0.00KB | 1.1MB | 2.2KB |
| TUIX.Py | 1 | 79µs | 49.2% | 75µs–82µs | 12.7K ops/s | 40.44ms | 19.6MB | 0.00KB | 22.6MB | 46.2KB |
| TUIX.Renderer | 1 | 79µs | 37.6% | 77µs–82µs | 12.6K ops/s | 40.09ms | 4.2MB | 0.00KB | 7.8KB | 16B |
| TUIX.Core | 1 | 92µs | 46.6% | 88µs–95µs | 10.9K ops/s | 45.88ms | 4.6MB | 0.00KB | 7.8KB | 16B |
| ratatui | 1 | 106µs | 31.1% | 103µs–109µs | 9.4K ops/s | 53.23ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| opentui-react | 1 | 135µs | 47.5% | 130µs–141µs | 7.4K ops/s | 71.26ms | 39.3MB | 0.00KB | 4.6MB | 9.5KB |
| bubbletea | 1 | 152µs | 76.1% | 142µs–162µs | 6.6K ops/s | 76.02ms | 20.5MB | 256.0KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 294µs | 36.2% | 284µs–303µs | 3.4K ops/s | 147.95ms | 37.3MB | 0.00KB | 83.5MB | 171.0KB |
| blessed | 1 | 472µs | 47.0% | 452µs–491µs | 2.1K ops/s | 236.02ms | 529.0MB | 140.0KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 586µs | 34.8% | 568µs–604µs | 1.7K ops/s | 293.19ms | 96.4MB | 0.00KB | 2.3MB | 4.7KB |
| rich | 1 | 889µs | 30.1% | 866µs–913µs | 1.1K ops/s | 444.99ms | 32.7MB | 88.0KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 1.60ms | 32.9% | 1.55ms–1.64ms | 626 ops/s | 799.48ms | 37.1MB | 16.0KB | 2.3MB | 4.7KB |
| ink | 1 | 1.65ms | 29.0% | 1.60ms–1.69ms | 608 ops/s | 823.15ms | 366.9MB | 4.0KB | 77.6KB | 159B |
| urwid | 1 | 2.12ms | 28.4% | 2.06ms–2.17ms | 473 ops/s | 1.06s | 42.5MB | 4.0KB | 2.3MB | 4.7KB |

## terminal-frame-fill (cols=120,dirtyLines=40,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 182µs | 370.3% | 123µs–241µs | 5.5K ops/s | 90.99ms | 210.0MB | 18.2MB | 1.1MB | 2.2KB |
| ratatui | 1 | 224µs | 41.1% | 216µs–232µs | 4.5K ops/s | 112.01ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| bubbletea | 1 | 331µs | 54.3% | 316µs–347µs | 3.0K ops/s | 165.74ms | 20.5MB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 338µs | 34.4% | 327µs–348µs | 3.0K ops/s | 190.41ms | 37.3MB | 0.00KB | 83.5MB | 171.0KB |
| opentui-react | 1 | 481µs | 60.2% | 456µs–507µs | 2.1K ops/s | 254.15ms | 39.3MB | 0.00KB | 92.3MB | 189.0KB |
| TUIX.Py | 1 | 575µs | 25.3% | 562µs–587µs | 1.7K ops/s | 308.97ms | 19.5MB | 12.0KB | 42.9MB | 87.8KB |
| TUIX.Renderer | 1 | 726µs | 32.0% | 705µs–746µs | 1.4K ops/s | 383.05ms | 4.2MB | 0.00KB | 42.8MB | 87.6KB |
| TUIX.Core | 1 | 727µs | 21.5% | 714µs–741µs | 1.4K ops/s | 363.74ms | 4.6MB | 0.00KB | 41.7MB | 85.4KB |
| blessed | 1 | 817µs | 32.2% | 794µs–840µs | 1.2K ops/s | 408.84ms | 529.3MB | 352.0KB | 2.7MB | 5.5KB |
| terminal-kit | 1 | 1.37ms | 28.9% | 1.33ms–1.40ms | 732 ops/s | 683.30ms | 96.5MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 1.75ms | 28.4% | 1.71ms–1.79ms | 572 ops/s | 874.66ms | 366.9MB | 0.00KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 3.09ms | 28.1% | 3.01ms–3.16ms | 324 ops/s | 1.54s | 37.6MB | 460.0KB | 2.3MB | 4.7KB |
| urwid | 1 | 3.41ms | 26.8% | 3.33ms–3.49ms | 294 ops/s | 1.70s | 42.6MB | 84.0KB | 2.3MB | 4.7KB |
| rich | 1 | 4.24ms | 27.7% | 4.13ms–4.34ms | 236 ops/s | 2.12s | 32.9MB | 80.0KB | 2.3MB | 4.7KB |

## terminal-screen-transition (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 166µs | 371.5% | 128µs–204µs | 6.0K ops/s | 166.11ms | 210.2MB | 0.00KB | 2.2MB | 2.2KB |
| bubbletea | 1 | 218µs | 55.4% | 211µs–226µs | 4.6K ops/s | 218.67ms | 20.5MB | 0.00KB | 40.6MB | 41.6KB |
| ratatui | 1 | 253µs | 31.2% | 248µs–258µs | 4.0K ops/s | 252.89ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| opentui-core | 1 | 311µs | 49.2% | 301µs–320µs | 3.2K ops/s | 362.00ms | 37.3MB | 0.00KB | 171.1MB | 175.2KB |
| opentui-react | 1 | 467µs | 38.0% | 456µs–478µs | 2.1K ops/s | 484.25ms | 39.3MB | 0.00KB | 187.1MB | 191.6KB |
| TUIX.Py | 1 | 552µs | 43.2% | 537µs–567µs | 1.8K ops/s | 597.15ms | 19.9MB | 32.0KB | 166.7MB | 170.8KB |
| TUIX.Renderer | 1 | 584µs | 43.7% | 568µs–600µs | 1.7K ops/s | 626.46ms | 4.3MB | 0.00KB | 156.9MB | 160.7KB |
| TUIX.Core | 1 | 636µs | 43.8% | 619µs–654µs | 1.6K ops/s | 636.55ms | 4.7MB | 0.00KB | 153.0MB | 156.7KB |
| blessed | 1 | 893µs | 33.8% | 874µs–912µs | 1.1K ops/s | 893.21ms | 529.4MB | 4.0KB | 5.3MB | 5.4KB |
| terminal-kit | 1 | 1.41ms | 28.2% | 1.38ms–1.43ms | 711 ops/s | 1.41s | 96.5MB | 4.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.65ms | 31.2% | 1.61ms–1.68ms | 608 ops/s | 1.65s | 366.3MB | 160.0KB | 4.6MB | 4.7KB |
| rich | 1 | 1.91ms | 28.7% | 1.87ms–1.94ms | 525 ops/s | 1.91s | 32.8MB | 0.00KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 2.13ms | 26.9% | 2.09ms–2.16ms | 470 ops/s | 2.13s | 37.7MB | 144.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 2.66ms | 27.1% | 2.61ms–2.70ms | 376 ops/s | 2.66s | 42.6MB | 4.0KB | 4.6MB | 4.7KB |

## terminal-fps-stream (channels=12,cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 60µs | 34.4% | 59µs–61µs | 16.7K ops/s | 75.07ms | 19.7MB | 84.0KB | 8.4MB | 7.2KB |
| TUIX.Renderer | 1 | 66µs | 28.5% | 65µs–67µs | 15.2K ops/s | 79.84ms | 4.3MB | 0.00KB | 2.2MB | 1.9KB |
| TUIX.Core | 1 | 115µs | 33.0% | 113µs–117µs | 8.7K ops/s | 138.03ms | 4.7MB | 0.00KB | 2.2MB | 1.9KB |
| bubbletea | 1 | 190µs | 60.9% | 184µs–197µs | 5.3K ops/s | 228.90ms | 20.5MB | 0.00KB | 48.8MB | 41.6KB |
| ratatui | 1 | 220µs | 37.3% | 215µs–225µs | 4.5K ops/s | 264.20ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 244µs | 39.2% | 239µs–250µs | 4.1K ops/s | 358.20ms | 37.3MB | 0.00KB | 193.0MB | 164.7KB |
| opentui-react | 1 | 257µs | 39.5% | 251µs–263µs | 3.9K ops/s | 328.38ms | 39.3MB | 0.00KB | 202.0MB | 172.3KB |
| blessed | 1 | 424µs | 46.9% | 412µs–435µs | 2.4K ops/s | 508.63ms | 529.4MB | 4.0KB | 5.7MB | 4.8KB |
| rezitui | 1 | 519µs | 129.6% | 481µs–557µs | 1.9K ops/s | 623.71ms | 212.9MB | 3.3MB | 4.2MB | 3.6KB |
| terminal-kit | 1 | 1.30ms | 30.9% | 1.27ms–1.32ms | 771 ops/s | 1.56s | 96.6MB | 8.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.55ms | 29.6% | 1.52ms–1.57ms | 646 ops/s | 1.86s | 366.7MB | 8.0KB | 2.5MB | 2.1KB |
| rich | 1 | 1.91ms | 28.7% | 1.88ms–1.94ms | 523 ops/s | 2.30s | 32.9MB | 16.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.16ms | 30.5% | 2.12ms–2.20ms | 463 ops/s | 2.59s | 37.8MB | 88.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.73ms | 27.7% | 2.69ms–2.78ms | 366 ops/s | 3.28s | 42.6MB | 32.0KB | 5.5MB | 4.7KB |

## terminal-input-latency (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 27µs | 76.1% | 26µs–28µs | 36.9K ops/s | 28.30ms | 19.8MB | 0.00KB | 4.6MB | 4.7KB |
| TUIX.Renderer | 1 | 32µs | 56.7% | 31µs–33µs | 31.2K ops/s | 32.05ms | 4.3MB | 0.00KB | 15.6KB | 16B |
| TUIX.Core | 1 | 57µs | 34.7% | 55µs–58µs | 17.7K ops/s | 56.64ms | 4.7MB | 0.00KB | 15.6KB | 16B |
| rezitui | 1 | 73µs | 318.8% | 58µs–87µs | 13.8K ops/s | 72.71ms | 210.3MB | 0.00KB | 2.2MB | 2.2KB |
| ratatui | 1 | 97µs | 42.0% | 95µs–100µs | 10.3K ops/s | 97.54ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| opentui-react | 1 | 133µs | 44.3% | 129µs–136µs | 7.5K ops/s | 160.73ms | 39.3MB | 0.00KB | 833.0KB | 853B |
| opentui-core | 1 | 301µs | 36.1% | 294µs–307µs | 3.3K ops/s | 301.42ms | 37.3MB | 0.00KB | 166.6MB | 170.6KB |
| bubbletea | 1 | 325µs | 57.5% | 313µs–336µs | 3.1K ops/s | 325.11ms | 20.5MB | 0.00KB | 40.6MB | 41.6KB |
| blessed | 1 | 468µs | 42.9% | 456µs–481µs | 2.1K ops/s | 468.74ms | 529.5MB | 36.0KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 547µs | 33.6% | 536µs–559µs | 1.8K ops/s | 547.77ms | 97.0MB | 388.0KB | 4.6MB | 4.7KB |
| rich | 1 | 1.52ms | 29.2% | 1.49ms–1.54ms | 659 ops/s | 1.52s | 33.4MB | 500.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.90ms | 29.4% | 1.87ms–1.94ms | 525 ops/s | 1.91s | 366.9MB | 0.00KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 2.21ms | 27.2% | 2.17ms–2.25ms | 452 ops/s | 2.21s | 37.8MB | 4.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 2.55ms | 27.0% | 2.51ms–2.60ms | 391 ops/s | 2.56s | 42.7MB | 40.0KB | 4.6MB | 4.7KB |

## terminal-memory-soak (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| bubbletea | 1 | 230µs | 54.9% | 223µs–237µs | 4.3K ops/s | 276.61ms | 20.5MB | 0.00KB | 48.8MB | 41.6KB |
| ratatui | 1 | 246µs | 36.0% | 241µs–251µs | 4.1K ops/s | 295.48ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| rezitui | 1 | 257µs | 368.2% | 204µs–311µs | 3.9K ops/s | 309.23ms | 210.6MB | 352.0KB | 2.6MB | 2.2KB |
| opentui-core | 1 | 415µs | 38.3% | 406µs–424µs | 2.4K ops/s | 559.07ms | 37.3MB | 0.00KB | 200.4MB | 171.0KB |
| opentui-react | 1 | 419µs | 35.2% | 411µs–428µs | 2.4K ops/s | 514.74ms | 39.3MB | 0.00KB | 222.5MB | 189.9KB |
| TUIX.Renderer | 1 | 876µs | 35.0% | 859µs–893µs | 1.1K ops/s | 1.14s | 4.3MB | 0.00KB | 188.3MB | 160.7KB |
| TUIX.Py | 1 | 888µs | 49.1% | 863µs–912µs | 1.1K ops/s | 1.17s | 19.8MB | 68.0KB | 200.1MB | 170.8KB |
| blessed | 1 | 895µs | 37.9% | 876µs–914µs | 1.1K ops/s | 1.07s | 529.5MB | 0.00KB | 6.5MB | 5.5KB |
| TUIX.Core | 1 | 1.19ms | 43.8% | 1.16ms–1.22ms | 842 ops/s | 1.43s | 4.7MB | 0.00KB | 183.6MB | 156.7KB |
| terminal-kit | 1 | 1.48ms | 30.4% | 1.46ms–1.51ms | 674 ops/s | 1.78s | 97.0MB | 4.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.92ms | 29.1% | 1.89ms–1.95ms | 521 ops/s | 2.30s | 366.9MB | 8.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.97ms | 27.7% | 2.93ms–3.02ms | 336 ops/s | 3.57s | 37.9MB | 48.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 3.62ms | 27.3% | 3.56ms–3.67ms | 277 ops/s | 4.34s | 42.7MB | 44.0KB | 5.5MB | 4.7KB |
| rich | 1 | 3.88ms | 29.2% | 3.81ms–3.94ms | 258 ops/s | 4.65s | 33.5MB | 0.00KB | 5.5MB | 4.7KB |

## terminal-full-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 193µs | 31.6% | 190µs–197µs | 5.2K ops/s | 232.41ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 293µs | 44.2% | 285µs–300µs | 3.4K ops/s | 390.70ms | 37.3MB | 0.00KB | 204.3MB | 174.4KB |
| TUIX.Renderer | 1 | 352µs | 33.5% | 345µs–359µs | 2.8K ops/s | 447.40ms | 4.3MB | 0.00KB | 3.8MB | 3.2KB |
| opentui-react | 1 | 365µs | 49.5% | 355µs–375µs | 2.7K ops/s | 460.09ms | 39.3MB | 0.00KB | 149.5MB | 127.6KB |
| TUIX.Py | 1 | 383µs | 46.6% | 373µs–393µs | 2.6K ops/s | 488.05ms | 19.8MB | 0.00KB | 6.5MB | 5.6KB |
| TUIX.Core | 1 | 413µs | 44.2% | 403µs–424µs | 2.4K ops/s | 496.19ms | 4.7MB | 0.00KB | 3.8MB | 3.2KB |
| bubbletea | 1 | 466µs | 46.6% | 454µs–478µs | 2.1K ops/s | 559.66ms | 20.5MB | 0.00KB | 48.8MB | 41.6KB |
| blessed | 1 | 696µs | 36.5% | 681µs–710µs | 1.4K ops/s | 835.01ms | 529.2MB | 24.0KB | 3.8MB | 3.2KB |
| rezitui | 1 | 853µs | 72.1% | 818µs–887µs | 1.2K ops/s | 1.02s | 214.6MB | 2.6MB | 4.3MB | 3.7KB |
| terminal-kit | 1 | 1.11ms | 29.7% | 1.09ms–1.13ms | 901 ops/s | 1.33s | 97.2MB | 12.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.71ms | 29.2% | 1.68ms–1.74ms | 585 ops/s | 2.05s | 367.2MB | 184.0KB | 3.4MB | 2.9KB |
| rich | 1 | 1.80ms | 30.1% | 1.77ms–1.83ms | 555 ops/s | 2.16s | 33.7MB | 200.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.27ms | 30.6% | 2.23ms–2.31ms | 440 ops/s | 2.73s | 37.9MB | 8.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.81ms | 28.5% | 2.77ms–2.86ms | 355 ops/s | 3.38s | 42.8MB | 84.0KB | 5.5MB | 4.7KB |

## terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 178µs | 31.1% | 175µs–182µs | 5.6K ops/s | 214.16ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 307µs | 42.4% | 300µs–315µs | 3.3K ops/s | 379.27ms | 39.3MB | 0.00KB | 149.5MB | 127.6KB |
| opentui-core | 1 | 337µs | 56.0% | 327µs–348µs | 3.0K ops/s | 439.28ms | 37.3MB | 0.00KB | 204.3MB | 174.4KB |
| TUIX.Renderer | 1 | 434µs | 55.3% | 420µs–447µs | 2.3K ops/s | 561.90ms | 4.3MB | 0.00KB | 69.7MB | 59.5KB |
| bubbletea | 1 | 446µs | 45.4% | 435µs–458µs | 2.2K ops/s | 535.68ms | 20.5MB | 0.00KB | 48.8MB | 41.6KB |
| TUIX.Py | 1 | 464µs | 59.7% | 448µs–479µs | 2.2K ops/s | 600.96ms | 19.9MB | 64.0KB | 74.7MB | 63.7KB |
| TUIX.Core | 1 | 546µs | 57.6% | 529µs–564µs | 1.8K ops/s | 655.88ms | 4.7MB | 0.00KB | 69.7MB | 59.5KB |
| blessed | 1 | 721µs | 36.8% | 706µs–736µs | 1.4K ops/s | 865.40ms | 529.2MB | 12.0KB | 3.8MB | 3.2KB |
| rezitui | 1 | 759µs | 86.9% | 722µs–796µs | 1.3K ops/s | 911.28ms | 213.9MB | 88.0KB | 4.4MB | 3.7KB |
| terminal-kit | 1 | 1.09ms | 36.4% | 1.07ms–1.11ms | 917 ops/s | 1.31s | 97.3MB | 72.0KB | 5.5MB | 4.7KB |
| rich | 1 | 1.79ms | 29.1% | 1.76ms–1.82ms | 559 ops/s | 2.15s | 33.8MB | 88.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.83ms | 29.1% | 1.80ms–1.86ms | 546 ops/s | 2.20s | 367.3MB | 0.00KB | 5.4MB | 4.6KB |
| prompt-toolkit | 1 | 2.37ms | 27.2% | 2.34ms–2.41ms | 422 ops/s | 2.85s | 38.0MB | 36.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.74ms | 30.7% | 2.70ms–2.79ms | 365 ops/s | 3.29s | 42.9MB | 64.0KB | 5.5MB | 4.7KB |

## terminal-strict-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 163µs | 36.3% | 159µs–166µs | 6.1K ops/s | 195.40ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 311µs | 38.2% | 305µs–318µs | 3.2K ops/s | 374.16ms | 37.3MB | 0.00KB | 186.1MB | 158.8KB |
| TUIX.Py | 1 | 380µs | 45.3% | 370µs–389µs | 2.6K ops/s | 482.89ms | 20.0MB | 64.0KB | 6.5MB | 5.6KB |
| TUIX.Renderer | 1 | 395µs | 213.6% | 347µs–443µs | 2.5K ops/s | 500.49ms | 4.4MB | 0.00KB | 3.8MB | 3.2KB |
| TUIX.Core | 1 | 408µs | 35.1% | 400µs–416µs | 2.4K ops/s | 489.98ms | 4.7MB | 0.00KB | 3.8MB | 3.2KB |
| bubbletea | 1 | 478µs | 46.0% | 465µs–490µs | 2.1K ops/s | 573.55ms | 20.5MB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 478µs | 32.1% | 470µs–487µs | 2.1K ops/s | 574.76ms | 39.3MB | 0.00KB | 240.6MB | 205.3KB |
| rezitui | 1 | 661µs | 88.4% | 628µs–694µs | 1.5K ops/s | 793.55ms | 214.0MB | 12.0KB | 4.3MB | 3.7KB |
| blessed | 1 | 754µs | 35.1% | 739µs–769µs | 1.3K ops/s | 904.82ms | 529.2MB | 0.00KB | 3.8MB | 3.2KB |
| terminal-kit | 1 | 1.10ms | 33.5% | 1.08ms–1.12ms | 910 ops/s | 1.32s | 97.4MB | 32.0KB | 5.5MB | 4.7KB |
| rich | 1 | 1.58ms | 24.4% | 1.56ms–1.61ms | 632 ops/s | 1.90s | 33.8MB | 52.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.75ms | 29.6% | 1.72ms–1.78ms | 572 ops/s | 2.10s | 367.3MB | 0.00KB | 3.4MB | 2.9KB |
| prompt-toolkit | 1 | 2.35ms | 29.1% | 2.31ms–2.38ms | 426 ops/s | 2.82s | 38.1MB | 64.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.70ms | 32.0% | 2.65ms–2.75ms | 371 ops/s | 3.24s | 42.9MB | 0.00KB | 5.5MB | 4.7KB |

## terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 159µs | 35.0% | 156µs–162µs | 6.3K ops/s | 190.88ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 239µs | 30.0% | 235µs–243µs | 4.2K ops/s | 286.79ms | 37.3MB | 0.00KB | 186.1MB | 158.8KB |
| opentui-react | 1 | 485µs | 36.5% | 475µs–495µs | 2.1K ops/s | 583.05ms | 39.3MB | 0.00KB | 240.6MB | 205.3KB |
| TUIX.Renderer | 1 | 504µs | 50.9% | 490µs–519µs | 2.0K ops/s | 652.12ms | 4.4MB | 0.00KB | 69.7MB | 59.5KB |
| TUIX.Py | 1 | 507µs | 52.5% | 492µs–522µs | 2.0K ops/s | 657.25ms | 19.9MB | 0.00KB | 74.7MB | 63.7KB |
| TUIX.Core | 1 | 511µs | 55.9% | 495µs–527µs | 2.0K ops/s | 612.98ms | 4.8MB | 0.00KB | 69.7MB | 59.5KB |
| bubbletea | 1 | 542µs | 42.7% | 529µs–555µs | 1.8K ops/s | 651.05ms | 20.5MB | 0.00KB | 48.8MB | 41.6KB |
| blessed | 1 | 644µs | 39.2% | 630µs–659µs | 1.6K ops/s | 773.47ms | 529.2MB | 4.0KB | 3.8MB | 3.2KB |
| rezitui | 1 | 755µs | 84.0% | 719µs–790µs | 1.3K ops/s | 906.16ms | 214.0MB | 4.0KB | 4.4MB | 3.7KB |
| terminal-kit | 1 | 1.24ms | 30.3% | 1.22ms–1.26ms | 809 ops/s | 1.48s | 97.4MB | 0.00KB | 5.5MB | 4.7KB |
| rich | 1 | 1.61ms | 28.8% | 1.59ms–1.64ms | 620 ops/s | 1.94s | 33.8MB | 36.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.82ms | 29.2% | 1.79ms–1.85ms | 550 ops/s | 2.18s | 367.3MB | 0.00KB | 5.4MB | 4.6KB |
| prompt-toolkit | 1 | 2.39ms | 26.3% | 2.36ms–2.43ms | 418 ops/s | 2.87s | 38.1MB | 4.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.70ms | 28.3% | 2.66ms–2.74ms | 370 ops/s | 3.24s | 42.9MB | 0.00KB | 5.5MB | 4.7KB |

## terminal-virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 58µs | 274.2% | 48µs–68µs | 17.1K ops/s | 58.52ms | 214.0MB | 4.0KB | 4.1MB | 4.2KB |
| opentui-react | 1 | 145µs | 38.9% | 141µs–148µs | 6.9K ops/s | 148.32ms | 39.3MB | 0.00KB | 3.0MB | 3.1KB |
| bubbletea | 1 | 153µs | 68.8% | 147µs–160µs | 6.5K ops/s | 153.39ms | 20.5MB | 0.00KB | 40.6MB | 41.6KB |
| ratatui | 1 | 169µs | 28.0% | 166µs–172µs | 5.9K ops/s | 168.68ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| blessed | 1 | 199µs | 143.9% | 182µs–217µs | 5.0K ops/s | 199.53ms | 530.1MB | 0.00KB | 1.0MB | 1.0KB |
| opentui-core | 1 | 303µs | 42.8% | 295µs–311µs | 3.3K ops/s | 350.04ms | 37.3MB | 0.00KB | 165.1MB | 169.0KB |
| TUIX.Renderer | 1 | 747µs | 27.2% | 735µs–760µs | 1.3K ops/s | 784.76ms | 4.4MB | 0.00KB | 1.0MB | 1.1KB |
| TUIX.Core | 1 | 812µs | 29.3% | 797µs–827µs | 1.2K ops/s | 812.24ms | 4.8MB | 0.00KB | 1.0MB | 1.0KB |
| TUIX.Py | 1 | 831µs | 36.2% | 812µs–849µs | 1.2K ops/s | 865.55ms | 20.1MB | 64.0KB | 5.2MB | 5.3KB |
| rich | 1 | 876µs | 29.2% | 861µs–892µs | 1.1K ops/s | 877.14ms | 33.9MB | 36.0KB | 4.6MB | 4.7KB |
| terminal-kit | 1 | 1.24ms | 26.9% | 1.22ms–1.26ms | 806 ops/s | 1.24s | 129.3MB | 388.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 1.62ms | 31.2% | 1.59ms–1.66ms | 616 ops/s | 1.62s | 38.1MB | 64.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.68ms | 30.1% | 1.65ms–1.71ms | 594 ops/s | 1.68s | 368.2MB | 20.0KB | 262.7KB | 269B |
| urwid | 1 | 2.22ms | 28.4% | 2.18ms–2.26ms | 450 ops/s | 2.22s | 42.9MB | 32.0KB | 4.6MB | 4.7KB |

## terminal-table (cols=8,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| bubbletea | 1 | 231µs | 28.1% | 225µs–236µs | 4.3K ops/s | 115.41ms | 20.5MB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 243µs | 45.7% | 233µs–253µs | 4.1K ops/s | 148.28ms | 37.3MB | 0.00KB | 82.5MB | 169.0KB |
| ratatui | 1 | 263µs | 32.8% | 255µs–270µs | 3.8K ops/s | 131.43ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| opentui-react | 1 | 460µs | 38.5% | 444µs–475µs | 2.2K ops/s | 244.67ms | 39.3MB | 0.00KB | 91.4MB | 187.3KB |
| TUIX.Renderer | 1 | 532µs | 30.5% | 518µs–546µs | 1.9K ops/s | 289.02ms | 4.4MB | 0.00KB | 3.4MB | 7.0KB |
| TUIX.Core | 1 | 541µs | 31.5% | 526µs–556µs | 1.8K ops/s | 270.60ms | 4.8MB | 0.00KB | 3.3MB | 6.8KB |
| TUIX.Py | 1 | 560µs | 32.2% | 544µs–575µs | 1.8K ops/s | 318.92ms | 20.1MB | 0.00KB | 12.6MB | 25.8KB |
| blessed | 1 | 899µs | 38.3% | 869µs–930µs | 1.1K ops/s | 449.91ms | 530.2MB | 68.0KB | 3.0MB | 6.2KB |
| rezitui | 1 | 1.02ms | 71.7% | 954µs–1.08ms | 983 ops/s | 509.02ms | 215.6MB | 1.6MB | 7.6MB | 15.6KB |
| terminal-kit | 1 | 1.27ms | 27.6% | 1.24ms–1.30ms | 787 ops/s | 635.61ms | 129.2MB | 0.00KB | 2.3MB | 4.7KB |
| rich | 1 | 1.73ms | 35.5% | 1.67ms–1.78ms | 579 ops/s | 863.29ms | 33.9MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 1.90ms | 27.6% | 1.85ms–1.94ms | 527 ops/s | 949.59ms | 367.8MB | 0.00KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 2.65ms | 28.2% | 2.59ms–2.72ms | 377 ops/s | 1.33s | 38.1MB | 0.00KB | 2.3MB | 4.7KB |
| urwid | 1 | 3.10ms | 30.3% | 3.02ms–3.18ms | 323 ops/s | 1.55s | 42.9MB | 0.00KB | 2.3MB | 4.7KB |

## Relative Performance (vs fastest per scenario)

> Ratio = framework mean / fastest mean for each scenario. Lower is better. 1.0x = fastest.

| Scenario | blessed | terminal-kit | ink | rezitui | bubbletea | ratatui | opentui-core | opentui-react | TUIX.Renderer | TUIX.Core | TUIX.Py | rich | urwid | prompt-toolkit |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| startup | 242.2x [235.2x, 249.1x] | 4.1x [3.7x, 4.5x] | 16.5x [15.7x, 17.4x] | 1.7x [0.8x, 2.5x] | 2.1x [1.9x, 2.3x] | **1.0x** | 2.8x [2.6x, 3.1x] | 2.4x [2.2x, 2.6x] | 2.1x [2.1x, 2.2x] | 2.1x [2.0x, 2.1x] | 1.9x [1.8x, 2.0x] | 9.1x [8.9x, 9.3x] | 8.4x [8.2x, 8.6x] | 9.5x [8.8x, 10.2x] |
| tree-construction (items=10) | 1.5x [1.3x, 1.8x] | 6.1x [5.9x, 6.2x] | 9.7x [9.3x, 10.0x] | 2.0x [1.6x, 2.3x] | 3.6x [3.5x, 3.8x] | 1.8x [1.7x, 1.8x] | 7.7x [7.2x, 8.1x] | **1.0x** | 1.1x [1.1x, 1.2x] | 2.0x [2.0x, 2.0x] | 2.2x [2.1x, 2.3x] | 17.6x [17.1x, 18.1x] | 25.3x [24.7x, 25.9x] | 16.3x [16.0x, 16.7x] |
| tree-construction (items=100) | 4.6x [4.1x, 5.0x] | 11.5x [11.2x, 11.8x] | 41.7x [40.6x, 42.7x] | 1.0x [0.9x, 1.2x] | 1.9x [1.8x, 2.0x] | 1.4x [1.3x, 1.5x] | 1.4x [1.0x, 1.7x] | 1.1x [1.1x, 1.1x] | **1.0x** | 1.0x [1.0x, 1.1x] | 1.6x [1.5x, 1.7x] | 6.7x [6.5x, 6.8x] | 10.0x [9.8x, 10.3x] | 8.4x [8.1x, 8.6x] |
| tree-construction (items=500) | 21.3x [20.0x, 22.5x] | 69.0x [67.4x, 70.7x] | 215.0x [210.8x, 219.3x] | 2.6x [1.9x, 3.3x] | 6.5x [6.3x, 6.7x] | 4.8x [4.7x, 4.8x] | 1.6x [1.5x, 1.8x] | 2.2x [2.2x, 2.3x] | **1.0x** | 1.3x [1.2x, 1.3x] | 2.0x [2.0x, 2.1x] | 9.3x [9.1x, 9.5x] | 11.9x [11.6x, 12.2x] | 9.7x [9.4x, 9.9x] |
| tree-construction (items=1000) | 41.2x [39.5x, 42.8x] | 122.7x [120.1x, 125.3x] | 420.0x [412.4x, 427.6x] | 3.9x [3.2x, 4.7x] | 10.8x [10.6x, 11.1x] | 10.2x [9.9x, 10.4x] | 1.0x [1.0x, 1.1x] | 3.4x [3.0x, 3.8x] | 1.0x [1.0x, 1.1x] | **1.0x** | 2.2x [2.2x, 2.3x] | 9.3x [9.1x, 9.5x] | 12.0x [11.7x, 12.3x] | 9.3x [9.1x, 9.6x] |
| rerender | 3.1x [3.1x, 3.2x] | 31.5x [30.8x, 32.1x] | 73.4x [71.7x, 75.0x] | 6.9x [5.1x, 8.6x] | 12.1x [11.7x, 12.6x] | 5.3x [5.1x, 5.5x] | 12.8x [12.2x, 13.3x] | 5.8x [5.5x, 6.0x] | 1.2x [1.1x, 1.2x] | 1.7x [1.6x, 1.7x] | **1.0x** | 143.0x [140.1x, 145.9x] | 175.9x [172.9x, 179.0x] | 149.6x [146.8x, 152.5x] |
| content-update | 4.3x [4.1x, 4.5x] | 6.3x [6.1x, 6.4x] | 46.1x [44.9x, 47.3x] | 1.0x [0.7x, 1.4x] | 3.7x [3.6x, 3.8x] | **1.0x** | 2.7x [2.6x, 2.9x] | 2.7x [2.5x, 2.8x] | 2.0x [2.0x, 2.1x] | 2.7x [2.6x, 2.7x] | 2.2x [2.2x, 2.3x] | 22.0x [21.4x, 22.6x] | 19.8x [19.4x, 20.2x] | 18.3x [17.8x, 18.8x] |
| layout-stress (cols=4,rows=10) | 2.6x [2.2x, 3.1x] | 9.1x [8.8x, 9.4x] | 81.9x [79.2x, 84.5x] | 19.4x [18.3x, 20.4x] | 2.4x [2.3x, 2.5x] | 1.2x [1.2x, 1.3x] | 1.1x [1.1x, 1.2x] | 1.1x [1.1x, 1.2x] | **1.0x** | 1.1x [1.1x, 1.2x] | 1.5x [1.5x, 1.6x] | 13.4x [12.9x, 13.9x] | 17.5x [17.0x, 18.0x] | 15.6x [15.1x, 16.1x] |
| scroll-stress (items=2000) | 98.5x [88.6x, 108.5x] | 20.0x [18.6x, 21.5x] | 37.8x [33.0x, 42.7x] | 1.6x [1.2x, 1.9x] | 2.2x [2.0x, 2.4x] | 2.4x [2.3x, 2.5x] | 2.0x [1.7x, 2.2x] | **1.0x** | 6.8x [6.3x, 7.2x] | 5.3x [4.8x, 5.9x] | 6.7x [6.2x, 7.2x] | 11.7x [11.1x, 12.3x] | 25.9x [24.1x, 27.6x] | 16.0x [15.4x, 16.6x] |
| virtual-list (items=100000,viewport=40) | 1.6x [1.3x, 1.9x] | 8.8x [8.6x, 8.9x] | 12.1x [11.8x, 12.4x] | 1.8x [1.6x, 2.0x] | 1.8x [1.7x, 1.9x] | 1.2x [1.1x, 1.2x] | 1.7x [1.6x, 1.7x] | **1.0x** | 1.9x [1.9x, 2.0x] | 2.2x [2.1x, 2.3x] | 2.5x [2.5x, 2.6x] | 8.7x [8.5x, 8.8x] | 13.7x [13.5x, 13.9x] | 10.1x [9.9x, 10.3x] |
| tables (cols=8,rows=100) | 4.1x [3.9x, 4.3x] | 6.9x [6.7x, 7.2x] | 9.1x [8.8x, 9.4x] | 8.3x [7.7x, 8.9x] | 1.7x [1.6x, 1.8x] | **1.0x** | 1.6x [1.5x, 1.8x] | 2.9x [2.8x, 3.1x] | 1.2x [1.2x, 1.3x] | 1.3x [1.3x, 1.4x] | 1.2x [1.1x, 1.3x] | 10.6x [10.2x, 11.0x] | 15.1x [14.6x, 15.6x] | 14.0x [13.6x, 14.3x] |
| memory-profile | 4.5x [4.4x, 4.6x] | 7.8x [7.7x, 7.9x] | 11.0x [10.9x, 11.2x] | 1.4x [1.1x, 1.6x] | 1.7x [1.6x, 1.7x] | **1.0x** | 2.2x [2.1x, 2.2x] | 2.1x [2.1x, 2.1x] | 3.9x [3.9x, 4.0x] | 4.3x [4.3x, 4.4x] | 3.5x [3.5x, 3.6x] | 21.7x [21.4x, 22.0x] | 18.6x [18.3x, 18.8x] | 13.5x [13.3x, 13.7x] |
| terminal-rerender | 8.3x [8.2x, 8.4x] | 29.8x [29.2x, 30.5x] | 86.9x [85.2x, 88.5x] | 1.8x [1.5x, 2.2x] | 6.7x [6.4x, 7.0x] | 5.9x [5.8x, 6.1x] | 20.1x [19.6x, 20.7x] | 6.1x [5.9x, 6.2x] | **1.0x** | 2.5x [2.4x, 2.6x] | **1.0x** | 141.3x [138.8x, 143.7x] | 178.6x [175.6x, 181.5x] | 163.6x [160.8x, 166.4x] |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | 8.1x [7.8x, 8.4x] | 10.1x [9.8x, 10.4x] | 28.3x [27.6x, 29.0x] | **1.0x** | 2.6x [2.4x, 2.8x] | 1.8x [1.8x, 1.9x] | 5.0x [4.9x, 5.2x] | 2.3x [2.2x, 2.4x] | 1.4x [1.3x, 1.4x] | 1.6x [1.5x, 1.6x] | 1.3x [1.3x, 1.4x] | 15.3x [14.9x, 15.7x] | 36.3x [35.4x, 37.3x] | 27.5x [26.7x, 28.3x] |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | 4.5x [4.4x, 4.6x] | 7.5x [7.3x, 7.7x] | 9.6x [9.4x, 9.9x] | **1.0x** | 1.8x [1.7x, 1.9x] | 1.2x [1.2x, 1.3x] | 1.9x [1.8x, 1.9x] | 2.6x [2.5x, 2.8x] | 4.0x [3.9x, 4.1x] | 4.0x [3.9x, 4.1x] | 3.2x [3.1x, 3.2x] | 23.3x [22.7x, 23.9x] | 18.7x [18.3x, 19.2x] | 17.0x [16.6x, 17.4x] |
| terminal-screen-transition (cols=120,rows=40) | 5.4x [5.3x, 5.5x] | 8.5x [8.3x, 8.6x] | 9.9x [9.7x, 10.1x] | **1.0x** | 1.3x [1.3x, 1.4x] | 1.5x [1.5x, 1.6x] | 1.9x [1.8x, 1.9x] | 2.8x [2.8x, 2.9x] | 3.5x [3.4x, 3.6x] | 3.8x [3.7x, 3.9x] | 3.3x [3.2x, 3.4x] | 11.5x [11.3x, 11.7x] | 16.0x [15.7x, 16.3x] | 12.8x [12.6x, 13.0x] |
| terminal-fps-stream (channels=12,cols=120,rows=40) | 7.1x [6.9x, 7.3x] | 21.6x [21.3x, 22.0x] | 25.8x [25.4x, 26.3x] | 8.7x [8.0x, 9.3x] | 3.2x [3.1x, 3.3x] | 3.7x [3.6x, 3.7x] | 4.1x [4.0x, 4.2x] | 4.3x [4.2x, 4.4x] | 1.1x [1.1x, 1.1x] | 1.9x [1.9x, 2.0x] | **1.0x** | 31.9x [31.4x, 32.4x] | 45.6x [44.9x, 46.3x] | 36.1x [35.4x, 36.7x] |
| terminal-input-latency (cols=120,rows=40) | 17.3x [16.8x, 17.8x] | 20.2x [19.8x, 20.6x] | 70.3x [69.1x, 71.6x] | 2.7x [2.2x, 3.2x] | 12.0x [11.6x, 12.4x] | 3.6x [3.5x, 3.7x] | 11.1x [10.9x, 11.4x] | 4.9x [4.8x, 5.0x] | 1.2x [1.1x, 1.2x] | 2.1x [2.0x, 2.1x] | **1.0x** | 56.0x [55.0x, 57.0x] | 94.3x [92.8x, 95.9x] | 81.6x [80.3x, 83.0x] |
| terminal-memory-soak (cols=120,rows=40) | 3.9x [3.8x, 4.0x] | 6.4x [6.3x, 6.6x] | 8.3x [8.2x, 8.5x] | 1.1x [0.9x, 1.4x] | **1.0x** | 1.1x [1.0x, 1.1x] | 1.8x [1.8x, 1.8x] | 1.8x [1.8x, 1.9x] | 3.8x [3.7x, 3.9x] | 5.2x [5.0x, 5.3x] | 3.9x [3.7x, 4.0x] | 16.8x [16.6x, 17.1x] | 15.7x [15.5x, 15.9x] | 12.9x [12.7x, 13.1x] |
| terminal-full-ui (cols=120,rows=40,services=24) | 3.6x [3.5x, 3.7x] | 5.7x [5.6x, 5.8x] | 8.8x [8.7x, 9.0x] | 4.4x [4.2x, 4.6x] | 2.4x [2.3x, 2.5x] | **1.0x** | 1.5x [1.5x, 1.6x] | 1.9x [1.8x, 1.9x] | 1.8x [1.8x, 1.9x] | 2.1x [2.1x, 2.2x] | 2.0x [1.9x, 2.0x] | 9.3x [9.2x, 9.5x] | 14.6x [14.3x, 14.8x] | 11.8x [11.6x, 12.0x] |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 4.0x [4.0x, 4.1x] | 6.1x [6.0x, 6.2x] | 10.3x [10.1x, 10.4x] | 4.3x [4.0x, 4.5x] | 2.5x [2.4x, 2.6x] | **1.0x** | 1.9x [1.8x, 2.0x] | 1.7x [1.7x, 1.8x] | 2.4x [2.4x, 2.5x] | 3.1x [3.0x, 3.2x] | 2.6x [2.5x, 2.7x] | 10.0x [9.9x, 10.2x] | 15.4x [15.1x, 15.6x] | 13.3x [13.1x, 13.5x] |
| terminal-strict-ui (cols=120,rows=40,services=24) | 4.6x [4.5x, 4.7x] | 6.7x [6.6x, 6.9x] | 10.7x [10.6x, 10.9x] | 4.1x [3.9x, 4.3x] | 2.9x [2.9x, 3.0x] | **1.0x** | 1.9x [1.9x, 2.0x] | 2.9x [2.9x, 3.0x] | 2.4x [2.1x, 2.7x] | 2.5x [2.5x, 2.6x] | 2.3x [2.3x, 2.4x] | 9.7x [9.6x, 9.9x] | 16.6x [16.3x, 16.9x] | 14.4x [14.2x, 14.6x] |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 4.1x [4.0x, 4.1x] | 7.8x [7.6x, 7.9x] | 11.4x [11.3x, 11.6x] | 4.7x [4.5x, 5.0x] | 3.4x [3.3x, 3.5x] | **1.0x** | 1.5x [1.5x, 1.5x] | 3.1x [3.0x, 3.1x] | 3.2x [3.1x, 3.3x] | 3.2x [3.1x, 3.3x] | 3.2x [3.1x, 3.3x] | 10.1x [10.0x, 10.3x] | 17.0x [16.7x, 17.3x] | 15.0x [14.8x, 15.3x] |
| terminal-virtual-list (items=100000,viewport=40) | 3.4x [3.1x, 3.7x] | 21.2x [20.9x, 21.6x] | 28.9x [28.3x, 29.4x] | **1.0x** | 2.6x [2.5x, 2.7x] | 2.9x [2.8x, 2.9x] | 5.2x [5.1x, 5.3x] | 2.5x [2.4x, 2.5x] | 12.8x [12.6x, 13.0x] | 13.9x [13.7x, 14.2x] | 14.2x [13.9x, 14.6x] | 15.0x [14.7x, 15.3x] | 38.1x [37.4x, 38.7x] | 27.8x [27.3x, 28.4x] |
| terminal-table (cols=8,rows=40) | 3.9x [3.8x, 4.0x] | 5.5x [5.4x, 5.6x] | 8.2x [8.0x, 8.4x] | 4.4x [4.1x, 4.7x] | **1.0x** | 1.1x [1.1x, 1.2x] | 1.1x [1.0x, 1.1x] | 2.0x [1.9x, 2.1x] | 2.3x [2.2x, 2.4x] | 2.3x [2.3x, 2.4x] | 2.4x [2.4x, 2.5x] | 7.5x [7.3x, 7.7x] | 13.4x [13.1x, 13.8x] | 11.5x [11.2x, 11.8x] |

## Memory Comparison

| Scenario | Framework | Peak RSS | RSS Before | RSS After | RSS Growth |
|---|---|---:|---:|---:|---:|
| startup | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| startup | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| startup | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 4.0KB |
| startup | bubbletea | 11.9MB | 11.0MB | 11.9MB | 904.0KB |
| startup | TUIX.Py | 18.9MB | 18.8MB | 18.9MB | 64.0KB |
| startup | rich | 31.8MB | 31.3MB | 31.8MB | 476.0KB |
| startup | opentui-core | 33.4MB | 33.0MB | 33.4MB | 412.0KB |
| startup | opentui-react | 35.9MB | 35.5MB | 35.9MB | 416.0KB |
| startup | prompt-toolkit | 36.0MB | 35.9MB | 36.0MB | 20.0KB |
| startup | urwid | 42.1MB | 42.1MB | 42.1MB | 36.0KB |
| startup | terminal-kit | 46.8MB | 47.0MB | 46.8MB | 0.00KB |
| startup | ink | 65.2MB | 62.6MB | 65.2MB | 2.6MB |
| startup | rezitui | 81.4MB | 68.0MB | 81.4MB | 13.4MB |
| startup | blessed | 152.2MB | 78.2MB | 152.2MB | 74.0MB |
| tree-construction (items=10) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=10) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=10) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 16.0KB |
| tree-construction (items=10) | bubbletea | 16.2MB | 11.9MB | 16.2MB | 4.3MB |
| tree-construction (items=10) | TUIX.Py | 19.0MB | 19.0MB | 19.0MB | 16.0KB |
| tree-construction (items=10) | rich | 31.8MB | 31.8MB | 31.8MB | 32.0KB |
| tree-construction (items=10) | opentui-core | 34.1MB | 33.4MB | 34.1MB | 724.0KB |
| tree-construction (items=10) | prompt-toolkit | 36.0MB | 36.0MB | 36.0MB | 16.0KB |
| tree-construction (items=10) | opentui-react | 36.1MB | 35.9MB | 36.1MB | 112.0KB |
| tree-construction (items=10) | urwid | 42.2MB | 42.1MB | 42.2MB | 76.0KB |
| tree-construction (items=10) | terminal-kit | 46.9MB | 46.9MB | 46.9MB | 4.0KB |
| tree-construction (items=10) | ink | 69.2MB | 65.8MB | 69.2MB | 3.4MB |
| tree-construction (items=10) | rezitui | 102.3MB | 83.1MB | 102.3MB | 19.2MB |
| tree-construction (items=10) | blessed | 160.3MB | 153.8MB | 160.3MB | 6.5MB |
| tree-construction (items=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=100) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=100) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 12.0KB |
| tree-construction (items=100) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| tree-construction (items=100) | TUIX.Py | 19.1MB | 19.0MB | 19.1MB | 32.0KB |
| tree-construction (items=100) | rich | 31.8MB | 31.8MB | 31.8MB | 16.0KB |
| tree-construction (items=100) | opentui-core | 34.8MB | 34.1MB | 34.8MB | 732.0KB |
| tree-construction (items=100) | prompt-toolkit | 36.0MB | 36.0MB | 36.0MB | 20.0KB |
| tree-construction (items=100) | opentui-react | 36.2MB | 36.1MB | 36.2MB | 104.0KB |
| tree-construction (items=100) | urwid | 42.2MB | 42.2MB | 42.2MB | 16.0KB |
| tree-construction (items=100) | terminal-kit | 46.8MB | 46.7MB | 46.8MB | 32.0KB |
| tree-construction (items=100) | ink | 107.3MB | 84.9MB | 107.3MB | 22.4MB |
| tree-construction (items=100) | rezitui | 110.1MB | 98.9MB | 110.1MB | 11.2MB |
| tree-construction (items=100) | blessed | 242.6MB | 163.7MB | 242.6MB | 78.9MB |
| tree-construction (items=500) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=500) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=500) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| tree-construction (items=500) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 16.0KB |
| tree-construction (items=500) | TUIX.Py | 19.1MB | 19.1MB | 19.1MB | 16.0KB |
| tree-construction (items=500) | rich | 31.8MB | 31.8MB | 31.8MB | 16.0KB |
| tree-construction (items=500) | opentui-core | 35.1MB | 34.8MB | 35.1MB | 348.0KB |
| tree-construction (items=500) | prompt-toolkit | 36.0MB | 36.0MB | 36.0MB | 16.0KB |
| tree-construction (items=500) | opentui-react | 36.5MB | 36.2MB | 36.5MB | 344.0KB |
| tree-construction (items=500) | urwid | 42.2MB | 42.2MB | 42.2MB | 28.0KB |
| tree-construction (items=500) | terminal-kit | 47.3MB | 46.8MB | 47.3MB | 564.0KB |
| tree-construction (items=500) | rezitui | 168.6MB | 113.3MB | 168.6MB | 55.3MB |
| tree-construction (items=500) | ink | 188.9MB | 107.8MB | 188.9MB | 81.1MB |
| tree-construction (items=500) | blessed | 451.5MB | 259.8MB | 451.5MB | 191.7MB |
| tree-construction (items=1000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=1000) | TUIX.Renderer | 4.0MB | 3.9MB | 4.0MB | 48.0KB |
| tree-construction (items=1000) | TUIX.Core | 4.2MB | 4.1MB | 4.2MB | 124.0KB |
| tree-construction (items=1000) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| tree-construction (items=1000) | TUIX.Py | 19.1MB | 19.1MB | 19.1MB | 16.0KB |
| tree-construction (items=1000) | rich | 31.9MB | 31.8MB | 31.9MB | 16.0KB |
| tree-construction (items=1000) | opentui-core | 35.1MB | 35.1MB | 35.1MB | 0.00KB |
| tree-construction (items=1000) | prompt-toolkit | 36.0MB | 36.0MB | 36.0MB | 16.0KB |
| tree-construction (items=1000) | opentui-react | 38.2MB | 36.6MB | 38.2MB | 1.6MB |
| tree-construction (items=1000) | urwid | 42.3MB | 42.2MB | 42.3MB | 24.0KB |
| tree-construction (items=1000) | terminal-kit | 55.5MB | 47.4MB | 55.5MB | 8.1MB |
| tree-construction (items=1000) | rezitui | 188.5MB | 168.7MB | 188.5MB | 19.8MB |
| tree-construction (items=1000) | ink | 364.8MB | 192.3MB | 364.8MB | 172.5MB |
| tree-construction (items=1000) | blessed | 527.6MB | 451.8MB | 527.6MB | 75.8MB |
| rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| rerender | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| rerender | TUIX.Core | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| rerender | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| rerender | TUIX.Py | 19.2MB | 19.1MB | 19.2MB | 32.0KB |
| rerender | rich | 31.9MB | 31.9MB | 31.9MB | 28.0KB |
| rerender | opentui-core | 35.1MB | 35.1MB | 35.1MB | 0.00KB |
| rerender | prompt-toolkit | 36.1MB | 36.0MB | 36.1MB | 104.0KB |
| rerender | opentui-react | 38.2MB | 38.2MB | 38.2MB | 0.00KB |
| rerender | urwid | 42.3MB | 42.3MB | 42.3MB | 84.0KB |
| rerender | terminal-kit | 55.4MB | 55.5MB | 55.4MB | 0.00KB |
| rerender | rezitui | 196.6MB | 188.5MB | 196.6MB | 8.1MB |
| rerender | ink | 364.7MB | 364.7MB | 364.7MB | 44.0KB |
| rerender | blessed | 526.7MB | 526.7MB | 526.7MB | 60.0KB |
| content-update | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| content-update | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| content-update | TUIX.Core | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| content-update | bubbletea | 16.2MB | 16.2MB | 16.2MB | 8.0KB |
| content-update | TUIX.Py | 19.3MB | 19.2MB | 19.3MB | 72.0KB |
| content-update | rich | 32.3MB | 31.9MB | 32.3MB | 332.0KB |
| content-update | opentui-core | 35.1MB | 35.1MB | 35.1MB | 0.00KB |
| content-update | prompt-toolkit | 36.1MB | 36.1MB | 36.1MB | 0.00KB |
| content-update | opentui-react | 39.1MB | 38.2MB | 39.1MB | 892.0KB |
| content-update | urwid | 42.3MB | 42.3MB | 42.3MB | 0.00KB |
| content-update | terminal-kit | 55.7MB | 55.7MB | 55.7MB | 0.00KB |
| content-update | rezitui | 190.5MB | 195.6MB | 190.5MB | 0.00KB |
| content-update | ink | 365.5MB | 364.9MB | 365.5MB | 564.0KB |
| content-update | blessed | 527.4MB | 529.0MB | 527.4MB | 0.00KB |
| layout-stress (cols=4,rows=10) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Core | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| layout-stress (cols=4,rows=10) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Py | 19.3MB | 19.3MB | 19.3MB | 0.00KB |
| layout-stress (cols=4,rows=10) | rich | 32.3MB | 32.3MB | 32.3MB | 48.0KB |
| layout-stress (cols=4,rows=10) | opentui-core | 35.1MB | 35.1MB | 35.1MB | 0.00KB |
| layout-stress (cols=4,rows=10) | prompt-toolkit | 36.2MB | 36.1MB | 36.2MB | 52.0KB |
| layout-stress (cols=4,rows=10) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| layout-stress (cols=4,rows=10) | urwid | 42.3MB | 42.3MB | 42.3MB | 0.00KB |
| layout-stress (cols=4,rows=10) | terminal-kit | 55.5MB | 55.5MB | 55.5MB | 0.00KB |
| layout-stress (cols=4,rows=10) | rezitui | 191.7MB | 189.7MB | 191.7MB | 2.0MB |
| layout-stress (cols=4,rows=10) | ink | 365.9MB | 365.2MB | 365.9MB | 712.0KB |
| layout-stress (cols=4,rows=10) | blessed | 527.7MB | 533.0MB | 527.7MB | 0.00KB |
| scroll-stress (items=2000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Core | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| scroll-stress (items=2000) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Py | 19.3MB | 19.3MB | 19.3MB | 0.00KB |
| scroll-stress (items=2000) | rich | 32.4MB | 32.4MB | 32.4MB | 84.0KB |
| scroll-stress (items=2000) | opentui-core | 35.3MB | 35.1MB | 35.3MB | 124.0KB |
| scroll-stress (items=2000) | prompt-toolkit | 36.2MB | 36.2MB | 36.2MB | 0.00KB |
| scroll-stress (items=2000) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| scroll-stress (items=2000) | urwid | 42.3MB | 42.3MB | 42.3MB | 0.00KB |
| scroll-stress (items=2000) | terminal-kit | 55.6MB | 55.6MB | 55.6MB | 0.00KB |
| scroll-stress (items=2000) | rezitui | 191.7MB | 191.7MB | 191.7MB | 16.0KB |
| scroll-stress (items=2000) | ink | 365.9MB | 365.9MB | 365.9MB | 4.0KB |
| scroll-stress (items=2000) | blessed | 527.3MB | 527.9MB | 527.3MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Core | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Py | 19.3MB | 19.3MB | 19.3MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | bubbletea | 20.2MB | 20.2MB | 20.2MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | rich | 32.0MB | 32.4MB | 32.0MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | opentui-core | 35.3MB | 35.3MB | 35.3MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | prompt-toolkit | 36.3MB | 36.2MB | 36.3MB | 88.0KB |
| virtual-list (items=100000,viewport=40) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | urwid | 42.4MB | 42.3MB | 42.4MB | 64.0KB |
| virtual-list (items=100000,viewport=40) | terminal-kit | 96.7MB | 91.3MB | 96.7MB | 5.4MB |
| virtual-list (items=100000,viewport=40) | rezitui | 193.6MB | 191.9MB | 193.6MB | 1.7MB |
| virtual-list (items=100000,viewport=40) | ink | 366.8MB | 368.2MB | 366.8MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | blessed | 529.4MB | 529.5MB | 529.4MB | 0.00KB |
| tables (cols=8,rows=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Core | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Py | 19.3MB | 19.3MB | 19.3MB | 0.00KB |
| tables (cols=8,rows=100) | bubbletea | 20.2MB | 20.2MB | 20.2MB | 0.00KB |
| tables (cols=8,rows=100) | rich | 32.1MB | 32.1MB | 32.1MB | 52.0KB |
| tables (cols=8,rows=100) | opentui-core | 35.3MB | 35.3MB | 35.3MB | 0.00KB |
| tables (cols=8,rows=100) | prompt-toolkit | 36.3MB | 36.3MB | 36.3MB | 8.0KB |
| tables (cols=8,rows=100) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| tables (cols=8,rows=100) | urwid | 42.4MB | 42.4MB | 42.4MB | 0.00KB |
| tables (cols=8,rows=100) | terminal-kit | 96.3MB | 96.5MB | 96.3MB | 0.00KB |
| tables (cols=8,rows=100) | rezitui | 192.2MB | 192.1MB | 192.2MB | 100.0KB |
| tables (cols=8,rows=100) | ink | 366.8MB | 366.8MB | 366.8MB | 0.00KB |
| tables (cols=8,rows=100) | blessed | 529.3MB | 529.3MB | 529.3MB | 0.00KB |
| memory-profile | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| memory-profile | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| memory-profile | TUIX.Core | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| memory-profile | TUIX.Py | 19.4MB | 19.3MB | 19.4MB | 64.0KB |
| memory-profile | bubbletea | 20.2MB | 20.2MB | 20.2MB | 8.0KB |
| memory-profile | rich | 32.5MB | 32.1MB | 32.5MB | 448.0KB |
| memory-profile | opentui-core | 35.3MB | 35.3MB | 35.3MB | 48.0KB |
| memory-profile | prompt-toolkit | 36.5MB | 36.3MB | 36.5MB | 268.0KB |
| memory-profile | opentui-react | 39.3MB | 39.1MB | 39.3MB | 276.0KB |
| memory-profile | urwid | 42.5MB | 42.4MB | 42.5MB | 64.0KB |
| memory-profile | terminal-kit | 96.3MB | 96.3MB | 96.3MB | 12.0KB |
| memory-profile | rezitui | 193.2MB | 192.4MB | 193.2MB | 752.0KB |
| memory-profile | ink | 366.8MB | 366.8MB | 366.8MB | 12.0KB |
| memory-profile | blessed | 528.8MB | 528.6MB | 528.8MB | 188.0KB |
| terminal-rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-rerender | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| terminal-rerender | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-rerender | TUIX.Py | 19.6MB | 19.5MB | 19.6MB | 64.0KB |
| terminal-rerender | bubbletea | 20.2MB | 20.2MB | 20.2MB | 0.00KB |
| terminal-rerender | rich | 32.6MB | 32.5MB | 32.6MB | 24.0KB |
| terminal-rerender | opentui-core | 36.1MB | 36.1MB | 36.1MB | 24.0KB |
| terminal-rerender | prompt-toolkit | 37.0MB | 36.8MB | 37.0MB | 208.0KB |
| terminal-rerender | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-rerender | urwid | 42.5MB | 42.5MB | 42.5MB | 0.00KB |
| terminal-rerender | terminal-kit | 96.4MB | 96.3MB | 96.4MB | 4.0KB |
| terminal-rerender | rezitui | 193.2MB | 193.2MB | 193.2MB | 8.0KB |
| terminal-rerender | ink | 366.8MB | 366.8MB | 366.8MB | 12.0KB |
| terminal-rerender | blessed | 528.9MB | 528.9MB | 528.9MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | TUIX.Py | 19.6MB | 19.6MB | 19.6MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | bubbletea | 20.5MB | 20.2MB | 20.5MB | 256.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | rich | 32.7MB | 32.6MB | 32.7MB | 88.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | prompt-toolkit | 37.1MB | 37.1MB | 37.1MB | 16.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | urwid | 42.5MB | 42.5MB | 42.5MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | terminal-kit | 96.4MB | 96.4MB | 96.4MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | rezitui | 191.8MB | 193.2MB | 191.8MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ink | 366.9MB | 366.9MB | 366.9MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | blessed | 529.0MB | 528.9MB | 529.0MB | 140.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | TUIX.Py | 19.5MB | 19.4MB | 19.5MB | 12.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | rich | 32.9MB | 32.8MB | 32.9MB | 80.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | prompt-toolkit | 37.6MB | 37.1MB | 37.6MB | 460.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | urwid | 42.6MB | 42.5MB | 42.6MB | 84.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | terminal-kit | 96.5MB | 96.5MB | 96.5MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | rezitui | 210.0MB | 191.8MB | 210.0MB | 18.2MB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ink | 366.9MB | 366.9MB | 366.9MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | blessed | 529.3MB | 529.0MB | 529.3MB | 352.0KB |
| terminal-screen-transition (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | TUIX.Py | 19.9MB | 19.9MB | 19.9MB | 32.0KB |
| terminal-screen-transition (cols=120,rows=40) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | rich | 32.8MB | 33.0MB | 32.8MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | prompt-toolkit | 37.7MB | 37.6MB | 37.7MB | 144.0KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | urwid | 42.6MB | 42.6MB | 42.6MB | 4.0KB |
| terminal-screen-transition (cols=120,rows=40) | terminal-kit | 96.5MB | 96.5MB | 96.5MB | 4.0KB |
| terminal-screen-transition (cols=120,rows=40) | rezitui | 210.2MB | 210.7MB | 210.2MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | ink | 366.3MB | 366.2MB | 366.3MB | 160.0KB |
| terminal-screen-transition (cols=120,rows=40) | blessed | 529.4MB | 529.4MB | 529.4MB | 4.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | TUIX.Py | 19.7MB | 19.7MB | 19.7MB | 84.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | rich | 32.9MB | 32.9MB | 32.9MB | 16.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | prompt-toolkit | 37.8MB | 37.7MB | 37.8MB | 88.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | urwid | 42.6MB | 42.6MB | 42.6MB | 32.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | terminal-kit | 96.6MB | 96.5MB | 96.6MB | 8.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | rezitui | 212.9MB | 209.6MB | 212.9MB | 3.3MB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ink | 366.7MB | 366.7MB | 366.7MB | 8.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | blessed | 529.4MB | 529.4MB | 529.4MB | 4.0KB |
| terminal-input-latency (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | TUIX.Py | 19.8MB | 19.8MB | 19.8MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | rich | 33.4MB | 32.9MB | 33.4MB | 500.0KB |
| terminal-input-latency (cols=120,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | prompt-toolkit | 37.8MB | 37.8MB | 37.8MB | 4.0KB |
| terminal-input-latency (cols=120,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | urwid | 42.7MB | 42.6MB | 42.7MB | 40.0KB |
| terminal-input-latency (cols=120,rows=40) | terminal-kit | 97.0MB | 96.6MB | 97.0MB | 388.0KB |
| terminal-input-latency (cols=120,rows=40) | rezitui | 210.3MB | 212.9MB | 210.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | ink | 366.9MB | 366.9MB | 366.9MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | blessed | 529.5MB | 529.4MB | 529.5MB | 36.0KB |
| terminal-memory-soak (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Py | 19.8MB | 19.7MB | 19.8MB | 68.0KB |
| terminal-memory-soak (cols=120,rows=40) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | rich | 33.5MB | 33.6MB | 33.5MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | prompt-toolkit | 37.9MB | 37.8MB | 37.9MB | 48.0KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | urwid | 42.7MB | 42.7MB | 42.7MB | 44.0KB |
| terminal-memory-soak (cols=120,rows=40) | terminal-kit | 97.0MB | 97.0MB | 97.0MB | 4.0KB |
| terminal-memory-soak (cols=120,rows=40) | rezitui | 210.6MB | 210.3MB | 210.6MB | 352.0KB |
| terminal-memory-soak (cols=120,rows=40) | ink | 366.9MB | 366.9MB | 366.9MB | 8.0KB |
| terminal-memory-soak (cols=120,rows=40) | blessed | 529.5MB | 529.5MB | 529.5MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Py | 19.8MB | 19.8MB | 19.8MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | rich | 33.7MB | 33.5MB | 33.7MB | 200.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | prompt-toolkit | 37.9MB | 37.9MB | 37.9MB | 8.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | urwid | 42.8MB | 42.7MB | 42.8MB | 84.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | terminal-kit | 97.2MB | 97.2MB | 97.2MB | 12.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | rezitui | 214.6MB | 212.0MB | 214.6MB | 2.6MB |
| terminal-full-ui (cols=120,rows=40,services=24) | ink | 367.2MB | 367.1MB | 367.2MB | 184.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | blessed | 529.2MB | 529.2MB | 529.2MB | 24.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Py | 19.9MB | 19.8MB | 19.9MB | 64.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rich | 33.8MB | 33.7MB | 33.8MB | 88.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | prompt-toolkit | 38.0MB | 38.0MB | 38.0MB | 36.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | urwid | 42.9MB | 42.8MB | 42.9MB | 64.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 97.3MB | 97.3MB | 97.3MB | 72.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 213.9MB | 213.9MB | 213.9MB | 88.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 367.3MB | 367.3MB | 367.3MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 529.2MB | 529.2MB | 529.2MB | 12.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Renderer | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Py | 20.0MB | 19.9MB | 20.0MB | 64.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | rich | 33.8MB | 33.8MB | 33.8MB | 52.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | prompt-toolkit | 38.1MB | 38.0MB | 38.1MB | 64.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | urwid | 42.9MB | 43.0MB | 42.9MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | terminal-kit | 97.4MB | 97.3MB | 97.4MB | 32.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | rezitui | 214.0MB | 213.9MB | 214.0MB | 12.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ink | 367.3MB | 367.3MB | 367.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | blessed | 529.2MB | 529.2MB | 529.2MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Renderer | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.8MB | 4.8MB | 4.8MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Py | 19.9MB | 19.9MB | 19.9MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rich | 33.8MB | 33.8MB | 33.8MB | 36.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | prompt-toolkit | 38.1MB | 38.1MB | 38.1MB | 4.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | urwid | 42.9MB | 42.9MB | 42.9MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 97.4MB | 97.4MB | 97.4MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 214.0MB | 214.0MB | 214.0MB | 4.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 367.3MB | 367.3MB | 367.3MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 529.2MB | 529.2MB | 529.2MB | 4.0KB |
| terminal-virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Renderer | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Core | 4.8MB | 4.8MB | 4.8MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Py | 20.1MB | 20.0MB | 20.1MB | 64.0KB |
| terminal-virtual-list (items=100000,viewport=40) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | rich | 33.9MB | 33.9MB | 33.9MB | 36.0KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | prompt-toolkit | 38.1MB | 38.1MB | 38.1MB | 64.0KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | urwid | 42.9MB | 42.9MB | 42.9MB | 32.0KB |
| terminal-virtual-list (items=100000,viewport=40) | terminal-kit | 129.3MB | 128.9MB | 129.3MB | 388.0KB |
| terminal-virtual-list (items=100000,viewport=40) | rezitui | 214.0MB | 214.0MB | 214.0MB | 4.0KB |
| terminal-virtual-list (items=100000,viewport=40) | ink | 368.2MB | 368.2MB | 368.2MB | 20.0KB |
| terminal-virtual-list (items=100000,viewport=40) | blessed | 530.1MB | 531.5MB | 530.1MB | 0.00KB |
| terminal-table (cols=8,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Renderer | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Core | 4.8MB | 4.8MB | 4.8MB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Py | 20.1MB | 20.1MB | 20.1MB | 0.00KB |
| terminal-table (cols=8,rows=40) | bubbletea | 20.5MB | 20.5MB | 20.5MB | 0.00KB |
| terminal-table (cols=8,rows=40) | rich | 33.9MB | 33.9MB | 33.9MB | 0.00KB |
| terminal-table (cols=8,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-table (cols=8,rows=40) | prompt-toolkit | 38.1MB | 38.1MB | 38.1MB | 0.00KB |
| terminal-table (cols=8,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-table (cols=8,rows=40) | urwid | 42.9MB | 42.9MB | 42.9MB | 0.00KB |
| terminal-table (cols=8,rows=40) | terminal-kit | 129.2MB | 129.2MB | 129.2MB | 0.00KB |
| terminal-table (cols=8,rows=40) | rezitui | 215.6MB | 214.0MB | 215.6MB | 1.6MB |
| terminal-table (cols=8,rows=40) | ink | 367.8MB | 367.8MB | 367.8MB | 0.00KB |
| terminal-table (cols=8,rows=40) | blessed | 530.2MB | 530.1MB | 530.2MB | 68.0KB |

