#include "Base.h"
#include "Environment.h"
#include "Unicode.h"

wstring Environment_get(wstring src) {
	assert(src);
	size_t hkey_size;
	local string hkey = NULL;

	wstos(&hkey, &hkey_size, src, wcslen(src));

	string hvalue = getenv(hkey); JZ(hvalue, r1);

	wstring dst; size_t dst_size;
	stows(&dst, &dst_size, hvalue, strlen(hvalue));

	return SE(FOK), dst;
r1: return SE(ENOMSG), NULL;
}