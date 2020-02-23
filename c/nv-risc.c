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

int nv_ash(int v, int n){
	return v<<n;
}
int nv_risc_execute(nv_risc_t *risc, int start){
	risc->R[14] = 0;
	risc->R[15] = start + RISC_PROG_ORG;
	int nxt = 0;
	int opc = 0;
	int a = 0;
	int b = 0;
	int c = 0;
	while(1){
		nxt = risc->R[15] + 4;
		risc->IR = risc->M[risc->R[15]/4];
		opc = risc->IR >> 26;
		a = (risc->IR >> 22) % 0x10;
		b = (risc->IR >> 18) % 0x10;
		c = risc->IR % 0x40000;

		// if (opc < RISC_MOVI){ //F0
		// 	c = risc->R[risc->IR%0x10];
		// }else if (opc < RISC_BEQ){ //F1, F2
		// 	c = risc->IR%0x40000;
		// 	if (c >= 0x20000){
		// 		c -= 0x40000;
		// 	}
		// } else {
		// 	c = risc->IR%0x4000000;
		// 	if (c >= 0x2000000){
		// 		c -= 0x4000000;
		// 	}
		// }
		switch(opc){
			case RISC_MOVI:case RISC_MOV: risc->R[a] = nv_ash(c,b); break;
			case RISC_ADDI:case RISC_ADD: risc->R[a] = risc->R[b] + c; break;
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
			case RISC_LDW: risc->R[a] = risc->M[(risc->R[b]+c)/4];	break;
			case RISC_LDB: /*no impl*/ break;
			case RISC_POP:
				risc->R[a] = risc->M[risc->R[b] /4];
				risc->R[b] += c;
			break;
			case RISC_STW:
				risc->M[(risc->R[b]+c) /4] = risc->R[a];
			break;
			case RISC_STB:/*no impl*/break;
			case RISC_PSH:
				risc->R[b] -= c;
				risc->M[risc->R[b] /4] = risc->R[a];
			break;
			case RISC_RD: scanf("%d", &risc->R[a]); break;
			case RISC_WRD: printf(" %u\n", risc->R[c]); break;
			case RISC_WRH: printf(" 0x%x\n", risc->R[c]); break;
			case RISC_WRL: printf("\n"); break;
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
				nxt = risc->R[c % 0x10];
				if (nxt == 0){
					return 0;
				} 
			break;
			default: printf("error: unknow opc %u, IR = %u a =%u b=%u c=%u\n", opc, risc->IR, a, b, c); exit(1); break;
		}
		risc->R[15] = nxt;
	}

	return 0;
}

int nv_risc_load(nv_risc_t *risc, int *code, size_t len){
	size_t i;
	for (i=0; i< len; ++i){
		risc->M[i + RISC_PROG_ORG/4] = code[i];
	}
	return 0;
}
