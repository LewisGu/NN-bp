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
 * $Id: bpl.c,v 1.3 2011/12/21 01:12:50 kobayasi Exp $
 */

#include <stdio.h>
#include <signal.h>
#include "neuro.h"
#include "param_config.h"

/* signal() 用に Global Variable にする */
BPNet bp;
char weightdata[129];

void
NetSigTerm(int dummy)
{
    fprintf(stderr,"\n＊＊＊＊＊中断シグナルを受け取りました！！＊＊＊＊＊\n");
    fprintf(stderr,"結合係数を保存しています...");
    NetWeightSave(&bp,weightdata);
    fprintf(stderr,"\r結合係数の保存が完了しました\n");
    exit(15);
}

int main(int argc, char *argv[])
{
    struct BPparam bp_param;
    int	 layer,*num,patterns,int_tmp;
    int	 i,j,itelate,ret;
    double min_err,exec_err,all_error;
    Data *data;
    char inputdata[129],teachdata[129];
    char errordata[129];
    FILE *fp_err;
    double eta,alpha,slope;
    
    if (argc==2){ /* ２つ目の引数は設定ファイル */
	ret = param_config(argv[1], &bp_param);
	if (ret){
	    fprintf(stderr,"ファイル %s からの読み込みにエラーが発生しました．\n",
		    argv[1]);
	    fprintf(stderr,"実行を中止します．\n");
	    exit(1);
	}
	data = Dmalloc(bp_param.patterns);
	strncpy(weightdata, bp_param.weightdata, strlen(bp_param.weightdata));
    }
    else {
	fprintf(stderr,"\n");
	fprintf(stderr,"ネットワークの層の数   => ");scanf("%d",&bp_param.layers);
	bp_param.units = intmalloc(bp_param.layers);

	fprintf(stderr,"学習パターン数         => ");scanf("%d",&bp_param.patterns);
	data = Dmalloc(bp_param.patterns);

	for( i = 0; i < bp_param.layers; i++ ) {
	    fprintf(stderr,"第 %2d 層目のユニット数 => ",i+1);
	    scanf("%d",&int_tmp);
	    bp_param.units[i] = int_tmp;
	}
    
	bp_param.inputdata = (char *)malloc(INIT_BUFFER_SIZE);
	fprintf(stderr,"入力データファイル名   => ");scanf("%s",bp_param.inputdata);
	bp_param.teachdata = (char *)malloc(INIT_BUFFER_SIZE);
	fprintf(stderr,"教師データファイル名   => ");scanf("%s",bp_param.teachdata);
	bp_param.weightdata = (char *)malloc(INIT_BUFFER_SIZE);
	fprintf(stderr,"ウェイト保存ファイル名 => ");scanf("%s",bp_param.weightdata);
	strncpy(weightdata, bp_param.weightdata, strlen(bp_param.weightdata));
	bp_param.errordata = (char *)malloc(INIT_BUFFER_SIZE);
	fprintf(stderr,"学習誤差保存ファイル名 => ");scanf("%s",bp_param.errordata);

	fprintf(stderr,"学習回数(整数値)       => ");scanf("%d",&bp_param.itelate);
	fprintf(stderr,"許容誤差(実数値)       => ");scanf("%lf",&bp_param.min_err);
	fprintf(stderr,"学習係数(実数値)       => ");scanf("%lf",&bp_param.eta);
	fprintf(stderr,"慣性係数(実数値)       => ");scanf("%lf",&bp_param.alpha);
	fprintf(stderr,"シグモイド関数の傾き   => ");scanf("%lf",&bp_param.slope);
    }
    
    fprintf(stderr,"層の数                 => %d\n", bp_param.layers);
    fprintf(stderr,"学習パターン数         => %d\n", bp_param.patterns);
    for (i=0;i<bp_param.layers;i++)
	fprintf(stderr,"第%2d層のユニット数     => %d\n", i+1, bp_param.units[i]);
    fprintf(stderr,"教師入力データファイル => %s\n", bp_param.inputdata);
    fprintf(stderr,"教師出力データファイル => %s\n", bp_param.teachdata);
    fprintf(stderr,"ウェイト保存ファイル   => %s\n", bp_param.weightdata);
    fprintf(stderr,"学習誤差保存ファイル   => %s\n", bp_param.errordata);
    fprintf(stderr,"学習回数               => %d\n", bp_param.itelate);
    fprintf(stderr,"許容誤差               => %lf\n", bp_param.min_err);
    fprintf(stderr,"学習係数               => %lf\n", bp_param.eta);
    fprintf(stderr,"慣性係数               => %lf\n", bp_param.alpha);
    fprintf(stderr,"シグモイド関数の傾き   => %lf\n", bp_param.slope);
    
    /* ネットワーク構造の設定 */
    NetCreate(&bp,bp_param.units,bp_param.layers);
    fprintf(stderr,"ネットワーク構造決定\n");
    NetWeightInit(&bp);
    fprintf(stderr,"ネットワーク結合初期化完了\n");
	
    /* ネットワークに与えるデータの初期化 */
    if ( NetDataInit2(&bp,data,bp_param.patterns,bp_param.inputdata,bp_param.teachdata) ){
	fprintf(stderr,"データロードに失敗しました。\n");
	exit( 1 );
    }
    fprintf(stderr,"データロード終了\n");
    
    signal(SIGINT,NetSigTerm);	/* この位置がミソ */
    signal(SIGTERM,NetSigTerm);	/* ネットワークの構築前だと */
    signal(SIGKILL,NetSigTerm);	/* 実行時におかしくなる */
    
    eta = bp_param.eta;
    alpha = bp_param.alpha;
    slope = bp_param.slope;
    min_err = bp_param.min_err;
    itelate = bp_param.itelate;
    patterns = bp_param.patterns;
    
    i = 1;
    fp_err = fopen(bp_param.errordata,"w");
    do {
	for( all_error = 0.0,j = 0; j < patterns; j++ ){
	    NetOutput(&bp,data[j],slope);
	    exec_err = NetSqError(&bp,data[j]);
	    NetLearn(&bp,data[j],eta,alpha,slope);
	    all_error += exec_err;
	}
	all_error /= (double)patterns;
#ifndef BINMODE
	fprintf(fp_err,"%6d%25.15lE\n",i, all_error);
	fflush(fp_err);
#else
	fwrite(&i,sizeof(long),1,fp_err);
	fwrite(&all_error,sizeof(double),1,fp_err);
	fflush(fp_err);
#endif
	i += 1;
    }while( all_error >= min_err && i < itelate);
    fclose(fp_err);
    NetWeightSave(&bp,weightdata);
    
    return 0;
}
