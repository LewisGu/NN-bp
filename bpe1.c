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
 * $Id: bpe1.c,v 1.5 2012/01/03 07:15:45 kobayasi Exp $
 */

#include <stdio.h>
#include "neuro.h"
#include "param_config.h"

int main(int argc, char *argv[])
{
    struct BPparam bp_param;
    BPNet bp;
    int	 layer,*num,patterns,int_tmp,pat,last_layer;
    int	 i,j,itelate;
    double min_err,exec_err,all_error,*in,*out,slope;
    double max_element;
    int  max_index,ret;
    Data *data;
    char inputdata[129];
    char weightdata[129];
    
    if (argc==2){
	ret = param_config(argv[1], &bp_param);
	if (ret){
	    fprintf(stderr,"ファイル %s からの読み込みに失敗しました．\n",
		    argv[1]);
	    fprintf(stderr,"実行を中止します．\n");
	    exit(1);
	}
	data = Dmalloc(bp_param.patterns);
    }
    else {
	fprintf(stderr,"\n");
	fprintf(stderr,"ネットワークの層の数   => ");scanf("%d",&bp_param.layers);
	bp_param.units = intmalloc(bp_param.layers);

	fprintf(stderr,"入力パターン総数       => ");scanf("%d",&bp_param.patterns);
	data = Dmalloc(bp_param.patterns);

	for( i = 0; i < bp_param.layers; i++ ) {
	    fprintf(stderr,"第 %2d 層目のユニット数 => ",i+1);
	    scanf("%d",&int_tmp);
	    bp_param.units[i] = int_tmp;
	}
	
	bp_param.inputdata = (char *)malloc(INIT_BUFFER_SIZE);
	fprintf(stderr,"入力データファイル名   => ");scanf("%s",bp_param.inputdata);
	bp_param.weightdata = (char *)malloc(INIT_BUFFER_SIZE);
	fprintf(stderr,"ウェイト保存ファイル名 => ");scanf("%s",bp_param.weightdata);
	fprintf(stderr,"シグモイド関数の傾き   => ");scanf("%lf",&bp_param.slope);
    }
    
    fprintf(stderr,"層の数                 => %d\n", bp_param.layers);
    fprintf(stderr,"学習パターン数         => %d\n", bp_param.patterns);
    for (i=0;i<bp_param.layers;i++)
	fprintf(stderr,"第%2d層のユニット数     => %d\n", i+1, bp_param.units[i]);
    fprintf(stderr,"入力データファイル     => %s\n", bp_param.inputdata);
    fprintf(stderr,"ウェイト保存ファイル   => %s\n", bp_param.weightdata);
    
    NetCreate(&bp,bp_param.units,bp_param.layers);
    fprintf(stderr,"ネットワーク構造決定\n");
    if ( NetWeightLoad(&bp,bp_param.weightdata) ){
	fprintf(stderr,"ウェイトロードに失敗しました。\n");
	exit( 2 );
    }
    fprintf(stderr,"ネットワーク結合初期化完了\n");
    
    if ( NetDataInitRecog(&bp,data,bp_param.patterns,bp_param.inputdata) ){
	fprintf(stderr,"データロードに失敗しました。\n");
	exit( 1 );
    }
    fprintf(stderr,"データロード終了\n");
	
    fprintf(stderr,"実行開始\n");
    for(int_tmp=0;int_tmp<bp_param.patterns;int_tmp++){
	NetOutput(&bp,data[int_tmp],bp_param.slope);
	in = NetExtOutval(&bp,1);
	out = NetExtOutval(&bp,bp_param.layers);
	for( i = 0; i < bp_param.units[0]; i++ )
	    fprintf(stderr,"%.6lf,", in[i]);
	last_layer = bp_param.units[bp_param.layers-1];
	for( i = 0; i < last_layer; i++ ){
	    fprintf(stderr,"%.6lf", out[i]);
	    if (i!=last_layer-1) fprintf(stderr,",");
	}
	fprintf(stderr,"\n");
    }
    fprintf(stderr,"プログラム終了\n");
    return 0;
}

