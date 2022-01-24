#include <unistd.h>
#include <pwd.h>
#include <libgen.h>
#include "Base.h"
#include "StringTranslate.h"
#include "Unicode.h"
#include "main.h"

size_t StringTranslate_arg(StringBuffer strBuf, uint src) {
	assert(strBuf);
	wchar value = CHAR_NULL;
	size_t rep_size = 1;

	size_t str_size = StringBuffer_len(strBuf) - src;
	JZ(str_size >= rep_size, e1);

	wstring str = StringBuffer_get(strBuf) + src;
	JZ(str[0] == CHAR_SPACE, e1);

	StringBuffer_replace(strBuf, src, rep_size, &value, 1);
	return SE(FOK), 1;
e1:	return SE(ENOMSG);
}

size_t StringTranslate_ctoc(StringBuffer strBuf, uint src) {
	assert(strBuf);
	wchar value;
	size_t rep_size = 2;

	size_t str_size = StringBuffer_len(strBuf) - src;
	JZ(str_size >= rep_size, e1);

	wstring str = StringBuffer_get(strBuf) + src;
	JZ(str[0] == CHAR_BSLASH, e1);

	switch (str[1]) {
		case L'a':
			value = CHAR_BELL;
			break;
		case L'e':
		case L'E':
			value = CHAR_ESC;
			break;
		case L'n':
			value = CHAR_LF;
			break;
		case L'r':
			value = CHAR_CR;
			break;
		case L't':
			value = CHAR_TAB;
			break;
		case L'v':
			value = CHAR_VTAB;
			break;
		case CHAR_BSLASH:
			value = CHAR_BSLASH;
			break;
		default:
			goto e1;
	}

	StringBuffer_replace(strBuf, src, rep_size, &value, 1);
	return SE(FOK), 1;
e1:	return SE(ENOMSG);
}

size_t StringTranslate_prompt(StringBuffer strBuf, uint src) {
	assert(strBuf);
	size_t src_size = 2;
	size_t dst_size = 0;
	size_t hdst_size = 128;
	local wstring dst = NULL; 
	local string hdst = ARRAY(char, hdst_size);

	size_t str_size = StringBuffer_len(strBuf) - src;
	JZ(str_size >= src_size, e1);

	wstring str = StringBuffer_get(strBuf) + src;
	JZ(str[0] == CHAR_BSLASH, e1);

	switch (str[1]) {
		case L'[':
		case L']':
			hdst = sdup("");
			break;
		case L'$':
			hdst = sdup(geteuid() ? "$" : "#");
			break;
		case L'u': {
			string name = getpwuid(geteuid())->pw_name;
			RESIZE(hdst, char, strlen(name) + 1);
			hdst = sdup(name);
			break;
		}
		case L's': {
			// Extract to variable functions & check env
			string path = sdup(argv[0]);
			string name = basename(path);
			RESIZE(hdst, char, strlen(name) + 1);
			strcpy(hdst, name);
			break;
		}
		case L'h':
			while (true) {
				assert(gethostname(hdst, hdst_size) == FOK);
				if (errno == FOK) goto r1;
				assert(errno != ENAMETOOLONG);
				RESIZE(hdst, char, hdst_size <<= 1);
			}
			break;
		case L'w':
			while (true) {
				if (getcwd(hdst, hdst_size) != NULL) goto r1;
				assert(errno != ERANGE);
				RESIZE(hdst, char, hdst_size <<= 1);
			}
			break;
		default:
			goto e1;
	}

r1:	stows(&dst, &dst_size, hdst, strlen(hdst));
	StringBuffer_replace(strBuf, src, src_size, dst, dst_size);
	return SE(FOK), dst_size;
e1:	return SE(ENOMSG);
}

size_t StringTranslate_stoc(StringBuffer strBuf, uint src) {
	assert(strBuf);
	uint fmt_base;
	size_t fmt_size;
	size_t prefix_size = 2;
	bool (*isvalid)(wint_t);

	size_t str_size = StringBuffer_len(strBuf) - src;
	JZ(str_size >= prefix_size, e1);

	wstring str = StringBuffer_get(strBuf) + src;
	JZ(str[0] == CHAR_BSLASH, e1);

	switch (str[1]) {
		case L'b':
			fmt_size = 8;
			fmt_base = 2;
			isvalid = &iswbdigit;
			break;
		case L'0':
			fmt_size = 3;
			fmt_base = 8;
			isvalid = &iswodigit;
			break;
		case L'x':
			fmt_size = 2;
			fmt_base = 16;
			isvalid = &iswxdigit;
			break;
		case L'u':
			fmt_size = 4;
			fmt_base = 16;
			isvalid = &iswxdigit;
			break;
		case L'U':
			fmt_size = 8;
			fmt_base = 16;
			isvalid = &iswxdigit;
			break;
		default:
			goto e1;
	}

	uint i;
	str += prefix_size;
	for (i = 0; i < fmt_size && i < str_size && (*isvalid)(str[i]); i++);
	size_t rep_size = prefix_size + i;

	uint fmt_end = src + rep_size;
	StringBuffer_add(strBuf, fmt_end, CHAR_NULL);
	str = StringBuffer_get(strBuf) + src + prefix_size;
	wchar value = wcstol(str, NULL, fmt_base);
	StringBuffer_remove(strBuf, fmt_end);

	StringBuffer_replace(strBuf, src, rep_size, &value, 1);
	return SE(FOK), 1;
e1:	return SE(ENOMSG);
}

size_t StringTranslate_group(StringBuffer strBuf, uint src, StringTranslator* translators) {
	size_t rep_size = 1;
	for (StringTranslator* translator = translators; *translator; translator++) {
		rep_size = (**translator)(strBuf, src);
		if (errno == FOK) break;
		if (errno == ENOMSG) rep_size = 1;
	}

	return SE(FOK), rep_size;
}

bool StringTranslate(StringBuffer strBuf, StringTranslator* translators) {
	for (uint i = 0; i < StringBuffer_len(strBuf);) {
		i += StringTranslate_group(strBuf, i, translators);
	}

	return SE(FOK);
}

bool StringTranslate_shell(StringBuffer strBuf) {
	StringTranslator translators[] = {
		&StringTranslate_prompt,
		&StringTranslate_ctoc,
		&StringTranslate_stoc,
		NULL
	};

	return StringTranslate(strBuf, translators);
}

bool StringTranslate_exec(StringBuffer strBuf) {
	StringTranslator translators[] = {
		&StringTranslate_ctoc,
		&StringTranslate_stoc,
		&StringTranslate_arg,
		NULL
	};

	return StringTranslate(strBuf, translators);
}
