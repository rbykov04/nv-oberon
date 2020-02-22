// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-risc.c
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
int _mem[1024];


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

const char *nv_risc_getSym(const char *pos, int *sym, int *val){
	while(*pos <=' ' && *pos) pos++;
	switch (*pos){
		case '0'...'9':
			*sym = RISC_VAL;
			return nv_risc_number(pos, val);
		break;
		case 'W':
			*sym = RISC_WRD;
			pos+=3;
		break;
		case 'A':
			*sym = RISC_ADD;
			pos+=3;
		break;
		default: *sym = RISC_NULL; break; 
	}
	return pos;
}	


unsigned int nv_risc_code(int op, int a, int b, int c){
	printf("r put %d %d %d %d", op, a, b, c);
	unsigned int res = op;
	res <<= 4;
	res +=a;
	res <<= 4;
	res +=b;
	res <<= 18;
	res +=c % 0x40000;
	return res;
}
int nv_ricsReadFile(const char *filename, unsigned int *mem, size_t len){
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
int nv_risc_execute(nv_risc_t *risc, int start){
	risc->R[14] = 0;
	risc->R[15] = start + RISC_PROG_ORG;
	unsigned int nxt = 0;
	unsigned int opc = 0;
	unsigned int a = 0;
	unsigned int b = 0;
	unsigned int c = 0;
	while(1){
		nxt = risc->R[15] + 4;
		risc->IR = risc->M[risc->R[15]/4];
		opc = risc->IR >> 26;
		a = (risc->IR >> 22) % 0x10;
		b = (risc->IR >> 18) % 0x10;
		c = risc->IR % 0x40000;
		switch(opc){
			case RISC_WRD: printf("%u\n", risc->R[c]); break;
			case RISC_ADD: risc->R[a] = risc->R[b] + c; break;
			default: printf("error: unknow opc %u, IR = %u a =%u b=%u c=%u\n", opc, risc->IR, a, b, c); exit(1); break;
		}
		risc->R[15] = nxt;
	}

	return 0;
}

int nv_risc_load(nv_risc_t *risc, unsigned int *code, size_t len){
	size_t i;
	for (i=0; i< len; ++i){
		risc->M[i + RISC_PROG_ORG/4] = code[i];
	}
	return 0;
}
