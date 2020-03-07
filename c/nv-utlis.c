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
#include <stdarg.h>

#include "nv-utils.h"

int nv_writeFile(const char *filename, nv_writer_t *w){
	int fd = open(filename, O_CREAT | O_WRONLY, 0666);
	if (fd == -1){
		perror("open");
		return -1;
	}
	return nv_Texts.appendTo(w, fd);
}
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
	close(fd);
	return buf;
}

const char *strf(const char *format, ...){
	static char buf[8][256];
	static int index = 0;
	char *str = buf[index++ & 7];
	va_list ap;
	va_start(ap, format);
	vsnprintf(str, sizeof buf[0], format, ap);
	va_end(ap);
	return str;
}

const char *nv_changeSuffix(const char *filename, const char *new_suffix){
	char prefix[100] = {0};

	size_t j = 0;
	size_t i = 0;
	for (i = 0; filename[i] && filename[i] != '.'; ++i, ++j){
		prefix[j] = filename[i];
	}
	while (filename[i]){
		i++;
		if (filename[i] == '.'){
			while(i != j){
				prefix[j] = filename[i];
				j++;
			}
		}
	}
	
	return strf("%s%s", prefix, new_suffix);

}
