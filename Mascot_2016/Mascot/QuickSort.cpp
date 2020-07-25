/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#include "stdafx.h" 

#include<math.h>

#include "QuickSort.h"

GT* Gt;
GT_SWAP* GtSwap;
GT_GET* GtGetVal;

long GetMinXLv(LvBead*Lv)
{
	register long X1=(Lv->ZLVPoint1)->ZPTCO1;
	register long X2=(Lv->ZLVPoint2)->ZPTCO1;
	return X1<X2? X1: X2;
}
AUXObj GtGetXLv(long k)
{
	AUXObj T;
	T.L = GetMinXLv((LvBead*)PTRAux_1[k]);
	return T;
}
void GtSwapLv(long k, long m)
{
	void*&K = PTRAux_1[k];
	void*&M = PTRAux_1[m];
	void*T=K;K=M;M=T;
}
char GtXLv(AUXObj k, AUXObj m, char f)
{
	register long x_k;
	register long x_m;
	if(!f)
	{
		x_k = GetMinXLv((LvBead*)PTRAux_1[k.L]);
		x_m = GetMinXLv((LvBead*)PTRAux_1[m.L]);
	}
	else
	if(f==1)x_m = GetMinXLv((LvBead*)PTRAux_1[m.L]),
		x_k = k.L;
	else	x_k = GetMinXLv((LvBead*)PTRAux_1[k.L]),
		x_m = m.L;

	if(x_k>x_m)return 1;
	return 0;
}
AUXObj GtGetVt(long k)
{
	AUXObj T;
	T.P = PTRAux_1[k];
	return T;
}
void GtSwapVt(long k, long m)
{
	void*&K = PTRAux_1[k];
	void*&M = PTRAux_1[m];
	void*T=K;K=M;M=T;
}
char GtYVt(AUXObj k, AUXObj m, char f) // sort order by second points top to bottom
{
	register VtBead*i;
	register VtBead*j;
	if(!f)
	{
		i = (VtBead*)PTRAux_1[k.L];
		j = (VtBead*)PTRAux_1[m.L];
	}
	else
	if(f==1)j = (VtBead*)PTRAux_1[m.L], i = (VtBead*)k.P;
	else	i = (VtBead*)PTRAux_1[k.L], j = (VtBead*)m.P;

	long y_k = i->ZVTCO2->ZPTCO2;
	long y_m = j->ZVTCO2->ZPTCO2;
	if(y_k>y_m)return 0;
	if(y_k<y_m)return 1;
	if(i->ZVTCO2->ZPTCO1>j->ZVTCO2->ZPTCO1)return 1;
	return 0;
}
char GtyVt(AUXObj k, AUXObj m, char f) // sort order by first points bottom to top
{
	register VtBead*i;
	register VtBead*j;
	if(!f)
	{
		i = (VtBead*)PTRAux_1[k.L];
		j = (VtBead*)PTRAux_1[m.L];
	}
	else
	if(f==1)j = (VtBead*)PTRAux_1[m.L], i = (VtBead*)k.P;
	else	i = (VtBead*)PTRAux_1[k.L], j = (VtBead*)m.P;

	long y_k = i->ZVTCO1->ZPTCO2;
	long y_m = j->ZVTCO1->ZPTCO2;
	if(y_k<y_m)return 0;
	if(y_k>y_m)return 1;
	if(i->ZVTCO1->ZPTCO1>j->ZVTCO1->ZPTCO1)return 1;
	return 0;
}
void GtSwapLb(long k, long m)
{
	void*&K = PTRAux_2[k];
	void*&M = PTRAux_2[m];
	void*T=K;K=M;M=T;
}
char GtYLb(AUXObj k, AUXObj m, char f)
{
	register LbBead*i;
	register LbBead*j;
	if(f==0)i = (LbBead*)PTRAux_2[k.L], j = (LbBead*)PTRAux_2[m.L];
	else
	if(f==1)j = (LbBead*)PTRAux_2[m.L], i = (LbBead*)k.P;
	else	i = (LbBead*)PTRAux_2[k.L], j = (LbBead*)m.P;

	long y_k = i->ZLBXym->ZPTCO2;
	long y_m = j->ZLBXym->ZPTCO2;
	if(y_k>y_m)return 0;
	if(y_k<y_m)return 1;
	if(i->ZLBXym->ZPTCO1>j->ZLBXym->ZPTCO1)return 1;
	return 0;
}
char GtyLb(AUXObj k, AUXObj m, char f)
{
	register LbBead*i;
	register LbBead*j;
	if(f==0)i = (LbBead*)PTRAux_2[k.L], j = (LbBead*)PTRAux_2[m.L];
	else
	if(f==1)j = (LbBead*)PTRAux_2[m.L], i = (LbBead*)k.P;
	else	i = (LbBead*)PTRAux_2[k.L], j = (LbBead*)m.P;

	long y_k = i->ZLBXym->ZPTCO2;
	long y_m = j->ZLBXym->ZPTCO2;
	if(y_k<y_m)return 0;
	if(y_k>y_m)return 1;
	if(i->ZLBXym->ZPTCO1>j->ZLBXym->ZPTCO1)return 1;
	return 0;
}
char GtxLb(AUXObj k, AUXObj m, char f)
{
	register LbBead*i;
	register LbBead*j;
	if(f==0)i = (LbBead*)PTRAux_2[k.L], j = (LbBead*)PTRAux_2[m.L];
	else
	if(f==1)j = (LbBead*)PTRAux_2[m.L], i = (LbBead*)k.P;
	else	i = (LbBead*)PTRAux_2[k.L], j = (LbBead*)m.P;

	long x_k = i->ZLBXym->ZPTCO1;
	long x_m = j->ZLBXym->ZPTCO1;
	if(x_k<x_m)return 0;
	if(x_k>x_m)return 1;
	if(i->ZLBXym->ZPTCO2>j->ZLBXym->ZPTCO2)return 1;
	return 0;
}
AUXObj GtGetLb(long k)
{
	AUXObj T;
	T.P=PTRAux_2[k];
	return T;
}
void Quick_Sort(long l, long r)
{
	static AUXObj X;
	AUXObj L;
	static AUXObj R;
	X.L = (l+r)>>1;
	if(X.L!=l)
	{
		L.L=l;
		R.L=r;
		if(Gt(L, X, 0))GtSwap(l, X.L);
		if(Gt(X, R, 0))GtSwap(r, X.L);
		if(Gt(L, X, 0))GtSwap(l, X.L);
	}
	else
	{
		R.L=r;
		if(Gt(X, R, 0))GtSwap(r, X.L);
		return;
	}
	if(r-l<3)return;
	X = GtGetVal(X.L);
	L.L = l+1;
	R.L = r-1;
	while(L.L<R.L)
	{
		while(Gt(X, L, 1))++L.L;
		while(Gt(R, X, 2))--R.L;
		if(L.L<R.L)
		{
			GtSwap(L.L, R.L), ++L.L, --R.L;
		}
	}
	if(l<R.L)Quick_Sort(l, R.L);
	if(L.L<r)Quick_Sort(L.L, r);
}



void Lv_Sort(int firts_lb)
{
	LbBead*Lb;
	LvBead*Le;
	LvBead*Lv;
	long Vt=0;
	long Ls=0;
	for(Ls = firts_lb; Ls<ZLBLIM; ++Ls)
	{
		Lb = GetLbBead(Ls);
		Le = Lb->ZLBFirstLvBead;
		if(!Le)continue;
		Lv = Le;
		do
		{
			++Vt;

			Lv->ZLVMyVector = 0;
			Lv->ZLVNextConcident = 0;
			PTRAux_1[Vt] = Lv;
			Lv = Lv->ZLVNextLvBead;
		}
		while(Lv&&Lv!=Le);
	}
	Gt = GtXLv;
	GtSwap = GtSwapLv;
	GtGetVal = GtGetXLv;
	if(Vt>1)Quick_Sort(1, Vt);

	PTRAux_1Lim = Vt+1;
}
