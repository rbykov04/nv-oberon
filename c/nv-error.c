// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-error.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "nv-utils.h"
#include "nv-error.h"
#define NV_COLOR_ERROR "\033[1m\033[31m"
#define NV_COLOR_RESET "\033[0m"
void nv_error(nv_compiler_t *cmplr, int code){
	size_t pos = nv_Texts.pos(cmplr->R);
	if (1 || pos > cmplr->lastpos +4){ //fake error
		printf("%s:%ld:%ld err%4d symb %4d\n", cmplr->current_filename, nv_Texts.line(cmplr->R), pos, code, cmplr->sym);
	}
}

void nv_mark(nv_compiler_t *cmplr, const char *fmt, ...){
	size_t pos = nv_Texts.pos(cmplr->R) - 1;
	if (pos > cmplr->errpos){
		printf("%s:%ld:%ld"NV_COLOR_ERROR" error: "NV_COLOR_RESET, cmplr->current_filename,  nv_Texts.line(cmplr->R), pos);
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
		char *s = strchr(cmplr->R->start_line, '\n');
		if (!s){
			printf("\n%s\n", cmplr->R->start_line);
		}else{
			*s = '\0';
			printf("\n%s\n", cmplr->R->start_line);
			*s = '\n';
		}
		int i = 0;
		for (i =0; i< pos -1; ++i){ printf(" ");}
		printf(NV_COLOR_ERROR"^~\n"NV_COLOR_RESET);
	}
	cmplr->errpos = pos;
	cmplr->error = 1;
}
