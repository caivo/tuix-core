"""
Scene Stats and Compaction Demo (v0.3 feature)

Demonstrates the new scene management features in v0.3:
- Getting scene statistics (buffer counts, memory usage, activity)
- Compacting scene pixels (defragmenting pixel memory)
- Compacting cold scenes (inactive scenes)
- Using snapshot APIs for safe, lock-free data access
"""

import time
import ctypes
from tuix.core import engine, builders, scenes, registry, objects, buffers, input, _structs, _tuix_cy


def get_object_by_uid(uid):
    """Get a ctypes pointer to a TuixObject by uid."""
    obj_addr = _tuix_cy._tuix_get_object_addr_by_uid(uid)
    if obj_addr == 0:
        return None
    return ctypes.cast(ctypes.c_void_p(obj_addr), ctypes.POINTER(_structs.TuixObject)).contents


def format_bytes(num_bytes):
    """Format byte count as human-readable string."""
    if num_bytes < 1024:
        return f"{num_bytes}B"
    elif num_bytes < 1024 * 1024:
        return f"{num_bytes / 1024:.1f}KB"
    else:
        return f"{num_bytes / (1024 * 1024):.1f}MB"


def demo_scene_stats():
    """Demonstrate scene statistics gathering."""
    print("\n=== Scene Statistics Demo ===")
    
    if engine.init() != 0:
        print("tuix_init() failed")
        return 1
    
    builders.register_standard()
    scenes.init_scene(b"StatsScene")
    registry.registry.current_scene_name = b"StatsScene"
    input.listen()
    
    # Create various objects
    print("\nCreating objects...")
    uids = []
    
    # Create some canvases (memory-intensive)
    for i in range(3):
        uid = objects.create_object(
            builders.CANVAS,
            b"StatsScene",
            0.7, 0.5,
            0.1 + i * 0.15,
            0.1
        )
        uids.append(uid)
        print(f"  Created CANVAS {i+1}: uid={uid}")
    
    # Create some widgets
    for i in range(2):
        uid = objects.create_object(
            builders.PROGRESSBAR,
            b"StatsScene",
            0.3, 0.05,
            0.1, 0.05 + i * 0.08
        )
        uids.append(uid)
        print(f"  Created PROGRESSBAR {i+1}: uid={uid}")
    
    # Run a few frames to initialize
    for _ in range(3):
        engine.main_loop()
        time.sleep(0.05)
    
    # Get and display scene stats
    print("\n--- Scene Statistics ---")
    stats = scenes.get_scene_stats(b"StatsScene")
    
    if stats:
        print(f"Buffer count:        {stats['buffer_count']}")
        print(f"Active buffers:      {stats['active']}")
        print(f"Current focus:       {stats['current_focus']}")
        print(f"Pixel memory:        {format_bytes(stats['pixel_bytes'])}")
        print(f"Approx heap memory:  {format_bytes(stats['approx_heap_bytes'])}")
        print(f"Last active frame:   {stats['last_active_frame']}")
        print(f"Last compact frame:  {stats['last_compacted_frame']}")
    
    # Test compaction
    print("\n--- Testing Compaction ---")
    print("Before compaction:")
    stats_before = scenes.get_scene_stats(b"StatsScene")
    if stats_before:
        print(f"  Pixel bytes: {format_bytes(stats_before['pixel_bytes'])}")
    
    print("Compacting pixels...")
    freed_bytes = scenes.compact_scene_pixels(b"StatsScene")
    print(f"  Bytes freed: {format_bytes(freed_bytes)}")
    time.sleep(0.1)
    
    print("After compaction:")
    stats_after = scenes.get_scene_stats(b"StatsScene")
    if stats_after:
        print(f"  Pixel bytes: {format_bytes(stats_after['pixel_bytes'])}")
    
    # Cleanup
    print("\nCleaning up...")
    for uid in uids:
        buffers.free_buffer(b"StatsScene", uid)
    scenes.free_scene(b"StatsScene")
    engine.shutdown()
    
    print("✓ Scene stats demo complete")
    return 0

def demo_snapshot_api():
    """Demonstrate safe snapshot APIs for lock-free data access."""
    print("\n=== Snapshot APIs Demo ===")
    
    if engine.init() != 0:
        print("tuix_init() failed")
        return 1
    
    builders.register_standard()
    scenes.init_scene(b"SnapshotScene")
    registry.registry.current_scene_name = b"SnapshotScene"
    input.listen()
    
    # Create a canvas
    uid = objects.create_object(
        builders.CANVAS,
        b"SnapshotScene",
        0.8, 0.6,
        0.1, 0.1
    )
    print(f"Created canvas: uid={uid}")
    
    # Initialize geometry
    for _ in range(2):
        engine.main_loop()
        time.sleep(0.05)
    
    # Get buffer snapshot
    print("\n--- Buffer Snapshot ---")
    buf_snap = buffers.get_buffer_snapshot(b"SnapshotScene", uid)
    if buf_snap:
        print(f"Width:        {buf_snap['width']}")
        print(f"Height:       {buf_snap['height']}")
        print(f"Parent uid:   {buf_snap['parent_uid']}")
        print(f"Z-index:      {buf_snap['z_index']}")
        print(f"Redraw needed: {buf_snap['required_redraw']}")
    
    # Get object snapshot
    print("\n--- Object Snapshot ---")
    obj_snap = objects.get_object_snapshot_by_uid(uid)
    if obj_snap:
        print(f"UID:               {obj_snap['uid']}")
        print(f"Width modifier:    {obj_snap['width_mod']:.1%}")
        print(f"Height modifier:   {obj_snap['height_mod']:.1%}")
        print(f"Margin top:        {obj_snap['margin_top_mod']:.1%}")
        print(f"Margin left:       {obj_snap['margin_left_mod']:.1%}")
    
    # Draw on canvas using object reference
    print("\n--- Drawing on Canvas ---")
    obj = get_object_by_uid(uid)
    if obj and buf_snap:
        w = buf_snap['width']
        h = buf_snap['height']
        # Draw frame
        objects.tuix_canvas_draw_rect(obj, 0, 0, w, h, b'.', 0, 100, 100, 255, 0, 0, 0)
        # Draw title using snapshot data
        title = f"Canvas {uid}x{h}".encode()
        objects.tuix_canvas_draw_text(obj, 2, 2, title, 255, 255, 0, 0, 0, 0)
        print("✓ Drew frame and title on canvas")
    
    # Render a few frames
    print("\nRendering (3 frames)...")
    for _ in range(3):
        engine.main_loop()
        time.sleep(0.2)
    
    # Get updated stats
    print("\n--- Updated Statistics ---")
    stats = scenes.get_scene_stats(b"SnapshotScene")
    if stats:
        print(f"Buffer count: {stats['buffer_count']}")
        print(f"Pixel memory: {format_bytes(stats['pixel_bytes'])}")
    
    # Cleanup
    buffers.free_buffer(b"SnapshotScene", uid)
    scenes.free_scene(b"SnapshotScene")
    engine.shutdown()
    
    print("\n✓ Snapshot APIs demo complete")
    return 0


def main():
    """Run all scene management demonstrations."""
    print("Scene Stats and Compaction Demo (Tuix v0.3)")
    print("=" * 50)
    
    # Run stats demo
    if demo_scene_stats() != 0:
        return 1
    
    time.sleep(1)
    
    # Run snapshot demo
    if demo_snapshot_api() != 0:
        return 1
    
    print("\n" + "=" * 50)
    print("All demos completed successfully!")
    return 0


if __name__ == "__main__":
    exit(main())
