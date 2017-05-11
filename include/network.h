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
 * $Id: network.h,v 1.1.1.1 2001/03/28 03:07:35 kobayasi Exp $
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <math.h>
#include "net_str.h"

#ifdef __cplusplus
extern "C" {
#endif

extern	double sigmoid(double,double);
extern	double dsigmoid(double,double);
extern	void NetCreate(BPNet *bp,int *number,int layer);
extern	void NetFree(PePP,int *,DataP);
extern	void NetOutput(BPNet *bp,Data data,double slope);
extern	double NetSqError(BPNet *bp,Data data);
extern	double *NetExtOutval(BPNet *bp,int layer);
extern	void NetLearn(BPNet *bp,Data data,double eta,double alpha,double slope);
extern	void NetLearnAll(BPNet *bp,Data *data,int pattern,double eta,double alpha,double slope);
extern	void NetReset(BPNet *bp,int layer);
extern	int NetVariance(BPNet *bp,Data *data,int pattern,int layer,double);
extern	void NetPeDel1(BPNet *bp,int layer,int pe);
extern	void NetPeAdd1(BPNet *bp,int layer);
extern	void NetResetLayerPe(BPNet *bp,int layer,int pe);
extern  double NetKullbackError(BPNet *bp,Data data);
extern  void NetLearnHyper(BPNet *bp,Data data,double eta,double alpha,double slope);
extern  void NetLearnHyperLambda(BPNet *bp,Data data,double eta,double alpha,double slope,long cur_iter,long max_iter);

#ifdef __cplusplus
}
#endif

#endif	/* !NETWORK_H */
