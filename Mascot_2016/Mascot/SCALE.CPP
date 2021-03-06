/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#include "stdafx.h" 

#include<MATH.H>
#include<STDIO.H>
#include "fqeval.hpp"
/**/
/**/
void
ScalePoints(void)
{
	register long p;
	PtBead*Pt;
	IPOINT = IPOINT5;
	for(p = 1; p<ZPTLIM; ++p)
	{
		Pt = GetPtBead(p);
		Pt->ZPTCO1 <<= S_FACTOR;
		Pt->ZPTCO2 <<= S_FACTOR;
	}
	PXMIN <<= S_FACTOR;
	PYMIN <<= S_FACTOR;
	PXMAX <<= S_FACTOR;
	PYMAX <<= S_FACTOR;
}
void
Back_Scale_1(void)
{
	register long p, l;
	PtBead*Pt;

	double sf = ((int64_t)1L)<<S_FACTOR;

	for(p = 1; p<ZPTLIM; ++p) {
		Pt = GetPtBead(p);
		Pt->ZPTCO1 = (long)floor((double)Pt->ZPTCO1/sf +0.5);
		Pt->ZPTCO2 = (long)floor((double)Pt->ZPTCO2/sf +0.5);
	}
	PXMIN >>= S_FACTOR;
	PYMIN >>= S_FACTOR;
	PXMAX >>= S_FACTOR;
	PYMAX >>= S_FACTOR;
}
void
Back_Scale_2(void)
{
	register long X;
	register long Y;
	long p;
	char f;
	register long S;
	register long h;
	PtBead*Pt;
	PtBead*PT;

	CleanZPTHASH();
	for(p = 1; p<ZPTLIM; ++p)
	{
		Pt = GetPtBead(p);
		if (!Pt) {
			continue;
		}
		if(Pt->ZPTAux) {
			/* 
				Residual after the previous iteration. 
				Skip it. 
			*/
			continue;
		}
		Y = Pt->ZPTCO2;
		X = Pt->ZPTCO1;
		S = X+Y;
		h = S&ZPBLIM;
		if(!h)
		{
			if(S)
			{
				do S>>=1; while(!(S&1));
				h = S&ZPBLIM;
			}
		}
		PT = ZPTHASH[h];
		while(PT)
		{
			if(PT->ZPTCO1==X&&PT->ZPTCO2==Y)break;
			PT = PT->ZPTNxt;
		}
		if(!PT)
		{
			Pt->ZPTNxt = ZPTHASH[h];
			ZPTHASH[h] = Pt;
			Pt->ZPTAux = 0;
		}
		else 
		{ 
			Pt->ZPTAux = PT;
		} 
	}
	IPOINT = IPOINT4;
}

void
Back_Scale_5(void)
{
	register long lv;
	LvBead* Lv;
	PtBead* Pt;
	for (lv = 1; lv < ZLVLIM; lv++)
	{
		Lv = GetLvBead(lv);
		PtBead*& P1 = Lv->ZLVPoint1;
		PtBead*& P2 = Lv->ZLVPoint2;

		Pt = (PtBead*)P1->ZPTAux;
		if (Pt)P1 = Pt;

		Pt = (PtBead*)P2->ZPTAux;
		if (Pt)P2 = Pt;
	}
}

void
Back_Scale_3(void)
{
	VtBead*V4;
	VtBead*Vt;
	PtBead*Pt;
	PtBead*P1;
	PtBead*P2;
	register char FA, FB;

	long vt;
	for(vt = 2; vt<ZVTLIM; vt+=2)
	{
		FA = FB = 0;
		Vt = GetVtBead(vt);

		V4 = Vt->ZVTMeInreverse;
		if(!V4)
		{
			continue;
		}
		P1 = Vt->ZVTCO1;
		P2 = Vt->ZVTCO2;

		Pt = (PtBead*)P1->ZPTAux;
		if(Pt)
		{
			Vt->ZVTCO1 = V4->ZVTCO2 = Pt;
			FA = 1;
		}
		Pt = (PtBead*)P2->ZPTAux;
		if(Pt)
		{
			Vt->ZVTCO2 = V4->ZVTCO1 = Pt;
			FB = 1;
		}
		if(FA||FB)
		{
			Relink_Lv_Pt(Vt);
			Relink_Lv_Pt(V4);
		}
	}
}
void
Back_Scale_4(void)
{
	LbBead*Lb;
	PtBead*P1;
	PtBead*P2;
	LvBead*Lv;
	LvBead*Le;
	LvBead*Lo;
	VtBead*Vt;
	VtBead*Vr;
	long lb;
	for(lb = 1; lb<ZLBLIM; ++lb)
	{
		Lb = GetLbBead(lb);
		Lv = Lb->ZLBFirstLvBead;
		if(!Lv)
		{
			continue;
		}
		Le = Lv;
		do
		{
			P1 = Lv->ZLVPoint1;
			P2 = Lv->ZLVPoint2;
			if(P1==P2)Lv = Lv->ZLVNextLvBead;
		}
		while(Lv&&(P1==P2)&&(Lv!=Le));
		if(P1==P2)
		{
			Lv = Lb->ZLBFirstLvBead, Lb->ZLBFirstLvBead = 0;
			do
			{
				Vt = Lv->ZLVMyVector;
				if(Vt)
				{
					Vr = Vt->ZVTMeInreverse;
					Vt->ZVTMeInreverse = 0;
					if(Vr) {
						Vr->ZVTMeInreverse = 0;
					}
				}
				Lv = Lv->ZLVNextLvBead;
			}
			while(Lv&&(Lv!=Le));
			continue;
		}
		else if(Lv!=Le) Lb->ZLBFirstLvBead = Lv;
		Le = Lv;
		Lo = Le;
		Lv = Lv->ZLVNextLvBead;
		if(Lv)
		do
		{
			P1 = Lv->ZLVPoint1;
			P2 = Lv->ZLVPoint2;
			if(P1==P2)
			{
				Vt = Lv->ZLVMyVector;
				if(Vt)
				{
					Vr = Vt->ZVTMeInreverse;
					Vt->ZVTMeInreverse = 0;
					if(Vr)Vr->ZVTMeInreverse = 0;
				}
				Lo->ZLVNextLvBead = Lv->ZLVNextLvBead;
			}
			else Lo = Lv;
			Lv = Lv->ZLVNextLvBead;
		}
		while(Lv&&(Le!=Lv));
	}
}
void
CleanPvBead(void)
{
	long p;
	for(p = 1; p<ZPTLIM; ++p) GetPtBead(p)->ZPTAux = 0;
}
long
Back_ScalePoints(void)
{
	CleanPvBead();

	Back_Scale_1();
	Back_Scale_2();

	CleanPvBead();

	return 0; 
}
