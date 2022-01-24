#pragma once

#include "Types.h"
#include "StringBuffer.h"

typedef size_t (*StringTranslator)(StringBuffer strBuf, uint src);

bool StringTranslate(StringBuffer strBuf, StringTranslator* converters);

bool StringTranslate_shell(StringBuffer strBuf);

bool StringTranslate_exec(StringBuffer strBuf);
