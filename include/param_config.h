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
 * $Id: param_config.h,v 1.1 2003/04/07 04:40:49 kobayasi Exp $
 */

#ifndef __PARAM_CONFIG__
#define __PARAM_CONFIG__

enum { NOCASE=0, CASE=1 };
enum { NOARRAY=0, ARRAY=1 };
enum { INIT_BUFFER_SIZE=1024 };

struct param_str {
    char directive[1024];
    int  dir_length;
    int  dir_case_flag;
};

struct BPparam {
    int layers;
    int patterns;
    int *units;
    char *inputdata;
    char *teachdata;
    char *weightdata;
    char *errordata;
    char *resultdata;
    long itelate;
    double min_err;
    double eta;
    double alpha;
    double slope;
};

int param_config(char *, struct BPparam *);

#endif
