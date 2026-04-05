# Benchmark Results

> 2026-04-05T22:19:42.505Z | Node v22.19.0 | Windows_NT 10.0.26200 | win32 x64 | 12th Gen Intel(R) Core(TM) i5-12450HX (12 cores) | RAM 24352MB
> Libraries tested: blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React, TUIX.Renderer, TUIX.Core, TUIX.Python, Rich, Urwid, PromptToolkit
> Byte columns: "Bytes(local)" = bytes produced per frame (framework counter); "Bytes(pty)" = observed PTY bytes (cross-framework comparable).

## startup

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 195µs | 26.3% | 185µs–205µs | 5.1K ops/s | 21.49ms | 27.0KB | 0.00KB | 1.6MB | 16.0KB |
| rezitui | 1 | 197µs | 164.8% | 133µs–261µs | 5.1K ops/s | 19.83ms | 81.7MB | 13.9MB | 133.7KB | 1.3KB |
| tuix-python | 1 | 229µs | 39.9% | 211µs–247µs | 4.4K ops/s | 22.95ms | 18.4MB | 52.0KB | 6.7MB | 68.3KB |
| TUIX.Core | 1 | 257µs | 22.3% | 246µs–269µs | 3.9K ops/s | 25.75ms | 4.0MB | 4.0KB | 6.0MB | 61.7KB |
| TUIX.Renderer | 1 | 258µs | 37.8% | 239µs–277µs | 3.9K ops/s | 25.80ms | 3.9MB | 4.0KB | 6.3MB | 64.4KB |
| bubbletea | 1 | 279µs | 50.7% | 251µs–307µs | 3.6K ops/s | 27.94ms | 11.7MB | 904.0KB | 1.6MB | 16.0KB |
| opentui-react | 1 | 385µs | 47.2% | 350µs–421µs | 2.6K ops/s | 38.57ms | 35.8MB | 416.0KB | 7.4MB | 76.0KB |
| terminal-kit | 1 | 527µs | 19.4% | 507µs–547µs | 1.9K ops/s | 52.88ms | 46.4MB | 0.00KB | 190.0KB | 1.9KB |
| opentui-core | 1 | 755µs | 42.1% | 693µs–818µs | 1.3K ops/s | 75.62ms | 33.3MB | 412.0KB | 6.7MB | 68.5KB |
| prompt-toolkit | 1 | 1.09ms | 29.6% | 1.03ms–1.15ms | 918 ops/s | 108.99ms | 36.1MB | 16.0KB | 189.7KB | 1.9KB |
| rich | 1 | 1.42ms | 18.4% | 1.37ms–1.47ms | 706 ops/s | 141.75ms | 32.0MB | 620.0KB | 189.7KB | 1.9KB |
| urwid | 1 | 1.69ms | 32.2% | 1.58ms–1.79ms | 593 ops/s | 168.58ms | 41.8MB | 24.0KB | 187.5KB | 1.9KB |
| ink | 1 | 2.51ms | 28.8% | 2.37ms–2.65ms | 398 ops/s | 251.31ms | 65.0MB | 6.7MB | 189.7KB | 1.9KB |
| blessed | 1 | 25.64ms | 16.4% | 24.81ms–26.46ms | 39 ops/s | 2.56s | 152.1MB | 74.5MB | 229.2KB | 2.3KB |

## tree-construction (items=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 54µs | 16.0% | 53µs–54µs | 18.7K ops/s | 26.82ms | 4.0MB | 0.00KB | 555.7KB | 1.1KB |
| opentui-react | 1 | 55µs | 31.3% | 54µs–57µs | 18.0K ops/s | 28.45ms | 35.9MB | 92.0KB | 3.6MB | 7.5KB |
| TUIX.Renderer | 1 | 66µs | 20.2% | 65µs–67µs | 15.1K ops/s | 33.14ms | 3.9MB | 0.00KB | 1.1MB | 2.3KB |
| blessed | 1 | 92µs | 275.4% | 70µs–114µs | 10.9K ops/s | 46.29ms | 161.1MB | 6.9MB | 126.0KB | 258B |
| ratatui | 1 | 96µs | 37.0% | 93µs–99µs | 10.5K ops/s | 47.88ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| rezitui | 1 | 98µs | 259.8% | 76µs–121µs | 10.2K ops/s | 49.39ms | 100.4MB | 17.1MB | 668.5KB | 1.3KB |
| tuix-python | 1 | 104µs | 35.0% | 101µs–108µs | 9.6K ops/s | 52.62ms | 18.5MB | 52.0KB | 7.7MB | 15.7KB |
| bubbletea | 1 | 159µs | 40.4% | 153µs–165µs | 6.3K ops/s | 79.59ms | 16.2MB | 4.5MB | 7.8MB | 16.0KB |
| terminal-kit | 1 | 286µs | 21.7% | 281µs–292µs | 3.5K ops/s | 143.33ms | 46.7MB | 32.0KB | 950.2KB | 1.9KB |
| opentui-core | 1 | 304µs | 85.0% | 282µs–327µs | 3.3K ops/s | 157.03ms | 34.2MB | 944.0KB | 33.4MB | 68.3KB |
| ink | 1 | 510µs | 46.1% | 489µs–530µs | 2.0K ops/s | 255.13ms | 69.1MB | 3.1MB | 23.9KB | 49B |
| rich | 1 | 783µs | 26.1% | 765µs–801µs | 1.3K ops/s | 391.71ms | 32.1MB | 28.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 847µs | 19.7% | 832µs–861µs | 1.2K ops/s | 423.47ms | 36.1MB | 16.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.25ms | 28.1% | 1.22ms–1.28ms | 801 ops/s | 624.83ms | 41.9MB | 76.0KB | 937.5KB | 1.9KB |

## tree-construction (items=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 92µs | 26.4% | 90µs–94µs | 10.9K ops/s | 45.98ms | 4.0MB | 0.00KB | 1.2MB | 2.5KB |
| TUIX.Renderer | 1 | 96µs | 42.8% | 93µs–100µs | 10.4K ops/s | 48.20ms | 3.9MB | 0.00KB | 1.3MB | 2.7KB |
| rezitui | 1 | 111µs | 158.1% | 96µs–127µs | 9.0K ops/s | 55.87ms | 110.6MB | 11.8MB | 668.5KB | 1.3KB |
| opentui-react | 1 | 140µs | 32.2% | 136µs–144µs | 7.1K ops/s | 77.10ms | 36.1MB | 104.0KB | 8.9MB | 18.3KB |
| opentui-core | 1 | 148µs | 194.8% | 123µs–173µs | 6.8K ops/s | 108.91ms | 34.9MB | 440.0KB | 33.8MB | 69.2KB |
| tuix-python | 1 | 180µs | 46.6% | 173µs–188µs | 5.5K ops/s | 90.65ms | 18.6MB | 16.0KB | 17.2MB | 35.2KB |
| ratatui | 1 | 185µs | 31.1% | 180µs–190µs | 5.4K ops/s | 92.76ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| bubbletea | 1 | 205µs | 31.0% | 200µs–211µs | 4.9K ops/s | 102.76ms | 16.2MB | 8.0KB | 7.8MB | 16.0KB |
| blessed | 1 | 515µs | 126.0% | 458µs–572µs | 1.9K ops/s | 257.99ms | 244.4MB | 79.3MB | 490.2KB | 1004B |
| rich | 1 | 815µs | 27.2% | 796µs–835µs | 1.2K ops/s | 407.92ms | 32.1MB | 16.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 914µs | 33.0% | 888µs–941µs | 1.1K ops/s | 457.20ms | 36.1MB | 16.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.16ms | 25.4% | 1.14ms–1.19ms | 860 ops/s | 581.76ms | 41.9MB | 20.0KB | 937.5KB | 1.9KB |
| terminal-kit | 1 | 1.42ms | 26.4% | 1.39ms–1.46ms | 702 ops/s | 712.53ms | 46.5MB | 0.00KB | 950.2KB | 1.9KB |
| ink | 1 | 4.56ms | 28.1% | 4.45ms–4.67ms | 219 ops/s | 2.28s | 106.4MB | 32.7MB | 287.6KB | 589B |

## tree-construction (items=500)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 58µs | 28.4% | 56µs–59µs | 17.3K ops/s | 28.85ms | 3.9MB | 48.0KB | 1.3MB | 2.7KB |
| TUIX.Core | 1 | 110µs | 34.7% | 107µs–114µs | 9.1K ops/s | 55.25ms | 4.0MB | 0.00KB | 1.2MB | 2.5KB |
| tuix-python | 1 | 127µs | 26.3% | 124µs–130µs | 7.9K ops/s | 63.67ms | 18.6MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-core | 1 | 145µs | 62.6% | 137µs–153µs | 6.9K ops/s | 252.87ms | 34.9MB | 0.00KB | 33.7MB | 68.9KB |
| rezitui | 1 | 324µs | 288.9% | 242µs–406µs | 3.1K ops/s | 162.27ms | 167.8MB | 54.6MB | 668.5KB | 1.3KB |
| opentui-react | 1 | 325µs | 32.5% | 315µs–334µs | 3.1K ops/s | 211.48ms | 37.1MB | 1.0MB | 8.9MB | 18.2KB |
| ratatui | 1 | 508µs | 5.2% | 506µs–510µs | 2.0K ops/s | 253.99ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| bubbletea | 1 | 723µs | 26.8% | 706µs–740µs | 1.4K ops/s | 361.54ms | 16.2MB | 0.00KB | 7.8MB | 16.0KB |
| rich | 1 | 861µs | 27.6% | 841µs–882µs | 1.2K ops/s | 431.00ms | 32.1MB | 12.0KB | 948.7KB | 1.9KB |
| prompt-toolkit | 1 | 964µs | 26.5% | 942µs–987µs | 1.0K ops/s | 482.34ms | 36.1MB | 16.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.30ms | 30.1% | 1.26ms–1.33ms | 770 ops/s | 649.27ms | 42.0MB | 32.0KB | 937.5KB | 1.9KB |
| blessed | 1 | 2.09ms | 61.2% | 1.98ms–2.20ms | 478 ops/s | 1.05s | 454.1MB | 193.2MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 6.45ms | 26.6% | 6.30ms–6.60ms | 155 ops/s | 3.23s | 46.8MB | 308.0KB | 950.2KB | 1.9KB |
| ink | 1 | 22.20ms | 21.4% | 21.78ms–22.62ms | 45 ops/s | 11.10s | 188.1MB | 81.2MB | 1.6MB | 3.3KB |

## tree-construction (items=1000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 72µs | 28.4% | 70µs–74µs | 13.9K ops/s | 35.97ms | 4.0MB | 48.0KB | 1.2MB | 2.5KB |
| TUIX.Renderer | 1 | 84µs | 45.5% | 80µs–87µs | 12.0K ops/s | 41.84ms | 3.9MB | 48.0KB | 1.3MB | 2.7KB |
| opentui-core | 1 | 97µs | 43.3% | 93µs–100µs | 10.4K ops/s | 400.25ms | 35.0MB | 96.0KB | 33.8MB | 69.3KB |
| tuix-python | 1 | 125µs | 15.4% | 123µs–126µs | 8.0K ops/s | 62.56ms | 18.7MB | 72.0KB | 17.2MB | 35.2KB |
| opentui-react | 1 | 278µs | 166.5% | 238µs–319µs | 3.6K ops/s | 179.38ms | 38.0MB | 848.0KB | 8.9MB | 18.2KB |
| rezitui | 1 | 473µs | 199.5% | 390µs–555µs | 2.1K ops/s | 236.39ms | 185.9MB | 18.1MB | 668.5KB | 1.3KB |
| ratatui | 1 | 986µs | 12.4% | 975µs–997µs | 1.0K ops/s | 492.93ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| prompt-toolkit | 1 | 1.00ms | 29.1% | 977µs–1.03ms | 998 ops/s | 501.35ms | 36.2MB | 16.0KB | 948.7KB | 1.9KB |
| rich | 1 | 1.03ms | 26.5% | 1.00ms–1.05ms | 974 ops/s | 513.53ms | 32.1MB | 16.0KB | 948.7KB | 1.9KB |
| bubbletea | 1 | 1.05ms | 26.3% | 1.02ms–1.07ms | 955 ops/s | 523.51ms | 16.2MB | 0.00KB | 7.8MB | 16.0KB |
| urwid | 1 | 1.30ms | 26.5% | 1.27ms–1.33ms | 768 ops/s | 651.48ms | 42.0MB | 16.0KB | 937.5KB | 1.9KB |
| blessed | 1 | 3.93ms | 40.3% | 3.79ms–4.06ms | 255 ops/s | 1.96s | 521.4MB | 67.6MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 12.54ms | 22.1% | 12.30ms–12.79ms | 80 ops/s | 6.27s | 55.1MB | 8.2MB | 950.2KB | 1.9KB |
| ink | 1 | 42.86ms | 20.0% | 42.11ms–43.61ms | 23 ops/s | 21.43s | 355.0MB | 136.3MB | 3.3MB | 6.7KB |

## rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 10µs | 14.5% | 10µs–11µs | 95.9K ops/s | 10.45ms | 3.9MB | 0.00KB | 15.6KB | 16B |
| tuix-python | 1 | 13µs | 32.7% | 12µs–13µs | 79.9K ops/s | 13.06ms | 18.7MB | 0.00KB | 1.8MB | 1.9KB |
| TUIX.Core | 1 | 23µs | 45.9% | 22µs–23µs | 44.0K ops/s | 22.76ms | 4.1MB | 0.00KB | 15.6KB | 16B |
| blessed | 1 | 26µs | 27.3% | 25µs–26µs | 39.1K ops/s | 25.69ms | 520.5MB | 0.00KB | 0.00KB | 0.00KB |
| opentui-react | 1 | 40µs | 43.9% | 39µs–42µs | 24.8K ops/s | 40.49ms | 38.0MB | 44.0KB | 3.9KB | 4B |
| rezitui | 1 | 43µs | 416.3% | 32µs–54µs | 23.3K ops/s | 43.12ms | 191.9MB | 6.0MB | 1.3MB | 1.3KB |
| ratatui | 1 | 57µs | 36.7% | 56µs–58µs | 17.6K ops/s | 56.90ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| bubbletea | 1 | 91µs | 58.5% | 88µs–94µs | 11.0K ops/s | 91.39ms | 16.2MB | 0.00KB | 15.6MB | 16.0KB |
| opentui-core | 1 | 133µs | 51.3% | 129µs–137µs | 7.5K ops/s | 133.37ms | 35.0MB | 0.00KB | 66.9MB | 68.5KB |
| terminal-kit | 1 | 213µs | 35.2% | 208µs–218µs | 4.7K ops/s | 213.11ms | 54.9MB | 0.00KB | 1.9MB | 1.9KB |
| ink | 1 | 647µs | 51.1% | 627µs–668µs | 1.5K ops/s | 647.60ms | 354.9MB | 208.0KB | 1.9MB | 1.9KB |
| prompt-toolkit | 1 | 1.07ms | 26.8% | 1.05ms–1.09ms | 933 ops/s | 1.07s | 36.2MB | 32.0KB | 1.9MB | 1.9KB |
| rich | 1 | 1.29ms | 29.1% | 1.26ms–1.31ms | 778 ops/s | 1.29s | 32.1MB | 32.0KB | 1.9MB | 1.9KB |
| urwid | 1 | 1.46ms | 26.4% | 1.44ms–1.48ms | 685 ops/s | 1.46s | 42.1MB | 72.0KB | 1.8MB | 1.9KB |

## content-update

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 25µs | 22.1% | 25µs–26µs | 39.5K ops/s | 12.67ms | 4.1MB | 0.00KB | 6.5MB | 13.4KB |
| rezitui | 1 | 62µs | 302.3% | 46µs–79µs | 16.1K ops/s | 31.17ms | 188.0MB | 0.00KB | 668.5KB | 1.3KB |
| ratatui | 1 | 84µs | 32.6% | 81µs–86µs | 11.9K ops/s | 41.96ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| tuix-python | 1 | 110µs | 37.2% | 106µs–113µs | 9.1K ops/s | 63.16ms | 18.7MB | 0.00KB | 17.1MB | 35.0KB |
| TUIX.Renderer | 1 | 120µs | 11.6% | 119µs–121µs | 8.3K ops/s | 68.71ms | 3.9MB | 0.00KB | 5.8MB | 11.9KB |
| opentui-react | 1 | 134µs | 61.3% | 127µs–142µs | 7.4K ops/s | 72.43ms | 38.9MB | 900.0KB | 15.3MB | 31.3KB |
| opentui-core | 1 | 215µs | 48.5% | 206µs–224µs | 4.7K ops/s | 113.12ms | 35.0MB | 0.00KB | 33.4MB | 68.5KB |
| bubbletea | 1 | 236µs | 38.1% | 228µs–244µs | 4.2K ops/s | 118.06ms | 16.2MB | 0.00KB | 7.8MB | 16.0KB |
| terminal-kit | 1 | 268µs | 28.6% | 261µs–275µs | 3.7K ops/s | 134.04ms | 55.1MB | 68.0KB | 950.2KB | 1.9KB |
| blessed | 1 | 286µs | 56.7% | 272µs–300µs | 3.5K ops/s | 142.98ms | 520.8MB | 0.00KB | 1.1MB | 2.3KB |
| prompt-toolkit | 1 | 1.08ms | 26.8% | 1.06ms–1.11ms | 924 ops/s | 541.25ms | 36.2MB | 16.0KB | 948.7KB | 1.9KB |
| rich | 1 | 1.27ms | 25.3% | 1.24ms–1.30ms | 788 ops/s | 634.79ms | 32.7MB | 460.0KB | 948.7KB | 1.9KB |
| urwid | 1 | 1.29ms | 23.4% | 1.26ms–1.31ms | 778 ops/s | 642.90ms | 42.1MB | 0.00KB | 937.5KB | 1.9KB |
| ink | 1 | 3.07ms | 27.0% | 2.99ms–3.14ms | 326 ops/s | 1.53s | 355.0MB | 0.00KB | 953.1KB | 1.9KB |

## layout-stress (cols=4,rows=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| opentui-react | 1 | 48µs | 24.0% | 46µs–49µs | 21.0K ops/s | 15.99ms | 38.9MB | 0.00KB | 1.2KB | 4B |
| tuix-python | 1 | 61µs | 27.2% | 59µs–63µs | 16.3K ops/s | 20.66ms | 18.7MB | 0.00KB | 1.1MB | 3.6KB |
| TUIX.Renderer | 1 | 76µs | 11.8% | 75µs–77µs | 13.2K ops/s | 25.56ms | 3.9MB | 0.00KB | 1.1MB | 3.7KB |
| opentui-core | 1 | 80µs | 29.4% | 77µs–83µs | 12.5K ops/s | 31.84ms | 35.0MB | 0.00KB | 19.8MB | 67.7KB |
| TUIX.Core | 1 | 102µs | 30.3% | 98µs–105µs | 9.8K ops/s | 30.55ms | 4.1MB | 0.00KB | 1.1MB | 3.6KB |
| ratatui | 1 | 109µs | 18.0% | 107µs–111µs | 9.2K ops/s | 32.77ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| bubbletea | 1 | 161µs | 34.1% | 155µs–167µs | 6.2K ops/s | 48.41ms | 16.2MB | 0.00KB | 4.7MB | 16.0KB |
| blessed | 1 | 198µs | 141.0% | 166µs–229µs | 5.1K ops/s | 59.35ms | 521.4MB | 0.00KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 549µs | 26.0% | 533µs–565µs | 1.8K ops/s | 164.80ms | 55.1MB | 4.0KB | 570.1KB | 1.9KB |
| rich | 1 | 855µs | 26.2% | 830µs–880µs | 1.2K ops/s | 256.66ms | 32.8MB | 8.0KB | 569.2KB | 1.9KB |
| prompt-toolkit | 1 | 934µs | 26.9% | 906µs–962µs | 1.1K ops/s | 280.29ms | 36.2MB | 8.0KB | 569.2KB | 1.9KB |
| rezitui | 1 | 1.27ms | 56.5% | 1.19ms–1.35ms | 788 ops/s | 380.93ms | 188.7MB | 1.0MB | 1.6MB | 5.6KB |
| urwid | 1 | 1.36ms | 24.9% | 1.33ms–1.40ms | 733 ops/s | 409.45ms | 42.1MB | 0.00KB | 562.5KB | 1.9KB |
| ink | 1 | 5.56ms | 26.3% | 5.39ms–5.72ms | 180 ops/s | 1.67s | 355.7MB | 488.0KB | 1.2MB | 4.2KB |

## scroll-stress (items=2000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 24µs | 14.3% | 23µs–25µs | 41.7K ops/s | 1.20ms | 4.1MB | 0.00KB | 800B | 16B |
| opentui-react | 1 | 45µs | 46.9% | 39µs–51µs | 22.4K ops/s | 2.32ms | 38.9MB | 0.00KB | 135.6KB | 2.7KB |
| rezitui | 1 | 51µs | 24.5% | 48µs–55µs | 19.5K ops/s | 2.57ms | 188.9MB | 236.0KB | 66.8KB | 1.3KB |
| opentui-core | 1 | 92µs | 50.7% | 79µs–105µs | 10.9K ops/s | 5.18ms | 35.1MB | 100.0KB | 3.1MB | 62.8KB |
| bubbletea | 1 | 98µs | 69.7% | 79µs–117µs | 10.2K ops/s | 4.90ms | 16.2MB | 0.00KB | 798.0KB | 16.0KB |
| ratatui | 1 | 114µs | 3.5% | 113µs–115µs | 8.8K ops/s | 5.70ms | 27.0KB | 0.00KB | 798.0KB | 16.0KB |
| TUIX.Renderer | 1 | 166µs | 14.1% | 160µs–173µs | 6.0K ops/s | 8.51ms | 3.9MB | 0.00KB | 1.5MB | 30.8KB |
| tuix-python | 1 | 195µs | 29.9% | 179µs–212µs | 5.1K ops/s | 10.03ms | 18.7MB | 4.0KB | 1.6MB | 32.4KB |
| prompt-toolkit | 1 | 492µs | 17.2% | 468µs–515µs | 2.0K ops/s | 24.61ms | 36.2MB | 4.0KB | 94.9KB | 1.9KB |
| rich | 1 | 615µs | 25.2% | 572µs–658µs | 1.6K ops/s | 30.76ms | 32.9MB | 80.0KB | 94.9KB | 1.9KB |
| urwid | 1 | 806µs | 17.6% | 767µs–846µs | 1.2K ops/s | 40.34ms | 42.1MB | 0.00KB | 93.8KB | 1.9KB |
| terminal-kit | 1 | 993µs | 16.2% | 949µs–1.04ms | 1.0K ops/s | 49.68ms | 55.2MB | 0.00KB | 95.0KB | 1.9KB |
| ink | 1 | 1.27ms | 62.9% | 1.05ms–1.49ms | 789 ops/s | 63.40ms | 355.7MB | 4.0KB | 93.2KB | 1.9KB |
| blessed | 1 | 3.69ms | 18.2% | 3.51ms–3.88ms | 271 ops/s | 184.78ms | 521.2MB | 212.0KB | 115.5KB | 2.3KB |

## virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 19µs | 71.1% | 18µs–20µs | 53.1K ops/s | 18.89ms | 4.1MB | 0.00KB | 2.2MB | 2.2KB |
| opentui-react | 1 | 41µs | 35.6% | 40µs–41µs | 24.7K ops/s | 42.13ms | 38.9MB | 0.00KB | 1.8MB | 1.8KB |
| TUIX.Renderer | 1 | 62µs | 35.2% | 61µs–63µs | 16.1K ops/s | 67.76ms | 4.0MB | 0.00KB | 2.3MB | 2.3KB |
| rezitui | 1 | 83µs | 177.7% | 74µs–92µs | 12.1K ops/s | 82.76ms | 190.4MB | 1.2MB | 2.4MB | 2.5KB |
| tuix-python | 1 | 88µs | 27.3% | 86µs–89µs | 11.4K ops/s | 94.98ms | 18.7MB | 32.0KB | 2.2MB | 2.3KB |
| ratatui | 1 | 97µs | 32.5% | 95µs–99µs | 10.3K ops/s | 97.21ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| blessed | 1 | 108µs | 147.3% | 98µs–117µs | 9.3K ops/s | 107.72ms | 522.6MB | 360.0KB | 616.2KB | 631B |
| bubbletea | 1 | 108µs | 66.2% | 104µs–112µs | 9.3K ops/s | 108.25ms | 20.2MB | 4.0MB | 15.6MB | 16.0KB |
| opentui-core | 1 | 109µs | 50.9% | 106µs–113µs | 9.2K ops/s | 132.39ms | 35.1MB | 0.00KB | 66.1MB | 67.7KB |
| terminal-kit | 1 | 463µs | 31.1% | 454µs–472µs | 2.2K ops/s | 463.40ms | 96.1MB | 4.3MB | 1.9MB | 1.9KB |
| rich | 1 | 567µs | 30.8% | 556µs–578µs | 1.8K ops/s | 567.27ms | 32.9MB | 12.0KB | 1.9MB | 1.9KB |
| prompt-toolkit | 1 | 587µs | 29.4% | 576µs–597µs | 1.7K ops/s | 586.94ms | 36.3MB | 72.0KB | 1.9MB | 1.9KB |
| ink | 1 | 704µs | 41.9% | 686µs–722µs | 1.4K ops/s | 704.45ms | 356.7MB | 4.0KB | 153.3KB | 157B |
| urwid | 1 | 970µs | 29.2% | 953µs–988µs | 1.0K ops/s | 970.75ms | 42.1MB | 64.0KB | 1.8MB | 1.9KB |

## tables (cols=8,rows=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 15µs | 28.2% | 14µs–15µs | 68.9K ops/s | 4.37ms | 4.1MB | 0.00KB | 1.5MB | 5.2KB |
| tuix-python | 1 | 60µs | 43.7% | 57µs–63µs | 16.5K ops/s | 21.24ms | 18.7MB | 8.0KB | 4.2MB | 14.3KB |
| ratatui | 1 | 82µs | 40.5% | 78µs–86µs | 12.2K ops/s | 24.60ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| TUIX.Renderer | 1 | 85µs | 8.3% | 85µs–86µs | 11.7K ops/s | 28.78ms | 4.0MB | 0.00KB | 1.6MB | 5.4KB |
| bubbletea | 1 | 134µs | 49.1% | 127µs–142µs | 7.5K ops/s | 40.32ms | 20.2MB | 0.00KB | 4.7MB | 16.0KB |
| opentui-core | 1 | 193µs | 48.2% | 182µs–203µs | 5.2K ops/s | 73.42ms | 35.1MB | 0.00KB | 19.8MB | 67.7KB |
| opentui-react | 1 | 252µs | 49.2% | 238µs–266µs | 4.0K ops/s | 82.79ms | 38.9MB | 0.00KB | 20.6MB | 70.4KB |
| blessed | 1 | 312µs | 46.8% | 296µs–329µs | 3.2K ops/s | 93.68ms | 523.4MB | 0.00KB | 821.5KB | 2.7KB |
| terminal-kit | 1 | 521µs | 30.8% | 503µs–539µs | 1.9K ops/s | 156.29ms | 96.1MB | 32.0KB | 570.1KB | 1.9KB |
| ink | 1 | 611µs | 22.1% | 596µs–626µs | 1.6K ops/s | 183.43ms | 356.7MB | 0.00KB | 569.2KB | 1.9KB |
| rezitui | 1 | 737µs | 66.6% | 682µs–793µs | 1.4K ops/s | 221.26ms | 189.8MB | 256.0KB | 2.4MB | 8.0KB |
| prompt-toolkit | 1 | 783µs | 18.7% | 767µs–800µs | 1.3K ops/s | 235.09ms | 36.3MB | 8.0KB | 569.2KB | 1.9KB |
| rich | 1 | 823µs | 30.8% | 794µs–851µs | 1.2K ops/s | 246.93ms | 32.9MB | 0.00KB | 569.2KB | 1.9KB |
| urwid | 1 | 1.13ms | 22.3% | 1.11ms–1.16ms | 882 ops/s | 340.28ms | 42.1MB | 0.00KB | 562.5KB | 1.9KB |

## memory-profile

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 82µs | 20.4% | 82µs–83µs | 12.1K ops/s | 164.93ms | 27.0KB | 0.00KB | 31.2MB | 16.0KB |
| rezitui | 1 | 111µs | 473.3% | 88µs–134µs | 9.0K ops/s | 222.89ms | 190.9MB | 940.0KB | 2.6MB | 1.3KB |
| bubbletea | 1 | 137µs | 62.0% | 133µs–141µs | 7.3K ops/s | 274.22ms | 20.2MB | 0.00KB | 31.2MB | 16.0KB |
| opentui-react | 1 | 146µs | 28.3% | 145µs–148µs | 6.8K ops/s | 306.18ms | 39.2MB | 272.0KB | 147.0MB | 75.3KB |
| TUIX.Renderer | 1 | 160µs | 32.9% | 158µs–162µs | 6.2K ops/s | 372.74ms | 4.0MB | 0.00KB | 125.9MB | 64.4KB |
| opentui-core | 1 | 177µs | 44.7% | 174µs–181µs | 5.6K ops/s | 417.18ms | 35.1MB | 52.0KB | 133.8MB | 68.5KB |
| tuix-python | 1 | 189µs | 31.0% | 187µs–192µs | 5.3K ops/s | 435.24ms | 18.9MB | 84.0KB | 133.5MB | 68.3KB |
| TUIX.Core | 1 | 211µs | 32.3% | 208µs–214µs | 4.7K ops/s | 423.04ms | 4.1MB | 0.00KB | 120.6MB | 61.7KB |
| blessed | 1 | 376µs | 45.2% | 368µs–383µs | 2.7K ops/s | 751.44ms | 522.6MB | 0.00KB | 4.6MB | 2.3KB |
| terminal-kit | 1 | 542µs | 29.3% | 535µs–549µs | 1.8K ops/s | 1.08s | 95.9MB | 76.0KB | 3.7MB | 1.9KB |
| ink | 1 | 785µs | 30.7% | 774µs–796µs | 1.3K ops/s | 1.57s | 356.7MB | 12.0KB | 3.7MB | 1.9KB |
| prompt-toolkit | 1 | 1.03ms | 24.6% | 1.02ms–1.05ms | 967 ops/s | 2.07s | 36.6MB | 320.0KB | 3.7MB | 1.9KB |
| urwid | 1 | 1.53ms | 28.4% | 1.51ms–1.55ms | 654 ops/s | 3.06s | 42.2MB | 84.0KB | 3.7MB | 1.9KB |
| rich | 1 | 1.64ms | 28.7% | 1.62ms–1.67ms | 608 ops/s | 3.29s | 32.9MB | 40.0KB | 3.7MB | 1.9KB |

## terminal-rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| tuix-python | 1 | 27µs | 33.5% | 26µs–27µs | 37.5K ops/s | 27.19ms | 19.1MB | 64.0KB | 4.6MB | 4.7KB |
| TUIX.Renderer | 1 | 30µs | 31.5% | 30µs–31µs | 33.1K ops/s | 30.27ms | 4.1MB | 0.00KB | 15.6KB | 16B |
| TUIX.Core | 1 | 34µs | 28.4% | 34µs–35µs | 29.3K ops/s | 34.14ms | 4.3MB | 0.00KB | 15.6KB | 16B |
| rezitui | 1 | 37µs | 310.2% | 30µs–44µs | 27.0K ops/s | 37.18ms | 189.1MB | 0.00KB | 2.2MB | 2.2KB |
| ratatui | 1 | 112µs | 26.9% | 111µs–114µs | 8.9K ops/s | 112.53ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| bubbletea | 1 | 120µs | 62.6% | 116µs–125µs | 8.3K ops/s | 120.45ms | 20.2MB | 0.00KB | 40.6MB | 41.6KB |
| opentui-react | 1 | 123µs | 41.9% | 119µs–126µs | 8.2K ops/s | 122.80ms | 39.2MB | 0.00KB | 3.9KB | 4B |
| blessed | 1 | 222µs | 30.4% | 218µs–226µs | 4.5K ops/s | 221.92ms | 522.6MB | 0.00KB | 0.00KB | 0.00KB |
| opentui-core | 1 | 400µs | 41.5% | 390µs–410µs | 2.5K ops/s | 400.26ms | 35.9MB | 24.0KB | 167.1MB | 171.1KB |
| terminal-kit | 1 | 476µs | 29.0% | 468µs–485µs | 2.1K ops/s | 476.46ms | 96.1MB | 128.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.52ms | 29.3% | 1.49ms–1.55ms | 659 ops/s | 1.52s | 356.7MB | 4.0KB | 4.6MB | 4.7KB |
| rich | 1 | 2.45ms | 23.6% | 2.41ms–2.49ms | 408 ops/s | 2.45s | 33.0MB | 16.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 2.90ms | 27.9% | 2.85ms–2.95ms | 344 ops/s | 2.90s | 37.2MB | 284.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 3.29ms | 24.8% | 3.24ms–3.34ms | 304 ops/s | 3.29s | 42.3MB | 64.0KB | 4.6MB | 4.7KB |

## terminal-frame-fill (cols=120,dirtyLines=1,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 33µs | 55.2% | 31µs–34µs | 30.6K ops/s | 16.76ms | 4.1MB | 0.00KB | 7.8KB | 16B |
| TUIX.Core | 1 | 37µs | 22.4% | 36µs–38µs | 27.0K ops/s | 18.52ms | 4.5MB | 0.00KB | 1.2MB | 2.4KB |
| tuix-python | 1 | 43µs | 39.0% | 41µs–44µs | 23.5K ops/s | 22.30ms | 19.1MB | 0.00KB | 22.6MB | 46.2KB |
| rezitui | 1 | 67µs | 213.0% | 55µs–80µs | 14.9K ops/s | 33.72ms | 191.0MB | 1.8MB | 1.1MB | 2.2KB |
| ratatui | 1 | 123µs | 50.4% | 117µs–128µs | 8.1K ops/s | 61.40ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| bubbletea | 1 | 139µs | 84.8% | 129µs–149µs | 7.2K ops/s | 69.53ms | 20.7MB | 264.0KB | 20.3MB | 41.6KB |
| opentui-react | 1 | 145µs | 37.2% | 140µs–150µs | 6.9K ops/s | 76.02ms | 39.2MB | 0.00KB | 4.6MB | 9.5KB |
| opentui-core | 1 | 285µs | 32.4% | 277µs–293µs | 3.5K ops/s | 143.40ms | 36.9MB | 0.00KB | 83.5MB | 171.0KB |
| blessed | 1 | 468µs | 46.1% | 449µs–487µs | 2.1K ops/s | 234.17ms | 522.7MB | 80.0KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 516µs | 30.5% | 502µs–530µs | 1.9K ops/s | 258.15ms | 96.1MB | 0.00KB | 2.3MB | 4.7KB |
| rich | 1 | 826µs | 29.5% | 805µs–848µs | 1.2K ops/s | 413.53ms | 33.1MB | 112.0KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 1.40ms | 24.0% | 1.37ms–1.43ms | 716 ops/s | 698.42ms | 37.2MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 1.65ms | 28.6% | 1.61ms–1.69ms | 605 ops/s | 826.83ms | 356.7MB | 4.0KB | 77.6KB | 159B |
| urwid | 1 | 1.87ms | 24.0% | 1.83ms–1.91ms | 536 ops/s | 933.23ms | 42.3MB | 0.00KB | 2.3MB | 4.7KB |

## terminal-frame-fill (cols=120,dirtyLines=40,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 65µs | 28.1% | 63µs–66µs | 15.4K ops/s | 32.47ms | 4.5MB | 0.00KB | 41.8MB | 85.5KB |
| ratatui | 1 | 196µs | 8.8% | 195µs–198µs | 5.1K ops/s | 98.04ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| rezitui | 1 | 253µs | 377.7% | 169µs–336µs | 4.0K ops/s | 126.45ms | 220.6MB | 29.6MB | 1.1MB | 2.2KB |
| bubbletea | 1 | 305µs | 47.7% | 293µs–318µs | 3.3K ops/s | 152.79ms | 20.7MB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 327µs | 34.6% | 317µs–337µs | 3.1K ops/s | 185.39ms | 36.9MB | 0.00KB | 83.5MB | 171.0KB |
| TUIX.Renderer | 1 | 353µs | 30.4% | 343µs–362µs | 2.8K ops/s | 198.25ms | 4.2MB | 0.00KB | 42.8MB | 87.8KB |
| opentui-react | 1 | 416µs | 29.5% | 405µs–427µs | 2.4K ops/s | 220.68ms | 39.2MB | 0.00KB | 92.3MB | 189.0KB |
| tuix-python | 1 | 432µs | 26.4% | 422µs–442µs | 2.3K ops/s | 240.60ms | 19.0MB | 0.00KB | 42.9MB | 87.8KB |
| blessed | 1 | 823µs | 35.8% | 797µs–849µs | 1.2K ops/s | 411.67ms | 522.9MB | 188.0KB | 2.7MB | 5.5KB |
| terminal-kit | 1 | 1.15ms | 26.0% | 1.12ms–1.17ms | 873 ops/s | 573.13ms | 96.4MB | 4.0KB | 2.3MB | 4.7KB |
| ink | 1 | 1.92ms | 27.2% | 1.87ms–1.96ms | 522 ops/s | 958.22ms | 356.7MB | 0.00KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 2.94ms | 25.6% | 2.88ms–3.01ms | 340 ops/s | 1.47s | 37.2MB | 0.00KB | 2.3MB | 4.7KB |
| urwid | 1 | 3.38ms | 27.8% | 3.29ms–3.46ms | 296 ops/s | 1.69s | 42.3MB | 36.0KB | 2.3MB | 4.7KB |
| rich | 1 | 3.66ms | 24.2% | 3.59ms–3.74ms | 273 ops/s | 1.83s | 33.0MB | 0.00KB | 2.3MB | 4.7KB |

## terminal-screen-transition (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 196µs | 403.8% | 147µs–244µs | 5.1K ops/s | 195.71ms | 219.0MB | 0.00KB | 2.2MB | 2.2KB |
| bubbletea | 1 | 209µs | 58.6% | 202µs–217µs | 4.8K ops/s | 209.32ms | 20.7MB | 0.00KB | 40.6MB | 41.6KB |
| ratatui | 1 | 222µs | 28.7% | 218µs–226µs | 4.5K ops/s | 222.20ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| opentui-core | 1 | 317µs | 43.2% | 308µs–325µs | 3.2K ops/s | 375.42ms | 36.9MB | 0.00KB | 171.1MB | 175.2KB |
| TUIX.Renderer | 1 | 319µs | 52.0% | 309µs–330µs | 3.1K ops/s | 366.68ms | 4.3MB | 0.00KB | 157.1MB | 160.9KB |
| tuix-python | 1 | 348µs | 50.2% | 337µs–358µs | 2.9K ops/s | 390.13ms | 19.5MB | 28.0KB | 166.7MB | 170.8KB |
| TUIX.Core | 1 | 399µs | 50.3% | 386µs–411µs | 2.5K ops/s | 398.71ms | 4.6MB | 0.00KB | 153.2MB | 156.8KB |
| opentui-react | 1 | 449µs | 34.7% | 440µs–459µs | 2.2K ops/s | 465.69ms | 39.2MB | 0.00KB | 187.1MB | 191.6KB |
| blessed | 1 | 744µs | 33.5% | 729µs–760µs | 1.3K ops/s | 744.64ms | 522.9MB | 0.00KB | 5.3MB | 5.4KB |
| terminal-kit | 1 | 1.17ms | 26.1% | 1.15ms–1.19ms | 856 ops/s | 1.17s | 96.4MB | 104.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.52ms | 29.0% | 1.50ms–1.55ms | 656 ops/s | 1.52s | 356.7MB | 0.00KB | 4.6MB | 4.7KB |
| rich | 1 | 1.79ms | 27.5% | 1.76ms–1.82ms | 559 ops/s | 1.79s | 33.1MB | 0.00KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 1.99ms | 27.3% | 1.95ms–2.02ms | 503 ops/s | 1.99s | 37.2MB | 64.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 2.53ms | 28.7% | 2.49ms–2.58ms | 395 ops/s | 2.53s | 42.4MB | 16.0KB | 4.6MB | 4.7KB |

## terminal-fps-stream (channels=12,cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 40µs | 27.9% | 40µs–41µs | 24.7K ops/s | 48.55ms | 4.6MB | 0.00KB | 2.3MB | 2.0KB |
| TUIX.Renderer | 1 | 68µs | 39.1% | 67µs–70µs | 14.6K ops/s | 82.90ms | 4.3MB | 0.00KB | 2.3MB | 2.0KB |
| tuix-python | 1 | 71µs | 32.3% | 70µs–72µs | 14.1K ops/s | 88.20ms | 19.2MB | 36.0KB | 8.4MB | 7.2KB |
| bubbletea | 1 | 182µs | 61.1% | 176µs–189µs | 5.5K ops/s | 219.12ms | 20.7MB | 0.00KB | 48.8MB | 41.6KB |
| ratatui | 1 | 215µs | 23.8% | 212µs–218µs | 4.7K ops/s | 257.75ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 237µs | 38.8% | 231µs–242µs | 4.2K ops/s | 349.94ms | 36.9MB | 0.00KB | 193.0MB | 164.7KB |
| opentui-react | 1 | 277µs | 35.9% | 272µs–283µs | 3.6K ops/s | 354.90ms | 39.2MB | 0.00KB | 202.0MB | 172.3KB |
| rezitui | 1 | 485µs | 124.2% | 451µs–519µs | 2.1K ops/s | 581.99ms | 222.1MB | 2.2MB | 4.2MB | 3.6KB |
| blessed | 1 | 498µs | 46.9% | 485µs–511µs | 2.0K ops/s | 597.65ms | 522.2MB | 28.0KB | 5.7MB | 4.8KB |
| terminal-kit | 1 | 1.17ms | 26.6% | 1.15ms–1.18ms | 858 ops/s | 1.40s | 96.6MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 1.62ms | 29.7% | 1.59ms–1.65ms | 618 ops/s | 1.94s | 356.8MB | 0.00KB | 2.5MB | 2.1KB |
| rich | 1 | 1.83ms | 28.1% | 1.80ms–1.86ms | 547 ops/s | 2.19s | 33.1MB | 4.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.01ms | 28.6% | 1.97ms–2.04ms | 498 ops/s | 2.41s | 37.2MB | 0.00KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.47ms | 23.3% | 2.43ms–2.50ms | 405 ops/s | 2.96s | 42.4MB | 68.0KB | 5.5MB | 4.7KB |

## terminal-input-latency (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Renderer | 1 | 32µs | 67.2% | 30µs–33µs | 31.6K ops/s | 31.70ms | 4.3MB | 0.00KB | 15.6KB | 16B |
| tuix-python | 1 | 38µs | 27.7% | 37µs–39µs | 26.4K ops/s | 39.00ms | 19.3MB | 64.0KB | 4.6MB | 4.7KB |
| TUIX.Core | 1 | 40µs | 40.8% | 39µs–41µs | 24.9K ops/s | 40.26ms | 4.5MB | 0.00KB | 52.7KB | 54B |
| rezitui | 1 | 66µs | 297.5% | 54µs–78µs | 15.2K ops/s | 65.96ms | 220.6MB | 0.00KB | 2.2MB | 2.2KB |
| ratatui | 1 | 119µs | 28.5% | 117µs–122µs | 8.4K ops/s | 119.53ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| opentui-react | 1 | 147µs | 42.1% | 144µs–151µs | 6.8K ops/s | 177.17ms | 39.2MB | 0.00KB | 833.0KB | 853B |
| opentui-core | 1 | 293µs | 28.8% | 288µs–298µs | 3.4K ops/s | 293.63ms | 36.9MB | 0.00KB | 166.6MB | 170.6KB |
| bubbletea | 1 | 309µs | 56.7% | 298µs–320µs | 3.2K ops/s | 308.99ms | 20.7MB | 8.0KB | 40.6MB | 41.6KB |
| blessed | 1 | 419µs | 41.7% | 408µs–430µs | 2.4K ops/s | 419.02ms | 522.4MB | 216.0KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 503µs | 31.2% | 493µs–512µs | 2.0K ops/s | 502.87ms | 96.5MB | 48.0KB | 4.6MB | 4.7KB |
| rich | 1 | 1.26ms | 25.1% | 1.24ms–1.28ms | 792 ops/s | 1.26s | 33.5MB | 412.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.84ms | 28.4% | 1.81ms–1.88ms | 543 ops/s | 1.84s | 356.7MB | 28.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 1.95ms | 27.9% | 1.92ms–1.99ms | 512 ops/s | 1.95s | 37.3MB | 68.0KB | 4.6MB | 4.7KB |
| urwid | 1 | 2.32ms | 24.4% | 2.29ms–2.36ms | 430 ops/s | 2.32s | 42.5MB | 4.0KB | 4.6MB | 4.7KB |

## terminal-memory-soak (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 213µs | 20.1% | 211µs–216µs | 4.7K ops/s | 255.89ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| bubbletea | 1 | 215µs | 56.3% | 208µs–222µs | 4.6K ops/s | 258.34ms | 20.7MB | 0.00KB | 48.8MB | 41.6KB |
| rezitui | 1 | 229µs | 399.2% | 177µs–281µs | 4.4K ops/s | 275.27ms | 220.7MB | 80.0KB | 2.6MB | 2.2KB |
| TUIX.Renderer | 1 | 362µs | 23.2% | 358µs–367µs | 2.8K ops/s | 506.53ms | 4.3MB | 0.00KB | 188.5MB | 160.9KB |
| opentui-core | 1 | 370µs | 30.4% | 363µs–376µs | 2.7K ops/s | 497.81ms | 36.9MB | 0.00KB | 200.4MB | 171.0KB |
| opentui-react | 1 | 403µs | 32.1% | 396µs–411µs | 2.5K ops/s | 494.89ms | 39.2MB | 0.00KB | 222.5MB | 189.9KB |
| tuix-python | 1 | 441µs | 27.4% | 434µs–448µs | 2.3K ops/s | 608.44ms | 19.3MB | 64.0KB | 200.1MB | 170.8KB |
| TUIX.Core | 1 | 481µs | 30.9% | 472µs–489µs | 2.1K ops/s | 576.83ms | 4.6MB | 0.00KB | 183.8MB | 156.8KB |
| blessed | 1 | 799µs | 34.9% | 783µs–814µs | 1.3K ops/s | 958.69ms | 522.4MB | 24.0KB | 6.5MB | 5.5KB |
| terminal-kit | 1 | 1.41ms | 27.9% | 1.38ms–1.43ms | 711 ops/s | 1.69s | 96.6MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 1.79ms | 29.0% | 1.76ms–1.82ms | 558 ops/s | 2.15s | 356.7MB | 0.00KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.69ms | 26.0% | 2.65ms–2.73ms | 371 ops/s | 3.23s | 37.3MB | 8.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 3.38ms | 26.2% | 3.33ms–3.43ms | 296 ops/s | 4.05s | 42.5MB | 32.0KB | 5.5MB | 4.7KB |
| rich | 1 | 3.68ms | 27.2% | 3.63ms–3.74ms | 271 ops/s | 4.42s | 33.7MB | 272.0KB | 5.5MB | 4.7KB |

## terminal-full-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 41µs | 47.9% | 40µs–43µs | 24.2K ops/s | 49.72ms | 4.5MB | 0.00KB | 3.9MB | 3.3KB |
| TUIX.Renderer | 1 | 93µs | 36.8% | 91µs–95µs | 10.8K ops/s | 124.61ms | 4.3MB | 0.00KB | 3.9MB | 3.3KB |
| tuix-python | 1 | 118µs | 31.6% | 116µs–120µs | 8.5K ops/s | 151.90ms | 19.3MB | 0.00KB | 6.5MB | 5.6KB |
| ratatui | 1 | 170µs | 18.2% | 168µs–172µs | 5.9K ops/s | 204.02ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 278µs | 36.5% | 272µs–284µs | 3.6K ops/s | 370.49ms | 36.9MB | 0.00KB | 204.3MB | 174.4KB |
| opentui-react | 1 | 300µs | 34.2% | 294µs–306µs | 3.3K ops/s | 379.34ms | 39.2MB | 0.00KB | 149.5MB | 127.6KB |
| bubbletea | 1 | 497µs | 46.5% | 484µs–510µs | 2.0K ops/s | 596.53ms | 20.7MB | 0.00KB | 48.8MB | 41.6KB |
| blessed | 1 | 633µs | 38.8% | 619µs–647µs | 1.6K ops/s | 759.63ms | 522.5MB | 28.0KB | 3.8MB | 3.2KB |
| rezitui | 1 | 780µs | 78.3% | 745µs–815µs | 1.3K ops/s | 936.72ms | 221.3MB | 0.00KB | 4.3MB | 3.7KB |
| terminal-kit | 1 | 983µs | 34.1% | 964µs–1.00ms | 1.0K ops/s | 1.18s | 97.0MB | 4.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.61ms | 29.8% | 1.58ms–1.64ms | 622 ops/s | 1.93s | 357.1MB | 184.0KB | 3.4MB | 2.9KB |
| rich | 1 | 1.71ms | 27.0% | 1.69ms–1.74ms | 584 ops/s | 2.06s | 33.6MB | 0.00KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.06ms | 29.9% | 2.03ms–2.10ms | 485 ops/s | 2.48s | 37.4MB | 64.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.45ms | 24.4% | 2.42ms–2.49ms | 408 ops/s | 2.94s | 42.6MB | 104.0KB | 5.5MB | 4.7KB |

## terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 43µs | 32.4% | 42µs–43µs | 23.4K ops/s | 51.28ms | 4.6MB | 0.00KB | 69.9MB | 59.7KB |
| TUIX.Renderer | 1 | 113µs | 54.0% | 109µs–116µs | 8.9K ops/s | 161.13ms | 4.3MB | 0.00KB | 69.9MB | 59.7KB |
| tuix-python | 1 | 118µs | 54.1% | 114µs–122µs | 8.5K ops/s | 160.39ms | 19.4MB | 64.0KB | 74.7MB | 63.7KB |
| ratatui | 1 | 179µs | 31.8% | 176µs–182µs | 5.6K ops/s | 214.81ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 321µs | 38.5% | 314µs–328µs | 3.1K ops/s | 396.51ms | 39.2MB | 0.00KB | 149.5MB | 127.6KB |
| opentui-core | 1 | 330µs | 51.6% | 321µs–340µs | 3.0K ops/s | 428.69ms | 36.9MB | 0.00KB | 204.3MB | 174.4KB |
| bubbletea | 1 | 502µs | 44.0% | 489µs–514µs | 2.0K ops/s | 602.72ms | 20.7MB | 0.00KB | 48.8MB | 41.6KB |
| blessed | 1 | 610µs | 43.2% | 595µs–625µs | 1.6K ops/s | 732.33ms | 522.5MB | 0.00KB | 3.8MB | 3.2KB |
| rezitui | 1 | 662µs | 91.5% | 628µs–696µs | 1.5K ops/s | 795.05ms | 226.5MB | 5.2MB | 4.4MB | 3.7KB |
| terminal-kit | 1 | 970µs | 29.0% | 955µs–986µs | 1.0K ops/s | 1.17s | 97.0MB | 0.00KB | 5.5MB | 4.7KB |
| rich | 1 | 1.57ms | 26.0% | 1.55ms–1.59ms | 636 ops/s | 1.89s | 33.7MB | 60.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.74ms | 29.1% | 1.71ms–1.76ms | 576 ops/s | 2.08s | 357.2MB | 8.0KB | 5.4MB | 4.6KB |
| prompt-toolkit | 1 | 1.94ms | 29.5% | 1.91ms–1.98ms | 515 ops/s | 2.33s | 37.4MB | 64.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.29ms | 22.9% | 2.26ms–2.32ms | 437 ops/s | 2.75s | 42.7MB | 52.0KB | 5.5MB | 4.7KB |

## terminal-strict-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 38µs | 51.8% | 37µs–39µs | 26.2K ops/s | 45.86ms | 4.7MB | 0.00KB | 3.9MB | 3.3KB |
| tuix-python | 1 | 83µs | 14.5% | 83µs–84µs | 12.0K ops/s | 107.08ms | 19.4MB | 4.0KB | 6.5MB | 5.6KB |
| TUIX.Renderer | 1 | 84µs | 29.0% | 83µs–85µs | 11.9K ops/s | 112.05ms | 4.3MB | 0.00KB | 3.9MB | 3.3KB |
| ratatui | 1 | 171µs | 14.7% | 170µs–173µs | 5.8K ops/s | 205.46ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 294µs | 36.7% | 288µs–300µs | 3.4K ops/s | 353.44ms | 36.9MB | 0.00KB | 186.1MB | 158.8KB |
| bubbletea | 1 | 464µs | 47.7% | 451µs–476µs | 2.2K ops/s | 556.85ms | 20.7MB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 495µs | 35.4% | 485µs–505µs | 2.0K ops/s | 594.49ms | 39.2MB | 0.00KB | 240.6MB | 205.3KB |
| blessed | 1 | 618µs | 38.3% | 605µs–631µs | 1.6K ops/s | 741.84ms | 522.5MB | 0.00KB | 3.8MB | 3.2KB |
| rezitui | 1 | 708µs | 82.8% | 675µs–741µs | 1.4K ops/s | 850.40ms | 219.6MB | 0.00KB | 4.3MB | 3.7KB |
| terminal-kit | 1 | 975µs | 27.5% | 959µs–990µs | 1.0K ops/s | 1.17s | 97.1MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 1.59ms | 29.6% | 1.57ms–1.62ms | 628 ops/s | 1.91s | 357.2MB | 0.00KB | 3.4MB | 2.9KB |
| rich | 1 | 1.65ms | 26.7% | 1.62ms–1.67ms | 607 ops/s | 1.98s | 33.7MB | 40.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 2.01ms | 27.6% | 1.98ms–2.05ms | 496 ops/s | 2.42s | 37.4MB | 20.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.48ms | 27.3% | 2.44ms–2.52ms | 403 ops/s | 2.98s | 42.5MB | 0.00KB | 5.5MB | 4.7KB |

## terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 42µs | 43.3% | 41µs–43µs | 23.9K ops/s | 50.30ms | 4.5MB | 0.00KB | 69.9MB | 59.7KB |
| TUIX.Renderer | 1 | 85µs | 51.4% | 83µs–88µs | 11.7K ops/s | 120.36ms | 4.3MB | 0.00KB | 69.9MB | 59.7KB |
| tuix-python | 1 | 111µs | 53.5% | 108µs–115µs | 9.0K ops/s | 151.60ms | 19.4MB | 32.0KB | 74.7MB | 63.7KB |
| ratatui | 1 | 166µs | 19.4% | 164µs–167µs | 6.0K ops/s | 198.88ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 259µs | 33.6% | 254µs–264µs | 3.9K ops/s | 310.80ms | 36.9MB | 0.00KB | 186.1MB | 158.8KB |
| opentui-react | 1 | 508µs | 77.3% | 486µs–530µs | 2.0K ops/s | 610.28ms | 39.2MB | 0.00KB | 240.6MB | 205.3KB |
| bubbletea | 1 | 617µs | 42.9% | 602µs–632µs | 1.6K ops/s | 741.06ms | 20.7MB | 0.00KB | 48.8MB | 41.6KB |
| rezitui | 1 | 642µs | 83.4% | 612µs–672µs | 1.6K ops/s | 771.24ms | 220.8MB | 1.2MB | 4.4MB | 3.7KB |
| blessed | 1 | 686µs | 37.9% | 671µs–701µs | 1.5K ops/s | 823.91ms | 522.5MB | 0.00KB | 3.8MB | 3.2KB |
| terminal-kit | 1 | 919µs | 24.9% | 906µs–932µs | 1.1K ops/s | 1.10s | 97.1MB | 56.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.60ms | 29.9% | 1.58ms–1.63ms | 624 ops/s | 1.92s | 357.2MB | 0.00KB | 5.4MB | 4.6KB |
| rich | 1 | 1.70ms | 27.9% | 1.68ms–1.73ms | 587 ops/s | 2.05s | 33.8MB | 72.0KB | 5.5MB | 4.7KB |
| prompt-toolkit | 1 | 1.97ms | 26.8% | 1.94ms–2.00ms | 509 ops/s | 2.36s | 37.5MB | 100.0KB | 5.5MB | 4.7KB |
| urwid | 1 | 2.57ms | 26.8% | 2.53ms–2.61ms | 389 ops/s | 3.08s | 42.6MB | 36.0KB | 5.5MB | 4.7KB |

## terminal-virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 54µs | 26.5% | 53µs–55µs | 18.5K ops/s | 54.14ms | 4.7MB | 0.00KB | 5.2MB | 5.3KB |
| rezitui | 1 | 66µs | 244.2% | 56µs–76µs | 15.2K ops/s | 65.97ms | 220.7MB | 0.00KB | 4.1MB | 4.2KB |
| opentui-react | 1 | 106µs | 35.5% | 104µs–108µs | 9.4K ops/s | 108.86ms | 39.2MB | 0.00KB | 3.0MB | 3.1KB |
| TUIX.Renderer | 1 | 135µs | 26.7% | 133µs–137µs | 7.4K ops/s | 145.78ms | 4.3MB | 0.00KB | 5.3MB | 5.4KB |
| bubbletea | 1 | 176µs | 85.9% | 166µs–185µs | 5.7K ops/s | 175.78ms | 20.7MB | 0.00KB | 40.6MB | 41.6KB |
| ratatui | 1 | 185µs | 11.4% | 184µs–186µs | 5.4K ops/s | 184.88ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| tuix-python | 1 | 208µs | 32.2% | 204µs–212µs | 4.8K ops/s | 220.92ms | 19.5MB | 28.0KB | 5.2MB | 5.3KB |
| blessed | 1 | 219µs | 115.4% | 203µs–235µs | 4.6K ops/s | 219.33ms | 523.4MB | 0.00KB | 1.0MB | 1.0KB |
| opentui-core | 1 | 307µs | 41.8% | 299µs–315µs | 3.3K ops/s | 351.90ms | 36.9MB | 0.00KB | 165.1MB | 169.0KB |
| rich | 1 | 939µs | 30.8% | 922µs–957µs | 1.1K ops/s | 940.19ms | 34.0MB | 216.0KB | 4.6MB | 4.7KB |
| terminal-kit | 1 | 1.29ms | 29.6% | 1.27ms–1.31ms | 775 ops/s | 1.29s | 128.9MB | 324.0KB | 4.6MB | 4.7KB |
| prompt-toolkit | 1 | 1.43ms | 27.6% | 1.41ms–1.46ms | 698 ops/s | 1.43s | 37.5MB | 0.00KB | 4.6MB | 4.7KB |
| ink | 1 | 1.61ms | 29.2% | 1.58ms–1.64ms | 621 ops/s | 1.61s | 358.1MB | 0.00KB | 262.7KB | 269B |
| urwid | 1 | 1.86ms | 24.5% | 1.84ms–1.89ms | 537 ops/s | 1.86s | 42.6MB | 36.0KB | 4.6MB | 4.7KB |

## terminal-table (cols=8,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 58µs | 11.4% | 58µs–59µs | 17.1K ops/s | 29.27ms | 4.7MB | 0.00KB | 5.1MB | 10.5KB |
| tuix-python | 1 | 139µs | 17.0% | 137µs–141µs | 7.2K ops/s | 83.60ms | 19.5MB | 12.0KB | 12.6MB | 25.8KB |
| TUIX.Renderer | 1 | 154µs | 30.9% | 149µs–158µs | 6.5K ops/s | 86.40ms | 4.4MB | 0.00KB | 5.2MB | 10.7KB |
| bubbletea | 1 | 220µs | 28.5% | 214µs–225µs | 4.5K ops/s | 110.05ms | 20.7MB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 239µs | 38.2% | 231µs–247µs | 4.2K ops/s | 145.67ms | 36.9MB | 0.00KB | 82.5MB | 169.0KB |
| ratatui | 1 | 245µs | 38.0% | 236µs–253µs | 4.1K ops/s | 122.30ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| opentui-react | 1 | 450µs | 38.7% | 435µs–465µs | 2.2K ops/s | 241.18ms | 39.2MB | 0.00KB | 91.4MB | 187.3KB |
| blessed | 1 | 902µs | 40.5% | 870µs–934µs | 1.1K ops/s | 451.03ms | 522.8MB | 56.0KB | 3.0MB | 6.2KB |
| rezitui | 1 | 947µs | 71.0% | 888µs–1.01ms | 1.1K ops/s | 473.74ms | 223.6MB | 1.8MB | 7.6MB | 15.6KB |
| terminal-kit | 1 | 1.23ms | 27.0% | 1.20ms–1.26ms | 811 ops/s | 616.66ms | 129.0MB | 8.0KB | 2.3MB | 4.7KB |
| rich | 1 | 1.63ms | 27.9% | 1.59ms–1.67ms | 615 ops/s | 813.07ms | 34.0MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 1.67ms | 31.3% | 1.62ms–1.71ms | 600 ops/s | 834.22ms | 357.5MB | 0.00KB | 2.3MB | 4.7KB |
| prompt-toolkit | 1 | 2.40ms | 28.6% | 2.34ms–2.46ms | 417 ops/s | 1.20s | 37.5MB | 0.00KB | 2.3MB | 4.7KB |
| urwid | 1 | 2.83ms | 25.5% | 2.76ms–2.89ms | 354 ops/s | 1.41s | 42.7MB | 24.0KB | 2.3MB | 4.7KB |

## Relative Performance (vs fastest per scenario)

> Ratio = framework mean / fastest mean for each scenario. Lower is better. 1.0x = fastest.

| Scenario | blessed | terminal-kit | ink | rezitui | bubbletea | ratatui | opentui-core | opentui-react | TUIX.Renderer | TUIX.Core | tuix-python | rich | urwid | prompt-toolkit |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| startup | 131.4x [127.2x, 135.6x] | 2.7x [2.6x, 2.8x] | 12.9x [12.1x, 13.6x] | **1.0x** | 1.4x [1.3x, 1.6x] | **1.0x** | 3.9x [3.6x, 4.2x] | 2.0x [1.8x, 2.2x] | 1.3x [1.2x, 1.4x] | 1.3x [1.3x, 1.4x] | 1.2x [1.1x, 1.3x] | 7.3x [7.0x, 7.5x] | 8.6x [8.1x, 9.2x] | 5.6x [5.3x, 5.9x] |
| tree-construction (items=10) | 1.7x [1.3x, 2.1x] | 5.3x [5.2x, 5.4x] | 9.5x [9.1x, 9.9x] | 1.8x [1.4x, 2.3x] | 3.0x [2.9x, 3.1x] | 1.8x [1.7x, 1.8x] | 5.7x [5.3x, 6.1x] | 1.0x [1.0x, 1.1x] | 1.2x [1.2x, 1.3x] | **1.0x** | 1.9x [1.9x, 2.0x] | 14.6x [14.3x, 14.9x] | 23.3x [22.7x, 23.9x] | 15.8x [15.5x, 16.1x] |
| tree-construction (items=100) | 5.6x [5.0x, 6.2x] | 15.5x [15.1x, 15.9x] | 49.6x [48.4x, 50.9x] | 1.2x [1.0x, 1.4x] | 2.2x [2.2x, 2.3x] | 2.0x [2.0x, 2.1x] | 1.6x [1.3x, 1.9x] | 1.5x [1.5x, 1.6x] | 1.0x [1.0x, 1.1x] | **1.0x** | 2.0x [1.9x, 2.0x] | 8.9x [8.7x, 9.1x] | 12.7x [12.4x, 12.9x] | 9.9x [9.7x, 10.2x] |
| tree-construction (items=500) | 36.3x [34.3x, 38.2x] | 111.9x [109.3x, 114.5x] | 385.0x [377.7x, 392.2x] | 5.6x [4.2x, 7.0x] | 12.5x [12.2x, 12.8x] | 8.8x [8.8x, 8.8x] | 2.5x [2.4x, 2.7x] | 5.6x [5.5x, 5.8x] | **1.0x** | 1.9x [1.9x, 2.0x] | 2.2x [2.1x, 2.2x] | 14.9x [14.6x, 15.3x] | 22.5x [21.9x, 23.1x] | 16.7x [16.3x, 17.1x] |
| tree-construction (items=1000) | 54.6x [52.7x, 56.5x] | 174.5x [171.1x, 177.8x] | 596.2x [585.7x, 606.6x] | 6.6x [5.4x, 7.7x] | 14.6x [14.2x, 14.9x] | 13.7x [13.6x, 13.9x] | 1.3x [1.3x, 1.4x] | 3.9x [3.3x, 4.4x] | 1.2x [1.1x, 1.2x] | **1.0x** | 1.7x [1.7x, 1.8x] | 14.3x [13.9x, 14.6x] | 18.1x [17.7x, 18.5x] | 13.9x [13.6x, 14.3x] |
| rerender | 2.5x [2.4x, 2.5x] | 20.4x [20.0x, 20.9x] | 62.1x [60.1x, 64.1x] | 4.1x [3.1x, 5.2x] | 8.7x [8.4x, 9.1x] | 5.5x [5.3x, 5.6x] | 12.8x [12.4x, 13.2x] | 3.9x [3.8x, 4.0x] | **1.0x** | 2.2x [2.1x, 2.2x] | 1.2x [1.2x, 1.2x] | 123.4x [121.1x, 125.6x] | 140.0x [137.7x, 142.3x] | 102.8x [101.1x, 104.5x] |
| content-update | 11.3x [10.7x, 11.9x] | 10.6x [10.3x, 10.9x] | 121.3x [118.4x, 124.2x] | 2.5x [1.8x, 3.1x] | 9.3x [9.0x, 9.6x] | 3.3x [3.2x, 3.4x] | 8.5x [8.1x, 8.9x] | 5.3x [5.0x, 5.6x] | 4.7x [4.7x, 4.8x] | **1.0x** | 4.3x [4.2x, 4.5x] | 50.2x [49.1x, 51.3x] | 50.8x [49.8x, 51.9x] | 42.8x [41.8x, 43.8x] |
| layout-stress (cols=4,rows=10) | 4.1x [3.5x, 4.8x] | 11.5x [11.2x, 11.8x] | 116.5x [113.0x, 120.0x] | 26.6x [24.9x, 28.3x] | 3.4x [3.2x, 3.5x] | 2.3x [2.2x, 2.3x] | 1.7x [1.6x, 1.7x] | **1.0x** | 1.6x [1.6x, 1.6x] | 2.1x [2.1x, 2.2x] | 1.3x [1.2x, 1.3x] | 17.9x [17.4x, 18.4x] | 28.6x [27.8x, 29.4x] | 19.6x [19.0x, 20.2x] |
| scroll-stress (items=2000) | 154.1x [146.3x, 161.9x] | 41.4x [39.6x, 43.3x] | 52.9x [43.7x, 62.1x] | 2.1x [2.0x, 2.3x] | 4.1x [3.3x, 4.9x] | 4.8x [4.7x, 4.8x] | 3.8x [3.3x, 4.4x] | 1.9x [1.6x, 2.1x] | 6.9x [6.7x, 7.2x] | **1.0x** | 8.2x [7.5x, 8.8x] | 25.6x [23.9x, 27.4x] | 33.6x [32.0x, 35.3x] | 20.5x [19.5x, 21.5x] |
| virtual-list (items=100000,viewport=40) | 5.7x [5.2x, 6.2x] | 24.6x [24.1x, 25.1x] | 37.4x [36.4x, 38.3x] | 4.4x [3.9x, 4.9x] | 5.7x [5.5x, 6.0x] | 5.2x [5.1x, 5.3x] | 5.8x [5.6x, 6.0x] | 2.2x [2.1x, 2.2x] | 3.3x [3.2x, 3.4x] | **1.0x** | 4.7x [4.6x, 4.7x] | 30.1x [29.5x, 30.6x] | 51.5x [50.6x, 52.4x] | 31.1x [30.6x, 31.7x] |
| tables (cols=8,rows=100) | 21.5x [20.4x, 22.6x] | 35.9x [34.6x, 37.1x] | 42.1x [41.0x, 43.2x] | 50.8x [47.0x, 54.6x] | 9.2x [8.7x, 9.8x] | 5.6x [5.4x, 5.9x] | 13.3x [12.5x, 14.0x] | 17.4x [16.4x, 18.3x] | 5.9x [5.8x, 5.9x] | **1.0x** | 4.2x [4.0x, 4.4x] | 56.7x [54.7x, 58.6x] | 78.1x [76.1x, 80.1x] | 54.0x [52.8x, 55.1x] |
| memory-profile | 4.6x [4.5x, 4.6x] | 6.6x [6.5x, 6.7x] | 9.5x [9.4x, 9.7x] | 1.4x [1.1x, 1.6x] | 1.7x [1.6x, 1.7x] | **1.0x** | 2.2x [2.1x, 2.2x] | 1.8x [1.8x, 1.8x] | 1.9x [1.9x, 2.0x] | 2.6x [2.5x, 2.6x] | 2.3x [2.3x, 2.3x] | 20.0x [19.7x, 20.2x] | 18.6x [18.3x, 18.8x] | 12.6x [12.4x, 12.7x] |
| terminal-rerender | 8.3x [8.2x, 8.5x] | 17.9x [17.6x, 18.2x] | 57.0x [55.9x, 58.0x] | 1.4x [1.1x, 1.7x] | 4.5x [4.3x, 4.7x] | 4.2x [4.2x, 4.3x] | 15.0x [14.6x, 15.4x] | 4.6x [4.5x, 4.7x] | 1.1x [1.1x, 1.2x] | 1.3x [1.3x, 1.3x] | **1.0x** | 91.9x [90.6x, 93.3x] | 123.6x [121.7x, 125.5x] | 109.0x [107.1x, 110.9x] |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | 14.3x [13.7x, 14.9x] | 15.8x [15.3x, 16.2x] | 50.5x [49.2x, 51.8x] | 2.1x [1.7x, 2.4x] | 4.2x [3.9x, 4.6x] | 3.8x [3.6x, 3.9x] | 8.7x [8.5x, 8.9x] | 4.4x [4.3x, 4.6x] | **1.0x** | 1.1x [1.1x, 1.2x] | 1.3x [1.3x, 1.3x] | 25.3x [24.6x, 25.9x] | 57.0x [55.8x, 58.2x] | 42.7x [41.8x, 43.6x] |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | 12.7x [12.3x, 13.1x] | 17.7x [17.3x, 18.1x] | 29.5x [28.8x, 30.2x] | 3.9x [2.6x, 5.2x] | 4.7x [4.5x, 4.9x] | 3.0x [3.0x, 3.0x] | 5.0x [4.9x, 5.2x] | 6.4x [6.2x, 6.6x] | 5.4x [5.3x, 5.6x] | **1.0x** | 6.7x [6.5x, 6.8x] | 56.5x [55.3x, 57.7x] | 52.0x [50.8x, 53.3x] | 45.4x [44.4x, 46.4x] |
| terminal-screen-transition (cols=120,rows=40) | 3.8x [3.7x, 3.9x] | 6.0x [5.9x, 6.1x] | 7.8x [7.7x, 7.9x] | **1.0x** | 1.1x [1.0x, 1.1x] | 1.1x [1.1x, 1.2x] | 1.6x [1.6x, 1.7x] | 2.3x [2.2x, 2.3x] | 1.6x [1.6x, 1.7x] | 2.0x [2.0x, 2.1x] | 1.8x [1.7x, 1.8x] | 9.2x [9.0x, 9.3x] | 12.9x [12.7x, 13.2x] | 10.2x [10.0x, 10.3x] |
| terminal-fps-stream (channels=12,cols=120,rows=40) | 12.3x [12.0x, 12.6x] | 28.8x [28.4x, 29.3x] | 40.0x [39.4x, 40.7x] | 12.0x [11.1x, 12.8x] | 4.5x [4.4x, 4.7x] | 5.3x [5.2x, 5.4x] | 5.9x [5.7x, 6.0x] | 6.9x [6.7x, 7.0x] | 1.7x [1.7x, 1.7x] | **1.0x** | 1.8x [1.7x, 1.8x] | 45.2x [44.5x, 46.0x] | 61.0x [60.2x, 61.8x] | 49.7x [48.9x, 50.5x] |
| terminal-input-latency (cols=120,rows=40) | 13.2x [12.9x, 13.6x] | 15.9x [15.6x, 16.2x] | 58.2x [57.2x, 59.2x] | 2.1x [1.7x, 2.5x] | 9.8x [9.4x, 10.1x] | 3.8x [3.7x, 3.8x] | 9.3x [9.1x, 9.4x] | 4.7x [4.5x, 4.8x] | **1.0x** | 1.3x [1.2x, 1.3x] | 1.2x [1.2x, 1.2x] | 39.9x [39.3x, 40.5x] | 73.4x [72.3x, 74.5x] | 61.7x [60.7x, 62.8x] |
| terminal-memory-soak (cols=120,rows=40) | 3.7x [3.7x, 3.8x] | 6.6x [6.5x, 6.7x] | 8.4x [8.3x, 8.5x] | 1.1x [0.8x, 1.3x] | **1.0x** | **1.0x** | 1.7x [1.7x, 1.8x] | 1.9x [1.9x, 1.9x] | 1.7x [1.7x, 1.7x] | 2.3x [2.2x, 2.3x] | 2.1x [2.0x, 2.1x] | 17.3x [17.0x, 17.5x] | 15.8x [15.6x, 16.1x] | 12.6x [12.4x, 12.8x] |
| terminal-full-ui (cols=120,rows=40,services=24) | 15.3x [15.0x, 15.6x] | 23.7x [23.3x, 24.2x] | 38.9x [38.2x, 39.5x] | 18.8x [18.0x, 19.7x] | 12.0x [11.7x, 12.3x] | 4.1x [4.1x, 4.1x] | 6.7x [6.6x, 6.9x] | 7.3x [7.1x, 7.4x] | 2.2x [2.2x, 2.3x] | **1.0x** | 2.9x [2.8x, 2.9x] | 41.4x [40.8x, 42.0x] | 59.2x [58.4x, 60.0x] | 49.9x [49.0x, 50.7x] |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 14.3x [13.9x, 14.6x] | 22.7x [22.4x, 23.1x] | 40.6x [40.0x, 41.3x] | 15.5x [14.7x, 16.3x] | 11.8x [11.5x, 12.1x] | 4.2x [4.1x, 4.3x] | 7.7x [7.5x, 8.0x] | 7.5x [7.4x, 7.7x] | 2.6x [2.6x, 2.7x] | **1.0x** | 2.8x [2.7x, 2.8x] | 36.8x [36.3x, 37.3x] | 53.6x [52.9x, 54.3x] | 45.5x [44.7x, 46.3x] |
| terminal-strict-ui (cols=120,rows=40,services=24) | 16.2x [15.8x, 16.5x] | 25.5x [25.1x, 25.9x] | 41.7x [41.0x, 42.4x] | 18.5x [17.7x, 19.4x] | 12.1x [11.8x, 12.5x] | 4.5x [4.4x, 4.5x] | 7.7x [7.5x, 7.9x] | 13.0x [12.7x, 13.2x] | 2.2x [2.2x, 2.2x] | **1.0x** | 2.2x [2.2x, 2.2x] | 43.1x [42.5x, 43.8x] | 64.9x [63.9x, 65.9x] | 52.8x [51.9x, 53.6x] |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 16.4x [16.0x, 16.7x] | 21.9x [21.6x, 22.3x] | 38.3x [37.6x, 38.9x] | 15.3x [14.6x, 16.1x] | 14.7x [14.4x, 15.1x] | 4.0x [3.9x, 4.0x] | 6.2x [6.1x, 6.3x] | 12.1x [11.6x, 12.7x] | 2.0x [2.0x, 2.1x] | **1.0x** | 2.7x [2.6x, 2.7x] | 40.7x [40.0x, 41.3x] | 61.3x [60.4x, 62.3x] | 46.9x [46.2x, 47.6x] |
| terminal-virtual-list (items=100000,viewport=40) | 4.1x [3.8x, 4.3x] | 23.8x [23.4x, 24.3x] | 29.8x [29.2x, 30.3x] | 1.2x [1.0x, 1.4x] | 3.2x [3.1x, 3.4x] | 3.4x [3.4x, 3.4x] | 5.7x [5.5x, 5.8x] | 2.0x [1.9x, 2.0x] | 2.5x [2.5x, 2.5x] | **1.0x** | 3.9x [3.8x, 3.9x] | 17.4x [17.0x, 17.7x] | 34.5x [33.9x, 35.0x] | 26.5x [26.0x, 27.0x] |
| terminal-table (cols=8,rows=40) | 15.4x [14.9x, 16.0x] | 21.1x [20.6x, 21.6x] | 28.5x [27.7x, 29.3x] | 16.2x [15.2x, 17.2x] | 3.8x [3.7x, 3.9x] | 4.2x [4.0x, 4.3x] | 4.1x [3.9x, 4.2x] | 7.7x [7.4x, 8.0x] | 2.6x [2.6x, 2.7x] | **1.0x** | 2.4x [2.3x, 2.4x] | 27.8x [27.1x, 28.5x] | 48.3x [47.2x, 49.4x] | 41.0x [40.0x, 42.1x] |

## Memory Comparison

| Scenario | Framework | Peak RSS | RSS Before | RSS After | RSS Growth |
|---|---|---:|---:|---:|---:|
| startup | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| startup | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 4.0KB |
| startup | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 4.0KB |
| startup | bubbletea | 11.7MB | 10.8MB | 11.7MB | 904.0KB |
| startup | tuix-python | 18.4MB | 18.3MB | 18.4MB | 52.0KB |
| startup | rich | 32.0MB | 31.4MB | 32.0MB | 620.0KB |
| startup | opentui-core | 33.3MB | 32.9MB | 33.3MB | 412.0KB |
| startup | opentui-react | 35.8MB | 35.4MB | 35.8MB | 416.0KB |
| startup | prompt-toolkit | 36.1MB | 36.1MB | 36.1MB | 16.0KB |
| startup | urwid | 41.8MB | 41.8MB | 41.8MB | 24.0KB |
| startup | terminal-kit | 46.4MB | 46.6MB | 46.4MB | 0.00KB |
| startup | ink | 65.0MB | 58.3MB | 65.0MB | 6.7MB |
| startup | rezitui | 81.7MB | 67.8MB | 81.7MB | 13.9MB |
| startup | blessed | 152.1MB | 77.7MB | 152.1MB | 74.5MB |
| tree-construction (items=10) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=10) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=10) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tree-construction (items=10) | bubbletea | 16.2MB | 11.7MB | 16.2MB | 4.5MB |
| tree-construction (items=10) | tuix-python | 18.5MB | 18.5MB | 18.5MB | 52.0KB |
| tree-construction (items=10) | rich | 32.1MB | 32.0MB | 32.1MB | 28.0KB |
| tree-construction (items=10) | opentui-core | 34.2MB | 33.3MB | 34.2MB | 944.0KB |
| tree-construction (items=10) | opentui-react | 35.9MB | 35.8MB | 35.9MB | 92.0KB |
| tree-construction (items=10) | prompt-toolkit | 36.1MB | 36.1MB | 36.1MB | 16.0KB |
| tree-construction (items=10) | urwid | 41.9MB | 41.9MB | 41.9MB | 76.0KB |
| tree-construction (items=10) | terminal-kit | 46.7MB | 46.6MB | 46.7MB | 32.0KB |
| tree-construction (items=10) | ink | 69.1MB | 66.0MB | 69.1MB | 3.1MB |
| tree-construction (items=10) | rezitui | 100.4MB | 83.4MB | 100.4MB | 17.1MB |
| tree-construction (items=10) | blessed | 161.1MB | 154.2MB | 161.1MB | 6.9MB |
| tree-construction (items=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=100) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=100) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tree-construction (items=100) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 8.0KB |
| tree-construction (items=100) | tuix-python | 18.6MB | 18.6MB | 18.6MB | 16.0KB |
| tree-construction (items=100) | rich | 32.1MB | 32.1MB | 32.1MB | 16.0KB |
| tree-construction (items=100) | opentui-core | 34.9MB | 34.5MB | 34.9MB | 440.0KB |
| tree-construction (items=100) | opentui-react | 36.1MB | 36.0MB | 36.1MB | 104.0KB |
| tree-construction (items=100) | prompt-toolkit | 36.1MB | 36.1MB | 36.1MB | 16.0KB |
| tree-construction (items=100) | urwid | 41.9MB | 41.9MB | 41.9MB | 20.0KB |
| tree-construction (items=100) | terminal-kit | 46.5MB | 46.7MB | 46.5MB | 0.00KB |
| tree-construction (items=100) | ink | 106.4MB | 73.6MB | 106.4MB | 32.7MB |
| tree-construction (items=100) | rezitui | 110.6MB | 98.8MB | 110.6MB | 11.8MB |
| tree-construction (items=100) | blessed | 244.4MB | 165.1MB | 244.4MB | 79.3MB |
| tree-construction (items=500) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=500) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 48.0KB |
| tree-construction (items=500) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tree-construction (items=500) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| tree-construction (items=500) | tuix-python | 18.6MB | 18.6MB | 18.6MB | 16.0KB |
| tree-construction (items=500) | rich | 32.1MB | 32.1MB | 32.1MB | 12.0KB |
| tree-construction (items=500) | opentui-core | 34.9MB | 34.9MB | 34.9MB | 0.00KB |
| tree-construction (items=500) | prompt-toolkit | 36.1MB | 36.1MB | 36.1MB | 16.0KB |
| tree-construction (items=500) | opentui-react | 37.1MB | 36.1MB | 37.1MB | 1.0MB |
| tree-construction (items=500) | urwid | 42.0MB | 41.9MB | 42.0MB | 32.0KB |
| tree-construction (items=500) | terminal-kit | 46.8MB | 46.5MB | 46.8MB | 308.0KB |
| tree-construction (items=500) | rezitui | 167.8MB | 113.2MB | 167.8MB | 54.6MB |
| tree-construction (items=500) | ink | 188.1MB | 106.9MB | 188.1MB | 81.2MB |
| tree-construction (items=500) | blessed | 454.1MB | 260.9MB | 454.1MB | 193.2MB |
| tree-construction (items=1000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=1000) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 48.0KB |
| tree-construction (items=1000) | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 48.0KB |
| tree-construction (items=1000) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| tree-construction (items=1000) | tuix-python | 18.7MB | 18.6MB | 18.7MB | 72.0KB |
| tree-construction (items=1000) | rich | 32.1MB | 32.1MB | 32.1MB | 16.0KB |
| tree-construction (items=1000) | opentui-core | 35.0MB | 34.9MB | 35.0MB | 96.0KB |
| tree-construction (items=1000) | prompt-toolkit | 36.2MB | 36.1MB | 36.2MB | 16.0KB |
| tree-construction (items=1000) | opentui-react | 38.0MB | 37.1MB | 38.0MB | 848.0KB |
| tree-construction (items=1000) | urwid | 42.0MB | 42.0MB | 42.0MB | 16.0KB |
| tree-construction (items=1000) | terminal-kit | 55.1MB | 46.8MB | 55.1MB | 8.2MB |
| tree-construction (items=1000) | rezitui | 185.9MB | 167.8MB | 185.9MB | 18.1MB |
| tree-construction (items=1000) | ink | 355.0MB | 218.7MB | 355.0MB | 136.3MB |
| tree-construction (items=1000) | blessed | 521.4MB | 453.8MB | 521.4MB | 67.6MB |
| rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| rerender | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| rerender | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| rerender | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| rerender | tuix-python | 18.7MB | 18.7MB | 18.7MB | 0.00KB |
| rerender | rich | 32.1MB | 32.1MB | 32.1MB | 32.0KB |
| rerender | opentui-core | 35.0MB | 35.0MB | 35.0MB | 0.00KB |
| rerender | prompt-toolkit | 36.2MB | 36.2MB | 36.2MB | 32.0KB |
| rerender | opentui-react | 38.0MB | 38.0MB | 38.0MB | 44.0KB |
| rerender | urwid | 42.1MB | 42.0MB | 42.1MB | 72.0KB |
| rerender | terminal-kit | 54.9MB | 55.1MB | 54.9MB | 0.00KB |
| rerender | rezitui | 191.9MB | 186.0MB | 191.9MB | 6.0MB |
| rerender | ink | 354.9MB | 354.7MB | 354.9MB | 208.0KB |
| rerender | blessed | 520.5MB | 521.0MB | 520.5MB | 0.00KB |
| content-update | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| content-update | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| content-update | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| content-update | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| content-update | tuix-python | 18.7MB | 18.7MB | 18.7MB | 0.00KB |
| content-update | rich | 32.7MB | 32.3MB | 32.7MB | 460.0KB |
| content-update | opentui-core | 35.0MB | 35.0MB | 35.0MB | 0.00KB |
| content-update | prompt-toolkit | 36.2MB | 36.2MB | 36.2MB | 16.0KB |
| content-update | opentui-react | 38.9MB | 38.0MB | 38.9MB | 900.0KB |
| content-update | urwid | 42.1MB | 42.1MB | 42.1MB | 0.00KB |
| content-update | terminal-kit | 55.1MB | 55.0MB | 55.1MB | 68.0KB |
| content-update | rezitui | 188.0MB | 191.8MB | 188.0MB | 0.00KB |
| content-update | ink | 355.0MB | 355.0MB | 355.0MB | 0.00KB |
| content-update | blessed | 520.8MB | 523.1MB | 520.8MB | 0.00KB |
| layout-stress (cols=4,rows=10) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| layout-stress (cols=4,rows=10) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| layout-stress (cols=4,rows=10) | tuix-python | 18.7MB | 18.7MB | 18.7MB | 0.00KB |
| layout-stress (cols=4,rows=10) | rich | 32.8MB | 32.8MB | 32.8MB | 8.0KB |
| layout-stress (cols=4,rows=10) | opentui-core | 35.0MB | 35.0MB | 35.0MB | 0.00KB |
| layout-stress (cols=4,rows=10) | prompt-toolkit | 36.2MB | 36.2MB | 36.2MB | 8.0KB |
| layout-stress (cols=4,rows=10) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| layout-stress (cols=4,rows=10) | urwid | 42.1MB | 42.1MB | 42.1MB | 0.00KB |
| layout-stress (cols=4,rows=10) | terminal-kit | 55.1MB | 55.1MB | 55.1MB | 4.0KB |
| layout-stress (cols=4,rows=10) | rezitui | 188.7MB | 187.6MB | 188.7MB | 1.0MB |
| layout-stress (cols=4,rows=10) | ink | 355.7MB | 355.3MB | 355.7MB | 488.0KB |
| layout-stress (cols=4,rows=10) | blessed | 521.4MB | 526.0MB | 521.4MB | 0.00KB |
| scroll-stress (items=2000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Renderer | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| scroll-stress (items=2000) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| scroll-stress (items=2000) | tuix-python | 18.7MB | 18.7MB | 18.7MB | 4.0KB |
| scroll-stress (items=2000) | rich | 32.9MB | 32.8MB | 32.9MB | 80.0KB |
| scroll-stress (items=2000) | opentui-core | 35.1MB | 35.0MB | 35.1MB | 100.0KB |
| scroll-stress (items=2000) | prompt-toolkit | 36.2MB | 36.2MB | 36.2MB | 4.0KB |
| scroll-stress (items=2000) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| scroll-stress (items=2000) | urwid | 42.1MB | 42.1MB | 42.1MB | 0.00KB |
| scroll-stress (items=2000) | terminal-kit | 55.2MB | 55.2MB | 55.2MB | 0.00KB |
| scroll-stress (items=2000) | rezitui | 188.9MB | 188.7MB | 188.9MB | 236.0KB |
| scroll-stress (items=2000) | ink | 355.7MB | 355.7MB | 355.7MB | 4.0KB |
| scroll-stress (items=2000) | blessed | 521.2MB | 520.9MB | 521.2MB | 212.0KB |
| virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | tuix-python | 18.7MB | 18.7MB | 18.7MB | 32.0KB |
| virtual-list (items=100000,viewport=40) | bubbletea | 20.2MB | 16.2MB | 20.2MB | 4.0MB |
| virtual-list (items=100000,viewport=40) | rich | 32.9MB | 32.9MB | 32.9MB | 12.0KB |
| virtual-list (items=100000,viewport=40) | opentui-core | 35.1MB | 35.1MB | 35.1MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | prompt-toolkit | 36.3MB | 36.2MB | 36.3MB | 72.0KB |
| virtual-list (items=100000,viewport=40) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | urwid | 42.1MB | 42.1MB | 42.1MB | 64.0KB |
| virtual-list (items=100000,viewport=40) | terminal-kit | 96.1MB | 91.8MB | 96.1MB | 4.3MB |
| virtual-list (items=100000,viewport=40) | rezitui | 190.4MB | 189.2MB | 190.4MB | 1.2MB |
| virtual-list (items=100000,viewport=40) | ink | 356.7MB | 356.7MB | 356.7MB | 4.0KB |
| virtual-list (items=100000,viewport=40) | blessed | 522.6MB | 522.2MB | 522.6MB | 360.0KB |
| tables (cols=8,rows=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| tables (cols=8,rows=100) | tuix-python | 18.7MB | 18.7MB | 18.7MB | 8.0KB |
| tables (cols=8,rows=100) | bubbletea | 20.2MB | 20.2MB | 20.2MB | 0.00KB |
| tables (cols=8,rows=100) | rich | 32.9MB | 32.9MB | 32.9MB | 0.00KB |
| tables (cols=8,rows=100) | opentui-core | 35.1MB | 35.1MB | 35.1MB | 0.00KB |
| tables (cols=8,rows=100) | prompt-toolkit | 36.3MB | 36.3MB | 36.3MB | 8.0KB |
| tables (cols=8,rows=100) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| tables (cols=8,rows=100) | urwid | 42.1MB | 42.1MB | 42.1MB | 0.00KB |
| tables (cols=8,rows=100) | terminal-kit | 96.1MB | 96.1MB | 96.1MB | 32.0KB |
| tables (cols=8,rows=100) | rezitui | 189.8MB | 189.6MB | 189.8MB | 256.0KB |
| tables (cols=8,rows=100) | ink | 356.7MB | 356.7MB | 356.7MB | 0.00KB |
| tables (cols=8,rows=100) | blessed | 523.4MB | 523.4MB | 523.4MB | 0.00KB |
| memory-profile | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| memory-profile | TUIX.Renderer | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| memory-profile | TUIX.Core | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| memory-profile | tuix-python | 18.9MB | 18.8MB | 18.9MB | 84.0KB |
| memory-profile | bubbletea | 20.2MB | 20.2MB | 20.2MB | 0.00KB |
| memory-profile | rich | 32.9MB | 32.9MB | 32.9MB | 40.0KB |
| memory-profile | opentui-core | 35.1MB | 35.1MB | 35.1MB | 52.0KB |
| memory-profile | prompt-toolkit | 36.6MB | 36.3MB | 36.6MB | 320.0KB |
| memory-profile | opentui-react | 39.2MB | 38.9MB | 39.2MB | 272.0KB |
| memory-profile | urwid | 42.2MB | 42.1MB | 42.2MB | 84.0KB |
| memory-profile | terminal-kit | 95.9MB | 95.9MB | 95.9MB | 76.0KB |
| memory-profile | rezitui | 190.9MB | 190.0MB | 190.9MB | 940.0KB |
| memory-profile | ink | 356.7MB | 356.7MB | 356.7MB | 12.0KB |
| memory-profile | blessed | 522.6MB | 523.4MB | 522.6MB | 0.00KB |
| terminal-rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-rerender | TUIX.Renderer | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| terminal-rerender | TUIX.Core | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-rerender | tuix-python | 19.1MB | 19.0MB | 19.1MB | 64.0KB |
| terminal-rerender | bubbletea | 20.2MB | 20.2MB | 20.2MB | 0.00KB |
| terminal-rerender | rich | 33.0MB | 32.9MB | 33.0MB | 16.0KB |
| terminal-rerender | opentui-core | 35.9MB | 35.9MB | 35.9MB | 24.0KB |
| terminal-rerender | prompt-toolkit | 37.2MB | 36.9MB | 37.2MB | 284.0KB |
| terminal-rerender | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-rerender | urwid | 42.3MB | 42.2MB | 42.3MB | 64.0KB |
| terminal-rerender | terminal-kit | 96.1MB | 96.0MB | 96.1MB | 128.0KB |
| terminal-rerender | rezitui | 189.1MB | 190.9MB | 189.1MB | 0.00KB |
| terminal-rerender | ink | 356.7MB | 356.7MB | 356.7MB | 4.0KB |
| terminal-rerender | blessed | 522.6MB | 522.6MB | 522.6MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | TUIX.Renderer | 4.1MB | 4.1MB | 4.1MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | tuix-python | 19.1MB | 19.1MB | 19.1MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | bubbletea | 20.7MB | 20.5MB | 20.7MB | 264.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | rich | 33.1MB | 33.0MB | 33.1MB | 112.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | prompt-toolkit | 37.2MB | 37.2MB | 37.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | urwid | 42.3MB | 42.3MB | 42.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | terminal-kit | 96.1MB | 96.1MB | 96.1MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | rezitui | 191.0MB | 189.1MB | 191.0MB | 1.8MB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ink | 356.7MB | 356.7MB | 356.7MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | blessed | 522.7MB | 522.6MB | 522.7MB | 80.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | TUIX.Renderer | 4.2MB | 4.2MB | 4.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | tuix-python | 19.0MB | 19.0MB | 19.0MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | rich | 33.0MB | 33.1MB | 33.0MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | prompt-toolkit | 37.2MB | 37.2MB | 37.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | urwid | 42.3MB | 42.3MB | 42.3MB | 36.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | terminal-kit | 96.4MB | 96.4MB | 96.4MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | rezitui | 220.6MB | 191.0MB | 220.6MB | 29.6MB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ink | 356.7MB | 356.7MB | 356.7MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | blessed | 522.9MB | 522.7MB | 522.9MB | 188.0KB |
| terminal-screen-transition (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | tuix-python | 19.5MB | 19.5MB | 19.5MB | 28.0KB |
| terminal-screen-transition (cols=120,rows=40) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | rich | 33.1MB | 33.1MB | 33.1MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | prompt-toolkit | 37.2MB | 37.2MB | 37.2MB | 64.0KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | urwid | 42.4MB | 42.3MB | 42.4MB | 16.0KB |
| terminal-screen-transition (cols=120,rows=40) | terminal-kit | 96.4MB | 96.3MB | 96.4MB | 104.0KB |
| terminal-screen-transition (cols=120,rows=40) | rezitui | 219.0MB | 220.6MB | 219.0MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | ink | 356.7MB | 356.7MB | 356.7MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | blessed | 522.9MB | 522.9MB | 522.9MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | tuix-python | 19.2MB | 19.2MB | 19.2MB | 36.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | rich | 33.1MB | 33.1MB | 33.1MB | 4.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | prompt-toolkit | 37.2MB | 37.2MB | 37.2MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | urwid | 42.4MB | 42.4MB | 42.4MB | 68.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | terminal-kit | 96.6MB | 96.6MB | 96.6MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | rezitui | 222.1MB | 219.9MB | 222.1MB | 2.2MB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ink | 356.8MB | 356.8MB | 356.8MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | blessed | 522.2MB | 522.1MB | 522.2MB | 28.0KB |
| terminal-input-latency (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | tuix-python | 19.3MB | 19.2MB | 19.3MB | 64.0KB |
| terminal-input-latency (cols=120,rows=40) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 8.0KB |
| terminal-input-latency (cols=120,rows=40) | rich | 33.5MB | 33.1MB | 33.5MB | 412.0KB |
| terminal-input-latency (cols=120,rows=40) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | prompt-toolkit | 37.3MB | 37.2MB | 37.3MB | 68.0KB |
| terminal-input-latency (cols=120,rows=40) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | urwid | 42.5MB | 42.5MB | 42.5MB | 4.0KB |
| terminal-input-latency (cols=120,rows=40) | terminal-kit | 96.5MB | 96.4MB | 96.5MB | 48.0KB |
| terminal-input-latency (cols=120,rows=40) | rezitui | 220.6MB | 222.1MB | 220.6MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | ink | 356.7MB | 356.6MB | 356.7MB | 28.0KB |
| terminal-input-latency (cols=120,rows=40) | blessed | 522.4MB | 522.2MB | 522.4MB | 216.0KB |
| terminal-memory-soak (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | tuix-python | 19.3MB | 19.2MB | 19.3MB | 64.0KB |
| terminal-memory-soak (cols=120,rows=40) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | rich | 33.7MB | 33.4MB | 33.7MB | 272.0KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | prompt-toolkit | 37.3MB | 37.3MB | 37.3MB | 8.0KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | urwid | 42.5MB | 42.5MB | 42.5MB | 32.0KB |
| terminal-memory-soak (cols=120,rows=40) | terminal-kit | 96.6MB | 96.6MB | 96.6MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | rezitui | 220.7MB | 220.6MB | 220.7MB | 80.0KB |
| terminal-memory-soak (cols=120,rows=40) | ink | 356.7MB | 356.7MB | 356.7MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | blessed | 522.4MB | 522.4MB | 522.4MB | 24.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | tuix-python | 19.3MB | 19.3MB | 19.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | rich | 33.6MB | 33.7MB | 33.6MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | prompt-toolkit | 37.4MB | 37.3MB | 37.4MB | 64.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | urwid | 42.6MB | 42.5MB | 42.6MB | 104.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | terminal-kit | 97.0MB | 97.0MB | 97.0MB | 4.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | rezitui | 221.3MB | 221.4MB | 221.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | ink | 357.1MB | 357.0MB | 357.1MB | 184.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | blessed | 522.5MB | 522.5MB | 522.5MB | 28.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.6MB | 4.6MB | 4.6MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | tuix-python | 19.4MB | 19.3MB | 19.4MB | 64.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rich | 33.7MB | 33.6MB | 33.7MB | 60.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | prompt-toolkit | 37.4MB | 37.4MB | 37.4MB | 64.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | urwid | 42.7MB | 42.6MB | 42.7MB | 52.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 97.0MB | 97.0MB | 97.0MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 226.5MB | 221.3MB | 226.5MB | 5.2MB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 357.2MB | 357.2MB | 357.2MB | 8.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 522.5MB | 522.5MB | 522.5MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | tuix-python | 19.4MB | 19.4MB | 19.4MB | 4.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | rich | 33.7MB | 33.7MB | 33.7MB | 40.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | prompt-toolkit | 37.4MB | 37.4MB | 37.4MB | 20.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | urwid | 42.5MB | 42.7MB | 42.5MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | terminal-kit | 97.1MB | 97.1MB | 97.1MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | rezitui | 219.6MB | 226.5MB | 219.6MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ink | 357.2MB | 357.2MB | 357.2MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | blessed | 522.5MB | 522.5MB | 522.5MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.5MB | 4.5MB | 4.5MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | tuix-python | 19.4MB | 19.4MB | 19.4MB | 32.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rich | 33.8MB | 33.7MB | 33.8MB | 72.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | prompt-toolkit | 37.5MB | 37.4MB | 37.5MB | 100.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | urwid | 42.6MB | 42.5MB | 42.6MB | 36.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 97.1MB | 97.1MB | 97.1MB | 56.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 220.8MB | 219.6MB | 220.8MB | 1.2MB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 357.2MB | 357.2MB | 357.2MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 522.5MB | 522.5MB | 522.5MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Renderer | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | tuix-python | 19.5MB | 19.5MB | 19.5MB | 28.0KB |
| terminal-virtual-list (items=100000,viewport=40) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | rich | 34.0MB | 33.8MB | 34.0MB | 216.0KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | prompt-toolkit | 37.5MB | 37.5MB | 37.5MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | urwid | 42.6MB | 42.6MB | 42.6MB | 36.0KB |
| terminal-virtual-list (items=100000,viewport=40) | terminal-kit | 128.9MB | 128.6MB | 128.9MB | 324.0KB |
| terminal-virtual-list (items=100000,viewport=40) | rezitui | 220.7MB | 220.8MB | 220.7MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | ink | 358.1MB | 358.1MB | 358.1MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | blessed | 523.4MB | 524.8MB | 523.4MB | 0.00KB |
| terminal-table (cols=8,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Renderer | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-table (cols=8,rows=40) | tuix-python | 19.5MB | 19.5MB | 19.5MB | 12.0KB |
| terminal-table (cols=8,rows=40) | bubbletea | 20.7MB | 20.7MB | 20.7MB | 0.00KB |
| terminal-table (cols=8,rows=40) | rich | 34.0MB | 34.0MB | 34.0MB | 0.00KB |
| terminal-table (cols=8,rows=40) | opentui-core | 36.9MB | 36.9MB | 36.9MB | 0.00KB |
| terminal-table (cols=8,rows=40) | prompt-toolkit | 37.5MB | 37.5MB | 37.5MB | 0.00KB |
| terminal-table (cols=8,rows=40) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| terminal-table (cols=8,rows=40) | urwid | 42.7MB | 42.6MB | 42.7MB | 24.0KB |
| terminal-table (cols=8,rows=40) | terminal-kit | 129.0MB | 129.0MB | 129.0MB | 8.0KB |
| terminal-table (cols=8,rows=40) | rezitui | 223.6MB | 221.8MB | 223.6MB | 1.8MB |
| terminal-table (cols=8,rows=40) | ink | 357.5MB | 357.5MB | 357.5MB | 0.00KB |
| terminal-table (cols=8,rows=40) | blessed | 522.8MB | 522.8MB | 522.8MB | 56.0KB |

