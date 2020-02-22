// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-sym-table-vector.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */

#include <stdlib.h>
#include <string.h>

#include "nv-error.h"
#include "nv-compiler.h"
#include "nv-sym-table-vector.h"

int nv_vector_init(nv_vector_t* vec, size_t size){
	vec->start = malloc(size * sizeof (nv_object_t));
	vec->end = vec->start + size;
}
int nv_vector_free(nv_vector_t* vec){
	if (!vec->start){
		return 0;
	}
	free(vec->start);
	vec->start = NULL;
	vec->end = NULL;
	vec->cur = NULL;
	return 0;
}
int nv_vector_realloc(nv_vector_t* vec, int coef){
	size_t size = (vec->end - vec->start)/ sizeof (nv_object_t);
	size_t cur = (vec->cur - vec->start)/ sizeof (nv_object_t);
	vec->start = reallocarray(vec, size *coef,  sizeof (nv_object_t));
	vec->cur = vec->start + size;
}

int nv_vector_push_back(nv_vector_t* vec, const nv_object_t* obj){
	if (vec->cur == vec->end){
		nv_vector_realloc(vec, 2);
	}
	nv_object_copy(vec->cur, obj);
	int index = (vec->cur -vec->start)/sizeof (nv_object_t);
	vec->cur++;
	return index;
}
nv_object_t* nv_vector_index(nv_vector_t* vec, size_t index){
	return vec->start + index;
}
nv_object_t* nv_vector_begin(nv_vector_t* vec){
	return vec->start;
}
nv_object_t* nv_vector_end(nv_vector_t* vec){
	return vec->cur+1;
}

nv_object_t* nv_vector_back(nv_vector_t* vec){
	return vec->cur;
}

void* nv_symTableInit(){
	nv_vector_t *table  = malloc(sizeof(nv_vector_t));
	nv_vector_init(table, 100);

	return (void*)table;
}
void* nv_symTableFree(void *table){
	nv_vector_t *vec = (nv_vector_t*)table;
	nv_vector_free(vec);
	free(vec);
}
nv_object_t* nv_symTableNew(nv_compiler_t *cmpl, void *table, const nv_object_t* obj){
	nv_vector_t *vec = (nv_vector_t*)table;
	nv_object_t *it = nv_vector_begin(vec);
	nv_object_t *end = nv_vector_end(vec);
	int i = 0;
	for(; it != end; ++it){
		if (!strcmp(it->name, obj->name)){
			nv_mark(cmpl, "redeclaration");
			return it;
		}
		i++;
	}
	nv_vector_push_back(vec, obj);
	return nv_vector_back(vec);
}
