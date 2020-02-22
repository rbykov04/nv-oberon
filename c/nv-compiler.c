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

nv_compiler_t *nv_compilerInit(const char *filename, const char *text){
	static nv_reader_t r;
	static nv_compiler_t cmpl;
	nv_readerInit(&r, text);
	cmpl.sym_table = nv_SymTable.new();
	cmpl.R = &r;\
	cmpl.lastpos = 0;
	cmpl.errpos = 0;
	cmpl.error = 0;
	cmpl.current_filename = filename;
	return &cmpl;
}

void nv_compile(const char *filename, const char *s){
	if (!*s){
		return;
	}
	nv_reader_t r;
	nv_compiler_t *cmpl = nv_compilerInit(filename, s);
	printf("\ntest: %s \nBEGIN\n", s);

	nv_getSym(cmpl);
	nv_syntax(cmpl);
	printf("END\n");
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
	if (argv[1][0] == '-' && argv[1][1] == 'r' ){
		nv_risc_t risc;
		memset((void*)&risc, 0, sizeof(risc));
		unsigned int test[1024];
		int len = nv_ricsReadFile(argv[2], test, sizeof(test)/sizeof(int));
		nv_risc_load(&risc, test, len);
		nv_risc_execute(&risc, 0);
		return 0;
	}
	int i = 0;
	for (i = 1; i< argc; ++i){
		nv_compile_file(argv[i]);
	}
	return 0;
}
