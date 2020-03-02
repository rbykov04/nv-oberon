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
#include <limits.h>

#include "nv-lex-key-table.h"
#include "nv-lex.h"
#include "nv-error.h"


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

int nv_comment(nv_compiler_t *cmpl){
	while(1) {
		while(1) {
			while (cmpl->ch == '('){
				nv_Texts.read(cmpl->R, &cmpl->ch);
				if (cmpl->ch == '*'){
					nv_comment(cmpl);
				}
			}
			if (cmpl->ch  == '*'){
				nv_Texts.read(cmpl->R, &cmpl->ch);
				break;
			}
			if (cmpl->R->eot){
				break;
			}
			nv_Texts.read(cmpl->R, &cmpl->ch);
		}
		if (cmpl->ch  == ')'){
			nv_Texts.read(cmpl->R, &cmpl->ch);
			break;
		}
		if (cmpl->R->eot){
			nv_mark(cmpl, "comment not end");
			break;
		}
	};
	return 0;
}

int nv_number(nv_compiler_t *cmpl){
	cmpl->val = 0;
	do{
		int d = cmpl->ch - '0';
		if ((INT_MAX - d)/10 > cmpl->val){
			cmpl->val = cmpl->val* 10 + d;
		}else{
			nv_mark(cmpl, "number is very big");
			cmpl->val = 0;
		}
		nv_Texts.read(cmpl->R, &cmpl->ch);
	}while('0' <= cmpl->ch && cmpl->ch <='9' && !cmpl->R->eot);
	cmpl->sym = LEX_NUMBER;
	return 0;
}
int nv_ident(nv_compiler_t *cmpl){
	int i = 0;
	do{
		cmpl->id[i] = cmpl->ch;
		i++;
		nv_Texts.read(cmpl->R, &cmpl->ch);
	}while(i < IDLEN && !cmpl->R->eot &&
		   ('A' <= cmpl->ch && cmpl->ch <='Z')|| 
		   ('a' <= cmpl->ch && cmpl->ch <='z')|| 
		   ('0' <= cmpl->ch && cmpl->ch <='9')
	);
	cmpl->id[i] = 0;
	cmpl->sym = nv_find_lex(oberon_keywords, cmpl->id);
	if (cmpl->sym == LEX_NULL){
		cmpl->sym = LEX_IDENT;
	}
	return 0;
}
int nv_getSym(nv_compiler_t *cmpl){
	int i = 0;
	while (!cmpl->R->eot && cmpl->ch <= ' ') nv_Texts.read(cmpl->R, &cmpl->ch);
	switch (cmpl->ch){
		case '&':	cmpl->sym = LEX_AND; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '*':	cmpl->sym = LEX_TIMES; nv_Texts.read(cmpl->R, &cmpl->ch); break;
		case '/':	cmpl->sym = LEX_DIV; nv_Texts.read(cmpl->R, &cmpl->ch); break;
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
