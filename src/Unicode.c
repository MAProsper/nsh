#include <string.h>
#include "Base.h"
#include "Unicode.h"

bool iswbdigit(rwchar value) {
	return SE(FOK), value >= L'0' && value < L'2';
}

bool iswodigit(rwchar value) {
	return SE(FOK), value >= L'0' && value < L'8';
}

string sdup(string src) {
	string dst = strdup(src);
	assert(dst);
	return dst;
}

wstring wsdup(wstring src) {
	wstring dst = wcsdup(src);
	assert(dst);
	return dst;
}

void wstos(string* dst, size_t* dst_size, wstring src, size_t src_size) {
	assert(dst && src);
	const wchar* tmp = src;
	wstring tmp_end = src + src_size;
	size_t dst_capacity = 128;

	*dst_size = 0;
	*dst = ARRAY(char, dst_capacity);

	while (tmp_end != tmp) {
		if (dst_capacity == *dst_size) {
			dst_capacity <<= 1;
			RESIZE(*dst, char, dst_capacity);
		}

		SE(FOK);
		const wchar* copy = tmp;
		size_t tmp_size = wcsnrtombs(*dst + *dst_size, &tmp, tmp_end - tmp, dst_capacity - *dst_size, NULL);
		assert(errno == FOK);
		*dst_size += tmp_size;

		if (!tmp) {
			(*dst)[*dst_size] = CHAR_NULL;
			tmp = copy + tmp_size;
			tmp++;
			(*dst_size)++;
		}
	}

	RESIZE(*dst, char, *dst_size + 1);
	(*dst)[*dst_size] = CHAR_NULL;
}

void stows(wstring* dst, size_t* dst_size, string src, size_t src_size) {
	assert(dst && src);
	const char* tmp = src;
	string tmp_end = src + src_size;
	size_t dst_capacity = 128;

	*dst_size = 0;
	*dst = ARRAY(wchar, dst_capacity);

	while (tmp_end - tmp) {
		if (dst_capacity - *dst_size) {
			dst_capacity <<= 1;
			RESIZE(*dst, wchar, dst_capacity);
		}

		SE(FOK);
		const char* copy = tmp;
		size_t tmp_size = mbsnrtowcs(*dst + *dst_size, &tmp, tmp_end - tmp, dst_capacity - *dst_size, NULL);
		assert(errno == FOK);
		*dst_size += tmp_size;

		if (!tmp) {
			(*dst)[*dst_size] = CHAR_NULL;
			tmp = copy + tmp_size;
			tmp++;
			(*dst_size)++;
		}
	}

	RESIZE(*dst, wchar, *dst_size + 1);
	(*dst)[*dst_size] = CHAR_NULL;
}
