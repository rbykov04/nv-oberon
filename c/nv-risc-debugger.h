// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-risc-debugger.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_RISC_DEBUGGER_H
#define NV_RISC_DEBUGGER_H
int nv_risc_debugger_init(nv_risc_t *t);


int nv_risc_debugger_resolve(nv_risc_t *t);

int nv_risc_debugger_before(
		nv_risc_t *t, 
		int OP,
		int a,
		int b,
		int c);
extern int risc_vm_debug;
int nv_risc_debugger_console(const char *text);
#endif //NV_RISC_DEBUGGER_H
