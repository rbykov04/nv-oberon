// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-lex-key-table.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */

#include "nv-lex-key-table.h"

int nv_find_lex(nv_keytab_t *table, const char *test){
	int i =0;
	while(table[i].name){
		if (!strcmp(test, table[i].name)){
			break;
		}
		i++;
	}
	return table[i].id;
}
