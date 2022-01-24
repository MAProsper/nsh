#pragma once

#include <stdlib.h>
#include "Types.h"

#define local __attribute__((cleanup(_Memory_delete)))
#define local_(type) __attribute__((cleanup(type##_delete))) type
void _Memory_delete(void* ref);

#define RESIZE(var, obj, size)	Memory_resize(&(var), sizeof(obj), size)
void* Memory_resize(void* src, size_t unit, size_t size);

#define ARRAY(obj, size)	Memory_array(sizeof(obj), size)
void* Memory_array(size_t unit, size_t size);

#define NEW(obj)	Memory_new(sizeof(obj))
void* Memory_new(size_t unit);
