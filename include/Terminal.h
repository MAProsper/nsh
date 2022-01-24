#pragma once

#include <termios.h>
#include "Types.h"

typedef struct termios Terminal_this;

#define Terminal Terminal
typedef Terminal_this* Terminal;

Terminal Terminal_get();

void Terminal_set(Terminal this);

void Terminal_setManual();

void Terminal_delete(Terminal* this);

void Terminal_showPromt();

void Terminal_setCursor(uint n);

void Terminal_setLine(wstring msg);

rwchar Terminal_getKey();
