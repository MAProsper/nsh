#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <wchar.h>

typedef int bool;
#define true 1
#define false 0

#define rwchar rwchar
typedef wint_t rwchar;

#define string string
typedef char* string;

#define wchar wchar
typedef wchar_t wchar;

#define wstring wstring
typedef wchar* wstring;
