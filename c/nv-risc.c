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
#include "nv-risc-asm.h"
#include "nv-utils.h"
#include "nv-risc-debugger.h"


int nv_ash(int v, int n){
	return v<<n;
}
int nv_risc_console(const char *text){
	if (risc_vm_debug) {
		nv_risc_debugger_console(text);
		return 0;
	}
	printf("%s",text);
}
int nv_risc_execute(nv_risc_t *risc, int start){
	risc->R[14] = 0;
	risc->R[15] = start + RISC_PROG_ORG;
	int nxt = 0;
	int opc = 0;
	int a = 0;
	int b = 0;
	int c = 0;
	nv_risc_debugger_init(risc);
	while(1){
		nxt = risc->R[15] + 4;
		risc->IR = risc->M[risc->R[15]/4];
		opc = risc->IR >> 26;
		a = (risc->IR >> 22) % 0x10;
		b = (risc->IR >> 18) % 0x10;
		c = risc->IR % 0x40000;

		if (opc < RISC_MOVI){ //F0
			c = risc->R[c];
		}
		nv_risc_debugger_before(risc, opc, a, b ,c);
		switch(opc){
			case RISC_MOVI:case RISC_MOV: risc->R[a] = nv_ash(c,b); break;
			case RISC_ADDI:case RISC_ADD: risc->R[a] = risc->R[b] + c;break;
			case RISC_SUBI:case RISC_SUB: risc->R[a] = risc->R[b] - c; break;
			case RISC_MULI:case RISC_MUL: risc->R[a] = risc->R[b] * c; break;
			case RISC_DIVI:case RISC_DIV: risc->R[a] = risc->R[b] / c; break;
			case RISC_MODI:case RISC_MOD: risc->R[a] = risc->R[b] % c; break;
			case RISC_CMPI:case RISC_CMP:
				risc->Z = (risc->R[b] == c);
				risc->N = (risc->R[b] < c); 
			break;
			case RISC_CHKI:
				if(risc->R[a] < 0 || risc->R[a] >=c){
					risc->R[a] = 0;
				}
			break;
			case RISC_LDW:
				//if (risc_vm_debug) //printf("LDW addr = %d, c= %d get %d\n",(risc->R[b]+c) /4, c, risc->M[(risc->R[b]+c)/4]);
				risc->R[a] = risc->M[(risc->R[b]+c)/4];	break;
			case RISC_LDB: /*no impl*/ break;
			case RISC_POP:
				risc->R[a] = risc->M[risc->R[b] /4];
				risc->R[b] += c;
			break;
			case RISC_STW:
				// if (risc_vm_debug) //printf("STW addr = %d ,c =%d; IR=%d set %d\n",(risc->R[b]+c) /4, c, risc->IR, risc->R[a]);
				risc->M[(risc->R[b]+c) /4] = risc->R[a];
			break;
			case RISC_STB:/*no impl*/break;
			case RISC_PSH:
				risc->R[b] -= c;
				risc->M[risc->R[b] /4] = risc->R[a];
			break;
			case RISC_RD: scanf("%d", &risc->R[a]); break;
			case RISC_WRD: nv_risc_console(strf(" %u", risc->R[c])); break;
			case RISC_WRH: nv_risc_console(strf(" 0x%x\n", risc->R[c])); break;
			case RISC_WRL: nv_risc_console(strf("\n")); break;
			case RISC_BEQ: if (risc->Z){ nxt = risc->R[15] + c*4;} break;
			case RISC_BNE: if (!risc->Z){nxt = risc->R[15] + c*4;} break;
			case RISC_BLT: if (risc->N){ nxt = risc->R[15] + c*4;} break;
			case RISC_BGE: if (!risc->N){ nxt = risc->R[15] + c*4;} break;
			case RISC_BLE: 
				if (risc->N || risc->Z){ 
					nxt = risc->R[15] + c*4;
				}
			break;
			case RISC_BGT: 
				if (!risc->Z && !risc->N){ 
					nxt = risc->R[15] + c*4;
				}
			break;
			case RISC_BR: nxt = risc->R[15] + c*4; break;
			case RISC_BSR: 
				nxt = risc->R[15] + c*4;
				risc->R[14] = risc->R[15]+4;
			break;
			case RISC_RET:
				goto end__;
				return 0;
				nxt = risc->R[c % 0x10];
				if (nxt == 0){
					return 0;
				} 
			break;
			default: //printf("error: unknow opc %u, IR = %u a =%u b=%u c=%u\n", opc, risc->IR, a, b, c);
				goto end__;
			 break;
		}
		risc->R[15] = nxt;
	}
end__:
	nv_risc_debugger_resolve(risc);
	return 0;
}

int nv_risc_code_to_asm(nv_writer_t *w, int *code, size_t len){
	size_t i;
	for (i=0; i< len; ++i){
		unsigned int IR = (unsigned int) code[i];
		int opc = IR >> 26;
		int a = (IR >> 22) % 0x10;
		int b = (IR >> 18) % 0x10;
		int c = IR % 0x40000;
		const char *name = nv_find_lex_name(nv_RiscCode, opc);
		nv_Texts.write(w, strf("%4s%5d%5d%5d\n", name, a, b, c));

	}
	return 0;
}
int nv_risc_load(nv_risc_t *risc, int *code, size_t len){
	size_t i;
	for (i=0; i< len; ++i){
		//printf("op %d\n", code[i]);
		risc->M[i + RISC_PROG_ORG/4] = code[i];
	}
	return 0;
}

int nv_rics_put(int *mem, int op, int a, int b, int c){
	*mem = nv_risc_code(op, a,b,c);
	return 0;
}
