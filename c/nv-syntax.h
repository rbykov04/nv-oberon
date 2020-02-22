// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-syntax.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_SYNTAX_H
#define NV_SYNTAX_H
int nv_term(nv_compiler_t *cmpl);
int nv_expression(nv_compiler_t *cmpl);
int nv_syntax(nv_compiler_t *cmpl);
int nv_statSequence(nv_compiler_t *cmpl);
int nv_type(nv_compiler_t *cmpl);
int nv_declaration(nv_compiler_t *cmpl);

#endif //NV_SYNTAX_H