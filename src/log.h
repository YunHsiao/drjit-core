/*
    src/log.h -- Logging, log levels, assertions, string-related code.

    Copyright (c) 2021 Wenzel Jakob <wenzel.jakob@epfl.ch>

    All rights reserved. Use of this source code is governed by a BSD-style
    license that can be found in the LICENSE file.
*/

#pragma once

#include <drjit-core/jit.h>
#include <stdarg.h>
#include "common.h"

static constexpr DrJitLogLevel Disable = DrJitLogLevel::Disable;
static constexpr DrJitLogLevel Error   = DrJitLogLevel::Error;
static constexpr DrJitLogLevel Warn    = DrJitLogLevel::Warn;
static constexpr DrJitLogLevel Info    = DrJitLogLevel::Info;
static constexpr DrJitLogLevel InfoSym = DrJitLogLevel::InfoSym;
static constexpr DrJitLogLevel Debug   = DrJitLogLevel::Debug;
static constexpr DrJitLogLevel Trace   = DrJitLogLevel::Trace;

#if defined(NDEBUG)
#  define jitc_trace(...) do { } while (0)
#  define jitc_assert(...) do { } while (0)
#else
#  define jitc_trace(...) jitc_log(Trace, __VA_ARGS__)
#define jitc_assert(cond, fmt, ...)                                            \
    if (unlikely(!(cond)))                                                     \
        jitc_fail("drjit: assertion failure (\"%s\") in line %i: " fmt, #cond, \
                  __LINE__, ##__VA_ARGS__);
#endif

/// Print a log message with the specified log level and message
#if defined(__GNUC__)
    __attribute__((__format__ (__printf__, 2, 3)))
#endif
extern void jitc_log(DrJitLogLevel level, const char* fmt, ...);

/// Print a log message with the specified log level and message
extern void jitc_vlog(DrJitLogLevel level, const char* fmt, va_list args);

/// Raise a std::runtime_error with the given message
#if defined(__GNUC__)
    __attribute__((noreturn, __format__ (__printf__, 1, 2)))
#else
    [[noreturn]]
#endif
extern void jitc_raise(const char* fmt, ...);

/// Raise a std::runtime_error with the given message
[[noreturn]] extern void jitc_vraise(const char* fmt, va_list args);

/// Immediately terminate the application due to a fatal internal error
#if defined(__GNUC__)
    __attribute__((noreturn, __format__ (__printf__, 1, 2)))
#else
   [[noreturn]]
#endif
extern void jitc_fail(const char* fmt, ...) noexcept;

/// Immediately terminate the application due to a fatal internal error
[[noreturn]] extern void jitc_vfail(const char* fmt, va_list args) noexcept;

/// Return and clear the log buffer
extern char *jitc_log_buffer();

/// Convert a number of bytes into a human-readable string (returns static buffer!)
extern const char *jitc_mem_string(size_t size);

/// Convert a time in microseconds into a human-readable string (returns static buffer!)
extern const char *jitc_time_string(float us);

/// Return the number of microseconds since the previous timer() call
extern float timer();
