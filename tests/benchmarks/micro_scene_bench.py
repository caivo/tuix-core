#!/usr/bin/env python3
import struct
import time
import sys

from tuix.core import _tuix_cy as cy


def build_command_buffer(n_objects: int):
    # string table: builder at index 0, scene at index 1
    builder = b"canvas"
    scene = b"scene0"
    str_payload = builder + b"\x00" + scene + b"\x00"
    parts = []
    # CMD 255: string table
    parts.append(struct.pack('<B I', 255, len(str_payload)))
    parts.append(str_payload)
    # CMD 1: init_scene (payload uint32 index of scene)
    parts.append(struct.pack('<B I I', 1, 4, 1))
    # many create_object entries
    for i in range(n_objects):
        # builder_idx=0, scene_idx=1, floats w,h,mt,ml
        payload = struct.pack('<I I f f f f', 0, 1, 10.0, 3.0, 0.0, 0.0)
        parts.append(struct.pack('<B I', 10, len(payload)))
        parts.append(payload)
    return b''.join(parts)


if __name__ == '__main__':
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 1000
    cy.init_registry()
    cy.tuix_register_standart_builders()
    buf = build_command_buffer(n)
    # warmup
    cy.execute_command_buffer(buf)
    t0 = time.perf_counter()
    processed = cy.execute_command_buffer(buf)
    t1 = time.perf_counter()
    dt = t1 - t0
    print(f"processed commands: {processed}")
    print(f"objects: {n}")
    print(f"total time: {dt:.6f} s")
    print(f"per object: {dt / n * 1e6:.2f} us")
