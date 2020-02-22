// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-compiler.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_COMPILER_H
#define NV_COMPILER_H

#include "nv-text.h"

#define IDLEN 31
typedef char Identifier[IDLEN+1];

typedef struct {
	nv_reader_t *R;
	int sym;
	char ch;
	size_t lastpos;
	size_t errpos;
	int error;
	Identifier id;
	int val;
} nv_compiler_t;

nv_compiler_t *nv_compilerInit(const char *text);

void nv_compile(const char *s);

#endif //NV_COMPILER_H