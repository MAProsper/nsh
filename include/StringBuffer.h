#pragma once

#include "Types.h"

typedef struct {
	size_t size;
	size_t capacity;
	wstring data;
} StringBuffer_this;

#define StringBuffer StringBuffer
typedef StringBuffer_this* StringBuffer;

StringBuffer StringBuffer_new();

StringBuffer StringBuffer_from(wstring src, size_t src_size);

void StringBuffer_replace(StringBuffer this, uint dst, size_t dst_size, wstring src, size_t src_size);

void StringBuffer_add(StringBuffer this, uint dst, wchar src);

void StringBuffer_remove(StringBuffer this, uint dst);

size_t StringBuffer_len(StringBuffer this);

wstring StringBuffer_get(StringBuffer this);

void StringBuffer_delete(StringBuffer* this);
