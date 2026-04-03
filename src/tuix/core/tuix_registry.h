#ifndef TUIX_tuix_registry_H
#define TUIX_tuix_registry_H

#include "types.h"

#if defined(_WIN32)
#if defined(TUIX_BUILDING_CORE)
#define TUIX_REGISTRY_API __declspec(dllexport)
#else
#define TUIX_REGISTRY_API __declspec(dllimport)
#endif
#else
#define TUIX_REGISTRY_API
#endif

/* Single shared registry instance used by all modules. */
extern TUIX_REGISTRY_API TuixRegistry tuix_registry;

/* Simple cross-platform lock for protecting registry/buffer mutations. */
#ifdef _WIN32
#include <windows.h>
extern TUIX_REGISTRY_API CRITICAL_SECTION tuix_registry_lock;
#define tuix_lock() EnterCriticalSection(&tuix_registry_lock)
#define tuix_unlock() LeaveCriticalSection(&tuix_registry_lock)
#else
#include <pthread.h>
extern TUIX_REGISTRY_API pthread_mutex_t tuix_registry_lock;
#define tuix_lock() pthread_mutex_lock(&tuix_registry_lock)
#define tuix_unlock() pthread_mutex_unlock(&tuix_registry_lock)
#endif

#endif