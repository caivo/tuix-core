# Changelog

All notable changes to this project will be documented in this file.

The format is inspired by Keep a Changelog.

Versioning policy (pre-1.0): this project is in active v0.x development, and breaking changes can occur in both minor and patch releases.

## [0.5.0] - 2026-05-21

### Added
- Added the first nested-layout contract with `TuixLayoutSlot`, `TuixLayoutRect`, builder-level `layout_children`, parent-first subtree rebuilds, and core APIs for per-child slot/layout overrides and grid placement.
- Added `RowBuilder` and `ColumnBuilder` as stack/flex-style layout parents with gap, padding, justify, align, and per-child grow/shrink/basis/min/max slot handling.
- Added `SplitPaneBuilder` with divider hit-zone support, drag resize, keyboard resize, and generic mouse-capture integration.
- Added `GridBuilder` with explicit row/column track definitions, fixed and weighted tracks, and per-child row/col/span placement.
- Added `CheckboxBuilder` with label, checked/disabled state, and mouse/keyboard toggling APIs.
- Added `ListViewBuilder` as a viewport-backed virtual list with selection, activation, wheel scrolling, and keyboard navigation.
- Added `TextAreaBuilder` as a multiline viewport-backed editor with cursor movement, scrolling, editing, placeholder support, and readonly mode.
- Added scene-level modal routing state and `DialogBuilder` with backdrop, centered body layout, focus trapping, `Esc`/backdrop close handling, and restore-focus logic.
- Generalized the viewport API from scroll-container-specific logic into builder callbacks used by the compositor, geometry resolver, and hitmap generation.
- Expanded examples and build coverage for nested layout, split panes, grids, checkboxes, list views, text areas, and dialogs.

### Changed
- Formalized builder pixel ownership with `returns_temporary_pixels` and added `pixels_owned` to `TuixBuffer`.
- Updated the core to copy builder-returned pixel buffers into core-owned memory unless the builder performs an in-place update, avoiding races with builder-internal threads.
- Stopped freeing persistent builder scratch buffers in the core render loop; widget builders now release cached pixel buffers from `destroy_state`.
- Reduced lock contention by keeping builder callbacks outside the global registry lock while preserving safe buffer snapshot/copy semantics during resize and build steps.
- Reworked scrollable container integration into a viewport-aware pipeline. The compositor, hitmap, geometry resolver, and input router now treat scrollable areas as first-class viewport owners with content-space child layout.
- Reworked mouse routing around hitmap target picking, viewport ownership, and generic mouse capture so scroll/drag interactions no longer depend on builder creation order.
- Reworked `VideoPlayerBuilder` playback synchronization to snapshot playback config under lock, keep decoder pipe ownership on the decoder thread, and wait for full thread exit during teardown.
- Updated standard builder registration and build scripts to include the new builder set.

### Fixed
- Prevented heap corruption on terminal resize by tightening buffer ownership and lifetime handling.
- Hardened registry destroy and scene clear paths to avoid freeing builder-owned pixel memory.
- Fixed stale internal pointers, use-after-free on redraw, and double-free during teardown by moving cached pixel cleanup into builder `destroy_state` paths.
- Fixed `VideoPlayerBuilder` races around its internal frame buffer, resize handling, play/seek/stop, and Windows shutdown.
- Hardened blank-row scratch buffer rebuilds so allocation failure during resize no longer falls through into null-pointer `memcpy` during scene composition.
- Fixed rendering diff instability in both text and halfblock renderers by basing row hashes, pixel equality, and run coalescing on terminal-visible ANSI color output instead of raw/RGB565 intermediates.
- Fixed redundant repaints when distinct source colors collapse to the same emitted terminal color.
- Fixed event-queue snapshotting and reduced mouse-event loss under heavy hover/drag traffic.
- Fixed `ScrollContainerBuilder` child placement in content space, viewport clipping behavior, redraw invalidations on content-size changes, and drag/wheel event delivery.
- Fixed assorted buffer allocation, compaction, and initialization paths to improve stability under concurrent resize and rendering workloads.

### Removed
- Removed `IconBuilder` from the standard widget set, build scripts, examples, and registration because it overlapped with simpler text-based primitives.
- Removed older scroll-container focus and blit workarounds after viewport-based routing replaced them.

## [0.4.0] - 2026-04-09

### Added
- Added frame-local compositor hitmap generation with a persistent hitmap accessor API.
- Added hitmap-based mouse click picking in focus routing before builder event dispatch.
- Added `TextBuilder` widget with runtime text/fg/bg configuration APIs.
- Added `BoxBuilder` widget with border/title rendering and color configuration API.
- Added `DividerBuilder` widget with horizontal/vertical orientation, symbol, and color APIs.
- Added `BadgeBuilder` widget with text and fg/bg color configuration APIs.
- Added `ButtonBuilder` widget with keyboard/mouse press handling and state retrieval APIs.
- Added `IconBuilder` widget for centered symbol rendering with configurable fg/bg colors.
- Added `TagBuilder` widget for compact label chips with configurable brackets and colors.
- Added `StatusBuilder` widget for IDLE/OK/WARN/ERROR display with configurable palette.
- Added `MenuBuilder` widget with keyboard/mouse selection and activation APIs.
- Added `ScrollContainerBuilder` widget with keyboard/mouse-wheel scrolling over virtual content.

### Changed
- Bumped package version from `0.3` to `0.4`.
- Updated standard builder registration to include Text/Box/Divider/Badge/Button/Icon/Tag/Status/Menu/ScrollContainer builders.
- Added lock coverage in main-loop scene cache refresh to reduce stale global reads.
- Improved scene registry safety by locking scene-list reads and scene-selection paths.
- Made subcycle registry init/free paths internally lock-safe.
- Updated input focus routing so mouse click focus-switch no longer consumes mouse events before builder handlers.

### Fixed
- Fixed UID allocation race by locking `next_uid` increments during object creation.
- Fixed scene initialization race by moving duplicate-name validation under registry lock.
- Improved subcycle teardown robustness with null checks and explicit state cleanup.
- Fixed teardown lifecycle ordering to destroy builder state before object memory release.
- Fixed compositor traversal capacity growth error path to avoid partial-allocation leaks.
- Fixed canvas-builder queued pixel initialization to prevent random style-flag corruption.
- Fixed canvas fallback pixel initialization to always set `custom_fg`/`custom_bg` flags.
- Fixed interactive-windows demo pixel writes to fully initialize style flags and cached color fields.
