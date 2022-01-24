#include <stdio.h>
#include "Base.h"
#include "Unicode.h"
#include <stdarg.h>

void aprintf(const char* format, ...) {
	assert(format);
	va_list args;
	va_start(args, format);
	int size = vsnprintf(NULL, 0, format, args);
	assert(size >= 0);
	va_end(args);
	va_start(args, format);
	assert(vprintf(format, args) == size);
	va_end(args);
}

void Logging_info(string msg) {
	aprintf("%s", msg);
}

void Logging_winfo(wstring msg) {
	local string tmp; size_t tmp_size;
	wstos(&tmp, &tmp_size, msg, wcslen(msg));
	Logging_info(tmp);
}
