// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-text.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_TEXT_H
#define NV_TEXT_H

typedef struct {
	const char *str;
	const char *start_line;
	const char *end;
	size_t pos;
	size_t line;
	int eot;
} nv_reader_t;

typedef struct {
	int (*read)(nv_reader_t *r, char *ch);
	size_t (*pos)(nv_reader_t *r);
	size_t (*line)(nv_reader_t *r);
} nv_texts_t;

int nv_readerInit(nv_reader_t *r, const char *text);

extern nv_texts_t nv_Texts;

#endif //NV_TEXT_H
