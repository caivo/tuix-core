# Changelog

All notable changes to this project will be documented in this file.

The format is inspired by Keep a Changelog.

Versioning policy (pre-1.0): this project is in active v0.x development, and breaking changes can occur in both minor and patch releases.

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
