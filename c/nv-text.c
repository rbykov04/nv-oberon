// SPDX-License-Identifier: GPL-3.0-or-later 
/* nv-text.c
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nv-text.h"

int nv_readerInit(nv_reader_t *r, const char *text){
	r->str = text;
	r->start_line = text;
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
int nv_writerInit(nv_writer_t *w, char *text, size_t len){
	w->start = text;
	w->end = text + len;
	w->pos = w->start;
}

int nv_textAppend(nv_writer_t *w, int fd){
	write(fd, w->start, w->pos - w->start);
	w->pos = w->start;
}
int nv_textWrite(nv_writer_t *w, const char *str){
	w->pos += snprintf(w->pos, w->end - w->pos, "%s", str);
}

nv_texts_t nv_Texts = {
	.read=nv_textRead,
	.pos=nv_textPos,
	.line=nv_textLine,
	.write=nv_textWrite,
	.appendTo=nv_textAppend,
};
