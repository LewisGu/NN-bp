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
 * $Id: net_str.h,v 1.1.1.1 2001/03/28 03:07:35 kobayasi Exp $
 */

#ifndef NET_STR_H
#define	NET_STR_H

typedef struct bp_pe {	  /* 処理要素構造体 */
    double OutVal;        /* 出力値 */
    double Variance;      /* 出力の分散値 */
    double Mean;          /* 出力の平均値 */
    double InVal;         /* 処理要素への入力総和値 */
    double ErrVal;        /* 処理要素の誤差信号値 */
    double ThreVal;       /* 処理要素の閾値 */
    double dThreVal;      /* 処理要素の閾値の修正量 */
    double *WRVal;        /* 処理要素の重みへのポインタ */
    double *dWRVal;       /* 処理要素の重みの修正量へのポインタ */
} Pe;

typedef struct bp_data {  /* データ構造体 */
    double *Input;        /* 入力ベクトルへのポインタ */
    double *Teach;        /* 教師ベクトルへのポインタ */
} Data;

typedef struct bp_net {
    struct bp_pe **pe; /* 処理要素構造体へのポインタへのポインタ */
    long layer;        /* ネットワークの層の数 */
    int *pe_num;       /* ネットワークの各層の処理要素の数 */
} BPNet;

typedef Pe **	PePP;      /* 処理要素構造体へのポインタへのポインタ */
typedef Pe *	PeP;       /* 処理要素構造体へのポインタ */
typedef Data *	DataP;     /* データ構造体へのポインタ */

#ifdef WORDS_BIGENDIAN
#define BP_64_SWAP(a) {                                             \
        u_int64_t _tmp = a;                                         \
        ((char *)&a)[0] = ((char *)&_tmp)[7];                       \
        ((char *)&a)[1] = ((char *)&_tmp)[6];                       \
        ((char *)&a)[2] = ((char *)&_tmp)[5];                       \
        ((char *)&a)[3] = ((char *)&_tmp)[4];                       \
        ((char *)&a)[4] = ((char *)&_tmp)[3];                       \
        ((char *)&a)[5] = ((char *)&_tmp)[2];                       \
        ((char *)&a)[6] = ((char *)&_tmp)[1];                       \
        ((char *)&a)[7] = ((char *)&_tmp)[0];                       \
}

#define BP_32_SWAP(a) {                                             \
        u_int32_t _tmp = a;                                         \
        ((char *)&a)[0] = ((char *)&_tmp)[3];                       \
        ((char *)&a)[1] = ((char *)&_tmp)[2];                       \
        ((char *)&a)[2] = ((char *)&_tmp)[1];                       \
        ((char *)&a)[3] = ((char *)&_tmp)[0];                       \
}
#else   /* ! WORDS_BIGENDIAN */
#define BP_64_SWAP(a) {                                             \
	;                                                           \
}

#define BP_32_SWAP(a) {                                             \
	;                                                           \
}

#endif  /* WORDS_BIGENDIAN */

#endif	/* !NET_STR_H */
