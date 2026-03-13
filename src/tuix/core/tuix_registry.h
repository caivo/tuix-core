#ifndef TUIX_tuix_registry_H
#define TUIX_tuix_registry_H

#include "types.h"

/* Single shared registry instance used by all modules. */
extern TuixRegistry tuix_registry;

/* Simple cross-platform lock for protecting registry/buffer mutations. */
#ifdef _WIN32
#include <windows.h>
extern CRITICAL_SECTION tuix_registry_lock;
#define tuix_lock() EnterCriticalSection(&tuix_registry_lock)
#define tuix_unlock() LeaveCriticalSection(&tuix_registry_lock)
#else
#include <pthread.h>
extern pthread_mutex_t tuix_registry_lock;
#define tuix_lock() pthread_mutex_lock(&tuix_registry_lock)
#define tuix_unlock() pthread_mutex_unlock(&tuix_registry_lock)
#endif

#endif