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
/* MSVC equivalent of __attribute__((constructor)): place a pointer in the
   CRT initializer table so tuix_init_console runs before main/DllMain. */
#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) static void (*_tuix_init_console_ctor)(void) = tuix_init_console;
#endif

#endif

#define CHUNK_SIZE (256 * 1024)
#define TUIX_SYM_BYTES 8

static inline int ansi_gray_24(int v) {
    return 232 + (v * 23 + 127) / 255;
}

static inline int rgb_to_6(int v) {
    return (v * 5 + 127) / 255;
}

static inline int ansi_cube_216(int r, int g, int b) {
    return 16 + 36 * rgb_to_6(r) + 6 * rgb_to_6(g) + rgb_to_6(b);
}


static inline void ansi_index_to_rgb(int idx, TuixRGBTuple *out) {
    static const int basic[16][3] = {
        {0,0,0}, {128,0,0}, {0,128,0}, {128,128,0}, {0,0,128}, {128,0,128}, {0,128,128}, {192,192,192},
        {128,128,128}, {255,0,0}, {0,255,0}, {255,255,0}, {0,0,255}, {255,0,255}, {0,255,255}, {255,255,255}
    };

    if (idx >= 16 && idx <= 231) {
        int c = idx - 16;
        int r = c / 36, g = (c % 36) / 6, b = c % 6;
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


static TuixPixel *prev_pixels = NULL;
static int prev_w = 0, prev_h = 0;

static unsigned long long *prev_hash = NULL;
static unsigned long long *curr_hash = NULL;
static int hash_cap = 0;

/* Number of bytes written by the most recent tuix_render_streaming() call. */
static size_t last_frame_bytes = 0;

/* When set, force the next render to do a full redraw because manual
    dirty rows were requested before hash arrays were initialized. */
static int manual_dirty_all = 0;

static inline unsigned int color_render_key(TuixRGBTuple c);

void tuix_render_invalidate_all(void) {
    manual_dirty_all = 1;
}

/* FNV-1a row hash using terminal-visible color choices. */
static inline unsigned long long row_hash(TuixPixel *row, int w) {
    unsigned long long h = 0xcbf29ce484222325ULL;
    for (int i = 0; i < w; i++) {
        for (int k = 0; k < TUIX_SYM_BYTES && row[i].sym[k]; k++)
            h = (h ^ (unsigned long long)(unsigned char)row[i].sym[k]) * 0x100000001b3ULL;

        TuixPixelStyles *s = &row[i].styles;
        unsigned int bits = (s->bold & 1) | ((s->dim & 1) << 1) | ((s->italic & 1) << 2)
                          | ((s->underlined & 1) << 3) | ((s->custom_fg & 1) << 4) | ((s->custom_bg & 1) << 5);
        h = (h ^ bits) * 0x100000001b3ULL;
        if (s->custom_fg) {
            unsigned int fg_key = color_render_key(s->fg);
            h = (h ^ fg_key) * 0x100000001b3ULL;
        }
        if (s->custom_bg) {
            unsigned int bg_key = color_render_key(s->bg);
            h = (h ^ bg_key) * 0x100000001b3ULL;
        }
    }
    return h;
}

#ifdef _WIN32
static inline void flush_chunk(char *out, size_t *pos) {
    if (*pos == 0) return;
    HANDLE h = (HANDLE)_get_osfhandle(_fileno(stdout));
    if (h == INVALID_HANDLE_VALUE) h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    WriteFile(h, out, (DWORD)*pos, &written, NULL);
    last_frame_bytes += (size_t)written;
    *pos = 0;
}
#else
static inline void flush_chunk(char *out, size_t *pos) {
    if (*pos == 0) return;
    size_t written = fwrite(out, 1, *pos, stdout);
    last_frame_bytes += written;
    *pos = 0;
}
#endif

static inline int pixels_equal(const TuixPixel *a, const TuixPixel *b) {
    if (!a || !b) return 0;
    if (a->sym[0] != b->sym[0] || a->sym[1] != b->sym[1]) return 0;
    /* check full sym if multi-byte */
    if (a->sym[0]) {
        for (int k = 2; k < TUIX_SYM_BYTES && (a->sym[k] || b->sym[k]); k++)
            if (a->sym[k] != b->sym[k]) return 0;
    }
    const TuixPixelStyles *sa = &a->styles;
    const TuixPixelStyles *sb = &b->styles;

    int mask_a = (sa->bold & 1) | ((sa->dim & 1) << 1) | ((sa->italic & 1) << 2) | ((sa->underlined & 1) << 3) | ((sa->custom_fg & 1) << 4) | ((sa->custom_bg & 1) << 5);
    int mask_b = (sb->bold & 1) | ((sb->dim & 1) << 1) | ((sb->italic & 1) << 2) | ((sb->underlined & 1) << 3) | ((sb->custom_fg & 1) << 4) | ((sb->custom_bg & 1) << 5);
    if (mask_a != mask_b) return 0;

    if (sa->custom_fg) {
        if (color_render_key(sa->fg) != color_render_key(sb->fg)) return 0;
    }
    if (sa->custom_bg) {
        if (color_render_key(sa->bg) != color_render_key(sb->bg)) return 0;
    }
    return 1;
}

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

    /* ANSI256 palette - skip 0-15, already handled above. */
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

    if (out_rgb) *out_rgb = *target;
    return 5;
}

/* Precomputed ANSI best-match LUT indexed by RGB565.
   65536 entries x 2 bytes = 128KB. Packed: bits[15:9]=mode, bits[8:0]=palette index. */
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
                TuixRGBTuple t = { (unsigned char)rv, (unsigned char)gv, (unsigned char)bv };
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

/* Stable key for the actual terminal color choice emitted for an RGB input.
   ANSI16/ANSI256 colors key by palette index; truecolor keys retain RGB565. */
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

static inline int itoa3(char *buf, int val) {
    if (val >= 10000) { buf[0] = '0' + val/10000; buf[1] = '0' + (val/1000)%10; buf[2] = '0' + (val/100)%10; buf[3] = '0' + (val/10)%10; buf[4] = '0' + val%10; return 5; }
    if (val >= 1000)  { buf[0] = '0' + val/1000; buf[1] = '0' + (val/100)%10; buf[2] = '0' + (val/10)%10; buf[3] = '0' + val%10; return 4; }
    if (val >= 100)   { buf[0] = '0' + val/100; buf[1] = '0' + (val/10)%10; buf[2] = '0' + val%10; return 3; }
    if (val >= 10)    { buf[0] = '0' + val/10; buf[1] = '0' + val%10; return 2; }
    buf[0] = '0' + val; return 1;
}

/* =========================================================================
   OPT 1+2+3: emit_cursor_move
   Chooses the shortest correct sequence to position the cursor at (tr, tc)
   given that the cursor is currently at (cr, cc), all 1-indexed.
   cr/cc = -1 means unknown position.

   Priority:
     a) Same row, move forward  → CUF  \e[NC  or \e[C      (3-4 bytes vs 6-9)
     b) Move to col 1 of next row → \r\n                    (2 bytes   vs 5-8)
     c) Absolute CUP with parameter elision:
          (1,1) → \e[H          (3 bytes)
          (Y,1) → \e[YH         (3-6 bytes, saves trailing ";1")
          (1,X) → \e[;XH        (4-6 bytes, saves leading "1")
          (Y,X) → \e[Y;XH       (full form)
   ========================================================================= */
static inline int emit_cursor_move(char *buf, int tr, int tc, int cr, int cc) {
    int n = 0;

    if (cr == tr && tc > cc && cc > 0) {
        /* OPT 1: CUF — cursor forward on the same row.
           \e[C  for delta=1 (3 bytes), \e[NC for delta>1 (4+ bytes).
           Replaces \e[Y;XH which is 6-9 bytes on a typical 80x24 terminal. */
        int delta = tc - cc;
        buf[n++] = '\x1b'; buf[n++] = '[';
        if (delta > 1) n += itoa3(buf + n, delta);
        buf[n++] = 'C';

    } else if (tc == 1 && cr >= 0 && tr == cr + 1) {
        /* OPT 2: \r\n for sequential row rendering to column 1.
           2 bytes vs \e[Y;1H which is 7+ bytes. */
        buf[n++] = '\r'; buf[n++] = '\n';

    } else {
        /* OPT 3: Absolute CUP with parameter elision.
           VT100/ECMA-48: missing Pl defaults to 1, missing Pc defaults to 1.
             \e[H   = (1,1)   saves 2 bytes vs \e[1;1H
             \e[YH  = (Y,1)   saves 2 bytes vs \e[Y;1H  ← very common: first col of each row
             \e[;XH = (1,X)   saves 1 byte  vs \e[1;XH  */
        buf[n++] = '\x1b'; buf[n++] = '[';
        if (tr == 1 && tc == 1) {
            /* \e[H — home, both params at default */
        } else if (tc == 1) {
            /* \e[YH — column 1, omit trailing ";1" */
            n += itoa3(buf + n, tr);
        } else if (tr == 1) {
            /* \e[;XH — row 1, omit leading "1" */
            buf[n++] = ';';
            n += itoa3(buf + n, tc);
        } else {
            n += itoa3(buf + n, tr);
            buf[n++] = ';';
            n += itoa3(buf + n, tc);
        }
        buf[n++] = 'H';
    }
    return n;
}

/* =========================================================================
   OPT 4: emit_sgr_diff — folds the mandatory reset into the first SGR params.

   Before: !have_style → write "\e[0m" (4 bytes) then a separate "\e[attr m".
   After:  !have_style → start buf with "0" as the first param so the two
           sequences collapse into one: "\e[0;attr m".
   Saves one full CSI header+terminator (ESC [ … m = 3 extra bytes) on every
   first-style-emit per frame or after a hard reset.
   ========================================================================= */
static inline void emit_sgr_diff(
    char *out, size_t *pos, size_t out_cap,
    const TuixPixelStyles *to,
    TuixPixelStyles *cur,
    int *have_style) {
    char buf[128];
    int n = 0;
    int need = 0;

    buf[0] = '\x1b'; buf[1] = '['; n = 2;

    /* OPT 4: instead of a separate \e[0m, inject "0" as the first SGR param.
       Subsequent params append with ";", giving \e[0;bold;fg m in one shot. */
    if (!*have_style) {
        buf[n++] = '0';
        need = 1;
        *cur = (TuixPixelStyles){0};
        *have_style = 1;
    }

    if (to->bold != cur->bold || to->dim != cur->dim) {
        if (!to->bold && !to->dim) {
            if (need) buf[n++] = ';';
            buf[n++] = '2'; buf[n++] = '2';
            need = 1;
        } else {
            if (to->bold) {
                if (need) buf[n++] = ';';
                buf[n++] = '1'; need = 1;
            }
            if (to->dim) {
                if (need) buf[n++] = ';';
                buf[n++] = '2'; need = 1;
            }
        }
    }

    if (to->italic != cur->italic) {
        if (need) buf[n++] = ';';
        if (to->italic) buf[n++] = '3';
        else { buf[n++] = '2'; buf[n++] = '3'; }
        need = 1;
    }

    if (to->underlined != cur->underlined) {
        if (need) buf[n++] = ';';
        if (to->underlined) buf[n++] = '4';
        else { buf[n++] = '2'; buf[n++] = '4'; }
        need = 1;
    }

    TuixRGBTuple to_fg_q = to->fg;
    TuixRGBTuple to_bg_q = to->bg;
    TuixRGBTuple cur_fg_q = cur->fg;
    TuixRGBTuple cur_bg_q = cur->bg;

    if (to->custom_fg) {
        if (!cur->custom_fg ||
            to_fg_q.r != cur_fg_q.r ||
            to_fg_q.g != cur_fg_q.g ||
            to_fg_q.b != cur_fg_q.b)
        {
            int idx = 0;
            int mode = ansi_best_match(&to_fg_q, &idx);
            if (mode == 0) {
                int code = (idx < 8) ? (30 + idx) : (90 + idx - 8);
                if (need) buf[n++] = ';';
                n += itoa3(buf + n, code);
            } else if (mode == 1) {
                if (need) buf[n++] = ';';
                memcpy(buf + n, "38;5;", 5); n += 5;
                n += itoa3(buf + n, idx);
            } else {
                if (need) buf[n++] = ';';
                memcpy(buf + n, "38;2;", 5); n += 5;
                n += itoa3(buf + n, to_fg_q.r); buf[n++] = ';';
                n += itoa3(buf + n, to_fg_q.g); buf[n++] = ';';
                n += itoa3(buf + n, to_fg_q.b);
            }
            need = 1;
        }
    } else if (cur->custom_fg) {
        if (need) buf[n++] = ';';
        buf[n++] = '3'; buf[n++] = '9';
        need = 1;
    }

    if (to->custom_bg) {
        if (!cur->custom_bg ||
            to_bg_q.r != cur_bg_q.r ||
            to_bg_q.g != cur_bg_q.g ||
            to_bg_q.b != cur_bg_q.b)
        {
            int idx = 0;
            int mode = ansi_best_match(&to_bg_q, &idx);
            if (mode == 0) {
                int code = (idx < 8) ? (40 + idx) : (100 + idx - 8);
                if (need) buf[n++] = ';';
                n += itoa3(buf + n, code);
            } else if (mode == 1) {
                if (need) buf[n++] = ';';
                memcpy(buf + n, "48;5;", 5); n += 5;
                n += itoa3(buf + n, idx);
            } else {
                if (need) buf[n++] = ';';
                memcpy(buf + n, "48;2;", 5); n += 5;
                n += itoa3(buf + n, to_bg_q.r); buf[n++] = ';';
                n += itoa3(buf + n, to_bg_q.g); buf[n++] = ';';
                n += itoa3(buf + n, to_bg_q.b);
            }
            need = 1;
        }
    } else if (cur->custom_bg) {
        if (need) buf[n++] = ';';
        buf[n++] = '4'; buf[n++] = '9';
        need = 1;
    }

    if (need) {
        buf[n++] = 'm';
        memcpy(out + *pos, buf, n);
        *pos += n;
        *cur = *to;
        cur->fg = to_fg_q;
        cur->bg = to_bg_q;
    }
}

void tuix_render_streaming(
    TuixFinalBuffer *buffer,
    TuixRowDoneCallback on_row_done,
    void *user_data) {
    if (!buffer) return;

    /* Reset per-frame byte counter and honor any manual dirty request */
    last_frame_bytes = 0;
    if (manual_dirty_all) { buffer->full_redraw = 1; manual_dirty_all = 0; }

    /* Snapshot pixel pointer to avoid races if buffer->pixels is changed
       or reallocated concurrently during a resize. Use local `pixels`
       for all reads in this render invocation. */
    TuixPixel *pixels = buffer->pixels;
    if (!pixels) return;

    int W = buffer->width;
    int H = buffer->height;
    if (W <= 0 || H <= 0) return;

    /* Allocate/reallocate prev_pixels as a single contiguous block */
    if (!prev_pixels || prev_w != W || prev_h != H) {
        free(prev_pixels);
        prev_pixels = calloc((size_t)H * (size_t)W, sizeof(TuixPixel));
        if (!prev_pixels) return;

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

    /* Allocate hash arrays */
    if (hash_cap < H) {
        free(prev_hash); free(curr_hash);
        prev_hash = calloc(H, sizeof(unsigned long long));
        curr_hash = malloc(sizeof(unsigned long long) * H);
        hash_cap = H;
        /* prev_hash zeroed - force full_redraw so hashes synchronize */
        buffer->full_redraw = 1;
    }

    if (!buffer->full_redraw) {
        for (int y = 0; y < H; y++)
            curr_hash[y] = row_hash(&pixels[(size_t)y * W], W);
    }

    /* Quantize colors only for rows that actually changed. */
    for (int y = 0; y < H; y++) {
        if (!buffer->full_redraw && prev_hash[y] == curr_hash[y])
            continue;
        TuixPixel *row = &pixels[(size_t)y * W];
        for (int x = 0; x < W; x++) {
            TuixPixel *p = &row[x];
            p->q_fg = tuix_rgb16(p->styles.fg);
            p->q_bg = tuix_rgb16(p->styles.bg);
            p->q_cached = 1;
        }
    }

    static int g_is_tty = -1;
    if (g_is_tty < 0) g_is_tty = is_stdout_tty();
    if (g_is_tty) { memcpy(out + pos, "\x1b[?25l", 6); pos += 6; }

    TuixPixelStyles cur_style = (TuixPixelStyles){0};
    int have_style = 0;
    int cursor_row = -1, cursor_col = -1;  /* 1-indexed terminal cursor */

    for (int y = 0; y < H; y++) {

        if (!buffer->full_redraw && prev_hash[y] == curr_hash[y]) {
            if (on_row_done) on_row_done(buffer, y, user_data);
            continue;
        }

        int x = 0;
        while (x < W) {
            if (!buffer->full_redraw) {
                while (x < W && pixels_equal(&pixels[(size_t)y * W + x], &prev_pixels[(size_t)y * W + x]))
                    x++;
                if (x >= W) break;
            }

            TuixPixelStyles *s = &pixels[(size_t)y * W + x].styles;
            TuixPixelStyles quant_s = *s;
            unsigned int fg_key = s->custom_fg ? color_render_key(s->fg) : 0;
            unsigned int bg_key = s->custom_bg ? color_render_key(s->bg) : 0;
            if (s->custom_fg) quant_s.fg = pixels[(size_t)y * W + x].q_fg;
            if (s->custom_bg) quant_s.bg = pixels[(size_t)y * W + x].q_bg;

            int run_start = x;
            int run_end = x + 1;

            /* Group consecutive pixels with the same style into one SGR sequence. */
            for (; run_end < W; run_end++) {
                if (!buffer->full_redraw &&
                    pixels_equal(&pixels[(size_t)y * W + run_end], &prev_pixels[(size_t)y * W + run_end]))
                    break;

                TuixPixelStyles *s2 = &pixels[(size_t)y * W + run_end].styles;
                int mask1 = (s->bold & 1) | ((s->dim & 1) << 1) | ((s->italic & 1) << 2) | ((s->underlined & 1) << 3) | ((s->custom_fg & 1) << 4) | ((s->custom_bg & 1) << 5);
                int mask2 = (s2->bold & 1) | ((s2->dim & 1) << 1) | ((s2->italic & 1) << 2) | ((s2->underlined & 1) << 3) | ((s2->custom_fg & 1) << 4) | ((s2->custom_bg & 1) << 5);
                if (mask1 != mask2) break;

                if (s->custom_fg) {
                    if (color_render_key(s2->fg) != fg_key) break;
                }
                if (s->custom_bg) {
                    if (color_render_key(s2->bg) != bg_key) break;
                }
            }

            /* OPT 1+2+3: emit shortest cursor positioning sequence */
            {
                int target_row = y + 1;
                int target_col = run_start + 1;
                if (cursor_row != target_row || cursor_col != target_col) {
                    int esc_len = emit_cursor_move(esc, target_row, target_col, cursor_row, cursor_col);
                    memcpy(out + pos, esc, esc_len);
                    pos += esc_len;
                }
            }

            /* OPT 4: SGR diff with folded reset (no separate \e[0m) */
            emit_sgr_diff(out, &pos, out_cap, &quant_s, &cur_style, &have_style);

            /* OPT 5: REP (Repeat Preceding Graphic Character, ECMA-48 §8.3.103)
               Within a style-uniform run, detect consecutive identical sym
               values and replace them with a single char + \e[Nb.

               Break-even analysis (sym length S, repeat count R):
                 Literal:  S * R bytes
                 REP:      S + 2 + digits(R-1) bytes   ← ESC [ (R-1) b
               REP wins when R >= 2 and (2 + digits(R-1)) < S*(R-1).
               For S=1 (ASCII/Latin): wins at R >= 4  (3 < 3 false, 3 < 4 true at R=5).
                 Actually: S=1, R=4: literal=4, REP=1+2+1=4 → tie. R=5: 5 vs 4 → win.
               For S=3 (common 3-byte UTF-8): wins at R=2 already (3 vs 1+2+1=4, no)
                 R=2: literal=6, REP=3+2+1=6 → tie. R=3: 9 vs 3+2+1=6 → win.

               Support: xterm, VTE, kitty, Alacritty, Windows Terminal — all major
               terminals. REP is part of VT102 / ECMA-48. */
            for (int i = run_start; i < run_end; ) {
                const char *utf = pixels[(size_t)y * W + i].sym;

                /* Measure symbol byte length */
                int slen = 0;
                while (slen < TUIX_SYM_BYTES && utf[slen]) slen++;
                if (!slen) { i++; continue; }

                /* Count consecutive identical symbols (full fixed buffer compare) */
                int rep = 1;
                while (i + rep < run_end &&
                       memcmp(utf, pixels[(size_t)y * W + i + rep].sym, TUIX_SYM_BYTES) == 0)
                    rep++;

                /* Emit character once */
                memcpy(out + pos, utf, slen);
                pos += slen;

                if (rep > 1) {
                    /* REP \e[(rep-1)b: 3 + digits(rep-1) bytes */
                    int rn = rep - 1;
                    int rdigits = (rn >= 10000) ? 5 : (rn >= 1000) ? 4 :
                                  (rn >= 100)   ? 3 : (rn >= 10)   ? 2 : 1;
                    if (3 + rdigits < slen * rn) {
                        out[pos++] = '\x1b';
                        out[pos++] = '[';
                        pos += itoa3(out + pos, rn);
                        out[pos++] = 'b';
                        i += rep;
                        if (pos >= out_cap - 1024) flush_chunk(out, &pos);
                        continue;
                    }
                }

                i++;
                if (pos >= out_cap - 1024) flush_chunk(out, &pos);
            }

            x = run_end;
            cursor_row = y + 1;
            cursor_col = run_end + 1;
        }

        if (on_row_done) on_row_done(buffer, y, user_data);
    }

    if (g_is_tty) { memcpy(out + pos, "\x1b[?25h", 6); pos += 6; }
    memcpy(out + pos, "\x1b[0m", 4); pos += 4;
    flush_chunk(out, &pos);
    fflush(stdout);

    if (buffer->full_redraw) {
        memcpy(prev_pixels, pixels, sizeof(TuixPixel) * (size_t)W * (size_t)H);
        for (int y = 0; y < H; y++) {
            prev_hash[y] = row_hash(&pixels[(size_t)y * W], W);
        }
    } else {
        for (int y = 0; y < H; y++) {
            if (prev_hash[y] != curr_hash[y]) {
                memcpy(&prev_pixels[(size_t)y * W], &pixels[(size_t)y * W], sizeof(TuixPixel) * W);
                prev_hash[y] = curr_hash[y];
            }
        }
    }

    if (buffer->full_redraw) buffer->full_redraw = 0;
}

void tuix_render_mark_rows_dirty(int y0, int y1) {
    if (y0 < 0) y0 = 0;
    if (y1 <= y0) return;
    if (!prev_hash || hash_cap == 0) {
        /* No hash arrays yet; request a full redraw on next render. */
        manual_dirty_all = 1;
        return;
    }
    if (y0 >= hash_cap) return;
    if (y1 > hash_cap) y1 = hash_cap;
    for (int y = y0; y < y1; y++) prev_hash[y] ^= 0xFFFFFFFFFFFFFFFFULL;
}

size_t tuix_render_get_last_frame_bytes(void) {
    return last_frame_bytes;
}
