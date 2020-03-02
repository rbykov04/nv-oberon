// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-lex-key-table.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_LEX_KEY_TABLE_H
#define NV_LEX_KEY_TABLE_H

#include "nv-compiler.h"

typedef struct {
	const char *name;
	int id;
} nv_keytab_t;

int nv_find_lex(nv_keytab_t *table, const char *test);
const char *nv_find_lex_name(nv_keytab_t *table, int i);

#endif //NV_LEX_KEY_TABLE_H
