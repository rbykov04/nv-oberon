// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-risc-asm.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "nv-risc.h"
#include "nv-utils.h"

#include "nv-risc-asm.h"

nv_keytab_t nv_RiscCode[] ={
	{"MOV", RISC_MOV},
	{"MVN", RISC_MVN},
	{"ADD", RISC_ADD},
	{"SUB", RISC_SUB},
	{"MUL", RISC_MUL},
	{"DIV", RISC_DIV},
	{"MOD", RISC_MOD},
	{"CMP", RISC_CMP},
	{"MOVI", RISC_MOVI},
	{"MVNI", RISC_MVNI},
	{"ADDI", RISC_ADDI},
	{"SUBI", RISC_SUBI},
	{"MULI", RISC_MULI},
	{"DIVI", RISC_DIVI},
	{"MODI", RISC_MODI},
	{"CMPI", RISC_CMPI},
	{"CHKI", RISC_CHKI},
	{"LDW", RISC_LDW},
	{"LDB", RISC_LDB},
	{"POP", RISC_POP},
	{"STW", RISC_STW},
	{"STB", RISC_STB},
	{"PSH", RISC_PSH},
	{"RD", RISC_RD},
	{"WRD", RISC_WRD},
	{"WRH", RISC_WRH},
	{"WRL", RISC_WRL},
	{"BEQ", RISC_BEQ},
	{"BNE", RISC_BNE},
	{"BLT", RISC_BLT},
	{"BGE", RISC_BGE},
	{"BLE", RISC_BLE},
	{"BGT", RISC_BGT},
	{"BR", RISC_BR},
	{"BSR", RISC_BSR},
	{"RET", RISC_RET},
	{"VAL", RISC_VAL},
	{NULL, RISC_NULL}
};
const char * nv_risc_number(const char *pos, int *val){
	int res = 0;
	do{
		int d = *pos - '0';
		if ((INT_MAX - d)/10 > res){
			res = res * 10 + d;
		}else{
			res = 0;
		}
		pos++;
	}while('0' <= *pos&& *pos <='9' && *pos);
	*val = res;
	return pos;
}

const char * nv_risc_op(const char *pos, int *code){
	int i = 0;
	char opc[10] = {0};
	do{
		opc[i] = *pos;
		i++;
		pos++;
	}while(i < sizeof(opc) && *pos &&  ('A' <= *pos && *pos <='Z'));
	opc[i] = 0;
	*code = nv_find_lex(nv_RiscCode, opc);
	return pos;
}
const char *nv_risc_getSym(const char *pos, int *sym, int *val){
	while(*pos <=' ' && *pos) pos++;
	switch (*pos){
		case '0'...'9':
			*sym = RISC_VAL;
			return nv_risc_number(pos, val);
		break;
		case 'A'...'Z':
			return nv_risc_op(pos, sym);
		break;

		default: *sym = RISC_NULL; break; 
	}
	return pos;
}	


int nv_risc_code(int op, int a, int b, int c){
	printf("%4s %4d %4d %4d\n", nv_find_lex_name(nv_RiscCode, op), a, b, c);
	unsigned int res = (unsigned int)op;
	res <<= 4;
	res +=a;
	res <<= 4;
	res +=b;
	res <<= 18;
	res +=c % 0x40000;
	return (int)res;
}
int nv_ricsReadFile(const char *filename, int *mem, size_t len){
	char *text = nv_readFile(filename);
	if (!text){
		return -1;
	}
	int i = 0;
	const char *pos = text;
	int op, a, b ,c;
	int sym;
	int v;
	while(*pos){
		pos = nv_risc_getSym(pos, &op, &v);
		if (op == RISC_NULL){
			break;
		}
		pos = nv_risc_getSym(pos, &sym, &a);
		pos = nv_risc_getSym(pos, &sym, &b);
		pos = nv_risc_getSym(pos, &sym, &c);
		mem[i] = nv_risc_code(op, a,b,c);
		++i;
	} 

	free(text);
	return i;
}
