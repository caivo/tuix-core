/* input.c - Unified single-thread input module
 * One thread reads all input. On Windows uses native ReadConsoleInput
 * (no VT input mode - avoids mouse-as-keyboard-chars bug).
 * On POSIX uses raw mode + SGR mouse parsing.
 *
 * Supports: mouse1-3, x1/x2 buttons, press, release, hover, drag,
 *           double-click, vertical scroll, horizontal scroll.
 */

#include "input.h"
#include "../types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>
#endif

int tuix_input_keyboard_enabled = 1;
int tuix_input_mouse_enabled    = 1;

#define QUEUE_CAP 128

static TuixKeyboardKey kb_queue[QUEUE_CAP];
static int kb_head = 0, kb_tail = 0;

static TuixMouseKey mi_queue[QUEUE_CAP];
static int mi_head = 0, mi_tail = 0;

static volatile int input_running = 0;

#ifdef _WIN32
static HANDLE          input_thread = NULL;
static CRITICAL_SECTION input_cs;
static int             cs_initialized = 0;
#define LOCK()   EnterCriticalSection(&input_cs)
#define UNLOCK() LeaveCriticalSection(&input_cs)
#else
static pthread_t       input_thread;
static pthread_mutex_t input_mutex = PTHREAD_MUTEX_INITIALIZER;
#define LOCK()   pthread_mutex_lock(&input_mutex)
#define UNLOCK() pthread_mutex_unlock(&input_mutex)
#endif

static TuixMouseKey    snap_mouse    = {0};
static TuixKeyboardKey snap_keyboard = {0};
static TuixInputSnapshot snap = { .mouse = &snap_mouse, .keyboard = &snap_keyboard };

static double now_seconds(void) {
    return (double)clock() / (double)CLOCKS_PER_SEC;
}

static void push_keyboard(int code) {
    TuixKeyboardKey ev;
    ev.btn       = code;
    ev.code      = code;
    ev.scancode  = -1;
    ev.modifiers = 0;
    ev.pressed   = 1;
    ev.repeat    = 0;
    ev.timestamp = now_seconds();
    ev.has_event = 1;
    memset(ev.text, 0, sizeof(ev.text));
    if (isprint(code) && code < 128) {
        ev.text[0] = (char)code;
        ev.text[1] = '\0';
    }
    int next = (kb_head + 1) % QUEUE_CAP;
    if (next != kb_tail) {
        kb_queue[kb_head] = ev;
        kb_head = next;
    }
}

static void push_mouse_ev(int event, int btn, int buttons_held,
                           int col, int row) {
    TuixMouseKey ev;
    ev.event        = event;
    ev.btn          = btn;
    ev.buttons_held = buttons_held;
    ev.col          = col;
    ev.row          = row;
    ev.timestamp    = now_seconds();
    ev.has_event    = 1;
    int next = (mi_head + 1) % QUEUE_CAP;
    if (next != mi_tail) {
        mi_queue[mi_head] = ev;
        mi_head = next;
    }
}

#ifdef _WIN32

/* Previous button state for detecting press/release edges */
static DWORD prev_btn_state = 0;

/* Map Win32 button flag to TUIX_BTN_* */
static int win32_btn_id(DWORD flag) {
    if (flag & FROM_LEFT_1ST_BUTTON_PRESSED)  return TUIX_BTN_LEFT;
    if (flag & FROM_LEFT_2ND_BUTTON_PRESSED)  return TUIX_BTN_MIDDLE;
    if (flag & RIGHTMOST_BUTTON_PRESSED)      return TUIX_BTN_RIGHT;
    if (flag & 0x0008)                        return TUIX_BTN_X1;  /* FROM_LEFT_3RD */
    if (flag & 0x0010)                        return TUIX_BTN_X2;  /* FROM_LEFT_4TH */
    return TUIX_BTN_LEFT;
}

/* Convert Win32 dwButtonState to our bitmask */
static int win32_buttons_mask(DWORD s) {
    int m = 0;
    if (s & FROM_LEFT_1ST_BUTTON_PRESSED)  m |= (1 << TUIX_BTN_LEFT);
    if (s & FROM_LEFT_2ND_BUTTON_PRESSED)  m |= (1 << TUIX_BTN_MIDDLE);
    if (s & RIGHTMOST_BUTTON_PRESSED)      m |= (1 << TUIX_BTN_RIGHT);
    if (s & 0x0008)                        m |= (1 << TUIX_BTN_X1);
    if (s & 0x0010)                        m |= (1 << TUIX_BTN_X2);
    return m;
}

static DWORD WINAPI input_thread_fn(LPVOID arg) {
    (void)arg;

    HANDLE hIn  = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    /* Enable VT processing on stdout only (for rendering output) */
    DWORD out_mode;
    if (GetConsoleMode(hOut, &out_mode)) {
        out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, out_mode);
    }

    /* Configure stdin: native events, NO virtual terminal input */
    DWORD orig_in_mode;
    GetConsoleMode(hIn, &orig_in_mode);
    DWORD in_mode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
    if (tuix_input_mouse_enabled) {
        in_mode |= ENABLE_MOUSE_INPUT;
    }
    /* No ENABLE_VIRTUAL_TERMINAL_INPUT - we want raw KEY_EVENT +
       MOUSE_EVENT records, not VT escape sequences */
    SetConsoleMode(hIn, in_mode);

    prev_btn_state = 0;
    INPUT_RECORD rec[64];
    DWORD n_read;

    while (input_running) {
        DWORD wait = WaitForSingleObject(hIn, 10);
        if (wait != WAIT_OBJECT_0) continue;
        if (!ReadConsoleInputW(hIn, rec, 64, &n_read)) continue;

        for (DWORD i = 0; i < n_read; i++) {
            if (rec[i].EventType == KEY_EVENT && tuix_input_keyboard_enabled) {
                KEY_EVENT_RECORD *kr = &rec[i].Event.KeyEvent;
                if (!kr->bKeyDown) continue;
                int code = kr->uChar.UnicodeChar;
                if (code == 0) code = kr->wVirtualKeyCode;
                if (code > 0) {
                    LOCK();
                    push_keyboard(code);
                    UNLOCK();
                }
            }
            else if (rec[i].EventType == MOUSE_EVENT && tuix_input_mouse_enabled) {
                MOUSE_EVENT_RECORD *mr = &rec[i].Event.MouseEvent;
                int col = mr->dwMousePosition.X + 1;
                int row = mr->dwMousePosition.Y + 1;
                DWORD bs = mr->dwButtonState;
                int mask = win32_buttons_mask(bs);

                switch (mr->dwEventFlags) {
                case 0: {
                    /* Button state changed - detect press & release edges */
                    DWORD pressed  = bs & ~prev_btn_state;   /* newly pressed  */
                    DWORD released = prev_btn_state & ~bs;    /* newly released */
                    prev_btn_state = bs;

                    /* Emit press events */
                    DWORD check_flags[] = {
                        FROM_LEFT_1ST_BUTTON_PRESSED,
                        FROM_LEFT_2ND_BUTTON_PRESSED,
                        RIGHTMOST_BUTTON_PRESSED,
                        0x0008, 0x0010
                    };
                    LOCK();
                    for (int b = 0; b < 5; b++) {
                        if (pressed & check_flags[b])
                            push_mouse_ev(TUIX_MOUSE_PRESS, win32_btn_id(check_flags[b]), mask, col, row);
                    }
                    for (int b = 0; b < 5; b++) {
                        if (released & check_flags[b])
                            push_mouse_ev(TUIX_MOUSE_RELEASE, win32_btn_id(check_flags[b]), mask, col, row);
                    }
                    UNLOCK();
                    break;
                }
                case MOUSE_MOVED: {
                    int ev = (bs == 0) ? TUIX_MOUSE_HOVER : TUIX_MOUSE_DRAG;
                    int btn = TUIX_BTN_LEFT;
                    if (bs & RIGHTMOST_BUTTON_PRESSED)     btn = TUIX_BTN_RIGHT;
                    if (bs & FROM_LEFT_2ND_BUTTON_PRESSED) btn = TUIX_BTN_MIDDLE;
                    LOCK();
                    push_mouse_ev(ev, btn, mask, col, row);
                    UNLOCK();
                    break;
                }
                case DOUBLE_CLICK: {
                    int btn = win32_btn_id(bs);
                    LOCK();
                    push_mouse_ev(TUIX_MOUSE_DOUBLE_CLICK, btn, mask, col, row);
                    UNLOCK();
                    break;
                }
                case MOUSE_WHEELED: {
                    short delta = (short)HIWORD(bs);
                    int ev = (delta > 0) ? TUIX_MOUSE_SCROLL_UP : TUIX_MOUSE_SCROLL_DOWN;
                    LOCK();
                    push_mouse_ev(ev, 0, mask, col, row);
                    UNLOCK();
                    break;
                }
                case MOUSE_HWHEELED: {
                    short delta = (short)HIWORD(bs);
                    int ev = (delta > 0) ? TUIX_MOUSE_HSCROLL_RIGHT : TUIX_MOUSE_HSCROLL_LEFT;
                    LOCK();
                    push_mouse_ev(ev, 0, mask, col, row);
                    UNLOCK();
                    break;
                }
                default: break;
                }
            }
        }
    }

    SetConsoleMode(hIn, orig_in_mode);
    return 0;
}

#else

static struct termios orig_termios;
static int raw_enabled = 0;

static void disable_raw_mode(void) {
    if (!raw_enabled) return;
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    raw_enabled = 0;
}

static void enable_raw_mode(void) {
    if (raw_enabled) return;
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_cc[VMIN]  = 0;   /* non-blocking: return immediately */
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    raw_enabled = 1;
}

static int kbhit_nonblock(void) {
    int n = 0;
    ioctl(STDIN_FILENO, FIONREAD, &n);
    return n;
}

static int getch_nonblock(void) {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) return (int)(unsigned char)c;
    return -1;
}

static void sleep_ms(int ms) {
    usleep(ms * 1000);
}

/* Returns 1 if the buffer starts with ESC and looks like a partially
 * received escape sequence (CSI, SGR mouse, SS3, etc.) that needs
 * more bytes before we can dispatch or discard it.
 * Guards against infinite accumulation with a 64-byte cap. */
static int is_incomplete_escape(const char *buf, int len) {
    if (len < 1 || buf[0] != '\x1b') return 0;
    if (len >= 64) return 0;           /* safety cap - flush oversized seqs */
    if (len == 1) return 1;            /* bare ESC - could be start of anything */

    if (buf[1] == '[') {
        /* CSI sequence: ESC [ <params> <final byte 0x40-0x7E> */
        for (int i = 2; i < len; i++) {
            unsigned char ch = (unsigned char)buf[i];
            if (ch >= 0x40 && ch <= 0x7E)
                return 0;              /* found final byte - sequence complete */
        }
        return 1;                      /* still accumulating intermediate/param bytes */
    }

    if (buf[1] == 'O') {
        /* SS3 sequence: ESC O <letter> - needs exactly 3 bytes */
        return len < 3;
    }

    /* Other ESC+char sequences are 2 bytes; already have them */
    return 0;
}

/* SGR mouse: ESC [ < Btn ; Col ; Row M/m
 * Btn encoding: low 2 bits = button (0=left,1=mid,2=right,3=release/move)
 *               bit 5 (32) = motion
 *               bit 6 (64) = scroll
 * Returns bytes consumed, or 0 if not a complete sequence. */
static int try_parse_mouse(const char *buf, int len) {
    if (len < 6) return 0;
    if (buf[0] != '\x1b' || buf[1] != '[' || buf[2] != '<') return 0;

    int raw_btn = 0, col = 0, row = 0;
    char typ = 0;
    int n = 0;
    if (sscanf(buf + 3, "%d;%d;%d%c%n", &raw_btn, &col, &row, &typ, &n) != 4 || n == 0)
        return 0;

    int is_motion = (raw_btn & 32) != 0;
    int is_scroll = (raw_btn & 64) != 0;
    int btn_bits  = raw_btn & 3;

    int event = TUIX_MOUSE_NONE;
    int btn   = TUIX_BTN_LEFT;
    int mask  = 0;

    if (is_scroll) {
        event = (btn_bits == 0) ? TUIX_MOUSE_SCROLL_UP : TUIX_MOUSE_SCROLL_DOWN;
    } else if (is_motion) {
        if (btn_bits == 3) {
            event = TUIX_MOUSE_HOVER;   /* no button held */
        } else {
            event = TUIX_MOUSE_DRAG;
            btn = btn_bits; /* 0=left, 1=mid, 2=right */
            mask = 1 << btn;
        }
    } else if (typ == 'M') {
        event = TUIX_MOUSE_PRESS;
        btn = btn_bits;
        mask = 1 << btn;
    } else if (typ == 'm') {
        event = TUIX_MOUSE_RELEASE;
        btn = btn_bits;
    }

    LOCK();
    push_mouse_ev(event, btn, mask, col, row);
    UNLOCK();
    return 3 + n;
}

static void *input_thread_fn(void *arg) {
    (void)arg;
    enable_raw_mode();

    if (tuix_input_mouse_enabled) {
        /* ?1003h = all-motion tracking, ?1006h = SGR extended format */
        printf("\x1b[?1003h\x1b[?1006h"); fflush(stdout);
    }

    char buf[128];
    int  buf_len = 0;

    while (input_running) {
        while (kbhit_nonblock()) {
            int c = getch_nonblock();
            if (c == -1) break;

            if (buf_len < (int)sizeof(buf) - 1)
                buf[buf_len++] = (char)c;
            buf[buf_len] = '\0';

            if (tuix_input_mouse_enabled) {
                int consumed = try_parse_mouse(buf, buf_len);
                if (consumed > 0) {
                    int remaining = buf_len - consumed;
                    if (remaining > 0)
                        memmove(buf, buf + consumed, remaining);
                    buf_len = remaining;
                    buf[buf_len] = '\0';
                    continue;
                }
            }

            /* If buffer looks like a partial escape sequence, keep
             * accumulating - don't flush yet. */
            if (is_incomplete_escape(buf, buf_len))
                continue;

            /* Flush as keyboard events */
            if (tuix_input_keyboard_enabled) {
                LOCK();
                for (int i = 0; i < buf_len; i++)
                    push_keyboard((unsigned char)buf[i]);
                UNLOCK();
            }
            buf_len = 0;
        }

        /* Flush stale buffer - but give incomplete escape sequences
         * a grace period so inter-byte gaps don't cause premature
         * flushing of mouse / function-key sequences. */
        if (buf_len > 0 && !kbhit_nonblock()) {
            if (is_incomplete_escape(buf, buf_len)) {
                /* Wait up to ~8ms for more bytes (escape sequences
                 * arrive within a few hundred µs on a local terminal;
                 * 8ms covers slow SSH links too). */
                for (int w = 0; w < 4; w++) {
                    sleep_ms(2);
                    if (kbhit_nonblock()) break;
                }
                if (kbhit_nonblock()) continue; /* more data - keep parsing */
            }
            /* Not an escape, or timed out - flush as keyboard */
            if (tuix_input_keyboard_enabled) {
                LOCK();
                for (int i = 0; i < buf_len; i++)
                    push_keyboard((unsigned char)buf[i]);
                UNLOCK();
            }
            buf_len = 0;
        }

        sleep_ms(1);
    }

    if (tuix_input_mouse_enabled) {
        printf("\x1b[?1003l\x1b[?1006l"); fflush(stdout);
    }
    disable_raw_mode();
    return NULL;
}
#endif


void listen_input(void) {
    if (input_running) return;

#ifdef _WIN32
    if (!cs_initialized) { InitializeCriticalSection(&input_cs); cs_initialized = 1; }
#endif

    kb_head = kb_tail = 0;
    mi_head = mi_tail = 0;
    input_running = 1;

#ifdef _WIN32
    input_thread = CreateThread(NULL, 0, input_thread_fn, NULL, 0, NULL);
#else
    pthread_create(&input_thread, NULL, input_thread_fn, NULL);
#endif
}

void stop_input_listening(void) {
    if (!input_running) return;
    input_running = 0;
#ifdef _WIN32
    if (input_thread) {
        WaitForSingleObject(input_thread, INFINITE);
        CloseHandle(input_thread);
        input_thread = NULL;
    }
#else
    pthread_join(input_thread, NULL);
#endif
}

TuixInputSnapshot get_input_snapshot(void) {
    LOCK();
    if (kb_tail != kb_head) {
        snap_keyboard = kb_queue[kb_tail];
        kb_tail = (kb_tail + 1) % QUEUE_CAP;
    } else {
        snap_keyboard.has_event = 0;
    }
    if (mi_tail != mi_head) {
        snap_mouse = mi_queue[mi_tail];
        mi_tail = (mi_tail + 1) % QUEUE_CAP;
    } else {
        snap_mouse.has_event = 0;
    }
    UNLOCK();
    return snap;
}
