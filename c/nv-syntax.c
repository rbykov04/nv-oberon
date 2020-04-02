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
#include "nv-risc.h"
#include "nv-sym-table.h"



enum {
	T0 = 0, T1 =1
};
void nv_record(int type, const char *id, int n){
	//printf("record %d %s %d\n", type, id, n);
}
int nv_getReg(nv_compiler_t *cmpl, int *r){
	cmpl->regs++;
	*r = cmpl->regs+1;
	return 0;
}
int nv_load(nv_compiler_t *cmpl, nv_item_t *x){
	if (x->mode == CLASS_VAR){
		int r;
		nv_getReg(cmpl, &r);
		nv_rics_put(cmpl->mem_of_code+cmpl->cmd, RISC_MOVI, r, 0,  0);
		cmpl->cmd++;
		nv_rics_put(cmpl->mem_of_code+cmpl->cmd, RISC_LDW, r, r, x->a);
		cmpl->cmd++;
		x->r = r;
	}else if (x->mode == CLASS_CONST){
		if (x->a == 0){
			x->r = 0;
		}else{
			nv_getReg(cmpl, &x->r);
			nv_rics_put(cmpl->mem_of_code+cmpl->cmd, RISC_MOVI, x->r, 0,  x->a);
			cmpl->cmd++;
		}
	}
	x->mode = CLASS_REG;
	return 0;
}
int nv_op1(nv_compiler_t *cmpl, int op, nv_item_t *x){
	if(op == LEX_MINUS){
		if (x->mode == CLASS_CONST ){
			x->a += -x->a;
		}else if (x->mode == CLASS_VAR ){
			nv_load(cmpl, x);
			nv_rics_put(cmpl->mem_of_code+cmpl->cmd, RISC_MVN, x->r, 0, x->r);
			cmpl->cmd++;
		}
	}
	return 0;
}
int nv_op2(nv_compiler_t *cmpl, int op, nv_item_t *x, nv_item_t *y){
	if (x->mode == CLASS_CONST && y->mode == CLASS_CONST){
		if(op == LEX_PLUS){
			x->a += y->a;
		} else if(op == LEX_DIV){
			x->a /= y->a;
		} else if(op == LEX_TIMES){
			x->a *= y->a;
		} else if(op == LEX_MINUS){
			x->a -= y->a;
		}
		return 0;
	}else {
		switch(op){
			case LEX_PLUS: 	nv_putOp(cmpl, RISC_ADD,x,y); break;
			case LEX_MINUS:	nv_putOp(cmpl, RISC_SUB,x,y); break;
			case LEX_TIMES: nv_putOp(cmpl, RISC_MUL,x,y); break;
			case LEX_DIV: 	nv_putOp(cmpl, RISC_DIV,x,y); break;
			default: return -1;
		}
		return 0;
	}

}
int nv_putOp(nv_compiler_t *cmpl, int cd, nv_item_t *x, nv_item_t *y){
	if (x->mode != CLASS_REG){
		nv_load(cmpl, x);
	}
	if (y->mode == CLASS_CONST){
		nv_rics_put(cmpl->mem_of_code+cmpl->cmd, cd + RISC_MOVI, x->r, x->r, y->a);
		cmpl->cmd++;
	}else{
		if (y->mode != CLASS_REG){
			nv_load(cmpl, y);
		}
		nv_rics_put(cmpl->mem_of_code+cmpl->cmd, cd, x->r, x->r, y->r);
		cmpl->cmd++;
		cmpl->regs -= y->r;
	}
	return 0;
}

int nv_index(nv_compiler_t *cmpl, nv_item_t *x, nv_item_t *y){
	return 0;
}

int nv_selector(nv_compiler_t *cmpl, nv_item_t *x){
	nv_item_t y;
	
	while(cmpl->sym == LEX_LBRAK || cmpl->sym == LEX_PERIOD){
		if (cmpl->sym == LEX_LBRAK){
			nv_getSym(cmpl);
			nv_expression(cmpl, &y);
			if (x->type->from == CLASS_ARRAY){
				nv_index(cmpl, x, &y);
				nv_wait_sym(cmpl, LEX_RBRAK);
			} else {
				nv_mark(cmpl, "not array");
			}

		}
	}


	return 0;
}
int nv_factor(nv_compiler_t *cmpl, nv_item_t *x){
	if (cmpl->sym == LEX_IDENT){
		void *it = nv_SymTable.find(cmpl->sym_table, cmpl->id);
		if (nv_SymTable.is_end(cmpl->sym_table, it)){
			nv_mark(cmpl, "variable %s undeclared", cmpl->id);
		}else{
			nv_object_t *obj = nv_SymTableIt.get(cmpl->sym_table, it);
			nv_make_item(cmpl, x, obj);
		}
		nv_SymTableIt.release(it);
		nv_getSym(cmpl);
	}else if (cmpl->sym == LEX_NUMBER){
		x->mode = CLASS_CONST;
		x->a = cmpl->val;
		nv_getSym(cmpl);
	}else if (cmpl->sym < LEX_LPAREN){
		nv_mark(cmpl, "identificator?");
	}
	// do{
	// 	nv_getSym(cmpl);
	// }while(cmpl->sym >= LEX_LPAREN);
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
	return 0;
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
		// nv_expression(cmpl);
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
	return 0;
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
	while(1){
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
		}
		if (cmpl->sym == LEX_VAR){
			nv_getSym(cmpl);
			while(cmpl->sym == LEX_IDENT){
				void *first = NULL;
				nv_syntax_identList(cmpl, CLASS_VAR, &first);
				nv_type_t *type =NULL;
				nv_syntax_type1(cmpl, &type);
				if (first){
					while(!nv_SymTable.is_end(cmpl->sym_table, first)){
						nv_object_t *t = nv_SymTableIt.get(cmpl->sym_table, first);
						t->type = type;
						cmpl->addr += type->size;
						t->val = 0- cmpl->addr;
						first = nv_SymTableIt.next(first);
					}
					nv_SymTableIt.release(first);
				}
				nv_wait_sym(cmpl, LEX_SEMICOLON);
			}

		}
		if (cmpl->sym >= LEX_CONST && cmpl->sym <=LEX_VAR){
			nv_mark(cmpl, "declaration?");
		}else{
			break;
		}
	}
	return 0;
}

int nv_term(nv_compiler_t *cmpl, nv_item_t *x){
	nv_factor(cmpl, x);

	while(cmpl->sym == LEX_TIMES || cmpl->sym == LEX_DIV){
		int op = cmpl->sym;
		nv_item_t y;
		nv_getSym(cmpl);
		nv_factor(cmpl, &y);
		nv_op2(cmpl, op, x, &y);
	}
	return 0;
}
int nv_simple_expression(nv_compiler_t *cmpl, nv_item_t *x){

	nv_item_t y;
	if (cmpl->sym == LEX_PLUS){
		nv_getSym(cmpl);
		nv_term(cmpl, x);
	}else if (cmpl->sym == LEX_MINUS){
		nv_getSym(cmpl);
		nv_term(cmpl, x);
		nv_op1(cmpl, LEX_MINUS, x);
	}else{
		nv_term(cmpl, x);
	}
	while(cmpl->sym == LEX_PLUS || cmpl->sym == LEX_MINUS){
		int op = cmpl->sym;
		nv_item_t y;
		nv_getSym(cmpl);
		nv_term(cmpl, &y);
		nv_op2(cmpl, op, x, &y);
	}

	return 0;
}
int nv_expression(nv_compiler_t *cmpl, nv_item_t *x){
	nv_simple_expression(cmpl, x);
	// nv_term(cmpl);
	// while(cmpl->sym == LEX_AND){
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
	//nv_expression(cmpl);
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
	return 0;
}
static int nv_writeln(nv_compiler_t *cmpl){
	nv_wait_sym(cmpl, LEX_LPAREN);
	nv_item_t x;
	memset(&x, 0, sizeof(nv_item_t));
	nv_expression(cmpl, &x);

	if (x.mode != CLASS_REG){
		nv_load(cmpl, &x);
	}
	nv_rics_put(cmpl->mem_of_code+cmpl->cmd, RISC_WRD, 0, 0, x.r);
	cmpl->cmd++;
	nv_rics_put(cmpl->mem_of_code+cmpl->cmd, RISC_WRL, 0, 0, 0);
	cmpl->cmd++;
	nv_wait_sym(cmpl, LEX_RPAREN);
	nv_wait_sym(cmpl, LEX_SEMICOLON);
	return 0;
}

int nv_statSequence(nv_compiler_t *cmpl){
	nv_item_t x, y;
	while (cmpl->sym == LEX_IDENT){
		if (!strcmp(cmpl->id, "writeln")){
			nv_getSym(cmpl);
			nv_writeln(cmpl);
			continue;
		}
		void *it = nv_SymTable.find(cmpl->sym_table, cmpl->id);
		if (nv_SymTable.is_end(cmpl->sym_table, it)){
			nv_mark(cmpl, "variable %s undeclared", cmpl->id);
		}else{
			nv_getSym(cmpl);
			nv_object_t *obj  =nv_SymTableIt.get(cmpl->sym_table, it);
			x.mode = obj->class;
			x.a = -obj->val;
			x.r = 0;
			nv_selector(cmpl, &x);
			if (cmpl->sym == LEX_BECOMES){

				nv_getSym(cmpl);
				nv_expression(cmpl, &y);
				if (y.mode != CLASS_REG){
					nv_load(cmpl, &y);
				}
				nv_rics_put(cmpl->mem_of_code+cmpl->cmd, RISC_STW, y.r, 0, x.a);
				cmpl->cmd++;
				cmpl->regs--;
				nv_wait_sym(cmpl, LEX_SEMICOLON);
			}
		}
		nv_SymTableIt.release(it);
	}
	return 0;
}
int nv_syntax(nv_compiler_t *cmpl){
	nv_syntax_add_to_key_table(cmpl, CLASS_TYPE, 1, "BOOLEAN", &nv_BoolType);
	nv_syntax_add_to_key_table(cmpl, CLASS_TYPE, 2, "INTEGER", &nv_IntType);
	nv_declaration(cmpl);
	nv_wait_sym(cmpl, LEX_BEGIN);
	nv_statSequence(cmpl);
	nv_rics_put(cmpl->mem_of_code+cmpl->cmd, RISC_RET, 0,0,0);
	cmpl->cmd++;
	return 0;
}

