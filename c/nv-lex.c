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
int nv_mark(nv_compiler_t *cmpl, const char *text){

}
int nv_comment(nv_compiler_t *cmpl){

}
int nv_ident(nv_compiler_t *cmpl){

}
int nv_number(nv_compiler_t *cmpl){

}

nv_keytab_t oberon_keywords[] = {
	{"BY", LEX_NULL},
	{"DO", LEX_DO},
	{"IF", LEX_IF},
	{"IN", LEX_NULL},
	{"IS", LEX_NULL},
	{"OF", LEX_OF},
	{"TO", LEX_OR},
	{"END", LEX_END},
	{"FOR", LEX_NULL},
	{"MOD", LEX_MOD},
	{"VAR", LEX_VAR},
	{"CASE", LEX_NULL},
	{"ELSE", LEX_ELSE},
	{"EXIT", LEX_NULL},
	{"THEN", LEX_THEN},
	{"TYPE", LEX_TYPE},
	{"WITH", LEX_NULL},
	{"ARRAY", LEX_ARRAY},
	{"BEGIN", LEX_BEGIN},
	{"CONST", LEX_CONST},
	{"ELSIF", LEX_ELSIF},
	{"IMPORT", LEX_NULL},
	{"UNTIL", LEX_NULL},
	{"WHILE", LEX_WHILE},
	{"RECORD", LEX_RECORD},
	{"RETURN", LEX_NULL},
	{"POINTER", LEX_NULL},
	{"PROCEDURE", LEX_PROCEDURE},
	{"DIV", LEX_DIV},
	{"LOOP", LEX_NULL},
	{"MODULE", LEX_MODULE},
	{NULL, LEX_NULL},
};
int nv_getSym(nv_compiler_t *cmpl){
	int i = 0;
	while (!cmpl->R->eot && cmpl->ch <= ' ') nv_Texts.read(cmpl->R, &cmpl->ch);
	switch (cmpl->ch){
		case '&':	cmpl->sym = LEX_AND; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '*':	cmpl->sym = LEX_TIMES; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '+':	cmpl->sym = LEX_PLUS; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '-':	cmpl->sym = LEX_MINUS; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '=':	cmpl->sym = LEX_EQL; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '#':	cmpl->sym = LEX_EQL; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '<':
			nv_Texts.read(cmpl->R, &cmpl->ch);
			if (cmpl->ch == '='){
				cmpl->sym = LEX_LEQ;
				nv_Texts.read(cmpl->R, &cmpl->ch);
			}else{
				cmpl->sym = LEX_LSS;
			}
		break;
		case '>':
			nv_Texts.read(cmpl->R, &cmpl->ch);
			if (cmpl->ch == '='){
				cmpl->sym = LEX_GEQ;
				nv_Texts.read(cmpl->R, &cmpl->ch);
			}else{
				cmpl->sym = LEX_GTR;
			}
		break;
		case ';':	cmpl->sym = LEX_SEMICOLON; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case ',':	cmpl->sym = LEX_COMMA; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case ':':
			nv_Texts.read(cmpl->R, &cmpl->ch);
			if (cmpl->ch == '='){
				cmpl->sym = LEX_BECOMES;
				nv_Texts.read(cmpl->R, &cmpl->ch);
			}else{
				cmpl->sym = LEX_COLON;
			}
		break;
		case '.':	cmpl->sym = LEX_PERIOD; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '(':
			nv_Texts.read(cmpl->R, &cmpl->ch);
			if (cmpl->ch == '*'){
				nv_comment(cmpl);
				nv_getSym(cmpl);
			}else{
				cmpl->sym = LEX_LPAREN;
			}
		break;
		case ')':	cmpl->sym = LEX_RPAREN; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '[':	cmpl->sym = LEX_LBRAK; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case ']':	cmpl->sym = LEX_RBRAK; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '0'...'9':	nv_number(cmpl); break;
		case 'A'...'Z':
		case 'a'...'z':	nv_ident(cmpl); break;
		case '~':	cmpl->sym = LEX_NOT; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		default: 	cmpl->sym = LEX_NULL; nv_Texts.read(cmpl->R, &cmpl->ch);
	}
	return 0;
}

nv_lexer_t OSS;