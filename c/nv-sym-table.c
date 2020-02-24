// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-sym-table.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */

#include <stdlib.h>
#include <string.h>

#include "nv-sym-table.h"
#include "nv-error.h"

#include "nv-sym-table-vector.h"

int nv_object_copy(nv_object_t* to, const nv_object_t* from){
	memcpy((void *)to, (const void*)from, sizeof(nv_object_t));
	return 0;
}
nv_sym_table_t nv_SymTable;
nv_sym_table_it_t nv_SymTableIt;

void nv_sym_table_init(){
	memcpy(&nv_SymTable, &nv_Vec, sizeof(nv_Vec));
	memcpy(&nv_SymTableIt, &nv_VecIt, sizeof(nv_SymTableIt));
}
