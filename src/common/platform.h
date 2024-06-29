#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(_MSC_VER)
struct IUnknown; // Workaround for "combaseapi.h(229): error C2187: syntax error: 'identifier' was unexpected here" when using /permissive-
#define PLATFORM_WINDOWS
#elif __APPLE__
#define PLATFORM_APPLE
#define PLATFORM_POSIX
#define _XOPEN_SOURCE
#elif __linux__
#define PLATFORM_LINUX
#define PLATFORM_POSIX
#endif
