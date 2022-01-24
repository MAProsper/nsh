#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <sys/wait.h>
#include "Base.h"
#include "main.h"
#include "Unicode.h"
#include "StringBuffer.h"
#include "StringTranslate.h"
#include "Terminal.h"
#include "Environment.h"

string* sbtoargs(StringBuffer strBuf) {
	assert(strBuf);
	size_t args_size = 0;
	size_t args_capacity = 128;
	string* args = ARRAY(string, args_capacity);

	StringTranslate_exec(strBuf);

	string tmp = NULL;
	size_t tmp_size = 0;
	
	wstos(&tmp, &tmp_size, StringBuffer_get(strBuf), StringBuffer_len(strBuf));
	string tmp_end = tmp + tmp_size;

	while (tmp != tmp_end) {
		if (args_size == args_capacity) {
			args_capacity <<= 1;
			RESIZE(args, string, args_capacity);
		}

		args[args_size] = tmp;
		tmp += strlen(tmp) + 1;
		args_size++;
	}

	RESIZE(args, string, args_size + 1);
	args[args_size] = NULL;
	return args;
}

StringBuffer Terminal_getCmd() {
	uint cursor = 0;
	rwchar key = CHAR_NULL;
	local_(Terminal) term = Terminal_get();
	StringBuffer strBuf = StringBuffer_new();

	Terminal_setManual();
	Terminal_showPromt();
	StringBuffer_add(strBuf, 0, CHAR_NULL);

	// term->c_cc[VEOF]

	while (true) {
		key = Terminal_getKey();
		size_t size = StringBuffer_len(strBuf) - 1;

		if (key == term->c_cc[VEOF]) goto r2;

		if (key == term->c_cc[VERASE] && cursor > 0)
			StringBuffer_remove(strBuf, --cursor);

		if (key == CHAR_ESC) {
			key = Terminal_getKey();
			if (key == L'[') {
				key = Terminal_getKey();
				while (key >= 0x30 && key <= 0x3F) key = Terminal_getKey();
				while (key >= 0x20 && key <= 0x2F) key = Terminal_getKey();
				if (key >= 0x40 && key <= 0x7E) switch (key) {
					case L'C':
						if (cursor < size) cursor++;
						break;
					case L'D':
						if (cursor > 0) cursor--;
						break;
				}
				// fprintf(stderr, "\rESC %x ", key);
				key = CHAR_ESC;
			}
		}

		if (iswprint(key)) {
			StringBuffer_add(strBuf, cursor++, key);
		} else {
			// fprintf(stderr, "\rINV %x ", key);
		}

		Terminal_setLine(StringBuffer_get(strBuf));
		Terminal_setCursor(cursor);
	}

r2:	aprintf("\n");
	Terminal_set(term);
	SE(key == CHAR_LF ? FOK : ENOMEDIUM);
	return strBuf;
}

void Terminal_exec(StringBuffer strBuf) {
	assert(strBuf);
	local string* args = sbtoargs(strBuf);
	if (fork()) {
		wait(NULL);
	} else {
		execvp(args[0], args);
		if (errno == ENOENT) {
			execl("/usr/lib/command-not-found", "command-not-found", "--", args[0], NULL);
			SE(ENOENT);
		}
		perror(args[0]);
		exit(FBAD);
	}
}

int main(int _argc, string _argv[]) {
	// Setup
	setlocale(LC_ALL, ""); // or C.UTF-8
	argc = _argc; argv = _argv;

	assert(isatty(fileno(stdin)));
	wstring TERM = L"xterm";
	local wstring term = Environment_get(L"TERM");
	assert(!wcsncmp(TERM, term, wcslen(TERM)));

	// Main
	while (true) {
		local_(StringBuffer) strBuf = Terminal_getCmd();
		if (StringBuffer_len(strBuf) == 1) break;
		Terminal_exec(strBuf);
	}

	return FOK;
}
