#include "Base.h"
#include "StringBuffer.h"

StringBuffer StringBuffer_new() {
	StringBuffer this = NEW(StringBuffer_this);
	this->capacity = 128;
	this->size = 0;
	this->data = ARRAY(wchar, this->capacity);
	return SE(FOK), this;
}

StringBuffer StringBuffer_from(wstring src, size_t src_size) {
	assert(src);
	StringBuffer this = StringBuffer_new();
	StringBuffer_replace(this, 0, 0, src, src_size);
	return SE(FOK), this;
}

void StringBuffer_replace(StringBuffer this, uint dst, size_t dst_size, wstring src, size_t src_size) {
	assert(this && src && dst_size <= this->size);
	uint dst_end = dst + dst_size;
	intmax_t size_diff = src_size - dst_size;
	size_t size_new = this->size + size_diff;
	size_t size_move = this->size - dst_end;

	if (size_new > this->capacity) {
		size_t capacity_new = size_new << 1;
		RESIZE(this->data, wchar, capacity_new);
		this->capacity = capacity_new;
	}

	wstring move_src = this->data + dst_end;
	wstring move_dst = move_src + size_diff;
	wstring src_dst = this->data + dst;
	wmemmove(move_dst, move_src, size_move);
	wmemcpy(src_dst, src, src_size);
	this->size = size_new;
}

void StringBuffer_add(StringBuffer this, uint dst, wchar src) {
	StringBuffer_replace(this, dst, 0, &src, 1);
}

void StringBuffer_remove(StringBuffer this, uint dst) {
	StringBuffer_replace(this, dst, 1, L"", 0);
}

size_t StringBuffer_len(StringBuffer this) {
	assert(this);
	return SE(FOK), this->size;
}

wstring StringBuffer_get(StringBuffer this) {
	assert(this);
	return SE(FOK), this->data;
}

void StringBuffer_delete(StringBuffer* this) {
	assert(this && *this);
	free((*this)->data);
	free(*this);
	SE(FOK);
}
