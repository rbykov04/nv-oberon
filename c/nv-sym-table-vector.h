// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-sym-table-vector.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef N_SYM_TABLE_VECTOR_H
#define N_SYM_TABLE_VECTOR_H

#include "nv-sym-table.h"

typedef struct {
	nv_object_t *start;
	nv_object_t *end;
	nv_object_t *cur;

} nv_vector_t;

int nv_vector_init(nv_vector_t* vec, size_t size);
int nv_vector_free(nv_vector_t* vec);
int nv_vector_realloc(nv_vector_t* vec, int coef);
int nv_vector_push_back(nv_vector_t* vec, const nv_object_t* obj);
nv_object_t* nv_vector_index(nv_vector_t* vec, size_t index);
nv_object_t* nv_vector_begin(nv_vector_t* vec);
nv_object_t* nv_vector_end(nv_vector_t* vec);

void *nv_symTableInit();
void* nv_symTableFree(void *table);
nv_object_t* nv_symTableNew(nv_compiler_t *cmpl, void *table, const nv_object_t* obj);
#endif //N_SYM_TABLE_VECTOR_H