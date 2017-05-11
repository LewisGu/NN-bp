/*
 * Copyright (c) 1994-2003
 *     Kansai University.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following coditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions of binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors nor he names of the university
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY KANSAI UNIVERSITY ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL KANSAI UNIVERSITY BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * $Id: read_lines.c,v 1.1 2003/04/07 04:40:49 kobayasi Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { INIT_BUFFER_LENGTH=1024 };

char *read_nextline(FILE *fp)
{
    char *linebuf, *ret_line, *ret;
    int bufsize, count, n_count;

    bufsize = INIT_BUFFER_LENGTH;
    linebuf = (char *)malloc(bufsize);
    ret_line= (char *)malloc(bufsize);
    memset(linebuf, 0, bufsize);
    memset(ret_line,0, bufsize);

    do {
	ret=fgets(linebuf, bufsize, fp);
    }	while(!feof(fp) && ret && (*linebuf=='#' || *linebuf==';' || *linebuf=='\n'));

    if (ret==NULL) return (char *)NULL;
    if (feof(fp))  return (char *)NULL;

    count=strlen(linebuf); *(linebuf+count) = '\0';
    count--;
    while(isspace(*(linebuf+count-1))){
	*(linebuf+count-1)='\0';
	count--;
    }

    strncpy(ret_line, linebuf, count);
/*  *(ret_line+count)='\0'; */

    if (*(ret_line+count-1)=='\\'){
	*(ret_line+count-1)='\0';
	count--;
	free(linebuf);
	linebuf = read_nextline(fp);
	if (linebuf==NULL){
	    fprintf(stderr, "Detect EOF!!! Invalid continuous line.\n");
	    exit(1);
	}
	n_count = strlen(linebuf);
	if (count+n_count>=bufsize){
	    ret_line = (char *)realloc(ret_line, count+n_count);
	}
	strncat(ret_line, linebuf, n_count);
    }

    if (linebuf) free(linebuf);
    return ret_line;
}
