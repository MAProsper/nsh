#include <stdio.h>
#include "Base.h"
#include "Terminal.h"
#include "Unicode.h"
#include "StringBuffer.h"
#include "StringTranslate.h"
#include "Environment.h"

Terminal Terminal_get() {
	Terminal this = NEW(Terminal_this);
	int fd = fileno(stdin);
	assert(tcgetattr(fd, this) == FOK);
	return SE(FOK), this;
}

void Terminal_delete(Terminal* this) {
	assert(this && *this);
	free(*this);
}

void Terminal_set(Terminal this) {
	assert(this);
	int fd = fileno(stdin);
	assert(tcsetattr(fd, TCSANOW, this) == FOK);
}

void Terminal_setManual() {
	// Input: maps CR -> LF
	// Output: maps LF -> CR-LF
	local_(Terminal) this = Terminal_get();
	this->c_lflag &= ~(ISIG | ICANON | ECHO);
	Terminal_set(this);
}

void Terminal_showPromt() {
	// Promt, save
	local wstring prompt = Environment_get(L"PS1");
	if (!prompt) prompt = wsdup(L"\\$ ");

	local_(StringBuffer) strBuf = StringBuffer_from(prompt, wcslen(prompt) + 1);
	StringTranslate_shell(strBuf);	// Change to StringTranslate_prompt

	aprintf("%ls\x1B[s", StringBuffer_get(strBuf));
}

void Terminal_setCursor(uint n) {
	// Restore, move
    aprintf("\x1B[u");
	if (n) aprintf("\x1B[%dC", n);
}

void Terminal_setLine(wstring msg) {
	// Restore, clear, write
	assert(msg);
	aprintf("\x1B[u\x1B[J%ls", msg);
}

rwchar Terminal_getKey() {
	return getwchar();  // Revise error codes
}