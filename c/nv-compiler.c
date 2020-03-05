// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-compiler.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nv-compiler.h"
#include "nv-sym-table.h"
#include "nv-lex.h"
#include "nv-syntax.h"
#include "nv-utils.h"
#include "nv-risc.h"
#include "nv-risc-asm.h"

nv_compiler_t *nv_compilerInit(const char *filename, const char *text, int *code, size_t code_size){
	static nv_reader_t r;
	static nv_compiler_t cmpl;
	nv_readerInit(&r, text);
	cmpl.sym_table = nv_SymTable.new();
	cmpl.R = &r;
	cmpl.lastpos = 0;
	cmpl.errpos = 0;
	cmpl.error = 0;
	cmpl.current_filename = filename;
	cmpl.mem_of_code = code;
	cmpl.cmd = 0;
	cmpl.regs = 0;
	cmpl.addr = 0;
	cmpl.code_npos = code_size;
	return &cmpl;
}

int nv_compile_and_run(nv_compiler_t *cmpl){
	nv_syntax(cmpl);

	void *it = nv_SymTableIt.begin(cmpl->sym_table);
	int i = 0;
	while(!nv_SymTable.is_end(cmpl->sym_table, it)){
		nv_object_t *t = nv_SymTableIt.get(cmpl->sym_table, it);
		//printf("sym_table[%d]:%5d %s", i, t->class, t->name);
		if (t->class == CLASS_VAR){
			//printf(" of type size =%ld; ", t->type->size);
			//printf("; addr = %d\n", t->val);
		}else{
			//printf("\n");
		}
		it = nv_SymTableIt.next(it);
		i++;
	}
	//printf("addr =  %d\n********RUN*******\n", addr);
	nv_SymTableIt.release(it);
	nv_risc_t risc;
	memset((void*)&risc, 0, sizeof(risc));
	nv_risc_load(&risc, cmpl->mem_of_code, cmpl->cmd);
	nv_risc_execute(&risc, 0);
	// while(cmpl->sym == LEX_IDENT){
	// 	nv_production(cmpl);
	// }
	return 0;
}

void nv_compile(const char *filename, const char *s){
	if (!*s){
		return;
	}
	nv_reader_t r;
	int code[1024];
	memset(code, 0, sizeof(code));
	nv_compiler_t *cmpl = nv_compilerInit(filename, s, code, 1024-1);
	//printf("\ntest: %s \nBEGIN\n", s);

	nv_getSym(cmpl);
	nv_compile_and_run(cmpl);
	//printf("END\n");
}
int nv_compile_file(const char *filename){
	char *text = nv_readFile(filename);
	if (!text){
		return -1;
	}
	nv_compile(filename, text);
	free(text);
	return 0;
}

int main(int argc, char *argv[]){
	if (argc < 2){
		return 0;
	}
	nv_sym_table_init();
	if (argv[1][0] == '-'){
		if ( argv[1][1] == 'r' ){
			nv_risc_t risc;
			memset((void*)&risc, 0, sizeof(risc));
			int test[1024] = {};
			int len = nv_ricsReadFile(argv[2], test, sizeof(test)/sizeof(int));
			nv_risc_load(&risc, test, len);
			nv_risc_execute(&risc, 0);
			return 0;
		}else if ( argv[1][1] == 'a' ){
			int nv_console_loglevel = 0;
			nv_compile_file(argv[2]);
			return 0;
		}else if ( argv[1][1] == 't' ){
			int nv_console_loglevel = 0;
			nv_compile_file(argv[2]);
			return 0;
		}
	}
	int i = 0;
	for (i = 1; i< argc; ++i){
		nv_compile_file(argv[i]);
	}
	return 0;
}
