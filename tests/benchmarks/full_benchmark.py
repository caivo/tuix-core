"""
TUIX Python Full-Pipeline Benchmark Suite (25 scenarios)
=========================================================
Uses NativeBuffer (C-level fills + render) matching
the C benchmark methodology exactly — same fill logic,
same parameters, same render path.
"""
import os
import sys
import time
import math
import json

from tuix.core._tuix_cy import (
    NativeBuffer,
    init, shutdown,
    tuix_register_standart_builders,
    tuix_init_scene, tuix_free_scene,
    tuix_create_object,
    set_terminal_size, set_current_scene_name,
    pipeline_tick, force_scene_redraw,
    tuix_free_buffer,
    time_now_us,
)

# ── Config ─────────────────────────────────────────────
WARMUP_N = 20
TERM_W, TERM_H = 120, 40

# ── Timer ──────────────────────────────────────────────
def now_us():
    return time.perf_counter() * 1e6

# ── RSS ───────────────────────────────────────────────
def rss_kb():
    try:
        import psutil
        return int(psutil.Process(os.getpid()).memory_info().rss / 1024)
    except ImportError:
        return 0

# ── Stats ─────────────────────────────────────────────
class Stats:
    __slots__ = ('n','min','max','mean','med','p95','p99','sd','ops','cv','ci95lo','ci95hi')
    def __init__(self):
        self.n = self.min = self.max = self.mean = self.med = 0
        self.p95 = self.p99 = self.sd = self.ops = self.cv = 0
        self.ci95lo = self.ci95hi = 0

def compute_stats(arr):
    s = Stats()
    n = len(arr)
    if n == 0:
        return s
    arr_sorted = sorted(arr)
    s.n = n
    s.min = arr_sorted[0]
    s.max = arr_sorted[-1]
    s.med = arr_sorted[n // 2]
    s.p95 = arr_sorted[int(n * 0.95)]
    s.p99 = arr_sorted[int(n * 0.99)]
    s.mean = sum(arr) / n
    var = sum((x - s.mean) ** 2 for x in arr) / n
    s.sd = math.sqrt(var)
    s.ops = 1e6 / s.mean if s.mean > 0 else 0
    s.cv = s.sd / s.mean if s.mean > 0 else 0
    m = 1.96 * s.sd / math.sqrt(n)
    s.ci95lo = s.mean - m
    s.ci95hi = s.mean + m
    return s

# ── Result ────────────────────────────────────────────
class Result:
    __slots__ = ('name','params_json','w','h','nf','frame_times',
                 'pty_bytes','rss_before','rss_after','total_wall_ms','st')
    def __init__(self, name, params, w, h, nf):
        self.name = name
        self.params_json = params
        self.w = w
        self.h = h
        self.nf = nf
        self.frame_times = [0.0] * nf
        self.pty_bytes = 0
        self.rss_before = 0
        self.rss_after = 0
        self.total_wall_ms = 0.0
        self.st = Stats()

# ── Fill IDs ──────────────────────────────────────────
FILL_NOOP           = 0
FILL_RANDOM         = 1
FILL_GRADIENT       = 2
FILL_CONTENT_UPDATE = 3
FILL_TREE           = 4
FILL_LAYOUT_GRID    = 5
FILL_SCROLL         = 6
FILL_VLIST          = 7
FILL_TABLE          = 8
FILL_DIRTY          = 9
FILL_DASHBOARD      = 10
FILL_FPS            = 11
FILL_INPUT          = 12
FILL_TRANSITION     = 13
FILL_DASH_NAV       = 14

def _do_fill(buf, fill_id, frame, extra):
    """Call the appropriate C-level fill method with correct params."""
    if fill_id == FILL_NOOP:
        return
    elif fill_id == FILL_RANDOM:
        buf.fill_random(frame)
    elif fill_id == FILL_GRADIENT:
        buf.fill_gradient(frame)
    elif fill_id == FILL_CONTENT_UPDATE:
        buf.fill_content_update(frame)
    elif fill_id == FILL_TREE:
        buf.fill_tree(extra[0], frame)
    elif fill_id == FILL_LAYOUT_GRID:
        buf.fill_layout_grid(extra[0], extra[1], frame)
    elif fill_id == FILL_SCROLL:
        buf.fill_scroll(frame)
    elif fill_id == FILL_VLIST:
        buf.fill_virtual_list(frame, extra[0])
    elif fill_id == FILL_TABLE:
        buf.fill_table(extra[0], frame)
    elif fill_id == FILL_DIRTY:
        buf.fill_dirty_rows(extra[0], frame)
    elif fill_id == FILL_DASHBOARD:
        buf.fill_dashboard(extra[0], frame)
    elif fill_id == FILL_FPS:
        buf.fill_fps_region(frame, extra[0])
    elif fill_id == FILL_INPUT:
        buf.fill_input_cell(frame)
    elif fill_id == FILL_TRANSITION:
        buf.fill_transition(frame)
    elif fill_id == FILL_DASH_NAV:
        buf.fill_dash_nav(extra[0], extra[1], frame)

# ── Runners ───────────────────────────────────────────

def run_normal(name, params, w, h, frames, fill_id, extra, full_redraw_each=0):
    r = Result(name, params, w, h, frames)
    buf = NativeBuffer(w, h)

    # initial fill
    _do_fill(buf, fill_id, 0, extra)

    # warmup
    for i in range(WARMUP_N):
        _do_fill(buf, fill_id, i, extra)
        if full_redraw_each:
            buf.full_redraw = 1
        buf.render()

    # PTY byte measurement
    _do_fill(buf, fill_id, 0, extra)
    buf.full_redraw = 1
    r.pty_bytes = len(buf.render_to_bytes())

    # measured run
    r.rss_before = rss_kb()
    wall0 = now_us()
    for i in range(frames):
        _do_fill(buf, fill_id, i, extra)
        if full_redraw_each:
            buf.full_redraw = 1
        t0 = now_us()
        buf.render()
        r.frame_times[i] = now_us() - t0
    r.total_wall_ms = (now_us() - wall0) / 1000.0
    r.rss_after = rss_kb()
    r.st = compute_stats(r.frame_times)
    return r


def run_startup(name, params, w, h, frames, fill_id, extra):
    r = Result(name, params, w, h, frames)

    # warmup
    for i in range(WARMUP_N):
        buf = NativeBuffer(w, h)
        _do_fill(buf, fill_id, i, extra)
        buf.full_redraw = 1
        buf.render()
        del buf

    # PTY bytes
    buf = NativeBuffer(w, h)
    _do_fill(buf, fill_id, 0, extra)
    buf.full_redraw = 1
    r.pty_bytes = len(buf.render_to_bytes())
    del buf

    # measured run
    r.rss_before = rss_kb()
    wall0 = now_us()
    for i in range(frames):
        t0 = now_us()
        buf = NativeBuffer(w, h)
        _do_fill(buf, fill_id, i, extra)
        buf.full_redraw = 1
        buf.render()
        del buf
        r.frame_times[i] = now_us() - t0
    r.total_wall_ms = (now_us() - wall0) / 1000.0
    r.rss_after = rss_kb()
    r.st = compute_stats(r.frame_times)
    return r


# ── Output ────────────────────────────────────────────

def write_results(dirpath, framework, defW, defH, results):
    os.makedirs(dirpath, exist_ok=True)

    # JSON
    path = os.path.join(dirpath, "benchmark.json")
    out = {"meta": {}, "results": []}
    out["meta"]["timestamp"] = time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())
    plat = "linux"
    if sys.platform == "win32":
        plat = "win32"
    elif sys.platform == "darwin":
        plat = "darwin"
    out["meta"]["platform"] = plat
    out["meta"]["framework"] = framework
    out["meta"]["defaultResolution"] = "%dx%d" % (defW, defH)
    out["meta"]["terminalResolution"] = "%dx%d" % (TERM_W, TERM_H)
    out["meta"]["warmup"] = WARMUP_N

    for r in results:
        st = r.st
        ms = 1.0 / 1000.0
        timing = {
            "n": st.n,
            "mean": st.mean * ms,
            "median": st.med * ms,
            "p95": st.p95 * ms,
            "p99": st.p99 * ms,
            "min": st.min * ms,
            "max": st.max * ms,
            "stddev": st.sd * ms,
            "cv": st.cv,
            "meanCi95Low": st.ci95lo * ms,
            "meanCi95High": st.ci95hi * ms,
        }
        resobj = {
            "scenario": r.name,
            "framework": framework,
            "params": json.loads(r.params_json) if isinstance(r.params_json, str) else (r.params_json or {}),
            "metrics": {
                "timing": timing,
                "memBefore": {"rssKb": r.rss_before},
                "memAfter": {"rssKb": r.rss_after},
                "memPeak": {"rssKb": r.rss_after},
                "rssGrowthKb": max(0, r.rss_after - r.rss_before),
                "iterations": r.nf,
                "totalWallMs": r.total_wall_ms,
                "opsPerSec": st.ops,
                "framesProduced": r.nf,
                "bytesProduced": int(r.pty_bytes) * int(r.nf),
                "ptyBytesObserved": int(r.pty_bytes),
            }
        }
        out["results"].append(resobj)
    with open(path, "w") as f:
        json.dump(out, f, indent=2)

    # Summary CSV
    path = os.path.join(dirpath, "benchmark_summary.csv")
    with open(path, "w") as f:
        f.write("scenario,params,resolution,frames,mean_us,median_us,p95_us,p99_us,min_us,max_us,stddev_us,cv,ci95_low_us,ci95_high_us,ops_s,bytes_pty,peak_rss_kb,total_wall_ms\n")
        for r in results:
            st = r.st
            f.write("%s,%s,%dx%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.4f,%.2f,%.2f,%.0f,%d,%d,%.3f\n" % (
                r.name, r.params_json, r.w, r.h, r.nf,
                st.mean, st.med, st.p95, st.p99, st.min, st.max, st.sd, st.cv,
                st.ci95lo, st.ci95hi, st.ops,
                int(r.pty_bytes) * int(r.nf), r.rss_after, r.total_wall_ms))

    # Frames CSV
    path = os.path.join(dirpath, "benchmark_frames.csv")
    with open(path, "w") as f:
        f.write("scenario,frame,time_us\n")
        for r in results:
            for i, t in enumerate(r.frame_times):
                f.write("%s,%d,%.2f\n" % (r.name, i, t))

    sys.stderr.write("  -> %s/benchmark.json\n" % dirpath)
    sys.stderr.write("  -> %s/benchmark_summary.csv\n" % dirpath)
    sys.stderr.write("  -> %s/benchmark_frames.csv\n" % dirpath)


# ── Redirect stdout to NUL ────────────────────────────
def redirect_null():
    sys.stdout.flush()
    devnull = os.open(os.devnull, os.O_WRONLY)
    os.dup2(devnull, 1)
    os.close(devnull)


# ── Main ──────────────────────────────────────────────
def main():
    W, H = 80, 24
    FRAMEWORK = "TUIX.Py"
    TOTAL = 25

    redirect_null()

    sys.stderr.write("\n")
    sys.stderr.write("  ========================================================\n")
    sys.stderr.write("   TUIX Core – Full Pipeline Benchmark (Python/Cython)\n")
    sys.stderr.write("   %d scenarios   Default: %dx%d   Terminal: %dx%d\n" % (TOTAL, W, H, TERM_W, TERM_H))
    sys.stderr.write("   Warm-up: %d   NativeBuffer + C-level fills (C-matching)\n" % WARMUP_N)
    sys.stderr.write("  ========================================================\n\n")

    results = []
    n = 0

    def log(label):
        nonlocal n
        n += 1
        sys.stderr.write("  [%2d/%d] %s\n" % (n, TOTAL, label))

    # 1. startup
    log("startup")
    results.append(run_startup("startup", "{}", W, H, 100,
                               FILL_RANDOM, ()))

    # 2-5. tree-construction
    for items in [10, 100, 500, 1000]:
        log("tree-construction items=%d" % items)
        results.append(run_startup("tree-construction",
                                   json.dumps({"items": items}), W, H, 500,
                                   FILL_TREE, (items,)))

    # 6. rerender
    log("rerender")
    results.append(run_normal("rerender", "{}", W, H, 1000,
                              FILL_NOOP, (), 0))

    # 7. content-update
    log("content-update")
    results.append(run_normal("content-update", "{}", W, H, 500,
                              FILL_CONTENT_UPDATE, (), 0))

    # 8. layout-stress
    log("layout-stress")
    results.append(run_normal("layout-stress",
                              json.dumps({"rows": 10, "cols": 4}), W, H, 300,
                              FILL_LAYOUT_GRID, (10, 4), 1))

    # 9. scroll-stress
    log("scroll-stress")
    results.append(run_normal("scroll-stress",
                              json.dumps({"items": 2000}), W, H, 50,
                              FILL_SCROLL, (), 0))

    # 10. virtual-list
    log("virtual-list")
    results.append(run_normal("virtual-list",
                              json.dumps({"items": 100000, "viewport": 40}), W, H, 1000,
                              FILL_VLIST, (100000,), 0))

    # 11. tables
    log("tables")
    results.append(run_normal("tables",
                              json.dumps({"rows": 100, "cols": 8}), W, H, 300,
                              FILL_TABLE, (8,), 0))

    # 12. memory-profile
    log("memory-profile")
    results.append(run_normal("memory-profile", "{}", W, H, 2000,
                              FILL_RANDOM, (), 1))

    # 13. terminal-rerender
    log("terminal-rerender")
    results.append(run_normal("terminal-rerender", "{}",
                              TERM_W, TERM_H, 1000,
                              FILL_NOOP, (), 0))

    # 14. terminal-frame-fill dirty=1
    log("terminal-frame-fill dirty=1")
    results.append(run_normal("terminal-frame-fill",
                              json.dumps({"dirty": 1}), TERM_W, TERM_H, 1000,
                              FILL_DIRTY, (1,), 0))

    # 15. terminal-frame-fill dirty=40
    log("terminal-frame-fill dirty=40")
    results.append(run_normal("terminal-frame-fill",
                              json.dumps({"dirty": 40}), TERM_W, TERM_H, 1000,
                              FILL_DIRTY, (40,), 0))

    # 16. terminal-screen-transition
    log("terminal-screen-transition")
    results.append(run_normal("terminal-screen-transition", "{}",
                              TERM_W, TERM_H, 500,
                              FILL_TRANSITION, (), 1))

    # 17. terminal-fps-stream
    log("terminal-fps-stream")
    results.append(run_normal("terminal-fps-stream",
                              json.dumps({"services": 12}), TERM_W, TERM_H, 2000,
                              FILL_FPS, (12,), 0))

    # 18. terminal-input-latency
    log("terminal-input-latency")
    results.append(run_normal("terminal-input-latency", "{}",
                              TERM_W, TERM_H, 1000,
                              FILL_INPUT, (), 0))

    # 19. terminal-memory-soak
    log("terminal-memory-soak")
    pj = json.dumps({"rows": TERM_H, "cols": TERM_W})
    results.append(run_normal("terminal-memory-soak", pj,
                              TERM_W, TERM_H, 1200,
                              FILL_RANDOM, (), 1))

    # 20. terminal-full-ui
    log("terminal-full-ui")
    results.append(run_normal("terminal-full-ui",
                              json.dumps({"services": 24}), TERM_W, TERM_H, 1200,
                              FILL_DASHBOARD, (24,), 0))

    # 21. terminal-full-ui-navigation
    log("terminal-full-ui-navigation")
    results.append(run_normal("terminal-full-ui-navigation",
                              json.dumps({"services": 24, "dwell": 8}), TERM_W, TERM_H, 1200,
                              FILL_DASH_NAV, (24, 8), 0))

    # 22. terminal-strict-ui
    log("terminal-strict-ui")
    results.append(run_normal("terminal-strict-ui",
                              json.dumps({"services": 24}), TERM_W, TERM_H, 1200,
                              FILL_DASHBOARD, (24,), 0))

    # 23. terminal-strict-ui-navigation
    log("terminal-strict-ui-navigation")
    results.append(run_normal("terminal-strict-ui-navigation",
                              json.dumps({"services": 24, "dwell": 8}), TERM_W, TERM_H, 1200,
                              FILL_DASH_NAV, (24, 8), 0))

    # 24. terminal-virtual-list
    log("terminal-virtual-list")
    results.append(run_normal("terminal-virtual-list",
                              json.dumps({"items": 100000, "viewport": 40}), TERM_W, TERM_H, 1000,
                              FILL_VLIST, (100000,), 0))

    # 25. terminal-table
    log("terminal-table")
    results.append(run_normal("terminal-table",
                              json.dumps({"rows": 40, "cols": 8}), TERM_W, TERM_H, 500,
                              FILL_TABLE, (8,), 0))

    # write outputs and print summary
    write_results('.', FRAMEWORK, W, H, results)

    sys.stderr.write("\n")
    sys.stderr.write("  %-36s %9s %9s %9s %9s %9s %9s %10s\n" % (
        "Scenario", "Mean(us)", "Med(us)", "P95(us)", "P99(us)", "SD(us)", "ops/s", "PTY-bytes"))
    sys.stderr.write("  " + ("-" * (36 + 9 * 6 + 10 + 7)) + "\n")
    for r in results:
        st = r.st
        label = r.name
        if len(label) > 36:
            label = label[:36]
        sys.stderr.write("  %-36s %9.1f %9.1f %9.1f %9.1f %9.1f %9.0f %10d\n" % (
            label, st.mean, st.med, st.p95, st.p99, st.sd, st.ops, int(r.pty_bytes)))
    sys.stderr.write("\n")


if __name__ == "__main__":
    main()