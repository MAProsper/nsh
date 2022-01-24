#pragma once

#include <errno.h>

#define FOK		0
#define FBAD	-1
#define SE(err)	(errno = err) ? FBAD : FOK

#define JZ(val, err)	if (!(val)) goto err
#define JE(val, err)	(SE(FOK), val); JZ(!errno, err)
