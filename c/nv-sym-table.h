// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-sym-table.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef N_SYM_TABLE_H
#define N_SYM_TABLE_H

#include "nv-compiler.h"
struct nv_type_;
typedef struct {
	int form;
	int len;
	void *fields;
	struct nv_type_ *base;
} nv_type_t;

typedef struct {
	Identifier name;
	int class;
	int val;
	nv_type_t *type; 
} nv_object_t;
int nv_object_copy(nv_object_t* to, const nv_object_t* from);

typedef struct {
	void* (*new)();
	void* (*delete)(void *table);
	nv_object_t* (*insert)(nv_compiler_t *cmplr, void *table, const nv_object_t* obj);
} nv_sym_table_t;

extern nv_sym_table_t nv_SymTable;
#endif //N_SYM_TABLE_H
