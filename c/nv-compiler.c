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
#include "nv-lex.h"
#include "nv-syntax.h"

nv_compiler_t *nv_compilerInit(const char *text){
	static nv_reader_t r;
	static nv_compiler_t cmpl;
	nv_readerInit(&r, text);
	cmpl.R = &r;\
	cmpl.lastpos = 0;
	return &cmpl;
}

void nv_compile(const char *s){
	if (!*s){
		return;
	}
	nv_reader_t r;
	nv_compiler_t *cmpl = nv_compilerInit(s);
	printf("\ntest: %s \nBEGIN\n", s);

	nv_getSym(cmpl);
	nv_syntax(cmpl);
	printf("END\n");
}

int main(int argc, char *argv[]){
	if (argc == 2){
		nv_compile(argv[1]);
		return 0;
	}
	nv_compile("a = b.");
	// nv_compile("\"bra1241");
	return 0;
}