// Simple command-buffer executor for tuix
#ifndef TUIX_BATCH_EXECUTOR_H
#define TUIX_BATCH_EXECUTOR_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Execute a compact command buffer. Returns number of commands processed or -1 on error.
int tuix_execute_command_buffer(const unsigned char* buf, size_t len);

// Return a monotonic timestamp in microseconds.
uint64_t tuix_time_now_us();

#ifdef __cplusplus
}
#endif

#endif // TUIX_BATCH_EXECUTOR_H
