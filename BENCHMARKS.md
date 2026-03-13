# Benchmark Results

> 2026-03-11T23:59:22.081Z | Node v22.19.0 | Windows_NT 10.0.26200 | win32 x64 | 12th Gen Intel(R) Core(TM) i5-12450HX (12 cores) | RAM 24284MB
> Libraries tested: blessed, terminal-kit, Ink, ReziTUI, Bubble Tea, Ratatui, OpenTUI.Core, OpenTUI.React, TUIX.Core, TUIX.Python | Missing: TUIX.Renderer
> Byte columns: "Bytes(local)" = bytes produced per frame (framework counter); "Bytes(pty)" = observed PTY bytes (cross-framework comparable).

## startup

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 152µs | 45.1% | 139µs–166µs | 6.6K ops/s | 16.88ms | 27.0KB | 0.00KB | 0.00KB | 0.00KB |
| rezitui | 1 | 165µs | 47.2% | 150µs–180µs | 6.1K ops/s | 16.65ms | 76.1MB | 8.0MB | 133.7KB | 1.3KB |
| TUIX.Core | 1 | 302µs | 36.4% | 280µs–324µs | 3.3K ops/s | 30.21ms | 3.9MB | 4.0KB | 6.0MB | 61.7KB |
| TUIX.Py | 1 | 361µs | 30.7% | 339µs–382µs | 2.8K ops/s | 36.14ms | 18.1MB | 56.0KB | 6.7MB | 68.3KB |
| opentui-react | 1 | 441µs | 62.6% | 387µs–495µs | 2.3K ops/s | 44.17ms | 36.5MB | 1.4MB | 0.00KB | 0.00KB |
| opentui-core | 1 | 451µs | 59.8% | 398µs–504µs | 2.2K ops/s | 45.21ms | 33.8MB | 624.0KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 574µs | 29.5% | 541µs–607µs | 1.7K ops/s | 57.62ms | 46.3MB | 0.00KB | 190.0KB | 1.9KB |
| ink | 1 | 2.49ms | 24.1% | 2.38ms–2.61ms | 401 ops/s | 249.46ms | 64.7MB | 2.7MB | 189.7KB | 1.9KB |
| bubbletea | 1 | 11.13ms | 31.1% | 10.46ms–11.81ms | 90 ops/s | 1.11s | 11.9MB | 304.0KB | 7.0MB | 72.1KB |
| blessed | 1 | 41.14ms | 22.7% | 39.31ms–42.97ms | 24 ops/s | 4.11s | 152.1MB | 74.1MB | 229.2KB | 2.3KB |

## tree-construction (items=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 55µs | 9.0% | 54µs–55µs | 18.2K ops/s | 27.46ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| opentui-react | 1 | 61µs | 40.0% | 59µs–64µs | 16.3K ops/s | 31.65ms | 36.5MB | 0.00KB | 3.6MB | 7.5KB |
| blessed | 1 | 93µs | 229.6% | 74µs–112µs | 10.8K ops/s | 46.67ms | 160.8MB | 6.8MB | 126.0KB | 258B |
| TUIX.Core | 1 | 110µs | 39.4% | 106µs–114µs | 9.1K ops/s | 55.01ms | 3.9MB | 0.00KB | 555.7KB | 1.1KB |
| rezitui | 1 | 120µs | 277.5% | 91µs–149µs | 8.4K ops/s | 60.18ms | 100.5MB | 18.4MB | 668.5KB | 1.3KB |
| TUIX.Py | 1 | 124µs | 28.5% | 121µs–127µs | 8.0K ops/s | 62.42ms | 18.1MB | 16.0KB | 7.7MB | 15.7KB |
| opentui-core | 1 | 306µs | 76.6% | 286µs–327µs | 3.3K ops/s | 158.26ms | 34.9MB | 1.1MB | 33.4MB | 68.3KB |
| terminal-kit | 1 | 317µs | 35.0% | 307µs–327µs | 3.2K ops/s | 158.83ms | 46.6MB | 4.0KB | 950.2KB | 1.9KB |
| ink | 1 | 512µs | 35.3% | 496µs–527µs | 2.0K ops/s | 256.15ms | 68.4MB | 2.0MB | 23.9KB | 49B |
| bubbletea | 1 | 4.62ms | 70.8% | 4.33ms–4.90ms | 217 ops/s | 2.31s | 11.9MB | 0.00KB | 14.5MB | 29.7KB |

## tree-construction (items=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 105µs | 30.9% | 102µs–108µs | 9.5K ops/s | 52.53ms | 3.9MB | 0.00KB | 1.2MB | 2.5KB |
| rezitui | 1 | 113µs | 293.8% | 84µs–142µs | 8.9K ops/s | 56.51ms | 110.9MB | 12.0MB | 668.5KB | 1.3KB |
| opentui-react | 1 | 118µs | 28.9% | 115µs–121µs | 8.5K ops/s | 64.39ms | 36.5MB | 0.00KB | 8.9MB | 18.3KB |
| ratatui | 1 | 145µs | 8.4% | 144µs–146µs | 6.9K ops/s | 72.65ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| opentui-core | 1 | 146µs | 201.1% | 120µs–172µs | 6.9K ops/s | 106.09ms | 35.6MB | 728.0KB | 33.8MB | 69.2KB |
| TUIX.Py | 1 | 151µs | 35.1% | 146µs–155µs | 6.6K ops/s | 75.53ms | 18.2MB | 16.0KB | 17.2MB | 35.2KB |
| blessed | 1 | 436µs | 123.7% | 388µs–483µs | 2.3K ops/s | 218.14ms | 240.7MB | 76.0MB | 490.2KB | 1004B |
| terminal-kit | 1 | 1.74ms | 42.3% | 1.67ms–1.80ms | 576 ops/s | 870.30ms | 46.7MB | 28.0KB | 950.2KB | 1.9KB |
| ink | 1 | 4.24ms | 28.7% | 4.14ms–4.35ms | 236 ops/s | 2.12s | 104.1MB | 19.3MB | 287.6KB | 589B |
| bubbletea | 1 | 45.46ms | 8.3% | 45.13ms–45.79ms | 22 ops/s | 22.73s | 12.0MB | 72.0KB | 35.0MB | 71.8KB |

## tree-construction (items=500)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 104µs | 16.3% | 102µs–105µs | 9.6K ops/s | 51.93ms | 3.9MB | 0.00KB | 1.2MB | 2.5KB |
| opentui-core | 1 | 143µs | 66.4% | 135µs–152µs | 7.0K ops/s | 234.76ms | 35.6MB | 0.00KB | 33.7MB | 68.9KB |
| TUIX.Py | 1 | 146µs | 45.9% | 140µs–152µs | 6.8K ops/s | 73.37ms | 18.2MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-react | 1 | 225µs | 196.0% | 187µs–264µs | 4.4K ops/s | 151.09ms | 36.5MB | 0.00KB | 8.9MB | 18.2KB |
| rezitui | 1 | 304µs | 307.9% | 222µs–387µs | 3.3K ops/s | 152.39ms | 168.7MB | 57.6MB | 668.5KB | 1.3KB |
| ratatui | 1 | 505µs | 8.3% | 501µs–508µs | 2.0K ops/s | 252.39ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| blessed | 1 | 2.24ms | 66.1% | 2.11ms–2.37ms | 447 ops/s | 1.12s | 453.6MB | 192.2MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 5.34ms | 27.4% | 5.21ms–5.47ms | 187 ops/s | 2.67s | 46.9MB | 180.0KB | 950.2KB | 1.9KB |
| ink | 1 | 21.37ms | 23.1% | 20.93ms–21.80ms | 47 ops/s | 10.69s | 193.9MB | 88.7MB | 1.6MB | 3.3KB |
| bubbletea | 1 | 308.19ms | 27.4% | 300.80ms–315.59ms | 3 ops/s | 154.10s | 12.0MB | 8.0KB | 35.0MB | 71.8KB |

## tree-construction (items=1000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 97µs | 43.5% | 93µs–101µs | 10.3K ops/s | 48.54ms | 4.0MB | 96.0KB | 1.2MB | 2.5KB |
| opentui-core | 1 | 123µs | 73.8% | 115µs–131µs | 8.1K ops/s | 447.77ms | 35.6MB | 0.00KB | 33.8MB | 69.3KB |
| TUIX.Py | 1 | 167µs | 38.0% | 161µs–172µs | 6.0K ops/s | 83.57ms | 18.2MB | 16.0KB | 17.2MB | 35.2KB |
| opentui-react | 1 | 271µs | 30.7% | 263µs–278µs | 3.7K ops/s | 178.79ms | 36.6MB | 148.0KB | 8.9MB | 18.2KB |
| rezitui | 1 | 456µs | 234.0% | 362µs–549µs | 2.2K ops/s | 227.89ms | 180.0MB | 11.2MB | 668.5KB | 1.3KB |
| ratatui | 1 | 962µs | 10.3% | 953µs–971µs | 1.0K ops/s | 481.06ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| blessed | 1 | 4.24ms | 47.5% | 4.07ms–4.42ms | 236 ops/s | 2.12s | 528.4MB | 74.6MB | 508.8KB | 1.0KB |
| terminal-kit | 1 | 10.08ms | 21.1% | 9.89ms–10.26ms | 99 ops/s | 5.04s | 55.0MB | 8.1MB | 950.2KB | 1.9KB |
| ink | 1 | 42.13ms | 19.1% | 41.42ms–42.83ms | 24 ops/s | 21.06s | 358.4MB | 139.7MB | 3.3MB | 6.7KB |
| bubbletea | 1 | 562.95ms | 28.2% | 549.05ms–576.84ms | 2 ops/s | 281.47s | 16.0MB | 4.0MB | 35.0MB | 71.8KB |

## rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 10µs | 5.9% | 10µs–10µs | 96.1K ops/s | 10.76ms | 18.3MB | 88.0KB | 1.8MB | 1.9KB |
| blessed | 1 | 31µs | 32.2% | 30µs–31µs | 32.7K ops/s | 30.72ms | 527.7MB | 0.00KB | 0.00KB | 0.00KB |
| TUIX.Core | 1 | 37µs | 44.9% | 36µs–38µs | 26.7K ops/s | 37.49ms | 4.0MB | 0.00KB | 15.6KB | 16B |
| ratatui | 1 | 50µs | 41.0% | 48µs–51µs | 20.1K ops/s | 49.84ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| rezitui | 1 | 71µs | 335.0% | 56µs–86µs | 14.1K ops/s | 71.05ms | 187.6MB | 7.6MB | 1.3MB | 1.3KB |
| opentui-core | 1 | 126µs | 57.9% | 122µs–131µs | 7.9K ops/s | 126.67ms | 35.6MB | 0.00KB | 66.9MB | 68.5KB |
| terminal-kit | 1 | 182µs | 33.1% | 178µs–186µs | 5.5K ops/s | 181.92ms | 54.8MB | 0.00KB | 1.9MB | 1.9KB |
| opentui-react | 1 | 184µs | 36.4% | 180µs–188µs | 5.4K ops/s | 192.92ms | 38.7MB | 876.0KB | 73.5MB | 75.3KB |
| ink | 1 | 552µs | 37.4% | 539µs–565µs | 1.8K ops/s | 552.48ms | 358.0MB | 0.00KB | 1.9MB | 1.9KB |
| bubbletea | 1 | 10.98ms | 29.8% | 10.78ms–11.18ms | 91 ops/s | 10.98s | 16.0MB | 0.00KB | 70.4MB | 72.1KB |

## content-update

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 65µs | 266.9% | 50µs–80µs | 15.4K ops/s | 32.45ms | 182.6MB | 0.00KB | 668.5KB | 1.3KB |
| ratatui | 1 | 104µs | 21.6% | 102µs–106µs | 9.6K ops/s | 51.90ms | 27.0KB | 0.00KB | 7.8MB | 16.0KB |
| TUIX.Py | 1 | 122µs | 43.0% | 118µs–127µs | 8.2K ops/s | 70.47ms | 18.3MB | 0.00KB | 17.1MB | 35.0KB |
| TUIX.Core | 1 | 136µs | 38.3% | 132µs–141µs | 7.3K ops/s | 68.26ms | 4.0MB | 0.00KB | 6.5MB | 13.4KB |
| opentui-core | 1 | 158µs | 51.0% | 151µs–165µs | 6.3K ops/s | 83.27ms | 35.6MB | 0.00KB | 33.4MB | 68.5KB |
| opentui-react | 1 | 233µs | 46.2% | 224µs–243µs | 4.3K ops/s | 119.83ms | 38.9MB | 4.0KB | 36.7MB | 75.3KB |
| terminal-kit | 1 | 251µs | 26.2% | 245µs–257µs | 4.0K ops/s | 125.72ms | 54.8MB | 8.0KB | 950.2KB | 1.9KB |
| blessed | 1 | 350µs | 63.0% | 330µs–369µs | 2.9K ops/s | 174.96ms | 528.2MB | 0.00KB | 1.1MB | 2.3KB |
| ink | 1 | 2.96ms | 35.4% | 2.87ms–3.05ms | 338 ops/s | 1.48s | 358.2MB | 532.0KB | 953.1KB | 1.9KB |
| bubbletea | 1 | 10.60ms | 32.0% | 10.30ms–10.90ms | 94 ops/s | 5.30s | 16.0MB | 0.00KB | 34.9MB | 71.5KB |

## layout-stress (cols=4,rows=10)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 52µs | 18.3% | 51µs–53µs | 19.3K ops/s | 17.42ms | 18.3MB | 0.00KB | 1.1MB | 3.6KB |
| opentui-react | 1 | 56µs | 18.1% | 55µs–58µs | 17.7K ops/s | 19.46ms | 38.9MB | 0.00KB | 1.2KB | 4B |
| TUIX.Core | 1 | 143µs | 30.2% | 138µs–148µs | 7.0K ops/s | 42.91ms | 4.3MB | 0.00KB | 1.1MB | 3.6KB |
| blessed | 1 | 215µs | 189.9% | 169µs–261µs | 4.7K ops/s | 64.49ms | 528.8MB | 0.00KB | 0.00KB | 0.00KB |
| ink | 1 | 5.01ms | 21.7% | 4.89ms–5.14ms | 199 ops/s | 1.50s | 358.0MB | 0.00KB | 1.2MB | 4.2KB |
| bubbletea | 1 | 9.79ms | 33.9% | 9.42ms–10.17ms | 102 ops/s | 2.94s | 16.0MB | 0.00KB | 20.9MB | 71.3KB |

## scroll-stress (items=2000)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 56µs | 23.9% | 53µs–60µs | 17.7K ops/s | 2.83ms | 183.4MB | 92.0KB | 66.8KB | 1.3KB |
| TUIX.Core | 1 | 66µs | 43.3% | 58µs–74µs | 15.2K ops/s | 3.29ms | 4.3MB | 0.00KB | 800B | 16B |
| TUIX.Py | 1 | 106µs | 3.9% | 105µs–108µs | 9.4K ops/s | 5.46ms | 18.3MB | 0.00KB | 1.6MB | 32.4KB |
| ratatui | 1 | 109µs | 5.5% | 107µs–110µs | 9.2K ops/s | 5.45ms | 27.0KB | 0.00KB | 798.0KB | 16.0KB |
| opentui-core | 1 | 177µs | 64.5% | 146µs–209µs | 5.6K ops/s | 9.84ms | 35.8MB | 124.0KB | 3.1MB | 62.8KB |
| opentui-react | 1 | 189µs | 36.4% | 170µs–208µs | 5.3K ops/s | 10.52ms | 39.0MB | 0.00KB | 3.8MB | 77.3KB |
| terminal-kit | 1 | 636µs | 23.4% | 595µs–677µs | 1.6K ops/s | 31.82ms | 55.1MB | 0.00KB | 95.0KB | 1.9KB |
| ink | 1 | 1.33ms | 62.7% | 1.10ms–1.57ms | 749 ops/s | 66.75ms | 358.0MB | 64.0KB | 93.2KB | 1.9KB |
| blessed | 1 | 4.28ms | 36.9% | 3.85ms–4.72ms | 233 ops/s | 214.30ms | 530.1MB | 1.5MB | 115.5KB | 2.3KB |
| bubbletea | 1 | 10.00ms | 35.6% | 9.01ms–10.99ms | 100 ops/s | 500.02ms | 16.0MB | 0.00KB | 3.5MB | 71.3KB |

## virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 66µs | 226.1% | 57µs–75µs | 15.2K ops/s | 66.06ms | 188.4MB | 4.6MB | 2.4MB | 2.5KB |
| TUIX.Py | 1 | 70µs | 26.2% | 69µs–71µs | 14.4K ops/s | 75.19ms | 18.3MB | 0.00KB | 2.2MB | 2.3KB |
| TUIX.Core | 1 | 89µs | 41.6% | 87µs–91µs | 11.2K ops/s | 89.02ms | 4.4MB | 0.00KB | 2.2MB | 2.2KB |
| opentui-core | 1 | 99µs | 49.4% | 96µs–102µs | 10.1K ops/s | 126.39ms | 35.8MB | 0.00KB | 66.1MB | 67.7KB |
| ratatui | 1 | 100µs | 33.6% | 98µs–102µs | 10.0K ops/s | 100.06ms | 27.0KB | 0.00KB | 15.6MB | 16.0KB |
| blessed | 1 | 121µs | 372.9% | 93µs–149µs | 8.2K ops/s | 121.38ms | 530.2MB | 0.00KB | 616.2KB | 631B |
| opentui-react | 1 | 197µs | 43.5% | 192µs–202µs | 5.1K ops/s | 213.17ms | 39.0MB | 0.00KB | 75.5MB | 77.3KB |
| terminal-kit | 1 | 434µs | 29.8% | 426µs–442µs | 2.3K ops/s | 434.71ms | 97.5MB | 6.9MB | 1.9MB | 1.9KB |
| ink | 1 | 683µs | 38.5% | 667µs–700µs | 1.5K ops/s | 683.57ms | 359.2MB | 0.00KB | 153.3KB | 157B |
| bubbletea | 1 | 9.75ms | 35.0% | 9.54ms–9.96ms | 103 ops/s | 9.75s | 16.0MB | 0.00KB | 69.6MB | 71.3KB |

## tables (cols=8,rows=100)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 52µs | 8.0% | 51µs–52µs | 19.3K ops/s | 18.27ms | 18.3MB | 8.0KB | 4.2MB | 14.3KB |
| ratatui | 1 | 82µs | 18.1% | 81µs–84µs | 12.2K ops/s | 24.69ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| TUIX.Core | 1 | 120µs | 24.6% | 116µs–123µs | 8.4K ops/s | 35.90ms | 4.4MB | 0.00KB | 1.5MB | 5.2KB |
| opentui-core | 1 | 157µs | 57.8% | 147µs–168µs | 6.4K ops/s | 58.48ms | 35.8MB | 0.00KB | 19.8MB | 67.7KB |
| opentui-react | 1 | 193µs | 44.8% | 183µs–203µs | 5.2K ops/s | 61.45ms | 39.2MB | 0.00KB | 20.6MB | 70.4KB |
| blessed | 1 | 357µs | 57.6% | 334µs–380µs | 2.8K ops/s | 107.15ms | 529.7MB | 92.0KB | 821.5KB | 2.7KB |
| terminal-kit | 1 | 524µs | 37.0% | 502µs–546µs | 1.9K ops/s | 157.26ms | 97.5MB | 56.0KB | 570.1KB | 1.9KB |
| rezitui | 1 | 662µs | 74.2% | 607µs–718µs | 1.5K ops/s | 198.77ms | 184.7MB | 0.00KB | 2.4MB | 8.0KB |
| ink | 1 | 701µs | 34.8% | 674µs–729µs | 1.4K ops/s | 210.57ms | 359.4MB | 0.00KB | 569.2KB | 1.9KB |
| bubbletea | 1 | 9.73ms | 35.9% | 9.33ms–10.12ms | 103 ops/s | 2.92s | 16.0MB | 0.00KB | 20.9MB | 71.3KB |

## memory-profile

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 89µs | 36.9% | 88µs–91µs | 11.2K ops/s | 178.85ms | 27.0KB | 0.00KB | 31.2MB | 16.0KB |
| rezitui | 1 | 116µs | 460.3% | 92µs–139µs | 8.6K ops/s | 232.05ms | 191.1MB | 5.9MB | 2.6MB | 1.3KB |
| TUIX.Core | 1 | 174µs | 29.3% | 172µs–176µs | 5.7K ops/s | 348.19ms | 4.4MB | 0.00KB | 120.6MB | 61.7KB |
| TUIX.Py | 1 | 176µs | 29.2% | 174µs–178µs | 5.7K ops/s | 404.41ms | 18.5MB | 116.0KB | 133.5MB | 68.3KB |
| opentui-core | 1 | 183µs | 50.7% | 179µs–187µs | 5.5K ops/s | 426.00ms | 35.8MB | 16.0KB | 133.8MB | 68.5KB |
| opentui-react | 1 | 216µs | 41.8% | 212µs–220µs | 4.6K ops/s | 439.54ms | 39.2MB | 40.0KB | 147.0MB | 75.3KB |
| blessed | 1 | 337µs | 49.8% | 330µs–344µs | 3.0K ops/s | 674.12ms | 529.5MB | 0.00KB | 4.6MB | 2.3KB |
| terminal-kit | 1 | 545µs | 36.2% | 537µs–554µs | 1.8K ops/s | 1.09s | 97.3MB | 0.00KB | 3.7MB | 1.9KB |
| ink | 1 | 733µs | 32.6% | 723µs–744µs | 1.4K ops/s | 1.47s | 359.4MB | 4.0KB | 3.7MB | 1.9KB |
| bubbletea | 1 | 11.06ms | 29.8% | 10.92ms–11.21ms | 90 ops/s | 22.12s | 16.0MB | 0.00KB | 140.8MB | 72.1KB |

## terminal-rerender

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 26µs | 9.1% | 26µs–26µs | 38.4K ops/s | 26.38ms | 18.6MB | 0.00KB | 4.6MB | 4.7KB |
| rezitui | 1 | 38µs | 335.1% | 30µs–46µs | 26.5K ops/s | 37.87ms | 189.8MB | 0.00KB | 2.2MB | 2.2KB |
| TUIX.Core | 1 | 51µs | 61.2% | 49µs–53µs | 19.5K ops/s | 51.25ms | 4.7MB | 0.00KB | 15.6KB | 16B |
| ratatui | 1 | 112µs | 36.6% | 109µs–115µs | 8.9K ops/s | 112.05ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| blessed | 1 | 190µs | 30.2% | 187µs–194µs | 5.3K ops/s | 190.48ms | 529.4MB | 4.0KB | 0.00KB | 0.00KB |
| opentui-core | 1 | 398µs | 45.2% | 387µs–409µs | 2.5K ops/s | 398.63ms | 36.2MB | 0.00KB | 167.1MB | 171.1KB |
| opentui-react | 1 | 432µs | 36.0% | 422µs–441µs | 2.3K ops/s | 440.82ms | 41.0MB | 1008.0KB | 185.4MB | 189.9KB |
| terminal-kit | 1 | 486µs | 34.9% | 476µs–497µs | 2.1K ops/s | 486.71ms | 97.5MB | 80.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.59ms | 41.3% | 1.55ms–1.63ms | 630 ops/s | 1.59s | 359.5MB | 96.0KB | 4.6MB | 4.7KB |
| bubbletea | 1 | 27.70ms | 13.5% | 27.47ms–27.93ms | 36 ops/s | 27.70s | 16.0MB | 0.00KB | 176.0MB | 180.2KB |

## terminal-frame-fill (cols=120,dirtyLines=1,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 65µs | 201.6% | 54µs–77µs | 15.4K ops/s | 32.66ms | 191.4MB | 1.6MB | 1.1MB | 2.2KB |
| ratatui | 1 | 123µs | 34.4% | 120µs–127µs | 8.1K ops/s | 61.70ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 284µs | 42.1% | 274µs–295µs | 3.5K ops/s | 143.20ms | 37.3MB | 0.00KB | 83.5MB | 171.0KB |
| blessed | 1 | 350µs | 42.1% | 337µs–363µs | 2.9K ops/s | 174.96ms | 529.6MB | 4.0KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 470µs | 26.4% | 459µs–481µs | 2.1K ops/s | 235.20ms | 97.5MB | 28.0KB | 2.3MB | 4.7KB |
| opentui-react | 1 | 478µs | 36.5% | 463µs–493µs | 2.1K ops/s | 243.80ms | 41.0MB | 0.00KB | 92.7MB | 189.9KB |
| ink | 1 | 1.78ms | 33.8% | 1.73ms–1.84ms | 561 ops/s | 892.11ms | 359.5MB | 0.00KB | 2.3MB | 4.7KB |
| bubbletea | 1 | 27.75ms | 13.5% | 27.42ms–28.08ms | 36 ops/s | 13.87s | 16.0MB | 0.00KB | 88.0MB | 180.2KB |

## terminal-frame-fill (cols=120,dirtyLines=40,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 219µs | 17.6% | 215µs–222µs | 4.6K ops/s | 109.31ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| rezitui | 1 | 257µs | 383.5% | 171µs–344µs | 3.9K ops/s | 128.68ms | 202.6MB | 11.1MB | 1.1MB | 2.2KB |
| opentui-core | 1 | 324µs | 32.9% | 315µs–334µs | 3.1K ops/s | 182.86ms | 37.3MB | 0.00KB | 83.5MB | 171.0KB |
| opentui-react | 1 | 415µs | 29.2% | 405µs–426µs | 2.4K ops/s | 212.19ms | 41.0MB | 0.00KB | 92.7MB | 189.9KB |
| blessed | 1 | 799µs | 34.7% | 774µs–823µs | 1.3K ops/s | 399.59ms | 529.9MB | 308.0KB | 2.7MB | 5.5KB |
| terminal-kit | 1 | 1.02ms | 17.3% | 1.01ms–1.04ms | 978 ops/s | 511.49ms | 97.6MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 1.46ms | 29.1% | 1.43ms–1.50ms | 684 ops/s | 731.67ms | 359.5MB | 0.00KB | 2.3MB | 4.7KB |
| bubbletea | 1 | 27.80ms | 12.4% | 27.49ms–28.10ms | 36 ops/s | 13.90s | 16.2MB | 128.0KB | 88.0MB | 180.2KB |

## terminal-screen-transition (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 170µs | 368.5% | 131µs–209µs | 5.9K ops/s | 170.30ms | 202.8MB | 0.00KB | 2.2MB | 2.2KB |
| ratatui | 1 | 219µs | 21.1% | 216µs–222µs | 4.6K ops/s | 219.08ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| opentui-core | 1 | 307µs | 48.9% | 298µs–316µs | 3.3K ops/s | 354.46ms | 37.3MB | 0.00KB | 171.1MB | 175.2KB |
| opentui-react | 1 | 405µs | 31.1% | 397µs–413µs | 2.5K ops/s | 414.09ms | 41.0MB | 0.00KB | 185.4MB | 189.9KB |
| blessed | 1 | 842µs | 37.7% | 822µs–862µs | 1.2K ops/s | 842.34ms | 529.9MB | 0.00KB | 5.3MB | 5.4KB |
| terminal-kit | 1 | 1.01ms | 20.7% | 998µs–1.02ms | 989 ops/s | 1.01s | 97.7MB | 4.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.53ms | 32.3% | 1.50ms–1.56ms | 654 ops/s | 1.53s | 359.5MB | 4.0KB | 4.6MB | 4.7KB |
| bubbletea | 1 | 26.15ms | 14.5% | 25.92ms–26.39ms | 38 ops/s | 26.15s | 16.2MB | 0.00KB | 180.1MB | 184.4KB |

## terminal-fps-stream (channels=12,cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratatui | 1 | 191µs | 13.8% | 189µs–192µs | 5.2K ops/s | 228.80ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 266µs | 40.9% | 260µs–272µs | 3.8K ops/s | 386.32ms | 37.3MB | 0.00KB | 193.0MB | 164.7KB |
| blessed | 1 | 426µs | 49.8% | 414µs–438µs | 2.3K ops/s | 510.97ms | 529.9MB | 4.0KB | 5.7MB | 4.8KB |
| opentui-react | 1 | 454µs | 36.8% | 445µs–463µs | 2.2K ops/s | 555.78ms | 41.0MB | 0.00KB | 222.5MB | 189.9KB |
| rezitui | 1 | 546µs | 133.6% | 505µs–588µs | 1.8K ops/s | 656.02ms | 203.9MB | 0.00KB | 4.2MB | 3.6KB |
| terminal-kit | 1 | 1.06ms | 25.5% | 1.05ms–1.08ms | 942 ops/s | 1.27s | 97.8MB | 8.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.57ms | 34.6% | 1.54ms–1.60ms | 638 ops/s | 1.88s | 359.7MB | 64.0KB | 2.5MB | 2.1KB |
| bubbletea | 1 | 25.97ms | 13.1% | 25.78ms–26.17ms | 39 ops/s | 31.17s | 16.2MB | 0.00KB | 203.2MB | 173.4KB |

## terminal-input-latency (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 61µs | 274.1% | 50µs–71µs | 16.5K ops/s | 60.92ms | 205.5MB | 1.6MB | 2.2MB | 2.2KB |
| ratatui | 1 | 145µs | 34.0% | 142µs–148µs | 6.9K ops/s | 145.38ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| opentui-core | 1 | 323µs | 42.6% | 315µs–332µs | 3.1K ops/s | 324.15ms | 37.3MB | 0.00KB | 166.6MB | 170.6KB |
| blessed | 1 | 401µs | 46.1% | 390µs–413µs | 2.5K ops/s | 401.23ms | 530.0MB | 24.0KB | 0.00KB | 0.00KB |
| terminal-kit | 1 | 503µs | 32.6% | 493µs–513µs | 2.0K ops/s | 503.30ms | 97.9MB | 240.0KB | 4.6MB | 4.7KB |
| opentui-react | 1 | 523µs | 42.9% | 509µs–537µs | 1.9K ops/s | 533.39ms | 41.0MB | 0.00KB | 185.4MB | 189.9KB |
| ink | 1 | 1.70ms | 32.0% | 1.67ms–1.73ms | 588 ops/s | 1.70s | 359.7MB | 0.00KB | 4.6MB | 4.7KB |
| bubbletea | 1 | 27.59ms | 13.2% | 27.37ms–27.82ms | 36 ops/s | 27.59s | 16.2MB | 0.00KB | 175.9MB | 180.2KB |

## terminal-memory-soak (cols=120,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 204µs | 386.9% | 160µs–249µs | 4.9K ops/s | 245.42ms | 211.3MB | 5.8MB | 2.6MB | 2.2KB |
| ratatui | 1 | 222µs | 31.4% | 218µs–226µs | 4.5K ops/s | 266.91ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 411µs | 38.6% | 402µs–420µs | 2.4K ops/s | 551.75ms | 37.3MB | 0.00KB | 200.4MB | 171.0KB |
| TUIX.Py | 1 | 425µs | 29.0% | 418µs–432µs | 2.4K ops/s | 585.94ms | 19.3MB | 0.00KB | 200.1MB | 170.8KB |
| opentui-react | 1 | 462µs | 48.4% | 450µs–475µs | 2.2K ops/s | 567.19ms | 41.0MB | 0.00KB | 222.5MB | 189.9KB |
| TUIX.Core | 1 | 550µs | 35.8% | 539µs–561µs | 1.8K ops/s | 660.00ms | 4.9MB | 0.00KB | 183.8MB | 156.8KB |
| blessed | 1 | 883µs | 38.9% | 864µs–903µs | 1.1K ops/s | 1.06s | 530.0MB | 0.00KB | 6.5MB | 5.5KB |
| terminal-kit | 1 | 1.34ms | 28.4% | 1.32ms–1.36ms | 747 ops/s | 1.61s | 98.0MB | 16.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.54ms | 27.8% | 1.52ms–1.57ms | 648 ops/s | 1.85s | 359.7MB | 0.00KB | 5.5MB | 4.7KB |
| bubbletea | 1 | 27.73ms | 15.8% | 27.48ms–27.97ms | 36 ops/s | 33.27s | 16.2MB | 0.00KB | 211.1MB | 180.2KB |

## terminal-full-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 111µs | 48.5% | 108µs–114µs | 9.0K ops/s | 133.47ms | 4.9MB | 124.0KB | 3.9MB | 3.3KB |
| ratatui | 1 | 204µs | 39.7% | 199µs–208µs | 4.9K ops/s | 244.81ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 305µs | 44.7% | 298µs–313µs | 3.3K ops/s | 405.92ms | 37.3MB | 0.00KB | 204.3MB | 174.4KB |
| opentui-react | 1 | 339µs | 57.2% | 328µs–350µs | 3.0K ops/s | 428.24ms | 41.0MB | 0.00KB | 149.5MB | 127.6KB |
| blessed | 1 | 659µs | 46.8% | 641µs–676µs | 1.5K ops/s | 790.69ms | 530.0MB | 4.0KB | 3.8MB | 3.2KB |
| rezitui | 1 | 817µs | 79.1% | 780µs–853µs | 1.2K ops/s | 980.99ms | 214.0MB | 2.0MB | 4.3MB | 3.7KB |
| terminal-kit | 1 | 865µs | 22.9% | 854µs–876µs | 1.2K ops/s | 1.04s | 98.2MB | 8.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.53ms | 32.7% | 1.51ms–1.56ms | 652 ops/s | 1.84s | 359.9MB | 184.0KB | 3.4MB | 2.9KB |
| bubbletea | 1 | 25.09ms | 13.7% | 24.89ms–25.28ms | 40 ops/s | 30.10s | 16.2MB | 0.00KB | 214.1MB | 182.7KB |

## terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 171µs | 66.5% | 164µs–177µs | 5.9K ops/s | 204.82ms | 4.9MB | 0.00KB | 69.9MB | 59.7KB |
| ratatui | 1 | 178µs | 28.8% | 176µs–181µs | 5.6K ops/s | 214.21ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 334µs | 41.7% | 326µs–341µs | 3.0K ops/s | 411.94ms | 41.0MB | 0.00KB | 149.5MB | 127.6KB |
| opentui-core | 1 | 335µs | 51.5% | 325µs–344µs | 3.0K ops/s | 433.92ms | 37.3MB | 0.00KB | 204.3MB | 174.4KB |
| blessed | 1 | 582µs | 40.3% | 569µs–595µs | 1.7K ops/s | 698.89ms | 530.0MB | 0.00KB | 3.8MB | 3.2KB |
| rezitui | 1 | 796µs | 97.4% | 752µs–839µs | 1.3K ops/s | 955.62ms | 216.4MB | 2.4MB | 4.4MB | 3.7KB |
| terminal-kit | 1 | 952µs | 31.1% | 936µs–969µs | 1.1K ops/s | 1.14s | 98.3MB | 60.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.48ms | 29.5% | 1.45ms–1.50ms | 677 ops/s | 1.77s | 359.9MB | 0.00KB | 3.4MB | 2.9KB |
| bubbletea | 1 | 25.17ms | 15.3% | 24.96ms–25.39ms | 40 ops/s | 30.21s | 16.2MB | 0.00KB | 214.1MB | 182.7KB |

## terminal-strict-ui (cols=120,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 111µs | 34.4% | 108µs–113µs | 9.0K ops/s | 132.71ms | 4.9MB | 0.00KB | 3.9MB | 3.3KB |
| ratatui | 1 | 169µs | 21.6% | 167µs–171µs | 5.9K ops/s | 202.53ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-react | 1 | 299µs | 38.9% | 292µs–305µs | 3.3K ops/s | 370.22ms | 41.0MB | 0.00KB | 149.5MB | 127.6KB |
| opentui-core | 1 | 322µs | 40.8% | 314µs–329µs | 3.1K ops/s | 418.90ms | 37.3MB | 0.00KB | 204.3MB | 174.4KB |
| blessed | 1 | 597µs | 39.3% | 584µs–610µs | 1.7K ops/s | 716.72ms | 528.9MB | 0.00KB | 3.8MB | 3.2KB |
| rezitui | 1 | 694µs | 101.8% | 654µs–735µs | 1.4K ops/s | 834.22ms | 211.3MB | 0.00KB | 4.3MB | 3.7KB |
| terminal-kit | 1 | 887µs | 25.4% | 874µs–900µs | 1.1K ops/s | 1.06s | 98.3MB | 4.0KB | 5.5MB | 4.7KB |
| ink | 1 | 1.60ms | 32.9% | 1.57ms–1.63ms | 625 ops/s | 1.92s | 359.9MB | 0.00KB | 3.4MB | 2.9KB |
| bubbletea | 1 | 25.28ms | 14.4% | 25.08ms–25.49ms | 40 ops/s | 30.34s | 16.2MB | 0.00KB | 214.1MB | 182.7KB |

## terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Core | 1 | 153µs | 61.7% | 148µs–158µs | 6.5K ops/s | 183.74ms | 4.9MB | 0.00KB | 69.9MB | 59.7KB |
| ratatui | 1 | 173µs | 26.0% | 170µs–175µs | 5.8K ops/s | 207.26ms | 27.0KB | 0.00KB | 48.8MB | 41.6KB |
| opentui-core | 1 | 330µs | 44.2% | 322µs–339µs | 3.0K ops/s | 429.95ms | 37.3MB | 0.00KB | 204.3MB | 174.4KB |
| opentui-react | 1 | 362µs | 44.9% | 353µs–371µs | 2.8K ops/s | 446.86ms | 41.0MB | 0.00KB | 149.5MB | 127.6KB |
| blessed | 1 | 616µs | 43.3% | 601µs–631µs | 1.6K ops/s | 739.11ms | 529.1MB | 184.0KB | 3.8MB | 3.2KB |
| rezitui | 1 | 669µs | 105.8% | 629µs–710µs | 1.5K ops/s | 804.12ms | 211.7MB | 388.0KB | 4.4MB | 3.7KB |
| terminal-kit | 1 | 959µs | 29.7% | 943µs–975µs | 1.0K ops/s | 1.15s | 98.4MB | 0.00KB | 5.5MB | 4.7KB |
| ink | 1 | 1.47ms | 29.1% | 1.45ms–1.50ms | 680 ops/s | 1.77s | 359.9MB | 0.00KB | 3.4MB | 2.9KB |
| bubbletea | 1 | 43.26ms | 18.0% | 42.82ms–43.70ms | 23 ops/s | 51.91s | 16.2MB | 0.00KB | 214.1MB | 182.7KB |

## terminal-virtual-list (items=100000,viewport=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| rezitui | 1 | 66µs | 268.5% | 55µs–77µs | 15.2K ops/s | 66.16ms | 212.6MB | 956.0KB | 4.1MB | 4.2KB |
| TUIX.Py | 1 | 160µs | 21.5% | 158µs–162µs | 6.2K ops/s | 170.47ms | 19.2MB | 64.0KB | 5.2MB | 5.3KB |
| TUIX.Core | 1 | 194µs | 35.8% | 189µs–198µs | 5.2K ops/s | 193.76ms | 4.9MB | 0.00KB | 5.2MB | 5.3KB |
| ratatui | 1 | 215µs | 29.0% | 211µs–218µs | 4.7K ops/s | 214.71ms | 27.0KB | 0.00KB | 40.6MB | 41.6KB |
| blessed | 1 | 215µs | 102.8% | 201µs–228µs | 4.7K ops/s | 214.99ms | 530.2MB | 8.0KB | 1.0MB | 1.0KB |
| opentui-core | 1 | 298µs | 43.2% | 290µs–305µs | 3.4K ops/s | 346.14ms | 37.3MB | 0.00KB | 165.1MB | 169.0KB |
| opentui-react | 1 | 597µs | 26.6% | 587µs–607µs | 1.7K ops/s | 622.55ms | 41.0MB | 0.00KB | 194.3MB | 199.0KB |
| terminal-kit | 1 | 1.19ms | 28.2% | 1.17ms–1.21ms | 842 ops/s | 1.19s | 129.0MB | 364.0KB | 4.6MB | 4.7KB |
| ink | 1 | 1.57ms | 33.2% | 1.54ms–1.60ms | 638 ops/s | 1.57s | 360.8MB | 0.00KB | 262.7KB | 269B |
| bubbletea | 1 | 37.34ms | 31.2% | 36.61ms–38.06ms | 27 ops/s | 37.34s | 16.2MB | 0.00KB | 174.0MB | 178.2KB |

## terminal-table (cols=8,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 157µs | 32.8% | 152µs–161µs | 6.4K ops/s | 93.29ms | 19.3MB | 0.00KB | 12.6MB | 25.8KB |
| TUIX.Core | 1 | 227µs | 36.5% | 220µs–235µs | 4.4K ops/s | 113.70ms | 4.9MB | 0.00KB | 5.1MB | 10.5KB |
| ratatui | 1 | 238µs | 28.4% | 232µs–243µs | 4.2K ops/s | 118.83ms | 27.0KB | 0.00KB | 20.3MB | 41.6KB |
| opentui-core | 1 | 344µs | 59.0% | 326µs–361µs | 2.9K ops/s | 196.58ms | 37.3MB | 0.00KB | 82.5MB | 169.0KB |
| opentui-react | 1 | 405µs | 34.5% | 393µs–418µs | 2.5K ops/s | 215.65ms | 41.0MB | 0.00KB | 91.4MB | 187.3KB |
| rezitui | 1 | 865µs | 61.7% | 818µs–912µs | 1.2K ops/s | 432.72ms | 214.6MB | 2.6MB | 7.6MB | 15.6KB |
| blessed | 1 | 891µs | 45.2% | 856µs–926µs | 1.1K ops/s | 445.69ms | 530.2MB | 0.00KB | 3.0MB | 6.2KB |
| terminal-kit | 1 | 1.17ms | 30.3% | 1.14ms–1.20ms | 854 ops/s | 585.54ms | 129.2MB | 0.00KB | 2.3MB | 4.7KB |
| ink | 1 | 1.62ms | 29.8% | 1.57ms–1.66ms | 619 ops/s | 808.53ms | 360.5MB | 0.00KB | 2.3MB | 4.7KB |
| bubbletea | 1 | 32.06ms | 28.3% | 31.26ms–32.85ms | 31 ops/s | 16.03s | 16.2MB | 0.00KB | 87.0MB | 178.2KB |

## layout-stress (cols=4,rows=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| opentui-core | 1 | 87µs | 38.7% | 83µs–91µs | 11.5K ops/s | 34.16ms | 35.6MB | 0.00KB | 19.8MB | 67.7KB |
| ratatui | 1 | 111µs | 21.1% | 108µs–113µs | 9.0K ops/s | 33.26ms | 27.0KB | 0.00KB | 4.7MB | 16.0KB |
| terminal-kit | 1 | 486µs | 32.9% | 468µs–505µs | 2.1K ops/s | 146.01ms | 55.0MB | 0.00KB | 570.1KB | 1.9KB |
| rezitui | 1 | 1.26ms | 64.2% | 1.17ms–1.35ms | 796 ops/s | 377.16ms | 183.4MB | 1.9MB | 1.6MB | 5.6KB |

## terminal-frame-fill (dirty=1)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 35µs | 25.7% | 34µs–35µs | 28.9K ops/s | 36.23ms | 18.6MB | 64.0KB | 45.2MB | 46.2KB |
| TUIX.Core | 1 | 66µs | 34.8% | 65µs–67µs | 15.2K ops/s | 66.00ms | 4.7MB | 0.00KB | 2.3MB | 2.4KB |

## terminal-frame-fill (dirty=40)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 332µs | 29.2% | 326µs–338µs | 3.0K ops/s | 370.19ms | 18.6MB | 0.00KB | 85.8MB | 87.8KB |
| TUIX.Core | 1 | 395µs | 34.8% | 387µs–404µs | 2.5K ops/s | 395.19ms | 4.7MB | 0.00KB | 83.5MB | 85.5KB |

## terminal-screen-transition

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 366µs | 51.7% | 349µs–382µs | 2.7K ops/s | 205.92ms | 18.7MB | 0.00KB | 83.4MB | 170.8KB |
| TUIX.Core | 1 | 367µs | 57.8% | 349µs–386µs | 2.7K ops/s | 183.58ms | 4.8MB | 0.00KB | 76.6MB | 156.8KB |

## terminal-fps-stream (services=12)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 71µs | 26.8% | 70µs–72µs | 14.1K ops/s | 146.22ms | 18.8MB | 64.0KB | 14.1MB | 7.2KB |
| TUIX.Core | 1 | 100µs | 37.0% | 99µs–102µs | 10.0K ops/s | 200.65ms | 4.8MB | 0.00KB | 3.9MB | 2.0KB |

## terminal-input-latency

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 33µs | 32.5% | 32µs–34µs | 30.2K ops/s | 33.96ms | 18.9MB | 64.0KB | 4.6MB | 4.7KB |
| TUIX.Core | 1 | 107µs | 96.7% | 101µs–113µs | 9.4K ops/s | 107.53ms | 4.8MB | 0.00KB | 52.7KB | 54B |

## terminal-full-ui (services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 83µs | 19.7% | 82µs–84µs | 12.0K ops/s | 107.12ms | 19.0MB | 64.0KB | 6.5MB | 5.6KB |

## terminal-full-ui-navigation (dwell=8,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 105µs | 57.9% | 102µs–109µs | 9.5K ops/s | 143.25ms | 19.1MB | 28.0KB | 74.7MB | 63.7KB |

## terminal-strict-ui (services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 93µs | 26.6% | 92µs–94µs | 10.7K ops/s | 119.71ms | 19.1MB | 40.0KB | 6.5MB | 5.6KB |

## terminal-strict-ui-navigation (dwell=8,services=24)

| Framework | Runs | Mean | Run CV | Mean CI95 | ops/s | Wall | Peak RSS | RSS Growth | Bytes(local) | Bytes(pty) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| TUIX.Py | 1 | 104µs | 51.2% | 101µs–107µs | 9.6K ops/s | 140.94ms | 19.3MB | 64.0KB | 74.7MB | 63.7KB |

## Relative Performance (vs fastest per scenario)

> Ratio = framework mean / fastest mean for each scenario. Lower is better. 1.0x = fastest.

| Scenario | blessed | terminal-kit | ink | rezitui | bubbletea | ratatui | opentui-core | opentui-react | TUIX.Core | TUIX.Py |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| startup | 270.1x [258.0x, 282.1x] | 3.8x [3.5x, 4.0x] | 16.4x [15.6x, 17.1x] | 1.1x [1.0x, 1.2x] | 73.1x [68.6x, 77.5x] | **1.0x** | 3.0x [2.6x, 3.3x] | 2.9x [2.5x, 3.3x] | 2.0x [1.8x, 2.1x] | 2.4x [2.2x, 2.5x] |
| tree-construction (items=10) | 1.7x [1.4x, 2.0x] | 5.8x [5.6x, 6.0x] | 9.3x [9.0x, 9.6x] | 2.2x [1.7x, 2.7x] | 84.2x [79.0x, 89.4x] | **1.0x** | 5.6x [5.2x, 6.0x] | 1.1x [1.1x, 1.2x] | 2.0x [1.9x, 2.1x] | 2.3x [2.2x, 2.3x] |
| tree-construction (items=100) | 4.1x [3.7x, 4.6x] | 16.5x [15.9x, 17.2x] | 40.4x [39.4x, 41.4x] | 1.1x [0.8x, 1.3x] | 432.8x [429.7x, 436.0x] | 1.4x [1.4x, 1.4x] | 1.4x [1.1x, 1.6x] | 1.1x [1.1x, 1.1x] | **1.0x** | 1.4x [1.4x, 1.5x] |
| tree-construction (items=500) | 21.5x [20.3x, 22.8x] | 51.4x [50.2x, 52.7x] | 205.8x [201.7x, 210.0x] | 2.9x [2.1x, 3.7x] | 2968.6x [2897.3x, 3039.8x] | 4.9x [4.8x, 4.9x] | 1.4x [1.3x, 1.5x] | 2.2x [1.8x, 2.5x] | **1.0x** | 1.4x [1.4x, 1.5x] |
| tree-construction (items=1000) | 43.7x [41.9x, 45.5x] | 103.8x [101.9x, 105.7x] | 434.1x [426.8x, 441.4x] | 4.7x [3.7x, 5.7x] | 5801.1x [5658.0x, 5944.3x] | 9.9x [9.8x, 10.0x] | 1.3x [1.2x, 1.3x] | 2.8x [2.7x, 2.9x] | **1.0x** | 1.7x [1.7x, 1.8x] |
| rerender | 2.9x [2.9x, 3.0x] | 17.5x [17.1x, 17.8x] | 53.1x [51.8x, 54.3x] | 6.8x [5.4x, 8.2x] | 1054.9x [1035.5x, 1074.4x] | 4.8x [4.7x, 4.9x] | 12.2x [11.7x, 12.6x] | 17.7x [17.3x, 18.1x] | 3.6x [3.5x, 3.7x] | **1.0x** |
| content-update | 5.4x [5.1x, 5.7x] | 3.9x [3.8x, 4.0x] | 45.7x [44.3x, 47.1x] | **1.0x** | 163.7x [159.1x, 168.3x] | 1.6x [1.6x, 1.6x] | 2.4x [2.3x, 2.6x] | 3.6x [3.5x, 3.8x] | 2.1x [2.0x, 2.2x] | 1.9x [1.8x, 2.0x] |
| layout-stress (cols=4,rows=10) | 4.1x [3.3x, 5.0x] | — | 96.9x [94.5x, 99.2x] | — | 189.2x [181.9x, 196.4x] | — | — | 1.1x [1.1x, 1.1x] | 2.8x [2.7x, 2.9x] | **1.0x** |
| scroll-stress (items=2000) | 75.9x [68.1x, 83.6x] | 11.3x [10.5x, 12.0x] | 23.6x [19.5x, 27.7x] | **1.0x** | 177.1x [159.6x, 194.6x] | 1.9x [1.9x, 2.0x] | 3.1x [2.6x, 3.7x] | 3.3x [3.0x, 3.7x] | 1.2x [1.0x, 1.3x] | 1.9x [1.9x, 1.9x] |
| virtual-list (items=100000,viewport=40) | 1.8x [1.4x, 2.3x] | 6.6x [6.5x, 6.7x] | 10.4x [10.1x, 10.6x] | **1.0x** | 147.9x [144.7x, 151.2x] | 1.5x [1.5x, 1.5x] | 1.5x [1.5x, 1.5x] | 3.0x [2.9x, 3.1x] | 1.4x [1.3x, 1.4x] | 1.1x [1.0x, 1.1x] |
| tables (cols=8,rows=100) | 6.9x [6.4x, 7.3x] | 10.1x [9.7x, 10.5x] | 13.5x [13.0x, 14.1x] | 12.8x [11.7x, 13.9x] | 187.7x [180.1x, 195.4x] | 1.6x [1.6x, 1.6x] | 3.0x [2.8x, 3.2x] | 3.7x [3.5x, 3.9x] | 2.3x [2.2x, 2.4x] | **1.0x** |
| memory-profile | 3.8x [3.7x, 3.9x] | 6.1x [6.0x, 6.2x] | 8.2x [8.1x, 8.3x] | 1.3x [1.0x, 1.6x] | 123.8x [122.2x, 125.4x] | **1.0x** | 2.1x [2.0x, 2.1x] | 2.4x [2.4x, 2.5x] | 1.9x [1.9x, 2.0x] | 2.0x [1.9x, 2.0x] |
| terminal-rerender | 7.3x [7.2x, 7.4x] | 18.7x [18.3x, 19.1x] | 61.0x [59.4x, 62.5x] | 1.4x [1.1x, 1.8x] | 1064.1x [1055.2x, 1073.0x] | 4.3x [4.2x, 4.4x] | 15.3x [14.9x, 15.7x] | 16.6x [16.2x, 16.9x] | 2.0x [1.9x, 2.0x] | **1.0x** |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | 5.4x [5.2x, 5.6x] | 7.2x [7.1x, 7.4x] | 27.4x [26.6x, 28.2x] | **1.0x** | 426.0x [421.0x, 431.0x] | 1.9x [1.8x, 2.0x] | 4.4x [4.2x, 4.5x] | 7.3x [7.1x, 7.6x] | — | — |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | 3.7x [3.5x, 3.8x] | 4.7x [4.6x, 4.7x] | 6.7x [6.5x, 6.9x] | 1.2x [0.8x, 1.6x] | 127.2x [125.8x, 128.6x] | **1.0x** | 1.5x [1.4x, 1.5x] | 1.9x [1.9x, 1.9x] | — | — |
| terminal-screen-transition (cols=120,rows=40) | 4.9x [4.8x, 5.1x] | 5.9x [5.9x, 6.0x] | 9.0x [8.8x, 9.2x] | **1.0x** | 153.7x [152.3x, 155.1x] | 1.3x [1.3x, 1.3x] | 1.8x [1.7x, 1.9x] | 2.4x [2.3x, 2.4x] | — | — |
| terminal-fps-stream (channels=12,cols=120,rows=40) | 2.2x [2.2x, 2.3x] | 5.6x [5.5x, 5.7x] | 8.2x [8.1x, 8.4x] | 2.9x [2.6x, 3.1x] | 136.3x [135.3x, 137.3x] | **1.0x** | 1.4x [1.4x, 1.4x] | 2.4x [2.3x, 2.4x] | — | — |
| terminal-input-latency (cols=120,rows=40) | 6.6x [6.4x, 6.8x] | 8.3x [8.1x, 8.4x] | 28.0x [27.4x, 28.5x] | **1.0x** | 454.1x [450.3x, 457.8x] | 2.4x [2.3x, 2.4x] | 5.3x [5.2x, 5.5x] | 8.6x [8.4x, 8.8x] | — | — |
| terminal-memory-soak (cols=120,rows=40) | 4.3x [4.2x, 4.4x] | 6.6x [6.4x, 6.7x] | 7.6x [7.4x, 7.7x] | **1.0x** | 135.7x [134.5x, 136.9x] | 1.1x [1.1x, 1.1x] | 2.0x [2.0x, 2.1x] | 2.3x [2.2x, 2.3x] | 2.7x [2.6x, 2.7x] | 2.1x [2.0x, 2.1x] |
| terminal-full-ui (cols=120,rows=40,services=24) | 5.9x [5.8x, 6.1x] | 7.8x [7.7x, 7.9x] | 13.8x [13.5x, 14.0x] | 7.3x [7.0x, 7.7x] | 225.6x [223.9x, 227.4x] | 1.8x [1.8x, 1.9x] | 2.7x [2.7x, 2.8x] | 3.0x [2.9x, 3.1x] | **1.0x** | — |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 3.4x [3.3x, 3.5x] | 5.6x [5.5x, 5.7x] | 8.7x [8.5x, 8.8x] | 4.7x [4.4x, 4.9x] | 147.5x [146.3x, 148.8x] | 1.0x [1.0x, 1.1x] | 2.0x [1.9x, 2.0x] | 2.0x [1.9x, 2.0x] | **1.0x** | — |
| terminal-strict-ui (cols=120,rows=40,services=24) | 5.4x [5.3x, 5.5x] | 8.0x [7.9x, 8.1x] | 14.5x [14.2x, 14.7x] | 6.3x [5.9x, 6.6x] | 228.7x [226.9x, 230.6x] | 1.5x [1.5x, 1.5x] | 2.9x [2.8x, 3.0x] | 2.7x [2.6x, 2.8x] | **1.0x** | — |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | 4.0x [3.9x, 4.1x] | 6.3x [6.2x, 6.4x] | 9.6x [9.5x, 9.8x] | 4.4x [4.1x, 4.6x] | 282.6x [279.7x, 285.5x] | 1.1x [1.1x, 1.1x] | 2.2x [2.1x, 2.2x] | 2.4x [2.3x, 2.4x] | **1.0x** | — |
| terminal-virtual-list (items=100000,viewport=40) | 3.3x [3.0x, 3.5x] | 18.0x [17.7x, 18.3x] | 23.8x [23.3x, 24.2x] | **1.0x** | 565.6x [554.7x, 576.6x] | 3.3x [3.2x, 3.3x] | 4.5x [4.4x, 4.6x] | 9.0x [8.9x, 9.2x] | 2.9x [2.9x, 3.0x] | 2.4x [2.4x, 2.5x] |
| terminal-table (cols=8,rows=40) | 5.7x [5.5x, 5.9x] | 7.5x [7.3x, 7.7x] | 10.3x [10.1x, 10.6x] | 5.5x [5.2x, 5.8x] | 204.8x [199.7x, 209.9x] | 1.5x [1.5x, 1.6x] | 2.2x [2.1x, 2.3x] | 2.6x [2.5x, 2.7x] | 1.5x [1.4x, 1.5x] | **1.0x** |
| layout-stress (cols=4,rows=40) | — | 5.6x [5.4x, 5.8x] | — | 14.4x [13.4x, 15.4x] | — | 1.3x [1.2x, 1.3x] | **1.0x** | — | — | — |
| terminal-frame-fill (dirty=1) | — | — | — | — | — | — | — | — | 1.9x [1.9x, 1.9x] | **1.0x** |
| terminal-frame-fill (dirty=40) | — | — | — | — | — | — | — | — | 1.2x [1.2x, 1.2x] | **1.0x** |
| terminal-screen-transition | — | — | — | — | — | — | — | — | **1.0x** | **1.0x** |
| terminal-fps-stream (services=12) | — | — | — | — | — | — | — | — | 1.4x [1.4x, 1.4x] | **1.0x** |
| terminal-input-latency | — | — | — | — | — | — | — | — | 3.2x [3.0x, 3.4x] | **1.0x** |
| terminal-full-ui (services=24) | — | — | — | — | — | — | — | — | — | **1.0x** |
| terminal-full-ui-navigation (dwell=8,services=24) | — | — | — | — | — | — | — | — | — | **1.0x** |
| terminal-strict-ui (services=24) | — | — | — | — | — | — | — | — | — | **1.0x** |
| terminal-strict-ui-navigation (dwell=8,services=24) | — | — | — | — | — | — | — | — | — | **1.0x** |

## Memory Comparison

| Scenario | Framework | Peak RSS | RSS Before | RSS After | RSS Growth |
|---|---|---:|---:|---:|---:|
| startup | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| startup | TUIX.Core | 3.9MB | 3.9MB | 3.9MB | 4.0KB |
| startup | bubbletea | 11.9MB | 11.7MB | 11.9MB | 304.0KB |
| startup | TUIX.Py | 18.1MB | 18.0MB | 18.1MB | 56.0KB |
| startup | opentui-core | 33.8MB | 33.2MB | 33.8MB | 624.0KB |
| startup | opentui-react | 36.5MB | 35.1MB | 36.5MB | 1.4MB |
| startup | terminal-kit | 46.3MB | 46.4MB | 46.3MB | 0.00KB |
| startup | ink | 64.7MB | 62.0MB | 64.7MB | 2.7MB |
| startup | rezitui | 76.1MB | 68.0MB | 76.1MB | 8.0MB |
| startup | blessed | 152.1MB | 78.0MB | 152.1MB | 74.1MB |
| tree-construction (items=10) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=10) | TUIX.Core | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=10) | bubbletea | 11.9MB | 11.9MB | 11.9MB | 0.00KB |
| tree-construction (items=10) | TUIX.Py | 18.1MB | 18.1MB | 18.1MB | 16.0KB |
| tree-construction (items=10) | opentui-core | 34.9MB | 33.8MB | 34.9MB | 1.1MB |
| tree-construction (items=10) | opentui-react | 36.5MB | 36.5MB | 36.5MB | 0.00KB |
| tree-construction (items=10) | terminal-kit | 46.6MB | 46.6MB | 46.6MB | 4.0KB |
| tree-construction (items=10) | ink | 68.4MB | 66.4MB | 68.4MB | 2.0MB |
| tree-construction (items=10) | rezitui | 100.5MB | 82.0MB | 100.5MB | 18.4MB |
| tree-construction (items=10) | blessed | 160.8MB | 154.1MB | 160.8MB | 6.8MB |
| tree-construction (items=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=100) | TUIX.Core | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=100) | bubbletea | 12.0MB | 11.9MB | 12.0MB | 72.0KB |
| tree-construction (items=100) | TUIX.Py | 18.2MB | 18.2MB | 18.2MB | 16.0KB |
| tree-construction (items=100) | opentui-core | 35.6MB | 34.9MB | 35.6MB | 728.0KB |
| tree-construction (items=100) | opentui-react | 36.5MB | 36.5MB | 36.5MB | 0.00KB |
| tree-construction (items=100) | terminal-kit | 46.7MB | 46.7MB | 46.7MB | 28.0KB |
| tree-construction (items=100) | ink | 104.1MB | 84.8MB | 104.1MB | 19.3MB |
| tree-construction (items=100) | rezitui | 110.9MB | 98.8MB | 110.9MB | 12.0MB |
| tree-construction (items=100) | blessed | 240.7MB | 164.7MB | 240.7MB | 76.0MB |
| tree-construction (items=500) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=500) | TUIX.Core | 3.9MB | 3.9MB | 3.9MB | 0.00KB |
| tree-construction (items=500) | bubbletea | 12.0MB | 12.0MB | 12.0MB | 8.0KB |
| tree-construction (items=500) | TUIX.Py | 18.2MB | 18.2MB | 18.2MB | 16.0KB |
| tree-construction (items=500) | opentui-core | 35.6MB | 35.6MB | 35.6MB | 0.00KB |
| tree-construction (items=500) | opentui-react | 36.5MB | 36.5MB | 36.5MB | 0.00KB |
| tree-construction (items=500) | terminal-kit | 46.9MB | 46.7MB | 46.9MB | 180.0KB |
| tree-construction (items=500) | rezitui | 168.7MB | 111.1MB | 168.7MB | 57.6MB |
| tree-construction (items=500) | ink | 193.9MB | 105.2MB | 193.9MB | 88.7MB |
| tree-construction (items=500) | blessed | 453.6MB | 261.4MB | 453.6MB | 192.2MB |
| tree-construction (items=1000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tree-construction (items=1000) | TUIX.Core | 4.0MB | 3.9MB | 4.0MB | 96.0KB |
| tree-construction (items=1000) | bubbletea | 16.0MB | 12.0MB | 16.0MB | 4.0MB |
| tree-construction (items=1000) | TUIX.Py | 18.2MB | 18.2MB | 18.2MB | 16.0KB |
| tree-construction (items=1000) | opentui-core | 35.6MB | 35.6MB | 35.6MB | 0.00KB |
| tree-construction (items=1000) | opentui-react | 36.6MB | 36.5MB | 36.6MB | 148.0KB |
| tree-construction (items=1000) | terminal-kit | 55.0MB | 46.9MB | 55.0MB | 8.1MB |
| tree-construction (items=1000) | rezitui | 180.0MB | 168.8MB | 180.0MB | 11.2MB |
| tree-construction (items=1000) | ink | 358.4MB | 218.7MB | 358.4MB | 139.7MB |
| tree-construction (items=1000) | blessed | 528.4MB | 453.8MB | 528.4MB | 74.6MB |
| rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| rerender | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| rerender | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| rerender | TUIX.Py | 18.3MB | 18.2MB | 18.3MB | 88.0KB |
| rerender | opentui-core | 35.6MB | 35.6MB | 35.6MB | 0.00KB |
| rerender | opentui-react | 38.7MB | 37.9MB | 38.7MB | 876.0KB |
| rerender | terminal-kit | 54.8MB | 55.0MB | 54.8MB | 0.00KB |
| rerender | rezitui | 187.6MB | 180.0MB | 187.6MB | 7.6MB |
| rerender | ink | 358.0MB | 358.4MB | 358.0MB | 0.00KB |
| rerender | blessed | 527.7MB | 528.1MB | 527.7MB | 0.00KB |
| content-update | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| content-update | TUIX.Core | 4.0MB | 4.0MB | 4.0MB | 0.00KB |
| content-update | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| content-update | TUIX.Py | 18.3MB | 18.3MB | 18.3MB | 0.00KB |
| content-update | opentui-core | 35.6MB | 35.6MB | 35.6MB | 0.00KB |
| content-update | opentui-react | 38.9MB | 38.9MB | 38.9MB | 4.0KB |
| content-update | terminal-kit | 54.8MB | 54.8MB | 54.8MB | 8.0KB |
| content-update | rezitui | 182.6MB | 186.5MB | 182.6MB | 0.00KB |
| content-update | ink | 358.2MB | 357.6MB | 358.2MB | 532.0KB |
| content-update | blessed | 528.2MB | 529.2MB | 528.2MB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Core | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| layout-stress (cols=4,rows=10) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| layout-stress (cols=4,rows=10) | TUIX.Py | 18.3MB | 18.3MB | 18.3MB | 0.00KB |
| layout-stress (cols=4,rows=10) | opentui-react | 38.9MB | 38.9MB | 38.9MB | 0.00KB |
| layout-stress (cols=4,rows=10) | ink | 358.0MB | 358.7MB | 358.0MB | 0.00KB |
| layout-stress (cols=4,rows=10) | blessed | 528.8MB | 533.9MB | 528.8MB | 0.00KB |
| scroll-stress (items=2000) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Core | 4.3MB | 4.3MB | 4.3MB | 0.00KB |
| scroll-stress (items=2000) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| scroll-stress (items=2000) | TUIX.Py | 18.3MB | 18.3MB | 18.3MB | 0.00KB |
| scroll-stress (items=2000) | opentui-core | 35.8MB | 35.7MB | 35.8MB | 124.0KB |
| scroll-stress (items=2000) | opentui-react | 39.0MB | 39.0MB | 39.0MB | 0.00KB |
| scroll-stress (items=2000) | terminal-kit | 55.1MB | 55.1MB | 55.1MB | 0.00KB |
| scroll-stress (items=2000) | rezitui | 183.4MB | 183.4MB | 183.4MB | 92.0KB |
| scroll-stress (items=2000) | ink | 358.0MB | 358.0MB | 358.0MB | 64.0KB |
| scroll-stress (items=2000) | blessed | 530.1MB | 528.6MB | 530.1MB | 1.5MB |
| virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Core | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | TUIX.Py | 18.3MB | 18.3MB | 18.3MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | opentui-core | 35.8MB | 35.8MB | 35.8MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | opentui-react | 39.0MB | 39.0MB | 39.0MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | terminal-kit | 97.5MB | 90.7MB | 97.5MB | 6.9MB |
| virtual-list (items=100000,viewport=40) | rezitui | 188.4MB | 183.8MB | 188.4MB | 4.6MB |
| virtual-list (items=100000,viewport=40) | ink | 359.2MB | 359.2MB | 359.2MB | 0.00KB |
| virtual-list (items=100000,viewport=40) | blessed | 530.2MB | 531.0MB | 530.2MB | 0.00KB |
| tables (cols=8,rows=100) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Core | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| tables (cols=8,rows=100) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| tables (cols=8,rows=100) | TUIX.Py | 18.3MB | 18.3MB | 18.3MB | 8.0KB |
| tables (cols=8,rows=100) | opentui-core | 35.8MB | 35.8MB | 35.8MB | 0.00KB |
| tables (cols=8,rows=100) | opentui-react | 39.2MB | 39.2MB | 39.2MB | 0.00KB |
| tables (cols=8,rows=100) | terminal-kit | 97.5MB | 97.4MB | 97.5MB | 56.0KB |
| tables (cols=8,rows=100) | rezitui | 184.7MB | 184.8MB | 184.7MB | 0.00KB |
| tables (cols=8,rows=100) | ink | 359.4MB | 359.4MB | 359.4MB | 0.00KB |
| tables (cols=8,rows=100) | blessed | 529.7MB | 529.6MB | 529.7MB | 92.0KB |
| memory-profile | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| memory-profile | TUIX.Core | 4.4MB | 4.4MB | 4.4MB | 0.00KB |
| memory-profile | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| memory-profile | TUIX.Py | 18.5MB | 18.4MB | 18.5MB | 116.0KB |
| memory-profile | opentui-core | 35.8MB | 35.8MB | 35.8MB | 16.0KB |
| memory-profile | opentui-react | 39.2MB | 39.2MB | 39.2MB | 40.0KB |
| memory-profile | terminal-kit | 97.3MB | 97.5MB | 97.3MB | 0.00KB |
| memory-profile | rezitui | 191.1MB | 185.2MB | 191.1MB | 5.9MB |
| memory-profile | ink | 359.4MB | 359.4MB | 359.4MB | 4.0KB |
| memory-profile | blessed | 529.5MB | 529.9MB | 529.5MB | 0.00KB |
| terminal-rerender | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-rerender | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-rerender | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| terminal-rerender | TUIX.Py | 18.6MB | 18.6MB | 18.6MB | 0.00KB |
| terminal-rerender | opentui-core | 36.2MB | 36.2MB | 36.2MB | 0.00KB |
| terminal-rerender | opentui-react | 41.0MB | 40.1MB | 41.0MB | 1008.0KB |
| terminal-rerender | terminal-kit | 97.5MB | 97.4MB | 97.5MB | 80.0KB |
| terminal-rerender | rezitui | 189.8MB | 191.1MB | 189.8MB | 0.00KB |
| terminal-rerender | ink | 359.5MB | 359.4MB | 359.5MB | 96.0KB |
| terminal-rerender | blessed | 529.4MB | 529.4MB | 529.4MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | bubbletea | 16.0MB | 16.0MB | 16.0MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | terminal-kit | 97.5MB | 97.5MB | 97.5MB | 28.0KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | rezitui | 191.4MB | 189.8MB | 191.4MB | 1.6MB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | ink | 359.5MB | 359.5MB | 359.5MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=1,rows=40) | blessed | 529.6MB | 529.6MB | 529.6MB | 4.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | bubbletea | 16.2MB | 16.0MB | 16.2MB | 128.0KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | terminal-kit | 97.6MB | 97.6MB | 97.6MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | rezitui | 202.6MB | 191.5MB | 202.6MB | 11.1MB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | ink | 359.5MB | 359.5MB | 359.5MB | 0.00KB |
| terminal-frame-fill (cols=120,dirtyLines=40,rows=40) | blessed | 529.9MB | 529.6MB | 529.9MB | 308.0KB |
| terminal-screen-transition (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | terminal-kit | 97.7MB | 97.7MB | 97.7MB | 4.0KB |
| terminal-screen-transition (cols=120,rows=40) | rezitui | 202.8MB | 203.2MB | 202.8MB | 0.00KB |
| terminal-screen-transition (cols=120,rows=40) | ink | 359.5MB | 359.5MB | 359.5MB | 4.0KB |
| terminal-screen-transition (cols=120,rows=40) | blessed | 529.9MB | 529.9MB | 529.9MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | terminal-kit | 97.8MB | 97.8MB | 97.8MB | 8.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | rezitui | 203.9MB | 204.1MB | 203.9MB | 0.00KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | ink | 359.7MB | 359.6MB | 359.7MB | 64.0KB |
| terminal-fps-stream (channels=12,cols=120,rows=40) | blessed | 529.9MB | 529.9MB | 529.9MB | 4.0KB |
| terminal-input-latency (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | terminal-kit | 97.9MB | 97.7MB | 97.9MB | 240.0KB |
| terminal-input-latency (cols=120,rows=40) | rezitui | 205.5MB | 203.9MB | 205.5MB | 1.6MB |
| terminal-input-latency (cols=120,rows=40) | ink | 359.7MB | 359.7MB | 359.7MB | 0.00KB |
| terminal-input-latency (cols=120,rows=40) | blessed | 530.0MB | 529.9MB | 530.0MB | 24.0KB |
| terminal-memory-soak (cols=120,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Core | 4.9MB | 4.9MB | 4.9MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | TUIX.Py | 19.3MB | 19.3MB | 19.3MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | terminal-kit | 98.0MB | 98.0MB | 98.0MB | 16.0KB |
| terminal-memory-soak (cols=120,rows=40) | rezitui | 211.3MB | 205.5MB | 211.3MB | 5.8MB |
| terminal-memory-soak (cols=120,rows=40) | ink | 359.7MB | 359.7MB | 359.7MB | 0.00KB |
| terminal-memory-soak (cols=120,rows=40) | blessed | 530.0MB | 530.0MB | 530.0MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.9MB | 4.7MB | 4.9MB | 124.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-full-ui (cols=120,rows=40,services=24) | terminal-kit | 98.2MB | 98.2MB | 98.2MB | 8.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | rezitui | 214.0MB | 212.0MB | 214.0MB | 2.0MB |
| terminal-full-ui (cols=120,rows=40,services=24) | ink | 359.9MB | 359.7MB | 359.9MB | 184.0KB |
| terminal-full-ui (cols=120,rows=40,services=24) | blessed | 530.0MB | 530.0MB | 530.0MB | 4.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.9MB | 4.9MB | 4.9MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 98.3MB | 98.2MB | 98.3MB | 60.0KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 216.4MB | 214.0MB | 216.4MB | 2.4MB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 359.9MB | 359.9MB | 359.9MB | 0.00KB |
| terminal-full-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 530.0MB | 530.0MB | 530.0MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | TUIX.Core | 4.9MB | 4.9MB | 4.9MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | terminal-kit | 98.3MB | 98.3MB | 98.3MB | 4.0KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | rezitui | 211.3MB | 216.5MB | 211.3MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | ink | 359.9MB | 359.9MB | 359.9MB | 0.00KB |
| terminal-strict-ui (cols=120,rows=40,services=24) | blessed | 528.9MB | 530.0MB | 528.9MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | TUIX.Core | 4.9MB | 4.9MB | 4.9MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | terminal-kit | 98.4MB | 98.4MB | 98.4MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | rezitui | 211.7MB | 211.3MB | 211.7MB | 388.0KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | ink | 359.9MB | 359.9MB | 359.9MB | 0.00KB |
| terminal-strict-ui-navigation (cols=120,dwell=8,rows=40,services=24) | blessed | 529.1MB | 528.9MB | 529.1MB | 184.0KB |
| terminal-virtual-list (items=100000,viewport=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Core | 4.9MB | 4.9MB | 4.9MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | TUIX.Py | 19.2MB | 19.1MB | 19.2MB | 64.0KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | terminal-kit | 129.0MB | 128.6MB | 129.0MB | 364.0KB |
| terminal-virtual-list (items=100000,viewport=40) | rezitui | 212.6MB | 211.7MB | 212.6MB | 956.0KB |
| terminal-virtual-list (items=100000,viewport=40) | ink | 360.8MB | 360.8MB | 360.8MB | 0.00KB |
| terminal-virtual-list (items=100000,viewport=40) | blessed | 530.2MB | 530.2MB | 530.2MB | 8.0KB |
| terminal-table (cols=8,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Core | 4.9MB | 4.9MB | 4.9MB | 0.00KB |
| terminal-table (cols=8,rows=40) | bubbletea | 16.2MB | 16.2MB | 16.2MB | 0.00KB |
| terminal-table (cols=8,rows=40) | TUIX.Py | 19.3MB | 19.3MB | 19.3MB | 0.00KB |
| terminal-table (cols=8,rows=40) | opentui-core | 37.3MB | 37.3MB | 37.3MB | 0.00KB |
| terminal-table (cols=8,rows=40) | opentui-react | 41.0MB | 41.0MB | 41.0MB | 0.00KB |
| terminal-table (cols=8,rows=40) | terminal-kit | 129.2MB | 129.2MB | 129.2MB | 0.00KB |
| terminal-table (cols=8,rows=40) | rezitui | 214.6MB | 212.0MB | 214.6MB | 2.6MB |
| terminal-table (cols=8,rows=40) | ink | 360.5MB | 360.5MB | 360.5MB | 0.00KB |
| terminal-table (cols=8,rows=40) | blessed | 530.2MB | 530.2MB | 530.2MB | 0.00KB |
| layout-stress (cols=4,rows=40) | ratatui | 27.0KB | 27.0KB | 27.0KB | 0.00KB |
| layout-stress (cols=4,rows=40) | opentui-core | 35.6MB | 35.6MB | 35.6MB | 0.00KB |
| layout-stress (cols=4,rows=40) | terminal-kit | 55.0MB | 55.0MB | 55.0MB | 0.00KB |
| layout-stress (cols=4,rows=40) | rezitui | 183.4MB | 181.4MB | 183.4MB | 1.9MB |
| terminal-frame-fill (dirty=1) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-frame-fill (dirty=1) | TUIX.Py | 18.6MB | 18.6MB | 18.6MB | 64.0KB |
| terminal-frame-fill (dirty=40) | TUIX.Core | 4.7MB | 4.7MB | 4.7MB | 0.00KB |
| terminal-frame-fill (dirty=40) | TUIX.Py | 18.6MB | 18.6MB | 18.6MB | 0.00KB |
| terminal-screen-transition | TUIX.Core | 4.8MB | 4.8MB | 4.8MB | 0.00KB |
| terminal-screen-transition | TUIX.Py | 18.7MB | 18.7MB | 18.7MB | 0.00KB |
| terminal-fps-stream (services=12) | TUIX.Core | 4.8MB | 4.8MB | 4.8MB | 0.00KB |
| terminal-fps-stream (services=12) | TUIX.Py | 18.8MB | 18.7MB | 18.8MB | 64.0KB |
| terminal-input-latency | TUIX.Core | 4.8MB | 4.8MB | 4.8MB | 0.00KB |
| terminal-input-latency | TUIX.Py | 18.9MB | 18.8MB | 18.9MB | 64.0KB |
| terminal-full-ui (services=24) | TUIX.Py | 19.0MB | 18.9MB | 19.0MB | 64.0KB |
| terminal-full-ui-navigation (dwell=8,services=24) | TUIX.Py | 19.1MB | 19.1MB | 19.1MB | 28.0KB |
| terminal-strict-ui (services=24) | TUIX.Py | 19.1MB | 19.0MB | 19.1MB | 40.0KB |
| terminal-strict-ui-navigation (dwell=8,services=24) | TUIX.Py | 19.3MB | 19.2MB | 19.3MB | 64.0KB |