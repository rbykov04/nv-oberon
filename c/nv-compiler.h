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
	Identifier id;
} nv_compiler_t;

nv_compiler_t *nv_compilerInit(const char *text);

enum {
	LEX_IDENT 	= 0,
	LEX_LITERAL = 2,
	LEX_LPAREN 	= 3,
	LEX_LBRAK 	= 4,
	LEX_LBRACE 	= 5,
	LEX_BAR 	= 6,
	LEX_EQL 	= 7,
	LEX_RPAREN 	= 8,
	LEX_RBRAK 	= 9,
	LEX_RBRACE 	= 10,
	LEX_PERIOD 	= 11,
	LEX_OTHER 	= 12
};

void nv_compile(const char *s);

#endif //NV_COMPILER_H