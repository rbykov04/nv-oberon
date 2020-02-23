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
	LEX_NULL = 0,
	LEX_TIMES = 1,
	LEX_DIV = 3,
	LEX_MOD =4,
	LEX_AND = 5,
	LEX_PLUS =6,
	LEX_MINUS =7,
	LEX_OR =8,
	LEX_EQL = 9,
	LEX_NEQ = 10,
	LEX_LSS = 11,
	LEX_GEQ = 12,
	LEX_LEQ = 13,
	LEX_GTR = 14,
	LEX_PERIOD = 18,
	LEX_COMMA = 19,
	LEX_COLON = 20,
	LEX_RPAREN = 22,
	LEX_RBRAK =23,
	LEX_OF =25,
	LEX_THEN = 26,
	LEX_LPAREN = 29,
	LEX_LBRAK = 30,
	LEX_NOT = 32,
	LEX_BECOMES = 33,
	LEX_NUMBER = 34,
	LEX_IDENT = 37,
	LEX_SEMICOLON = 38,
	LEX_END =40,
	LEX_ELSE = 41,
	LEX_ELSIF =42,
	LEX_IF = 44,
	LEX_WHILE = 46,
	LEX_DO = 47,
	LEX_ARRAY = 54,
	LEX_RECORD = 55,
	LEX_CONST =57,
	LEX_TYPE =58,
	LEX_VAR =59,
	LEX_PROCEDURE =60,
	LEX_BEGIN = 61,
	LEX_MODULE = 63,
	LEX_EOF =64
};

typedef struct {
	int (*init)(nv_compiler_t *cmpl);
	int (*get)(nv_compiler_t *cmpl);
	int (*mark)(const char *msg);
} nv_lexer_t;
extern nv_lexer_t OSS;

int nv_getSym(nv_compiler_t *cmpl);

#endif //NV_LEX_H
