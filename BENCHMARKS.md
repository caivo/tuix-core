# Benchmark Results

> 2026-04-03T17:55:20.531Z | Node v22.19.0 | Windows_NT 10.0.26200 | win32 x64 | 12th Gen Intel(R) Core(TM) i5-12450HX (12 cores) | RAM 24352MB
> Libraries tested: blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React, TUIX.Renderer, TUIX.Core, TUIX.Python, Rich, Urwid, PromptToolkit
> Byte columns: "Bytes(local)" = bytes produced per frame (framework counter); "Bytes(pty)" = observed PTY bytes (cross-framework comparable).

## startup

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 176µs | 27.4% | 167µs–185µs | 5.7K ops/s | 17.61ms | 4.0MB | 4.0KB | 6.0MB | 61.7KB |
| tuix-python | 1 | 177µs | 5.1% | 175µs–178µs | 5.7K ops/s | 17.71ms | 18.2MB | 4.0KB | 6.7MB | 68.3KB |
| TUIX.Renderer | 1 | 195µs | 32.3% | 183µs–207µs | 5.1K ops/s | 19.51ms | 3.9MB | 4.0KB | 6.3MB | 64.4KB |
| rezitui | 1 | 201µs | 180.2% | 130µs–272µs | 5.0K ops/s | 20.26ms | 78.4MB | 13.4MB | 133.7KB | 1.3KB |
| ratatui | 1 | 258µs | 15.5% | 250µs–266µs | 3.9K ops/s | 28.36ms | 27.0KB | 0.00KB | 1.6MB | 16.0KB |
| bubbletea | 1 | 333µs | 75.5% | 284µs–383µs | 3.0K ops/s | 33.40ms | 16.0MB | 5.5MB | 1.6MB | 16.0KB |
| terminal-kit | 1 | 478µs | 10.8% | 468µs–489µs | 2.1K ops/s | 48.00ms | 43.4MB | 28.0KB | 190.0KB | 1.9KB |
| opentui-react | 1 | 589µs | 63.7% | 516µs–663µs | 1.7K ops/s | 59.03ms | 35.9MB | 412.0KB | 7.4MB | 76.0KB |
| opentui-core | 1 | 635µs | 46.5% | 577µs–693µs | 1.6K ops/s | 63.58ms | 33.3MB | 412.0KB | 6.7MB | 68.5KB |
| prompt-toolkit | 1 | 1.02ms | 20.9% | 980µs–1.06ms | 979 ops/s | 102.17ms | 36.2MB | 24.0KB | 189.7KB | 1.9KB |
| urwid | 1 | 1.42ms | 12.7% | 1.39ms–1.46ms | 703 ops/s | 142.38ms | 42.0MB | 24.0KB | 187.5KB | 1.9KB |
| rich | 1 | 1.43ms | 15.5% | 1.38ms–1.47ms | 701 ops/s | 142.67ms | 31.5MB | 492.0KB | 189.7KB | 1.9KB |
| ink | 1 | 3.74ms | 28.8% | 3.53ms–3.95ms | 268 ops/s | 373.88ms | 62.3MB | 2.7MB | 189.7KB | 1.9KB |
| blessed | 1 | 31.80ms | 26.2% | 30.16ms–33.43ms | 31 ops/s | 3.18s | 149.5MB | 74.4MB | 229.2KB | 2.3KB |

## tree-construction (items=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 35µs | 54.5% | 34µs–37µs | 28.3K ops/s | 17.68ms | 4.0MB | 0.00KB | 555.7KB | 1.1KB |
| TUIX.Renderer | 1 | 47µs | 30.6% | 45µs–48µs | 21.5K ops/s | 23.30ms | 3.9MB | 0.00KB | 1.1MB | 2.3KB |
| opentui-react | 1 | 58µs | 55.7% | 55µs–61µs | 17.3K ops/s | 29.88ms | 36.0MB | 44.0KB | 3.6MB | 7.5KB |
| ratatui | 1 | 65µs | 32.6% | 64µs–67µs | 15.3K ops/s | 32.77ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| tuix-python | 1 | 77µs | 16.6% | 75µs–78µs | 13.1K ops/s | 38.52ms | 18.2MB | 16.0KB | 7.7MB | 15.7KB |
| blessed | 1 | 101µs | 272.3% | 77µs–126µs | 9.9K ops/s | 50.96ms | 157.9MB | 6.4MB | 126.0KB | 258B |
| rezitui | 1 | 115µs | 192.7% | 96µs–135µs | 8.7K ops/s | 57.88ms | 98.1MB | 18.7MB | 668.5KB | 1.3KB |
| bubbletea | 1 | 220µs | 72.5% | 206µs–234µs | 4.5K ops/s | 110.08ms | 16.6MB | 296.0KB | 7.8MB | 16.0KB |
| terminal-kit | 1 | 290µs | 21.0% | 285µs–296µs | 3.4K ops/s | 145.46ms | 43.7MB | 36.0KB | 950.2KB | 1.9KB |
| opentui-core | 1 | 325µs | 56.0% | 309µs–341µs | 3.1K ops/s | 166.69ms | 33.3MB | 0.00KB | 33.4MB | 68.3KB |
| ink | 1 | 707µs | 61.0% | 670µs–745µs | 1.4K ops/s | 354.41ms | 66.1MB | 2.7MB | 23.9KB | 49B |
| prompt-toolkit | 1 | 921µs | 32.9% | 895µs–948µs | 1.1K ops/s | 460.93ms | 36.2MB | 32.0KB | 948.7KB | 1.9KB |
| rich | 1 | 971µs | 34.1% | 942µs–1.00ms | 1.0K ops/s | 486.08ms | 31.5MB | 24.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.61ms | 34.7% | 1.56ms–1.66ms | 622 ops/s | 803.96ms | 42.1MB | 76.0KB | 937.5KB | 1.9KB |

## tree-construction (items=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 49µs | 12.7% | 49µs–50µs | 20.2K ops/s | 24.73ms | 4.0MB | 0.00KB | 1.2MB | 2.5KB |
| TUIX.Renderer | 1 | 68µs | 17.8% | 67µs–70µs | 14.6K ops/s | 34.26ms | 3.9MB | 0.00KB | 1.3MB | 2.7KB |
| opentui-react | 1 | 105µs | 18.3% | 103µs–107µs | 9.5K ops/s | 57.54ms | 36.1MB | 104.0KB | 8.9MB | 18.3KB |
| rezitui | 1 | 122µs | 258.0% | 94µs–150µs | 8.2K ops/s | 61.29ms | 107.5MB | 11.3MB | 668.5KB | 1.3KB |
| tuix-python | 1 | 125µs | 20.0% | 123µs–127µs | 8.0K ops/s | 62.62ms | 18.3MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-core | 1 | 136µs | 35.8% | 132µs–141µs | 7.3K ops/s | 101.14ms | 34.0MB | 736.0KB | 33.8MB | 69.2KB |
| ratatui | 1 | 173µs | 24.9% | 169µs–177µs | 5.8K ops/s | 86.63ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| bubbletea | 1 | 239µs | 52.8% | 228µs–250µs | 4.2K ops/s | 119.64ms | 16.6MB | 16.0KB | 7.8MB | 16.0KB |
| blessed | 1 | 484µs | 126.2% | 430µs–537µs | 2.1K ops/s | 242.13ms | 240.9MB | 79.6MB | 490.2KB | 1004B |
| rich | 1 | 879µs | 32.7% | 854µs–904µs | 1.1K ops/s | 439.96ms | 31.6MB | 28.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 894µs | 27.8% | 872µs–916µs | 1.1K ops/s | 447.30ms | 36.3MB | 72.0KB | 948.7KB | 1.9KB |
| terminal-kit | 1 | 1.27ms | 29.1% | 1.24ms–1.30ms | 786 ops/s | 636.64ms | 43.7MB | 80.0KB | 950.2KB | 1.9KB |
| urwid | 1 | 1.71ms | 31.1% | 1.66ms–1.75ms | 586 ops/s | 853.34ms | 42.1MB | 16.0KB | 937.5KB | 1.9KB |
| ink | 1 | 7.43ms | 37.7% | 7.19ms–7.68ms | 135 ops/s | 3.72s | 101.3MB | 19.2MB | 287.6KB | 589B |

## tree-construction (items=500)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 46µs | 34.4% | 44µs–47µs | 22.0K ops/s | 22.77ms | 3.9MB | 48.0KB | 1.3MB | 2.7KB |
| TUIX.Core | 1 | 48µs | 9.2% | 48µs–49µs | 20.6K ops/s | 24.25ms | 4.0MB | 0.00KB | 1.2MB | 2.5KB |
| tuix-python | 1 | 128µs | 21.1% | 126µs–130µs | 7.8K ops/s | 64.23ms | 18.3MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-core | 1 | 159µs | 195.5% | 132µs–186µs | 6.3K ops/s | 250.29ms | 34.3MB | 244.0KB | 33.7MB | 68.9KB |
| opentui-react | 1 | 217µs | 29.1% | 211µs–222µs | 4.6K ops/s | 132.23ms | 36.3MB | 208.0KB | 8.9MB | 18.2KB |
| rezitui | 1 | 318µs | 276.3% | 241µs–395µs | 3.1K ops/s | 159.20ms | 166.2MB | 55.6MB | 668.5KB | 1.3KB |
| ratatui | 1 | 600µs | 26.1% | 586µs–614µs | 1.7K ops/s | 300.08ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| bubbletea | 1 | 670µs | 30.6% | 652µs–688µs | 1.5K ops/s | 335.11ms | 16.6MB | 0.00KB | 7.8MB | 16.0KB |
| prompt-toolkit | 1 | 859µs | 18.8% | 844µs–873µs | 1.2K ops/s | 429.48ms | 36.3MB | 32.0KB | 948.7KB | 1.9KB |
| rich | 1 | 1.07ms | 35.3% | 1.04ms–1.10ms | 934 ops/s | 535.86ms | 31.6MB | 12.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.46ms | 28.4% | 1.43ms–1.50ms | 683 ops/s | 732.39ms | 42.1MB | 32.0KB | 937.5KB | 1.9KB |
| blessed | 1 | 1.93ms | 57.4% | 1.83ms–2.03ms | 518 ops/s | 966.67ms | 450.9MB | 193.3MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 9.01ms | 48.8% | 8.62ms–9.39ms | 111 ops/s | 4.51s | 43.6MB | 0.00KB | 950.2KB | 1.9KB |
| ink | 1 | 44.24ms | 17.7% | 43.55ms–44.93ms | 23 ops/s | 22.12s | 194.4MB | 90.7MB | 1.6MB | 3.3KB |

## tree-construction (items=1000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 49µs | 11.6% | 49µs–50µs | 20.2K ops/s | 24.73ms | 4.0MB | 0.00KB | 1.2MB | 2.5KB |
| TUIX.Renderer | 1 | 59µs | 19.5% | 58µs–60µs | 17.0K ops/s | 29.46ms | 3.9MB | 0.00KB | 1.3MB | 2.7KB |
| opentui-core | 1 | 103µs | 48.5% | 99µs–108µs | 9.7K ops/s | 408.58ms | 34.8MB | 236.0KB | 33.8MB | 69.3KB |
| tuix-python | 1 | 140µs | 29.8% | 137µs–144µs | 7.1K ops/s | 70.47ms | 18.4MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-react | 1 | 278µs | 154.4% | 241µs–316µs | 3.6K ops/s | 207.10ms | 38.0MB | 1.5MB | 8.9MB | 18.2KB |
| rezitui | 1 | 449µs | 219.4% | 363µs–536µs | 2.2K ops/s | 224.71ms | 184.0MB | 17.6MB | 668.5KB | 1.3KB |
| rich | 1 | 894µs | 31.5% | 869µs–918µs | 1.1K ops/s | 447.13ms | 31.6MB | 12.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 984µs | 29.0% | 959µs–1.01ms | 1.0K ops/s | 492.18ms | 36.4MB | 116.0KB | 948.7KB | 1.9KB |
| ratatui | 1 | 1.18ms | 31.5% | 1.15ms–1.21ms | 847 ops/s | 590.10ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| bubbletea | 1 | 1.23ms | 20.7% | 1.21ms–1.25ms | 811 ops/s | 616.41ms | 16.6MB | 0.00KB | 7.8MB | 16.0KB |
| urwid | 1 | 1.64ms | 29.6% | 1.59ms–1.68ms | 612 ops/s | 818.05ms | 42.1MB | 16.0KB | 937.5KB | 1.9KB |
| blessed | 1 | 4.02ms | 43.9% | 3.87ms–4.18ms | 249 ops/s | 2.01s | 516.7MB | 65.0MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 24.72ms | 13.6% | 24.43ms–25.02ms | 40 ops/s | 12.36s | 51.8MB | 8.1MB | 950.2KB | 1.9KB |
| ink | 1 | 79.91ms | 19.2% | 78.57ms–81.26ms | 13 ops/s | 39.96s | 347.0MB | 140.4MB | 3.3MB | 6.7KB |

## rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| tuix-python | 1 | 11µs | 16.3% | 11µs–11µs | 88.6K ops/s | 11.81ms | 18.4MB | 32.0KB | 1.8MB | 1.9KB |
| TUIX.Renderer | 1 | 11µs | 158.3% | 10µs–12µs | 88.0K ops/s | 11.40ms | 3.9MB | 0.00KB | 15.6KB | 16B |
| TUIX.Core | 1 | 13µs | 49.0% | 13µs–14µs | 75.6K ops/s | 13.25ms | 4.1MB | 0.00KB | 15.6KB | 16B |
| blessed | 1 | 30µs | 272.5% | 25µs–35µs | 33.6K ops/s | 29.90ms | 516.4MB | 0.00KB | 0.00KB | 0.00KB |
| opentui-react | 1 | 42µs | 46.3% | 40µs–43µs | 24.1K ops/s | 41.67ms | 38.0MB | 0.00KB | 3.9KB | 4B |
| rezitui | 1 | 51µs | 333.7% | 41µs–62µs | 19.6K ops/s | 51.28ms | 191.6MB | 7.6MB | 1.3MB | 1.3KB |
| ratatui | 1 | 66µs | 44.3% | 65µs–68µs | 15.1K ops/s | 66.48ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| opentui-core | 1 | 111µs | 51.8% | 107µs–114µs | 9.0K ops/s | 110.92ms | 34.8MB | 0.00KB | 66.9MB | 68.5KB |
| bubbletea | 1 | 121µs | 50.6% | 117µs–124µs | 8.3K ops/s | 120.87ms | 16.6MB | 0.00KB | 15.6MB | 16.0KB |
| terminal-kit | 1 | 455µs | 26.5% | 447µs–462µs | 2.2K ops/s | 455.22ms | 52.0MB | 112.0KB | 1.9MB | 1.9KB |
| ink | 1 | 1.03ms | 36.3% | 1.01ms–1.06ms | 966 ops/s | 1.04s | 346.5MB | 0.00KB | 1.9MB | 1.9KB |
| prompt-toolkit | 1 | 1.31ms | 36.4% | 1.28ms–1.34ms | 764 ops/s | 1.31s | 36.6MB | 160.0KB | 1.9MB | 1.9KB |
| rich | 1 | 1.54ms | 34.0% | 1.51ms–1.57ms | 650 ops/s | 1.54s | 31.6MB | 32.0KB | 1.9MB | 1.9KB |
| urwid | 1 | 1.89ms | 30.6% | 1.86ms–1.93ms | 529 ops/s | 1.89s | 42.2MB | 68.0KB | 1.8MB | 1.9KB |

## content-update

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 13µs | 7.6% | 13µs–13µs | 78.4K ops/s | 6.39ms | 4.0MB | 0.00KB | 6.5MB | 13.4KB |
| rezitui | 1 | 68µs | 429.6% | 42µs–94µs | 14.7K ops/s | 34.04ms | 184.9MB | 0.00KB | 668.5KB | 1.3KB |
| tuix-python | 1 | 79µs | 29.4% | 77µs–81µs | 12.6K ops/s | 45.59ms | 18.5MB | 72.0KB | 17.1MB | 35.0KB |
| TUIX.Renderer | 1 | 81µs | 35.3% | 79µs–84µs | 12.3K ops/s | 46.12ms | 3.9MB | 0.00KB | 5.8MB | 11.9KB |
| opentui-react | 1 | 92µs | 42.7% | 88µs–95µs | 10.9K ops/s | 49.40ms | 38.9MB | 880.0KB | 15.3MB | 31.3KB |
| ratatui | 1 | 104µs | 39.7% | 101µs–108µs | 9.6K ops/s | 52.23ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| opentui-core | 1 | 147µs | 51.6% | 140µs–153µs | 6.8K ops/s | 77.27ms | 34.8MB | 0.00KB | 33.4MB | 68.5KB |
| bubbletea | 1 | 247µs | 36.8% | 239µs–255µs | 4.1K ops/s | 123.41ms | 16.6MB | 8.0KB | 7.8MB | 16.0KB |
| blessed | 1 | 325µs | 58.7% | 308µs–341µs | 3.1K ops/s | 162.36ms | 516.9MB | 0.00KB | 1.1MB | 2.3KB |
| terminal-kit | 1 | 617µs | 34.9% | 598µs–636µs | 1.6K ops/s | 308.84ms | 52.1MB | 56.0KB | 950.2KB | 1.9KB |
| prompt-toolkit | 1 | 1.08ms | 28.9% | 1.05ms–1.11ms | 925 ops/s | 541.12ms | 36.6MB | 72.0KB | 948.7KB | 1.9KB |
| rich | 1 | 1.38ms | 27.8% | 1.34ms–1.41ms | 726 ops/s | 688.72ms | 32.1MB | 420.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.85ms | 28.7% | 1.80ms–1.89ms | 541 ops/s | 924.54ms | 42.2MB | 0.00KB | 937.5KB | 1.9KB |
| ink | 1 | 5.86ms | 22.8% | 5.74ms–5.98ms | 171 ops/s | 2.93s | 346.9MB | 160.0KB | 953.1KB | 1.9KB |

## layout-stress (cols=4,rows=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 41µs | 7.3% | 41µs–42µs | 24.2K ops/s | 13.97ms | 3.9MB | 0.00KB | 1.1MB | 3.7KB |
| opentui-react | 1 | 50µs | 20.0% | 49µs–51µs | 20.1K ops/s | 16.69ms | 38.9MB | 0.00KB | 1.2KB | 4B |
| tuix-python | 1 | 51µs | 21.3% | 49µs–52µs | 19.7K ops/s | 17.08ms | 18.5MB | 0.00KB | 1.1MB | 3.6KB |
| TUIX.Core | 1 | 54µs | 25.9% | 53µs–56µs | 18.4K ops/s | 16.32ms | 4.1MB | 0.00KB | 1.1MB | 3.6KB |
| opentui-core | 1 | 88µs | 58.3% | 82µs–94µs | 11.4K ops/s | 34.79ms | 34.8MB | 0.00KB | 19.8MB | 67.7KB |
| ratatui | 1 | 92µs | 25.7% | 89µs–94µs | 10.9K ops/s | 27.56ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| bubbletea | 1 | 164µs | 35.8% | 157µs–171µs | 6.1K ops/s | 49.25ms | 16.6MB | 0.00KB | 4.7MB | 16.0KB |
| blessed | 1 | 194µs | 148.6% | 162µs–227µs | 5.1K ops/s | 58.37ms | 517.0MB | 0.00KB | 0.00KB | 0.00KB |
| rich | 1 | 804µs | 20.1% | 785µs–822µs | 1.2K ops/s | 241.24ms | 32.1MB | 12.0KB | 569.2KB | 1.9KB |
| terminal-kit | 1 | 1.12ms | 21.4% | 1.09ms–1.15ms | 892 ops/s | 336.78ms | 52.1MB | 0.00KB | 570.1KB | 1.9KB |
| prompt-toolkit | 1 | 1.14ms | 35.6% | 1.10ms–1.19ms | 874 ops/s | 343.39ms | 36.7MB | 56.0KB | 569.2KB | 1.9KB |
| rezitui | 1 | 1.38ms | 52.7% | 1.29ms–1.46ms | 727 ops/s | 412.78ms | 187.1MB | 2.0MB | 1.6MB | 5.6KB |
| urwid | 1 | 1.63ms | 31.8% | 1.57ms–1.69ms | 613 ops/s | 489.49ms | 42.2MB | 0.00KB | 562.5KB | 1.9KB |
| ink | 1 | 11.21ms | 20.4% | 10.95ms–11.47ms | 89 ops/s | 3.36s | 347.3MB | 640.0KB | 1.2MB | 4.2KB |

## scroll-stress (items=2000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 13µs | 14.1% | 12µs–13µs | 77.5K ops/s | 0.65ms | 4.0MB | 0.00KB | 800B | 16B |
| opentui-react | 1 | 45µs | 39.5% | 40µs–49µs | 22.5K ops/s | 2.30ms | 38.9MB | 0.00KB | 135.6KB | 2.7KB |
| rezitui | 1 | 65µs | 50.2% | 56µs–74µs | 15.5K ops/s | 3.24ms | 187.2MB | 56.0KB | 66.8KB | 1.3KB |
| opentui-core | 1 | 93µs | 59.4% | 78µs–108µs | 10.8K ops/s | 5.19ms | 34.9MB | 100.0KB | 3.1MB | 62.8KB |
| TUIX.Renderer | 1 | 100µs | 31.3% | 91µs–108µs | 10.0K ops/s | 5.11ms | 3.9MB | 0.00KB | 1.5MB | 30.8KB |
| bubbletea | 1 | 103µs | 35.5% | 93µs–113µs | 9.7K ops/s | 5.17ms | 16.6MB | 0.00KB | 798.0KB | 16.0KB |
| tuix-python | 1 | 121µs | 40.0% | 107µs–134µs | 8.3K ops/s | 6.21ms | 18.5MB | 0.00KB | 1.6MB | 32.4KB |
| ratatui | 1 | 125µs | 14.6% | 120µs–130µs | 8.0K ops/s | 6.26ms | 27.0KB | 0.00KB | 798.0KB | 16.0KB |
| prompt-toolkit | 1 | 670µs | 31.5% | 612µs–729µs | 1.5K ops/s | 33.53ms | 36.7MB | 0.00KB | 94.9KB | 1.9KB |
| rich | 1 | 705µs | 29.8% | 647µs–763µs | 1.4K ops/s | 35.30ms | 32.2MB | 80.0KB | 94.9KB | 1.9KB |
| urwid | 1 | 855µs | 14.2% | 821µs–889µs | 1.2K ops/s | 42.78ms | 42.2MB | 0.00KB | 93.8KB | 1.9KB |
| terminal-kit | 1 | 1.23ms | 11.9% | 1.19ms–1.27ms | 812 ops/s | 61.63ms | 52.1MB | 0.00KB | 95.0KB | 1.9KB |
| ink | 1 | 2.99ms | 57.5% | 2.52ms–3.47ms | 334 ops/s | 149.89ms | 347.3MB | 4.0KB | 93.2KB | 1.9KB |
| blessed | 1 | 3.84ms | 21.5% | 3.61ms–4.07ms | 260 ops/s | 192.14ms | 516.9MB | 0.00KB | 115.5KB | 2.3KB |

## virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 13µs | 7.5% | 12µs–13µs | 80.0K ops/s | 12.53ms | 4.1MB | 0.00KB | 2.2MB | 2.2KB |
| opentui-react | 1 | 42µs | 23.4% | 41µs–42µs | 24.0K ops/s | 43.21ms | 38.9MB | 0.00KB | 1.8MB | 1.8KB |
| TUIX.Renderer | 1 | 62µs | 49.6% | 60µs–64µs | 16.1K ops/s | 67.91ms | 4.0MB | 0.00KB | 2.3MB | 2.3KB |
| tuix-python | 1 | 62µs | 27.3% | 61µs–64µs | 16.0K ops/s | 68.15ms | 18.5MB | 4.0KB | 2.2MB | 2.3KB |
| ratatui | 1 | 82µs | 25.9% | 81µs–83µs | 12.2K ops/s | 82.22ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| rezitui | 1 | 85µs | 216.9% | 74µs–97µs | 11.7K ops/s | 85.62ms | 189.4MB | 2.0MB | 2.4MB | 2.5KB |
| bubbletea | 1 | 107µs | 62.4% | 103µs–111µs | 9.3K ops/s | 107.24ms | 20.8MB | 256.0KB | 15.6MB | 16.0KB |
| blessed | 1 | 118µs | 303.9% | 96µs–141µs | 8.4K ops/s | 118.67ms | 520.0MB | 148.0KB | 616.2KB | 631B |
| opentui-core | 1 | 126µs | 56.9% | 121µs–130µs | 8.0K ops/s | 152.40ms | 34.9MB | 0.00KB | 66.1MB | 67.7KB |
| prompt-toolkit | 1 | 598µs | 30.3% | 587µs–609µs | 1.7K ops/s | 598.59ms | 36.7MB | 0.00KB | 1.9MB | 1.9KB |
| rich | 1 | 610µs | 39.8% | 595µs–625µs | 1.6K ops/s | 610.55ms | 32.5MB | 308.0KB | 1.9MB | 1.9KB |
| terminal-kit | 1 | 1.07ms | 43.9% | 1.04ms–1.10ms | 938 ops/s | 1.07s | 93.4MB | 4.3MB | 1.9MB | 1.9KB |
| urwid | 1 | 1.07ms | 31.8% | 1.05ms–1.09ms | 935 ops/s | 1.07s | 42.3MB | 64.0KB | 1.8MB | 1.9KB |
| ink | 1 | 1.54ms | 28.5% | 1.51ms–1.56ms | 651 ops/s | 1.54s | 347.4MB | 0.00KB | 153.3KB | 157B |

## tables (cols=8,rows=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 13µs | 19.2% | 13µs–13µs | 77.5K ops/s | 3.88ms | 4.1MB | 0.00KB | 1.5MB | 5.2KB |
| TUIX.Renderer | 1 | 44µs | 9.8% | 43µs–44µs | 22.8K ops/s | 14.83ms | 4.0MB | 0.00KB | 1.6MB | 5.4KB |
| tuix-python | 1 | 57µs | 29.0% | 55µs–59µs | 17.6K ops/s | 20.05ms | 18.5MB | 0.00KB | 4.2MB | 14.3KB |
| opentui-core | 1 | 109µs | 53.6% | 103µs–116µs | 9.1K ops/s | 41.92ms | 34.9MB | 0.00KB | 19.8MB | 67.7KB |
| bubbletea | 1 | 131µs | 46.3% | 124µs–138µs | 7.6K ops/s | 39.41ms | 20.8MB | 0.00KB | 4.7MB | 16.0KB |
| ratatui | 1 | 149µs | 15.8% | 146µs–152µs | 6.7K ops/s | 44.77ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| opentui-react | 1 | 183µs | 43.1% | 174µs–192µs | 5.5K ops/s | 60.43ms | 38.9MB | 0.00KB | 20.6MB | 70.4KB |
| blessed | 1 | 333µs | 44.3% | 317µs–350µs | 3.0K ops/s | 100.05ms | 519.5MB | 68.0KB | 821.5KB | 2.7KB |
| rich | 1 | 746µs | 24.2% | 726µs–766µs | 1.3K ops/s | 223.94ms | 32.5MB | 8.0KB | 569.2KB | 1.9KB |
| rezitui | 1 | 832µs | 84.7% | 752µs–911µs | 1.2K ops/s | 249.60ms | 187.5MB | 0.00KB | 2.4MB | 8.0KB |
| prompt-toolkit | 1 | 897µs | 28.9% | 867µs–926µs | 1.1K ops/s | 269.17ms | 36.7MB | 0.00KB | 569.2KB | 1.9KB |
| terminal-kit | 1 | 1.11ms | 28.8% | 1.07ms–1.14ms | 905 ops/s | 331.91ms | 93.3MB | 0.00KB | 570.1KB | 1.9KB |
| ink | 1 | 1.58ms | 23.3% | 1.53ms–1.62ms | 635 ops/s | 473.20ms | 348.0MB | 0.00KB | 569.2KB | 1.9KB |
| urwid | 1 | 1.61ms | 31.1% | 1.55ms–1.67ms | 621 ops/s | 482.94ms | 42.3MB | 0.00KB | 562.5KB | 1.9KB |

## memory-profile

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 98µs | 30.8% | 96µs–99µs | 10.3K ops/s | 195.22ms | 27.0KB | 0.00KB | 31.2MB | 16.0KB |
| rezitui | 1 | 136µs | 461.4% | 108µs–163µs | 7.4K ops/s | 271.49ms | 187.8MB | 76.0KB | 2.6MB | 1.3KB |
| TUIX.Renderer | 1 | 139µs | 19.7% | 138µs–140µs | 7.2K ops/s | 322.53ms | 4.0MB | 0.00KB | 125.9MB | 64.4KB |
| bubbletea | 1 | 143µs | 59.8% | 139µs–147µs | 7.0K ops/s | 286.13ms | 20.8MB | 0.00KB | 31.2MB | 16.0KB |
| opentui-core | 1 | 168µs | 46.2% | 164µs–171µs | 6.0K ops/s | 392.92ms | 34.9MB | 36.0KB | 133.8MB | 68.5KB |
| TUIX.Core | 1 | 171µs | 28.1% | 169µs–173µs | 5.8K ops/s | 342.42ms | 4.1MB | 0.00KB | 120.6MB | 61.7KB |
| tuix-python | 1 | 173µs | 31.9% | 171µs–176µs | 5.8K ops/s | 396.56ms | 18.6MB | 60.0KB | 133.5MB | 68.3KB |
| opentui-react | 1 | 188µs | 41.1% | 185µs–192µs | 5.3K ops/s | 398.64ms | 39.1MB | 228.0KB | 147.0MB | 75.3KB |
| blessed | 1 | 402µs | 44.4% | 394µs–410µs | 2.5K ops/s | 804.82ms | 519.3MB | 0.00KB | 4.6MB | 2.3KB |
| terminal-kit | 1 | 1.31ms | 21.8% | 1.30ms–1.32ms | 764 ops/s | 2.62s | 93.3MB | 44.0KB | 3.7MB | 1.9KB |
| prompt-toolkit | 1 | 1.35ms | 29.8% | 1.34ms–1.37ms | 738 ops/s | 2.71s | 36.8MB | 84.0KB | 3.7MB | 1.9KB |
| ink | 1 | 1.61ms | 23.0% | 1.60ms–1.63ms | 620 ops/s | 3.23s | 348.0MB | 12.0KB | 3.7MB | 1.9KB |
| rich | 1 | 1.89ms | 30.6% | 1.86ms–1.92ms | 529 ops/s | 3.78s | 32.4MB | 0.00KB | 3.7MB | 1.9KB |
| urwid | 1 | 1.93ms | 30.3% | 1.90ms–1.95ms | 518 ops/s | 3.86s | 42.3MB | 64.0KB | 3.7MB | 1.9KB |

## terminal-rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 25µs | 18.7% | 25µs–26µs | 39.5K ops/s | 25.33ms | 4.2MB | 0.00KB | 15.6KB | 16B |
| tuix-python | 1 | 28µs | 69.0% | 27µs–29µs | 35.9K ops/s | 28.70ms | 18.7MB | 68.0KB | 4.6MB | 4.7KB |
| TUIX.Core | 1 | 32µs | 31.6% | 31µs–32µs | 31.7K ops/s | 31.57ms | 4.4MB | 0.00KB | 15.6KB | 16B |
| rezitui | 1 | 40µs | 299.9% | 33µs–48µs | 24.9K ops/s | 40.36ms | 187.3MB | 0.00KB | 2.2MB | 2.2KB |
| opentui-react | 1 | 103µs | 21.2% | 102µs–104µs | 9.7K ops/s | 103.21ms | 39.1MB | 0.00KB | 3.9KB | 4B |
| ratatui | 1 | 120µs | 43.8% | 117µs–124µs | 8.3K ops/s | 120.31ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| bubbletea | 1 | 125µs | 59.1% | 121µs–130µs | 8.0K ops/s | 125.51ms | 24.8MB | 4.0MB | 40.6MB | 41.6KB |
| blessed | 1 | 196µs | 29.1% | 193µs–200µs | 5.1K ops/s | 196.58ms | 519.3MB | 4.0KB | 0.00KB | 0.00KB |
| opentui-core | 1 | 382µs | 43.9% | 372µs–393µs | 2.6K ops/s | 382.64ms | 35.7MB | 24.0KB | 167.1MB | 171.1KB |
| terminal-kit | 1 | 1.33ms | 20.9% | 1.31ms–1.35ms | 751 ops/s | 1.33s | 93.4MB | 8.0KB | 4.6MB | 4.7KB |
| rich | 1 | 2.82ms | 25.0% | 2.78ms–2.86ms | 355 ops/s | 2.82s | 32.2MB | 84.0KB | 4.6MB | 4.7KB |
| ink | 1 | 2.85ms | 24.1% | 2.81ms–2.89ms | 351 ops/s | 2.85s | 347.6MB | 0.00KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 3.55ms | 27.9% | 3.48ms–3.61ms | 282 ops/s | 3.55s | 37.0MB | 0.00KB | 4.6MB | 4.7KB |
| urwid | 1 | 4.55ms | 28.0% | 4.47ms–4.63ms | 220 ops/s | 4.55s | 42.4MB | 64.0KB | 4.6MB | 4.7KB |

## terminal-frame-fill (cols=120,dirtyLines=1,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 32µs | 35.1% | 31µs–33µs | 31.6K ops/s | 16.23ms | 4.1MB | 0.00KB | 7.8KB | 16B |
| TUIX.Core | 1 | 34µs | 45.2% | 33µs–36µs | 29.2K ops/s | 17.16ms | 4.4MB | 0.00KB | 1.2MB | 2.4KB |
| tuix-python | 1 | 38µs | 58.1% | 36µs–40µs | 26.2K ops/s | 20.07ms | 18.7MB | 0.00KB | 22.6MB | 46.2KB |
| rezitui | 1 | 54µs | 207.7% | 44µs–64µs | 18.4K ops/s | 27.19ms | 188.9MB | 1.6MB | 1.1MB | 2.2KB |
| opentui-react | 1 | 132µs | 34.4% | 128µs–136µs | 7.6K ops/s | 69.40ms | 39.1MB | 0.00KB | 4.6MB | 9.5KB |
| bubbletea | 1 | 144µs | 76.9% | 135µs–154µs | 6.9K ops/s | 72.23ms | 24.8MB | 0.00KB | 20.3MB | 41.6KB |
| ratatui | 1 | 144µs | 27.8% | 141µs–148µs | 6.9K ops/s | 72.19ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 274µs | 33.3% | 266µs–282µs | 3.6K ops/s | 138.17ms | 36.7MB | 0.00KB | 83.5MB | 171.0KB |
| blessed | 1 | 431µs | 37.7% | 417µs–445µs | 2.3K ops/s | 215.66ms | 519.6MB | 8.0KB | 0.00KB | 0.00KB |
| rich | 1 | 855µs | 51.2% | 817µs–893µs | 1.2K ops/s | 427.86ms | 32.4MB | 172.0KB | 2.3MB | 4.7KB |
| terminal-kit | 1 | 1.35ms | 27.2% | 1.32ms–1.38ms | 741 ops/s | 675.69ms | 93.2MB | 0.00KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 1.60ms | 32.3% | 1.55ms–1.64ms | 627 ops/s | 798.33ms | 37.1MB | 104.0KB | 2.3MB | 4.7KB |
| urwid | 1 | 2.41ms | 29.7% | 2.35ms–2.48ms | 414 ops/s | 1.21s | 42.4MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 3.36ms | 17.3% | 3.31ms–3.41ms | 298 ops/s | 1.68s | 348.1MB | 384.0KB | 77.6KB | 159B |

## terminal-frame-fill (cols=120,dirtyLines=40,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 32µs | 11.8% | 32µs–33µs | 30.9K ops/s | 16.19ms | 4.4MB | 0.00KB | 41.8MB | 85.5KB |
| rezitui | 1 | 174µs | 349.5% | 121µs–227µs | 5.7K ops/s | 87.15ms | 200.4MB | 11.5MB | 1.1MB | 2.2KB |
| TUIX.Renderer | 1 | 263µs | 19.3% | 259µs–268µs | 3.8K ops/s | 147.27ms | 4.2MB | 0.00KB | 42.8MB | 87.8KB |
| ratatui | 1 | 273µs | 32.2% | 265µs–280µs | 3.7K ops/s | 136.31ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| tuix-python | 1 | 339µs | 28.8% | 331µs–348µs | 2.9K ops/s | 188.69ms | 18.7MB | 0.00KB | 42.9MB | 87.8KB |
| opentui-core | 1 | 341µs | 35.7% | 330µs–351µs | 2.9K ops/s | 192.96ms | 36.7MB | 0.00KB | 83.5MB | 171.0KB |
| bubbletea | 1 | 365µs | 62.1% | 345µs–385µs | 2.7K ops/s | 182.80ms | 24.8MB | 0.00KB | 20.3MB | 41.6KB |
| opentui-react | 1 | 451µs | 32.1% | 438µs–464µs | 2.2K ops/s | 239.20ms | 39.1MB | 0.00KB | 92.3MB | 189.0KB |
| blessed | 1 | 890µs | 48.3% | 852µs–928µs | 1.1K ops/s | 445.29ms | 520.0MB | 312.0KB | 2.7MB | 5.5KB |
| terminal-kit | 1 | 3.03ms | 16.5% | 2.98ms–3.07ms | 330 ops/s | 1.51s | 93.3MB | 4.0KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 3.10ms | 28.0% | 3.03ms–3.18ms | 322 ops/s | 1.55s | 37.0MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 3.31ms | 19.7% | 3.25ms–3.37ms | 302 ops/s | 1.66s | 348.1MB | 0.00KB | 2.3MB | 4.7KB |
| urwid | 1 | 4.25ms | 33.3% | 4.12ms–4.37ms | 236 ops/s | 2.12s | 42.4MB | 4.0KB | 2.3MB | 4.7KB |
| rich | 1 | 4.35ms | 29.8% | 4.23ms–4.46ms | 230 ops/s | 2.17s | 33.0MB | 308.0KB | 2.3MB | 4.7KB |

## terminal-screen-transition (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 182µs | 382.7% | 139µs–225µs | 5.5K ops/s | 182.35ms | 206.7MB | 6.1MB | 2.2MB | 2.2KB |
| bubbletea | 1 | 218µs | 57.4% | 210µs–225µs | 4.6K ops/s | 217.75ms | 24.8MB | 0.00KB | 40.6MB | 41.6KB |
| TUIX.Renderer | 1 | 253µs | 45.3% | 246µs–260µs | 4.0K ops/s | 288.46ms | 4.3MB | 0.00KB | 157.1MB | 160.9KB |
| ratatui | 1 | 254µs | 101.2% | 238µs–270µs | 3.9K ops/s | 253.92ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| tuix-python | 1 | 293µs | 45.0% | 285µs–301µs | 3.4K ops/s | 328.65ms | 18.8MB | 64.0KB | 166.7MB | 170.8KB |
| TUIX.Core | 1 | 329µs | 45.0% | 320µs–338µs | 3.0K ops/s | 329.25ms | 4.6MB | 0.00KB | 153.2MB | 156.8KB |
| opentui-core | 1 | 445µs | 55.1% | 430µs–460µs | 2.2K ops/s | 519.12ms | 36.7MB | 0.00KB | 171.1MB | 175.2KB |
| opentui-react | 1 | 500µs | 40.2% | 488µs–513µs | 2.0K ops/s | 519.15ms | 39.1MB | 0.00KB | 187.1MB | 191.6KB |
| blessed | 1 | 863µs | 33.7% | 845µs–881µs | 1.2K ops/s | 863.65ms | 519.8MB | 4.0KB | 5.3MB | 5.4KB |
| rich | 1 | 2.16ms | 31.0% | 2.11ms–2.20ms | 464 ops/s | 2.16s | 33.0MB | 0.00KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 2.52ms | 29.3% | 2.48ms–2.57ms | 397 ops/s | 2.52s | 37.1MB | 68.0KB | 4.6MB | 4.7KB |
| terminal-kit | 1 | 2.71ms | 20.3% | 2.68ms–2.75ms | 369 ops/s | 2.71s | 93.4MB | 0.00KB | 4.6MB | 4.7KB |
| ink | 1 | 3.05ms | 21.3% | 3.01ms–3.09ms | 328 ops/s | 3.05s | 348.0MB | 28.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 3.17ms | 30.5% | 3.11ms–3.23ms | 316 ops/s | 3.17s | 42.4MB | 8.0KB | 4.6MB | 4.7KB |

## terminal-fps-stream (channels=12,cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 32µs | 15.0% | 31µs–32µs | 31.5K ops/s | 38.14ms | 4.5MB | 0.00KB | 2.3MB | 2.0KB |
| TUIX.Renderer | 1 | 49µs | 5.8% | 49µs–49µs | 20.3K ops/s | 59.64ms | 4.3MB | 0.00KB | 2.3MB | 2.0KB |
| tuix-python | 1 | 61µs | 35.8% | 59µs–62µs | 16.5K ops/s | 75.32ms | 18.8MB | 16.0KB | 8.4MB | 7.2KB |
| bubbletea | 1 | 182µs | 56.9% | 177µs–188µs | 5.5K ops/s | 219.14ms | 24.8MB | 0.00KB | 48.8MB | 41.6KB |
| ratatui | 1 | 226µs | 25.5% | 223µs–230µs | 4.4K ops/s | 271.78ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 251µs | 31.7% | 247µs–256µs | 4.0K ops/s | 320.76ms | 39.1MB | 0.00KB | 202.0MB | 172.3KB |
| blessed | 1 | 466µs | 45.1% | 454µs–478µs | 2.1K ops/s | 559.04ms | 519.6MB | 12.0KB | 5.7MB | 4.8KB |
| opentui-core | 1 | 608µs | 35.6% | 596µs–620µs | 1.6K ops/s | 837.23ms | 36.7MB | 0.00KB | 193.0MB | 164.7KB |
| rezitui | 1 | 835µs | 143.1% | 768µs–903µs | 1.2K ops/s | 1.00s | 207.6MB | 1.2MB | 4.2MB | 3.6KB |
| rich | 1 | 2.10ms | 29.8% | 2.06ms–2.13ms | 477 ops/s | 2.52s | 33.0MB | 0.00KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.60ms | 30.7% | 2.55ms–2.64ms | 385 ops/s | 3.12s | 37.1MB | 0.00KB | 5.5MB | 4.7KB |
| terminal-kit | 1 | 2.91ms | 18.3% | 2.88ms–2.94ms | 344 ops/s | 3.50s | 93.4MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 3.00ms | 23.3% | 2.96ms–3.04ms | 334 ops/s | 3.60s | 348.2MB | 8.0KB | 2.5MB | 2.1KB |
| urwid | 1 | 3.28ms | 30.1% | 3.22ms–3.33ms | 305 ops/s | 3.93s | 42.5MB | 64.0KB | 5.5MB | 4.7KB |

## terminal-input-latency (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| tuix-python | 1 | 27µs | 8.2% | 27µs–27µs | 36.6K ops/s | 28.17ms | 18.9MB | 32.0KB | 4.6MB | 4.7KB |
| TUIX.Renderer | 1 | 29µs | 57.3% | 28µs–30µs | 34.3K ops/s | 29.19ms | 4.3MB | 0.00KB | 15.6KB | 16B |
| TUIX.Core | 1 | 33µs | 55.9% | 32µs–34µs | 30.1K ops/s | 33.29ms | 4.5MB | 0.00KB | 52.7KB | 54B |
| ratatui | 1 | 115µs | 26.7% | 113µs–116µs | 8.7K ops/s | 114.61ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| opentui-react | 1 | 129µs | 45.3% | 125µs–132µs | 7.8K ops/s | 153.18ms | 39.1MB | 0.00KB | 833.0KB | 853B |
| rezitui | 1 | 185µs | 306.7% | 150µs–220µs | 5.4K ops/s | 185.65ms | 208.4MB | 824.0KB | 2.2MB | 2.2KB |
| blessed | 1 | 432µs | 42.0% | 420µs–443µs | 2.3K ops/s | 431.93ms | 520.0MB | 336.0KB | 0.00KB | 0.00KB |
| bubbletea | 1 | 561µs | 99.1% | 527µs–596µs | 1.8K ops/s | 561.61ms | 24.8MB | 0.00KB | 40.6MB | 41.6KB |
| opentui-core | 1 | 772µs | 66.5% | 740µs–803µs | 1.3K ops/s | 773.15ms | 36.7MB | 0.00KB | 166.6MB | 170.6KB |
| terminal-kit | 1 | 1.30ms | 21.5% | 1.28ms–1.31ms | 772 ops/s | 1.30s | 93.4MB | 24.0KB | 4.6MB | 4.7KB |
| rich | 1 | 1.52ms | 29.5% | 1.49ms–1.54ms | 660 ops/s | 1.52s | 33.4MB | 412.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 2.31ms | 29.7% | 2.27ms–2.35ms | 433 ops/s | 2.31s | 37.4MB | 224.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 3.02ms | 27.8% | 2.96ms–3.07ms | 331 ops/s | 3.02s | 42.5MB | 36.0KB | 4.6MB | 4.7KB |
| ink | 1 | 3.72ms | 20.7% | 3.67ms–3.77ms | 269 ops/s | 3.72s | 348.3MB | 0.00KB | 4.6MB | 4.7KB |

## terminal-memory-soak (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 318µs | 43.0% | 311µs–326µs | 3.1K ops/s | 382.07ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| TUIX.Renderer | 1 | 358µs | 20.3% | 354µs–362µs | 2.8K ops/s | 497.20ms | 4.3MB | 0.00KB | 188.5MB | 160.9KB |
| tuix-python | 1 | 392µs | 11.1% | 389µs–394µs | 2.6K ops/s | 539.77ms | 19.0MB | 80.0KB | 200.1MB | 170.8KB |
| rezitui | 1 | 434µs | 361.8% | 346µs–523µs | 2.3K ops/s | 522.51ms | 208.4MB | 8.0KB | 2.6MB | 2.2KB |
| TUIX.Core | 1 | 441µs | 19.1% | 436µs–445µs | 2.3K ops/s | 528.68ms | 4.6MB | 124.0KB | 183.8MB | 156.8KB |
| bubbletea | 1 | 458µs | 128.5% | 425µs–492µs | 2.2K ops/s | 550.27ms | 24.8MB | 8.0KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 846µs | 52.6% | 821µs–871µs | 1.2K ops/s | 1.04s | 39.1MB | 0.00KB | 222.5MB | 189.9KB |
| opentui-core | 1 | 863µs | 35.2% | 846µs–880µs | 1.2K ops/s | 1.13s | 36.7MB | 0.00KB | 200.4MB | 171.0KB |
| blessed | 1 | 1.01ms | 32.3% | 996µs–1.03ms | 986 ops/s | 1.22s | 520.0MB | 0.00KB | 6.5MB | 5.5KB |
| terminal-kit | 1 | 3.00ms | 18.3% | 2.97ms–3.03ms | 334 ops/s | 3.60s | 93.5MB | 0.00KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 3.47ms | 26.6% | 3.42ms–3.52ms | 288 ops/s | 4.17s | 37.2MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 3.49ms | 20.9% | 3.45ms–3.54ms | 286 ops/s | 4.20s | 348.4MB | 40.0KB | 5.5MB | 4.7KB |
| rich | 1 | 4.43ms | 29.0% | 4.36ms–4.50ms | 226 ops/s | 5.32s | 33.4MB | 360.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 4.67ms | 26.0% | 4.60ms–4.74ms | 214 ops/s | 5.60s | 42.5MB | 20.0KB | 5.5MB | 4.7KB |

## terminal-full-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 33µs | 18.7% | 32µs–33µs | 30.5K ops/s | 39.42ms | 4.6MB | 0.00KB | 3.9MB | 3.3KB |
| TUIX.Renderer | 1 | 72µs | 24.4% | 71µs–73µs | 13.8K ops/s | 94.56ms | 4.3MB | 0.00KB | 3.9MB | 3.3KB |
| tuix-python | 1 | 82µs | 23.8% | 81µs–83µs | 12.1K ops/s | 105.80ms | 19.0MB | 0.00KB | 6.5MB | 5.6KB |
| ratatui | 1 | 393µs | 45.7% | 383µs–403µs | 2.5K ops/s | 472.09ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| bubbletea | 1 | 608µs | 123.0% | 565µs–650µs | 1.6K ops/s | 729.45ms | 24.8MB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 646µs | 32.5% | 634µs–658µs | 1.5K ops/s | 809.73ms | 39.1MB | 0.00KB | 149.5MB | 127.6KB |
| blessed | 1 | 675µs | 38.6% | 660µs–690µs | 1.5K ops/s | 810.18ms | 520.1MB | 24.0KB | 3.8MB | 3.2KB |
| opentui-core | 1 | 689µs | 37.1% | 675µs–704µs | 1.5K ops/s | 895.95ms | 36.7MB | 0.00KB | 204.3MB | 174.4KB |
| rich | 1 | 1.95ms | 32.9% | 1.91ms–1.99ms | 513 ops/s | 2.34s | 33.7MB | 212.0KB | 5.5MB | 4.7KB |
| rezitui | 1 | 2.08ms | 76.1% | 1.99ms–2.17ms | 481 ops/s | 2.50s | 208.3MB | 0.00KB | 4.3MB | 3.7KB |
| terminal-kit | 1 | 2.44ms | 20.9% | 2.41ms–2.47ms | 409 ops/s | 2.93s | 93.9MB | 8.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.55ms | 29.2% | 2.51ms–2.60ms | 391 ops/s | 3.07s | 37.2MB | 16.0KB | 5.5MB | 4.7KB |
| ink | 1 | 2.60ms | 20.9% | 2.57ms–2.63ms | 385 ops/s | 3.12s | 348.7MB | 184.0KB | 3.4MB | 2.9KB |
| urwid | 1 | 3.49ms | 29.8% | 3.43ms–3.55ms | 287 ops/s | 4.19s | 42.6MB | 48.0KB | 5.5MB | 4.7KB |

## terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 43µs | 60.8% | 42µs–45µs | 23.2K ops/s | 51.70ms | 4.5MB | 0.00KB | 69.9MB | 59.7KB |
| TUIX.Renderer | 1 | 93µs | 53.4% | 90µs–95µs | 10.8K ops/s | 127.98ms | 4.3MB | 0.00KB | 69.9MB | 59.7KB |
| tuix-python | 1 | 101µs | 55.2% | 98µs–104µs | 9.9K ops/s | 136.73ms | 19.0MB | 32.0KB | 74.7MB | 63.7KB |
| ratatui | 1 | 337µs | 100.4% | 318µs–356µs | 3.0K ops/s | 404.10ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| bubbletea | 1 | 611µs | 97.2% | 578µs–645µs | 1.6K ops/s | 733.92ms | 24.8MB | 0.00KB | 48.8MB | 41.6KB |
| blessed | 1 | 662µs | 39.6% | 647µs–677µs | 1.5K ops/s | 794.45ms | 519.5MB | 300.0KB | 3.8MB | 3.2KB |
| opentui-react | 1 | 721µs | 33.9% | 707µs–734µs | 1.4K ops/s | 891.33ms | 39.1MB | 0.00KB | 149.5MB | 127.6KB |
| opentui-core | 1 | 774µs | 64.9% | 746µs–802µs | 1.3K ops/s | 991.23ms | 36.7MB | 0.00KB | 204.3MB | 174.4KB |
| rezitui | 1 | 1.93ms | 80.3% | 1.84ms–2.02ms | 517 ops/s | 2.32s | 210.8MB | 3.2MB | 4.4MB | 3.7KB |
| rich | 1 | 2.15ms | 31.8% | 2.11ms–2.18ms | 466 ops/s | 2.58s | 33.7MB | 76.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.30ms | 30.9% | 2.26ms–2.34ms | 435 ops/s | 2.76s | 37.6MB | 456.0KB | 5.5MB | 4.7KB |
| terminal-kit | 1 | 2.41ms | 18.7% | 2.38ms–2.44ms | 415 ops/s | 2.89s | 94.0MB | 48.0KB | 5.5MB | 4.7KB |
| ink | 1 | 2.93ms | 26.1% | 2.88ms–2.97ms | 342 ops/s | 3.51s | 349.0MB | 0.00KB | 5.4MB | 4.6KB |
| urwid | 1 | 3.53ms | 26.0% | 3.48ms–3.58ms | 283 ops/s | 4.23s | 42.6MB | 40.0KB | 5.5MB | 4.7KB |

## terminal-strict-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 33µs | 25.7% | 33µs–33µs | 30.3K ops/s | 39.64ms | 4.6MB | 0.00KB | 3.9MB | 3.3KB |
| TUIX.Renderer | 1 | 79µs | 37.8% | 77µs–81µs | 12.7K ops/s | 103.41ms | 4.3MB | 0.00KB | 3.9MB | 3.3KB |
| tuix-python | 1 | 83µs | 20.6% | 82µs–84µs | 12.1K ops/s | 106.69ms | 19.2MB | 96.0KB | 6.5MB | 5.6KB |
| ratatui | 1 | 355µs | 57.1% | 344µs–367µs | 2.8K ops/s | 426.71ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| bubbletea | 1 | 620µs | 112.4% | 581µs–660µs | 1.6K ops/s | 744.60ms | 24.8MB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 625µs | 32.8% | 613µs–637µs | 1.6K ops/s | 750.52ms | 36.7MB | 0.00KB | 186.1MB | 158.8KB |
| blessed | 1 | 690µs | 39.9% | 674µs–705µs | 1.5K ops/s | 827.91ms | 519.7MB | 68.0KB | 3.8MB | 3.2KB |
| opentui-react | 1 | 1.20ms | 100.1% | 1.13ms–1.27ms | 833 ops/s | 1.44s | 56.6MB | 17.5MB | 240.6MB | 205.3KB |
| rezitui | 1 | 1.83ms | 114.5% | 1.71ms–1.95ms | 546 ops/s | 2.20s | 210.8MB | 4.0KB | 4.3MB | 3.7KB |
| rich | 1 | 2.05ms | 31.4% | 2.02ms–2.09ms | 487 ops/s | 2.47s | 33.8MB | 36.0KB | 5.5MB | 4.7KB |
| terminal-kit | 1 | 2.32ms | 22.4% | 2.29ms–2.35ms | 430 ops/s | 2.79s | 94.0MB | 0.00KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.40ms | 29.3% | 2.36ms–2.44ms | 417 ops/s | 2.88s | 37.3MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 2.60ms | 17.9% | 2.57ms–2.62ms | 385 ops/s | 3.12s | 349.0MB | 0.00KB | 3.4MB | 2.9KB |
| urwid | 1 | 3.42ms | 30.3% | 3.36ms–3.47ms | 293 ops/s | 4.10s | 42.7MB | 84.0KB | 5.5MB | 4.7KB |

## terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 36µs | 53.4% | 34µs–37µs | 28.1K ops/s | 42.71ms | 4.6MB | 0.00KB | 69.9MB | 59.7KB |
| TUIX.Renderer | 1 | 86µs | 48.9% | 84µs–89µs | 11.6K ops/s | 119.62ms | 4.3MB | 0.00KB | 69.9MB | 59.7KB |
| tuix-python | 1 | 102µs | 51.2% | 99µs–105µs | 9.8K ops/s | 138.13ms | 19.1MB | 0.00KB | 74.7MB | 63.7KB |
| ratatui | 1 | 359µs | 48.1% | 349µs–369µs | 2.8K ops/s | 431.08ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 594µs | 27.2% | 585µs–603µs | 1.7K ops/s | 713.77ms | 36.7MB | 0.00KB | 186.1MB | 158.8KB |
| bubbletea | 1 | 624µs | 101.7% | 588µs–660µs | 1.6K ops/s | 749.64ms | 24.8MB | 0.00KB | 48.8MB | 41.6KB |
| blessed | 1 | 670µs | 40.2% | 655µs–685µs | 1.5K ops/s | 804.22ms | 519.8MB | 48.0KB | 3.8MB | 3.2KB |
| opentui-react | 1 | 1.21ms | 75.5% | 1.15ms–1.26ms | 830 ops/s | 1.45s | 58.0MB | 1.4MB | 240.6MB | 205.3KB |
| rezitui | 1 | 1.88ms | 90.9% | 1.79ms–1.98ms | 531 ops/s | 2.26s | 210.8MB | 20.0KB | 4.4MB | 3.7KB |
| rich | 1 | 2.17ms | 36.4% | 2.13ms–2.22ms | 460 ops/s | 2.61s | 33.8MB | 36.0KB | 5.5MB | 4.7KB |
| terminal-kit | 1 | 2.30ms | 22.1% | 2.27ms–2.33ms | 434 ops/s | 2.77s | 94.1MB | 0.00KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.54ms | 28.1% | 2.50ms–2.58ms | 393 ops/s | 3.05s | 37.6MB | 228.0KB | 5.5MB | 4.7KB |
| ink | 1 | 3.06ms | 23.7% | 3.01ms–3.10ms | 327 ops/s | 3.67s | 349.0MB | 0.00KB | 5.4MB | 4.6KB |
| urwid | 1 | 3.60ms | 28.8% | 3.54ms–3.65ms | 278 ops/s | 4.32s | 42.8MB | 64.0KB | 5.5MB | 4.7KB |

## terminal-virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 34µs | 15.7% | 34µs–35µs | 29.1K ops/s | 34.41ms | 4.6MB | 0.00KB | 5.2MB | 5.3KB |
| TUIX.Renderer | 1 | 133µs | 26.3% | 131µs–136µs | 7.5K ops/s | 144.08ms | 4.3MB | 0.00KB | 5.3MB | 5.4KB |
| rezitui | 1 | 147µs | 240.4% | 125µs–169µs | 6.8K ops/s | 147.78ms | 210.8MB | 4.0KB | 4.1MB | 4.2KB |
| tuix-python | 1 | 148µs | 23.4% | 146µs–150µs | 6.7K ops/s | 157.88ms | 19.3MB | 64.0KB | 5.2MB | 5.3KB |
| opentui-react | 1 | 180µs | 48.5% | 175µs–185µs | 5.6K ops/s | 185.09ms | 58.0MB | 0.00KB | 3.0MB | 3.1KB |
| blessed | 1 | 219µs | 93.9% | 206µs–232µs | 4.6K ops/s | 219.36ms | 520.8MB | 68.0KB | 1.0MB | 1.0KB |
| bubbletea | 1 | 351µs | 131.4% | 322µs–379µs | 2.9K ops/s | 350.98ms | 24.8MB | 0.00KB | 40.6MB | 41.6KB |
| ratatui | 1 | 359µs | 46.7% | 348µs–369µs | 2.8K ops/s | 358.81ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| opentui-core | 1 | 695µs | 41.3% | 677µs–712µs | 1.4K ops/s | 770.26ms | 36.7MB | 0.00KB | 165.1MB | 169.0KB |
| rich | 1 | 1.08ms | 34.0% | 1.05ms–1.10ms | 929 ops/s | 1.08s | 33.9MB | 36.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 1.57ms | 27.2% | 1.54ms–1.60ms | 637 ops/s | 1.57s | 37.6MB | 8.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 2.56ms | 29.8% | 2.51ms–2.61ms | 390 ops/s | 2.56s | 42.8MB | 0.00KB | 4.6MB | 4.7KB |
| terminal-kit | 1 | 2.86ms | 19.5% | 2.82ms–2.89ms | 350 ops/s | 2.86s | 125.7MB | 416.0KB | 4.6MB | 4.7KB |
| ink | 1 | 3.23ms | 21.5% | 3.19ms–3.27ms | 310 ops/s | 3.23s | 350.2MB | 4.0KB | 262.7KB | 269B |

## terminal-table (cols=8,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 31µs | 15.1% | 31µs–31µs | 32.2K ops/s | 15.56ms | 4.6MB | 0.00KB | 5.1MB | 10.5KB |
| TUIX.Renderer | 1 | 117µs | 26.0% | 115µs–120µs | 8.5K ops/s | 66.23ms | 4.4MB | 0.00KB | 5.2MB | 10.7KB |
| tuix-python | 1 | 241µs | 87.9% | 223µs–260µs | 4.1K ops/s | 136.73ms | 19.2MB | 0.00KB | 12.6MB | 25.8KB |
| bubbletea | 1 | 420µs | 82.1% | 390µs–450µs | 2.4K ops/s | 210.35ms | 24.8MB | 0.00KB | 20.3MB | 41.6KB |
| ratatui | 1 | 475µs | 42.0% | 457µs–492µs | 2.1K ops/s | 237.50ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 557µs | 33.7% | 541µs–574µs | 1.8K ops/s | 323.21ms | 36.7MB | 0.00KB | 82.5MB | 169.0KB |
| blessed | 1 | 1.03ms | 39.3% | 993µs–1.06ms | 973 ops/s | 514.48ms | 520.6MB | 8.0KB | 3.0MB | 6.2KB |
| opentui-react | 1 | 1.18ms | 33.6% | 1.15ms–1.22ms | 846 ops/s | 623.42ms | 58.0MB | 0.00KB | 91.4MB | 187.3KB |
| rich | 1 | 2.05ms | 32.7% | 1.99ms–2.10ms | 489 ops/s | 1.02s | 33.4MB | 0.00KB | 2.3MB | 4.7KB |
| rezitui | 1 | 2.16ms | 62.0% | 2.05ms–2.28ms | 462 ops/s | 1.08s | 212.6MB | 1.8MB | 7.6MB | 15.6KB |
| prompt-toolkit | 1 | 2.70ms | 29.4% | 2.63ms–2.77ms | 370 ops/s | 1.35s | 37.4MB | 0.00KB | 2.3MB | 4.7KB |
| terminal-kit | 1 | 2.89ms | 20.0% | 2.84ms–2.94ms | 346 ops/s | 1.45s | 126.0MB | 4.0KB | 2.3MB | 4.7KB |
| ink | 1 | 3.40ms | 15.3% | 3.35ms–3.44ms | 294 ops/s | 1.70s | 349.5MB | 0.00KB | 2.3MB | 4.7KB |
| urwid | 1 | 4.01ms | 24.5% | 3.93ms–4.10ms | 249 ops/s | 2.01s | 42.8MB | 0.00KB | 2.3MB | 4.7KB |

## Relative Performance (vs fastest per scenario)

> Ratio = framework mean / fastest mean for each scenario. Lower is better. 1.0x = fastest.

| Scenario | blessed | terminal-kit | ink | rezitui | bubbletea | ratatui | opentui-core | opentui-react | TUIX.Renderer | TUIX.Core | tuix-python | rich | urwid | prompt-toolkit |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| startup | 180.7x [171.4x, 189.9x] | 2.7x [2.7x, 2.8x] | 21.2x [20.0x, 22.4x] | 1.1x [0.7x, 1.5x] | 1.9x [1.6x, 2.2x] | 1.5x [1.4x, 1.5x] | 3.6x [3.3x, 3.9x] | 3.3x [2.9x, 3.8x] | 1.1x [1.0x, 1.2x] | **1.0x** | **1.0x** | 8.1x [7.9x, 8.3x] | 8.1x [7.9x, 8.3x] | 5.8x [5.6x, 6.0x] |
| tree-construction (items=10) | 2.9x [2.2x, 3.6x] | 8.2x [8.1x, 8.4x] | 20.0x [19.0x, 21.1x] | 3.3x [2.7x, 3.8x] | 6.2x [5.8x, 6.6x] | 1.9x [1.8x, 1.9x] | 9.2x [8.7x, 9.6x] | 1.6x [1.6x, 1.7x] | 1.3x [1.3x, 1.4x] | **1.0x** | 2.2x [2.1x, 2.2x] | 27.5x [26.7x, 28.3x] | 45.5x [44.1x, 46.9x] | 26.1x [25.3x, 26.8x] |
| tree-construction (items=100) | 9.8x [8.7x, 10.9x] | 25.7x [25.1x, 26.4x] | 150.4x [145.4x, 155.4x] | 2.5x [1.9x, 3.0x] | 4.8x [4.6x, 5.1x] | 3.5x [3.4x, 3.6x] | 2.8x [2.7x, 2.8x] | 2.1x [2.1x, 2.2x] | 1.4x [1.4x, 1.4x] | **1.0x** | 2.5x [2.5x, 2.6x] | 17.8x [17.3x, 18.3x] | 34.5x [33.6x, 35.5x] | 18.1x [17.7x, 18.5x] |
| tree-construction (items=500) | 42.5x [40.3x, 44.6x] | 197.9x [189.5x, 206.4x] | 972.3x [957.2x, 987.4x] | 7.0x [5.3x, 8.7x] | 14.7x [14.3x, 15.1x] | 13.2x [12.9x, 13.5x] | 3.5x [2.9x, 4.1x] | 4.8x [4.6x, 4.9x] | **1.0x** | 1.1x [1.1x, 1.1x] | 2.8x [2.8x, 2.9x] | 23.5x [22.8x, 24.3x] | 32.2x [31.4x, 33.0x] | 18.9x [18.6x, 19.2x] |
| tree-construction (items=1000) | 81.4x [78.3x, 84.5x] | 500.1x [494.2x, 506.1x] | 1616.6x [1589.3x, 1643.8x] | 9.1x [7.3x, 10.8x] | 24.9x [24.5x, 25.4x] | 23.9x [23.2x, 24.5x] | 2.1x [2.0x, 2.2x] | 5.6x [4.9x, 6.4x] | 1.2x [1.2x, 1.2x] | **1.0x** | 2.8x [2.8x, 2.9x] | 18.1x [17.6x, 18.6x] | 33.1x [32.2x, 33.9x] | 19.9x [19.4x, 20.4x] |
| rerender | 2.6x [2.2x, 3.1x] | 40.3x [39.6x, 40.9x] | 91.7x [89.6x, 93.7x] | 4.5x [3.6x, 5.5x] | 10.7x [10.3x, 11.0x] | 5.9x [5.7x, 6.0x] | 9.8x [9.5x, 10.1x] | 3.7x [3.6x, 3.8x] | **1.0x** | 1.2x [1.1x, 1.2x] | **1.0x** | 136.3x [133.4x, 139.1x] | 167.6x [164.4x, 170.8x] | 115.9x [113.3x, 118.5x] |
| content-update | 25.4x [24.1x, 26.8x] | 48.4x [46.9x, 49.8x] | 459.2x [450.0x, 468.4x] | 5.3x [3.3x, 7.3x] | 19.3x [18.7x, 20.0x] | 8.2x [7.9x, 8.5x] | 11.5x [11.0x, 12.0x] | 7.2x [6.9x, 7.5x] | 6.4x [6.2x, 6.5x] | **1.0x** | 6.2x [6.1x, 6.4x] | 107.9x [105.3x, 110.6x] | 144.9x [141.2x, 148.5x] | 84.8x [82.6x, 86.9x] |
| layout-stress (cols=4,rows=10) | 4.7x [3.9x, 5.5x] | 27.1x [26.5x, 27.8x] | 271.1x [264.8x, 277.3x] | 33.3x [31.3x, 35.2x] | 4.0x [3.8x, 4.1x] | 2.2x [2.2x, 2.3x] | 2.1x [2.0x, 2.3x] | 1.2x [1.2x, 1.2x] | **1.0x** | 1.3x [1.3x, 1.4x] | 1.2x [1.2x, 1.3x] | 19.4x [19.0x, 19.9x] | 39.4x [38.0x, 40.9x] | 27.7x [26.5x, 28.8x] |
| scroll-stress (items=2000) | 297.6x [279.9x, 315.4x] | 95.4x [92.3x, 98.6x] | 232.0x [195.0x, 268.9x] | 5.0x [4.3x, 5.7x] | 8.0x [7.2x, 8.8x] | 9.7x [9.3x, 10.1x] | 7.2x [6.0x, 8.4x] | 3.4x [3.1x, 3.8x] | 7.7x [7.1x, 8.4x] | **1.0x** | 9.3x [8.3x, 10.4x] | 54.6x [50.1x, 59.1x] | 66.2x [63.6x, 68.9x] | 51.9x [47.4x, 56.4x] |
| virtual-list (items=100000,viewport=40) | 9.5x [7.7x, 11.3x] | 85.3x [83.0x, 87.6x] | 122.8x [120.6x, 125.0x] | 6.8x [5.9x, 7.8x] | 8.6x [8.2x, 8.9x] | 6.6x [6.5x, 6.7x] | 10.1x [9.7x, 10.4x] | 3.3x [3.3x, 3.4x] | 5.0x [4.8x, 5.1x] | **1.0x** | 5.0x [4.9x, 5.1x] | 48.8x [47.6x, 50.0x] | 85.6x [83.9x, 87.2x] | 47.8x [46.9x, 48.7x] |
| tables (cols=8,rows=100) | 25.8x [24.5x, 27.1x] | 85.6x [82.9x, 88.4x] | 122.1x [118.9x, 125.3x] | 64.4x [58.3x, 70.6x] | 10.2x [9.6x, 10.7x] | 11.6x [11.3x, 11.8x] | 8.5x [8.0x, 9.0x] | 14.2x [13.5x, 14.9x] | 3.4x [3.4x, 3.4x] | **1.0x** | 4.4x [4.3x, 4.5x] | 57.8x [56.2x, 59.4x] | 124.7x [120.3x, 129.1x] | 69.5x [67.2x, 71.8x] |
| memory-profile | 4.1x [4.0x, 4.2x] | 13.4x [13.3x, 13.5x] | 16.5x [16.4x, 16.7x] | 1.4x [1.1x, 1.7x] | 1.5x [1.4x, 1.5x] | **1.0x** | 1.7x [1.7x, 1.8x] | 1.9x [1.9x, 2.0x] | 1.4x [1.4x, 1.4x] | 1.8x [1.7x, 1.8x] | 1.8x [1.8x, 1.8x] | 19.4x [19.1x, 19.6x] | 19.8x [19.5x, 20.0x] | 13.9x [13.7x, 14.1x] |
| terminal-rerender | 7.8x [7.6x, 7.9x] | 52.6x [52.0x, 53.3x] | 112.6x [111.0x, 114.3x] | 1.6x [1.3x, 1.9x] | 5.0x [4.8x, 5.1x] | 4.8x [4.6x, 4.9x] | 15.1x [14.7x, 15.5x] | 4.1x [4.0x, 4.1x] | **1.0x** | 1.2x [1.2x, 1.3x] | 1.1x [1.1x, 1.1x] | 111.5x [109.8x, 113.2x] | 179.8x [176.7x, 182.9x] | 140.1x [137.7x, 142.6x] |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | 13.6x [13.2x, 14.1x] | 42.6x [41.6x, 43.7x] | 106.1x [104.5x, 107.7x] | 1.7x [1.4x, 2.0x] | 4.6x [4.3x, 4.9x] | 4.6x [4.4x, 4.7x] | 8.7x [8.4x, 8.9x] | 4.2x [4.1x, 4.3x] | **1.0x** | 1.1x [1.0x, 1.1x] | 1.2x [1.1x, 1.3x] | 27.0x [25.8x, 28.2x] | 76.2x [74.2x, 78.2x] | 50.4x [49.0x, 51.8x] |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | 27.5x [26.4x, 28.7x] | 93.6x [92.2x, 94.9x] | 102.3x [100.6x, 104.1x] | 5.4x [3.7x, 7.0x] | 11.3x [10.7x, 11.9x] | 8.4x [8.2x, 8.7x] | 10.5x [10.2x, 10.9x] | 14.0x [13.6x, 14.3x] | 8.1x [8.0x, 8.3x] | **1.0x** | 10.5x [10.2x, 10.8x] | 134.4x [130.9x, 137.9x] | 131.3x [127.4x, 135.1x] | 95.9x [93.6x, 98.3x] |
| terminal-screen-transition (cols=120,rows=40) | 4.7x [4.6x, 4.8x] | 14.9x [14.7x, 15.1x] | 16.7x [16.5x, 17.0x] | **1.0x** | 1.2x [1.2x, 1.2x] | 1.4x [1.3x, 1.5x] | 2.4x [2.4x, 2.5x] | 2.7x [2.7x, 2.8x] | 1.4x [1.3x, 1.4x] | 1.8x [1.8x, 1.9x] | 1.6x [1.6x, 1.7x] | 11.8x [11.6x, 12.1x] | 17.4x [17.1x, 17.7x] | 13.8x [13.6x, 14.1x] |
| terminal-fps-stream (channels=12,cols=120,rows=40) | 14.7x [14.3x, 15.0x] | 91.7x [90.7x, 92.6x] | 94.4x [93.2x, 95.7x] | 26.3x [24.2x, 28.4x] | 5.7x [5.6x, 5.9x] | 7.1x [7.0x, 7.2x] | 19.2x [18.8x, 19.5x] | 7.9x [7.8x, 8.1x] | 1.5x [1.5x, 1.6x] | **1.0x** | 1.9x [1.9x, 2.0x] | 66.0x [64.9x, 67.1x] | 103.2x [101.4x, 104.9x] | 81.8x [80.4x, 83.3x] |
| terminal-input-latency (cols=120,rows=40) | 15.8x [15.4x, 16.2x] | 47.4x [46.7x, 48.0x] | 136.0x [134.2x, 137.7x] | 6.8x [5.5x, 8.0x] | 20.5x [19.3x, 21.8x] | 4.2x [4.1x, 4.3x] | 28.2x [27.0x, 29.4x] | 4.7x [4.6x, 4.8x] | 1.1x [1.0x, 1.1x] | 1.2x [1.2x, 1.3x] | **1.0x** | 55.4x [54.4x, 56.4x] | 110.3x [108.4x, 112.2x] | 84.5x [82.9x, 86.0x] |
| terminal-memory-soak (cols=120,rows=40) | 3.2x [3.1x, 3.2x] | 9.4x [9.3x, 9.5x] | 11.0x [10.8x, 11.1x] | 1.4x [1.1x, 1.6x] | 1.4x [1.3x, 1.5x] | **1.0x** | 2.7x [2.7x, 2.8x] | 2.7x [2.6x, 2.7x] | 1.1x [1.1x, 1.1x] | 1.4x [1.4x, 1.4x] | 1.2x [1.2x, 1.2x] | 13.9x [13.7x, 14.1x] | 14.7x [14.4x, 14.9x] | 10.9x [10.7x, 11.1x] |
| terminal-full-ui (cols=120,rows=40,services=24) | 20.6x [20.1x, 21.0x] | 74.4x [73.6x, 75.3x] | 79.1x [78.2x, 80.1x] | 63.3x [60.6x, 66.0x] | 18.5x [17.2x, 19.8x] | 12.0x [11.7x, 12.3x] | 21.0x [20.6x, 21.4x] | 19.7x [19.3x, 20.0x] | 2.2x [2.2x, 2.2x] | **1.0x** | 2.5x [2.5x, 2.5x] | 59.4x [58.3x, 60.6x] | 106.3x [104.5x, 108.1x] | 77.8x [76.5x, 79.1x] |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 15.4x [15.0x, 15.7x] | 56.0x [55.4x, 56.6x] | 68.0x [67.0x, 69.0x] | 44.9x [42.9x, 46.9x] | 14.2x [13.4x, 15.0x] | 7.8x [7.4x, 8.3x] | 18.0x [17.3x, 18.6x] | 16.7x [16.4x, 17.1x] | 2.2x [2.1x, 2.2x] | **1.0x** | 2.3x [2.3x, 2.4x] | 49.9x [49.0x, 50.7x] | 82.0x [80.8x, 83.2x] | 53.4x [52.5x, 54.4x] |
| terminal-strict-ui (cols=120,rows=40,services=24) | 20.9x [20.4x, 21.4x] | 70.4x [69.5x, 71.3x] | 78.6x [77.8x, 79.4x] | 55.5x [51.9x, 59.1x] | 18.8x [17.6x, 20.0x] | 10.8x [10.4x, 11.1x] | 18.9x [18.6x, 19.3x] | 36.4x [34.3x, 38.4x] | 2.4x [2.3x, 2.4x] | **1.0x** | 2.5x [2.5x, 2.5x] | 62.2x [61.1x, 63.3x] | 103.5x [101.7x, 105.2x] | 72.7x [71.5x, 74.0x] |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 18.8x [18.4x, 19.3x] | 64.7x [63.9x, 65.6x] | 85.9x [84.8x, 87.1x] | 53.0x [50.3x, 55.7x] | 17.6x [16.6x, 18.6x] | 10.1x [9.8x, 10.4x] | 16.7x [16.5x, 17.0x] | 33.9x [32.5x, 35.4x] | 2.4x [2.4x, 2.5x] | **1.0x** | 2.9x [2.8x, 2.9x] | 61.2x [59.9x, 62.4x] | 101.1x [99.5x, 102.8x] | 71.6x [70.4x, 72.7x] |
| terminal-virtual-list (items=100000,viewport=40) | 6.4x [6.0x, 6.7x] | 83.1x [82.1x, 84.1x] | 93.9x [92.7x, 95.2x] | 4.3x [3.6x, 4.9x] | 10.2x [9.4x, 11.0x] | 10.4x [10.1x, 10.7x] | 20.2x [19.7x, 20.7x] | 5.2x [5.1x, 5.4x] | 3.9x [3.8x, 3.9x] | **1.0x** | 4.3x [4.2x, 4.4x] | 31.3x [30.7x, 32.0x] | 74.5x [73.1x, 75.9x] | 45.7x [44.9x, 46.5x] |
| terminal-table (cols=8,rows=40) | 33.1x [31.9x, 34.2x] | 92.9x [91.3x, 94.5x] | 109.3x [107.8x, 110.8x] | 69.6x [65.8x, 73.4x] | 13.5x [12.5x, 14.5x] | 15.3x [14.7x, 15.8x] | 17.9x [17.4x, 18.5x] | 38.0x [36.9x, 39.1x] | 3.8x [3.7x, 3.9x] | **1.0x** | 7.8x [7.2x, 8.4x] | 65.8x [63.9x, 67.7x] | 129.1x [126.4x, 131.9x] | 86.9x [84.6x, 89.1x] |

## Memory Comparison

| Scenario | Framework | Peak RSS | RSS Before | RSS After | RSS Growth |
|---|---|---:|---:|---:|---:|
| startup | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| startup | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 4.0KB |
| startup | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 4.0KB |
| startup | bubbletea | 16.0MB | 10.5MB | 16.0MB | 5.5MB |
| startup | tuix-python | 18.2MB | 18.2MB | 18.2MB | 4.0KB |
| startup | rich | 31.5MB | 31.0MB | 31.5MB | 492.0KB |
| startup | opentui-core | 33.3MB | 32.9MB | 33.3MB | 412.0KB |
| startup | opentui-react | 35.9MB | 35.5MB | 35.9MB | 412.0KB |
| startup | prompt-toolkit | 36.2MB | 36.1MB | 36.2MB | 24.0KB |
| startup | urwid | 42.0MB | 42.0MB | 42.0MB | 24.0KB |
| startup | terminal-kit | 43.4MB | 43.4MB | 43.4MB | 28.0KB |
| startup | ink | 62.3MB | 59.6MB | 62.3MB | 2.7MB |
| startup | rezitui | 78.4MB | 65.0MB | 78.4MB | 13.4MB |
| startup | blessed | 149.5MB | 75.1MB | 149.5MB | 74.4MB |
| tree-construction (items=10) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=10) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=10) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tree-construction (items=10) | bubbletea | 16.6MB | 16.3MB | 16.6MB | 296.0KB |
| tree-construction (items=10) | tuix-python | 18.2MB | 18.2MB | 18.2MB | 16.0KB |
| tree-construction (items=10) | rich | 31.5MB | 31.5MB | 31.5MB | 24.0KB |
| tree-construction (items=10) | opentui-core | 33.3MB | 33.3MB | 33.3MB | 0.00KB |
| tree-construction (items=10) | opentui-react | 36.0MB | 35.9MB | 36.0MB | 44.0KB |
| tree-construction (items=10) | prompt-toolkit | 36.2MB | 36.2MB | 36.2MB | 32.0KB |
| tree-construction (items=10) | urwid | 42.1MB | 42.0MB | 42.1MB | 76.0KB |
| tree-construction (items=10) | terminal-kit | 43.7MB | 43.7MB | 43.7MB | 36.0KB |
| tree-construction (items=10) | ink | 66.1MB | 63.3MB | 66.1MB | 2.7MB |
| tree-construction (items=10) | rezitui | 98.1MB | 79.4MB | 98.1MB | 18.7MB |
| tree-construction (items=10) | blessed | 157.9MB | 151.5MB | 157.9MB | 6.4MB |
| tree-construction (items=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=100) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=100) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tree-construction (items=100) | bubbletea | 16.6MB | 16.6MB | 16.6MB | 16.0KB |
| tree-construction (items=100) | tuix-python | 18.3MB | 18.3MB | 18.3MB | 16.0KB |
| tree-construction (items=100) | rich | 31.6MB | 31.5MB | 31.6MB | 28.0KB |
| tree-construction (items=100) | opentui-core | 34.0MB | 33.3MB | 34.0MB | 736.0KB |
| tree-construction (items=100) | opentui-react | 36.1MB | 36.0MB | 36.1MB | 104.0KB |
| tree-construction (items=100) | prompt-toolkit | 36.3MB | 36.2MB | 36.3MB | 72.0KB |
| tree-construction (items=100) | urwid | 42.1MB | 42.1MB | 42.1MB | 16.0KB |
| tree-construction (items=100) | terminal-kit | 43.7MB | 43.6MB | 43.7MB | 80.0KB |
| tree-construction (items=100) | ink | 101.3MB | 82.1MB | 101.3MB | 19.2MB |
| tree-construction (items=100) | rezitui | 107.5MB | 96.2MB | 107.5MB | 11.3MB |
| tree-construction (items=100) | blessed | 240.9MB | 161.4MB | 240.9MB | 79.6MB |
| tree-construction (items=500) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=500) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 48.0KB |
| tree-construction (items=500) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tree-construction (items=500) | bubbletea | 16.6MB | 16.6MB | 16.6MB | 0.00KB |
| tree-construction (items=500) | tuix-python | 18.3MB | 18.3MB | 18.3MB | 16.0KB |
| tree-construction (items=500) | rich | 31.6MB | 31.6MB | 31.6MB | 12.0KB |
| tree-construction (items=500) | opentui-core | 34.3MB | 34.0MB | 34.3MB | 244.0KB |
| tree-construction (items=500) | opentui-react | 36.3MB | 36.1MB | 36.3MB | 208.0KB |
| tree-construction (items=500) | prompt-toolkit | 36.3MB | 36.3MB | 36.3MB | 32.0KB |
| tree-construction (items=500) | urwid | 42.1MB | 42.1MB | 42.1MB | 32.0KB |
| tree-construction (items=500) | terminal-kit | 43.6MB | 43.7MB | 43.6MB | 0.00KB |
| tree-construction (items=500) | rezitui | 166.2MB | 110.7MB | 166.2MB | 55.6MB |
| tree-construction (items=500) | ink | 194.4MB | 103.7MB | 194.4MB | 90.7MB |
| tree-construction (items=500) | blessed | 450.9MB | 257.6MB | 450.9MB | 193.3MB |
| tree-construction (items=1000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=1000) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=1000) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tree-construction (items=1000) | bubbletea | 16.6MB | 16.6MB | 16.6MB | 0.00KB |
| tree-construction (items=1000) | tuix-python | 18.4MB | 18.3MB | 18.4MB | 16.0KB |
| tree-construction (items=1000) | rich | 31.6MB | 31.6MB | 31.6MB | 12.0KB |
| tree-construction (items=1000) | opentui-core | 34.8MB | 34.6MB | 34.8MB | 236.0KB |
| tree-construction (items=1000) | prompt-toolkit | 36.4MB | 36.3MB | 36.4MB | 116.0KB |
| tree-construction (items=1000) | opentui-react | 38.0MB | 36.5MB | 38.0MB | 1.5MB |
| tree-construction (items=1000) | urwid | 42.1MB | 42.1MB | 42.1MB | 16.0KB |
| tree-construction (items=1000) | terminal-kit | 51.8MB | 43.7MB | 51.8MB | 8.1MB |
| tree-construction (items=1000) | rezitui | 184.0MB | 166.4MB | 184.0MB | 17.6MB |
| tree-construction (items=1000) | ink | 347.0MB | 206.6MB | 347.0MB | 140.4MB |
| tree-construction (items=1000) | blessed | 516.7MB | 451.7MB | 516.7MB | 65.0MB |
| rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| rerender | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| rerender | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| rerender | bubbletea | 16.6MB | 16.6MB | 16.6MB | 0.00KB |
| rerender | tuix-python | 18.4MB | 18.4MB | 18.4MB | 32.0KB |
| rerender | rich | 31.6MB | 31.6MB | 31.6MB | 32.0KB |
| rerender | opentui-core | 34.8MB | 34.8MB | 34.8MB | 0.00KB |
| rerender | prompt-toolkit | 36.6MB | 36.4MB | 36.6MB | 160.0KB |
| rerender | opentui-react | 38.0MB | 38.0MB | 38.0MB | 0.00KB |
| rerender | urwid | 42.2MB | 42.1MB | 42.2MB | 68.0KB |
| rerender | terminal-kit | 52.0MB | 51.8MB | 52.0MB | 112.0KB |
| rerender | rezitui | 191.6MB | 184.0MB | 191.6MB | 7.6MB |
| rerender | ink | 346.5MB | 346.9MB | 346.5MB | 0.00KB |
| rerender | blessed | 516.4MB | 516.7MB | 516.4MB | 0.00KB |
| content-update | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| content-update | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| content-update | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| content-update | bubbletea | 16.6MB | 16.6MB | 16.6MB | 8.0KB |
| content-update | tuix-python | 18.5MB | 18.4MB | 18.5MB | 72.0KB |
| content-update | rich | 32.1MB | 31.7MB | 32.1MB | 420.0KB |
| content-update | opentui-core | 34.8MB | 34.8MB | 34.8MB | 0.00KB |
| content-update | prompt-toolkit | 36.6MB | 36.6MB | 36.6MB | 72.0KB |
| content-update | opentui-react | 38.9MB | 38.0MB | 38.9MB | 880.0KB |
| content-update | urwid | 42.2MB | 42.2MB | 42.2MB | 0.00KB |
| content-update | terminal-kit | 52.1MB | 52.0MB | 52.1MB | 56.0KB |
| content-update | rezitui | 184.9MB | 190.0MB | 184.9MB | 0.00KB |
| content-update | ink | 346.9MB | 346.7MB | 346.9MB | 160.0KB |
| content-update | blessed | 516.9MB | 518.5MB | 516.9MB | 0.00KB |
| layout-stress (cols=4,rows=10) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| layout-stress (cols=4,rows=10) | bubbletea | 16.6MB | 16.6MB | 16.6MB | 0.00KB |
| layout-stress (cols=4,rows=10) | tuix-python | 18.5MB | 18.5MB | 18.5MB | 0.00KB |
| layout-stress (cols=4,rows=10) | rich | 32.1MB | 32.1MB | 32.1MB | 12.0KB |
| layout-stress (cols=4,rows=10) | opentui-core | 34.8MB | 34.8MB | 34.8MB | 0.00KB |
| layout-stress (cols=4,rows=10) | prompt-toolkit | 36.7MB | 36.6MB | 36.7MB | 56.0KB |
| layout-stress (cols=4,rows=10) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| layout-stress (cols=4,rows=10) | urwid | 42.2MB | 42.2MB | 42.2MB | 0.00KB |
| layout-stress (cols=4,rows=10) | terminal-kit | 52.1MB | 52.1MB | 52.1MB | 0.00KB |
| layout-stress (cols=4,rows=10) | rezitui | 187.1MB | 185.1MB | 187.1MB | 2.0MB |
| layout-stress (cols=4,rows=10) | ink | 347.3MB | 346.6MB | 347.3MB | 640.0KB |
| layout-stress (cols=4,rows=10) | blessed | 517.0MB | 520.0MB | 517.0MB | 0.00KB |
| scroll-stress (items=2000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| scroll-stress (items=2000) | bubbletea | 16.6MB | 16.6MB | 16.6MB | 0.00KB |
| scroll-stress (items=2000) | tuix-python | 18.5MB | 18.5MB | 18.5MB | 0.00KB |
| scroll-stress (items=2000) | rich | 32.2MB | 32.1MB | 32.2MB | 80.0KB |
| scroll-stress (items=2000) | opentui-core | 34.9MB | 34.8MB | 34.9MB | 100.0KB |
| scroll-stress (items=2000) | prompt-toolkit | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| scroll-stress (items=2000) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| scroll-stress (items=2000) | urwid | 42.2MB | 42.2MB | 42.2MB | 0.00KB |
| scroll-stress (items=2000) | terminal-kit | 52.1MB | 52.1MB | 52.1MB | 0.00KB |
| scroll-stress (items=2000) | rezitui | 187.2MB | 187.1MB | 187.2MB | 56.0KB |
| scroll-stress (items=2000) | ink | 347.3MB | 347.3MB | 347.3MB | 4.0KB |
| scroll-stress (items=2000) | blessed | 516.9MB | 517.1MB | 516.9MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | tuix-python | 18.5MB | 18.5MB | 18.5MB | 4.0KB |
| virtual-list (items=100000,viewport=40) | bubbletea | 20.8MB | 20.6MB | 20.8MB | 256.0KB |
| virtual-list (items=100000,viewport=40) | rich | 32.5MB | 32.2MB | 32.5MB | 308.0KB |
| virtual-list (items=100000,viewport=40) | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | prompt-toolkit | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | urwid | 42.3MB | 42.2MB | 42.3MB | 64.0KB |
| virtual-list (items=100000,viewport=40) | terminal-kit | 93.4MB | 89.0MB | 93.4MB | 4.3MB |
| virtual-list (items=100000,viewport=40) | rezitui | 189.4MB | 187.4MB | 189.4MB | 2.0MB |
| virtual-list (items=100000,viewport=40) | ink | 347.4MB | 347.4MB | 347.4MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | blessed | 520.0MB | 519.9MB | 520.0MB | 148.0KB |
| tables (cols=8,rows=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| tables (cols=8,rows=100) | tuix-python | 18.5MB | 18.5MB | 18.5MB | 0.00KB |
| tables (cols=8,rows=100) | bubbletea | 20.8MB | 20.8MB | 20.8MB | 0.00KB |
| tables (cols=8,rows=100) | rich | 32.5MB | 32.5MB | 32.5MB | 8.0KB |
| tables (cols=8,rows=100) | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| tables (cols=8,rows=100) | prompt-toolkit | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| tables (cols=8,rows=100) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| tables (cols=8,rows=100) | urwid | 42.3MB | 42.3MB | 42.3MB | 0.00KB |
| tables (cols=8,rows=100) | terminal-kit | 93.3MB | 93.3MB | 93.3MB | 0.00KB |
| tables (cols=8,rows=100) | rezitui | 187.5MB | 188.6MB | 187.5MB | 0.00KB |
| tables (cols=8,rows=100) | ink | 348.0MB | 348.0MB | 348.0MB | 0.00KB |
| tables (cols=8,rows=100) | blessed | 519.5MB | 519.4MB | 519.5MB | 68.0KB |
| memory-profile | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| memory-profile | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| memory-profile | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| memory-profile | tuix-python | 18.6MB | 18.5MB | 18.6MB | 60.0KB |
| memory-profile | bubbletea | 20.8MB | 20.8MB | 20.8MB | 0.00KB |
| memory-profile | rich | 32.4MB | 32.5MB | 32.4MB | 0.00KB |
| memory-profile | opentui-core | 34.9MB | 34.9MB | 34.9MB | 36.0KB |
| memory-profile | prompt-toolkit | 36.8MB | 36.7MB | 36.8MB | 84.0KB |
| memory-profile | opentui-react | 39.1MB | 38.9MB | 39.1MB | 228.0KB |
| memory-profile | urwid | 42.3MB | 42.3MB | 42.3MB | 64.0KB |
| memory-profile | terminal-kit | 93.3MB | 93.3MB | 93.3MB | 44.0KB |
| memory-profile | rezitui | 187.8MB | 187.8MB | 187.8MB | 76.0KB |
| memory-profile | ink | 348.0MB | 348.0MB | 348.0MB | 12.0KB |
| memory-profile | blessed | 519.3MB | 519.6MB | 519.3MB | 0.00KB |
| terminal-rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-rerender | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| terminal-rerender | TUIX.Core | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-rerender | tuix-python | 18.7MB | 18.6MB | 18.7MB | 68.0KB |
| terminal-rerender | bubbletea | 24.8MB | 20.8MB | 24.8MB | 4.0MB |
| terminal-rerender | rich | 32.2MB | 32.1MB | 32.2MB | 84.0KB |
| terminal-rerender | opentui-core | 35.7MB | 35.7MB | 35.7MB | 24.0KB |
| terminal-rerender | prompt-toolkit | 37.0MB | 37.0MB | 37.0MB | 0.00KB |
| terminal-rerender | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| terminal-rerender | urwid | 42.4MB | 42.3MB | 42.4MB | 64.0KB |
| terminal-rerender | terminal-kit | 93.4MB | 93.4MB | 93.4MB | 8.0KB |
| terminal-rerender | rezitui | 187.3MB | 187.8MB | 187.3MB | 0.00KB |
| terminal-rerender | ink | 347.6MB | 348.0MB | 347.6MB | 0.00KB |
| terminal-rerender | blessed | 519.3MB | 519.3MB | 519.3MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | TUIX.Renderer | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | TUIX.Core | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | tuix-python | 18.7MB | 18.7MB | 18.7MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | rich | 32.4MB | 32.2MB | 32.4MB | 172.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | prompt-toolkit | 37.1MB | 37.0MB | 37.1MB | 104.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | urwid | 42.4MB | 42.4MB | 42.4MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | terminal-kit | 93.2MB | 93.4MB | 93.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | rezitui | 188.9MB | 187.3MB | 188.9MB | 1.6MB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ink | 348.1MB | 347.7MB | 348.1MB | 384.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | blessed | 519.6MB | 519.6MB | 519.6MB | 8.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | TUIX.Core | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | tuix-python | 18.7MB | 18.7MB | 18.7MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | rich | 33.0MB | 32.7MB | 33.0MB | 308.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | prompt-toolkit | 37.0MB | 37.1MB | 37.0MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | urwid | 42.4MB | 42.4MB | 42.4MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | terminal-kit | 93.3MB | 93.3MB | 93.3MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | rezitui | 200.4MB | 188.9MB | 200.4MB | 11.5MB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ink | 348.1MB | 348.1MB | 348.1MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | blessed | 520.0MB | 519.7MB | 520.0MB | 312.0KB |
| terminal-screen-transition (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | tuix-python | 18.8MB | 18.7MB | 18.8MB | 64.0KB |
| terminal-screen-transition (cols=120,rows=40) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | rich | 33.0MB | 33.0MB | 33.0MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | prompt-toolkit | 37.1MB | 37.0MB | 37.1MB | 68.0KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | urwid | 42.4MB | 42.4MB | 42.4MB | 8.0KB |
| terminal-screen-transition (cols=120,rows=40) | terminal-kit | 93.4MB | 93.4MB | 93.4MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | rezitui | 206.7MB | 200.7MB | 206.7MB | 6.1MB |
| terminal-screen-transition (cols=120,rows=40) | ink | 348.0MB | 347.9MB | 348.0MB | 28.0KB |
| terminal-screen-transition (cols=120,rows=40) | blessed | 519.8MB | 519.8MB | 519.8MB | 4.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | tuix-python | 18.8MB | 18.8MB | 18.8MB | 16.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | rich | 33.0MB | 33.0MB | 33.0MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | prompt-toolkit | 37.1MB | 37.1MB | 37.1MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | urwid | 42.5MB | 42.4MB | 42.5MB | 64.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | terminal-kit | 93.4MB | 93.6MB | 93.4MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | rezitui | 207.6MB | 206.5MB | 207.6MB | 1.2MB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ink | 348.2MB | 348.2MB | 348.2MB | 8.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | blessed | 519.6MB | 519.6MB | 519.6MB | 12.0KB |
| terminal-input-latency (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | tuix-python | 18.9MB | 18.8MB | 18.9MB | 32.0KB |
| terminal-input-latency (cols=120,rows=40) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | rich | 33.4MB | 33.0MB | 33.4MB | 412.0KB |
| terminal-input-latency (cols=120,rows=40) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | prompt-toolkit | 37.4MB | 37.2MB | 37.4MB | 224.0KB |
| terminal-input-latency (cols=120,rows=40) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | urwid | 42.5MB | 42.5MB | 42.5MB | 36.0KB |
| terminal-input-latency (cols=120,rows=40) | terminal-kit | 93.4MB | 93.4MB | 93.4MB | 24.0KB |
| terminal-input-latency (cols=120,rows=40) | rezitui | 208.4MB | 207.6MB | 208.4MB | 824.0KB |
| terminal-input-latency (cols=120,rows=40) | ink | 348.3MB | 348.3MB | 348.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | blessed | 520.0MB | 519.6MB | 520.0MB | 336.0KB |
| terminal-memory-soak (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Core | 4.6MB | 4.5MB | 4.6MB | 124.0KB |
| terminal-memory-soak (cols=120,rows=40) | tuix-python | 19.0MB | 18.9MB | 19.0MB | 80.0KB |
| terminal-memory-soak (cols=120,rows=40) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 8.0KB |
| terminal-memory-soak (cols=120,rows=40) | rich | 33.4MB | 33.1MB | 33.4MB | 360.0KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | prompt-toolkit | 37.2MB | 37.4MB | 37.2MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | urwid | 42.5MB | 42.5MB | 42.5MB | 20.0KB |
| terminal-memory-soak (cols=120,rows=40) | terminal-kit | 93.5MB | 93.5MB | 93.5MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | rezitui | 208.4MB | 208.4MB | 208.4MB | 8.0KB |
| terminal-memory-soak (cols=120,rows=40) | ink | 348.4MB | 348.3MB | 348.4MB | 40.0KB |
| terminal-memory-soak (cols=120,rows=40) | blessed | 520.0MB | 520.0MB | 520.0MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | tuix-python | 19.0MB | 19.0MB | 19.0MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | rich | 33.7MB | 33.4MB | 33.7MB | 212.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | prompt-toolkit | 37.2MB | 37.2MB | 37.2MB | 16.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | urwid | 42.6MB | 42.5MB | 42.6MB | 48.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | terminal-kit | 93.9MB | 93.9MB | 93.9MB | 8.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | rezitui | 208.3MB | 210.2MB | 208.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | ink | 348.7MB | 348.5MB | 348.7MB | 184.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | blessed | 520.1MB | 520.1MB | 520.1MB | 24.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | tuix-python | 19.0MB | 19.0MB | 19.0MB | 32.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rich | 33.7MB | 33.7MB | 33.7MB | 76.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | prompt-toolkit | 37.6MB | 37.2MB | 37.6MB | 456.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 39.1MB | 39.1MB | 39.1MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | urwid | 42.6MB | 42.6MB | 42.6MB | 40.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 94.0MB | 93.9MB | 94.0MB | 48.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 210.8MB | 207.6MB | 210.8MB | 3.2MB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 349.0MB | 349.0MB | 349.0MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 519.5MB | 519.2MB | 519.5MB | 300.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | tuix-python | 19.2MB | 19.1MB | 19.2MB | 96.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | rich | 33.8MB | 33.7MB | 33.8MB | 36.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | prompt-toolkit | 37.3MB | 37.6MB | 37.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | urwid | 42.7MB | 42.6MB | 42.7MB | 84.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-react | 56.6MB | 39.1MB | 56.6MB | 17.5MB |
| terminal-strict-ui (cols=120,rows=40,services=24) | terminal-kit | 94.0MB | 94.0MB | 94.0MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | rezitui | 210.8MB | 210.8MB | 210.8MB | 4.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ink | 349.0MB | 349.0MB | 349.0MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | blessed | 519.7MB | 519.6MB | 519.7MB | 68.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | tuix-python | 19.1MB | 19.1MB | 19.1MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rich | 33.8MB | 33.8MB | 33.8MB | 36.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | prompt-toolkit | 37.6MB | 37.4MB | 37.6MB | 228.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | urwid | 42.8MB | 42.7MB | 42.8MB | 64.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 58.0MB | 56.6MB | 58.0MB | 1.4MB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 94.1MB | 94.1MB | 94.1MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 210.8MB | 210.8MB | 210.8MB | 20.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 349.0MB | 349.0MB | 349.0MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 519.8MB | 519.7MB | 519.8MB | 48.0KB |
| terminal-virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | tuix-python | 19.3MB | 19.2MB | 19.3MB | 64.0KB |
| terminal-virtual-list (items=100000,viewport=40) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | rich | 33.9MB | 33.8MB | 33.9MB | 36.0KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | prompt-toolkit | 37.6MB | 37.6MB | 37.6MB | 8.0KB |
| terminal-virtual-list (items=100000,viewport=40) | urwid | 42.8MB | 42.8MB | 42.8MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-react | 58.0MB | 58.0MB | 58.0MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | terminal-kit | 125.7MB | 125.3MB | 125.7MB | 416.0KB |
| terminal-virtual-list (items=100000,viewport=40) | rezitui | 210.8MB | 210.8MB | 210.8MB | 4.0KB |
| terminal-virtual-list (items=100000,viewport=40) | ink | 350.2MB | 350.2MB | 350.2MB | 4.0KB |
| terminal-virtual-list (items=100000,viewport=40) | blessed | 520.8MB | 520.8MB | 520.8MB | 68.0KB |
| terminal-table (cols=8,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Renderer | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-table (cols=8,rows=40) | tuix-python | 19.2MB | 19.2MB | 19.2MB | 0.00KB |
| terminal-table (cols=8,rows=40) | bubbletea | 24.8MB | 24.8MB | 24.8MB | 0.00KB |
| terminal-table (cols=8,rows=40) | rich | 33.4MB | 33.4MB | 33.4MB | 0.00KB |
| terminal-table (cols=8,rows=40) | opentui-core | 36.7MB | 36.7MB | 36.7MB | 0.00KB |
| terminal-table (cols=8,rows=40) | prompt-toolkit | 37.4MB | 37.6MB | 37.4MB | 0.00KB |
| terminal-table (cols=8,rows=40) | urwid | 42.8MB | 42.8MB | 42.8MB | 0.00KB |
| terminal-table (cols=8,rows=40) | opentui-react | 58.0MB | 58.0MB | 58.0MB | 0.00KB |
| terminal-table (cols=8,rows=40) | terminal-kit | 126.0MB | 126.0MB | 126.0MB | 4.0KB |
| terminal-table (cols=8,rows=40) | rezitui | 212.6MB | 210.9MB | 212.6MB | 1.8MB |
| terminal-table (cols=8,rows=40) | ink | 349.5MB | 349.5MB | 349.5MB | 0.00KB |
| terminal-table (cols=8,rows=40) | blessed | 520.6MB | 520.6MB | 520.6MB | 8.0KB |

