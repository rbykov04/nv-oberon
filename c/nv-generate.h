// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-generate.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_GENERATE_H
#define NV_GENERATE_H

#include "nv-sym-table.h"
enum{
	CLASS_HEAD = 0,
	CLASS_VAR = 1,
	CLASS_PAR= 2,
	CLASS_CONST= 3,
	CLASS_FLD = 4,
	CLASS_TYPE = 5,
	CLASS_PROC = 6,
	CLASS_SPROC = 7,
	CLASS_REG = 8,
	CLASS_COND = 9,

};

extern nv_type_t nv_BoolType;
extern nv_type_t nv_IntType;
#endif //NV_GENERATE_H
