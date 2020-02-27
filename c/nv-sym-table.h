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
typedef struct nv_type_ {
	int from;
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
	void* (*insert)(nv_compiler_t *cmplr, void *table, const char *name, int class);
	void* (*find)(void *table, const char *name);
	int   (*is_end)(void *table, void* it);
} nv_sym_table_t;

typedef struct {
	void* (*begin)(void *table);
	void* (*end)(void *table);
	void* (*next)(void *it);
	void* (*release)(void *it);
	nv_object_t* (*get)(void *table, void *index);
	void* (*last)(void *table);

	int (*is_equal)(void *l, void *r);
} nv_sym_table_it_t;

extern nv_sym_table_t nv_SymTable;
extern nv_sym_table_it_t nv_SymTableIt;

void nv_sym_table_init();
#endif //N_SYM_TABLE_H
