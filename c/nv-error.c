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

#include "nv-error.h"

void nv_error(nv_compiler_t *cmplr, int code){
	size_t pos = nv_Texts.pos(cmplr->R);
	if (1 || pos > cmplr->lastpos +4){ //fake error
		printf(" pos%6ld err%4d symb %4d\n", pos, code, cmplr->sym);
	}
	exit(code);
}
