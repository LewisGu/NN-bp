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
 * $Id: weight.h,v 1.1.1.1 2001/03/28 03:07:35 kobayasi Exp $
 */

#ifndef WEIGHT_H
#define	WEIGHT_H

#include "net_str.h"

#ifdef __cplusplus
extern "C" {
#endif
	
extern	void NetWeightInit(BPNet *bp);
extern	int	 NetWeightLoad(BPNet *bp,char *loadfile);
extern	void NetWeightSave(BPNet *bp,char *savefile);
extern	long NetPassLoad(BPNet *bp,char *loadfile);
extern	void NetPassSave(BPNet *bp,int iter,char *savefile);
extern  void WeightBIN2TXT( char *src, char *dist );
extern  void WeightPBIN2TXT( char *src, char *dist );

#ifdef __cplusplus
}
#endif

#endif	/* !WEIGHT_H */
