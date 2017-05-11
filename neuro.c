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
 * $Id: neuro.c,v 1.2 2003/04/07 04:40:49 kobayasi Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

#include <math.h>
#include <sys/types.h>
/*
#include <varargs.h>
*/
#include <time.h>
#include "net_str.h"
#include "allocate.h"
#include "random.h"

/* static int need_swap_byte_order; */

#ifdef WORDS_BIGENDIAN
union LDBL {
    double dval;
    unsigned char  cval[8];
    unsigned short sval[4];
    unsigned long  lval[2];
};

long BYTESWAP_LONG(long a)
{
    union LDBL ldbl;
    unsigned char cval[SIZEOF_LONG];
    int i;

    ldbl.lval[0] = a;
    for(i=0;i<SIZEOF_LONG;i++)
        cval[i] = ldbl.cval[SIZEOF_LONG-1-i];
    for(i=0;i<SIZEOF_LONG;i++)
        ldbl.cval[i] = cval[i];

    return ldbl.lval[0];
}

double BYTESWAP_DOUBLE(double a)
{
    union LDBL ldbl;
    unsigned char cval[8];

    ldbl.dval = a;
    cval[0] = ldbl.cval[7];
    cval[1] = ldbl.cval[6];
    cval[2] = ldbl.cval[5];
    cval[3] = ldbl.cval[4];
    cval[4] = ldbl.cval[3];
    cval[5] = ldbl.cval[2];
    cval[6] = ldbl.cval[1];
    cval[7] = ldbl.cval[0];
    ldbl.cval[0] = cval[0];
    ldbl.cval[1] = cval[1];
    ldbl.cval[2] = cval[2];
    ldbl.cval[3] = cval[3];
    ldbl.cval[4] = cval[4];
    ldbl.cval[5] = cval[5];
    ldbl.cval[6] = cval[6];
    ldbl.cval[7] = cval[7];

    return ldbl.dval;
}
#else
#define BYTESWAP_LONG(a)   (a)
#define BYTESWAP_DOUBLE(a) (a)
#endif

Pe **
Peppmalloc(int num)
{
    Pe **pe;
    
    if ((pe=(Pe **)calloc((unsigned)num,sizeof(Pe *)))!=NULL)
	return pe;
    else {
	printf("Memory allocation error(Pe *): size %d\n",
	       num*sizeof(Pe *));
	exit(1);
    }

    return NULL;
}

int *
intmalloc(int num)
{
    int *tmp;
    
    if ((tmp=(int *)calloc((unsigned)num,sizeof(int)))!=NULL)
	return tmp;
    else {
	printf("Memory allocation error(int): size %d\n",
	       num*sizeof(int));
	exit(1);
    }

    return NULL;
}

Pe *
Pepmalloc(int num)
{
    Pe *pe;
    
    if ((pe=(Pe *)calloc((unsigned)num,sizeof(Pe)))!=NULL)
	return pe;
    else {
	printf("Memory allocation error(Pe): size %d\n",
	       num*sizeof(Pe));
	exit(1);
    }

    return NULL;
}

Data *
Dmalloc(int num)
{
    Data *data;
    
    if ((data=(Data *)calloc((unsigned)num,sizeof(Data)))!=NULL)
	return data;
    else {
	printf("Memory allocation error(Data): sizeof %d\n",
	       num*sizeof(Data));
	exit(1);
    }

    return NULL;
}

double *
dmalloc(int num)
{
    double *tmp;
    
    if ((tmp=(double *)calloc((unsigned)num,sizeof(double)))!=NULL)
	return tmp;
    else {
	printf("Memory allocation error(double): size %d\n",
	       num*sizeof(double));
	exit(1);
    }

    return NULL;
}

Pe *
Peprealloc(Pe *pe,int num)
{
    Pe *tmp;
    
    if ((tmp=(Pe *)realloc(pe,num*sizeof(Pe)))!=NULL)
	return tmp;
    else{
	fprintf(stderr,"Memory re-allocation error(Pe).\n");
	exit(0);
    }

    return NULL;
}

double *
drealloc(double *d,int num)
{
    double *tmp;
    
    if ((tmp=(double *)realloc(d,num*sizeof(double)))!=NULL)
	return tmp;
    else {
	fprintf(stderr,"Memory re-allocation error(double).\n");
	exit(0);
    }

    return NULL;
}

int
NetDataInit(BPNet *bp,Data *data,int pat,char *filename)
{
    int i,j,layer,input,output;
    double buf;
    FILE *fp;

    layer  = bp->layer;
    input  = bp->pe_num[0];
    output = bp->pe_num[layer-1];
    
    if ((fp=fopen(filename,"r"))==NULL)	return(1);
    
    for(i=0;i<pat;i++){
	data[i].Input=dmalloc(input);
	data[i].Teach=dmalloc(output);
	if ((data[i].Input==NULL)||(data[i].Teach==NULL)){
	    fprintf(stderr,"Memory allocation error in NetDataInit.\n");
	    exit(0);
	}
	for(j=0;j<input;j++){
	    fscanf(fp,"%lf",&buf);
	    data[i].Input[j] = buf;
	}
	for(j=0;j<output;j++){
	    fscanf(fp,"%lf",&buf);
	    data[i].Teach[j] = buf;
	}
    }
    fclose(fp);
    return( 0 );
}

int
NetDataInit2(BPNet *bp,Data *data,int pat,char *infile,char *outfile)
{
    int i,j,layer,input,output;
    double buf;
    FILE *fin;
    FILE *fout;
    int retval;

    layer  = bp->layer;
    input  = bp->pe_num[0];
    output = bp->pe_num[layer-1];
	
    fin = fopen(infile,"r");
    fout= fopen(outfile,"r");
    if (fin==NULL){
	retval = 1;
	fclose(fin);
    }
    else if (fout==NULL){
	retval = 2;
	fclose(fout);
    }
    else {
	for(i=0;i<pat;i++){
	    data[i].Input=dmalloc(input);
	    data[i].Teach=dmalloc(output);
	    if ((data[i].Input==NULL)||(data[i].Teach==NULL)){
		fprintf(stderr,"Memory allocation error in NetDataInit2.\n");
		exit(0);
	    }
	    for(j=0;j<input;j++){
		fscanf(fin,"%lf",&buf);		data[i].Input[j] = buf;
	    }
	    for(j=0;j<output;j++){
		fscanf(fout,"%lf",&buf);	data[i].Teach[j] = buf;
	    }
	}
	fclose(fin);fclose(fout);
	retval = 0;
    }
    return retval;
}

int
NetDataInit3(BPNet *bp,Data *data,int pat,int in1_num,char *infile1,char *infile2,char *outfile)
{
    int i,j,retval,layer,input,output;
    double buf;
    FILE *fin1;
    FILE *fin2;
    FILE *fout;

    layer  = bp->layer;
    input  = bp->pe_num[0];
    output = bp->pe_num[layer-1];
    
    fin1 = fopen(infile1,"r");
    fin2 = fopen(infile2,"r");
    fout = fopen(outfile,"r");

    if (fin1==NULL){
	retval = 1;
	fclose(fin1);
    }
    else if (fin2==NULL){
	retval = 2;
	fclose(fin2);
    }
    else if (fout==NULL){
	retval = 3;
	fclose(fout);
    }
    else {
	for(i=0;i<pat;i++){
	    data[i].Input=dmalloc(input);
	    data[i].Teach=dmalloc(output);
	    if ((data[i].Input==NULL)||(data[i].Teach==NULL)){
		fprintf(stderr,"Memory allocation error in NetDataInit3.\n");
		exit(0);
	    }
	    for(j=0;j<in1_num;j++){
		fscanf(fin1,"%lf",&buf);	data[i].Input[j] = buf;
	    }
	    for(j=in1_num;j<input;j++){
		fscanf(fin2,"%lf",&buf);	data[i].Input[j] = buf;
	    }
	    for(j=0;j<output;j++){
		fscanf(fout,"%lf",&buf);	data[i].Teach[j] = buf;
	    }
	}
	fclose(fin1);fclose(fin2);fclose(fout);
	retval = 0;
    }
    return retval;;
}

int
NetDataInitRecog(BPNet *bp,Data *data,int pat,char *file)
{
    int i,j,retval,in_layer;
    FILE *input;
    double buf;

    in_layer = bp->pe_num[0];
    
    input = fopen(file,"r");
    if (input==NULL){
	retval = 1;
	fclose(input);
    }
    else {
	for(i=0;i<pat;i++){
	    data[i].Input=dmalloc(in_layer);
	    if (data[i].Input==NULL){
		fprintf(stderr,"Memory allocation error in NetDataInitRecog.\n");
		exit(0);
	    }
	    for(j=0;j<in_layer;j++){
		fscanf(input,"%lf",&buf);
		data[i].Input[j] = buf;
	    }
	}
	fclose(input);
	retval = 0;
    }
    return retval;
}

int
NetDataSuper2(Data *data,int input,int output,int pat,char *infile,char *outfile)
{
    int i,j,retval;
    double buf;
    FILE *fin;
    FILE *fout;
	
    fin = fopen(infile,"r");
    fout= fopen(outfile,"r");
    if (fin==NULL){
	retval = 1;
	fclose(fin);
    }
    else {
	for(i=0;i<pat;i++){
	    data[i].Input=dmalloc(input);
	    data[i].Teach=dmalloc(output);
	    if ((data[i].Input==NULL)||(data[i].Teach==NULL)){
		fprintf(stderr,"Memory allocation error in NetDataSuper2.\n");
		exit(0);
	    }
	    for(j=0;j<input;j++){
		fscanf(fin,"%lf",&buf);		data[i].Input[j] = buf;
	    }
	    for(j=0;j<output;j++){
		fscanf(fout,"%lf",&buf);	data[i].Teach[j] = buf;
	    }
	}
	fclose(fin);fclose(fout);
	retval = 0;
    }
    return retval;
}

double
sigmoid(double x,double slope)
{
/*
    if (x<-slope*2.944439)	return 0.05;
    else if (x>=-slope*2.944439 && x<=slope*2.944439)
	return((double)1.0/((double)1.0+exp(-x/slope)));
    else if (x>slope*2.944439)	return 0.95;
*/
    return((double)1.0/((double)1.0+exp(-x/slope)));
}

double
dsigmoid(double x,double slope)
{
    return(sigmoid(x,slope)*((double)1.0-sigmoid(x,slope))/slope);
}

void
NetCreate(BPNet *bp,int *number,int layer)
{
    int	i,j;
    long netlong, hostlong;
    
    bp->layer = layer;
    bp->pe = Peppmalloc(bp->layer);
    bp->pe_num = intmalloc(bp->layer);
	
    for(i=0;i<layer;i++)	bp->pe_num[i] = number[i];
    
    for(i=0;i<bp->layer;i++){
	bp->pe[i]=Pepmalloc(bp->pe_num[i]);
	if (bp->pe[i]==NULL){
	    fprintf(stderr,"Memory allocation error in NetCreate.\n");
	    exit(0);
	}
	if (i>0){
	    for(j=0;j<bp->pe_num[i];j++){
		bp->pe[i][j].WRVal=dmalloc(bp->pe_num[i-1]);
		bp->pe[i][j].dWRVal=dmalloc(bp->pe_num[i-1]);
		if ((bp->pe[i][j].WRVal==NULL)||(bp->pe[i][j].dWRVal==NULL)){
		    fprintf(stderr,"Memory allocation error in NetCreate.\n");
		    exit(0);
		}
	    }
	}
    }

/*  hostlong = 1L;
    netlong = htonl(hostlong);
    if (netlong != hostlong){
        need_swap_byte_order = 0;
    }
    else {
        need_swap_byte_order = 1;
    }
*/    

    return;
}

void
NetFree(Pe **pe,int *num,Data *data)
{
    free(pe);
    free(num);
    free(data);
    return;
}

void
NetOutput(BPNet *bp,Data data,double slope)
{
    register int i,j;
    int k;
    
    for(i=0;i<bp->pe_num[0];i++)
	bp->pe[0][i].OutVal = data.Input[i];
    for(i=0;i<bp->pe_num[bp->layer-1];i++)
	bp->pe[bp->layer-1][i].ErrVal = 0.0;
    for(k=1;k<bp->layer;k++){
	for(i=0;i<bp->pe_num[k];i++){
	    bp->pe[k][i].InVal=bp->pe[k][i].ThreVal;
	    for(j=0;j<bp->pe_num[k-1];j++)
		bp->pe[k][i].InVal+=(bp->pe[k][i].WRVal[j]*bp->pe[k-1][j].OutVal);
	    bp->pe[k][i].OutVal=sigmoid(bp->pe[k][i].InVal,slope);
	}
    }
    return;
}

int
NetVariance(BPNet *bp,Data *data,int pat,int layer_num,double slope)
{
    /* layer_num count for 1,2,...,N */
    register int i,j;
    int unit = 0;
    double *sigma,*sigma_sub,min;
    
    min=1.0e9;
    sigma=dmalloc(bp->pe_num[layer_num-1]);
    sigma_sub=dmalloc(bp->pe_num[layer_num-1]);
    for(j=0;j<bp->pe_num[layer_num-1];j++){
	bp->pe[layer_num-1][j].Variance = 0.0;
	sigma[j] = 0.0;
	sigma_sub[j] = 0.0;
    }
    for(i=0;i<pat;i++){
	NetOutput(bp,data[i],slope);
	for(j=0;j<bp->pe_num[layer_num-1];j++){
	    sigma[j] += pow(bp->pe[layer_num-1][j].OutVal,2.0);
	    sigma_sub[j] += bp->pe[layer_num-1][j].OutVal;
	}
    }
    for(j=0;j<bp->pe_num[layer_num-1];j++){
	sigma[j] /= (double)pat;
	sigma_sub[j] /= (double)pat;
	bp->pe[layer_num-1][j].Mean = sigma_sub[j];
	sigma_sub[j] = pow(sigma_sub[j],2.0);
	bp->pe[layer_num-1][j].Variance = sigma[j]-sigma_sub[j];
	if (bp->pe[layer_num-1][j].Variance<min){
	    min = bp->pe[layer_num-1][j].Variance;
	    unit = j+1;
	}
    }
    free(sigma);free(sigma_sub);
    return unit;
}

double
NetSqError(BPNet *bp,Data data)
{
    register int i,layer;
    double retVal;

    layer = bp->layer;
    for(retVal=0.0,i=0;i<bp->pe_num[layer-1];i++)
	retVal+=pow(data.Teach[i]-bp->pe[layer-1][i].OutVal,2.0)/2.0;
    retVal /= (double)(bp->pe_num[layer-1]);
    return retVal;
}

double *
NetExtOutval(BPNet *bp,int layer)
{
    register int i;
    double *data;

    if (layer < 1 || layer > bp->layer) return (double *)NULL;
    data = dmalloc(bp->pe_num[layer-1]);
    for(i=0;i<bp->pe_num[layer-1];i++)
	data[i] = bp->pe[layer-1][i].OutVal;
    return data;
}

void
NetLearn(BPNet *bp,Data data,double eta,double alpha,double slope)
{
    register int i,j;
    int k,layer;
    double net;

    layer = bp->layer;
    for(i=0;i<bp->pe_num[layer-1];i++)
	bp->pe[layer-1][i].ErrVal = (data.Teach[i] - bp->pe[layer-1][i].OutVal) * dsigmoid(bp->pe[layer-1][i].InVal,slope);
    for(i=bp->layer-2;i>0;i--){
	for(j=0;j<bp->pe_num[i];j++){
	    for(net=0.0,k=0;k<bp->pe_num[i+1];k++)
		net += bp->pe[i+1][k].ErrVal * bp->pe[i+1][k].WRVal[j];
	    bp->pe[i][j].ErrVal = net * dsigmoid(bp->pe[i][j].InVal,slope);
	}
    }
    for(i=1;i<bp->layer;i++){
	for(k=0;k<bp->pe_num[i];k++){
	    for(j=0;j<bp->pe_num[i-1];j++){
		bp->pe[i][k].dWRVal[j] = eta * bp->pe[i][k].ErrVal * bp->pe[i-1][j].OutVal + alpha * bp->pe[i][k].dWRVal[j];
		bp->pe[i][k].WRVal[j] += bp->pe[i][k].dWRVal[j];
	    }
	    bp->pe[i][k].dThreVal = eta * bp->pe[i][k].ErrVal + alpha * bp->pe[i][k].dThreVal;
	    bp->pe[i][k].ThreVal += bp->pe[i][k].dThreVal;
	}
    }
    return;
}

void
NetDuplicate(BPNet *dst, BPNet *src)
{
    register int i,j;
    int k,layer,*number=src->pe_num;

    layer = src->layer;
    NetCreate(dst,number,layer);
    
    for(i=1;i<layer;i++){
	for(k=0;k<number[i];k++){
	    for(j=0;j<number[i-1];j++){
		dst->pe[i][k].dWRVal[j] = src->pe[i][k].dWRVal[j];
		dst->pe[i][k].WRVal[j]  = src->pe[i][k].WRVal[j];
	    }
	    dst->pe[i][k].dThreVal = src->pe[i][k].dThreVal;
	    dst->pe[i][k].ThreVal  = src->pe[i][k].dThreVal;
	}
    }
    return;
}

void
NetLearnUndo(BPNet *bp)
{
    register int i,j;
    int k,layer;

    layer = bp->layer;
    for(i=1;i<layer;i++){
	for(k=0;k<bp->pe_num[i];k++){
	    for(j=0;j<bp->pe_num[i-1];j++){
		bp->pe[i][k].WRVal[j] -= bp->pe[i][k].dWRVal[j];
		bp->pe[i][k].dWRVal[j] = 0.0;
	    }
	    bp->pe[i][k].ThreVal -= bp->pe[i][k].dThreVal;
	    bp->pe[i][k].dThreVal = 0.0;
	}
    }
    return;
}

void
NetLearnAll(BPNet *bp,Data *data,int pattern,double eta,double alpha,double slope)
{
    register int i,j;
    int k,layer;
    double net;

    layer = bp->layer;
    for(i=0;i<bp->pe_num[layer-1];i++)
	bp->pe[layer-1][i].ErrVal=0.0;

    for(k=0;k<pattern;k++){
	NetOutput(bp,data[k],slope);
	for(i=0;i<bp->pe_num[layer-1];i++){
	    bp->pe[layer-1][i].ErrVal += 
		(data[k].Teach[i] - bp->pe[layer-1][i].OutVal) 
		* dsigmoid(bp->pe[layer-1][i].InVal,slope);
	}
    }
    for(i=0;i<bp->pe_num[layer-1];i++){
	bp->pe[layer-1][i].ErrVal /= (double)pattern;
    }
    
    for(i=bp->layer-2;i>0;i--){
	for(j=0;j<bp->pe_num[i];j++){
	    for(net=0.0,k=0;k<bp->pe_num[i+1];k++)
		net += bp->pe[i+1][k].ErrVal * bp->pe[i+1][k].WRVal[j];
	    bp->pe[i][j].ErrVal = net * dsigmoid(bp->pe[i][j].InVal,slope);
	}
    }
    for(i=1;i<bp->layer;i++){
	for(k=0;k<bp->pe_num[i];k++){
	    for(j=0;j<bp->pe_num[i-1];j++){
		bp->pe[i][k].dWRVal[j] = eta * bp->pe[i][k].ErrVal * bp->pe[i-1][j].OutVal + alpha * bp->pe[i][k].dWRVal[j];
		bp->pe[i][k].WRVal[j] += bp->pe[i][k].dWRVal[j];
	    }
	    bp->pe[i][k].dThreVal = eta * bp->pe[i][k].ErrVal + alpha * bp->pe[i][k].dThreVal;
	    bp->pe[i][k].ThreVal += bp->pe[i][k].dThreVal;
	}
    }
    return;
}
void
NetReset(BPNet *bp,int layer)
{
    int i,j;

    if (layer<2 ||layer>bp->layer)	return;
    
    for(i=0;i<bp->pe_num[layer-1];i++){
	bp->pe[layer-1][i].ThreVal = drand_0303();
	bp->pe[layer-1][i].dThreVal = (double)0.0;
	for(j=0;j<bp->pe_num[layer-2];j++){
	    bp->pe[layer-1][i].WRVal[j] = drand_0303();
	    bp->pe[layer-1][i].dWRVal[j] = (double)0.0;
	}
    }
    return;
}

void
NetPeDel1(BPNet *bp,int l,int p)
{
    /* layer_num count for 1,2,...,N
       pe_num count for 1,2,...,N		*/
    register int i;
	
    bp->pe[l-1][p-1] = bp->pe[l-1][bp->pe_num[l-1]-1];
    bp->pe[l-1]=Peprealloc(bp->pe[l-1],bp->pe_num[l-1]-1);
    for(i=0;i<bp->pe_num[l];i++){
	bp->pe[l][i].ThreVal += bp->pe[l][i].WRVal[p-1]*bp->pe[l-1][p-1].Mean;
	
	bp->pe[l][i].WRVal[p-1]  = bp->pe[l][i].WRVal[bp->pe_num[l]-1];
	bp->pe[l][i].dWRVal[p-1] = bp->pe[l][i].dWRVal[bp->pe_num[l]-1];
	
	bp->pe[l][i].WRVal  = drealloc(bp->pe[l][i].WRVal ,bp->pe_num[l-1]-1);
	bp->pe[l][i].dWRVal = drealloc(bp->pe[l][i].dWRVal,bp->pe_num[l-1]-1);
    }
    bp->pe_num[l-1] -= 1;
    return;
}

void
NetPeAdd1(BPNet *bp,int l)
{
    /* layer_num count for 1,2,...,N */
    register int i;
    double *tmpw,*tmpdw;
    time_t nowtime;

    srand((unsigned)time(&nowtime));
    bp->pe[l-1]=Peprealloc(bp->pe[l-1],bp->pe_num[l-1]+1);
    tmpw  = dmalloc(bp->pe_num[l-2]);
    tmpdw = dmalloc(bp->pe_num[l-2]);
    for(i=0;i<bp->pe_num[l-2];i++){
	tmpw[i] = drand_0303();
	tmpdw[i] = 0.0;
    }
    bp->pe[l-1][bp->pe_num[l-1]].ThreVal  = drand_0303();
    bp->pe[l-1][bp->pe_num[l-1]].dThreVal = 0.0;
    bp->pe[l-1][bp->pe_num[l-1]].WRVal    = tmpw;
    bp->pe[l-1][bp->pe_num[l-1]].dWRVal   = tmpdw;
    for(i=0;i<bp->pe_num[l];i++){
	bp->pe[l][i].WRVal  = drealloc(bp->pe[l][i].WRVal, bp->pe_num[l-1]+1);
	bp->pe[l][i].dWRVal = drealloc(bp->pe[l][i].dWRVal,bp->pe_num[l-1]+1);
	bp->pe[l][i].WRVal[bp->pe_num[l-1]]  = drand_0303();
	bp->pe[l][i].dWRVal[bp->pe_num[l-1]] = 0.0;
    }
    bp->pe_num[l-1]+=1;
    return;
}

void
NetResetLayerPe(BPNet *bp,int l,int p)
{
    register int i;
    time_t nowtime;
    
    srand((unsigned)time(&nowtime));
    for(i=0;i<bp->pe_num[l];i++)
	bp->pe[l][i].ThreVal+=bp->pe[l-1][p-1].Mean*bp->pe[l][i].WRVal[p-1];
    for(i=0;i<bp->pe_num[l];i++){
	bp->pe[l][i].WRVal[p-1] = drand_0303();
	bp->pe[l][i].dWRVal[p-1]= 0.0;
    }
    bp->pe[l-1][p-1].ThreVal = drand_0303();
    bp->pe[l-1][p-1].dThreVal= 0.0;
    for(i=0;i<bp->pe_num[l-2];i++){
	bp->pe[l-1][p-1].WRVal[i] = drand_0303();
	bp->pe[l-1][p-1].dWRVal[i]= 0.0;
    }
    return;
}

double
NetKullbackError(BPNet *bp,Data data)
{
    int i,layer;
    double retVal;
    double T,O;

    layer = bp->layer;
    retVal = 0.0;
    for(i=0;i<bp->pe_num[layer-1];i++){
	T = data.Teach[i];
	O = bp->pe[layer-1][i].OutVal;
	retVal = retVal + T*log(T/O) + (1.0-T)*log((1.0-T)/(1.0-O));
    }
    return(retVal/(double)bp->pe_num[bp->layer-1]);
}

void
NetStoreKullbackError(BPNet *bp,Data data)
{
    int i,layer;

    layer = bp->layer;
    for(i=0;i<bp->pe_num[layer-1];i++)
	bp->pe[layer-1][i].ErrVal += data.Teach[i] - bp->pe[layer-1][i].OutVal;
}

void
NetLearnHyper(BPNet *bp,Data data,double eta,double alpha,double slope)
{
    register int i,j;
    int k,layer;
    double net;

    layer = bp->layer;
    for(i=0;i<bp->pe_num[layer-1];i++)
	bp->pe[layer-1][i].ErrVal = data.Teach[i] - bp->pe[layer-1][i].OutVal;
    for(i=layer-2;i>0;i--){
	for(j=0;j<bp->pe_num[i];j++){
	    for(net=0.0,k=0;k<bp->pe_num[i+1];k++)
		net += bp->pe[i+1][k].ErrVal * bp->pe[i+1][k].WRVal[j];
	    bp->pe[i][j].ErrVal = net * dsigmoid(bp->pe[i][j].InVal,slope);
	}
    }
    for(i=1;i<layer;i++){
	for(k=0;k<bp->pe_num[i];k++){
	    for(j=0;j<bp->pe_num[i-1];j++){
		bp->pe[i][k].dWRVal[j] = eta * bp->pe[i][k].ErrVal * bp->pe[i-1][j].OutVal + alpha * bp->pe[i][k].dWRVal[j];
		bp->pe[i][k].WRVal[j] += bp->pe[i][k].dWRVal[j];
	    }
	    bp->pe[i][k].dThreVal = eta * bp->pe[i][k].ErrVal + alpha * bp->pe[i][k].dThreVal;
	    bp->pe[i][k].ThreVal += bp->pe[i][k].dThreVal;
	}
    }
    return;
}

void
NetStoredLearnHyper(BPNet *bp,long pattern,double eta,double alpha,double slope)
{
    register int i,j;
    int k,layer;
    double net;
    
    layer = bp->layer;
    for(i=0;i<bp->pe_num[layer-1];i++)
	bp->pe[layer-1][i].ErrVal /= (double)pattern;
    for(i=layer-2;i>0;i--){
	for(j=0;j<bp->pe_num[i];j++){
	    for(net=0.0,k=0;k<bp->pe_num[i+1];k++)
		net += bp->pe[i+1][k].ErrVal * bp->pe[i+1][k].WRVal[j];
	    bp->pe[i][j].ErrVal = net * dsigmoid(bp->pe[i][j].InVal,slope);
	}
    }
    for(i=1;i<layer;i++){
	for(k=0;k<bp->pe_num[i];k++){
	    for(j=0;j<bp->pe_num[i-1];j++){
		bp->pe[i][k].dWRVal[j] = eta * bp->pe[i][k].ErrVal * bp->pe[i-1][j].OutVal + alpha * bp->pe[i][k].dWRVal[j];
		bp->pe[i][k].WRVal[j] += bp->pe[i][k].dWRVal[j];
	    }
	    bp->pe[i][k].dThreVal = eta * bp->pe[i][k].ErrVal + alpha * bp->pe[i][k].dThreVal;
	    bp->pe[i][k].ThreVal += bp->pe[i][k].dThreVal;
	}
    }
    return;
}

void
NetLearnHyperLambda(BPNet *bp,Data data,
			 double eta,double alpha,double slope,long lt,long lm)
{
    register int i,j;
    int k,layer;
    double net;

    layer = bp->layer;
    for(i=0;i<bp->pe_num[layer-1];i++)
	bp->pe[layer-1][i].ErrVal = data.Teach[i] - bp->pe[layer-1][i].OutVal-0.0005*(1.0-(double)lt/(double)lm)*bp->pe[layer-1][i].InVal*dsigmoid(bp->pe[layer-1][i].InVal,slope);
    for(i=layer-2;i>0;i--){
	for(j=0;j<bp->pe_num[i];j++){
	    for(net=0.0,k=0;k<bp->pe_num[i+1];k++)
		net += bp->pe[i+1][k].ErrVal * bp->pe[i+1][k].WRVal[j];
	    bp->pe[i][j].ErrVal = net * dsigmoid(bp->pe[i][j].InVal,slope)-0.0005*(1.0-(double)lt/(double)lm)*bp->pe[i][j].InVal;
	}
    }
    for(i=1;i<layer;i++){
	for(k=0;k<bp->pe_num[i];k++){
	    for(j=0;j<bp->pe_num[i-1];j++){
		bp->pe[i][k].dWRVal[j] = eta * bp->pe[i][k].ErrVal * bp->pe[i-1][j].OutVal + alpha * bp->pe[i][k].dWRVal[j];
		bp->pe[i][k].WRVal[j] += bp->pe[i][k].dWRVal[j];
	    }
	    bp->pe[i][k].dThreVal = eta * bp->pe[i][k].ErrVal + alpha * bp->pe[i][k].dThreVal;
	    bp->pe[i][k].ThreVal += bp->pe[i][k].dThreVal;
	}
    }
    return;
}

double
drand_0303(void)
{
    double retval;
    int rand_val;

    rand_val = rand()%32767;
    retval = ((double)rand_val/32767.0-0.5)*0.6;
    return retval;
}

#define VERSION	(double)2.0

#ifndef  SEEK_SET
#define SEEK_SET 0
#endif

void
savenew_msg(void)
{
    fprintf( stderr, "このファイルは Neural Network の重みのファイルではありません。\n" );
    exit(15);
}

void
NetWeightInit(BPNet *bp)
{
    int i,j,k;
    time_t nowtime;
    
    srand((unsigned)time(&nowtime));
    for(i=1;i<bp->layer;i++){
	for(j=0;j<bp->pe_num[i];j++){
	    bp->pe[i][j].ThreVal = drand_0303();
	    bp->pe[i][j].dThreVal = 0.0;
	    for(k=0;k<bp->pe_num[i-1];k++){
		bp->pe[i][j].WRVal[k]  = drand_0303();
		bp->pe[i][j].dWRVal[k] = 0.0;
	    }
	}
    }
    return;
}

int
NetWeightLoad(BPNet *bp,char *file)
{
    register int i,j;
    int k;
    FILE *fp;
    double buf, version;
	
    fp=fopen(file,"rb");
    if (fp==NULL)	return(-1);
    rewind( fp );
    fseek( fp, 0x00, SEEK_SET );
    fread( &buf, 8, 1, fp );
    version = BYTESWAP_DOUBLE(buf);
    if ( version < VERSION ){	/* バージョンチェック */
	savenew_msg();
	fclose( fp );
	fp = fopen( file, "rt" );
	rewind( fp );
	for(i=1;i<bp->layer;i++){
	    for(j=0;j<bp->pe_num[i];j++){
		fscanf(fp,"%lf",&buf); bp->pe[i][j].ThreVal=buf;
		for(k=0;k<bp->pe_num[i-1];k++){
		    fscanf(fp,"%lf",&buf); bp->pe[i][j].WRVal[k]=buf;
		}
	    }
	}
    }	else {
	fseek( fp, 0x10, SEEK_SET );	/* ウェイトは0x10バイト目から */
	for(i=1;i<bp->layer;i++){
	    for(j=0;j<bp->pe_num[i];j++){
		fread( &buf, 8, 1, fp );
		bp->pe[i][j].ThreVal =  BYTESWAP_DOUBLE(buf);
		for(k=0;k<bp->pe_num[i-1];k++){
		    fread( &buf, 8, 1, fp );
		    bp->pe[i][j].WRVal[k] = BYTESWAP_DOUBLE(buf);
		}
	    }
	}
    }
    fclose( fp );
    return( 0 );
}

void
NetWeightSave(BPNet *bp,char *file)
{
    register int i,j;
    int k;
    FILE *fp;
    double version, buf=VERSION;
	
    fp=fopen(file,"wb");
    rewind( fp );
    version = BYTESWAP_DOUBLE(buf);
    fwrite( &version, 8, 1, fp );
    fseek( fp, 0x10, SEEK_SET );	/* ウェイトは0x10バイト目から */
    for(i=1;i<bp->layer;i++){
	for(j=0;j<bp->pe_num[i];j++){
	    buf = BYTESWAP_DOUBLE(bp->pe[i][j].ThreVal);
	    fwrite( &buf, 8, 1, fp );
	    for(k=0;k<bp->pe_num[i-1];k++){
		buf = BYTESWAP_DOUBLE(bp->pe[i][j].WRVal[k]);
		fwrite( &buf, 8, 1, fp );
	    }
	}
    }
    fclose(fp);
    return;
}

void
WeightBIN2TXT( char *src, char *dist )
{
    FILE *fp_src, *fp_dist;
    double buf, value;

    fp_src	= fopen( src, "rb" );
    fp_dist	= fopen( dist, "wt" );

    if ( fp_src == NULL ){
	fprintf(stderr, "%s is not found.\n", src );
	exit( 1 );
    }

    fseek( fp_src, 0x10, SEEK_SET );
    while( fread( &buf, 8, 1, fp_src ) ){
        value = BYTESWAP_DOUBLE(buf);
	fprintf( fp_dist, "%30.15lf\n", value );
    }
    fclose( fp_src ); fclose( fp_dist );
    return;
}

long
NetPassLoad(BPNet *bp,char *filename)
{
    register int i,j;
    int k;
    double buf, version;
    long learn, lbuf;
    FILE *fp;
	
    fp=fopen(filename,"rb");
    if (fp==NULL){
	fprintf(stderr,"Weight File %s is not found.\n",
		filename);
	exit(0);
    }
    rewind( fp );
    fread( &buf, 8, 1, fp );
    version = BYTESWAP_DOUBLE(buf);
    if ( version < VERSION ){	/* バージョンチェック */
	savenew_msg();
	fclose( fp );
	fp = fopen( filename, "rt" );
	rewind( fp );
	fscanf(fp,"%ld\n",&learn);
	for(i=1;i<bp->layer;i++){
	    for(j=0;j<bp->pe_num[i];j++){
		fscanf(fp,"%lf",&buf);bp->pe[i][j].ThreVal  = buf;
		fscanf(fp,"%lf",&buf);bp->pe[i][j].dThreVal = buf;
		for(k=0;k<bp->pe_num[i-1];k++){
		    fscanf(fp,"%lf\n",&buf);bp->pe[i][j].WRVal[k]  = buf;
		    fscanf(fp,"%lf\n",&buf);bp->pe[i][j].dWRVal[k] = buf;
		}
	    }
	}
    }	else {
	fseek( fp, 0x10, SEEK_SET );	/* 学習回数は0x10バイト目に */
	fread( &lbuf, SIZEOF_LONG, 1, fp );
	learn = BYTESWAP_LONG(lbuf);
	fseek( fp, 0x20, SEEK_SET );	/* ウェイトは0x20バイト目から */
	for(i=1;i<bp->layer;i++){
	    for(j=0;j<bp->pe_num[i];j++){
		fread( &buf, 8, 1, fp );
		bp->pe[i][j].ThreVal  = BYTESWAP_DOUBLE(buf);
		fread( &buf, 8, 1, fp );
		bp->pe[i][j].dThreVal = BYTESWAP_DOUBLE(buf);
		for(k=0;k<bp->pe_num[i-1];k++){
		    fread( &buf, 8, 1, fp );
		    bp->pe[i][j].WRVal[k]  = BYTESWAP_DOUBLE(buf);
		    fread( &buf, 8, 1, fp );
		    bp->pe[i][j].dWRVal[k] = BYTESWAP_DOUBLE(buf);
		}
	    }
	}
    }
    fclose(fp);
    return learn;
}

void
NetPassSave(BPNet *bp,int t,char *filename)
{
    register int i,j;
    int k;
    long learn, lbuf = t;
    FILE *fp;
    double version, buf=VERSION;
	
    fp=fopen(filename,"wb");	rewind( fp );
    version = BYTESWAP_DOUBLE(buf);
    fwrite( &version, 8, 1, fp );
    fseek( fp, 0x10, SEEK_SET );	/* 学習回数は0x10バイト目に */
    learn = BYTESWAP_LONG(lbuf);
    fwrite( &learn, SIZEOF_LONG, 1, fp );
    fseek( fp, 0x20, SEEK_SET );	/* ウェイトは0x20バイト目から */
    for(i=1;i<bp->layer;i++){
	for(j=0;j<bp->pe_num[i];j++){
	    buf = BYTESWAP_DOUBLE(bp->pe[i][j].ThreVal);
	    fwrite( &buf,  8, 1, fp );
	    buf = BYTESWAP_DOUBLE(bp->pe[i][j].dThreVal);
	    fwrite( &buf, 8, 1, fp );
	    for(k=0;k<bp->pe_num[i-1];k++){
		buf = BYTESWAP_DOUBLE(bp->pe[i][j].WRVal[k]);
		fwrite( &buf,  8, 1, fp );
		buf = BYTESWAP_DOUBLE(bp->pe[i][j].dWRVal[k]);
		fwrite( &buf, 8, 1, fp );
	    }
	}
    }
    fclose(fp);
    return;
}
 
void
WeightPBIN2TXT( char *src, char *dist )
{
    FILE *fp_src, *fp_dist;
    double buf, value;

    fp_src	= fopen( src, "rb" );
    fp_dist	= fopen( dist, "wt" );
    if ( fp_src == NULL ){
	fprintf(stderr, "%s is not found.\n", src );
	exit( 1 );
    }

    fseek( fp_src, 0x20, SEEK_SET );
    while( fread( &buf, 8, 1, fp_src ) ){
	value = BYTESWAP_DOUBLE(buf);
	fprintf( fp_dist, "%30.15lf\n", value );
    }
    fclose( fp_src ); fclose( fp_dist );
    return;
}

