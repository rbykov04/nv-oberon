// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-risc-debugger.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#include <ncurses.h>

#include "nv-risc.h"
#include "nv-risc-asm.h" 
int nv_risc_debugger_init(nv_risc_t *t){
	initscr();
	
}

int nv_risc_debugger_resolve(nv_risc_t *t){
    getch();
    endwin();
}

int nv_risc_debugger_before(
		nv_risc_t *t, 
		int OP,
		int a,
		int b,
		int c){
	
	//;w
	
	const char *code = nv_find_lex_name(nv_RiscCode, OP);
	printw("%s%5d%5d%5d\n",code, a, b, c);
	refresh();
	
}

