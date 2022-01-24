#pragma once

#include <string.h>
#include <wctype.h>
#include "Types.h"

#define CHAR_NULL	0x00	// \0
#define CHAR_EOT	0x04	// Ctrl+D
#define CHAR_BELL	0x07	// \a
#define CHAR_TAB	0x09	// \t
#define CHAR_LF		0x0A	// \n
#define CHAR_VTAB	0x0B	// \v
#define CHAR_CR		0x0D	// \r
#define CHAR_ESC	0x1B	// \e
#define CHAR_SPACE	0x20
#define CHAR_QUOTE	0x22	// "
#define CHAR_APOS	0x27	// '
#define CHAR_BSLASH	0x5C
#define CHAR_DEL	0x7F
#define CHAR_EOF	WEOF

bool iswbdigit(wint_t value);

bool iswodigit(wint_t value);

string sdup(string src);

wstring wsdup(wstring src);

void wstos(string* dst, size_t* dst_size, wstring src, size_t src_size);

void stows(wstring* dst, size_t* dst_size, string src, size_t src_size);
