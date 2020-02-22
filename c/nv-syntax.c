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
int nv_declaration(nv_compiler_t *cmpl){
	if (cmpl->sym < LEX_CONST){
		nv_mark(cmpl, "declaration?");
		do{
			nv_getSym(cmpl);
		}while(cmpl->sym >= LEX_CONST);
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

int nv_syntax(nv_compiler_t *cmpl){
	while(cmpl->sym == LEX_IDENT){
		nv_production(cmpl);
	}
	return 0;
}