#pragma once

//#define DEBUG

#ifdef DEBUG
#define forceinline inline
#include <cassert>
#else
#define forceinline __forceinline
#define assert(_Expression)
#endif

#include <cstddef>
#include <stdint.h>
