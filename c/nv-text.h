//SPDX-License-Identifier: GPL-3.0-or-later
/* nv-text.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_TEXT_H
#define NV_TEXT_H

#include <stddef.h>

typedef struct {
	const char *str;
	const char *start_line;
	const char *end;
	size_t pos;
	size_t line;
	int eot;
} nv_reader_t;

typedef struct {
	char *start;
	char *pos;
	char *end;
} nv_writer_t;

typedef struct {
	int (*read)(nv_reader_t *r, char *ch);
	size_t (*pos)(nv_reader_t *r);
	size_t (*line)(nv_reader_t *r);
	int (*write)(nv_writer_t *r, const char *str);
	int (*appendTo)(nv_writer_t *r, int fd);
} nv_texts_t;

int nv_readerInit(nv_reader_t *r, const char *text);
int nv_writerInit(nv_writer_t *r, char *text, size_t len);

extern nv_texts_t nv_Texts;

#endif //NV_TEXT_H
