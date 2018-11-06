#pragma once
#include <string>
#include "log.h"
#include "platform.h"
#include "platform_compiler.h"
#include "platform_stacktrace.h"

#ifdef PLATFORM_WINDOWS

#define decaf_handle_assert(x, e, m) \
   if (!(x)) { \
      assertFailed(__FILE__, __LINE__, e, m); \
      __debugbreak(); \
      abort(); \
   }

#define decaf_handle_warn_assert(x, e, m) \
   if (!(x)) { \
      gLog->warn("Asserted `{}` ({}) at {}:{}", e, m, __FILE__, __LINE__); \
   }

#define decaf_host_fault(f, t) \
   hostFaultWithStackTrace(f, t); \
   __debugbreak(); \
   abort();

#else

#define decaf_handle_assert(x, e, m) \
   if (UNLIKELY(!(x))) { \
      assertFailed(__FILE__, __LINE__, e, m); \
      __builtin_trap(); \
   }

#define decaf_handle_warn_assert(x, e, m) \
   if (UNLIKELY(!(x))) { \
      gLog->warn("Asserted `{}` ({}) at {}:{}", e, m, __FILE__, __LINE__); \
   }

#define decaf_host_fault(f, t) \
   hostFaultWithStackTrace(f, t); \
   __builtin_trap();

#endif

#define decaf_assert(x, m) \
   decaf_handle_assert(x, #x, m)

#define decaf_check(x) \
   decaf_handle_assert(x, #x, "")

#define decaf_check_warn(x) \
   decaf_handle_warn_assert(x, #x, "")

#define decaf_abort(m) \
   decaf_handle_assert(false, "0", m)

void
assertFailed(const char *file,
             unsigned line,
             const char *expression,
             const std::string &message);

void
hostFaultWithStackTrace(const std::string &fault,
                        platform::StackTrace *stackTrace);
