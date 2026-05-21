"""
Buffer Hierarchy Demo (v0.3 feature)

Demonstrates the new buffer hierarchy system in v0.3:
- Creating parent-child buffer relationships
- Managing z-index for layering
- Safe reparenting with cycle detection
"""

import time
from tuix.core import engine, builders, scenes, objects, buffers, input


def demo_parent_child_hierarchy():
    """Demonstrate creating parent-child buffer relationships."""
    
    if engine.init() != 0:
        print("tuix_init() failed")
        return 1
    
    builders.register_standard()
    scenes.init_scene(b"HierarchyScene")
    scenes.select_scene(b"HierarchyScene")
    input.listen()
    
    # Create parent canvas (larger, background)
    parent_uid = objects.create_object(
        builders.CANVAS,
        b"HierarchyScene",
        0.8, 0.6,  # 80% width, 60% height
        0.1, 0.1   # 10% margin top, 10% margin left
    )
    
    # Create child canvas (smaller, foreground)
    child_uid = objects.create_object(
        builders.CANVAS,
        b"HierarchyScene",
        0.4, 0.3,  # 40% width, 30% height
        0.3, 0.3   # 30% margin top, 30% margin left
    )
    
    # Establish parent-child relationship
    result = buffers.set_buffer_parent(b"HierarchyScene", child_uid, parent_uid)
    if result != 0:
        print(f"Failed to set parent-child relationship: {result}")
    
    # Draw on both buffers
    engine.main_loop()  # Initialize geometry
    time.sleep(0.1)
    
    parent_obj = objects.get_object_by_uid(parent_uid)
    child_obj = objects.get_object_by_uid(child_uid)
    
    snap_parent = buffers.get_buffer_snapshot(b"HierarchyScene", parent_uid)
    snap_child = buffers.get_buffer_snapshot(b"HierarchyScene", child_uid)
    
    if parent_obj and snap_parent and snap_parent['width'] > 0:
        pw = snap_parent['width']
        ph = snap_parent['height']
        # Draw parent border
        objects.tuix_canvas_draw_rect(parent_obj, 0, 0, pw, ph, b'.', 0, 80, 200, 200, 0, 0, 0)
        objects.tuix_canvas_draw_text(parent_obj, 1, 1, b"PARENT", 255, 255, 0, 0, 0, 0)
    
    if child_obj and snap_child and snap_child['width'] > 0:
        cw = snap_child['width']
        ch = snap_child['height']
        # Draw child border
        objects.tuix_canvas_draw_rect(child_obj, 0, 0, cw, ch, b'*', 0, 255, 100, 100, 0, 0, 0)
        objects.tuix_canvas_draw_text(child_obj, 1, 1, b"CHILD", 255, 255, 255, 0, 0, 0)
    
    # Run for a few frames, showing the hierarchy
    for frame in range(5):
        engine.main_loop()
        time.sleep(0.2)
        snap = input.peek_snapshot()
        if snap and snap.keyboard and snap.keyboard.has_event:
            if snap.keyboard.code == 0x1B:  # ESC key
                break
    
    # Cleanup
    buffers.free_buffer(b"HierarchyScene", parent_uid)
    buffers.free_buffer(b"HierarchyScene", child_uid)
    scenes.free_scene(b"HierarchyScene")
    input.stop()
    engine.shutdown()
    
    return 0


def demo_z_index_layering():
    """Demonstrate z-index for controlling buffer layering."""
    
    if engine.init() != 0:
        print("tuix_init() failed")
        return 1
    
    builders.register_standard()
    scenes.init_scene(b"ZIndexScene")
    scenes.select_scene(b"ZIndexScene")
    input.listen()
    
    # Create three overlapping canvases
    uids = []
    colors = [
        ("RED", 255, 0, 0),
        ("GREEN", 0, 255, 0),
        ("BLUE", 0, 0, 255),
    ]
    
    for i, (name, r, g, b) in enumerate(colors):
        uid = objects.create_object(
            builders.CANVAS,
            b"ZIndexScene",
            0.6, 0.5,        # All same size
            0.1 + i * 0.15,  # Offset vertically
            0.1 + i * 0.15   # Offset horizontally
        )
        uids.append(uid)
        
        # Set z-index (higher = on top)
        z_result = buffers.set_buffer_z_index(b"ZIndexScene", uid, i)
    
    # Draw and render
    engine.main_loop()
    time.sleep(0.1)
    
    for uid, (name, r, g, b) in zip(uids, colors):
        obj = objects.get_object_by_uid(uid)
        snap = buffers.get_buffer_snapshot(b"ZIndexScene", uid)
        
        if obj and snap and snap['width'] > 0:
            w = snap['width']
            h = snap['height']
            # Draw colored rectangle
            objects.tuix_canvas_draw_rect(obj, 0, 0, w, h, b'#', 1, r, g, b, 0, 0, 0)
            # Add label
            lbl = name.encode()
            objects.tuix_canvas_draw_text(obj, 1, 1, lbl, 255, 255, 255, 0, 0, 0)
    
    # Render with z-ordering
    for frame in range(5):
        engine.main_loop()
        time.sleep(0.5)
    
    # Dynamically change z-ordering
    buffers.set_buffer_z_index(b"ZIndexScene", uids[2], 10)  # BLUE gets highest z
    
    for frame in range(3):
        engine.main_loop()
        time.sleep(0.3)
    
    # Cleanup
    for uid in uids:
        buffers.free_buffer(b"ZIndexScene", uid)
    scenes.free_scene(b"ZIndexScene")
    input.stop()
    engine.shutdown()
    
    return 0


def main():
    """Run both hierarchy demonstrations."""
    
    # Run parent-child hierarchy demo
    if demo_parent_child_hierarchy() != 0:
        return 1
    
    time.sleep(1)
    
    # Run z-index layering demo
    if demo_z_index_layering() != 0:
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
