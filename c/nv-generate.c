// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-generate.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#include "nv-generate.h"

nv_type_t nv_BoolType ={
	.size = 1
};

nv_type_t nv_IntType = {
	.size = 2
};


int nv_make_item(nv_compiler_t *cmpl, nv_item_t *x, nv_object_t *y){

	x->mode = y->class;
	x->type = y->type;
	x->a =y->val;
	x->r = 0;
	return 0;
}
