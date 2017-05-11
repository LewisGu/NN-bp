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
 * $Id: param_config.c,v 1.3 2003/04/07 05:42:29 kobayasi Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "param_config.h"
#include "read_lines.h"

int param_config (char *filename, struct BPparam *bp_param)
{
    FILE *fp;
    char *linebuf, *punct, *direct, *contents, *cntnt_ptr;
    int  linecount, line_length, i, prm_size, good_directive;
    int  content_length;
    struct param_str *prm_ptr;
    long longval;
    double dblval;
    int (*cmpfunc)(const char *, const char *, size_t);


    fp = fopen(filename, "r");
    if (fp==NULL){
	fprintf(stderr, "File %s is not able to open.\n", filename);
	return 1;
    }

    memset(bp_param, 0, sizeof(struct BPparam));

    linecount = 0;
    while(!feof(fp)){
	linebuf = read_nextline(fp);
	if (linebuf==NULL && feof(fp)){
	    break;
	}
	else if (linebuf==NULL){
	    fprintf(stderr, "Illegal line in %s.\n", filename);
	    exit(1);
	}
	line_length = strlen(linebuf);

	punct = strchr(linebuf, ':');
	if (!punct){
	    printf("Format error. : %s\n", linebuf);
	    exit(1);
	}
	linecount++;
	direct = linebuf;
	contents = punct; contents--;
	while(isspace(*contents)){ contents--; }
	contents++; *contents='\0';

	contents = punct; *contents++;
	while(isspace(*contents++)){ ; }
	contents--;

	if (!strncasecmp(direct, "layers", 6)){
	    longval = strtol(contents, NULL, 10);
	    if (longval==0){
		fprintf(stderr, "Illegal contents: %s\n", contents);
		exit(1);
	    }
	    bp_param->layers  = longval;
	}
	else if (!strncasecmp(direct, "patterns", 8)){
	    longval = strtol(contents, NULL, 10);
	    if (longval==0){
		fprintf(stderr, "Illegal contents: %s\n", contents);
		exit(1);
	    }
	    bp_param->patterns  = longval;
	}
	else if (!strncasecmp(direct, "units", 5)){
	    if (bp_param->layers!=0){
		if (bp_param->units){ free(bp_param->units); }
		bp_param->units = (int *)malloc(bp_param->layers * sizeof(int));
		cntnt_ptr = contents;
		i = 0;
		while(*cntnt_ptr!='\0'){
		    longval = strtol(cntnt_ptr, NULL, 10);
		    printf("%ld\n", longval);
		    if (longval==0){
			fprintf(stderr, "Illegal contents: %s\n", contents);
			exit(1);
		    }
		    bp_param->units[i] = longval;
		    while(!isspace(*cntnt_ptr)) cntnt_ptr++;
		    i++;
		    if (bp_param->layers<i){
			fprintf(stderr, "Mismatch layers(%ld) and units parameter.\n ignoring rest units parameters(%s).\n", bp_param->layers, contents);
			break;
		    }
		    while(isspace(*cntnt_ptr)&&*cntnt_ptr!='\0'){ cntnt_ptr++; }
		}
		if (bp_param->layers>i){
		    fprintf(stderr, "too few argument of units number: %s\n", contents);
		    exit(1);
		}
	    }
	    else {
		fprintf(stderr, "Please set the directive, \"layers\", first.\n");
		exit(1);
	    }
	}
	else if (!strncasecmp(direct, "inputdata", 9)){
	    content_length = strlen(contents)+1;
	    if (bp_param->inputdata){ free(bp_param->inputdata); }
	    bp_param->inputdata = malloc(content_length);
	    strncpy(bp_param->inputdata, contents, content_length);
	}
	else if (!strncasecmp(direct, "teachdata", 9)){
	    content_length = strlen(contents)+1;
	    if (bp_param->teachdata){ free(bp_param->teachdata); }
	    bp_param->teachdata = malloc(content_length);
	    strncpy(bp_param->teachdata, contents, content_length);
	}
	else if (!strncasecmp(direct, "weightdata", 10)){
	    content_length = strlen(contents)+1;
	    if (bp_param->weightdata){ free(bp_param->weightdata); }
	    bp_param->weightdata = malloc(content_length);
	    strncpy(bp_param->weightdata, contents, content_length);
	}
	else if (!strncasecmp(direct, "errordata", 9)){
	    content_length = strlen(contents)+1;
	    if (bp_param->errordata){ free(bp_param->errordata); }
	    bp_param->errordata = malloc(content_length);
	    strncpy(bp_param->errordata, contents, content_length);
	}
	else if (!strncasecmp(direct, "resultdata", 10)){
	    content_length = strlen(contents)+1;
	    if (bp_param->resultdata){ free(bp_param->resultdata); }
	    bp_param->resultdata = malloc(content_length);
	    strncpy(bp_param->resultdata, contents, content_length);
	}
	else if (!strncasecmp(direct, "itelate", 7)){
	    longval = strtol(contents, NULL, 10);
	    if (longval==0){
		fprintf(stderr, "Illegal number: %s\n", contents);
		exit(1);
	    }
	    bp_param->itelate = longval;
	}
	else if (!strncasecmp(direct, "min_err", 7)){
	    dblval = strtod(contents, NULL);
	    if (dblval==0.0){
		fprintf(stderr, "Illegal number: %s\n", contents);
		exit(1);
	    }
	    bp_param->min_err = dblval;
	}
	else if (!strncasecmp(direct, "eta", 3)){
	    dblval = strtod(contents, NULL);
	    if (dblval==0.0){
		fprintf(stderr, "Illegal number: %s\n", contents);
		exit(1);
	    }
	    bp_param->eta = dblval;
	}
	else if (!strncasecmp(direct, "alpha", 5)){
	    dblval = strtod(contents, NULL);
	    if (dblval==0.0){
		fprintf(stderr, "Illegal number: %s\n", contents);
		exit(1);
	    }
	    bp_param->alpha = dblval;
	}
	else if (!strncasecmp(direct, "slope", 5)){
	    dblval = strtod(contents, NULL);
	    if (dblval==0.0){
		fprintf(stderr, "Illegal number: %s\n", contents);
		exit(1);
	    }
	    bp_param->slope = dblval;
	}
	else{
	    fprintf(stderr, "Unknown directive: %s\n", direct);
	    exit(0);
	}

	memset(linebuf, 0, line_length);
	free(linebuf);
    }

    fclose(fp);

    return 0;
}
