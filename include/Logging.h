#pragma once

#include <assert.h>
#include "Types.h"

#ifdef NDEBUG
#define assert(val)	if (!(val)) abort()
#endif

void aprintf(const char* format, ...);

void Logging_info(string msg);

void Logging_winfo(wstring msg);
