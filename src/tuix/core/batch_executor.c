#include "batch_executor.h"
#include "buffers.h"
#include "object_manager.h"
#include "content_builder/builders/progressbar_builder.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <time.h>
#endif

// Simple command buffer format (stream of records):
// [cmd:1][payload_len:4 le][payload bytes]
// cmd values:
// 1 = init_scene: payload = null-terminated scene name
// 2 = free_scene: payload = null-terminated scene name
// 3 = clear_scene: payload = null-terminated scene name
// 10 = create_object: payload = builder_name\0scene_name\0(float w)(float h)(float mt)(float ml) (little-endian floats)
// 20 = progress_value: payload = uint32_t obj_ptr (native pointer-sized stored as 32-bit) + float value
// 21 = progress_style: payload = uint32_t obj_ptr + char fill + char empty + 6 bytes RGB values

int tuix_execute_command_buffer(const unsigned char* buf, size_t len) {
    size_t off = 0;
    int processed = 0;
    uint64_t t_start = 0;
    uint64_t t_end = 0;
    uint64_t time_case255 = 0;
    uint64_t time_case10 = 0;
    uint64_t time_other = 0;
    uint64_t t_before = 0;

    /* string table pointer array (optional, set by cmd 255) */
    static char **str_table = NULL;
    static size_t str_count = 0;
    /* Single contiguous arena holding all strings for the current table. */
    static char *str_arena = NULL;

    while (off + 5 <= len) {
        unsigned char cmd = buf[off];
        uint32_t payload_len = 0;
        memcpy(&payload_len, buf + off + 1, 4);
        off += 5;
        if (off + payload_len > len) return -1;
        const unsigned char* p = buf + off;

        if (t_start == 0) t_start = tuix_time_now_us();
        t_before = tuix_time_now_us();

        switch (cmd) {
            case 255: {
                /* Free previous table and arena */
                if (str_table) {
                    if (str_arena) {
                        free(str_arena);
                        str_arena = NULL;
                    }
                    free(str_table);
                    str_table = NULL;
                    str_count = 0;
                }
                /* count strings */
                size_t cnt = 0;
                for (size_t i = 0; i < payload_len; ++i) if (p[i] == 0) cnt++;
                if (cnt == 0) break;
                str_table = (char**) malloc(cnt * sizeof(char*));
                if (!str_table) break;
                char* arena = (char*) malloc(payload_len + 1);
                if (!arena) {
                    free(str_table);
                    str_table = NULL;
                    break;
                }
                memcpy(arena, p, payload_len);
                arena[payload_len] = '\0';
                size_t idx = 0; size_t pos = 0;
                while (pos < payload_len && idx < cnt) {
                    str_table[idx++] = arena + pos;
                    while (pos < payload_len && arena[pos] != '\0') pos++;
                    pos++;
                }
                str_arena = arena;
                str_count = idx;
                time_case255 += tuix_time_now_us() - t_before;
                break;
            }
            case 1: {
                if (payload_len >= 4) {
                    uint32_t idx = 0; memcpy(&idx, p, 4);
                    if (idx < str_count) tuix_init_scene((const char*) str_table[idx]);
                }
                break;
            }
            case 2: {
                if (payload_len >= 4) {
                    uint32_t idx = 0; memcpy(&idx, p, 4);
                    if (idx < str_count) tuix_free_scene((const char*) str_table[idx]);
                }
                break;
            }
            case 3: {
                if (payload_len >= 4) {
                    uint32_t idx = 0; memcpy(&idx, p, 4);
                    if (idx < str_count) tuix_clear_scene((const char*) str_table[idx]);
                }
                break;
            }
            case 10: {
                if (payload_len >= 8 + 16) {
                    uint32_t bidx = 0, sidx = 0;
                    memcpy(&bidx, p, 4);
                    memcpy(&sidx, p + 4, 4);
                    if (bidx < str_count && sidx < str_count) {
                        float w, h, mt, ml;
                        memcpy(&w, p + 8, 4);
                        memcpy(&h, p + 12, 4);
                        memcpy(&mt, p + 16, 4);
                        memcpy(&ml, p + 20, 4);
                        tuix_create_object(str_table[bidx], str_table[sidx], w, h, mt, ml);
                    }
                }
                time_case10 += tuix_time_now_us() - t_before;
                break;
            }
            case 20: {
                if (payload_len >= 8) {
                    uint32_t addr32 = 0; float v = 0.0f;
                    memcpy(&addr32, p, 4);
                    memcpy(&v, p+4, 4);
                    tuix_progressbar_set_value((void*)(uintptr_t)addr32, v);
                }
                break;
            }
            case 21: {
                if (payload_len >= 12) {
                    uint32_t addr32=0; char fill=0; char empty=0; unsigned char fr=0,fg=0,fb=0,er=0,eg=0,eb=0;
                    memcpy(&addr32, p, 4);
                    fill = *(char*)(p+4);
                    empty = *(char*)(p+5);
                    fr = *(unsigned char*)(p+6);
                    fg = *(unsigned char*)(p+7);
                    fb = *(unsigned char*)(p+8);
                    er = *(unsigned char*)(p+9);
                    eg = *(unsigned char*)(p+10);
                    eb = *(unsigned char*)(p+11);
                    tuix_progressbar_set_style((void*)(uintptr_t)addr32, fill, empty, fr, fg, fb, er, eg, eb);
                }
                break;
            }
            default:
                time_other += tuix_time_now_us() - t_before;
                break;
        }
        off += payload_len;
        processed += 1;
    }
    if (t_start != 0) {
        t_end = tuix_time_now_us();
        uint64_t total = t_end - t_start;
        fprintf(stderr, "[tuix_exec_profile] processed=%d total_us=%llu case255_us=%llu case10_us=%llu other_us=%llu\n", processed, (unsigned long long)total, (unsigned long long)time_case255, (unsigned long long)time_case10, (unsigned long long)time_other);
    }
    return processed;
}

uint64_t tuix_time_now_us() {
#ifdef _WIN32
    static LARGE_INTEGER freq = {0};
    LARGE_INTEGER now;
    if (freq.QuadPart == 0) QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&now);
    return (uint64_t)((now.QuadPart * 1000000) / freq.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (ts.tv_nsec / 1000ULL);
#endif
}
