// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-lex.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_LEX_H
#define NV_LEX_H

#include "nv-compiler.h"

enum {
	LEX_IDENT 	= 0,
	LEX_LITERA = 2,
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

int nv_getSym(nv_compiler_t *cmpl);

#endif //NV_LEX_H