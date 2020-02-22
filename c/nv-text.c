// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-text.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#include <stdlib.h>
#include <string.h>
#include "nv-text.h"

int nv_readerInit(nv_reader_t *r, const char *text){
	r->str = text;
	r->end = r->str + strlen(text) +1;
	r->pos = 0;
	r->eot = 0;
	return 0;
}

int nv_textRead(nv_reader_t *r, char *ch){
	if (r->eot){
		return -1;
	}
	*ch = *r->str;
	r->str++;
	if (*r->str == '\n'){
		r->line++;
		r->start_line = r->str + 1;
		r->pos = 0;
	}
	r->pos++;
	if (r->str == r->end){
		r->eot = 1;
	}
	return 0;
}

size_t nv_textLine(nv_reader_t *r){
	return r->line;
}
size_t nv_textPos(nv_reader_t *r){
	return r->pos;
}

nv_texts_t nv_Texts = {
	.read=nv_textRead,
	.pos=nv_textPos,
	.line=nv_textLine
};
