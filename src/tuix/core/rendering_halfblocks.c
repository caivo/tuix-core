#include "rendering.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(_WIN32)
#include <io.h>
#define is_stdout_tty() (_isatty(_fileno(stdout)))
#else
#include <unistd.h>
#define is_stdout_tty() (isatty(fileno(stdout)))
#endif

#ifdef _WIN32
#include <windows.h>

static void enable_ansi_escape_codes(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

#ifdef __GNUC__
__attribute__((constructor))
#endif
static void tuix_init_console(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setvbuf(stdout, NULL, _IONBF, 0);
    enable_ansi_escape_codes();
}

#ifdef _MSC_VER
#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) static void (*_tuix_init_console_ctor_hb)(void) = tuix_init_console;
#endif

#endif

#define CHUNK_SIZE (256 * 1024)
#define TUIX_SYM_BYTES 8
#define TUIX_NO_STATS

#ifndef TUIX_NO_STATS
static unsigned long long quant_frame_counter = 0;
static unsigned long quant_frame_pixels = 0;
static unsigned long quant_cache_misses = 0;
static unsigned long quant_calls = 0;
static unsigned long quant_strength_buckets[6] = {0};
static unsigned long skipped_by_pair_hash = 0;
static unsigned long skipped_by_pixel_equal = 0;
static unsigned long emitted_pixels = 0;
#endif

/* ========================================================= */
/* Color helpers (copied from rendering.c) */

static inline int ansi_gray_24(int v) {
    return 232 + (v * 23 + 127) / 255;
}

static inline int rgb_to_6(int v) {
    return (v * 5 + 127) / 255;
}

static inline int ansi_cube_216(int r, int g, int b) {
    return 16 + 36 * rgb_to_6(r) + 6 * rgb_to_6(g) + rgb_to_6(b);
}

static inline int rgb_to_8_bit(int r, int g, int b) {
    int mx = r; if (g > mx) mx = g; if (b > mx) mx = b;
    int mn = r; if (g < mn) mn = g; if (b < mn) mn = b;

    if (mx - mn <= 2) {
        return ansi_gray_24((r + g + b) / 3);
    }
    return ansi_cube_216(r, g, b);
}

/* Convert an ANSI 0-255 index to RGB (approximate xterm palette) */
static inline void ansi_index_to_rgb(int idx, TuixRGBTuple *out) {
    static const int basic[16][3] = {
        {0,0,0}, {128,0,0}, {0,128,0}, {128,128,0}, {0,0,128}, {128,0,128}, {0,128,128}, {192,192,192},
        {128,128,128}, {255,0,0}, {0,255,0}, {255,255,0}, {0,0,255}, {255,0,255}, {0,255,255}, {255,255,255}
    };

    if (idx >= 16 && idx <= 231) {
        int c = idx - 16;
        int r = c / 36;
        int g = (c % 36) / 6;
        int b = c % 6;
        const int levels[6] = {0,95,135,175,215,255};
        out->r = levels[r]; out->g = levels[g]; out->b = levels[b];
    } else if (idx >= 232 && idx <= 255) {
        int v = 8 + 10 * (idx - 232);
        out->r = out->g = out->b = v;
    } else {
        out->r = basic[idx & 15][0];
        out->g = basic[idx & 15][1];
        out->b = basic[idx & 15][2];
    }
}


/* prev_bg: flat array [H*W] storing only terminal-visible bg color keys from
   the previous frame. Halfblocks only compare bg output, so this is enough. */
static unsigned int *prev_bg = NULL;
static int prev_w = 0, prev_h = 0;

/* Pre-built buffer of repeated \xe2\x96\x80 (U+2580 ▀) for fast memcpy */
#define HB_RUN_MAX 512
static char halfblock_run[HB_RUN_MAX * 3];
static int halfblock_run_ready = 0;

/* Persistent pair hash buffers */
static unsigned long long *prev_pair_hash = NULL;
static unsigned long long *curr_pair_hash = NULL;
static int prev_num_pairs = 0;
static inline unsigned int color_render_key(TuixRGBTuple c);

#ifdef _WIN32
static HANDLE g_stdout_handle = INVALID_HANDLE_VALUE;
static inline void flush_chunk(char *out, size_t *pos) {
    if (*pos == 0) return;
    if (g_stdout_handle == INVALID_HANDLE_VALUE)
        g_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    WriteFile(g_stdout_handle, out, (DWORD)*pos, &written, NULL);
    *pos = 0;
}
#else
static inline void flush_chunk(char *out, size_t *pos) {
    if (*pos == 0) return;
    fwrite(out, 1, *pos, stdout);
    *pos = 0;
}
#endif

/* FNV-1a row-pair hash using terminal-visible bg color choices. */
static inline unsigned long long halfblock_row_pair_hash(TuixPixel *row1, TuixPixel *row2, int w) {
    unsigned long long h = 0xcbf29ce484222325ULL;
    if (!row1) return h;
    for (int i = 0; i < w; i++) {
        unsigned int c1 = color_render_key(row1[i].styles.bg);
        h = (h ^ c1) * 0x100000001b3ULL;
        if (row2) {
            unsigned int c2 = color_render_key(row2[i].styles.bg);
            h = (h ^ c2) * 0x100000001b3ULL;
        }
    }
    return h;
}



/* SGR DIFF EMITTER (uses truecolor codes but quantizes to 16-bit RGB first) */
// Return: 0=ANSI16, 1=ANSI256, 2=12bit, 3=18bit, 4=Gray, 5=Truecolor. Fills *out_idx or *out_rgb
static int ansi_best_match_inner(const TuixRGBTuple *target, int tol, int *out_idx, TuixRGBTuple *out_rgb) {
    static const int ansi16[16][3] = {
        {0,0,0}, {128,0,0}, {0,128,0}, {128,128,0}, {0,0,128}, {128,0,128}, {0,128,128}, {192,192,192},
        {128,128,128}, {255,0,0}, {0,255,0}, {255,255,0}, {0,0,255}, {255,0,255}, {0,255,255}, {255,255,255}
    };
    for (int i = 0; i < 16; i++) {
        int dr = abs(target->r - ansi16[i][0]);
        int dg = abs(target->g - ansi16[i][1]);
        int db = abs(target->b - ansi16[i][2]);
        int dist = dr; if (dg > dist) dist = dg; if (db > dist) dist = db;
        if (dist <= tol) { if (out_idx) *out_idx = i; return 0; }
    }

    /* ANSI256 check: start from index 16 (0-15 already checked above) */
    static TuixRGBTuple pal256[256];
    static int pal256_init = 0;
    if (!pal256_init) { for (int i = 0; i < 256; i++) ansi_index_to_rgb(i, &pal256[i]); pal256_init = 1; }
    for (int i = 16; i < 256; i++) {
        int dr = abs(target->r - pal256[i].r);
        int dg = abs(target->g - pal256[i].g);
        int db = abs(target->b - pal256[i].b);
        int dist = dr; if (dg > dist) dist = dg; if (db > dist) dist = db;
        if (dist <= tol) { if (out_idx) *out_idx = i; return 1; }
    }

    /* Modes 2-4 (12-bit cube, 18-bit cube, gray) all emit 38;2;R;G;B - same
       format as truecolor mode 5.  The 292 extra iterations provide no bandwidth
       savings and are skipped entirely.  Fall through to truecolor. */
    if (out_rgb) *out_rgb = *target;
    return 5;
}

/* Precomputed ANSI best-match LUT - direct-indexed by rgb16 5-6-5 bits.
   65536 entries × 2 bytes = 128KB (vs 1MB abm_cache).
   Packed: bits [15:9]=mode, bits [8:0]=palette index. */
#define ABM_LUT_SIZE 65536
static unsigned short abm_lut[ABM_LUT_SIZE];
static int abm_lut_init = 0;

static void abm_lut_build(void) {
    for (unsigned int r5 = 0; r5 < 32; r5++) {
        int rv = (int)((r5 * 255u + 15u) / 31u);
        for (unsigned int g6 = 0; g6 < 64; g6++) {
            int gv = (int)((g6 * 255u + 31u) / 63u);
            for (unsigned int b5 = 0; b5 < 32; b5++) {
                int bv = (int)((b5 * 255u + 15u) / 31u);
                TuixRGBTuple t; t.r = (unsigned char)rv; t.g = (unsigned char)gv; t.b = (unsigned char)bv;
                int idx = 0;
                int mode = ansi_best_match_inner(&t, 3, &idx, NULL);
                unsigned int ci = (r5 << 11) | (g6 << 5) | b5;
                abm_lut[ci] = (unsigned short)((mode << 9) | (idx & 0x1FF));
            }
        }
    }
    abm_lut_init = 1;
}

static inline int ansi_best_match(const TuixRGBTuple *target, int *out_idx) {
    if (!abm_lut_init) abm_lut_build();
    unsigned int r5 = ((unsigned int)target->r * 31u + 127u) / 255u;
    unsigned int g6 = ((unsigned int)target->g * 63u + 127u) / 255u;
    unsigned int b5 = ((unsigned int)target->b * 31u + 127u) / 255u;
    unsigned short v = abm_lut[(r5 << 11) | (g6 << 5) | b5];
    if (out_idx) *out_idx = v & 0x1FF;
    return v >> 9;
}

static inline unsigned int color_render_key(TuixRGBTuple c) {
    if (!abm_lut_init) abm_lut_build();
    unsigned int r5 = ((unsigned int)c.r * 31u + 127u) / 255u;
    unsigned int g6 = ((unsigned int)c.g * 63u + 127u) / 255u;
    unsigned int b5 = ((unsigned int)c.b * 31u + 127u) / 255u;
    unsigned int rgb565 = (r5 << 11) | (g6 << 5) | b5;
    unsigned short v = abm_lut[rgb565];
    unsigned int mode = v >> 9;
    if (mode == 5) return (mode << 16) | rgb565;
    return (mode << 16) | (v & 0x1FF);
}

/* Fast non-negative integer to ASCII (up to 5 digits, 0–99999) */
static inline int itoa3(char *buf, int val) {
    if (val >= 10000) { buf[0] = '0' + val/10000; buf[1] = '0' + (val/1000)%10; buf[2] = '0' + (val/100)%10; buf[3] = '0' + (val/10)%10; buf[4] = '0' + val%10; return 5; }
    if (val >= 1000)  { buf[0] = '0' + val/1000; buf[1] = '0' + (val/100)%10; buf[2] = '0' + (val/10)%10; buf[3] = '0' + val%10; return 4; }
    if (val >= 100)   { buf[0] = '0' + val/100; buf[1] = '0' + (val/10)%10; buf[2] = '0' + val%10; return 3; }
    if (val >= 10)    { buf[0] = '0' + val/10; buf[1] = '0' + val%10; return 2; }
    buf[0] = '0' + val; return 1;
}


/* ═══════════════════════════════════════════════════════════════
 *  Compact color emitter - picks shortest encoding per color:
 *  ANSI16 (2-3 bytes), ANSI256 (6-8 bytes), or truecolor (12-16).
 *  Uses the precomputed abm_lut for O(1) lookups.
 * ═══════════════════════════════════════════════════════════════ */
static inline int emit_color_code(char *buf, int n, const TuixRGBTuple *c, int is_bg) {
    int idx = 0;
    int mode = ansi_best_match(c, &idx);
    if (mode == 0) {
        /* ANSI16: 2-3 byte code - e.g. "31" or "102" */
        int code = is_bg ? ((idx < 8) ? (40 + idx) : (100 + idx - 8))
                        : ((idx < 8) ? (30 + idx) : (90 + idx - 8));
        n += itoa3(buf + n, code);
    } else if (mode == 1) {
        /* ANSI256: LUT guarantees tol<=3, always within threshold */
        memcpy(buf + n, is_bg ? "48;5;" : "38;5;", 5); n += 5;
        n += itoa3(buf + n, idx);
    } else {
        memcpy(buf + n, is_bg ? "48;2;" : "38;2;", 5); n += 5;
        n += itoa3(buf + n, c->r); buf[n++] = ';';
        n += itoa3(buf + n, c->g); buf[n++] = ';';
        n += itoa3(buf + n, c->b);
    }
    return n;
}

static inline void emit_halfblock_sgr(
    char *out, size_t *pos,
    const TuixRGBTuple *fg, const TuixRGBTuple *bg,
    TuixRGBTuple *cur_fg, TuixRGBTuple *cur_bg,
    int *have_style, int *cur_fg_valid)
{
    int need_fg = !*have_style || !*cur_fg_valid || fg->r != cur_fg->r || fg->g != cur_fg->g || fg->b != cur_fg->b;
    int need_bg = !*have_style || bg->r != cur_bg->r || bg->g != cur_bg->g || bg->b != cur_bg->b;
    if (!need_fg && !need_bg) return;

    int n = (int)*pos;
    out[n++] = '\x1b'; out[n++] = '[';
    if (!*have_style) {
        out[n++] = '0'; out[n++] = ';';
        *have_style = 1;
    }

    if (need_fg) {
        n = emit_color_code(out, n, fg, 0);
        *cur_fg_valid = 1;
    }
    if (need_bg) {
        if (need_fg) out[n++] = ';';
        n = emit_color_code(out, n, bg, 1);
    }
    out[n++] = 'm';
    *pos = (size_t)n;
    *cur_fg = *fg;
    *cur_bg = *bg;
}

/* When fg==bg (solid color), emit space with bg-only - saves ~15 bytes per cell */
static inline void emit_solid_sgr(
    char *out, size_t *pos,
    const TuixRGBTuple *bg,
    TuixRGBTuple *cur_fg, TuixRGBTuple *cur_bg,
    int *have_style, int *cur_fg_valid)
{
    int need_bg = !*have_style || bg->r != cur_bg->r || bg->g != cur_bg->g || bg->b != cur_bg->b;
    if (!need_bg) return;

    int n = (int)*pos;
    out[n++] = '\x1b'; out[n++] = '[';
    if (!*have_style) {
        out[n++] = '0'; out[n++] = ';';
        *have_style = 1;
    }
    n = emit_color_code(out, n, bg, 1);
    out[n++] = 'm';
    *pos = (size_t)n;
    /* Mark cur_fg invalid so next non-solid cell re-emits fg */
    *cur_fg_valid = 0;
    *cur_bg = *bg;
}

void tuix_render_streaming(
    TuixFinalBuffer *buffer,
    TuixRowDoneCallback on_row_done,
    void *user_data)
{
    if (!buffer || !buffer->pixels) return;

    int W = buffer->width;
    int H = buffer->height;

    if (W <= 0 || H <= 0) return;

    if (!halfblock_run_ready) {
        for (int i = 0; i < HB_RUN_MAX; i++) {
            halfblock_run[i * 3]     = (char)0xE2;
            halfblock_run[i * 3 + 1] = (char)0x96;
            halfblock_run[i * 3 + 2] = (char)0x80;
        }
        halfblock_run_ready = 1;
    }

    if (!prev_bg || prev_w != W || prev_h != H) {
        free(prev_bg);
        prev_bg = calloc((size_t)H * (size_t)W, sizeof(unsigned int));
        if (!prev_bg) return;

        prev_w = W;
        prev_h = H;
        buffer->full_redraw = 1;
    }

    static char *out = NULL;
    static size_t out_cap = 0;
    if (!out || out_cap < CHUNK_SIZE) {
        free(out);
        out = malloc(CHUNK_SIZE);
        out_cap = CHUNK_SIZE;
    }

    size_t pos = 0;
    char esc[64];

    /* Reset per-frame quantization stats */
#ifndef TUIX_NO_STATS
    quant_frame_counter++;
    quant_frame_pixels = (unsigned long)W * (unsigned long)H;
    quant_calls = 0;
    quant_cache_misses = 0;
    skipped_by_pair_hash = 0;
    skipped_by_pixel_equal = 0;
    emitted_pixels = 0;
    for (int _i = 0; _i < 6; _i++) quant_strength_buckets[_i] = 0;
#endif

    /* Quantization is now deferred: hash uses terminal-visible colors, and we only
       quantize pixels belonging to pairs whose hash actually changed.
       This eliminates the O(W*H) precompute for sparse-change frames. */

    int num_pairs = (H + 1) / 2;
    if (!prev_pair_hash || !curr_pair_hash || num_pairs != prev_num_pairs) {
        free(prev_pair_hash); free(curr_pair_hash);
        prev_pair_hash = malloc(sizeof(unsigned long long) * num_pairs);
        curr_pair_hash = malloc(sizeof(unsigned long long) * num_pairs);
        if (!prev_pair_hash || !curr_pair_hash) { free(prev_pair_hash); free(curr_pair_hash); prev_num_pairs = 0; return; }
        prev_num_pairs = num_pairs;
    }

    if (!buffer->full_redraw) {
        for (int y = 0; y < num_pairs; y++) {
            int y1 = y * 2;
            int y2 = y1 + 1;
            TuixPixel *curr_row1 = &buffer->pixels[(size_t)y1 * W];
            TuixPixel *curr_row2 = (y2 < H) ? &buffer->pixels[(size_t)y2 * W] : NULL;
            curr_pair_hash[y] = halfblock_row_pair_hash(curr_row1, curr_row2, W);
        }
    }

    /* Selective quantization: only precompute q_fg/q_bg for rows
       belonging to pairs whose hash changed (or on full redraw).
       Sparse-change frames (e.g. 1 dirty line) now quantize only
       the 1-2 affected rows instead of the entire W*H grid. */
    for (int yp = 0; yp < num_pairs; yp++) {
        if (!buffer->full_redraw && prev_pair_hash[yp] == curr_pair_hash[yp])
            continue;
        int y1 = yp * 2;
        int y2 = y1 + 1;
        for (int x = 0; x < W; x++) {
            TuixPixel *p = &buffer->pixels[(size_t)y1 * W + x];
            p->q_bg = tuix_rgb16(p->styles.bg);
            p->q_cached = 1;
        }
        if (y2 < H) {
            for (int x = 0; x < W; x++) {
                TuixPixel *p = &buffer->pixels[(size_t)y2 * W + x];
                p->q_bg = tuix_rgb16(p->styles.bg);
                p->q_cached = 1;
            }
        }
    }

    /* Cache is_stdout_tty result - avoids syscall per frame */
    static int g_is_tty = -1;
    if (g_is_tty < 0) g_is_tty = is_stdout_tty();
    if (g_is_tty) { memcpy(out + pos, "\x1b[?25l", 6); pos += 6; }

    TuixRGBTuple cur_fg = {0, 0, 0};
    TuixRGBTuple cur_bg = {0, 0, 0};
    int have_style = 0;
    int cur_fg_valid = 0;  /* whether cur_fg holds a meaningful value */
    int cursor_row = -1, cursor_col = -1;  /* 1-indexed terminal cursor */

    for (int y = 0; y < num_pairs; y++) {
        int y1 = y * 2;
        int y2 = y1 + 1;

        if (!buffer->pixels) {
            if (on_row_done) { on_row_done(buffer, y1, user_data); if (y2 < H) on_row_done(buffer, y2, user_data); }
            continue;
        }

        if (!buffer->full_redraw && prev_pair_hash[y] == curr_pair_hash[y]) {
            /* whole pair unchanged -> count skipped pixels */
#ifndef TUIX_NO_STATS
            skipped_by_pair_hash += (unsigned long)W * (unsigned long)((y2 < H) ? 2 : 1);
#endif
            if (on_row_done) { on_row_done(buffer, y1, user_data); if (y2 < H) on_row_done(buffer, y2, user_data); }
            continue;
        }

        int x = 0;
        while (x < W) {
            /* Skip unchanged pixels - bg-only comparison for halfblocks
               (halfblocks only use bg colors, so sym/fg/style changes are irrelevant) */
            if (!buffer->full_redraw) {
                while (x < W) {
                    unsigned int c1 = color_render_key(buffer->pixels[(size_t)y1 * W + x].styles.bg);
                    if (c1 != prev_bg[y1 * W + x]) break;
                    if (y2 < H) {
                        unsigned int c2 = color_render_key(buffer->pixels[(size_t)y2 * W + x].styles.bg);
                        if (c2 != prev_bg[y2 * W + x]) break;
                    }
                    x++;
                }
                if (x >= W) break;
            }

            /* Get quantized bg colors for top half / bottom half */
            TuixRGBTuple q_top = buffer->pixels[(size_t)y1 * W + x].q_cached
                ? buffer->pixels[(size_t)y1 * W + x].q_bg : tuix_rgb16(buffer->pixels[(size_t)y1 * W + x].styles.bg);
            TuixRGBTuple q_bot = (y2 < H)
                ? (buffer->pixels[(size_t)y2 * W + x].q_cached
                    ? buffer->pixels[(size_t)y2 * W + x].q_bg : tuix_rgb16(buffer->pixels[(size_t)y2 * W + x].styles.bg))
                : q_top;
            unsigned int top_key = color_render_key(buffer->pixels[(size_t)y1 * W + x].styles.bg);
            unsigned int bot_key = (y2 < H) ? color_render_key(buffer->pixels[(size_t)y2 * W + x].styles.bg) : top_key;

            /* Extend run: same terminal-visible bg colors in both rows */
            int run_start = x;
            int run_end = x + 1;
            for (; run_end < W; run_end++) {
                if (color_render_key(buffer->pixels[(size_t)y1 * W + run_end].styles.bg) != top_key) break;
                if (y2 < H) {
                    if (color_render_key(buffer->pixels[(size_t)y2 * W + run_end].styles.bg) != bot_key) break;
                }
            }

            /* Cursor positioning - only emit \e[Y;XH when needed */
            {
                int target_row = y + 1;
                int target_col = run_start + 1;
                if (cursor_row != target_row || cursor_col != target_col) {
                    int esc_len = 0;
                    esc[esc_len++] = '\x1b'; esc[esc_len++] = '[';
                    esc_len += itoa3(esc + esc_len, target_row);
                    esc[esc_len++] = ';';
                    esc_len += itoa3(esc + esc_len, target_col);
                    esc[esc_len++] = 'H';
                    memcpy(out + pos, esc, esc_len); pos += esc_len;
                }
            }

            /* Pick shortest encoding: solid (space+bg) or halfblock (▀+fg+bg) */
            int is_solid = (q_top.r == q_bot.r && q_top.g == q_bot.g && q_top.b == q_bot.b);
            if (is_solid) {
                emit_solid_sgr(out, &pos, &q_top, &cur_fg, &cur_bg, &have_style, &cur_fg_valid);
                int run_len = run_end - run_start;
                memset(out + pos, ' ', run_len);
                pos += run_len;
            } else {
                emit_halfblock_sgr(out, &pos, &q_top, &q_bot, &cur_fg, &cur_bg, &have_style, &cur_fg_valid);
                int run_len = run_end - run_start;
                memcpy(out + pos, halfblock_run, run_len * 3);
                pos += run_len * 3;
            }
            if (pos >= out_cap - 1024) flush_chunk(out, &pos);

            x = run_end;
            cursor_row = y + 1;
            cursor_col = run_end + 1;
        }

        if (on_row_done) { on_row_done(buffer, y1, user_data); if (y2 < H) on_row_done(buffer, y2, user_data); }
    }

    if (g_is_tty) { memcpy(out + pos, "\x1b[?25h", 6); pos += 6; }
    memcpy(out + pos, "\x1b[0m", 4); pos += 4;
    flush_chunk(out, &pos);
    fflush(stdout);

    /* Copy bg colors and update hashes for next frame */
    if (buffer->full_redraw) {
        /* Full redraw: copy all + compute hashes (we skipped hash loop at top) */
        for (int p = 0; p < num_pairs; p++) {
            int y1 = p * 2;
            int y2 = y1 + 1;
            TuixPixel *row1 = &buffer->pixels[(size_t)y1 * W];
            TuixPixel *row2 = (y2 < H) ? &buffer->pixels[(size_t)y2 * W] : NULL;
            if (row1) {
                unsigned int *dst = &prev_bg[y1 * W];
                for (int x = 0; x < W; x++) dst[x] = color_render_key(row1[x].styles.bg);
            }
            if (row2) {
                unsigned int *dst = &prev_bg[y2 * W];
                for (int x = 0; x < W; x++) dst[x] = color_render_key(row2[x].styles.bg);
            }
            prev_pair_hash[p] = halfblock_row_pair_hash(row1, row2, W);
        }
    } else {
        for (int p = 0; p < num_pairs; p++) {
            if (prev_pair_hash[p] != curr_pair_hash[p]) {
                int y1 = p * 2;
                int y2 = y1 + 1;
                if (buffer->pixels) {
                    unsigned int *dst = &prev_bg[y1 * W];
                    for (int x = 0; x < W; x++) dst[x] = color_render_key(buffer->pixels[(size_t)y1 * W + x].styles.bg);
                }
                if (y2 < H && buffer->pixels) {
                    unsigned int *dst = &prev_bg[y2 * W];
                    for (int x = 0; x < W; x++) dst[x] = color_render_key(buffer->pixels[(size_t)y2 * W + x].styles.bg);
                }
                prev_pair_hash[p] = curr_pair_hash[p];
            }
        }
    }

    if (buffer->full_redraw) buffer->full_redraw = 0;

#ifndef TUIX_NO_STATS
    /* Append per-frame quantization stats to DEBUG/quant_stats.csv */
    {
        FILE *f = NULL;
        /* check if file exists */
        f = fopen("quant_stats.csv", "r");
        int need_header = 0;
        if (!f) {
            need_header = 1;
        } else fclose(f);

        f = fopen("quant_stats.csv", "a");
        if (f) {
            if (need_header) fprintf(f, "frame,W,H,pixels,quant_calls,quant_misses,bucket_0,bucket_1_2,bucket_3_5,bucket_6_12,bucket_13_32,bucket_gt32,skipped_by_pair_hash,skipped_by_pixel_equal,emitted_pixels\n");
                fprintf(f, "%llu,%d,%d,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",
                        (unsigned long long)quant_frame_counter, W, H, quant_frame_pixels, quant_calls, quant_cache_misses,
                        quant_strength_buckets[0], quant_strength_buckets[1], quant_strength_buckets[2], quant_strength_buckets[3], quant_strength_buckets[4], quant_strength_buckets[5],
                        skipped_by_pair_hash, skipped_by_pixel_equal, emitted_pixels);
            fclose(f);
        }
    }
#endif
}
