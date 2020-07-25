#include "stdafx.h" 

#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "fqeval.hpp"

#include "QuickSort.h"
/**/
extern void zabort(void);
char*fqeval = "fqeval";
void(*zAbort_proc)(void);
/**/
char __GrpNONE__;
long S_Factor;
long S_FACTOR;
/**/
_sAlong ZLDCO0; /*X*/
_sAlong ZLDCO1; /*Y*/
_sAlong ZLDCO2; /*X*/
_sAlong ZLDCO3; /*Y*/
_sAlong ZLDCO4; /*X*/
_sAlong ZLDCO5; /*Y*/
/**/
_zAvoid PTRAux_1;
_zAvoid PTRAux_2;
_sAlong INDAux_3;
/**/
_zAvtbl ZVtTBl;
_zAltbl ZLvTBl;
_zAbtbl ZLbTBl;
_zAptbl ZPtTBl; // a table of unique points. 
/**/
long ZLDLIM;
long ZLBLIM;
long ZLVLIM;
long ZPTLIM;
long ZVTLIM;
long PTRAux_2Lim;
long PTRAux_1Lim;
long INDAux_3Lim;
/**/
PtBead**ZPBCOR; // a hash table of points from ZPtTBl. 
long ZPBLIM = 8191;
long __ZPBLIM__ = 8191;
/**/
PtBead*IPOINT0(long X, long Y); // Gets a point from ZPtTBl; creates a new point if x,y does not exist. 
PtBead*IPOINT4(long X, long Y); // Gets a point from ZPtTBl using hash table ZPBCOR. 
PtBead*IPOINT5(long X, long Y);
typedef PtBead*ipoint(long, long);
ipoint*IPOINT = IPOINT4;
/**/
long LYCODE[10]; /*LAYER-CODE*/
long LYMASK[10]; /*LAYER-MASKA*/
/**/
long Target_Function; /*FUNCTION-CODE*/
#define OVRELIM 0
#define ZANDNOT 1
#define ZZZANDJ 2
#define ZZZXORJ 3
#define ZZZZORJ 4
#define ZZZNOTJ 5
#define ZZZSIZE 6

#define ZZZAND1 -1
#define ZZZXOR1 -2
/**/
LbBead*Si_Lb;
VtBead*SI_V1;
VtBead*SI_V2;
long Si_St;
/**/
char SI_Error;
char PV_Error;
/**/
unsigned short PBLIM;
unsigned short PXCNT;
unsigned short PYCNT;
long XPDIV;
long YPDIV;
long PXMAX;
long PYMAX;
long PXMIN;
long PYMIN;
/**/
char Closed_Polygon;
/**/
long Delta_X_2;
long Delta_Y_1;
long Delta_Y_2;
/**/
long Delta_X;
long Delta_Y;
/**/
long Sized_X;
long Sized_Y;
/**/
long Sh_Xmin;
long Sh_Xmax;
long Sh_Ymin;
long Sh_Ymax;
long Sh_Xdir;
long Sh_Ydir;
long Sh_Botm;
VtBead*Sh_Vt;
PtBead*Sh_Ip;
PtBead*Sh_Pm;
void* *Sh_Au;
char   Sh_Bl;
/**/
LvBead*DJ_REVERSE_LV;
LvBead*DJ_FIRSTFR_LV;
/**/
char S_Free;
long STATUS;
long C_Layer;
char DummyClip;
char Open_Shape;
char BuildSLine;
/**/
extern unsigned short ILayer_Cnt;
char M_CONV_MODULE;
char F_EVAL_MODULE;
char F_INIT_MODULE;
/**/
char ComplexIntersectionAllowed;
char RepeatComplexIntersection;
/**/
void PrintOk(void);
/**/
long POINT_VECTOR_ANALYSIS_Pass_Count;
/**/
LvBead*SPLITLV_M(LvBead*L3, PtBead*IP);
LvBead*COMPSPLITLV_M(LvBead*L3, VtBead*V3, VtBead*V4, PtBead*IP);
/**/
compsplitlv*COMPSPLITLV = COMPSPLITLV_M;
splitlv*SPLITLV = SPLITLV_M;
/**/
void MERGELVCHAIN_M(VtBead*VT, VtBead*PV); 
mergelvchain*MERGELVCHAIN = MERGELVCHAIN_M;
/**/
void
zAbort(char*ids, long lin)
{
      fprintf(stderr, "\nModule %s: %ld", ids, lin);
      if(zAbort_proc)
      zAbort_proc();
      zabort();
}
/**/
PtBead*GetPtBead(long Pt)
{
	PtBead*pt = &ZPtTBl[Pt];
	return pt;
}
LvBead*GetLvBead(long Lv)
{
	LvBead*lv = &ZLvTBl[Lv];
	return lv;
}
LbBead*GetLbBead(long Lb)
{
	LbBead*lb = &ZLbTBl[Lb];
	return lb;
}
VtBead*GetVtBead(long Vt)
{
	VtBead*vt = &ZVtTBl[Vt];
	return vt;
}

void l_swap(long&A, long&B) { long T=A;A=B;B=T; }

char
COMBINE(LbBead*LB)
{
	LvBead*L0; /*AUXILIARY-LV-POINTER*/
	LvBead*L1; /*INITIAL-VECTOR*/
	LvBead*L2; /*LB-VECTOR-PRECEEDING-COINCIDENCE*/
	LvBead*L3; /*LAST-COINCIDENT-B3-VECTOR*/
	LvBead*L4; /*B3-VECTOR-AFTER-L3*/
	LvBead*L5; /*B3-VECTOR-PRECEEDING-COINCIDENCE*/
	LvBead*L6; /*LAST-COINCIDENT-LB-VECTOR*/
	LvBead*L7; /*FIRST-COINCIDENT-B3-VECTOR(L6-COINCIDENCE)*/
	LvBead*L8; /*AUXILIARY-LV-POINTER*/
	LvBead*L9; /*LB-VECTOR-AFTER-L6*/
	LbBead*B3; /*(COINCIDENT-LB)-POINTER*/
	LbBead*B1; /*AUXILIARY-LB-POINTER*/
	L1 = L0 = LB->ZLBFirstLvBead;
	L8 = FetchReverseLv(L1);
	B3 = L8? L8->ZLVMyLbBead: (B1=0);
	if(L8)
	do /*FIND-INITIAL-VECTOR*/
	{
		L1 = L1->ZLVNextLvBead;
		L8 = FetchReverseLv(L1);
		B1 = L8? L8->ZLVMyLbBead: 0;
	}
	while((L1!=L0)&&(B3==B1));
	/*FULLY-COINCIDENT?*/
	if(B3&&(L0==L1))return(2);
	L0 = L1;
	B3 = LB;
	do /*FIND-L3-L2-(L0-IS-SECOND-FOR-LB)*/
	{
		L2 = L0;
		L0 = L0->ZLVNextLvBead;
		L3 = FetchReverseLv(L0);
		if(L3)B3 = L3->ZLVMyLbBead;
	}
	while((B3==LB)&&(L1!=L0));
	/*NON-COINCIDENT?*/
	if(L1==L0)return(0);
	do /*FIND-LAST-COINCIDENT-WITH-B3(L6)*/
	{
		L6 = L0;
		L0 = L0->ZLVNextLvBead;
		L7 = FetchReverseLv(L6);
		L8 = FetchReverseLv(L0);
		B1 = L8? L8->ZLVMyLbBead: 0;
		if(B1!=B3)break;
		L8 = L8->ZLVNextLvBead;
		if(L8!=L7)break;
	}
	while(1);
	L9 = L6->ZLVNextLvBead;
	L4 = L3->ZLVNextLvBead;
	L0 = L4;
	while(L0!=L7)
	{
		L5 = L0;
		L0->ZLVMyLbBead = LB;
		L0 = L0->ZLVNextLvBead;
	}
	if(L4==L7)
	L2->ZLVNextLvBead = L9;
	else
	{
		L2->ZLVNextLvBead = L4;
		L5->ZLVNextLvBead = L9;
	}
	B3->ZLBFirstLvBead = 0L;
	LB->ZLBFirstLvBead = L2; /*Set New Origin*/
	return(1);
}
void
COMBINE_C_POLYGONS(void)
{
	char F;
	char S;
	long L;
	if(ZLBLIM<3) return;

	L = 1;
	while(L<ZLBLIM)GetLbBead(L++)->ZLBCO1 = 1;

	for(F = 1, L = 1; F; L = 1)
	for(F = 0; L<ZLBLIM; L+= 1)
	{
		LbBead*B = GetLbBead(L);
		if(B->ZLBCO1!=1||!B->ZLBFirstLvBead)
		{
		/*POLYGON IS FULLY OUTSIDE OR IT WAS COMBINED BEFORE.*/
			continue;
		}
		B->ZLBCO1 = S = COMBINE(B);
		if(S)F = 1;
	}
	L = 1;
	while(L<ZLBLIM)GetLbBead(L++)->ZLBCO1 = 1;
}
/**/
void CleanZPBCOR(unsigned short Cnt)
{
      memset(ZPBCOR, 0, Cnt*sizeof(PtBead*));
}
PtBead*IPOINT0(long X, long Y)
{
	PtBead*P;
	register long i;
	GetPtBead(ZPTLIM)->ZPTCO1 = X;
	GetPtBead(ZPTLIM)->ZPTCO2 = Y;

	P = GetPtBead(i=1);
	while(P->ZPTCO1!=X||P->ZPTCO2!=Y)
	{
		P = GetPtBead(++i);
	}
	if(i==ZPTLIM)GetPtBead(ZPTLIM++)->ZPTFirstSortedVtBead = 0;
	return P;
}
PtBead*IPOINT4(long X, long Y)
{
	PtBead*P;
	long S;
	long h;
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
	P = ZPBCOR[h];
	while(P)
	{
		if(P->ZPTCO1!=X||P->ZPTCO2!=Y)P = P->ZPTNxt;
		else
		{
			return(P);
		}
	}
	P = GetPtBead(ZPTLIM++);
	P->ZPTNxt = ZPBCOR[h];
	ZPBCOR[h] = P;
	P->ZPTCO1 = X;
	P->ZPTCO2 = Y;
	P->ZPTFirstSortedVtBead = 0;
	return(P);
}
PtBead*IPOINT5(long X, long Y)
{
	PtBead*P;
	long S;
	long h;
	S = (X+Y)>>S_FACTOR;
	h = S&ZPBLIM;
	if(!h)
	{
		if(S)
		{
			do
				S>>=1;
			while(!(S&1));
			h = S&ZPBLIM;
		}
	}
	P = ZPBCOR[h];
	while(P)
	{
		if(P->ZPTCO1!=X||P->ZPTCO2!=Y)P = P->ZPTNxt;
		else
		{
			return(P);
		}
	}
	P = GetPtBead(ZPTLIM++);
	P->ZPTNxt = ZPBCOR[h];
	ZPBCOR[h] = P;
	P->ZPTCO1 = X;
	P->ZPTCO2 = Y;
	P->ZPTFirstSortedVtBead = 0;
	return(P);
}
double Eval_Angle(double DX, double DY)
{
	double AN;
	if(DX==0.0)	AN = DY>0.0? 90.0: DY<0.0? 270.0: 0.0;
	else
	if(DY==0.0)	AN = DX>0.0? 0.0: 180.0;
	else
	{
		AN = atan(DY/DX)*(180.0/M_PI);
		if(DX<0.0) AN = 180.0+AN;
		else if(!(DY>0.0)) AN = 360.0+AN;
	}
	return(AN);
}
double sumangles;
char SS_Fl;
double
SUB_ANGLES(double A0, double A1)
{
	double A2;
	if(SS_Fl)
	{
		SS_Fl = 0;
		return(0);
	}
	A2 = A0-A1;
	if(A2>+180.0)A2 -= 360.0;
	else
	if(A2<-180.0)A2 += 360.0;
	return(A2);
}
double
COMPLVANGLE(LvBead*L)
{
	PtBead*A;
	PtBead*B;
	double X, Y;
	A = L->ZLVPoint1;
	B = L->ZLVPoint2;
	X = B->ZPTCO1-A->ZPTCO1;
	Y = B->ZPTCO2-A->ZPTCO2;
	if(X==0&&Y==0)
	{
		SS_Fl = 1;
		return(0);
	}
	return(Eval_Angle(X, Y));
}
double
COMPLDANGLE(long L)
{
	double X, Y;
	X = ZLDCO0[L]-ZLDCO0[L-1];
	Y = ZLDCO1[L]-ZLDCO1[L-1];
	if(X==0&&Y==0)
	{
		SS_Fl = 1;
		return(0);
	}
	return(Eval_Angle(X, Y));
}
double
COMPANGLE(long*x, long*y, long L)
{
	double X, Y;
	X = x[L]-x[L-1];
	Y = y[L]-y[L-1];
	if(X==0&&Y==0)
	{
		SS_Fl = 1;
		return(0);
	}
	return(Eval_Angle(X, Y));
}
long
DETERMINE_STAT_3(long*x, long*y, long lim)
{
	long S, D;
	double F, L, C, T;
	SS_Fl = 0;
	if(lim<3)
	{
		return STATUS=0;
	}
	L = F = COMPANGLE(x, y, 1);
	T = 0;
	for(D = 2; D<lim; ++D, L = C)
	{
		T += SUB_ANGLES(L,
				(C = COMPANGLE(x,y,D), SS_Fl? (C = L):C));
	}
	T += SUB_ANGLES(L, F);
	if(T>0.0) S = _EQ_(T, 360.0, 0.001) ? 1: 0;
	else S = _EQ_(T,-360.0, 0.001) ?-1: 0;
	STATUS = S;
sumangles = T;
	return(S);
}
long
DETERMINE_STAT_2(LbBead*B)
{
	long S;
	LvBead*E;
	LvBead*V;
	double F, L, C, T;
	E = B->ZLBFirstLvBead;

	SS_Fl = 0;
	if(!E)return(STATUS=0);

	L = F = COMPLVANGLE(E);
	T = 0;
	for(V = E->ZLVNextLvBead; V!=E; V = V->ZLVNextLvBead)
	{
		T += SUB_ANGLES(L,
				(C = COMPLVANGLE(V), SS_Fl? (C = L):C));
		L = C;
	}
	T += SUB_ANGLES(L, F);
	if(T>0.0) S = _EQ_(T, 360.0, 0.001) ? 1: 0;
	else S = _EQ_(T,-360.0, 0.001) ?-1: 0;
	STATUS = S;
sumangles = T;
	return(S);
}
LvBead*SPLITLV_M(LvBead*L3, PtBead*IP)
{
	LvBead*L4 = GetLvBead(ZLVLIM++);
	L4->ZLVMyVector = 0;
	L4->ZLVNextConcident = 0;
	L4->ZLVNextLvBead = L3->ZLVNextLvBead;
	L3->ZLVNextLvBead = L4;
	L4->ZLVMyLbBead = L3->ZLVMyLbBead;
	L4->ZLVPoint2 = L3->ZLVPoint2;
	L3->ZLVPoint2 = IP;
	L4->ZLVPoint1 = IP;
	return(L4);
}
LvBead*COMPSPLITLV_M(LvBead*L3, VtBead*V3, VtBead*V4, PtBead*IP)
{
	LvBead*L4; /*SECOND-PART-OF-L3*/
	LvBead*L7; /*COINCINDENT-FOR-L3*/
	LvBead*L8; /*COINCINDENT-FOR-L4*/
	L7 = L3->ZLVNextConcident;
	L8 = L7 ? COMPSPLITLV_M(L7, V3, V4, IP) : 0;
	L4 = SPLITLV_M(L3, IP);
	L4->ZLVMyVector = V4; /*SET-VECTOR-NUMBER*/
	L4->ZLVNextConcident = L8; /*SET-NEXT-COINCINDENT-VECTOR*/
	return(L4);
}
char VtIsRight(PtBead*P1,PtBead*P2)
{
	long X1;
	long X2;
	long Y1;
	long Y2;
	X1 = P1->ZPTCO1;
	X2 = P2->ZPTCO1;
	Y1 = P1->ZPTCO2;
	Y2 = P2->ZPTCO2;
	return !(X1>X2)&&!((X1==X2)&&(Y1>Y2));
}
char VtIsEven(VtBead*Vt)
{
	return ((Vt->ZVTPos>>1)<<1)==Vt->ZVTPos;
}
void
GetVtPair(VtBead*&V3, VtBead*&V4)
{
	long OldLim=ZVTLIM;
	V3 = GetVtBead(ZVTLIM++);
	V4 = GetVtBead(ZVTLIM++);

	V3->ZVTPos = OldLim++;
	V4->ZVTPos = OldLim++;
}
VtBead*SPLITVT(VtBead*V3, PtBead*IP)
{
	VtBead*V4; /*V3-OLD-REVERSE*/
	VtBead*V5; /*V4-NEW-REVERSE-OR-V3-SECOND-PART*/
	VtBead*V6; /*V3-NEW-REVERSE-OR-V4-SECOND-PART*/
	LvBead*L3;
	LvBead*L4;
	LvBead*L5;
	LvBead*L6;
	V4 = V3->ZVTMeInreverse;
	GetVtPair(V5, V6);
	if(!VtIsEven(V3))
	{
		VtBead*V0=V5;V5=V6;V6=V0;
	}
	V3->ZVTCO2 = V4->ZVTCO2 = IP;
	V5->ZVTCO1 = V6->ZVTCO1 = IP;
	V5->ZVTCO2 = V4->ZVTCO1;
	V6->ZVTCO2 = V3->ZVTCO1;
	V5->ZVTMeInreverse = V4;
	V6->ZVTMeInreverse = V3;
	V3->ZVTMeInreverse = V6;
	V4->ZVTMeInreverse = V5;
	L3 = V3->ZVTFirstCoincident; /*CHAIN-HEAD-COINCIDENT-VECTORS*/
	L4 = V4->ZVTFirstCoincident; /*CHAIN-HEAD-FOR-REVERSE-DIRECTION*/
	L5 = L3 ? COMPSPLITLV(L3, V3, V5, IP) : 0;
	L6 = L4 ? COMPSPLITLV(L4, V4, V6, IP) : 0;
	V6->ZVTFirstCoincident = L6;
	V5->ZVTFirstCoincident = L5;
	V5->ZVTStatus = V3->ZVTStatus;
	V6->ZVTStatus = V4->ZVTStatus;
	V6->ZVTAux = V5->ZVTAux = 0;
	V6->ZVTNextSortedVtBead = V5->ZVTNextSortedVtBead = 0;
	return(V5);
}
VtBead*INSERTV(PtBead*P1, PtBead*P2, LvBead*LV)
{
	VtBead*V3;
	VtBead*V4;
	GetVtPair(V3, V4);
	if(!VtIsRight(P1,P2))
	{
		VtBead*V0=V3;V3=V4;V4=V0;
	}
	if(LV)
	{
		LV->ZLVMyVector = V3;
	}
	V3->ZVTFirstCoincident = LV, V4->ZVTFirstCoincident = 0;
	V3->ZVTMeInreverse = V4;
	V4->ZVTMeInreverse = V3;
	V4->ZVTCO2 = V3->ZVTCO1 = P1;
	V4->ZVTCO1 = V3->ZVTCO2 = P2;
	V4->ZVTStatus = V3->ZVTStatus = 0;
	V3->ZVTAux = V4->ZVTAux = 0;
	V3->ZVTNextSortedVtBead = V4->ZVTNextSortedVtBead = 0;
	return(V3);
}
void
MERGELVCHAIN_M(VtBead*VT, VtBead*PV)
{
	LvBead*LQ;
	LvBead*LS;
	LvBead*LH;
	LvBead*LM;
	LH = VT->ZVTFirstCoincident;
	LM = PV->ZVTFirstCoincident;
	for(LS = LQ = LH; LQ; LQ = LQ->ZLVNextConcident)
	{
		LS = LQ, LQ->ZLVMyVector = PV;
	}
	if(LS)LS->ZLVNextConcident = LM, PV->ZVTFirstCoincident = LH;
}
void
SUBS_PV_BEAD(VtBead*Old, VtBead*New, char z)
{
	PtBead*PT;
	VtBead*VT;
	PT = Old->ZVTCO1;
	if(PT->ZPTFirstSortedVtBead==Old)
	{
		PT->ZPTFirstSortedVtBead = New;
		if(z)
		{
			New->ZVTNextSortedVtBead = Old->ZVTNextSortedVtBead;
		}
		return;
	}
	for(VT = PT->ZPTFirstSortedVtBead; VT; VT = VT->ZVTNextSortedVtBead)
	{
		if(VT->ZVTNextSortedVtBead!=Old)
		{
			continue;
		}
		if(z)
		{
			New->ZVTNextSortedVtBead = Old->ZVTNextSortedVtBead;
		}
		VT->ZVTNextSortedVtBead = New;
		return;
	}
}
void
ERASEVTBEAD(VtBead*VT, VtBead*PV)
{
	VtBead*Vt;
	VtBead*Pv;
	Vt = VT->ZVTMeInreverse;
	Pv = PV->ZVTMeInreverse;
	if(Vt->ZVTPos<VT->ZVTPos)
	{
		PV->ZVTMeInreverse = Vt;
		Vt->ZVTMeInreverse = PV;
		if(Pv->ZVTPos<VT->ZVTPos)
		{
			SUBS_PV_BEAD(Vt, Vt->ZVTNextSortedVtBead, 0);
		}
		SUBS_PV_BEAD(Pv, Vt, 1);

		VtBead*Tm=Vt;Vt=Pv;Pv=Tm;
	}
	MERGELVCHAIN(VT, PV);
	MERGELVCHAIN(Vt, Pv);
	if(S_Free)
	{
		PV->ZVTStatus |= VT->ZVTStatus;
		Pv->ZVTStatus |= Vt->ZVTStatus;
	}
	else
	if(!F_EVAL_MODULE)
	{
		PV->ZVTStatus = 0;
		Pv->ZVTStatus = 0;
	}
	/*
	memset(VT, 0, sizeof(VtBead));
	memset(Vt, 0, sizeof(VtBead));
	*/
	VT->ZVTMeInreverse = 0;
	Vt->ZVTMeInreverse = 0;
}
void
split_vt_pv
(VtBead*vt, VtBead*pv, long y2, long y4, PtBead*p2, PtBead*p4)
{
	if(y2<y4)SPLITVT(pv, p2);
	else SPLITVT(vt, p4);
}
void
POINT_VECTOR_ANALYSIS(long first)
{
	char ST, ER, H24, H42;
	VtBead*VT;
	PtBead*PT;
	VtBead*PV;
	PtBead*P2;
	PtBead*P4;
	VtBead*PS;
	long SL, SV, X2, Y2, X4, Y4, Vt;
	long double S24, DE4, DE2;

	PV_Error = 0;
	POINT_VECTOR_ANALYSIS_Pass_Count = 0;
	for(Vt=first; Vt<ZVTLIM; ++Vt)
	{
		VT = GetVtBead(Vt);

		if(!VT->ZVTMeInreverse)
		{
			continue;
		}
		PT = VT->ZVTCO1;
		P2 = VT->ZVTCO2;
		X2 = P2->ZPTCO1-PT->ZPTCO1;
		Y2 = P2->ZPTCO2-PT->ZPTCO2;

		if(!X2&&!Y2)
		{
			continue;
		}
		if(X2>0) SL = Y2<0 ? 4 : 1;
		else
		if(X2<0) SL = Y2>0 ? 2 : 3;
		else     SL = Y2>0 ? 2 : 4;

		PV = PT->ZPTFirstSortedVtBead;
		PS = 0;
		ER = 0;
		for(ST = 1; PV!=0 && ST; ++POINT_VECTOR_ANALYSIS_Pass_Count)
		{
			P4 = PV->ZVTCO2;
			X4 = P4->ZPTCO1-PT->ZPTCO1;
			Y4 = P4->ZPTCO2-PT->ZPTCO2;

			if(X4>0) SV = Y4<0 ? 4 : 1;
			else
			if(X4<0) SV = Y4>0 ? 2 : 3;
			else     SV = Y4>0 ? 2 : 4;

			if(SL>SV)ST = 1;
			else
			if(SL<SV)ST = 0;
			else
			if(P2==P4)ER = 1, ST = 0;
			else
			{
				S24 = (long double)X2*Y4-(long double)X4*Y2;

				DE4 = ((X4>0?X4:-X4)+(Y4>0?Y4:-Y4))/(3.0);
				DE2 = ((X2>0?X2:-X2)+(Y2>0?Y2:-Y2))/(3.0);

				H42 = S24 >DE2 ?-1 : S24 <-DE2 ? 1 : 0;
				H24 = S24 >DE4 ? 1 : S24 <-DE4 ?-1 : 0;

				ER = (!H42 && !H24)? 2:0;
				ST = S24 <0 && !ER;
			}
			if(ST)PS = PV, PV = PV->ZVTNextSortedVtBead;
			else if(ER)PV_Error = 1;
		}
		if(ER==0)
		{
			VT->ZVTNextSortedVtBead = PV;
			if(PS)PS->ZVTNextSortedVtBead = VT;
			else  PT->ZPTFirstSortedVtBead = VT;
			continue;
		}
		else
		if(ER==1)
		{
			if(VT!=PV)ERASEVTBEAD(VT, PV);
			continue;
		}
		--Vt;
		if(X2<0) X2 = -X2;
		if(X4<0) X4 = -X4;
		if(Y2<0) Y2 = -Y2;
		if(Y4<0) Y4 = -Y4;

		if(X2>Y2)
		split_vt_pv(VT, PV, X2, X4, P2, P4);
		else
		split_vt_pv(VT, PV, Y2, Y4, P2, P4);
	}
/*END LOOP FOR EACH VECTOR*/
}
char
CheckItSelf(LbBead*LB, VtBead*VT)
{
	LvBead*LE;
	LE = VT->ZVTFirstCoincident;
	while(LE&&(LE->ZLVMyLbBead!=LB))
	{
		LE = LE->ZLVNextConcident;
	}
	return LE!=0;
}
LvBead*FETCHREVERSE(LbBead*LB, LvBead*LV)
{
	VtBead*VR;
	LvBead*LS;
	LvBead*LR;
	long EC=0;
	LR = 0;
	VR = (LV->ZLVMyVector)->ZVTMeInreverse;
	LS = VR->ZVTFirstCoincident;
	while(LS)
	{
		if(LS->ZLVMyLbBead==LB)LR = LS, ++EC;
		LS = LS->ZLVNextConcident;
	}
	if(EC<=1)return LR;
/*GET-UP-DIRECT-SUCCESSOR*/
	LS = LV->ZLVNextLvBead;
	while(LS->ZLVMyVector!=VR)
	{
		LS = LS->ZLVNextLvBead;
	}
	return LS;
}
void
REMOVE_FROM_VT_CHAIN(LvBead*LV)
{
	VtBead*VT;
	LvBead*Lv;
	VT = LV->ZLVMyVector;
	Lv = VT->ZVTFirstCoincident;
	if(Lv==LV)
	{
		VT->ZVTFirstCoincident = LV->ZLVNextConcident;
		return;
	}
	while(Lv)
	{
		if(Lv->ZLVNextConcident==LV)
		{
			Lv->ZLVNextConcident = LV->ZLVNextConcident;
			return;
		}
		Lv = Lv->ZLVNextConcident;
	}
}
LvBead*CLEAR_LB_PTR(LvBead*Lv, LvBead*Ln)/*From Lv To Ln*/
{
	LvBead*LE;
	LvBead*LS;
	DummyClip = 1;
	LS = Lv;
	REMOVE_FROM_VT_CHAIN(Ln);
	for(Ln->ZLVMyLbBead = 0; LS!=Ln; LS = LS->ZLVNextLvBead)
	{
		LS->ZLVMyLbBead = 0;
		REMOVE_FROM_VT_CHAIN(LS);
	}
	LE = Ln->ZLVNextLvBead;
	Ln->ZLVNextLvBead = Lv;
	return LE;
}
long
DUMMYJUNCTION(LbBead*LB, LvBead*LV)
{
	long ER;
	LvBead*DJ;
	LvBead*LN; /*LV Coincidencer*/
	LvBead*LF;
	LvBead*LS;
	LN = FETCHREVERSE(LB, LV);
	DJ = LN;
	ER = LN ? 1 : 0;
	while(ER==1)
	{
		LF = LV;
		LS = LN;
		LV = LV->ZLVNextLvBead;
		if(LV==LN)ER = 2;
		else
		{
			LN = FETCHREVERSE(LB, LV);
			ER = LN ? 1 : 0;
			while(ER&&(LN->ZLVNextLvBead!=LS))
			{
				ER = DUMMYJUNCTION(LB, LV);
				if(ER)
				{
					LF->ZLVNextLvBead = CLEAR_LB_PTR(LV, LN);
					LV = LF->ZLVNextLvBead;
					LN = FETCHREVERSE(LB, LV);
					ER = LN ? 1 : 0;
				}
			}
			if(!ER)DJ_FIRSTFR_LV = LV->ZLVNextLvBead;
		}
	}
	DJ_REVERSE_LV = DJ;
	return ER;
}
LvBead*SET_INITIAL_VECTOR(LbBead*LB)
{
	LvBead*LE;
	LvBead*LV;
	LvBead*LN;
	LvBead*LZ;
	LvBead*LM;
	char badpoly = 0;

	LE = LV = LB->ZLBFirstLvBead;

	LN = FETCHREVERSE(LB, LV);
	LM = FETCHREVERSE(LB, LZ=LV->ZLVNextLvBead);

	if(LN||LM)
	do
	{
		LN = FETCHREVERSE(LB, LV=LZ->ZLVNextLvBead);
		LM = FETCHREVERSE(LB, LZ=LV->ZLVNextLvBead);
	}
	while((LN||LM)&&!(badpoly=(LV==LE||LZ==LE)));
	else badpoly = 0;

	if(badpoly)
	{
		CLEAR_LB_PTR(LE->ZLVNextLvBead, LE);
		LB->ZLBFirstLvBead = 0;
		PV_Error = 2;
		return 0;
	}
	return(LB->ZLBFirstLvBead=LZ);
}
void
DUMMYJUNCTIONANALYSIS(LbBead*LB)
{
	LvBead*LV;
	LvBead*LN;
	LvBead*LE;
	LV = SET_INITIAL_VECTOR(LB);
	if(!LV)return;

	LE = LV;
	LN = LV->ZLVNextLvBead;
	do /*For Each Non Coincidenced LV*/
	{
		DJ_FIRSTFR_LV = LN->ZLVNextLvBead;

		if(FetchReverseLv(LN))
		{
			while(DUMMYJUNCTION(LB, LN))
			{
				LN = CLEAR_LB_PTR(LN, DJ_REVERSE_LV);
				LV->ZLVNextLvBead = LN;
				DJ_FIRSTFR_LV = LN->ZLVNextLvBead;
			}
			do
			{
				LV = LN;
				LN = LV->ZLVNextLvBead;
			}
			while(LV!=LE&&LN!=DJ_FIRSTFR_LV);
		}
		else
		LV = LN,
		LN = LV->ZLVNextLvBead;
	}
	while(LV!=LE);
}
void
LDMake_ClosedPolygon(void)
{
	char ER = 0;
	ER |= ZLDCO0[0]!=ZLDCO0[ZLDLIM-1];
	ER |= ZLDCO1[0]!=ZLDCO1[ZLDLIM-1];
	if(ER)
	{
		long L = (ZLDLIM++);
		ZLDCO0[L] = ZLDCO0[0];
		ZLDCO1[L] = ZLDCO1[0];
	}
}
typedef char compare(unsigned char);
char XOR_1(unsigned char S)
{
	if((S&3)==1)return(1);
	return(0);
}
char AND_1(unsigned char S)
{
	if((S&3)==3)return(1);
	return(0);
}
char AN_D(unsigned char S)
{
	register short I;
	for(I=0; I<ILayer_Cnt; ++I)
	if(!(S&LYMASK[I]))return(0);
	return(1);
}
char O_R(unsigned char S)
{
	if(S)return(1);
	return(0);
}
unsigned char AND_NOT_MASK=0xFC;
char AND_NOT(unsigned char S)
{
	if(!S)return(0);
	if(S&AND_NOT_MASK)return(0);
	return(1);
}
unsigned char OVR_ELIM_MASK=0xff;
char OVR_ELIM(unsigned char S)
{
	if(S&OVR_ELIM_MASK)return(1);
	return(0);
}
char X_OR(unsigned char S)
{
	register short I;
	if(!S)return(0);
	for(I=0; I<ILayer_Cnt; ++I)
	if((S|LYMASK[I])==LYMASK[I])return(1);
	return(0);
}
char NO_T(unsigned char S)
{
	if(!S)return(0);
	if((S|12)==12)return(1);
	return(0);
}
compare *True_Stat = O_R;
compare *Cmp_Modules[6] ={OVR_ELIM, AND_NOT, AN_D, X_OR, O_R, NO_T};
void
TRAVERSE(VtBead*VT)
{
	LbBead*LB;
	LvBead*LE;
	LvBead*LV;
	VtBead*V3;
	VtBead*V4;
	long Lim_1=ZLBLIM;
	long Lim_2=ZLVLIM;
	char DJ=0;
	V3 = VT;
	LB = GetLbBead(ZLBLIM++);
	LE = GetLvBead(ZLVLIM);
	LB->ZLBFirstLvBead = LE;
	ZLDLIM = 0;
	do
	{
		V3->ZVTAux = 1;
		LV = GetLvBead(ZLVLIM++);

		LV->ZLVNextConcident = 0;
		LV->ZLVMyVector = V3;
		LV->ZLVMyLbBead = LB;
		LV->ZLVPoint1 = V3->ZVTCO1;
		LV->ZLVPoint2 = V3->ZVTCO2;
		LE->ZLVNextLvBead = V3->ZVTFirstCoincident = LV;
		LE = LV;
		V4 = V3;
		V3 = FETCHSUCCESSOR(V3->ZVTMeInreverse);
		if(V4->ZVTMeInreverse==V3)DJ = 1;
	}
	while(V3!=VT);
	LE = LV->ZLVNextLvBead = LB->ZLBFirstLvBead;
	if(DJ)
	{
		DUMMYJUNCTIONANALYSIS(LB);
	}
	DETERMINE_STAT_2(LB);
	if(STATUS==+1)return;
	if(F_EVAL_MODULE)
	{
fprintf(stderr, "\n!!! ATTEMPTING TO GENERATE NON RIGHT POLYGON !!!");
		zAbort(fqeval,__LINE__);
	}
	LB->ZLBFirstLvBead = 0;
	LV = LE;
	do
	{
		REMOVE_FROM_VT_CHAIN(LV);
		LV = LV->ZLVNextLvBead;
	}
	while(LV!=LE);
	ZLBLIM = Lim_1;
	ZLVLIM = Lim_2;
}
void
FORCE_C_POLYGONS(void)
{
	VtBead*Vt;
	long V;

	ZLBLIM = 1;
	ZLVLIM = 1;
	for(Vt=GetVtBead(V=0); V<ZVTLIM; Vt=GetVtBead(++V))
	{
		Vt->ZVTAux = 0;
		Vt->ZVTFirstCoincident = 0;
	}
	for(Vt=GetVtBead(V=2); V<ZVTLIM; Vt=GetVtBead(++V))
	{
		if(!Vt->ZVTMeInreverse||Vt->ZVTAux)
		{
			continue;
		}
		if(!True_Stat(Vt->ZVTStatus))
		{
			continue;
		}
		TRAVERSE(Vt);
	}
	COMBINE_C_POLYGONS();
}
long
__UNL__(LbBead*LB)
{
	LvBead*LE;
	LvBead*LV;
	LvBead*LN;
	VtBead*V1;
	VtBead*V2;

	long LD;

	double A1;
	double A2;

	LV = LB->ZLBFirstLvBead;
	LN = LV->ZLVNextLvBead;
	V1 = LV->ZLVMyVector->ZVTMeInreverse;
	V2 = LN->ZLVMyVector->ZVTMeInreverse;
	if(CheckItSelf(LB, V1)||CheckItSelf(LB, V2))
	{
		LV = SET_INITIAL_VECTOR(LB);
		if(!LV)return(0);
	}
	LE = LV;
	LN = LV->ZLVNextLvBead;
	A1 = 361.0;
	ZLDLIM = 0;
	do
	{
		DJ_FIRSTFR_LV = LN->ZLVNextLvBead;

		if(FetchReverseLv(LN))
		while(DUMMYJUNCTION(LB, LN))
		{
			LN = CLEAR_LB_PTR(LN, DJ_REVERSE_LV);
			LV->ZLVNextLvBead = LN;
			DJ_FIRSTFR_LV = LN->ZLVNextLvBead;
		}
		LD = (ZLDLIM++);

		ZLDCO0[LD] = LV->ZLVPoint2->ZPTCO1;
		ZLDCO1[LD] = LV->ZLVPoint2->ZPTCO2;

		A2 = COMPLVANGLE(LV);
		if(_EQ_(A1, A2, 1E-3))
		{
			ZLDLIM -= 1;
			ZLDCO0[LD-1] = ZLDCO0[LD];
			ZLDCO1[LD-1] = ZLDCO1[LD];
			if(LD-1)
			{
				A2 = COMPLDANGLE(LD-1);
			}
		}
		A1 = A2;
		LV = LN;
		LN = LV->ZLVNextLvBead;
		while(LN!=DJ_FIRSTFR_LV&&LV!=LE)
		{
			LD = (ZLDLIM++);

			ZLDCO0[LD] = LV->ZLVPoint2->ZPTCO1;
			ZLDCO1[LD] = LV->ZLVPoint2->ZPTCO2;
			A1 = COMPLVANGLE(LV);
			LV = LN;
			LN = LV->ZLVNextLvBead;
		}
	/*End Of Broken Junction*/
	}
	while(LV!=LE);

	LD = (ZLDLIM++);
	ZLDCO0[LD] = ZLDCO0[0];
	ZLDCO1[LD] = ZLDCO1[0];
	return ZLDLIM>3;
}
/*
new functions for polygon classification.
*/
char Si_LbNxt;
LbBead*Si_LbGrp;
void
LbGrp_Init(void)
{
      long LB = 0;
      while(++LB<ZLBLIM)
      {
	      LbBead*Lb=GetLbBead(LB);
	      Lb->ZLBNxt = 0;
	      Lb->ZLBGrp = Lb;
      }
}
void
LbGrp_Mdfy(LbBead*Lb)
{
      LbBead*LbGrp = Lb->ZLBGrp;
      LbBead*LbNxt;
      if(__GrpNONE__)return;
      if(Si_LbGrp==LbGrp)return;
      if(Si_LbNxt)
      for(Lb = LbGrp; 1; Lb = LbNxt)
      {
	      Lb->ZLBGrp = Si_LbGrp;
	      if((LbNxt=Lb->ZLBNxt)==0)
	      {
		      LbBead*nx = Si_Lb->ZLBNxt;
		      Si_Lb->ZLBNxt = LbGrp;
		      Lb->ZLBNxt = nx;
		      break;
	      }
      }
      else
      {
	      Si_Lb->ZLBGrp = LbGrp;
	      Si_Lb->ZLBNxt = LbGrp->ZLBNxt;
	      LbGrp->ZLBNxt = Si_Lb;
	      Si_LbNxt = 1;
	      Si_LbGrp = LbGrp;
      }
}
LbBead*LbGrp_FiMin(LbBead*LbGr)
{
      PtBead*pMin;
      long xMin;
      long yMin;
      LbBead*LbMi = LbGr;
      LbBead*LbNx = LbGr->ZLBNxt;
      if(!LbNx)return LbGr;

      pMin = LbGr->ZLBXym;
      xMin = pMin->ZPTCO1;
      yMin = pMin->ZPTCO2;

      LbBead*l;
      for(l = LbNx; l; l = l->ZLBNxt)
      {
	      PtBead*p;
	      long x;
	      long y;
	      p = l->ZLBXym;
	      x = p->ZPTCO1;
	      y = p->ZPTCO2;

	      if((x<xMin)||(x==xMin&&y<yMin))
	      {
		      xMin = x;
		      yMin = y;
		      pMin = p;
		      LbMi = l;
	      }
      }
      return LbMi;
}
void
SET_INSIDE_ONE_2(LvBead*Lv_0)
{
      LvBead*Lv_1;
      LvBead*Lv_2;
      VtBead*Vt_1;
      VtBead*Vt_2;
      VtBead*Vt_3;
      long Js;
      if(Lv_0->ZLVAux==Si_Lb)
      {
	return;
      }
      LbGrp_Mdfy(Lv_0->ZLVMyLbBead);

      Vt_1 = Lv_0->ZLVMyVector;
      Vt_2 = Vt_1->ZVTMeInreverse;
      do
      {
	      Vt_1->ZVTStatus|= Si_St;
	      Vt_2->ZVTStatus|= Si_St;
	      Lv_0->ZLVAux = Si_Lb;

	      Lv_0 = Lv_0->ZLVNextLvBead;
	      Vt_1 = Lv_0->ZLVMyVector;

	      Js = PTRAux_1Lim;
	      for(Vt_3 = Vt_1->ZVTCO1->ZPTFirstSortedVtBead; Vt_3; Vt_3 = Vt_3->ZVTNextSortedVtBead)
	      {
		      Lv_1 = Vt_3->ZVTFirstCoincident;
		      if(!Lv_1)continue;

		      Lv_2 = Lv_1;
		      do
		      if(Si_Lb==Lv_2->ZLVMyLbBead)
		      return;
		      while((Lv_2 = Lv_2->ZLVNextConcident)!=0);

		      if(Vt_3==Vt_2)continue;
		      if(Vt_3==Vt_1)continue;

		      if(Lv_1->ZLVAux!=Si_Lb)PTRAux_1[++Js] = (void*)Lv_1;
	      }
	      PTRAux_1Lim = Js;
	      Vt_2 = Vt_1->ZVTMeInreverse;
      }
      while(Lv_0->ZLVAux!=Si_Lb);
}
void
SET_LbGrp(LvBead*v)
{
      while(v)
      {
	      LbBead*p=v->ZLVMyLbBead;
	      v = v->ZLVNextConcident;
	      if(p!=Si_Lb)LbGrp_Mdfy(p);
      }
}
void
SET_INSIDE_ALL_1(void)
{
	VtBead*PV;
	VtBead*VR;
	SI_Error = 1;
	for(PV = FETCHSUCCESSOR(SI_V1); PV!=SI_V2; PV = FETCHSUCCESSOR(PV))
	{
		VR = PV->ZVTMeInreverse;

		if(CheckItSelf(Si_Lb, PV))return;
		if(CheckItSelf(Si_Lb, VR))return;
	}
	SI_Error = 0;
}
void
SET_INSIDE_ALL_2(void)
{
      VtBead*PV;
      LvBead*LS;
      VtBead*VR;
      SI_Error = 1;
      for(PV = FETCHSUCCESSOR(SI_V1); PV!=SI_V2; PV = FETCHSUCCESSOR(PV))
      {
	      VR = PV->ZVTMeInreverse;
	      LS = PV->ZVTFirstCoincident;

	      if(CheckItSelf(Si_Lb, PV))return;
	      if(CheckItSelf(Si_Lb, VR))return;

	      if(LS==0)continue;

	      PTRAux_1[PTRAux_1Lim=1] = (void*)LS;
	      while(PTRAux_1Lim>0)
	      {
		      SET_INSIDE_ONE_2((LvBead*)PTRAux_1[PTRAux_1Lim--]);
	      }
      }
      SI_Error = 0;
}
void
SET_INSIDE_ALL_3(void)
{
/*in reverse direction for SET_INSIDE_ALL_2.*/
	VtBead*PV;
	for(PV = FETCHSUCCESSOR(SI_V2); PV!=SI_V1; PV = FETCHSUCCESSOR(PV))
	{
	      SET_LbGrp(PV->ZVTFirstCoincident);
	}
}
void
POINT_VECTOR_CLASS_1(LbBead*LB) /*After DUMMYJUNCTIONANALYSIS Only*/
{
	VtBead*V4;
	VtBead*V5;
	VtBead*V6;
	LvBead*LE;
	LvBead*LV;
	LvBead*LN;

	LV = LB->ZLBFirstLvBead;
	LE = LV;

	LN = LV->ZLVNextLvBead;
	V4 = LV->ZLVMyVector;
	V4 = V4->ZVTMeInreverse;

	Si_Lb = LB;
	do
	{
		V5 = LN->ZLVMyVector;
		V6 = V5->ZVTMeInreverse;


		if(STATUS==+1)
		{
			SI_V1 = V4,SI_V2 = V5;
		}
		else
		{
			SI_V1 = V5,SI_V2 = V4;
		}
		SET_INSIDE_ALL_1();

		if(SI_Error)
		{
			return;
		}
		else V4 = V6, LV = LN, LN = LV->ZLVNextLvBead;
	}
	while(LV!=LE);
}
void
POINT_VECTOR_CLASS_2(LbBead*LB, long SX, long SY) /*After DUMMYJUNCTIONANALYSIS Only*/
{
      VtBead*V3;
      VtBead*V4;
      VtBead*V5;
      VtBead*V6;
      LvBead*LE;
      LvBead*LV;
      LvBead*LN;

      LV = LB->ZLBFirstLvBead;
      LE = LV;

      LN = LV->ZLVNextLvBead;
      V3 = LV->ZLVMyVector;
      V4 = V3->ZVTMeInreverse;

      Si_Lb = LB;
      Si_St = SY;

      Si_LbGrp = LB->ZLBGrp;
      Si_LbNxt = Si_LbGrp!=LB||LB->ZLBNxt!=0;

      PV_Error = 0;
      /*For Each LV*/
      do
      {
	      SET_LbGrp(V3->ZVTFirstCoincident);
	      SET_LbGrp(V4->ZVTFirstCoincident);

	      V5 = LN->ZLVMyVector;
	      V6 = V5->ZVTMeInreverse;

	      SI_V1 = V4;
	      SI_V2 = V5;

	      SET_INSIDE_ALL_2();

	      if(!V4->ZVTFirstCoincident&&!V6->ZVTFirstCoincident)
	      {
		       SET_INSIDE_ALL_3();
	      }
	      if(Target_Function<0&&(V3->ZVTStatus&SX))
	      V3->ZVTStatus |= SY;
	      V3->ZVTStatus |= SX;

	      if(SI_Error)
	      {
		      PV_Error = 1;
		      do
		      {
			      LV = LN;
			      LN = LV->ZLVNextLvBead;
			      V3 = LV->ZLVMyVector;
			      V4 = V3->ZVTMeInreverse;
		      }
		      while(CheckItSelf(LB, V4));
	      }
	      else V3 = V5, V4 = V6, LV = LN, LN = LV->ZLVNextLvBead;
      }
      while(LV!=LE);
}
LbBead*Loadmaster(char check_closure)
// Loads points from ZLDCO0 and ZLDCO1 into the Polygon (LB) and Vector (LV) objects. 
{
	if(check_closure != 0) { 
		LDMake_ClosedPolygon();
	} 

	LbBead*LB;
	LvBead*LV; // current LvBead
	LvBead*LE; // previous LvBead
	PtBead*P1;
	PtBead*P2;
	PtBead*PM; // lowest left point
	LvBead*LM=0; // LvBead of lowest left point
	long XM, YM; // X and Y of lowest left point. 
	long Lim_1=ZLBLIM;
	long Lim_2=ZLVLIM;
	long PCOUNT=1;
	long LD, X, Y;

	LB = GetLbBead(ZLBLIM++);
	LB->ZLBNxt = 0; 
	LB->ZLBGrp = 0; 
	LB->ZLBCO4 = C_Layer;
	LB->ZLBCO1 = STATUS;

	LB->ZLBFirstLvBead = GetLvBead(ZLVLIM);

	XM=ZLDCO0[0];
	YM=ZLDCO1[0];
	PM=P1=P2=IPOINT(X=XM, Y=YM);

	for(LD=1, LE=LV=LB->ZLBFirstLvBead; LD<ZLDLIM; ++LD, P1=P2, LE=LV)
	{
		long x = ZLDCO0[LD];
		long y = ZLDCO1[LD];
		if(x==X&&y==Y)
		{
			continue;
		}
		++PCOUNT;
		P2 = IPOINT(X=x, Y=y);

		LV = GetLvBead(ZLVLIM++);
		LV->ZLVMyLbBead = LB;
		LV->ZLVPoint1 = P1;
		LV->ZLVPoint2 = P2;
		LV->ZLVMyVector = 0;
		LV->ZLVNextConcident = 0;

		LE->ZLVNextLvBead = LV;
		if(X<XM? 1: X>XM? 0: Y<YM? 1: 0)XM=X, YM=Y, PM=P2, LM=LV;
	}
	LV->ZLVNextLvBead = LB->ZLBFirstLvBead;
	LB->ZLBXym = PM;

	if(!Open_Shape&&LM)
	{
		LB->ZLBFirstLvBead = LM;
	}

	if(!Open_Shape&&PCOUNT<=3)
	{
		ZLBLIM = Lim_1,
		ZLVLIM = Lim_2;
		LB = 0; 
	}

	return LB;
}
void
iniPvClass(void)
{
	long i;
	PTRAux_2Lim = ZLBLIM;
	for(i = 1; i<ZLVLIM; ++i)
	{
		GetLvBead(i)->ZLVAux = 0;
	}
	for(i = 1; i<ZLBLIM; ++i)
	{
		PTRAux_2[i] = (void*)GetLbBead(i);
	}
	Gt = GtxLb;
	GtGetVal = GtGetLb;
	GtSwap = GtSwapLb;
	if(PTRAux_2Lim>2)Quick_Sort(1, PTRAux_2Lim-1);
}
char
VtPointUp(VtBead*Vt)
{
	PtBead*i_1;
	PtBead*i_2;
	if(!Vt)zAbort(fqeval,__LINE__);
	i_1 = Vt->ZVTCO1;
	i_2 = Vt->ZVTCO2;
	if(i_1->ZPTCO2>i_2->ZPTCO2)
	{
		return 0;
	}
	return 1;
}
void
iniFiXVt_1(void)
{
	long i;
	long m;
	PTRAux_1Lim = 1;
	for(i = 2; i<ZVTLIM; i+=2)
	{
		m=i+1;
		VtBead*Vt = GetVtBead(i);
		VtBead*Vr = GetVtBead(m);
		VtBead*VT;
		VT = VtPointUp(Vt)? Vt: Vr;
		if(VT->ZVTMeInreverse)
		PTRAux_1[(PTRAux_1Lim++)] = (void*)VT;
		Vt->ZVTAux = 0;
		Vr->ZVTAux = 0;
	}
}
void
iniFiXVt_2(void)
{
	long i;
	PTRAux_1Lim = 1;
	for(i = 2; i<ZVTLIM; ++i)
	{
		VtBead*Vt = GetVtBead(i);
		if(Vt->ZVTAux)
		{
			PTRAux_1[PTRAux_1Lim++] = (void*)Vt;
			Vt->ZVTAux = 0;
		}
	}
}
void
iniFiXStatus(void)
{
	long i;
	if(Sh_Botm==1)
	{
		iniFiXVt_1();
	}
	else
	{
		iniFiXVt_2();
	}
	Gt = ((Sh_Botm>>1)<<1)!=Sh_Botm? GtyVt: GtYVt;
	GtGetVal = GtGetVt;
	GtSwap = GtSwapVt;
	if(PTRAux_1Lim>2)Quick_Sort(1, PTRAux_1Lim-1);

	for(i = 1; i<PTRAux_1Lim; ++i)
	{
		INDAux_3[i-1] = i;
	}
	INDAux_3[i-1] = 0;
	PTRAux_2Lim = 1;
	for(i = 1; i<ZLBLIM; ++i)
	{
		LbBead*Lb = GetLbBead(i);
		if(Lb!=Lb->ZLBGrp)
		{
			continue;
		}
		Lb = LbGrp_FiMin(Lb->ZLBGrp);
		PTRAux_2[PTRAux_2Lim++] = (void*)Lb;
	}
	Gt = ((Sh_Botm>>1)<<1)!=Sh_Botm? GtyLb: GtYLb;
	GtGetVal = GtGetLb;
	GtSwap = GtSwapLb;
	if(PTRAux_2Lim>2)Quick_Sort(1, PTRAux_2Lim-1);
}
void
Find_Ix_Intsect(LbBead*Lb)
{
	long XO, XM, XI, YM;
	long X3, X4, Y3, Y4, YMIN, YMAX;
	VtBead*VT;
	PtBead*IP;
	VtBead*V3;
	PtBead*P3;
	PtBead*P4;

	double xo;
	double xi;

	void**au;
	long* vp;
	long* vn;
	long  v3;

	char  Up=((Sh_Botm>>1)<<1)!=Sh_Botm;

	Sh_Pm = Lb->ZLBXym;
	Sh_Xmin = Sh_Pm->ZPTCO1;
	Sh_Ydir = Sh_Pm->ZPTCO2;

	VT = 0;
	IP = 0;
	xo = XO = Sh_Xdir = 0l;
	XM = Sh_Xmin<<S_Factor;
	YM = Sh_Ydir<<S_Factor;

	vp = &INDAux_3[0];
	for(v3 = (*vp); v3; vp = vn, v3 = (*vn))
	{
		vn = &INDAux_3[v3];
		au = &PTRAux_1[v3];

		V3 = (VtBead*)(*au);

		if(V3->ZVTPos!=((V3->ZVTPos>>1)<<1))
		{
			V3 = V3->ZVTMeInreverse;
		}
		P3 = V3->ZVTCO1;
		P4 = V3->ZVTCO2;

		Y3 = P3->ZPTCO2<<S_Factor;
		Y4 = P4->ZPTCO2<<S_Factor;

		YMIN = Y3>Y4? (YMAX=Y3, Y4): (YMAX=Y4, Y3);

		if(Up? YMAX<YM: YMIN>YM)
		{
			(*vp) = (*vn);
			vn = vp;
			continue;
		}
		if(Up? YMIN>YM: YMAX<YM)break;

		X3 = P3->ZPTCO1<<S_Factor;
		X4 = P4->ZPTCO1<<S_Factor;
		if(X4<XO||X3>=XM)continue;

		if(YMIN==YMAX) XO = X4, VT = 0, IP = P4;
		else if(Y3==YM&&X3>=XO) VT = 0, XO = xo = X3, IP = P3;
		else if(Y4==YM&&X4>=XO) VT = 0, XO = xo = X4, IP = P4;
		else if(X3==X4) VT = V3, XO = xo = X3, IP = 0;
		else
		{
			xi = ((double)(YM-Y3)*(X4-X3))/(Y4-Y3)+X3;
			XI = floor(xi+0.5);
			if(xo<xi&&XM>XI) VT = V3, XO = (xo = xi), IP = 0;
			else
			if(XM==XI)
			{
				Sh_Au = au;
				VT = V3, XO = XM-1, IP = 0;
				break;
			}
			else	continue;
		}
		Sh_Au = au;
		if(XO>=XM)
		{
			zAbort("Find_Ix_Intsect",__LINE__);
		}
	}
	Sh_Vt = VT;
	Sh_Ip = IP;
	Sh_Xdir = XO;
}
long
MAKE_MASK(long S)
{
	long I;
	for(I = 0; I<ILayer_Cnt; ++I)
	{
		if(S&LYMASK[I]) S |= LYCODE[I]<<1;
		if(S&LYCODE[I]) S ^= LYCODE[I];
	}
	return(S);
}
char
Class_IX_Status(LbBead*LB)
{
	VtBead*V3;
	VtBead*V5;
	LvBead*Lv;
	LvBead*Le;
	LbBead*Gr;
	LbBead*Nx;
	LbBead*Gr_Com;
	long IN;
	long PVAStart;

	char  Up=((Sh_Botm>>1)<<1)!=Sh_Botm;

	Sh_Bl = BuildSLine;
	Find_Ix_Intsect(LB);
	if(Sh_Vt)
	{
		V3 = VtPointUp(Sh_Vt)? Sh_Vt: Sh_Vt->ZVTMeInreverse;
	}
	else
	{
		if(!Sh_Ip)
		{
			return 1;
		}
		V5 = Sh_Ip->ZPTFirstSortedVtBead;
		if(!VtPointUp(V5))
		{
			do
			V3 = V5, V5 = V5->ZVTNextSortedVtBead;
			while(V5);
			V3 = V3->ZVTMeInreverse;
		}
		else V3 = V5;
	}
	V3->ZVTAux = 1;
	if(!V3->ZVTStatus)
	{
		return 0;
	}
	for(V5 = V3; 1; V5 = FETCHSUCCESSOR(V5))
	{
		Lv = V5->ZVTFirstCoincident;
		if(!Lv)
		{
			V5 = V5->ZVTMeInreverse;
			Lv = V5->ZVTFirstCoincident;
		}
		if(Lv)break;
	}
	Gr_Com = Lv->ZLVMyLbBead->ZLBGrp;

	IN = MAKE_MASK(V3->ZVTStatus);
	if(Sh_Bl)
	{
		Sh_Xdir = floor((double)Sh_Xdir/(1L<<S_Factor) +0.5);
		if(Sh_Vt)
		{
			PVAStart = ZVTLIM;
			V5 = SPLITVT(V3, (Sh_Ip=IPOINT(Sh_Xdir, Sh_Ydir)));
			(*Sh_Au) = (void*)(Up? V5: V3);
			POINT_VECTOR_ANALYSIS(PVAStart);
		}
		PVAStart = ZVTLIM;
		V3 = INSERTV(Sh_Ip, Sh_Pm, 0);
		POINT_VECTOR_ANALYSIS(PVAStart);
	}
	Gr = LB->ZLBGrp;
	Nx = Gr;
	do
	{
		LB = Nx;
		Nx = Nx->ZLBNxt;

		LB->ZLBGrp = Gr_Com;
		Lv = LB->ZLBFirstLvBead;
		Le = Lv;
		do
		{
			(V3=Lv->ZLVMyVector)->ZVTStatus |=IN;
			(V3=V3->ZVTMeInreverse)->ZVTStatus |=IN;

			Lv = Lv->ZLVNextLvBead;
		}
		while(Lv!=Le);
	}
	while(Nx);
	LB->ZLBNxt = Gr_Com->ZLBNxt;
	Gr_Com->ZLBNxt = Gr;
	return 2;
}
void
Classify_Quick(void)
{
	long i;
	for(i = 1; i<ZLBLIM; ++i)
	{
		LbBead*Lb = GetLbBead(i);
		if(!Lb->ZLBFirstLvBead)
		{
			continue;
		}
		DUMMYJUNCTIONANALYSIS(Lb);
		Set_Rstat(Lb, 0);
	}
}
void
Classify_Main(void)
{
	long Lb;
	char Si;
	char Fl;
	LbGrp_Init();
	iniPvClass();
	for(Lb = 1; Lb<ZLBLIM; ++Lb)
	{
		long L;
		long M;
		LbBead*LB = (LbBead*)PTRAux_2[Lb];
		M = (L = LB->ZLBCO4)<<1;

		POINT_VECTOR_CLASS_2(LB, L, M);
	}
	Sh_Botm = 1;
	ixclass_start:
	Si = 0;
	iniFiXStatus();
	for(Lb = 1; Lb<PTRAux_2Lim; ++Lb)
	{
		void*&Au = PTRAux_2[Lb];
		if(Au)
		{
			Fl = Class_IX_Status((LbBead*)Au);
			if(Fl)Au = 0;
			if(Fl==2)Si = 1;
		}
	}
	if(!Si)
	{
		return;
	}
	++Sh_Botm;
	goto ixclass_start;
}
void
Classify_Polygons(char quickclassify) { 
	if(quickclassify) 
	{ 
		Classify_Quick();
	} 
	else 
	{ 
		Classify_Main(); 
	}
} 
void
Set_Rstat(LbBead*I, char z)
{
	LvBead*S;
	LvBead*K;
	VtBead*J;
	VtBead*R;
	char T;
	S = I->ZLBFirstLvBead;
	if(S==0)
	{
		return;
	}
	K = S;
	T = I->ZLBCO1;
	do
	{
		J = K->ZLVMyVector;
		R = J->ZVTMeInreverse;
		K = K->ZLVNextLvBead;
		if(z==7)
		{
			char f_1=CheckItSelf(I, J);
			char f_2=CheckItSelf(I, R);
			if(f_1&&f_2)continue;
		}
		if(T==-1)J=R;
		if(z)J->ZVTStatus|=1;
		else J->ZVTStatus =1;
	}
	while(K!=S);
}
