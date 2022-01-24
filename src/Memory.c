#include "Base.h"

void _Memory_delete(void* ref) {
	void** tmp = (void**) ref;
	if (*tmp) free(*tmp);
	*tmp = NULL;
}

void* Memory_resize(void* src, size_t unit, size_t size) {
	assert(unit);
	if (!size) size++;
	void** tmp = (void**) src;
	*tmp = reallocarray(*tmp, size, unit);
	assert(*tmp != NULL);
	return *tmp;
}

void* Memory_array(size_t unit, size_t size) {
	void* src = NULL;
	return Memory_resize(&src, unit, size);
}

void* Memory_new(size_t unit) {
	return Memory_array(unit, 1);
}
