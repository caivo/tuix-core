# Benchmark Results

> 2026-04-09T07:26:08.098Z | Node v22.19.0 | Windows_NT 10.0.26200 | win32 x64 | 12th Gen Intel(R) Core(TM) i5-12450HX (12 cores) | RAM 24352MB
> Libraries tested: blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React, TUIX.Renderer, TUIX.Core, TUIX.Python, Rich, Urwid, PromptToolkit
> Byte columns: "Bytes(local)" = bytes produced per frame (framework counter); "Bytes(pty)" = observed PTY bytes (cross-framework comparable).

## startup

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 262µs | 9.6% | 257µs–267µs | 3.8K ops/s | 28.69ms | 27.0KB | 0.00KB | 1.6MB | 16.0KB |
| TUIX.Renderer | 1 | 277µs | 38.9% | 256µs–299µs | 3.6K ops/s | 27.74ms | 3.9MB | 4.0KB | 6.3MB | 64.4KB |
| bubbletea | 1 | 308µs | 69.8% | 266µs–350µs | 3.2K ops/s | 30.85ms | 16.0MB | 5.4MB | 1.6MB | 16.0KB |
| TUIX.Core | 1 | 311µs | 17.0% | 301µs–322µs | 3.2K ops/s | 31.13ms | 4.0MB | 4.0KB | 6.0MB | 61.7KB |
| rezitui | 1 | 346µs | 170.4% | 230µs–461µs | 2.9K ops/s | 34.98ms | 81.8MB | 14.0MB | 133.7KB | 1.3KB |
| tuix-python | 1 | 371µs | 10.7% | 363µs–379µs | 2.7K ops/s | 37.19ms | 18.4MB | 60.0KB | 6.7MB | 68.3KB |
| opentui-react | 1 | 449µs | 53.9% | 402µs–496µs | 2.2K ops/s | 44.96ms | 35.9MB | 424.0KB | 7.4MB | 76.0KB |
| opentui-core | 1 | 577µs | 46.3% | 525µs–630µs | 1.7K ops/s | 58.01ms | 33.3MB | 416.0KB | 6.7MB | 68.5KB |
| terminal-kit | 1 | 658µs | 15.2% | 638µs–677µs | 1.5K ops/s | 66.02ms | 46.4MB | 0.00KB | 190.0KB | 1.9KB |
| prompt-toolkit | 1 | 1.86ms | 11.0% | 1.82ms–1.90ms | 539 ops/s | 185.69ms | 35.7MB | 16.0KB | 189.7KB | 1.9KB |
| rich | 1 | 2.09ms | 20.0% | 2.01ms–2.17ms | 478 ops/s | 209.34ms | 31.5MB | 920.0KB | 189.7KB | 1.9KB |
| urwid | 1 | 2.26ms | 33.5% | 2.11ms–2.41ms | 442 ops/s | 226.24ms | 42.5MB | 24.0KB | 187.5KB | 1.9KB |
| ink | 1 | 3.26ms | 25.4% | 3.10ms–3.43ms | 306 ops/s | 326.49ms | 64.9MB | 2.7MB | 189.7KB | 1.9KB |
| blessed | 1 | 34.93ms | 18.5% | 33.66ms–36.20ms | 29 ops/s | 3.49s | 152.6MB | 74.9MB | 229.2KB | 2.3KB |

## tree-construction (items=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| opentui-react | 1 | 68µs | 30.3% | 67µs–70µs | 14.6K ops/s | 35.16ms | 36.0MB | 84.0KB | 3.6MB | 7.5KB |
| TUIX.Core | 1 | 68µs | 9.8% | 68µs–69µs | 14.6K ops/s | 34.24ms | 4.0MB | 0.00KB | 555.7KB | 1.1KB |
| TUIX.Renderer | 1 | 85µs | 39.8% | 82µs–88µs | 11.7K ops/s | 42.73ms | 3.9MB | 0.00KB | 1.1MB | 2.3KB |
| tuix-python | 1 | 98µs | 18.2% | 96µs–99µs | 10.2K ops/s | 49.12ms | 18.5MB | 16.0KB | 7.7MB | 15.7KB |
| blessed | 1 | 106µs | 230.4% | 84µs–127µs | 9.5K ops/s | 53.11ms | 161.0MB | 6.2MB | 126.0KB | 258B |
| ratatui | 1 | 107µs | 22.3% | 105µs–109µs | 9.4K ops/s | 53.52ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| rezitui | 1 | 170µs | 254.9% | 132µs–208µs | 5.9K ops/s | 85.41ms | 98.4MB | 15.1MB | 668.5KB | 1.3KB |
| bubbletea | 1 | 345µs | 44.3% | 332µs–358µs | 2.9K ops/s | 172.72ms | 16.0MB | 48.0KB | 7.8MB | 16.0KB |
| terminal-kit | 1 | 368µs | 16.0% | 363µs–373µs | 2.7K ops/s | 184.20ms | 46.6MB | 8.0KB | 950.2KB | 1.9KB |
| opentui-core | 1 | 435µs | 59.0% | 412µs–457µs | 2.3K ops/s | 223.21ms | 33.9MB | 640.0KB | 33.4MB | 68.3KB |
| ink | 1 | 807µs | 34.4% | 783µs–832µs | 1.2K ops/s | 404.23ms | 68.9MB | 3.1MB | 23.9KB | 49B |
| rich | 1 | 1.08ms | 24.0% | 1.05ms–1.10ms | 929 ops/s | 538.71ms | 31.6MB | 28.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 1.24ms | 22.6% | 1.22ms–1.27ms | 806 ops/s | 620.72ms | 35.8MB | 16.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.59ms | 22.1% | 1.56ms–1.62ms | 628 ops/s | 796.08ms | 42.5MB | 76.0KB | 937.5KB | 1.9KB |

## tree-construction (items=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 91µs | 21.3% | 89µs–93µs | 11.0K ops/s | 45.55ms | 4.0MB | 0.00KB | 1.2MB | 2.5KB |
| TUIX.Renderer | 1 | 138µs | 44.7% | 133µs–144µs | 7.2K ops/s | 69.14ms | 3.9MB | 0.00KB | 1.3MB | 2.7KB |
| opentui-react | 1 | 144µs | 27.6% | 141µs–148µs | 6.9K ops/s | 79.09ms | 36.1MB | 104.0KB | 8.9MB | 18.3KB |
| rezitui | 1 | 179µs | 141.9% | 157µs–201µs | 5.6K ops/s | 90.15ms | 110.4MB | 12.6MB | 668.5KB | 1.3KB |
| opentui-core | 1 | 199µs | 188.4% | 166µs–232µs | 5.0K ops/s | 140.97ms | 34.6MB | 668.0KB | 33.8MB | 69.2KB |
| ratatui | 1 | 208µs | 18.6% | 204µs–211µs | 4.8K ops/s | 103.87ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| tuix-python | 1 | 220µs | 18.0% | 217µs–224µs | 4.5K ops/s | 110.60ms | 18.6MB | 16.0KB | 17.2MB | 35.2KB |
| bubbletea | 1 | 424µs | 33.1% | 412µs–437µs | 2.4K ops/s | 212.35ms | 16.0MB | 16.0KB | 7.8MB | 16.0KB |
| blessed | 1 | 547µs | 114.4% | 492µs–602µs | 1.8K ops/s | 274.04ms | 240.4MB | 75.2MB | 490.2KB | 1004B |
| rich | 1 | 1.01ms | 23.8% | 984µs–1.03ms | 995 ops/s | 502.95ms | 31.6MB | 16.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 1.29ms | 15.5% | 1.28ms–1.31ms | 773 ops/s | 646.97ms | 35.8MB | 16.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.55ms | 19.7% | 1.52ms–1.58ms | 646 ops/s | 774.48ms | 42.6MB | 20.0KB | 937.5KB | 1.9KB |
| terminal-kit | 1 | 1.90ms | 21.7% | 1.86ms–1.94ms | 527 ops/s | 950.22ms | 46.7MB | 28.0KB | 950.2KB | 1.9KB |
| ink | 1 | 5.72ms | 19.6% | 5.62ms–5.82ms | 175 ops/s | 2.86s | 105.8MB | 21.2MB | 287.6KB | 589B |

## tree-construction (items=500)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 83µs | 25.1% | 81µs–84µs | 12.1K ops/s | 41.34ms | 4.0MB | 48.0KB | 1.2MB | 2.5KB |
| TUIX.Renderer | 1 | 86µs | 66.8% | 81µs–91µs | 11.6K ops/s | 42.95ms | 3.9MB | 48.0KB | 1.3MB | 2.7KB |
| opentui-core | 1 | 187µs | 59.6% | 177µs–196µs | 5.4K ops/s | 306.62ms | 34.9MB | 196.0KB | 33.7MB | 68.9KB |
| tuix-python | 1 | 210µs | 18.6% | 207µs–214µs | 4.8K ops/s | 105.56ms | 18.6MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-react | 1 | 304µs | 171.7% | 259µs–350µs | 3.3K ops/s | 197.48ms | 37.6MB | 1.5MB | 8.9MB | 18.2KB |
| rezitui | 1 | 467µs | 332.5% | 331µs–604µs | 2.1K ops/s | 234.00ms | 161.8MB | 48.7MB | 668.5KB | 1.3KB |
| ratatui | 1 | 658µs | 14.6% | 650µs–666µs | 1.5K ops/s | 329.05ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| bubbletea | 1 | 864µs | 28.8% | 843µs–886µs | 1.2K ops/s | 432.48ms | 16.0MB | 8.0KB | 7.8MB | 16.0KB |
| rich | 1 | 1.04ms | 24.7% | 1.01ms–1.06ms | 965 ops/s | 518.32ms | 31.6MB | 16.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 1.34ms | 16.0% | 1.32ms–1.36ms | 744 ops/s | 672.49ms | 35.8MB | 16.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.49ms | 20.7% | 1.46ms–1.51ms | 673 ops/s | 743.70ms | 42.6MB | 32.0KB | 937.5KB | 1.9KB |
| blessed | 1 | 2.80ms | 54.1% | 2.67ms–2.93ms | 357 ops/s | 1.40s | 454.1MB | 192.3MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 8.52ms | 19.3% | 8.38ms–8.67ms | 117 ops/s | 4.26s | 46.6MB | 0.00KB | 950.2KB | 1.9KB |
| ink | 1 | 27.50ms | 16.5% | 27.10ms–27.90ms | 36 ops/s | 13.75s | 182.9MB | 76.4MB | 1.6MB | 3.3KB |

## tree-construction (items=1000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 83µs | 65.9% | 78µs–87µs | 12.1K ops/s | 41.37ms | 3.9MB | 48.0KB | 1.3MB | 2.7KB |
| TUIX.Core | 1 | 126µs | 29.0% | 123µs–130µs | 7.9K ops/s | 63.27ms | 4.0MB | 48.0KB | 1.2MB | 2.5KB |
| opentui-core | 1 | 134µs | 69.7% | 126µs–142µs | 7.5K ops/s | 480.40ms | 34.9MB | 0.00KB | 33.8MB | 69.3KB |
| tuix-python | 1 | 219µs | 26.4% | 214µs–224µs | 4.6K ops/s | 109.79ms | 18.6MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-react | 1 | 313µs | 18.8% | 308µs–318µs | 3.2K ops/s | 204.77ms | 38.1MB | 540.0KB | 8.9MB | 18.2KB |
| rezitui | 1 | 636µs | 162.2% | 545µs–726µs | 1.6K ops/s | 318.01ms | 187.0MB | 25.4MB | 668.5KB | 1.3KB |
| prompt-toolkit | 1 | 1.22ms | 19.8% | 1.20ms–1.25ms | 817 ops/s | 612.54ms | 35.8MB | 24.0KB | 948.7KB | 1.9KB |
| rich | 1 | 1.24ms | 21.3% | 1.22ms–1.27ms | 805 ops/s | 621.87ms | 31.6MB | 16.0KB | 948.7KB | 1.9KB |
| bubbletea | 1 | 1.26ms | 33.6% | 1.23ms–1.30ms | 791 ops/s | 632.42ms | 16.0MB | 0.00KB | 7.8MB | 16.0KB |
| ratatui | 1 | 1.30ms | 17.7% | 1.28ms–1.32ms | 770 ops/s | 649.18ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| urwid | 1 | 1.68ms | 20.2% | 1.65ms–1.71ms | 594 ops/s | 841.66ms | 42.6MB | 20.0KB | 937.5KB | 1.9KB |
| blessed | 1 | 5.39ms | 40.5% | 5.20ms–5.59ms | 185 ops/s | 2.70s | 526.6MB | 72.3MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 16.74ms | 21.7% | 16.42ms–17.06ms | 60 ops/s | 8.37s | 54.9MB | 8.2MB | 950.2KB | 1.9KB |
| ink | 1 | 54.72ms | 14.1% | 54.04ms–55.39ms | 18 ops/s | 27.36s | 364.8MB | 180.7MB | 3.3MB | 6.7KB |

## rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 18µs | 20.7% | 17µs–18µs | 57.0K ops/s | 17.59ms | 3.9MB | 0.00KB | 15.6KB | 16B |
| TUIX.Core | 1 | 19µs | 50.6% | 18µs–19µs | 54.0K ops/s | 18.55ms | 4.1MB | 0.00KB | 15.6KB | 16B |
| tuix-python | 1 | 24µs | 78.1% | 23µs–26µs | 41.0K ops/s | 25.37ms | 18.6MB | 32.0KB | 1.8MB | 1.9KB |
| blessed | 1 | 35µs | 300.4% | 29µs–42µs | 28.2K ops/s | 35.56ms | 526.0MB | 220.0KB | 0.00KB | 0.00KB |
| opentui-react | 1 | 48µs | 24.9% | 48µs–49µs | 20.6K ops/s | 48.66ms | 38.1MB | 0.00KB | 3.9KB | 4B |
| ratatui | 1 | 67µs | 43.4% | 66µs–69µs | 14.9K ops/s | 67.43ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| rezitui | 1 | 73µs | 283.9% | 60µs–86µs | 13.7K ops/s | 73.24ms | 193.7MB | 6.7MB | 1.3MB | 1.3KB |
| bubbletea | 1 | 162µs | 59.9% | 156µs–168µs | 6.2K ops/s | 162.05ms | 16.0MB | 0.00KB | 15.6MB | 16.0KB |
| opentui-core | 1 | 184µs | 62.2% | 177µs–192µs | 5.4K ops/s | 184.81ms | 34.9MB | 0.00KB | 66.9MB | 68.5KB |
| terminal-kit | 1 | 262µs | 32.9% | 256µs–267µs | 3.8K ops/s | 261.75ms | 54.8MB | 0.00KB | 1.9MB | 1.9KB |
| ink | 1 | 812µs | 26.5% | 798µs–825µs | 1.2K ops/s | 812.07ms | 364.8MB | 0.00KB | 1.9MB | 1.9KB |
| prompt-toolkit | 1 | 1.30ms | 18.9% | 1.29ms–1.32ms | 768 ops/s | 1.30s | 36.1MB | 264.0KB | 1.9MB | 1.9KB |
| rich | 1 | 1.59ms | 20.4% | 1.57ms–1.61ms | 629 ops/s | 1.59s | 31.7MB | 32.0KB | 1.9MB | 1.9KB |
| urwid | 1 | 1.89ms | 20.2% | 1.87ms–1.92ms | 528 ops/s | 1.89s | 42.7MB | 72.0KB | 1.8MB | 1.9KB |

## content-update

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 28µs | 12.4% | 28µs–28µs | 35.7K ops/s | 14.05ms | 4.1MB | 0.00KB | 6.5MB | 13.4KB |
| rezitui | 1 | 88µs | 274.6% | 66µs–109µs | 11.4K ops/s | 43.89ms | 189.0MB | 0.00KB | 668.5KB | 1.3KB |
| ratatui | 1 | 122µs | 23.7% | 119µs–124µs | 8.2K ops/s | 60.91ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| tuix-python | 1 | 145µs | 15.8% | 143µs–147µs | 6.9K ops/s | 83.58ms | 18.7MB | 16.0KB | 17.1MB | 35.0KB |
| TUIX.Renderer | 1 | 150µs | 25.5% | 147µs–153µs | 6.7K ops/s | 86.22ms | 4.2MB | 296.0KB | 5.8MB | 11.9KB |
| opentui-core | 1 | 168µs | 41.3% | 162µs–174µs | 5.9K ops/s | 88.73ms | 34.9MB | 0.00KB | 33.4MB | 68.5KB |
| opentui-react | 1 | 187µs | 751.3% | 64µs–311µs | 5.3K ops/s | 98.41ms | 38.9MB | 824.0KB | 15.3MB | 31.3KB |
| blessed | 1 | 347µs | 54.5% | 331µs–364µs | 2.9K ops/s | 173.80ms | 526.5MB | 0.00KB | 1.1MB | 2.3KB |
| bubbletea | 1 | 384µs | 39.1% | 371µs–397µs | 2.6K ops/s | 192.14ms | 16.0MB | 0.00KB | 7.8MB | 16.0KB |
| terminal-kit | 1 | 402µs | 22.2% | 394µs–410µs | 2.5K ops/s | 201.20ms | 54.9MB | 80.0KB | 950.2KB | 1.9KB |
| prompt-toolkit | 1 | 1.25ms | 20.4% | 1.23ms–1.27ms | 802 ops/s | 623.92ms | 36.4MB | 364.0KB | 948.7KB | 1.9KB |
| rich | 1 | 1.70ms | 19.9% | 1.67ms–1.73ms | 587 ops/s | 851.52ms | 32.2MB | 412.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.88ms | 18.9% | 1.85ms–1.92ms | 531 ops/s | 942.40ms | 42.7MB | 0.00KB | 937.5KB | 1.9KB |
| ink | 1 | 3.80ms | 23.7% | 3.72ms–3.88ms | 263 ops/s | 1.90s | 365.3MB | 476.0KB | 953.1KB | 1.9KB |

## layout-stress (cols=4,rows=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| opentui-react | 1 | 61µs | 23.9% | 59µs–62µs | 16.4K ops/s | 20.42ms | 38.9MB | 0.00KB | 1.2KB | 4B |
| tuix-python | 1 | 68µs | 20.7% | 66µs–70µs | 14.7K ops/s | 23.13ms | 18.7MB | 8.0KB | 1.1MB | 3.6KB |
| TUIX.Renderer | 1 | 100µs | 34.9% | 96µs–104µs | 10.0K ops/s | 33.71ms | 4.2MB | 0.00KB | 1.1MB | 3.7KB |
| opentui-core | 1 | 109µs | 38.2% | 104µs–114µs | 9.2K ops/s | 42.96ms | 34.9MB | 0.00KB | 19.8MB | 67.7KB |
| ratatui | 1 | 116µs | 23.7% | 113µs–119µs | 8.6K ops/s | 34.87ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| TUIX.Core | 1 | 125µs | 16.3% | 122µs–127µs | 8.0K ops/s | 37.41ms | 4.1MB | 44.0KB | 1.1MB | 3.6KB |
| bubbletea | 1 | 247µs | 37.9% | 237µs–258µs | 4.0K ops/s | 74.31ms | 16.0MB | 0.00KB | 4.7MB | 16.0KB |
| blessed | 1 | 288µs | 140.8% | 243µs–334µs | 3.5K ops/s | 86.60ms | 526.9MB | 0.00KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 765µs | 19.0% | 749µs–782µs | 1.3K ops/s | 229.73ms | 55.1MB | 0.00KB | 570.1KB | 1.9KB |
| rich | 1 | 962µs | 18.1% | 943µs–982µs | 1.0K ops/s | 288.86ms | 32.2MB | 8.0KB | 569.2KB | 1.9KB |
| prompt-toolkit | 1 | 1.05ms | 19.2% | 1.03ms–1.08ms | 948 ops/s | 316.59ms | 36.4MB | 0.00KB | 569.2KB | 1.9KB |
| rezitui | 1 | 1.69ms | 52.2% | 1.59ms–1.79ms | 593 ops/s | 506.10ms | 189.6MB | 660.0KB | 1.6MB | 5.6KB |
| urwid | 1 | 1.76ms | 17.3% | 1.73ms–1.80ms | 567 ops/s | 529.67ms | 42.7MB | 0.00KB | 562.5KB | 1.9KB |
| ink | 1 | 6.90ms | 19.8% | 6.74ms–7.05ms | 145 ops/s | 2.07s | 365.3MB | 280.0KB | 1.2MB | 4.2KB |

## scroll-stress (items=2000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 27µs | 10.3% | 26µs–27µs | 37.5K ops/s | 1.33ms | 4.1MB | 0.00KB | 800B | 16B |
| opentui-react | 1 | 58µs | 43.1% | 51µs–65µs | 17.3K ops/s | 2.99ms | 38.9MB | 0.00KB | 135.6KB | 2.7KB |
| rezitui | 1 | 78µs | 17.2% | 74µs–82µs | 12.9K ops/s | 3.90ms | 189.7MB | 108.0KB | 66.8KB | 1.3KB |
| ratatui | 1 | 104µs | 10.2% | 101µs–107µs | 9.6K ops/s | 5.22ms | 27.0KB | 0.00KB | 798.0KB | 16.0KB |
| opentui-core | 1 | 111µs | 52.2% | 95µs–127µs | 9.0K ops/s | 6.24ms | 34.9MB | 0.00KB | 3.1MB | 62.8KB |
| bubbletea | 1 | 130µs | 68.0% | 105µs–154µs | 7.7K ops/s | 6.51ms | 16.0MB | 0.00KB | 798.0KB | 16.0KB |
| TUIX.Renderer | 1 | 132µs | 30.5% | 121µs–143µs | 7.6K ops/s | 6.77ms | 4.2MB | 0.00KB | 1.5MB | 30.8KB |
| tuix-python | 1 | 169µs | 20.6% | 159µs–178µs | 5.9K ops/s | 8.67ms | 18.7MB | 8.0KB | 1.6MB | 32.4KB |
| prompt-toolkit | 1 | 871µs | 11.5% | 843µs–899µs | 1.1K ops/s | 43.58ms | 36.4MB | 0.00KB | 94.9KB | 1.9KB |
| rich | 1 | 1.01ms | 15.3% | 964µs–1.05ms | 994 ops/s | 50.36ms | 32.3MB | 80.0KB | 94.9KB | 1.9KB |
| terminal-kit | 1 | 1.05ms | 19.5% | 989µs–1.10ms | 956 ops/s | 52.31ms | 55.2MB | 0.00KB | 95.0KB | 1.9KB |
| urwid | 1 | 1.07ms | 20.5% | 1.01ms–1.13ms | 931 ops/s | 53.72ms | 42.7MB | 0.00KB | 93.8KB | 1.9KB |
| ink | 1 | 2.13ms | 53.0% | 1.82ms–2.44ms | 470 ops/s | 106.47ms | 365.3MB | 4.0KB | 93.2KB | 1.9KB |
| blessed | 1 | 5.05ms | 18.6% | 4.79ms–5.31ms | 198 ops/s | 252.45ms | 527.2MB | 428.0KB | 115.5KB | 2.3KB |

## virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 31µs | 40.6% | 30µs–32µs | 32.2K ops/s | 31.16ms | 4.1MB | 0.00KB | 2.2MB | 2.2KB |
| opentui-react | 1 | 49µs | 24.0% | 48µs–50µs | 20.4K ops/s | 50.94ms | 38.9MB | 0.00KB | 1.8MB | 1.8KB |
| tuix-python | 1 | 82µs | 30.4% | 80µs–84µs | 12.2K ops/s | 89.01ms | 18.8MB | 92.0KB | 2.2MB | 2.3KB |
| TUIX.Renderer | 1 | 96µs | 34.0% | 94µs–98µs | 10.4K ops/s | 105.02ms | 4.2MB | 0.00KB | 2.3MB | 2.3KB |
| opentui-core | 1 | 126µs | 52.9% | 122µs–130µs | 8.0K ops/s | 152.80ms | 34.9MB | 0.00KB | 66.1MB | 67.7KB |
| ratatui | 1 | 128µs | 25.1% | 126µs–130µs | 7.8K ops/s | 127.95ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| rezitui | 1 | 134µs | 188.8% | 118µs–149µs | 7.5K ops/s | 133.85ms | 192.8MB | 2.6MB | 2.4MB | 2.5KB |
| bubbletea | 1 | 151µs | 78.2% | 144µs–159µs | 6.6K ops/s | 151.81ms | 20.0MB | 4.0MB | 15.6MB | 16.0KB |
| blessed | 1 | 168µs | 285.5% | 138µs–197µs | 6.0K ops/s | 167.94ms | 529.1MB | 0.00KB | 616.2KB | 631B |
| rich | 1 | 666µs | 23.1% | 657µs–676µs | 1.5K ops/s | 667.00ms | 32.3MB | 0.00KB | 1.9MB | 1.9KB |
| terminal-kit | 1 | 676µs | 23.4% | 666µs–685µs | 1.5K ops/s | 675.84ms | 97.5MB | 6.9MB | 1.9MB | 1.9KB |
| prompt-toolkit | 1 | 698µs | 21.3% | 689µs–707µs | 1.4K ops/s | 698.13ms | 36.7MB | 232.0KB | 1.9MB | 1.9KB |
| ink | 1 | 820µs | 35.8% | 802µs–838µs | 1.2K ops/s | 820.27ms | 365.7MB | 0.00KB | 153.3KB | 157B |
| urwid | 1 | 1.14ms | 22.4% | 1.12ms–1.15ms | 878 ops/s | 1.14s | 42.7MB | 64.0KB | 1.8MB | 1.9KB |

## tables (cols=8,rows=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 33µs | 57.3% | 31µs–35µs | 30.3K ops/s | 9.92ms | 4.1MB | 0.00KB | 1.5MB | 5.2KB |
| tuix-python | 1 | 92µs | 32.1% | 89µs–95µs | 10.9K ops/s | 32.96ms | 18.8MB | 0.00KB | 4.2MB | 14.3KB |
| TUIX.Renderer | 1 | 93µs | 29.6% | 90µs–96µs | 10.7K ops/s | 31.31ms | 4.2MB | 0.00KB | 1.6MB | 5.4KB |
| opentui-core | 1 | 144µs | 54.2% | 135µs–153µs | 7.0K ops/s | 54.30ms | 34.9MB | 0.00KB | 19.8MB | 67.7KB |
| ratatui | 1 | 149µs | 9.3% | 147µs–150µs | 6.7K ops/s | 44.61ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| bubbletea | 1 | 193µs | 47.8% | 183µs–204µs | 5.2K ops/s | 58.03ms | 20.0MB | 0.00KB | 4.7MB | 16.0KB |
| opentui-react | 1 | 233µs | 43.4% | 221µs–244µs | 4.3K ops/s | 76.83ms | 38.9MB | 0.00KB | 20.6MB | 70.4KB |
| blessed | 1 | 504µs | 39.3% | 481µs–526µs | 2.0K ops/s | 151.15ms | 528.5MB | 24.0KB | 821.5KB | 2.7KB |
| terminal-kit | 1 | 663µs | 23.6% | 645µs–680µs | 1.5K ops/s | 198.87ms | 97.3MB | 0.00KB | 570.1KB | 1.9KB |
| ink | 1 | 839µs | 28.0% | 812µs–866µs | 1.2K ops/s | 251.82ms | 366.2MB | 0.00KB | 569.2KB | 1.9KB |
| rezitui | 1 | 950µs | 69.5% | 875µs–1.02ms | 1.1K ops/s | 284.98ms | 191.0MB | 276.0KB | 2.4MB | 8.0KB |
| rich | 1 | 1.02ms | 21.4% | 1.00ms–1.05ms | 976 ops/s | 307.68ms | 32.3MB | 0.00KB | 569.2KB | 1.9KB |
| prompt-toolkit | 1 | 1.06ms | 19.8% | 1.04ms–1.08ms | 943 ops/s | 318.13ms | 36.7MB | 48.0KB | 569.2KB | 1.9KB |
| urwid | 1 | 1.68ms | 29.9% | 1.62ms–1.74ms | 595 ops/s | 504.75ms | 42.7MB | 0.00KB | 562.5KB | 1.9KB |

## memory-profile

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 127µs | 19.3% | 126µs–128µs | 7.9K ops/s | 254.86ms | 27.0KB | 0.00KB | 31.2MB | 16.0KB |
| rezitui | 1 | 160µs | 469.8% | 127µs–193µs | 6.3K ops/s | 319.70ms | 190.3MB | 0.00KB | 2.6MB | 1.3KB |
| bubbletea | 1 | 173µs | 62.5% | 168µs–177µs | 5.8K ops/s | 346.08ms | 20.3MB | 256.0KB | 31.2MB | 16.0KB |
| opentui-react | 1 | 174µs | 25.1% | 172µs–176µs | 5.7K ops/s | 364.75ms | 39.3MB | 300.0KB | 147.0MB | 75.3KB |
| opentui-core | 1 | 205µs | 42.0% | 201µs–209µs | 4.9K ops/s | 473.02ms | 35.0MB | 0.00KB | 133.8MB | 68.5KB |
| TUIX.Renderer | 1 | 220µs | 23.9% | 218µs–222µs | 4.5K ops/s | 511.79ms | 4.3MB | 0.00KB | 125.9MB | 64.4KB |
| tuix-python | 1 | 237µs | 22.8% | 234µs–239µs | 4.2K ops/s | 543.71ms | 19.0MB | 64.0KB | 133.5MB | 68.3KB |
| TUIX.Core | 1 | 251µs | 23.2% | 249µs–254µs | 4.0K ops/s | 502.78ms | 4.1MB | 0.00KB | 120.6MB | 61.7KB |
| blessed | 1 | 425µs | 35.6% | 418µs–432µs | 2.4K ops/s | 850.23ms | 528.2MB | 0.00KB | 4.6MB | 2.3KB |
| terminal-kit | 1 | 712µs | 22.9% | 705µs–719µs | 1.4K ops/s | 1.42s | 97.3MB | 16.0KB | 3.7MB | 1.9KB |
| ink | 1 | 996µs | 21.3% | 986µs–1.00ms | 1.0K ops/s | 1.99s | 366.2MB | 12.0KB | 3.7MB | 1.9KB |
| prompt-toolkit | 1 | 1.37ms | 20.4% | 1.35ms–1.38ms | 732 ops/s | 2.73s | 36.9MB | 180.0KB | 3.7MB | 1.9KB |
| urwid | 1 | 1.81ms | 20.0% | 1.79ms–1.82ms | 554 ops/s | 3.61s | 42.8MB | 64.0KB | 3.7MB | 1.9KB |
| rich | 1 | 1.98ms | 20.8% | 1.96ms–1.99ms | 506 ops/s | 3.95s | 32.6MB | 300.0KB | 3.7MB | 1.9KB |

## terminal-rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 41µs | 22.1% | 40µs–41µs | 24.6K ops/s | 40.71ms | 4.4MB | 0.00KB | 15.6KB | 16B |
| tuix-python | 1 | 48µs | 29.5% | 48µs–49µs | 20.6K ops/s | 49.34ms | 19.0MB | 0.00KB | 4.6MB | 4.7KB |
| rezitui | 1 | 50µs | 311.1% | 41µs–60µs | 19.8K ops/s | 50.56ms | 190.3MB | 0.00KB | 2.2MB | 2.2KB |
| TUIX.Core | 1 | 61µs | 32.2% | 59µs–62µs | 16.5K ops/s | 60.67ms | 4.5MB | 0.00KB | 15.6KB | 16B |
| opentui-react | 1 | 119µs | 19.3% | 118µs–121µs | 8.4K ops/s | 119.62ms | 39.3MB | 0.00KB | 3.9KB | 4B |
| ratatui | 1 | 176µs | 24.6% | 173µs–179µs | 5.7K ops/s | 176.18ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| bubbletea | 1 | 207µs | 65.7% | 199µs–215µs | 4.8K ops/s | 207.42ms | 20.3MB | 0.00KB | 40.6MB | 41.6KB |
| blessed | 1 | 257µs | 19.8% | 254µs–261µs | 3.9K ops/s | 257.65ms | 528.5MB | 4.0KB | 0.00KB | 0.00KB |
| opentui-core | 1 | 484µs | 41.8% | 472µs–497µs | 2.1K ops/s | 484.77ms | 35.5MB | 0.00KB | 167.1MB | 171.1KB |
| terminal-kit | 1 | 609µs | 24.8% | 600µs–619µs | 1.6K ops/s | 609.63ms | 97.5MB | 64.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.80ms | 22.6% | 1.77ms–1.82ms | 556 ops/s | 1.80s | 365.9MB | 0.00KB | 4.6MB | 4.7KB |
| rich | 1 | 3.20ms | 20.0% | 3.16ms–3.24ms | 313 ops/s | 3.20s | 32.6MB | 16.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 3.73ms | 19.4% | 3.69ms–3.78ms | 268 ops/s | 3.74s | 37.1MB | 8.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 4.28ms | 19.3% | 4.22ms–4.33ms | 234 ops/s | 4.28s | 42.9MB | 64.0KB | 4.6MB | 4.7KB |

## terminal-frame-fill (cols=120,dirtyLines=1,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| tuix-python | 1 | 43µs | 59.2% | 41µs–46µs | 23.1K ops/s | 22.69ms | 19.0MB | 0.00KB | 22.6MB | 46.2KB |
| TUIX.Renderer | 1 | 44µs | 31.4% | 43µs–46µs | 22.5K ops/s | 22.80ms | 4.4MB | 0.00KB | 7.8KB | 16B |
| rezitui | 1 | 69µs | 215.4% | 56µs–83µs | 14.4K ops/s | 34.82ms | 190.8MB | 504.0KB | 1.1MB | 2.2KB |
| TUIX.Core | 1 | 72µs | 21.2% | 70µs–73µs | 14.0K ops/s | 35.86ms | 4.5MB | 0.00KB | 1.2MB | 2.4KB |
| opentui-react | 1 | 138µs | 27.9% | 135µs–142µs | 7.2K ops/s | 72.63ms | 39.3MB | 0.00KB | 4.6MB | 9.5KB |
| ratatui | 1 | 200µs | 13.9% | 197µs–202µs | 5.0K ops/s | 99.91ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| bubbletea | 1 | 279µs | 65.9% | 263µs–295µs | 3.6K ops/s | 139.57ms | 20.3MB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 383µs | 41.1% | 369µs–397µs | 2.6K ops/s | 193.12ms | 36.2MB | 84.0KB | 83.5MB | 171.0KB |
| blessed | 1 | 400µs | 47.1% | 383µs–416µs | 2.5K ops/s | 199.90ms | 528.6MB | 4.0KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 708µs | 23.1% | 693µs–722µs | 1.4K ops/s | 354.09ms | 97.3MB | 0.00KB | 2.3MB | 4.7KB |
| rich | 1 | 1.01ms | 21.1% | 992µs–1.03ms | 989 ops/s | 505.85ms | 32.7MB | 112.0KB | 2.3MB | 4.7KB |
| ink | 1 | 2.02ms | 20.5% | 1.99ms–2.06ms | 494 ops/s | 1.01s | 366.2MB | 308.0KB | 77.6KB | 159B |
| prompt-toolkit | 1 | 2.06ms | 21.3% | 2.02ms–2.09ms | 486 ops/s | 1.03s | 37.1MB | 0.00KB | 2.3MB | 4.7KB |
| urwid | 1 | 2.28ms | 18.3% | 2.25ms–2.32ms | 438 ops/s | 1.14s | 42.9MB | 0.00KB | 2.3MB | 4.7KB |

## terminal-frame-fill (cols=120,dirtyLines=40,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 70µs | 12.4% | 69µs–70µs | 14.4K ops/s | 34.80ms | 4.4MB | 0.00KB | 41.8MB | 85.5KB |
| rezitui | 1 | 285µs | 371.0% | 193µs–378µs | 3.5K ops/s | 142.85ms | 196.6MB | 5.8MB | 1.1MB | 2.2KB |
| ratatui | 1 | 347µs | 23.1% | 340µs–354µs | 2.9K ops/s | 173.53ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 381µs | 28.5% | 372µs–391µs | 2.6K ops/s | 215.39ms | 36.3MB | 152.0KB | 83.5MB | 171.0KB |
| TUIX.Renderer | 1 | 405µs | 26.5% | 396µs–414µs | 2.5K ops/s | 228.15ms | 4.5MB | 0.00KB | 42.8MB | 87.8KB |
| opentui-react | 1 | 487µs | 28.1% | 475µs–499µs | 2.1K ops/s | 258.36ms | 39.3MB | 0.00KB | 92.3MB | 189.0KB |
| tuix-python | 1 | 519µs | 20.2% | 510µs–528µs | 1.9K ops/s | 289.98ms | 19.0MB | 12.0KB | 42.9MB | 87.8KB |
| bubbletea | 1 | 634µs | 49.7% | 606µs–662µs | 1.6K ops/s | 317.21ms | 20.3MB | 0.00KB | 20.3MB | 41.6KB |
| blessed | 1 | 1.12ms | 26.1% | 1.10ms–1.15ms | 890 ops/s | 561.83ms | 528.9MB | 284.0KB | 2.7MB | 5.5KB |
| terminal-kit | 1 | 1.54ms | 20.5% | 1.51ms–1.56ms | 651 ops/s | 768.88ms | 97.8MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 2.18ms | 19.7% | 2.14ms–2.22ms | 458 ops/s | 1.09s | 366.2MB | 0.00KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 3.68ms | 21.3% | 3.61ms–3.75ms | 272 ops/s | 1.84s | 37.1MB | 0.00KB | 2.3MB | 4.7KB |
| urwid | 1 | 4.40ms | 21.9% | 4.31ms–4.48ms | 228 ops/s | 2.20s | 42.9MB | 0.00KB | 2.3MB | 4.7KB |
| rich | 1 | 5.10ms | 19.2% | 5.02ms–5.19ms | 196 ops/s | 2.55s | 32.9MB | 88.0KB | 2.3MB | 4.7KB |

## terminal-screen-transition (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 216µs | 373.0% | 166µs–266µs | 4.6K ops/s | 215.93ms | 203.2MB | 5.7MB | 2.2MB | 2.2KB |
| ratatui | 1 | 299µs | 22.1% | 294µs–303µs | 3.3K ops/s | 298.61ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| TUIX.Renderer | 1 | 370µs | 49.1% | 359µs–381µs | 2.7K ops/s | 423.94ms | 4.6MB | 0.00KB | 157.1MB | 160.9KB |
| opentui-core | 1 | 391µs | 45.1% | 380µs–402µs | 2.6K ops/s | 450.81ms | 36.3MB | 0.00KB | 171.1MB | 175.2KB |
| bubbletea | 1 | 414µs | 54.2% | 400µs–428µs | 2.4K ops/s | 414.86ms | 24.5MB | 4.3MB | 40.6MB | 41.6KB |
| tuix-python | 1 | 451µs | 44.8% | 438µs–463µs | 2.2K ops/s | 505.06ms | 19.5MB | 32.0KB | 166.7MB | 170.8KB |
| TUIX.Core | 1 | 520µs | 43.8% | 506µs–534µs | 1.9K ops/s | 520.39ms | 4.5MB | 0.00KB | 153.2MB | 156.8KB |
| opentui-react | 1 | 547µs | 31.8% | 537µs–558µs | 1.8K ops/s | 566.41ms | 39.3MB | 0.00KB | 187.1MB | 191.6KB |
| blessed | 1 | 1.07ms | 25.7% | 1.05ms–1.08ms | 936 ops/s | 1.07s | 528.9MB | 4.0KB | 5.3MB | 5.4KB |
| terminal-kit | 1 | 1.67ms | 19.2% | 1.65ms–1.69ms | 598 ops/s | 1.67s | 97.7MB | 4.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.97ms | 20.1% | 1.95ms–1.99ms | 508 ops/s | 1.97s | 366.3MB | 16.0KB | 4.6MB | 4.7KB |
| rich | 1 | 2.16ms | 21.8% | 2.13ms–2.19ms | 463 ops/s | 2.16s | 32.9MB | 0.00KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 2.33ms | 18.7% | 2.30ms–2.35ms | 430 ops/s | 2.33s | 37.1MB | 40.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 3.44ms | 19.5% | 3.40ms–3.48ms | 291 ops/s | 3.44s | 42.9MB | 8.0KB | 4.6MB | 4.7KB |

## terminal-fps-stream (channels=12,cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 63µs | 42.6% | 61µs–64µs | 15.9K ops/s | 75.59ms | 4.6MB | 0.00KB | 2.3MB | 2.0KB |
| TUIX.Renderer | 1 | 71µs | 26.2% | 70µs–72µs | 14.1K ops/s | 85.97ms | 4.6MB | 0.00KB | 2.3MB | 2.0KB |
| tuix-python | 1 | 83µs | 31.6% | 82µs–85µs | 12.0K ops/s | 103.32ms | 19.2MB | 36.0KB | 8.4MB | 7.2KB |
| opentui-react | 1 | 338µs | 30.9% | 332µs–344µs | 3.0K ops/s | 429.77ms | 39.3MB | 0.00KB | 202.0MB | 172.3KB |
| ratatui | 1 | 348µs | 26.2% | 343µs–354µs | 2.9K ops/s | 418.19ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| bubbletea | 1 | 379µs | 58.3% | 366µs–391µs | 2.6K ops/s | 455.35ms | 24.5MB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 460µs | 44.2% | 448µs–471µs | 2.2K ops/s | 650.61ms | 36.3MB | 0.00KB | 193.0MB | 164.7KB |
| blessed | 1 | 537µs | 36.8% | 526µs–549µs | 1.9K ops/s | 645.21ms | 529.0MB | 12.0KB | 5.7MB | 4.8KB |
| rezitui | 1 | 723µs | 129.3% | 670µs–776µs | 1.4K ops/s | 868.02ms | 202.7MB | 0.00KB | 4.2MB | 3.6KB |
| terminal-kit | 1 | 1.62ms | 21.0% | 1.60ms–1.64ms | 617 ops/s | 1.95s | 97.6MB | 8.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.91ms | 20.2% | 1.89ms–1.93ms | 523 ops/s | 2.29s | 366.4MB | 20.0KB | 2.5MB | 2.1KB |
| rich | 1 | 2.32ms | 19.5% | 2.29ms–2.34ms | 432 ops/s | 2.78s | 32.9MB | 4.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.59ms | 21.0% | 2.56ms–2.63ms | 385 ops/s | 3.11s | 37.3MB | 116.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 3.23ms | 20.5% | 3.19ms–3.27ms | 310 ops/s | 3.88s | 43.0MB | 68.0KB | 5.5MB | 4.7KB |

## terminal-input-latency (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 35µs | 23.8% | 35µs–36µs | 28.2K ops/s | 35.49ms | 4.6MB | 0.00KB | 15.6KB | 16B |
| tuix-python | 1 | 48µs | 48.6% | 47µs–50µs | 20.7K ops/s | 49.93ms | 19.3MB | 32.0KB | 4.6MB | 4.7KB |
| TUIX.Core | 1 | 68µs | 30.6% | 66µs–69µs | 14.8K ops/s | 67.79ms | 4.6MB | 0.00KB | 52.7KB | 54B |
| rezitui | 1 | 86µs | 276.6% | 71µs–101µs | 11.6K ops/s | 86.26ms | 204.1MB | 1.5MB | 2.2MB | 2.2KB |
| opentui-react | 1 | 168µs | 33.9% | 164µs–171µs | 6.0K ops/s | 200.13ms | 39.3MB | 0.00KB | 833.0KB | 853B |
| ratatui | 1 | 187µs | 20.1% | 184µs–189µs | 5.4K ops/s | 186.67ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| blessed | 1 | 498µs | 34.9% | 487µs–509µs | 2.0K ops/s | 498.09ms | 529.0MB | 0.00KB | 0.00KB | 0.00KB |
| bubbletea | 1 | 557µs | 54.4% | 539µs–576µs | 1.8K ops/s | 557.85ms | 24.5MB | 0.00KB | 40.6MB | 41.6KB |
| opentui-core | 1 | 608µs | 48.2% | 590µs–627µs | 1.6K ops/s | 609.75ms | 36.3MB | 0.00KB | 166.6MB | 170.6KB |
| terminal-kit | 1 | 630µs | 25.5% | 620µs–640µs | 1.6K ops/s | 630.73ms | 97.6MB | 0.00KB | 4.6MB | 4.7KB |
| rich | 1 | 1.65ms | 20.3% | 1.63ms–1.67ms | 607 ops/s | 1.65s | 33.4MB | 472.0KB | 4.6MB | 4.7KB |
| ink | 1 | 2.12ms | 21.1% | 2.09ms–2.14ms | 473 ops/s | 2.12s | 365.9MB | 4.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 2.52ms | 19.3% | 2.49ms–2.55ms | 397 ops/s | 2.52s | 37.3MB | 84.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 2.92ms | 18.8% | 2.89ms–2.95ms | 342 ops/s | 2.92s | 43.0MB | 44.0KB | 4.6MB | 4.7KB |

## terminal-memory-soak (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 284µs | 349.5% | 228µs–340µs | 3.5K ops/s | 340.92ms | 212.1MB | 8.0MB | 2.6MB | 2.2KB |
| ratatui | 1 | 297µs | 25.2% | 293µs–301µs | 3.4K ops/s | 356.42ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| bubbletea | 1 | 423µs | 56.3% | 410µs–437µs | 2.4K ops/s | 508.55ms | 24.5MB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 516µs | 29.5% | 507µs–524µs | 1.9K ops/s | 633.15ms | 39.3MB | 0.00KB | 222.5MB | 189.9KB |
| TUIX.Renderer | 1 | 530µs | 26.1% | 522µs–537µs | 1.9K ops/s | 739.75ms | 4.6MB | 0.00KB | 188.5MB | 160.9KB |
| opentui-core | 1 | 566µs | 31.8% | 556µs–576µs | 1.8K ops/s | 767.70ms | 36.3MB | 0.00KB | 200.4MB | 171.0KB |
| TUIX.Core | 1 | 606µs | 23.7% | 598µs–614µs | 1.7K ops/s | 726.87ms | 4.6MB | 0.00KB | 183.8MB | 156.8KB |
| tuix-python | 1 | 640µs | 16.9% | 634µs–646µs | 1.6K ops/s | 881.02ms | 19.3MB | 40.0KB | 200.1MB | 170.8KB |
| blessed | 1 | 1.09ms | 25.8% | 1.08ms–1.11ms | 914 ops/s | 1.31s | 529.0MB | 0.00KB | 6.5MB | 5.5KB |
| terminal-kit | 1 | 1.75ms | 19.8% | 1.73ms–1.76ms | 573 ops/s | 2.10s | 97.7MB | 68.0KB | 5.5MB | 4.7KB |
| ink | 1 | 2.02ms | 21.4% | 2.00ms–2.05ms | 495 ops/s | 2.43s | 365.9MB | 28.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 3.61ms | 20.5% | 3.57ms–3.66ms | 277 ops/s | 4.34s | 37.2MB | 0.00KB | 5.5MB | 4.7KB |
| urwid | 1 | 4.18ms | 19.8% | 4.14ms–4.23ms | 239 ops/s | 5.02s | 43.0MB | 28.0KB | 5.5MB | 4.7KB |
| rich | 1 | 4.66ms | 25.3% | 4.59ms–4.72ms | 215 ops/s | 5.59s | 33.5MB | 0.00KB | 5.5MB | 4.7KB |

## terminal-full-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 61µs | 55.1% | 59µs–63µs | 16.4K ops/s | 73.27ms | 4.6MB | 0.00KB | 3.9MB | 3.3KB |
| TUIX.Renderer | 1 | 99µs | 25.6% | 98µs–101µs | 10.1K ops/s | 130.30ms | 4.6MB | 0.00KB | 3.9MB | 3.3KB |
| tuix-python | 1 | 129µs | 19.8% | 128µs–131µs | 7.7K ops/s | 166.19ms | 19.4MB | 88.0KB | 6.5MB | 5.6KB |
| ratatui | 1 | 260µs | 23.5% | 257µs–264µs | 3.8K ops/s | 312.68ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 404µs | 30.2% | 397µs–411µs | 2.5K ops/s | 506.15ms | 39.3MB | 0.00KB | 149.5MB | 127.6KB |
| opentui-core | 1 | 436µs | 42.6% | 425µs–446µs | 2.3K ops/s | 579.95ms | 36.3MB | 0.00KB | 204.3MB | 174.4KB |
| bubbletea | 1 | 645µs | 43.7% | 629µs–661µs | 1.6K ops/s | 774.07ms | 24.5MB | 0.00KB | 48.8MB | 41.6KB |
| blessed | 1 | 728µs | 33.3% | 714µs–741µs | 1.4K ops/s | 873.40ms | 529.0MB | 0.00KB | 3.8MB | 3.2KB |
| rezitui | 1 | 1.13ms | 91.9% | 1.08ms–1.19ms | 881 ops/s | 1.36s | 214.8MB | 1.7MB | 4.3MB | 3.7KB |
| terminal-kit | 1 | 1.32ms | 21.8% | 1.30ms–1.34ms | 758 ops/s | 1.58s | 98.1MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 1.94ms | 21.9% | 1.91ms–1.96ms | 517 ops/s | 2.32s | 366.7MB | 184.0KB | 3.4MB | 2.9KB |
| rich | 1 | 2.10ms | 20.1% | 2.07ms–2.12ms | 477 ops/s | 2.52s | 33.7MB | 156.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.47ms | 24.6% | 2.43ms–2.50ms | 406 ops/s | 2.96s | 37.4MB | 232.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 3.31ms | 20.2% | 3.27ms–3.35ms | 302 ops/s | 3.97s | 43.1MB | 60.0KB | 5.5MB | 4.7KB |

## terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 66µs | 32.5% | 65µs–67µs | 15.1K ops/s | 79.37ms | 4.5MB | 0.00KB | 69.9MB | 59.7KB |
| TUIX.Renderer | 1 | 141µs | 50.9% | 137µs–145µs | 7.1K ops/s | 197.44ms | 4.6MB | 0.00KB | 69.9MB | 59.7KB |
| tuix-python | 1 | 168µs | 49.5% | 163µs–173µs | 6.0K ops/s | 227.60ms | 19.4MB | 4.0KB | 74.7MB | 63.7KB |
| ratatui | 1 | 270µs | 19.8% | 266µs–273µs | 3.7K ops/s | 323.55ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 410µs | 41.4% | 400µs–419µs | 2.4K ops/s | 540.73ms | 36.3MB | 0.00KB | 204.3MB | 174.4KB |
| opentui-react | 1 | 436µs | 38.7% | 426µs–445µs | 2.3K ops/s | 537.57ms | 39.3MB | 0.00KB | 149.5MB | 127.6KB |
| bubbletea | 1 | 706µs | 38.1% | 690µs–721µs | 1.4K ops/s | 847.34ms | 24.5MB | 0.00KB | 48.8MB | 41.6KB |
| blessed | 1 | 862µs | 30.9% | 847µs–877µs | 1.2K ops/s | 1.03s | 529.0MB | 0.00KB | 3.8MB | 3.2KB |
| rezitui | 1 | 992µs | 81.4% | 946µs–1.04ms | 1.0K ops/s | 1.19s | 214.7MB | 0.00KB | 4.4MB | 3.7KB |
| terminal-kit | 1 | 1.34ms | 23.3% | 1.32ms–1.36ms | 747 ops/s | 1.61s | 98.2MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 1.92ms | 21.5% | 1.89ms–1.94ms | 522 ops/s | 2.30s | 366.7MB | 0.00KB | 5.4MB | 4.6KB |
| rich | 1 | 2.02ms | 20.2% | 2.00ms–2.05ms | 495 ops/s | 2.43s | 33.7MB | 24.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.51ms | 19.6% | 2.48ms–2.53ms | 399 ops/s | 3.01s | 37.5MB | 120.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 3.12ms | 20.5% | 3.09ms–3.16ms | 320 ops/s | 3.75s | 43.1MB | 40.0KB | 5.5MB | 4.7KB |

## terminal-strict-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 67µs | 28.1% | 66µs–68µs | 15.0K ops/s | 80.06ms | 4.7MB | 0.00KB | 3.9MB | 3.3KB |
| TUIX.Renderer | 1 | 111µs | 21.0% | 109µs–112µs | 9.0K ops/s | 146.03ms | 4.6MB | 0.00KB | 3.9MB | 3.3KB |
| tuix-python | 1 | 133µs | 17.5% | 131µs–134µs | 7.5K ops/s | 171.11ms | 19.5MB | 64.0KB | 6.5MB | 5.6KB |
| ratatui | 1 | 238µs | 22.9% | 235µs–241µs | 4.2K ops/s | 285.90ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 386µs | 32.6% | 379µs–393µs | 2.6K ops/s | 463.87ms | 36.3MB | 0.00KB | 186.1MB | 158.8KB |
| bubbletea | 1 | 699µs | 39.8% | 683µs–714µs | 1.4K ops/s | 839.08ms | 24.5MB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 783µs | 125.5% | 727µs–838µs | 1.3K ops/s | 940.46ms | 39.3MB | 0.00KB | 240.6MB | 205.3KB |
| rezitui | 1 | 810µs | 84.9% | 771µs–849µs | 1.2K ops/s | 973.11ms | 214.8MB | 28.0KB | 4.3MB | 3.7KB |
| blessed | 1 | 853µs | 28.3% | 839µs–867µs | 1.2K ops/s | 1.02s | 529.0MB | 0.00KB | 3.8MB | 3.2KB |
| terminal-kit | 1 | 1.35ms | 21.5% | 1.34ms–1.37ms | 739 ops/s | 1.63s | 98.2MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 1.82ms | 22.5% | 1.79ms–1.84ms | 551 ops/s | 2.18s | 366.7MB | 0.00KB | 3.4MB | 2.9KB |
| rich | 1 | 2.05ms | 19.9% | 2.02ms–2.07ms | 489 ops/s | 2.46s | 33.7MB | 40.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.46ms | 23.5% | 2.43ms–2.50ms | 406 ops/s | 2.96s | 37.6MB | 32.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 3.02ms | 19.4% | 2.99ms–3.06ms | 331 ops/s | 3.63s | 43.2MB | 52.0KB | 5.5MB | 4.7KB |

## terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 71µs | 30.6% | 70µs–73µs | 14.0K ops/s | 85.89ms | 4.7MB | 0.00KB | 69.9MB | 59.7KB |
| tuix-python | 1 | 140µs | 51.3% | 136µs–144µs | 7.1K ops/s | 189.91ms | 19.5MB | 0.00KB | 74.7MB | 63.7KB |
| TUIX.Renderer | 1 | 148µs | 51.5% | 144µs–152µs | 6.8K ops/s | 209.33ms | 4.6MB | 0.00KB | 69.9MB | 59.7KB |
| ratatui | 1 | 240µs | 28.1% | 236µs–243µs | 4.2K ops/s | 287.73ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 354µs | 39.7% | 346µs–361µs | 2.8K ops/s | 424.84ms | 36.3MB | 0.00KB | 186.1MB | 158.8KB |
| bubbletea | 1 | 707µs | 36.3% | 692µs–721µs | 1.4K ops/s | 848.61ms | 24.5MB | 0.00KB | 48.8MB | 41.6KB |
| blessed | 1 | 791µs | 33.9% | 775µs–806µs | 1.3K ops/s | 949.13ms | 529.0MB | 4.0KB | 3.8MB | 3.2KB |
| opentui-react | 1 | 868µs | 42.7% | 847µs–889µs | 1.2K ops/s | 1.04s | 39.3MB | 0.00KB | 240.6MB | 205.3KB |
| rezitui | 1 | 869µs | 76.1% | 832µs–906µs | 1.2K ops/s | 1.04s | 214.8MB | 0.00KB | 4.4MB | 3.7KB |
| terminal-kit | 1 | 1.35ms | 21.4% | 1.34ms–1.37ms | 740 ops/s | 1.62s | 98.2MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 1.88ms | 20.4% | 1.86ms–1.90ms | 532 ops/s | 2.26s | 366.7MB | 0.00KB | 5.4MB | 4.6KB |
| rich | 1 | 2.01ms | 20.3% | 1.99ms–2.03ms | 498 ops/s | 2.41s | 33.8MB | 36.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.55ms | 29.1% | 2.51ms–2.59ms | 392 ops/s | 3.06s | 37.5MB | 0.00KB | 5.5MB | 4.7KB |
| urwid | 1 | 3.07ms | 19.1% | 3.04ms–3.11ms | 325 ops/s | 3.69s | 43.3MB | 80.0KB | 5.5MB | 4.7KB |

## terminal-virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 70µs | 14.2% | 69µs–70µs | 14.3K ops/s | 69.83ms | 4.6MB | 0.00KB | 5.2MB | 5.3KB |
| rezitui | 1 | 85µs | 189.4% | 75µs–95µs | 11.8K ops/s | 85.26ms | 214.8MB | 0.00KB | 4.1MB | 4.2KB |
| opentui-react | 1 | 141µs | 30.0% | 138µs–143µs | 7.1K ops/s | 144.02ms | 39.3MB | 0.00KB | 3.0MB | 3.1KB |
| TUIX.Renderer | 1 | 187µs | 30.4% | 184µs–191µs | 5.3K ops/s | 202.77ms | 4.6MB | 0.00KB | 5.3MB | 5.4KB |
| ratatui | 1 | 230µs | 11.7% | 228µs–232µs | 4.3K ops/s | 230.22ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| tuix-python | 1 | 233µs | 28.5% | 229µs–237µs | 4.3K ops/s | 247.63ms | 19.6MB | 64.0KB | 5.2MB | 5.3KB |
| blessed | 1 | 235µs | 142.7% | 214µs–256µs | 4.3K ops/s | 235.29ms | 529.9MB | 0.00KB | 1.0MB | 1.0KB |
| bubbletea | 1 | 301µs | 75.5% | 287µs–315µs | 3.3K ops/s | 301.18ms | 24.5MB | 0.00KB | 40.6MB | 41.6KB |
| opentui-core | 1 | 483µs | 53.0% | 467µs–499µs | 2.1K ops/s | 548.65ms | 36.3MB | 0.00KB | 165.1MB | 169.0KB |
| rich | 1 | 1.01ms | 20.0% | 999µs–1.02ms | 989 ops/s | 1.01s | 33.8MB | 48.0KB | 4.6MB | 4.7KB |
| terminal-kit | 1 | 1.64ms | 20.4% | 1.62ms–1.66ms | 611 ops/s | 1.64s | 129.8MB | 332.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 1.75ms | 19.7% | 1.73ms–1.77ms | 572 ops/s | 1.75s | 37.5MB | 0.00KB | 4.6MB | 4.7KB |
| ink | 1 | 1.92ms | 23.8% | 1.89ms–1.95ms | 521 ops/s | 1.92s | 368.2MB | 0.00KB | 262.7KB | 269B |
| urwid | 1 | 2.47ms | 20.8% | 2.44ms–2.51ms | 404 ops/s | 2.48s | 43.3MB | 0.00KB | 4.6MB | 4.7KB |

## terminal-table (cols=8,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 68µs | 20.1% | 66µs–69µs | 14.8K ops/s | 33.82ms | 4.7MB | 0.00KB | 5.1MB | 10.5KB |
| TUIX.Renderer | 1 | 186µs | 23.4% | 182µs–190µs | 5.4K ops/s | 105.41ms | 4.6MB | 0.00KB | 5.2MB | 10.7KB |
| ratatui | 1 | 270µs | 20.7% | 265µs–275µs | 3.7K ops/s | 135.04ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| tuix-python | 1 | 289µs | 26.5% | 282µs–295µs | 3.5K ops/s | 171.94ms | 19.6MB | 0.00KB | 12.6MB | 25.8KB |
| opentui-core | 1 | 373µs | 37.3% | 360µs–385µs | 2.7K ops/s | 226.47ms | 36.3MB | 0.00KB | 82.5MB | 169.0KB |
| bubbletea | 1 | 451µs | 36.4% | 436µs–465µs | 2.2K ops/s | 225.56ms | 24.5MB | 0.00KB | 20.3MB | 41.6KB |
| opentui-react | 1 | 521µs | 30.4% | 507µs–534µs | 1.9K ops/s | 278.75ms | 39.3MB | 0.00KB | 91.4MB | 187.3KB |
| blessed | 1 | 1.20ms | 29.0% | 1.17ms–1.23ms | 832 ops/s | 600.98ms | 529.4MB | 0.00KB | 3.0MB | 6.2KB |
| rezitui | 1 | 1.21ms | 64.2% | 1.14ms–1.27ms | 829 ops/s | 603.67ms | 214.1MB | 2.7MB | 7.6MB | 15.6KB |
| terminal-kit | 1 | 1.74ms | 19.4% | 1.71ms–1.77ms | 576 ops/s | 868.82ms | 129.7MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 1.89ms | 22.8% | 1.85ms–1.93ms | 529 ops/s | 945.11ms | 367.9MB | 0.00KB | 2.3MB | 4.7KB |
| rich | 1 | 2.12ms | 19.5% | 2.08ms–2.15ms | 473 ops/s | 1.06s | 33.8MB | 0.00KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 3.05ms | 19.6% | 3.00ms–3.10ms | 328 ops/s | 1.53s | 37.6MB | 28.0KB | 2.3MB | 4.7KB |
| urwid | 1 | 3.68ms | 18.5% | 3.62ms–3.74ms | 272 ops/s | 1.84s | 43.3MB | 0.00KB | 2.3MB | 4.7KB |

## Relative Performance (vs fastest per scenario)

> Ratio = framework mean / fastest mean for each scenario. Lower is better. 1.0x = fastest.

| Scenario | blessed | terminal-kit | ink | rezitui | bubbletea | ratatui | opentui-core | opentui-react | TUIX.Renderer | TUIX.Core | tuix-python | rich | urwid | prompt-toolkit |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| startup | 133.3x [128.5x, 138.1x] | 2.5x [2.4x, 2.6x] | 12.5x [11.8x, 13.1x] | 1.3x [0.9x, 1.8x] | 1.2x [1.0x, 1.3x] | **1.0x** | 2.2x [2.0x, 2.4x] | 1.7x [1.5x, 1.9x] | 1.1x [1.0x, 1.1x] | 1.2x [1.1x, 1.2x] | 1.4x [1.4x, 1.4x] | 8.0x [7.7x, 8.3x] | 8.6x [8.1x, 9.2x] | 7.1x [6.9x, 7.2x] |
| tree-construction (items=10) | 1.5x [1.2x, 1.9x] | 5.4x [5.3x, 5.5x] | 11.8x [11.4x, 12.2x] | 2.5x [1.9x, 3.0x] | 5.0x [4.8x, 5.2x] | 1.6x [1.5x, 1.6x] | 6.4x [6.0x, 6.7x] | **1.0x** | 1.2x [1.2x, 1.3x] | **1.0x** | 1.4x [1.4x, 1.5x] | 15.7x [15.4x, 16.1x] | 23.3x [22.8x, 23.7x] | 18.1x [17.8x, 18.5x] |
| tree-construction (items=100) | 6.0x [5.4x, 6.6x] | 20.9x [20.5x, 21.3x] | 62.8x [61.7x, 63.9x] | 2.0x [1.7x, 2.2x] | 4.7x [4.5x, 4.8x] | 2.3x [2.2x, 2.3x] | 2.2x [1.8x, 2.5x] | 1.6x [1.5x, 1.6x] | 1.5x [1.5x, 1.6x] | **1.0x** | 2.4x [2.4x, 2.5x] | 11.0x [10.8x, 11.3x] | 17.0x [16.7x, 17.3x] | 14.2x [14.0x, 14.4x] |
| tree-construction (items=500) | 33.9x [32.3x, 35.5x] | 103.1x [101.4x, 104.9x] | 332.8x [328.0x, 337.6x] | 5.7x [4.0x, 7.3x] | 10.5x [10.2x, 10.7x] | 8.0x [7.9x, 8.1x] | 2.3x [2.1x, 2.4x] | 3.7x [3.1x, 4.2x] | 1.0x [1.0x, 1.1x] | **1.0x** | 2.5x [2.5x, 2.6x] | 12.5x [12.3x, 12.8x] | 18.0x [17.7x, 18.3x] | 16.3x [16.0x, 16.5x] |
| tree-construction (items=1000) | 65.3x [62.9x, 67.6x] | 202.4x [198.6x, 206.3x] | 661.8x [653.7x, 670.0x] | 7.7x [6.6x, 8.8x] | 15.3x [14.8x, 15.7x] | 15.7x [15.5x, 15.9x] | 1.6x [1.5x, 1.7x] | 3.8x [3.7x, 3.8x] | **1.0x** | 1.5x [1.5x, 1.6x] | 2.6x [2.6x, 2.7x] | 15.0x [14.8x, 15.3x] | 20.4x [20.0x, 20.7x] | 14.8x [14.6x, 15.1x] |
| rerender | 2.0x [1.6x, 2.4x] | 14.9x [14.6x, 15.2x] | 46.3x [45.5x, 47.0x] | 4.2x [3.4x, 4.9x] | 9.2x [8.9x, 9.6x] | 3.8x [3.7x, 3.9x] | 10.5x [10.1x, 10.9x] | 2.8x [2.7x, 2.8x] | **1.0x** | 1.1x [1.0x, 1.1x] | 1.4x [1.3x, 1.5x] | 90.6x [89.5x, 91.8x] | 107.9x [106.6x, 109.3x] | 74.2x [73.4x, 75.1x] |
| content-update | 12.4x [11.8x, 13.0x] | 14.3x [14.1x, 14.6x] | 135.6x [132.7x, 138.4x] | 3.1x [2.4x, 3.9x] | 13.7x [13.2x, 14.2x] | 4.3x [4.2x, 4.4x] | 6.0x [5.8x, 6.2x] | 6.7x [2.3x, 11.1x] | 5.3x [5.2x, 5.5x] | **1.0x** | 5.2x [5.1x, 5.2x] | 60.7x [59.6x, 61.7x] | 67.2x [66.1x, 68.3x] | 44.5x [43.7x, 45.3x] |
| layout-stress (cols=4,rows=10) | 4.7x [4.0x, 5.5x] | 12.6x [12.3x, 12.9x] | 113.4x [110.9x, 115.9x] | 27.7x [26.1x, 29.4x] | 4.1x [3.9x, 4.2x] | 1.9x [1.9x, 2.0x] | 1.8x [1.7x, 1.9x] | **1.0x** | 1.6x [1.6x, 1.7x] | 2.0x [2.0x, 2.1x] | 1.1x [1.1x, 1.1x] | 15.8x [15.5x, 16.1x] | 29.0x [28.5x, 29.6x] | 17.3x [17.0x, 17.7x] |
| scroll-stress (items=2000) | 189.4x [179.7x, 199.2x] | 39.3x [37.1x, 41.4x] | 79.9x [68.1x, 91.6x] | 2.9x [2.8x, 3.1x] | 4.9x [4.0x, 5.8x] | 3.9x [3.8x, 4.0x] | 4.2x [3.6x, 4.8x] | 2.2x [1.9x, 2.4x] | 5.0x [4.5x, 5.4x] | **1.0x** | 6.3x [6.0x, 6.7x] | 37.8x [36.2x, 39.4x] | 40.3x [38.0x, 42.6x] | 32.7x [31.6x, 33.7x] |
| virtual-list (items=100000,viewport=40) | 5.4x [4.4x, 6.3x] | 21.7x [21.4x, 22.0x] | 26.4x [25.8x, 27.0x] | 4.3x [3.8x, 4.8x] | 4.9x [4.6x, 5.1x] | 4.1x [4.0x, 4.2x] | 4.0x [3.9x, 4.2x] | 1.6x [1.5x, 1.6x] | 3.1x [3.0x, 3.2x] | **1.0x** | 2.6x [2.6x, 2.7x] | 21.4x [21.1x, 21.7x] | 36.6x [36.1x, 37.1x] | 22.4x [22.1x, 22.7x] |
| tables (cols=8,rows=100) | 15.3x [14.6x, 15.9x] | 20.1x [19.5x, 20.6x] | 25.4x [24.6x, 26.2x] | 28.8x [26.5x, 31.1x] | 5.9x [5.5x, 6.2x] | 4.5x [4.5x, 4.6x] | 4.4x [4.1x, 4.6x] | 7.1x [6.7x, 7.4x] | 2.8x [2.7x, 2.9x] | **1.0x** | 2.8x [2.7x, 2.9x] | 31.1x [30.3x, 31.8x] | 51.0x [49.3x, 52.7x] | 32.1x [31.4x, 32.9x] |
| memory-profile | 3.3x [3.3x, 3.4x] | 5.6x [5.5x, 5.6x] | 7.8x [7.7x, 7.9x] | 1.3x [1.0x, 1.5x] | 1.4x [1.3x, 1.4x] | **1.0x** | 1.6x [1.6x, 1.6x] | 1.4x [1.4x, 1.4x] | 1.7x [1.7x, 1.7x] | 2.0x [2.0x, 2.0x] | 1.9x [1.8x, 1.9x] | 15.5x [15.4x, 15.7x] | 14.2x [14.1x, 14.3x] | 10.7x [10.6x, 10.8x] |
| terminal-rerender | 6.3x [6.3x, 6.4x] | 15.0x [14.8x, 15.2x] | 44.2x [43.6x, 44.8x] | 1.2x [1.0x, 1.5x] | 5.1x [4.9x, 5.3x] | 4.3x [4.3x, 4.4x] | 11.9x [11.6x, 12.2x] | 2.9x [2.9x, 3.0x] | **1.0x** | 1.5x [1.5x, 1.5x] | 1.2x [1.2x, 1.2x] | 78.6x [77.6x, 79.6x] | 105.2x [103.9x, 106.4x] | 91.8x [90.7x, 92.9x] |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | 9.2x [8.9x, 9.6x] | 16.4x [16.0x, 16.7x] | 46.8x [46.0x, 47.6x] | 1.6x [1.3x, 1.9x] | 6.4x [6.1x, 6.8x] | 4.6x [4.6x, 4.7x] | 8.8x [8.5x, 9.2x] | 3.2x [3.1x, 3.3x] | 1.0x [1.0x, 1.1x] | 1.7x [1.6x, 1.7x] | **1.0x** | 23.4x [22.9x, 23.8x] | 52.8x [51.9x, 53.6x] | 47.5x [46.6x, 48.4x] |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | 16.2x [15.8x, 16.5x] | 22.1x [21.7x, 22.5x] | 31.4x [30.8x, 31.9x] | 4.1x [2.8x, 5.4x] | 9.1x [8.7x, 9.5x] | 5.0x [4.9x, 5.1x] | 5.5x [5.3x, 5.6x] | 7.0x [6.8x, 7.2x] | 5.8x [5.7x, 6.0x] | **1.0x** | 7.5x [7.3x, 7.6x] | 73.4x [72.2x, 74.6x] | 63.2x [62.0x, 64.4x] | 52.9x [51.9x, 53.9x] |
| terminal-screen-transition (cols=120,rows=40) | 4.9x [4.9x, 5.0x] | 7.7x [7.7x, 7.8x] | 9.1x [9.0x, 9.2x] | **1.0x** | 1.9x [1.9x, 2.0x] | 1.4x [1.4x, 1.4x] | 1.8x [1.8x, 1.9x] | 2.5x [2.5x, 2.6x] | 1.7x [1.7x, 1.8x] | 2.4x [2.3x, 2.5x] | 2.1x [2.0x, 2.1x] | 10.0x [9.9x, 10.1x] | 15.9x [15.7x, 16.1x] | 10.8x [10.7x, 10.9x] |
| terminal-fps-stream (channels=12,cols=120,rows=40) | 8.5x [8.4x, 8.7x] | 25.8x [25.5x, 26.1x] | 30.4x [30.0x, 30.7x] | 11.5x [10.6x, 12.3x] | 6.0x [5.8x, 6.2x] | 5.5x [5.5x, 5.6x] | 7.3x [7.1x, 7.5x] | 5.4x [5.3x, 5.5x] | 1.1x [1.1x, 1.1x] | **1.0x** | 1.3x [1.3x, 1.3x] | 36.8x [36.4x, 37.2x] | 51.3x [50.7x, 51.9x] | 41.2x [40.7x, 41.7x] |
| terminal-input-latency (cols=120,rows=40) | 14.0x [13.7x, 14.4x] | 17.8x [17.5x, 18.1x] | 59.7x [58.9x, 60.5x] | 2.4x [2.0x, 2.8x] | 15.7x [15.2x, 16.3x] | 5.3x [5.2x, 5.3x] | 17.2x [16.6x, 17.7x] | 4.7x [4.6x, 4.8x] | **1.0x** | 1.9x [1.9x, 1.9x] | 1.4x [1.3x, 1.4x] | 46.5x [45.9x, 47.1x] | 82.4x [81.4x, 83.4x] | 71.1x [70.2x, 71.9x] |
| terminal-memory-soak (cols=120,rows=40) | 3.9x [3.8x, 3.9x] | 6.1x [6.1x, 6.2x] | 7.1x [7.0x, 7.2x] | **1.0x** | 1.5x [1.4x, 1.5x] | 1.0x [1.0x, 1.1x] | 2.0x [2.0x, 2.0x] | 1.8x [1.8x, 1.8x] | 1.9x [1.8x, 1.9x] | 2.1x [2.1x, 2.2x] | 2.3x [2.2x, 2.3x] | 16.4x [16.2x, 16.6x] | 14.7x [14.6x, 14.9x] | 12.7x [12.6x, 12.9x] |
| terminal-full-ui (cols=120,rows=40,services=24) | 11.9x [11.7x, 12.2x] | 21.6x [21.4x, 21.9x] | 31.7x [31.3x, 32.1x] | 18.6x [17.6x, 19.6x] | 10.6x [10.3x, 10.8x] | 4.3x [4.2x, 4.3x] | 7.1x [7.0x, 7.3x] | 6.6x [6.5x, 6.7x] | 1.6x [1.6x, 1.7x] | **1.0x** | 2.1x [2.1x, 2.1x] | 34.4x [34.0x, 34.8x] | 54.3x [53.7x, 54.9x] | 40.4x [39.9x, 41.0x] |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 13.0x [12.8x, 13.3x] | 20.2x [20.0x, 20.5x] | 29.0x [28.6x, 29.3x] | 15.0x [14.3x, 15.7x] | 10.7x [10.4x, 10.9x] | 4.1x [4.0x, 4.1x] | 6.2x [6.1x, 6.3x] | 6.6x [6.4x, 6.7x] | 2.1x [2.1x, 2.2x] | **1.0x** | 2.5x [2.5x, 2.6x] | 30.6x [30.2x, 30.9x] | 47.3x [46.7x, 47.8x] | 37.9x [37.5x, 38.3x] |
| terminal-strict-ui (cols=120,rows=40,services=24) | 12.8x [12.6x, 13.0x] | 20.3x [20.1x, 20.6x] | 27.2x [26.9x, 27.6x] | 12.2x [11.6x, 12.7x] | 10.5x [10.2x, 10.7x] | 3.6x [3.5x, 3.6x] | 5.8x [5.7x, 5.9x] | 11.7x [10.9x, 12.6x] | 1.7x [1.6x, 1.7x] | **1.0x** | 2.0x [2.0x, 2.0x] | 30.7x [30.4x, 31.1x] | 45.3x [44.8x, 45.8x] | 37.0x [36.5x, 37.5x] |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 11.1x [10.9x, 11.3x] | 18.9x [18.7x, 19.1x] | 26.3x [26.0x, 26.6x] | 12.2x [11.6x, 12.7x] | 9.9x [9.7x, 10.1x] | 3.4x [3.3x, 3.4x] | 4.9x [4.8x, 5.1x] | 12.1x [11.8x, 12.4x] | 2.1x [2.0x, 2.1x] | **1.0x** | 2.0x [1.9x, 2.0x] | 28.1x [27.8x, 28.4x] | 43.0x [42.5x, 43.5x] | 35.7x [35.1x, 36.3x] |
| terminal-virtual-list (items=100000,viewport=40) | 3.4x [3.1x, 3.7x] | 23.5x [23.2x, 23.8x] | 27.5x [27.1x, 27.9x] | 1.2x [1.1x, 1.4x] | 4.3x [4.1x, 4.5x] | 3.3x [3.3x, 3.3x] | 6.9x [6.7x, 7.1x] | 2.0x [2.0x, 2.1x] | 2.7x [2.6x, 2.7x] | **1.0x** | 3.3x [3.3x, 3.4x] | 14.5x [14.3x, 14.7x] | 35.5x [35.0x, 35.9x] | 25.0x [24.7x, 25.3x] |
| terminal-table (cols=8,rows=40) | 17.8x [17.3x, 18.2x] | 25.7x [25.3x, 26.1x] | 28.0x [27.4x, 28.5x] | 17.9x [16.9x, 18.9x] | 6.7x [6.5x, 6.9x] | 4.0x [3.9x, 4.1x] | 5.5x [5.3x, 5.7x] | 7.7x [7.5x, 7.9x] | 2.8x [2.7x, 2.8x] | **1.0x** | 4.3x [4.2x, 4.4x] | 31.3x [30.8x, 31.9x] | 54.5x [53.6x, 55.4x] | 45.2x [44.4x, 45.9x] |

## Memory Comparison

| Scenario | Framework | Peak RSS | RSS Before | RSS After | RSS Growth |
|---|---|---:|---:|---:|---:|
| startup | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| startup | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 4.0KB |
| startup | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 4.0KB |
| startup | bubbletea | 16.0MB | 10.5MB | 16.0MB | 5.4MB |
| startup | tuix-python | 18.4MB | 18.4MB | 18.4MB | 60.0KB |
| startup | rich | 31.5MB | 30.6MB | 31.5MB | 920.0KB |
| startup | opentui-core | 33.3MB | 32.9MB | 33.3MB | 416.0KB |
| startup | prompt-toolkit | 35.7MB | 35.7MB | 35.7MB | 16.0KB |
| startup | opentui-react | 35.9MB | 35.5MB | 35.9MB | 424.0KB |
| startup | urwid | 42.5MB | 42.4MB | 42.5MB | 24.0KB |
| startup | terminal-kit | 46.4MB | 46.5MB | 46.4MB | 0.00KB |
| startup | ink | 64.9MB | 62.2MB | 64.9MB | 2.7MB |
| startup | rezitui | 81.8MB | 67.9MB | 81.8MB | 14.0MB |
| startup | blessed | 152.6MB | 77.7MB | 152.6MB | 74.9MB |
| tree-construction (items=10) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=10) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=10) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tree-construction (items=10) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 48.0KB |
| tree-construction (items=10) | tuix-python | 18.5MB | 18.5MB | 18.5MB | 16.0KB |
| tree-construction (items=10) | rich | 31.6MB | 31.5MB | 31.6MB | 28.0KB |
| tree-construction (items=10) | opentui-core | 33.9MB | 33.3MB | 33.9MB | 640.0KB |
| tree-construction (items=10) | prompt-toolkit | 35.8MB | 35.7MB | 35.8MB | 16.0KB |
| tree-construction (items=10) | opentui-react | 36.0MB | 35.9MB | 36.0MB | 84.0KB |
| tree-construction (items=10) | urwid | 42.5MB | 42.5MB | 42.5MB | 76.0KB |
| tree-construction (items=10) | terminal-kit | 46.6MB | 46.6MB | 46.6MB | 8.0KB |
| tree-construction (items=10) | ink | 68.9MB | 65.8MB | 68.9MB | 3.1MB |
| tree-construction (items=10) | rezitui | 98.4MB | 83.3MB | 98.4MB | 15.1MB |
| tree-construction (items=10) | blessed | 161.0MB | 154.7MB | 161.0MB | 6.2MB |
| tree-construction (items=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=100) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=100) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tree-construction (items=100) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 16.0KB |
| tree-construction (items=100) | tuix-python | 18.6MB | 18.5MB | 18.6MB | 16.0KB |
| tree-construction (items=100) | rich | 31.6MB | 31.6MB | 31.6MB | 16.0KB |
| tree-construction (items=100) | opentui-core | 34.6MB | 33.9MB | 34.6MB | 668.0KB |
| tree-construction (items=100) | prompt-toolkit | 35.8MB | 35.8MB | 35.8MB | 16.0KB |
| tree-construction (items=100) | opentui-react | 36.1MB | 36.0MB | 36.1MB | 104.0KB |
| tree-construction (items=100) | urwid | 42.6MB | 42.5MB | 42.6MB | 20.0KB |
| tree-construction (items=100) | terminal-kit | 46.7MB | 46.7MB | 46.7MB | 28.0KB |
| tree-construction (items=100) | ink | 105.8MB | 84.6MB | 105.8MB | 21.2MB |
| tree-construction (items=100) | rezitui | 110.4MB | 97.8MB | 110.4MB | 12.6MB |
| tree-construction (items=100) | blessed | 240.4MB | 165.2MB | 240.4MB | 75.2MB |
| tree-construction (items=500) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=500) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 48.0KB |
| tree-construction (items=500) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 48.0KB |
| tree-construction (items=500) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 8.0KB |
| tree-construction (items=500) | tuix-python | 18.6MB | 18.6MB | 18.6MB | 16.0KB |
| tree-construction (items=500) | rich | 31.6MB | 31.6MB | 31.6MB | 16.0KB |
| tree-construction (items=500) | opentui-core | 34.9MB | 34.7MB | 34.9MB | 196.0KB |
| tree-construction (items=500) | prompt-toolkit | 35.8MB | 35.8MB | 35.8MB | 16.0KB |
| tree-construction (items=500) | opentui-react | 37.6MB | 36.1MB | 37.6MB | 1.5MB |
| tree-construction (items=500) | urwid | 42.6MB | 42.6MB | 42.6MB | 32.0KB |
| tree-construction (items=500) | terminal-kit | 46.6MB | 46.7MB | 46.6MB | 0.00KB |
| tree-construction (items=500) | rezitui | 161.8MB | 113.1MB | 161.8MB | 48.7MB |
| tree-construction (items=500) | ink | 182.9MB | 106.5MB | 182.9MB | 76.4MB |
| tree-construction (items=500) | blessed | 454.1MB | 261.7MB | 454.1MB | 192.3MB |
| tree-construction (items=1000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=1000) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 48.0KB |
| tree-construction (items=1000) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 48.0KB |
| tree-construction (items=1000) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| tree-construction (items=1000) | tuix-python | 18.6MB | 18.6MB | 18.6MB | 16.0KB |
| tree-construction (items=1000) | rich | 31.6MB | 31.6MB | 31.6MB | 16.0KB |
| tree-construction (items=1000) | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| tree-construction (items=1000) | prompt-toolkit | 35.8MB | 35.8MB | 35.8MB | 24.0KB |
| tree-construction (items=1000) | opentui-react | 38.1MB | 37.6MB | 38.1MB | 540.0KB |
| tree-construction (items=1000) | urwid | 42.6MB | 42.6MB | 42.6MB | 20.0KB |
| tree-construction (items=1000) | terminal-kit | 54.9MB | 46.6MB | 54.9MB | 8.2MB |
| tree-construction (items=1000) | rezitui | 187.0MB | 161.6MB | 187.0MB | 25.4MB |
| tree-construction (items=1000) | ink | 364.8MB | 184.0MB | 364.8MB | 180.7MB |
| tree-construction (items=1000) | blessed | 526.6MB | 454.3MB | 526.6MB | 72.3MB |
| rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| rerender | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| rerender | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| rerender | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| rerender | tuix-python | 18.6MB | 18.6MB | 18.6MB | 32.0KB |
| rerender | rich | 31.7MB | 31.6MB | 31.7MB | 32.0KB |
| rerender | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| rerender | prompt-toolkit | 36.1MB | 35.8MB | 36.1MB | 264.0KB |
| rerender | opentui-react | 38.1MB | 38.1MB | 38.1MB | 0.00KB |
| rerender | urwid | 42.7MB | 42.6MB | 42.7MB | 72.0KB |
| rerender | terminal-kit | 54.8MB | 54.9MB | 54.8MB | 0.00KB |
| rerender | rezitui | 193.7MB | 187.0MB | 193.7MB | 6.7MB |
| rerender | ink | 364.8MB | 364.9MB | 364.8MB | 0.00KB |
| rerender | blessed | 526.0MB | 525.8MB | 526.0MB | 220.0KB |
| content-update | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| content-update | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| content-update | TUIX.Renderer | 4.2MB | 3.9MB | 4.2MB | 296.0KB |
| content-update | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| content-update | tuix-python | 18.7MB | 18.6MB | 18.7MB | 16.0KB |
| content-update | rich | 32.2MB | 31.8MB | 32.2MB | 412.0KB |
| content-update | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| content-update | prompt-toolkit | 36.4MB | 36.1MB | 36.4MB | 364.0KB |
| content-update | opentui-react | 38.9MB | 38.1MB | 38.9MB | 824.0KB |
| content-update | urwid | 42.7MB | 42.7MB | 42.7MB | 0.00KB |
| content-update | terminal-kit | 54.9MB | 54.9MB | 54.9MB | 80.0KB |
| content-update | rezitui | 189.0MB | 193.8MB | 189.0MB | 0.00KB |
| content-update | ink | 365.3MB | 364.8MB | 365.3MB | 476.0KB |
| content-update | blessed | 526.5MB | 529.2MB | 526.5MB | 0.00KB |
| layout-stress (cols=4,rows=10) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 44.0KB |
| layout-stress (cols=4,rows=10) | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| layout-stress (cols=4,rows=10) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| layout-stress (cols=4,rows=10) | tuix-python | 18.7MB | 18.7MB | 18.7MB | 8.0KB |
| layout-stress (cols=4,rows=10) | rich | 32.2MB | 32.2MB | 32.2MB | 8.0KB |
| layout-stress (cols=4,rows=10) | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| layout-stress (cols=4,rows=10) | prompt-toolkit | 36.4MB | 36.4MB | 36.4MB | 0.00KB |
| layout-stress (cols=4,rows=10) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| layout-stress (cols=4,rows=10) | urwid | 42.7MB | 42.7MB | 42.7MB | 0.00KB |
| layout-stress (cols=4,rows=10) | terminal-kit | 55.1MB | 55.1MB | 55.1MB | 0.00KB |
| layout-stress (cols=4,rows=10) | rezitui | 189.6MB | 189.0MB | 189.6MB | 660.0KB |
| layout-stress (cols=4,rows=10) | ink | 365.3MB | 365.1MB | 365.3MB | 280.0KB |
| layout-stress (cols=4,rows=10) | blessed | 526.9MB | 531.0MB | 526.9MB | 0.00KB |
| scroll-stress (items=2000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| scroll-stress (items=2000) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| scroll-stress (items=2000) | tuix-python | 18.7MB | 18.7MB | 18.7MB | 8.0KB |
| scroll-stress (items=2000) | rich | 32.3MB | 32.2MB | 32.3MB | 80.0KB |
| scroll-stress (items=2000) | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| scroll-stress (items=2000) | prompt-toolkit | 36.4MB | 36.4MB | 36.4MB | 0.00KB |
| scroll-stress (items=2000) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| scroll-stress (items=2000) | urwid | 42.7MB | 42.7MB | 42.7MB | 0.00KB |
| scroll-stress (items=2000) | terminal-kit | 55.2MB | 55.2MB | 55.2MB | 0.00KB |
| scroll-stress (items=2000) | rezitui | 189.7MB | 189.6MB | 189.7MB | 108.0KB |
| scroll-stress (items=2000) | ink | 365.3MB | 365.3MB | 365.3MB | 4.0KB |
| scroll-stress (items=2000) | blessed | 527.2MB | 526.8MB | 527.2MB | 428.0KB |
| virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | tuix-python | 18.8MB | 18.7MB | 18.8MB | 92.0KB |
| virtual-list (items=100000,viewport=40) | bubbletea | 20.0MB | 16.0MB | 20.0MB | 4.0MB |
| virtual-list (items=100000,viewport=40) | rich | 32.3MB | 32.3MB | 32.3MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | prompt-toolkit | 36.7MB | 36.4MB | 36.7MB | 232.0KB |
| virtual-list (items=100000,viewport=40) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | urwid | 42.7MB | 42.7MB | 42.7MB | 64.0KB |
| virtual-list (items=100000,viewport=40) | terminal-kit | 97.5MB | 90.6MB | 97.5MB | 6.9MB |
| virtual-list (items=100000,viewport=40) | rezitui | 192.8MB | 190.2MB | 192.8MB | 2.6MB |
| virtual-list (items=100000,viewport=40) | ink | 365.7MB | 367.1MB | 365.7MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | blessed | 529.1MB | 529.4MB | 529.1MB | 0.00KB |
| tables (cols=8,rows=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| tables (cols=8,rows=100) | tuix-python | 18.8MB | 18.8MB | 18.8MB | 0.00KB |
| tables (cols=8,rows=100) | bubbletea | 20.0MB | 20.0MB | 20.0MB | 0.00KB |
| tables (cols=8,rows=100) | rich | 32.3MB | 32.3MB | 32.3MB | 0.00KB |
| tables (cols=8,rows=100) | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| tables (cols=8,rows=100) | prompt-toolkit | 36.7MB | 36.7MB | 36.7MB | 48.0KB |
| tables (cols=8,rows=100) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| tables (cols=8,rows=100) | urwid | 42.7MB | 42.7MB | 42.7MB | 0.00KB |
| tables (cols=8,rows=100) | terminal-kit | 97.3MB | 97.3MB | 97.3MB | 0.00KB |
| tables (cols=8,rows=100) | rezitui | 191.0MB | 190.7MB | 191.0MB | 276.0KB |
| tables (cols=8,rows=100) | ink | 366.2MB | 366.2MB | 366.2MB | 0.00KB |
| tables (cols=8,rows=100) | blessed | 528.5MB | 528.5MB | 528.5MB | 24.0KB |
| memory-profile | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| memory-profile | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| memory-profile | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| memory-profile | tuix-python | 19.0MB | 18.9MB | 19.0MB | 64.0KB |
| memory-profile | bubbletea | 20.3MB | 20.0MB | 20.3MB | 256.0KB |
| memory-profile | rich | 32.6MB | 32.3MB | 32.6MB | 300.0KB |
| memory-profile | opentui-core | 35.0MB | 35.0MB | 35.0MB | 0.00KB |
| memory-profile | prompt-toolkit | 36.9MB | 36.7MB | 36.9MB | 180.0KB |
| memory-profile | opentui-react | 39.3MB | 39.0MB | 39.3MB | 300.0KB |
| memory-profile | urwid | 42.8MB | 42.7MB | 42.8MB | 64.0KB |
| memory-profile | terminal-kit | 97.3MB | 97.3MB | 97.3MB | 16.0KB |
| memory-profile | rezitui | 190.3MB | 191.2MB | 190.3MB | 0.00KB |
| memory-profile | ink | 366.2MB | 366.2MB | 366.2MB | 12.0KB |
| memory-profile | blessed | 528.2MB | 528.6MB | 528.2MB | 0.00KB |
| terminal-rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-rerender | TUIX.Renderer | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-rerender | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-rerender | tuix-python | 19.0MB | 19.0MB | 19.0MB | 0.00KB |
| terminal-rerender | bubbletea | 20.3MB | 20.3MB | 20.3MB | 0.00KB |
| terminal-rerender | rich | 32.6MB | 32.6MB | 32.6MB | 16.0KB |
| terminal-rerender | opentui-core | 35.5MB | 35.5MB | 35.5MB | 0.00KB |
| terminal-rerender | prompt-toolkit | 37.1MB | 37.1MB | 37.1MB | 8.0KB |
| terminal-rerender | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-rerender | urwid | 42.9MB | 42.8MB | 42.9MB | 64.0KB |
| terminal-rerender | terminal-kit | 97.5MB | 97.4MB | 97.5MB | 64.0KB |
| terminal-rerender | rezitui | 190.3MB | 190.4MB | 190.3MB | 0.00KB |
| terminal-rerender | ink | 365.9MB | 366.2MB | 365.9MB | 0.00KB |
| terminal-rerender | blessed | 528.5MB | 528.5MB | 528.5MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | TUIX.Renderer | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | tuix-python | 19.0MB | 19.0MB | 19.0MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | bubbletea | 20.3MB | 20.3MB | 20.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | rich | 32.7MB | 32.6MB | 32.7MB | 112.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-core | 36.2MB | 36.1MB | 36.2MB | 84.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | prompt-toolkit | 37.1MB | 37.1MB | 37.1MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | urwid | 42.9MB | 42.9MB | 42.9MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | terminal-kit | 97.3MB | 97.5MB | 97.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | rezitui | 190.8MB | 190.3MB | 190.8MB | 504.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ink | 366.2MB | 365.9MB | 366.2MB | 308.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | blessed | 528.6MB | 528.6MB | 528.6MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | TUIX.Core | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | TUIX.Renderer | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | tuix-python | 19.0MB | 19.0MB | 19.0MB | 12.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | bubbletea | 20.3MB | 20.3MB | 20.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | rich | 32.9MB | 32.8MB | 32.9MB | 88.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-core | 36.3MB | 36.2MB | 36.3MB | 152.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | prompt-toolkit | 37.1MB | 37.1MB | 37.1MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | urwid | 42.9MB | 42.9MB | 42.9MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | terminal-kit | 97.8MB | 97.8MB | 97.8MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | rezitui | 196.6MB | 190.8MB | 196.6MB | 5.8MB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ink | 366.2MB | 366.2MB | 366.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | blessed | 528.9MB | 528.7MB | 528.9MB | 284.0KB |
| terminal-screen-transition (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | TUIX.Core | 4.5MB | 4.6MB | 4.5MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | tuix-python | 19.5MB | 19.5MB | 19.5MB | 32.0KB |
| terminal-screen-transition (cols=120,rows=40) | bubbletea | 24.5MB | 20.3MB | 24.5MB | 4.3MB |
| terminal-screen-transition (cols=120,rows=40) | rich | 32.9MB | 32.9MB | 32.9MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | prompt-toolkit | 37.1MB | 37.1MB | 37.1MB | 40.0KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | urwid | 42.9MB | 42.9MB | 42.9MB | 8.0KB |
| terminal-screen-transition (cols=120,rows=40) | terminal-kit | 97.7MB | 97.7MB | 97.7MB | 4.0KB |
| terminal-screen-transition (cols=120,rows=40) | rezitui | 203.2MB | 197.5MB | 203.2MB | 5.7MB |
| terminal-screen-transition (cols=120,rows=40) | ink | 366.3MB | 366.3MB | 366.3MB | 16.0KB |
| terminal-screen-transition (cols=120,rows=40) | blessed | 528.9MB | 528.9MB | 528.9MB | 4.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | tuix-python | 19.2MB | 19.2MB | 19.2MB | 36.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | bubbletea | 24.5MB | 24.5MB | 24.5MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | rich | 32.9MB | 32.9MB | 32.9MB | 4.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | prompt-toolkit | 37.3MB | 37.1MB | 37.3MB | 116.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | urwid | 43.0MB | 42.9MB | 43.0MB | 68.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | terminal-kit | 97.6MB | 97.5MB | 97.6MB | 8.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | rezitui | 202.7MB | 203.9MB | 202.7MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ink | 366.4MB | 366.3MB | 366.4MB | 20.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | blessed | 529.0MB | 528.9MB | 529.0MB | 12.0KB |
| terminal-input-latency (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | tuix-python | 19.3MB | 19.2MB | 19.3MB | 32.0KB |
| terminal-input-latency (cols=120,rows=40) | bubbletea | 24.5MB | 24.5MB | 24.5MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | rich | 33.4MB | 32.9MB | 33.4MB | 472.0KB |
| terminal-input-latency (cols=120,rows=40) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | prompt-toolkit | 37.3MB | 37.3MB | 37.3MB | 84.0KB |
| terminal-input-latency (cols=120,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | urwid | 43.0MB | 43.0MB | 43.0MB | 44.0KB |
| terminal-input-latency (cols=120,rows=40) | terminal-kit | 97.6MB | 97.6MB | 97.6MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | rezitui | 204.1MB | 202.7MB | 204.1MB | 1.5MB |
| terminal-input-latency (cols=120,rows=40) | ink | 365.9MB | 365.9MB | 365.9MB | 4.0KB |
| terminal-input-latency (cols=120,rows=40) | blessed | 529.0MB | 529.0MB | 529.0MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | tuix-python | 19.3MB | 19.3MB | 19.3MB | 40.0KB |
| terminal-memory-soak (cols=120,rows=40) | bubbletea | 24.5MB | 24.5MB | 24.5MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | rich | 33.5MB | 33.6MB | 33.5MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | prompt-toolkit | 37.2MB | 37.3MB | 37.2MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | urwid | 43.0MB | 43.0MB | 43.0MB | 28.0KB |
| terminal-memory-soak (cols=120,rows=40) | terminal-kit | 97.7MB | 97.7MB | 97.7MB | 68.0KB |
| terminal-memory-soak (cols=120,rows=40) | rezitui | 212.1MB | 204.1MB | 212.1MB | 8.0MB |
| terminal-memory-soak (cols=120,rows=40) | ink | 365.9MB | 365.9MB | 365.9MB | 28.0KB |
| terminal-memory-soak (cols=120,rows=40) | blessed | 529.0MB | 529.0MB | 529.0MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | tuix-python | 19.4MB | 19.3MB | 19.4MB | 88.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | bubbletea | 24.5MB | 24.5MB | 24.5MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | rich | 33.7MB | 33.5MB | 33.7MB | 156.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | prompt-toolkit | 37.4MB | 37.2MB | 37.4MB | 232.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | urwid | 43.1MB | 43.0MB | 43.1MB | 60.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | terminal-kit | 98.1MB | 98.1MB | 98.1MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | rezitui | 214.8MB | 213.1MB | 214.8MB | 1.7MB |
| terminal-full-ui (cols=120,rows=40,services=24) | ink | 366.7MB | 366.5MB | 366.7MB | 184.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | blessed | 529.0MB | 529.0MB | 529.0MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | tuix-python | 19.4MB | 19.4MB | 19.4MB | 4.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 24.5MB | 24.5MB | 24.5MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rich | 33.7MB | 33.7MB | 33.7MB | 24.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | prompt-toolkit | 37.5MB | 37.4MB | 37.5MB | 120.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | urwid | 43.1MB | 43.1MB | 43.1MB | 40.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 98.2MB | 98.2MB | 98.2MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 214.7MB | 214.8MB | 214.7MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 366.7MB | 366.7MB | 366.7MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 529.0MB | 529.0MB | 529.0MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | tuix-python | 19.5MB | 19.4MB | 19.5MB | 64.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | bubbletea | 24.5MB | 24.5MB | 24.5MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | rich | 33.7MB | 33.7MB | 33.7MB | 40.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | prompt-toolkit | 37.6MB | 37.5MB | 37.6MB | 32.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | urwid | 43.2MB | 43.1MB | 43.2MB | 52.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | terminal-kit | 98.2MB | 98.2MB | 98.2MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | rezitui | 214.8MB | 214.7MB | 214.8MB | 28.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ink | 366.7MB | 366.7MB | 366.7MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | blessed | 529.0MB | 529.0MB | 529.0MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | tuix-python | 19.5MB | 19.5MB | 19.5MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 24.5MB | 24.5MB | 24.5MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rich | 33.8MB | 33.7MB | 33.8MB | 36.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | prompt-toolkit | 37.5MB | 37.6MB | 37.5MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | urwid | 43.3MB | 43.2MB | 43.3MB | 80.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 98.2MB | 98.2MB | 98.2MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 214.8MB | 214.8MB | 214.8MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 366.7MB | 366.7MB | 366.7MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 529.0MB | 529.0MB | 529.0MB | 4.0KB |
| terminal-virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | tuix-python | 19.6MB | 19.5MB | 19.6MB | 64.0KB |
| terminal-virtual-list (items=100000,viewport=40) | bubbletea | 24.5MB | 24.5MB | 24.5MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | rich | 33.8MB | 33.8MB | 33.8MB | 48.0KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | prompt-toolkit | 37.5MB | 37.5MB | 37.5MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | urwid | 43.3MB | 43.3MB | 43.3MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | terminal-kit | 129.8MB | 129.4MB | 129.8MB | 332.0KB |
| terminal-virtual-list (items=100000,viewport=40) | rezitui | 214.8MB | 214.8MB | 214.8MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | ink | 368.2MB | 368.2MB | 368.2MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | blessed | 529.9MB | 531.3MB | 529.9MB | 0.00KB |
| terminal-table (cols=8,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Renderer | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-table (cols=8,rows=40) | tuix-python | 19.6MB | 19.6MB | 19.6MB | 0.00KB |
| terminal-table (cols=8,rows=40) | bubbletea | 24.5MB | 24.5MB | 24.5MB | 0.00KB |
| terminal-table (cols=8,rows=40) | rich | 33.8MB | 33.8MB | 33.8MB | 0.00KB |
| terminal-table (cols=8,rows=40) | opentui-core | 36.3MB | 36.3MB | 36.3MB | 0.00KB |
| terminal-table (cols=8,rows=40) | prompt-toolkit | 37.6MB | 37.5MB | 37.6MB | 28.0KB |
| terminal-table (cols=8,rows=40) | opentui-react | 39.3MB | 39.3MB | 39.3MB | 0.00KB |
| terminal-table (cols=8,rows=40) | urwid | 43.3MB | 43.3MB | 43.3MB | 0.00KB |
| terminal-table (cols=8,rows=40) | terminal-kit | 129.7MB | 129.7MB | 129.7MB | 0.00KB |
| terminal-table (cols=8,rows=40) | rezitui | 214.1MB | 211.4MB | 214.1MB | 2.7MB |
| terminal-table (cols=8,rows=40) | ink | 367.9MB | 367.9MB | 367.9MB | 0.00KB |
| terminal-table (cols=8,rows=40) | blessed | 529.4MB | 529.4MB | 529.4MB | 0.00KB |

