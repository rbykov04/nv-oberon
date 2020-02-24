// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-syntax.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nv-lex.h"
#include "nv-error.h"

#include "nv-syntax.h"
#include "nv-generate.h"
#include "nv-sym-table.h"

enum {
	T0 = 0, T1 =1
};

void nv_record(int type, const char *id, int n){
	printf("record %d %s %d\n", type, id, n);
}

int nv_factor(nv_compiler_t *cmpl){
	if (cmpl->sym < LEX_LPAREN){
		nv_mark(cmpl, "identificator?");
	}
	do{
		nv_getSym(cmpl);
	}while(cmpl->sym >= LEX_LPAREN);
	return 0;
}
int nv_statSequence(nv_compiler_t *cmpl){
	if (cmpl->sym < LEX_IDENT){
		nv_mark(cmpl, "operator?");
		do{
			nv_getSym(cmpl);
		}while(cmpl->sym >= LEX_IDENT);
	}
	return 0;
}
int nv_type(nv_compiler_t *cmpl){
	if (cmpl->sym !=LEX_IDENT && cmpl->sym >= LEX_CONST){
		nv_mark(cmpl, "type?");
		do{
			nv_getSym(cmpl);
		}while(cmpl->sym == LEX_IDENT || cmpl->sym >= LEX_ARRAY );
	}
	return 0;
}
int nv_syntax_identList(nv_compiler_t *cmpl, int class, void **first){
	if (cmpl->sym == LEX_IDENT){
		*first = nv_SymTable.insert(cmpl, cmpl->sym_table, cmpl->id, class);
		nv_getSym(cmpl);
		while(cmpl->sym == LEX_COMMA){
			nv_getSym(cmpl);
			if (cmpl->sym == LEX_IDENT){
				void *it = nv_SymTable.insert(cmpl, cmpl->sym_table, cmpl->id, class);
				nv_SymTableIt.release(it);
				nv_getSym(cmpl);
			}else{
				nv_mark(cmpl, "identificator?");
			}
		}
		if (cmpl->sym == LEX_COLON){
			nv_getSym(cmpl);
		}else{
			nv_mark(cmpl, "wait ':'");
		}
	}else{
		nv_mark(cmpl, "identificator?");
	}
}
int nv_syntax_type1(nv_compiler_t *cmpl, void *first, nv_type_t **type){
	*type = NULL;
	if (cmpl->sym == LEX_IDENT){
		void *it = nv_SymTable.find(cmpl->sym_table, cmpl->id);
		if (!nv_SymTable.is_end(cmpl->sym_table, it)){
			nv_object_t *t = nv_SymTableIt.get(cmpl->sym_table, it);
			if (t->class == CLASS_TYPE){
				*type = t->type;
			}else{
				nv_mark(cmpl, "Type?");
			}
		}else{
			nv_mark(cmpl, "Type?");
		}
		nv_SymTableIt.release(it);
		nv_getSym(cmpl);

	}else{
		nv_mark(cmpl, "identificator?");
	}
}

int nv_declaration(nv_compiler_t *cmpl){
	if (cmpl->sym < LEX_CONST){
		nv_mark(cmpl, "declaration?");
		do{
			nv_getSym(cmpl);
		}while(cmpl->sym >= LEX_CONST);
	}
	if (cmpl->sym == LEX_VAR){
		nv_getSym(cmpl);
		while(cmpl->sym == LEX_IDENT){
			void *first = NULL;
			nv_syntax_identList(cmpl, CLASS_VAR, &first);
			nv_type_t *type =NULL;
			nv_syntax_type1(cmpl, first, &type);
			if (!first){
				while(!nv_SymTable.is_end(cmpl->sym_table, first)){
					nv_object_t *t = nv_SymTableIt.get(cmpl->sym_table, first);
					t->type = type;
					first = nv_SymTableIt.next(first);
				}
				nv_SymTableIt.release(first);
			}
		}

	}
	return 0;
}

int nv_term(nv_compiler_t *cmpl){
	do {
		nv_factor(cmpl);
	}while(cmpl->sym < LEX_AND);
	return 0;
}
int nv_expression(nv_compiler_t *cmpl){
	nv_term(cmpl);
	while(cmpl->sym == LEX_AND){
		nv_getSym(cmpl);
		nv_term(cmpl);
	}
	return 0;
}
int nv_production(nv_compiler_t *cmpl){
	nv_getSym(cmpl);
	if (cmpl->sym == LEX_EQL){
		nv_getSym(cmpl);
	}else{
		nv_error(cmpl, 7);
	}
	nv_expression(cmpl);
	if (cmpl->sym == LEX_PERIOD){
		nv_getSym(cmpl);
	}else{
		nv_error(cmpl, 8);
	}
	return 0;
}

int nv_syntax_add_to_key_table(
	nv_compiler_t *cmpl,
	int class,
	int n,
	const char *name,
	nv_type_t * type
){
	void *it = nv_SymTable.insert(cmpl, cmpl->sym_table,  name, class);
	nv_object_t *obj = nv_SymTableIt.get(cmpl->sym_table, it);
	obj->type = type;
	free(it);
}

int nv_syntax(nv_compiler_t *cmpl){
	nv_syntax_add_to_key_table(cmpl, CLASS_TYPE, 1, "BOOLEAN", &nv_BoolType);
	nv_syntax_add_to_key_table(cmpl, CLASS_TYPE, 2, "INTEGER", &nv_IntType);
	nv_declaration(cmpl);

	void *it = nv_SymTableIt.begin(cmpl->sym_table);
	int i = 0;
	while(!nv_SymTable.is_end(cmpl->sym_table, it)){
		nv_object_t *t = nv_SymTableIt.get(cmpl->sym_table, it);
		printf("sym_table[%d]:%5d %s \n", i, t->class, t->name);
		it = nv_SymTableIt.next(it);
		i++;
	}
	nv_SymTableIt.release(it);
	// while(cmpl->sym == LEX_IDENT){
	// 	nv_production(cmpl);
	// }
	return 0;
}
