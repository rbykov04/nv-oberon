// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-compiler.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_COMPILER_H
#define NV_COMPILER_H
#include <string.h>

#include "nv-text.h"

#define IDLEN 31
typedef char Identifier[IDLEN+1];

typedef struct {
	const char *current_filename;
	void *sym_table;
	nv_reader_t *R;
	int sym;
	char ch;
	size_t lastpos;
	size_t errpos;
	int error;
	Identifier id;
	int val;
	int *mem_of_code;
	size_t cmd;
	size_t code_npos;
	int regs;
	int addr;
} nv_compiler_t;

nv_compiler_t *nv_compilerInit(const char *filename, const char *text, int *code, size_t code_size);

int nv_compile(const char *filename, const char *s, int *code, size_t len);

#endif //NV_COMPILER_H
