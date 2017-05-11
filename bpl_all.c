/*
 * Copyright (c) 1994, 1999, 2000
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
 * THIS SOFTWARE IS PROVIDED BY FACULTY AND KANSAI UNIVERSITY ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL FACULTY AND KANSAI UNIVERSITY BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * $Id: bpl_all.c,v 1.2 2011/12/21 01:12:50 kobayasi Exp $
 */

#include <stdio.h>
#include "neuro.h"
#include <signal.h>

/* signal() 用に Global Variable にする */
BPNet bp, bp_dec, bp_inc;
char weightdata[129];

void
NetSigTerm(int dummy)
{
    fprintf(stderr,"\n*Receive termination signal* ... ");
    NetWeightSave(&bp,weightdata);
    fprintf(stderr,"End\n");
    exit(15);
}

int
main(void)
{
    int	 layer,*num,pattern,int_tmp;
    int	 i,j,z,iterate,learnfailure ;
    double min_err,exec_err,all_error,prev_error,prev_undo_error;
    Data *data;
    char inputdata[129],teachdata[129];
    char errordata[129];
    FILE *fp_err;
    double eta,alpha,slope;
    double init_eta, prev_eta;

    fprintf(stderr,"\n");
    fprintf(stderr,"ネットワークの層の数   => ");scanf("%d",&layer);
    fprintf(stderr,"%d\n",layer);
    num = intmalloc(layer);

    fprintf(stderr,"学習パターン数         => ");scanf("%d",&pattern);
    fprintf(stderr,"%d\n",pattern);
    data = Dmalloc(pattern);
    
    for( i = 0; i < layer; i++ ) {
	fprintf(stderr,"第 %2d 層目のユニット数 => ",i+1);
	scanf("%d",&int_tmp);
	num[i] = int_tmp;
	fprintf(stderr,"%d\n",num[i]);
    }

    fprintf(stderr,"入力データファイル名   => ");scanf("%s",inputdata);
    fprintf(stderr,"%s\n",inputdata);
    fprintf(stderr,"教師データファイル名   => ");scanf("%s",teachdata);
    fprintf(stderr,"%s\n",teachdata);
    fprintf(stderr,"ウェイト保存ファイル名 => ");scanf("%s",weightdata);
    fprintf(stderr,"%s\n",weightdata);
    fprintf(stderr,"学習誤差保存ファイル名 => ");scanf("%s",errordata);
    fprintf(stderr,"%s\n",errordata);

    fprintf(stderr,"学習回数(整数値)       => ");scanf("%d",&iterate);
    fprintf(stderr,"%d\n",iterate);
    fprintf(stderr,"許容誤差(実数値)       => ");scanf("%lf",&min_err);
    fprintf(stderr,"%lf\n",min_err);
    fprintf(stderr,"学習係数(実数値)       => ");scanf("%lf",&eta);
    fprintf(stderr,"%lf\n",eta); init_eta = eta;
    fprintf(stderr,"慣性係数(実数値)       => ");scanf("%lf",&alpha);
    fprintf(stderr,"%lf\n",alpha);
    fprintf(stderr,"シグモイド関数の傾き   => ");scanf("%lf",&slope);
    fprintf(stderr,"%lf\n",slope);
	
    /* ネットワーク構造の設定 */
    NetCreate(&bp,num,layer);
    fprintf(stderr,"ネットワーク構造決定\n");
    NetWeightInit(&bp);
    fprintf(stderr,"ネットワーク結合初期化完了\n");
	
    /* ネットワークに与えるデータの初期化 */
    if ( NetDataInit2(&bp,data,pattern,inputdata,teachdata) ){
	fprintf(stderr,"データロードに失敗しました。\n");
	exit( 1 );
    }
    fprintf(stderr,"データロード終了\n");
	
    signal(SIGINT,NetSigTerm);	/* この位置がミソ */
    signal(SIGTERM,NetSigTerm);	/* ネットワークの構築前だと */
    signal(SIGKILL,NetSigTerm);	/* 実行時におかしくなる */
	
    i = 0; learnfailure=0;
    prev_error = 10.0E3;
    fp_err = fopen(errordata,"w");
    for( all_error = 0.0,j = 0; j < pattern; j++ ){
	NetOutput(&bp,data[j],slope);
	exec_err = NetSqError(&bp,data[j]);
	all_error += exec_err;
    }
    all_error /= (double)pattern;
    do {
	NetLearnAll(&bp,data,pattern,eta,alpha,slope);
	i++;
	for( all_error = 0.0, j = 0; j < pattern; j++ ){
	    NetOutput(&bp,data[j],slope);
	    exec_err = NetSqError(&bp,data[j]);
	    all_error += exec_err;
	}
	all_error /= (double)pattern;
	fprintf(fp_err,"%6d%20.10lE%20.10lf\n", i, all_error, eta);
	fflush(fp_err);
	fprintf(stderr,"%6d%20.10lE%20.10lf\r", i, all_error, eta);
	fflush(stderr);
	
	if ( all_error < prev_error ){
	    if (eta<init_eta*10.0){
		eta += init_eta/100.0;
	    }
	}
	else {
	    fprintf(stderr,"\n"); z = 0;
	    while( all_error > prev_error ){
		NetLearnUndo(&bp);
		eta -= init_eta/100.0;
		if (eta<=0.0){
		    learnfailure = 1;
		    break;
		}
		z++;
		NetLearnAll(&bp,data,pattern,eta,alpha,slope);
		for( all_error = 0.0, j = 0; j < pattern; j++ ){
		    NetOutput(&bp,data[j],slope);
		    exec_err = NetSqError(&bp,data[j]);
		    all_error =+ exec_err;
		}
		all_error /= (double)pattern;
		fprintf(stderr,"%6d%20.10lE%20.10lE%20.10lf\r",
			z, prev_error,all_error,eta);
	    }		    
	    fprintf(stderr,"\n");
	}
	prev_error = all_error;
    }while(learnfailure==0 && i < iterate && all_error > min_err);
    fclose(fp_err);
    NetWeightSave(&bp,weightdata);
    
    return 0;
}
