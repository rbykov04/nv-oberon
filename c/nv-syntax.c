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
	switch (cmpl->sym){
		case LEX_IDENT: 
			nv_record(T0, cmpl->id, 1);
		break;
		// case LEX_LITERA: 
		// 	nv_record(T1, cmpl->id, 0);
		// break;
		case LEX_LPAREN:
			nv_getSym(cmpl);
			nv_expression(cmpl);
			if (cmpl->sym != LEX_RPAREN){
				nv_error(cmpl, 2);
			}
		break;
		case LEX_LBRAK:
			nv_getSym(cmpl);
			nv_expression(cmpl);
			if (cmpl->sym != LEX_RBRAK){
				nv_error(cmpl, 3);
			}
		break;
		case LEX_BEGIN:
			nv_getSym(cmpl);
			nv_expression(cmpl);
			if (cmpl->sym != LEX_END){
				nv_error(cmpl, 4);
			}
		break;
		default:
			nv_error(cmpl, 5);
			return -1;
	}
	nv_getSym(cmpl);
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