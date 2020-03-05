// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-sym-table-vector.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "nv-error.h"
#include "nv-compiler.h"
#include "nv-sym-table-vector.h"

typedef struct {
	nv_object_t *start;
	nv_object_t *end;
	nv_object_t *cur;

} nv_vector_t;
typedef int nv_vector_iterator_t;


int nv_vector_init(nv_vector_t* vec, size_t size){
	vec->start = malloc(size * sizeof (nv_object_t));
	vec->end = vec->start + size;
	vec->cur = vec->start;
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
	size_t size = (vec->end - vec->start) * sizeof (nv_object_t);
	size_t cur = (vec->cur - vec->start)* sizeof (nv_object_t);
	vec->start = reallocarray((void *)vec, size * coef,  sizeof (nv_object_t));
	vec->cur = vec->start + size;
}

int nv_vector_push_back(nv_vector_t* vec, const nv_object_t* obj){
	if (vec->cur == vec->end){
		nv_vector_realloc(vec, 2);
	}
	nv_object_copy(vec->cur, obj);
	int index = vec->cur - vec->start;
	vec->cur++;
	return index;
}
nv_object_t* nv_vector_index(nv_vector_t* vec, size_t index){
	return &vec->start[index];
}

int nv_vector_size(nv_vector_t* vec){
	return (vec->cur - vec->start );
}

nv_object_t* nv_vector_back(nv_vector_t* vec){
	return vec->cur;
}


void* nv_symTable_begin(void *table){
	return calloc(1 ,sizeof(nv_vector_iterator_t));
}
nv_object_t* nv_symTable_get(void *table, void *index){
	return nv_vector_index(table, *(nv_vector_iterator_t*)index);
}
void* nv_symTable_end(void *table){
	nv_vector_iterator_t *it = calloc(1, sizeof(nv_vector_iterator_t));
	*it = nv_vector_size(table);
	return it;
}
void* nv_symTable_last(void *table){
	nv_vector_iterator_t *it = calloc(1, sizeof(nv_vector_iterator_t));
	*it = nv_vector_size(table) - 1;
	return it;
}
void* nv_symTable_next(void *it){
	nv_vector_iterator_t *it_ = (int *)it;
	*it_ = *it_ + 1;
	return it_;
}
void* nv_symTable_release_it(void *it){
	free(it);
}
int nv_symTable_isEqual(void *r, void *l){
	nv_vector_iterator_t *it_l = (int *)l;
	nv_vector_iterator_t *it_r = (int *)r;
	//printf("nv_symTable_isEqual: l [%d] -r[%d]\n", *it_r, *(int *)it_l);
	return *it_r == *it_l;
}

nv_sym_table_it_t nv_VecIt = {
	.is_equal = nv_symTable_isEqual,
	.begin = nv_symTable_begin,
	.end = nv_symTable_end,
	.get = nv_symTable_get,
	.next = nv_symTable_next,
	.release = nv_symTable_release_it,
	.last = nv_symTable_last,
};


void* nv_symTableInit(){
	nv_vector_t *table  = malloc(sizeof(nv_vector_t));
	nv_vector_init(table, 100);

	return (void*)table;
}
void* nv_symTableFree(void *table){
	nv_vector_free(table);
	free(table);
}

void* nv_symTableNew(nv_compiler_t *cmpl, void *table, const char *name, int class){
	//printf("vec size %d\n", nv_vector_size(table));
	void *it = nv_VecIt.begin(table);
	void *end = nv_VecIt.end(table);
	//printf("it: first [%d] -end[%d]\n", *(int *)it, *(int *)end);
	for(; !nv_VecIt.is_equal(it, end); it = nv_VecIt.next(it) ){
		//printf("it[%d]: %s\n", *(int *)it, nv_VecIt.get(table, it)->name);
		if (!strcmp(nv_VecIt.get(table, it)->name, name)){
			nv_mark(cmpl, "redeclaration");
			nv_VecIt.release(end);
			return it;
		}
	}
	//printf("not exist %s\n");
	nv_VecIt.release(it);
	nv_VecIt.release(end);
	nv_object_t obj = {
		.class = class
	};
	strcpy(obj.name, name);
	nv_vector_push_back(table, &obj);
	return nv_VecIt.last(table);
}
void* nv_symTableFind(void *table, const char *name){
	void *it = nv_VecIt.begin(table);
	void *end = nv_VecIt.end(table);
	for(; !nv_VecIt.is_equal(it, end); it = nv_VecIt.next(it) ){
		if (!strcmp(nv_VecIt.get(table, it)->name, name)){
			break;
		}
	}
	
	nv_VecIt.release(end);
	return it;
}
int nv_symTableIsEnd(void *table, void *it){
	return nv_vector_size(table) ==*(nv_vector_iterator_t*)it;
}
nv_sym_table_t nv_Vec = {
	.new = nv_symTableInit,
	.insert = nv_symTableNew,
	.delete = nv_symTableFree,
	.find = nv_symTableFind,
	.is_end = nv_symTableIsEnd,
};
