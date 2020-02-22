// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-error.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_ERROR_H
#define NV_ERROR_H

#include "nv-compiler.h"

void nv_error(nv_compiler_t *cmplr, int code);
void nv_mark(nv_compiler_t *cmplr, const char *text);
#endif //NV_ERROR_H