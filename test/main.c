#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

typedef int bool;

#define true 1
#define false 0

#define string string
typedef char* string;

#define FOK		0
#define FBAD	-1
#define SE(err)	(errno = err) ? FBAD : FOK

#define JZ(val, err)	if (!(val)) goto err
#define JE(val, err)	(SE(FOK), val); JZ(!errno, err)

#define RESIZE(var, obj, n, err)	\
	do { \
		size_t _n = (n); \
		obj* _tmp = reallocarray(var, _n ? _n : 1, sizeof(obj)); \
		JZ(_tmp, err); \
		var = _tmp; \
	} while (false)

#define ARRAY(var, obj, n, err)	var = NULL; RESIZE(var, obj, n, err)
#define NEW(var, obj, err)		ARRAY(var, obj, 1, err)

#define var __auto_type
#define auto(type) __attribute__((cleanup(type##_delete))) type

#define CLASS(type) \
struct type##_this; \
typedef struct type##_this* type; \
struct type##_this

#define CLASS(type) \
struct type##_this; \
typedef struct type##_this* type; \
struct type##_this

#define CONSTRUCTOR(type) \
bool _##type##_new(type this); \
type type##_new() { \
	type NEW(this, struct type##_this, e1); \
	JE(_##type##_new(this), e2); \
	return SE(FOK), this; \
e2:	return free(this), NULL; \
e1:	return SE(ENOMEM), NULL; \
} \
bool _##type##_new(type this)

#define DESTRUCTOR(type) \
void _##type##_delete(type this); \
void type##_delete(type* ref) { \
	type this = *ref; \
	*ref = NULL; \
	if (this) { \
		_##type##_delete(this); \
		free(this); \
	} \
} \
void _##type##_delete(type this)

#define StringBuffer StringBuffer

CLASS(StringBuffer) {
	size_t size;
	size_t capacity;
	string data;
};

CONSTRUCTOR(StringBuffer) {
	this->size = 0;
	this->capacity = 128;
	ARRAY(this->data, char, this->capacity, e1);
	return SE(FOK);
e1:	return SE(ENOMEM);
}

DESTRUCTOR(StringBuffer) {
	free(this->data);
	printf("FREE\n");
}

string a() {
	var n = "malloc(1000);";
	var i = 1;
	return n + i;
}

int main() {
	void* p = malloc(1);
	void* n = reallocarray(p, 0, 1);
	perror("p");
	printf("%p\n", p);
	printf("%p\n", n);
}
