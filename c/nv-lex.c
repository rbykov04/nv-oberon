// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-lex.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nv-lex.h"
#include "nv-error.h"

int nv_getSym(nv_compiler_t *cmpl){
	int i = 0;
	while (!cmpl->R->eot && cmpl->ch <= ' ') nv_Texts.read(cmpl->R, &cmpl->ch);
	switch (cmpl->ch){
		case 'A' ...'Z': 
		case 'a' ... 'z':
			cmpl->sym = LEX_IDENT;
			while (
					i < IDLEN &&
					(
						('A'<= cmpl->ch && cmpl->ch <= 'Z')	||
						('a'<= cmpl->ch && cmpl->ch <= 'z') ||
						('0' <= cmpl->ch &&  cmpl->ch <= '9')
					)
			){
				cmpl->id[i] = cmpl->ch;
				i++;
				nv_Texts.read(cmpl->R, &cmpl->ch);
			}
			cmpl->id[i] = 0;
		break;
		case '"':
			cmpl->sym = LEX_LITERA;
			nv_Texts.read(cmpl->R, &cmpl->ch);
			while(cmpl->ch >= ' ' && cmpl->ch != '"' && i < IDLEN){
				cmpl->id[i] = cmpl->ch;
				i++;
				nv_Texts.read(cmpl->R, &cmpl->ch);
			}
			if (cmpl->ch < ' '){
				nv_error(cmpl, 1);
			}
			cmpl->id[i] = cmpl->ch;
			nv_Texts.read(cmpl->R, &cmpl->ch);
		break;
		case '=':	cmpl->sym = LEX_EQL; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '(':	cmpl->sym = LEX_LPAREN; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case ')':	cmpl->sym = LEX_RPAREN; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '[':	cmpl->sym = LEX_LBRAK; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case ']':	cmpl->sym = LEX_RBRAK; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '{':	cmpl->sym = LEX_LBRACE; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '}':	cmpl->sym = LEX_RBRACE; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '|':	cmpl->sym = LEX_BAR; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '.':	cmpl->sym = LEX_PERIOD; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		default: 	cmpl->sym = LEX_OTHER; nv_Texts.read(cmpl->R, &cmpl->ch);
	}
	return 0;
}