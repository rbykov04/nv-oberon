// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-syntax.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_SYNTAX_H
#define NV_SYNTAX_H

#include "nv-generate.h"

typedef struct {
	int mode;
	nv_type_t *type;
	int a;
	int r;
} nv_item_t; 

int nv_term(nv_compiler_t *cmpl, nv_item_t *x);
int nv_expression(nv_compiler_t *cmpl, nv_item_t *x);
int nv_syntax(nv_compiler_t *cmpl);
int nv_statSequence(nv_compiler_t *cmpl);
int nv_type(nv_compiler_t *cmpl);
int nv_declaration(nv_compiler_t *cmpl);
int nv_wait_sym(nv_compiler_t *cmpl, int sym);
int nv_putOp(nv_compiler_t *cmpl, int cd, nv_item_t *x, nv_item_t *y);
#endif //NV_SYNTAX_H
