// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-risc.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_RISC_H
#define NV_RISC_H
#include <stddef.h>

#define RISC_MEM_SIZE 4096
#define RISC_PROG_ORG 2048
enum{
	RISC_MOV = 0,
	RISC_MVN = 1,
	RISC_ADD = 2,
	RISC_SUB = 3,
	RISC_MUL = 4,
	RISC_DIV = 5,
	RISC_MOD = 6,
	RISC_CMP = 7,
	RISC_MOVI = 16,
	RISC_MVNI = 17,
	RISC_ADDI = 18,
	RISC_SUBI = 19,
	RISC_MULI = 20,
	RISC_DIVI = 21,
	RISC_MODI = 22,
	RISC_CMPI = 23,
	RISC_CHKI = 24,
	RISC_LDW = 32,
	RISC_LDB = 33,
	RISC_POP = 34,
	RISC_STW = 36,
	RISC_STB = 37,
	RISC_PSH = 38,
	RISC_RD = 40,
	RISC_WRD = 41,
	RISC_WRH = 42,
	RISC_WRL = 43,
	RISC_BEQ = 48,
	RISC_BNE = 49,
	RISC_BLT = 50,
	RISC_BGE = 51,
	RISC_BLE = 52,
	RISC_BGT = 53,
	RISC_BR  = 56,
	RISC_BSR = 57,
	RISC_RET = 58,
	RISC_NULL = 100,
	RISC_VAL = 100,
};
typedef struct {
	unsigned int IR;
	int N;
	int Z;
	int R[16];
	int M[RISC_MEM_SIZE/4];
} nv_risc_t;
int nv_ricsReadFile(const char *filename, int *mem, size_t len);
int nv_risc_execute(nv_risc_t *risc, int start);
int nv_risc_load(nv_risc_t *risc, int *code, size_t len);

#endif //NV_RISC_H
