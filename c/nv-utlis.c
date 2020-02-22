// SPDX-License-Identifier: GPL-3.0-or-later
/* nv-utils.c
 *
 *
 * Copyright (C) Ruslan Bykov rbykov04@gmail.com 2020
 *
 */
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "nv-utils.h"

char *nv_readFile(const char *filename){
	int fd = open(filename, O_RDONLY);
	if (fd == -1){
		perror("open");
		return NULL;
	}
	struct stat st;
	fstat(fd, &st);
	char *buf = malloc(st.st_size + 1);

	//to do: add save signal
	int res = read(fd, buf, st.st_size);
	if (res < 0){
		free(buf);
		buf = NULL;
	}else{
		buf[res + 1] = 0;
	}
	return buf;
}
