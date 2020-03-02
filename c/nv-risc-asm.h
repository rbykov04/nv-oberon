// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-risc-asm.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_RISC_ASM_H
#define NV_RISC_ASM_H

#include <stddef.h>
#include "nv-lex-key-table.h"
const char * nv_risc_number(const char *pos, int *val);
const char *nv_risc_getSym(const char *pos, int *sym, int *val);
int nv_risc_code(int op, int a, int b, int c);
int nv_ricsReadFile(const char *filename, int *mem, size_t len);

extern nv_keytab_t nv_RiscCode[];
#endif //NV_RISC_ASM_H
