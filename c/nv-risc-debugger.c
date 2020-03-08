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

int risc_vm_debug;
typedef struct{
	WINDOW *console;
	WINDOW *commands;
	WINDOW *registers;
} nv_risc_debugger_t;

static nv_risc_debugger_t deb;
WINDOW *create_newwin(int h, int w, int y, int x){
	WINDOW *win = newwin(h, w, y, x);

	box(win, 0, 0);
	wrefresh(win);
	return win;
}
int nv_risc_debugger_init(nv_risc_t *t){
	if (!risc_vm_debug) return 0;
	initscr();
	deb.commands = create_newwin(30, 80, 0, 0); 
	deb.console = create_newwin(10, 80, 31, 0); 
	deb.registers = create_newwin(80, 80, 0, 91); 

	
}

int nv_risc_debugger_resolve(nv_risc_t *t){
	if (!risc_vm_debug) return 0;
    getch();
	if (deb.console){
		wrefresh(deb.console);
		delwin(deb.console);
	}
	if (deb.commands){
		wrefresh(deb.commands);
		delwin(deb.commands);
	}
	if (deb.registers){
		wrefresh(deb.registers);
		delwin(deb.registers);
	}
    endwin();
}

int nv_risc_debugger_before(
		nv_risc_t *t, 
		int OP,
		int a,
		int b,
		int c){
	
	if (!risc_vm_debug) return 0;
	//;w
	
	const char *code = nv_find_lex_name(nv_RiscCode, OP);
	wprintw(deb.commands, "%s%5d%5d%5d\n",code, a, b, c);
	wrefresh(deb.commands);
	werase(deb.registers);
	wprintw(deb.registers, "REG| value\n");
	int i = 0;
	for (i=0; i<16; ++i){
		wprintw(deb.registers, "%3d%5d\n",i, t->R[i]);
	}
	wrefresh(deb.registers);
	refresh();
	
    //getch();
}

int nv_risc_debugger_console(const char *text){

	wprintw(deb.console, "%s",text);
	wrefresh(deb.console);
	refresh();
	return 0;
}
