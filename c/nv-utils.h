// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-utils.h
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#ifndef NV_UTILS_H
#define NV_UTILS_H

#include "nv-text.h"

char *nv_readFile(const char *filename);
int nv_writeFile(const char *filename, nv_writer_t *w);
const char *strf(char const *format, ...);
const char *nv_changeSuffix(const char *filename, const char *new_suffix);

#endif //NV_UTILS_H
