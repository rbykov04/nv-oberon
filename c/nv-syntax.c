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
int nv_syntax_type1(nv_compiler_t *cmpl, nv_type_t **type){
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

	}else if (cmpl->sym == LEX_ARRAY){
		nv_getSym(cmpl);
		nv_expression(cmpl);
		int n = 0;
		if (cmpl->sym == LEX_NUMBER){
			n = cmpl->val;
			nv_getSym(cmpl);
		}else{
			nv_mark(cmpl, "wait number");
		}
		nv_wait_sym(cmpl, LEX_OF);
		nv_type_t *base_type;
		nv_syntax_type1(cmpl, &base_type);
		nv_type_t *t = calloc(1, sizeof(nv_type_t));
		t->from = CLASS_ARRAY;
		t->base = base_type;
		t->len = n;
		*type = t;
		// type->size = n *

	}else{
		nv_mark(cmpl, "identificator?");
	}
}
int nv_wait_sym(nv_compiler_t *cmpl, int sym){
	if (cmpl->sym != sym){
		nv_mark(cmpl, "Unexpected sym. wait %d.", sym);
		return -1;
	}
	nv_getSym(cmpl);
	return 0;
}
int nv_declaration(nv_compiler_t *cmpl){
	if (cmpl->sym < LEX_CONST){
		nv_mark(cmpl, "declaration?");
		do{
			nv_getSym(cmpl);
		}while(cmpl->sym >= LEX_CONST);
	}
	if (cmpl->sym == LEX_TYPE){
		nv_getSym(cmpl);
		while(cmpl->sym == LEX_IDENT){
			void *it = nv_SymTable.insert(cmpl, cmpl->sym_table, cmpl->id, CLASS_TYPE);
			nv_getSym(cmpl);
			nv_wait_sym(cmpl, LEX_EQL);
			nv_object_t *obj = nv_SymTableIt.get(cmpl->sym_table, it);
					
			nv_syntax_type1(cmpl, &(obj->type));
			nv_wait_sym(cmpl, LEX_SEMICOLON);
		}
	}else if (cmpl->sym == LEX_VAR){
		nv_getSym(cmpl);
		while(cmpl->sym == LEX_IDENT){
			void *first = NULL;
			nv_syntax_identList(cmpl, CLASS_VAR, &first);
			nv_type_t *type =NULL;
			nv_syntax_type1(cmpl, &type);
			if (!first){
				while(!nv_SymTable.is_end(cmpl->sym_table, first)){
					nv_object_t *t = nv_SymTableIt.get(cmpl->sym_table, first);
					t->type = type;
					first = nv_SymTableIt.next(first);
				}
				nv_SymTableIt.release(first);
			}
			nv_wait_sym(cmpl, LEX_SEMICOLON);
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
	// nv_term(cmpl);
	// while(cmpl->sym == LEX_AND){
	// 	nv_getSym(cmpl);
	// 	nv_term(cmpl);
	// }
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
