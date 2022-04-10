/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#include "stdafx.h" 

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fqeval.hpp"
#include "fxeval.hpp"
const char*fzinit="fzinit";
void PrintOk(void);
long fZiOutT;
long fZiInpT;
/**/
long XgDIV; /*GRID-BLOCK-SIZE-UNITS-WIDE*/
long YgDIV; /*GRID-BLOCK-SIZE*/
long XgCNT; /*TOTAL-BLOCKS-IN-X-DIRECTION*/
long YgCNT; /*TOTAL-BLOCKS-IN-Y-DIRECTION*/
long XgCNT_2; /*TOTAL-BLOCKS-IN-X-DIRECTION*/
long YgCNT_2; /*TOTAL-BLOCKS-IN-Y-DIRECTION*/
/**/
VtBead*SpVtIm; /*RIGHT-VT-VECTOR-MATCHED-LV-VECTOR*/
/**/
long YEND; /*END-Yg-FOR-LOOP*/
long YBEGIN; /*START-Yg*/
long YSTART; /*Y-COORD-SETTED-BY-PREVIOUSLY-COMPUTATING*/
long XSTART; /*X-COORD*/
long XFIRST; /*FIRST-X-COORD-OF-A-VECTOR*/
long YFIRST; /*FIRST-Y-COORD*/
long SpTurn; /*VECTOR-WAS-SWITCHED-FLAG*/
long XSECOND; /*SECOND-X*/
long YSECOND; /*SECOND-Y*/
long SP_XMIN; /*MINIMAL-Xg-FOR-THIS-POLY*/
long SP_XMAX; /*MAX-Xg*/
long SP_YMIN; /*MINIMAL-Yg-FOR-THIS-VECTOR*/
long SP_YMAX; /*MAX-Yg*/
/**/
long XROUND;
long YROUND;
double __slope__; /*SLOPE-OF-A-VECTOR*/
/**/
char Out_of_Bound;
char Out_of_Bound_Xmin;
char Out_of_Bound_Ymin;
char Out_of_Bound_Xmax;
char Out_of_Bound_Ymax;
/**/
char SP_SX;
char SP_SY;
/**/
long*ZXgCO2;
long*ZXgCO1;
long*ZXgCO0;
long*ZYgCO2;
long*ZYgCO1;
long*ZYgCO0;
/**/
struct GvBead
{
	GvBead*CO1;
	PtBead*CO2;
	PtBead*CO3;
}	;
GvBead**ZGaTBl;

struct _zAgtbl: public XmemoryAllocator<GvBead>
{
};

_zAgtbl ZGVCor;
long ZGvLim;

void Unload_LV_Polygons(long Xg, long Yg)
{
	long I;
	OutDeltaX = -ZXgCO0[Xg-1];
	OutDeltaY = -ZYgCO0[Yg-1];

	if(ZXgCO0[Xg]<=CXMIN)return;
	if(ZYgCO0[Yg]<=CYMIN)return;

	if(ZXgCO0[Xg-1]>=CXMAX)return;
	if(ZYgCO0[Yg-1]>=CYMAX)return;

	if(Out_of_Bound)
	{
		if(Out_of_Bound_Xmin)--Xg;
		if(Out_of_Bound_Ymin)--Yg;
	}
	C_Fragment = (Xg-1)*YBCNT + Yg-1;

	for(I = 1; I<ZLBLIM; ++I)
	{
		LbBead*Lb=GetLbBead(I);
		if(Lb->ZLBFirstLvBead==0)
		{
			continue;
		}
		if(LbBead_Unload2InputXY(Lb))
		{
			++fZiOutT;
			MaSCOT_IO.Dix_output();
		}
	}
}
void
DETERMINEGRIDVECTOR(long Xg, long Yg)
{
	long IX;
	long IY;
	long JY;
	long JX;

	long IY0;
	long IX1;
	long IY1;

	IX1 = ZXgCO0[Xg];
	IY1 = ZYgCO0[Yg], IY0 = ZYgCO0[Yg-1];

	IX = XSTART;
	IY = YSTART;

	if(!SP_SX) JX = IX, JY = YSECOND<IY1 ? YSECOND : IY1;
	else
	if(!SP_SY) JY = IY, JX = XSECOND<IX1 ? XSECOND : IX1;
	else
	{
#define EVALUATEX(Y) (floor(((double)((Y)-YFIRST))/__slope__+XFIRST+0.5))
#define EVALUATEY(X) (floor(__slope__*((X)-XFIRST)+YFIRST+0.5))
		if(IX1<XSECOND) JX = IX1, JY = (long)EVALUATEY(JX);
		else JX = XSECOND, JY = YSECOND;

		if(SP_SY==+1)
		{
			if(JY>IY1) JY = IY1, JX = (long)EVALUATEX(JY);
		}
		else
		if(JY<IY0) JY = IY0, JX = (long)EVALUATEX(JY);
	}
	XSTART = JX, YSTART = JY;

	if(JX==IX&&JY==IY)
	{
		return;
	}
	VtBead*VT = SpVtIm;
	if(JX!=XSECOND||JY!=YSECOND)
	{
		SpVtIm = SPLITVT(SpVtIm, IPOINT(JX, JY));
	}
	if(SpTurn)
	{
		VT = VT->ZVTMeInreverse;
	}
	if(IX==JX&&JX==IX1&&Xg!=XgCNT)
	{
		Xg += XROUND;
	}
	GvBead*Gv = &ZGVCor[ZGvLim++];

	Gv->CO2 = VT->ZVTCO1;
	Gv->CO3 = VT->ZVTCO2;

	long Gb=(--Xg)*YgCNT+(--Yg);
	Gv->CO1 = ZGaTBl[Gb];
	ZGaTBl[Gb] = Gv;
}
void
SetGaFlags(long Xg)
{
	long Yg;
	long Cnt;
	long SI;

	if(XFIRST==XSECOND)SP_SX = 0;
	else SP_SX =+1;
	if(YFIRST<YSECOND) SP_SY =+1;
	else
	if(YFIRST>YSECOND) SP_SY =-1;
	else SP_SY = 0;

	if(!SP_SX&&!SP_SY)
	{
		return;
	}
	SI = YBEGIN<YEND? +1: -1;
	Cnt = (SI*(YEND-YBEGIN))+1;

	for(Yg=YBEGIN; Cnt; --Cnt, Yg+=SI)
	{
		DETERMINEGRIDVECTOR(Xg, Yg);
	}
}
typedef void GvS(LvBead*Lv);
GvS * GvSplit;
void
GvSplit_1(LvBead*LV)
{
	long X1, X2, Y1, Y2, S2, DX, DY;
	long Xg, IX, IY, Xg1, Xg2, Yg1, Yg2;

	X1 = LV->ZLVPoint1->ZPTCO1;
	Y1 = LV->ZLVPoint1->ZPTCO2;
	Y2 = LV->ZLVPoint2->ZPTCO2;
	X2 = LV->ZLVPoint2->ZPTCO1;
	DX = X2-X1;
	DY = Y2-Y1;
	XROUND = 0;
	if(!DX) /*X1-Eg-X2*/
	{
		YROUND = 0;

		if(Y1>Y2) S2 =-STATUS, l_swap(Y1, Y2);
		else S2 = STATUS;
		if(S2==+1) Xg1 = ((X1-CXMIN)/XgDIV)+1;
		else Xg1 = ((X1-CXMIN-1)/XgDIV)+1;

		Xg2 = Xg1;
		Yg1 = ((Y1-CYMIN)/YgDIV)+1;
		Yg2 = ((Y2-CYMIN-1)/YgDIV)+1;
	}
	else /*X1-NE-X2*/
	{
		if(X1>X2) S2 =-STATUS, l_swap(X1, X2), l_swap(Y1, Y2);
		else S2 = STATUS;

		Xg1 = ((X1-CXMIN)/XgDIV)+1;
		Xg2 = ((X2-CXMIN-1)/XgDIV)+1;

		YROUND = S2==+1? -1: 0;
		if(!DY) /*Y1-Eg-Y2*/
		{
			if(S2==+1) Yg1 = ((Y1-CYMIN-1)/YgDIV)+1;
			else Yg1 = ((Y1-CYMIN)/YgDIV)+1;
			Yg2 = Yg1;
		}
		else /*Y1-NE-Y2-(ALSO-X1-NE-X2)*/
		{
			if(Y2>Y1) /*POSITIVE-SLOPE*/
			{
				Yg1 = ((Y1-CYMIN)/YgDIV)+1;
				Yg2 = ((Y2-CYMIN-1)/YgDIV)+1;
				XROUND = S2==+1? +1: 0;
			}
			else /*NEGATIVE-SLOPE*/
			{
				Yg1 = ((Y1-CYMIN-1)/YgDIV)+1;
				Yg2 = ((Y2-CYMIN)/YgDIV)+1;
				XROUND = S2==+1? 0: +1;
			}
		} /*END-Y1-NE-Y2*/
	} /*END-X1-NE-X2*/
	if(Xg1<SP_XMIN) SP_XMIN = Xg1;
	if(Yg1<SP_YMIN) SP_YMIN = Yg1;
	if(Yg2>SP_YMAX) SP_YMAX = Yg2;
	if(Yg2<SP_YMIN) SP_YMIN = Yg2;
	if(Yg1>SP_YMAX) SP_YMAX = Yg1;
	if(Xg2>SP_XMAX) SP_XMAX = Xg2;

	XFIRST = XSTART = X1;
	YFIRST = YSTART = Y1;

	XSECOND = X2;
	YSECOND = Y2;

	SpTurn = S2!=STATUS;

	SpVtIm = SpTurn? LV->ZLVMyVector->ZVTMeInreverse: LV->ZLVMyVector;
	__slope__ = DX? (DY? ((double)DY)/DX: 0): LONG_MAX;

	/*FOR-EACH-BUT-THE-LAST-Xg-VALUE*/
	YEND = YBEGIN = Yg1;
	if(Yg1==Yg2)
	for(Xg = Xg1; Xg<Xg2; ++Xg)
	{
		SetGaFlags(Xg);
	}
	else
	for(Xg = Xg1; Xg<Xg2; ++Xg)
	{
		IX = ZXgCO0[Xg];
		IY = (long)floor(__slope__*(IX-X1)+Y1 +0.5);
		YEND = (IY-CYMIN+YROUND)/YgDIV+1;
		SetGaFlags(Xg);
		YBEGIN = YEND;
	}
	/*NOW-FOR-THE-LAST-Xg-VALUE*/
	Xg = Xg2;
	YEND = Yg2;
	SetGaFlags(Xg);
}
void
GvSplit_2(LvBead*LV)
{
	char SW;
	long X1, X2, Y1, Y2, S2, DX, DY;
	long Xg, IX, IY, Xg1, Xg2, Yg1, Yg2;
	long XR, YR, XB, YB;

	X1 = LV->ZLVPoint1->ZPTCO1;
	Y1 = LV->ZLVPoint1->ZPTCO2;
	Y2 = LV->ZLVPoint2->ZPTCO2;
	X2 = LV->ZLVPoint2->ZPTCO1;
	DX = X2-X1;
	DY = Y2-Y1;
	SW = 0;
	YROUND = XROUND = 0;
	if(!DX) /*X1-Eg-X2*/
	{
		if(Y1>Y2) S2 =-STATUS, l_swap(Y1, Y2);
		else S2 = STATUS;
		XR = S2==+1;
		YR = 1;
	}
	else /*X1-NE-X2*/
	{
		if(X1>X2) S2 =-STATUS, l_swap(X1, X2), l_swap(Y1, Y2);
		else S2 = STATUS;
		XR = 1;
		if(!DY) /*Y1-Eg-Y2*/ YR = S2==-1;
		else /*Y1-NE-Y2-(ALSO-X1-NE-X2)*/
		{
			YR = 1;
			if(Y2<Y1) /*NEGATIVE-SLOPE*/
			{
				l_swap(Y1, Y2);
				SW = 1;
				XROUND = S2==+1? 0: +1;
			}
			else XROUND = S2==+1? +1: 0;
		} /*END-Y1-NE-Y2*/
	} /*END-X1-NE-X2*/
	YB = ZYgCO0[1];
	XB = ZXgCO0[1];
	Yg1 = 1;
	while(Y1>YB)
	{
		YB = ZYgCO0[++Yg1];
	}
	if(YR&&YB==Y1)YB = ZYgCO0[++Yg1];

	Yg2 = Yg1;
	while(Y2>YB)
	{
		YB = ZYgCO0[++Yg2];
	}
	Xg1 = 1;
	while(X1>XB)
	{
		XB = ZXgCO0[++Xg1];
	}
	if(XR&&X1==XB)XB = ZXgCO0[++Xg1];

	Xg2 = Xg1;
	while(X2>XB)
	{
		XB = ZXgCO0[++Xg2];
	}
	if(SW)
	{
		l_swap(Y1, Y2);
		l_swap(Yg1, Yg2);
	}
	if(Xg1<SP_XMIN)SP_XMIN = Xg1;
	if(Yg1<SP_YMIN)SP_YMIN = Yg1;
	if(Yg2>SP_YMAX)SP_YMAX = Yg2;
	if(Yg2<SP_YMIN)SP_YMIN = Yg2;
	if(Yg1>SP_YMAX)SP_YMAX = Yg1;
	if(Xg2>SP_XMAX)SP_XMAX = Xg2;

	XFIRST = XSTART = X1;
	YFIRST = YSTART = Y1;

	XSECOND = X2;
	YSECOND = Y2;

	SpTurn = S2!=STATUS;

	SpVtIm = SpTurn? LV->ZLVMyVector->ZVTMeInreverse: LV->ZLVMyVector;
	__slope__ = DX? (DY? ((double)DY)/DX: 0): LONG_MAX;

	/*FOR-EACH-BUT-THE-LAST-Xg-VALUE*/
	YEND = YBEGIN = Yg1;
	if(Yg1==Yg2)
	for(Xg=Xg1; Xg<Xg2; ++Xg)SetGaFlags(Xg);
	else
	for(Xg=Xg1; Xg<Xg2; ++Xg)
	{
		IX = ZXgCO0[Xg];
		IY = (long)floor(__slope__*(IX-X1)+Y1+0.5);
		if(SW) /*NEGATIVE-SLOPE*/
		{
			YB = ZYgCO0[YEND-1];
			while(IY<YB)YB = ZYgCO0[--YEND-1];
			if(IY==YB&&S2==+1) --YEND;
		}
		else
		{
			YB = ZYgCO0[YEND];
			while(IY>YB)YB = ZYgCO0[++YEND];
			if(IY==YB&&S2==-1) ++YEND;
		}
		SetGaFlags(Xg);
		YBEGIN = YEND;
	}
	/*NOW-FOR-THE-LAST-Xg-VALUE*/
	Xg = Xg2;
	YEND = Yg2;
	SetGaFlags(Xg);
}
void
DEFINELBGRIDDATA(LbBead*LB)
{
	LvBead*LE;
	LvBead*LV;
	LvBead*LN;
	LE = LV = LB->ZLBFirstLvBead;
	if(LE)
	do
	{
		LN = LV->ZLVNextLvBead;
		GvSplit(LV);
		LV = LN;
	}
	while(LV!=LE);
}
long ZB_YUP, ZB_YLO, ZB_XRI, ZB_XLE;
void
LOADGRIDBOUNDARY(long Xg, long Yg)
{
/*Xg-RIGHT-BOUNDARY; Yg-UPPER-BOUNDARY*/
	ZLDLIM = 5;
	ZB_YUP=ZLDCO1[1]=ZLDCO1[2]=ZYgCO0[Yg]; /*UPPER*/
	ZB_YLO=ZLDCO1[0]=ZLDCO1[3]=ZYgCO0[Yg-1]; /*LOWER*/
	ZB_XRI=ZLDCO0[2]=ZLDCO0[3]=ZXgCO0[Xg]; /*RIGHT*/
	ZB_XLE=ZLDCO0[0]=ZLDCO0[1]=ZXgCO0[Xg-1]; /*LEFT*/
	ZLDCO0[4] = ZLDCO0[0];
	ZLDCO1[4] = ZLDCO1[0];
}
void
GRIDBLOCKGEN(long Xg, long Yg)
{
	long S = STATUS;
	LOADGRIDBOUNDARY(Xg, Yg);
	C_Fragment = (Xg-1)*YgCNT+(Yg-1);
	OutDeltaX = -ZB_XLE;
	OutDeltaY = -ZB_YLO;
	STATUS = 1;
	++fZiOutT;
	MaSCOT_IO.Dix_output();
	STATUS = S;
}
long
SETGRIDMODE(long Xg)
{
/*Xg-RIGHT-BOUNDARY*/
	LbBead*LB;
	LvBead*LV;
	LvBead*LE;
	long X1, X2, XB;
	long Lb;
	char IS;
	XB = ZXgCO0[Xg];
	for(Lb = 1; Lb<ZLBLIM; ++Lb)
	{
		LB = GetLbBead(Lb);
		if((LV = LE = LB->ZLBFirstLvBead)!=0)
		{
			X1 = LV->ZLVPoint1->ZPTCO1;
			do
			{
				X2 = LV->ZLVPoint2->ZPTCO1;
				IS = (X1==XB)&&(X1==X2);
				if(IS&&!LV->ZLVMyVector->ZVTStatus)
				{
					return(1);
				}
				LV = LV->ZLVNextLvBead;
				X1 = X2;
			}
			while(LE!=LV);
		}
	}
	return(0);
}
void
LOADGRIDVECTORS(long Gb)
{
	GvBead*Gv;
	PtBead*P1;
	PtBead*P2;
	PtBead*P3;
	PtBead*P4;
	VtBead*V3;
	VtBead*V4;
	unsigned char LS;
	unsigned char RS;
	long X1, Y1, X2, Y2;
	PTRAux_2Lim = 0;
	RS = STATUS==+1? 1: 0;
	LS = STATUS==+1? 0: 1;
	P3 = P4 = 0;
	for(Gv = ZGaTBl[Gb]; Gv; Gv = Gv->CO1)
	{
		P1 = Gv->CO2;
		P2 = Gv->CO3;
		X1 = P1->ZPTCO1;
		X2 = P2->ZPTCO1;

		if(P3!=P1&&P4!=P1)
		{
			Y1 = P1->ZPTCO2;
			if(X1==ZB_XLE||
			   X1==ZB_XRI||
			   Y1==ZB_YUP||
			   Y1==ZB_YLO)
			PTRAux_2[PTRAux_2Lim++] = (void*)P1;

			P1->ZPTFirstSortedVtBead = 0;
		}
		if(P3!=P2&&P4!=P2)
		{
			Y2 = P2->ZPTCO2;
			if(X2==ZB_XLE||
			   X2==ZB_XRI||
			   Y2==ZB_YUP||
			   Y2==ZB_YLO)
			PTRAux_2[PTRAux_2Lim++] = (void*)P2;

			P2->ZPTFirstSortedVtBead = 0;
		}
		P3 = P1, P4 = P2;

		GetVtPair(V3, V4);
		if(X1>X2)
		{
			VtBead*V0=V3;V3=V4;V4=V0;
		}
		V3->ZVTFirstCoincident = V4->ZVTFirstCoincident = 0;
		V3->ZVTMeInreverse = V4;
		V4->ZVTMeInreverse = V3;
		V4->ZVTCO2 = V3->ZVTCO1 = P1;
		V4->ZVTCO1 = V3->ZVTCO2 = P2;
		V3->ZVTStatus = RS;
		V4->ZVTStatus = LS;
	} /*END-FOR-EACH-GRID-VECTOR*/
}
void
GRID_INTERSECTION(void)
{
	LvBead*V;
	LvBead*E;
	PtBead*P;
	LvBead*T;
	long X,Y,A;
	VtBead*V3;
	VtBead*V4;
	PtBead*p1;
	PtBead*p2;
	long x1, x2, y1, y2;
	char horizontal;
	LbBead*L;
	L = GetLbBead(1);
	V = E = L->ZLBFirstLvBead;
	do
	{
		p1 = V->ZLVPoint1;
		p2 = V->ZLVPoint2;

		x1 = p1->ZPTCO1;
		x2 = p2->ZPTCO1;
		y1 = p1->ZPTCO2;
		y2 = p2->ZPTCO2;

		if(y1==y2)horizontal = 1;
		else horizontal = 0;
		for(A = 0; A<PTRAux_2Lim; ++A)
		{
			if((P=(PtBead*)PTRAux_2[A])==0)
			{
				continue;
			}
			X = P->ZPTCO1;
			Y = P->ZPTCO2;

			if(horizontal)
			{
				if(y1!=Y)
				{
					continue;
				}
				if(x1>=X&&x2>=X)continue;
				if(x1<=X&&x2<=X)continue;
			}
			else
			{
				if(x1!=X)
				{
					continue;
				}
				if(y1>=Y&&y2>=Y)continue;
				if(y1<=Y&&y2<=Y)continue;
			}
			T = GetLvBead(ZLVLIM++);

			T->ZLVNextConcident = 0;
			T->ZLVNextLvBead = V->ZLVNextLvBead;
			V->ZLVNextLvBead = T;
			T->ZLVMyLbBead = L;
			T->ZLVPoint2 = V->ZLVPoint2;
			V->ZLVPoint2 = P;
			T->ZLVPoint1 = P;

			PTRAux_2[A] = 0;

			p2 = P;
			x2 = p2->ZPTCO1;
			y2 = p2->ZPTCO2;
		}
		GetVtPair(V3, V4);

		V3->ZVTFirstCoincident = V, V4->ZVTFirstCoincident = 0;
		V3->ZVTMeInreverse = V4;
		V4->ZVTMeInreverse = V3;
		V4->ZVTCO2 = V3->ZVTCO1 = p1;
		V4->ZVTCO1 = V3->ZVTCO2 = p2;
		V3->ZVTStatus = V4->ZVTStatus = 0L;

		V = V->ZLVNextLvBead;
	}
	while(V!=E);
}
void
REBUILD_GRIDTABLE(void)
{
	long Y,X,L;
	for(Y = SP_YMIN; Y<=SP_YMAX; ++Y)
	for(X = SP_XMIN; X<=SP_XMAX; ++X)
	{
		long G=(X-1)*YgCNT+(Y-1);
		ZGaTBl[G] = 0;
	}
	Set_Rstat(GetLbBead(1), 7);
	FORCE_C_POLYGONS(0);

	SP_XMIN=XgCNT, SP_XMAX=0;
	SP_YMIN=YgCNT, SP_YMAX=0;

	ZGvLim = 1;
	for(L = 1; L<ZLBLIM; ++L)
	{
		LbBead*Lb = GetLbBead(L);
		if(!Lb->ZLBFirstLvBead)
		{
			continue;
		}
		DUMMYJUNCTIONANALYSIS(Lb);
		DEFINELBGRIDDATA(Lb);
	}
	STATUS = +1;
}
void REMOVE_GRID_POINT(long i)
{
	PtBead*P = GetPtBead(i);
	PtBead*p;
	long X;
	long Y;
	long h;
	register long S;
	X = P->ZPTCO1;
	Y = P->ZPTCO2;
	S = X+Y;
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
	p = ZPTHASH[h];
	if(p==P)
	{
		ZPTHASH[h] = P->ZPTNxt;
		return;
	}
	while(p&&p->ZPTNxt!=P)
	{
		p = p->ZPTNxt;
	}
	if(!p)zabort(fzinit,__LINE__);
	p->ZPTNxt = P->ZPTNxt;
}
void
CLEAR_PV_POINTERS(LbBead*L)
{
	LvBead*V;
	LvBead*E;
	V=E=L->ZLBFirstLvBead;
	if(!E) return;
	do
	{
		V->ZLVPoint1->ZPTFirstSortedVtBead = 0;
		V = V->ZLVNextLvBead;
	}
	while(V!=E);
}
long FULLANALYSIS(long X, long Y)
{
	long B, M, S, P;
	ZLBLIM=ZLVLIM=1;
	ZVTLIM = 2;
	LOADGRIDBOUNDARY(X, Y);

	B = (X-1)*YgCNT+(Y-1);
	LOADGRIDVECTORS(B);
	S = STATUS;
	P = ZPTLIM;
	STATUS = 1;
	Loadmaster();
	CLEAR_PV_POINTERS(GetLbBead(1));

	GRID_INTERSECTION();

	POINT_VECTOR_ANALYSIS(2, 0, 1);
	FORCE_C_POLYGONS(0);
	Unload_LV_Polygons(X, Y);
	M = SETGRIDMODE(X);
	STATUS = S;
	S = P;
	while(P!=ZPTLIM)REMOVE_GRID_POINT(P++);
	ZPTLIM = S;

	return(M);
}
void
INSERT_VECTORS(LbBead*Lb)
{
	LvBead*E;
	LvBead*V;
	V = E = Lb->ZLBFirstLvBead;
	do
	{
		INSERTV(V->ZLVPoint1, V->ZLVPoint2, V);
		V = V->ZLVNextLvBead;
	}
	while(V!=E);
}
char
SetSplitParam(void)
{
	if(PXMAX<=CXMIN)return 0;
	if(PXMIN>=CXMAX)return 0;
	if(PYMAX<=CYMIN)return 0;
	if(PYMIN>=CYMAX)return 0;
	long I=0;
	if(PYMIN<CYMIN||PYMAX>CYMAX)++I;
	if(PXMIN<CXMIN||PXMAX>CXMAX)++I;

	if(I)GvSplit = GvSplit_2;
	else GvSplit = GvSplit_1;

	if(I)ZXgCO0 = ZXgCO1, ZYgCO0 = ZYgCO1;
	else ZXgCO0 = ZXgCO2, ZYgCO0 = ZYgCO2;

	if(I)XgCNT = XgCNT_2, YgCNT = YgCNT_2;
	else XgCNT = XBCNT, YgCNT = YBCNT;

	if(!STATUS)STATUS=1;

	Out_of_Bound = I!=0;
	return 1;
}
void
Reverse_Lv_Chain(LbBead*Lb)
{
	LvBead*Lp;
	LvBead*Lo;
	LvBead*Lv;
	Lp=Lb->ZLBFirstLvBead;
	Lo=Lp?Lp->ZLVNextLvBead:0;
	Lv=Lo?Lo->ZLVNextLvBead:0;
	if(!Lv||Lv==Lp)
	{
		return;
	}
	do
	{
		Lo->ZLVNextLvBead=Lp;
		PtBead*Pt=Lo->ZLVPoint1;Lo->ZLVPoint1=Lo->ZLVPoint2;Lo->ZLVPoint2=Pt;
		Lp=Lo;
		Lo=Lv;
		Lv=Lv?Lv->ZLVNextLvBead:0;
	}
	while(Lo&&Lv!=Lp);
	if(!Lo)
	{
		zabort(fzinit,__LINE__);
	}
	Lb->ZLBStatus*=-1;
}
void
SPLIT_POLYGON(char params_were_set)
{
	long X;
	long Y;
	long B;
	long M;
	long S;
	long E;
	long T = C_Fragment;
	char PV_Error = 0; 

	if(!params_were_set) { 
		if(!SetSplitParam())
		{
			return;
		}
	} 
	ZLBLIM=ZLVLIM=ZPTLIM=ZGvLim=1;
	ZVTLIM = S = 2;

	SP_XMIN=XgCNT, SP_XMAX=0;
	SP_YMIN=YgCNT, SP_YMAX=0;

	if(!Out_of_Bound&&STATUS!=-1)
	{
		X = (PXMIN-CXMIN)/XgDIV;
		Y = (PYMIN-CYMIN)/YgDIV;
		B = X*YgCNT+Y;

		X = (PXMAX-CXMIN-1)/XgDIV;
		Y = (PYMAX-CYMIN-1)/YgDIV;
		if(GvSplit==GvSplit_1&&B==(X*YgCNT+Y))
		{
			C_Fragment = B;
			OutDeltaX = -ZXgCO0[X];
			OutDeltaY = -ZYgCO0[Y];
			++fZiOutT;
			MaSCOT_IO.Dix_output();
			C_Fragment = T;
			return;
		}
	}
	CleanZPTHASH();
	Loadmaster();
	if(STATUS==-1)
	{
		STATUS=1;
		Reverse_Lv_Chain(GetLbBead(1));
	}
	INSERT_VECTORS(GetLbBead(1));

	POINT_VECTOR_ANALYSIS(S, 0);
	S = ZVTLIM;
	DEFINELBGRIDDATA(GetLbBead(1));

	if(SP_XMIN==SP_XMAX&&SP_YMIN==SP_YMAX)
	{
		B = (SP_XMIN-1)*YgCNT+(SP_YMIN-1);
		ZGaTBl[B] = 0;

		C_Fragment = B;
		OutDeltaX = -ZXgCO0[SP_XMIN-1];
		OutDeltaY = -ZYgCO0[SP_YMIN-1];

		if(!Out_of_Bound)
		{
			++fZiOutT;
			MaSCOT_IO.Dix_output();
		}
		C_Fragment = T;
		return;
	}
	E = 0;
	PV_Error = POINT_VECTOR_ANALYSIS(S, 0);

	if(PV_Error)
	{
		E |= POINT_VECTOR_CLASS_1(GetLbBead(1));
	}
	if (E) 
	{
		REBUILD_GRIDTABLE();
	}

	for(Y = SP_YMIN; Y<=SP_YMAX; ++Y)
	{
		M = 0;
		for(X = SP_XMIN; X<=SP_XMAX; ++X)
		{
			B = (X-1)*YgCNT+(Y-1);
			if(ZGaTBl[B])
			{
				M = FULLANALYSIS(X, Y);
				ZGaTBl[B] = 0;
			}
			else
			if(M) GRIDBLOCKGEN(X, Y);
		} /*END-LOOP-OVER-Xg*/
	} /*END-LOOP-OVER-Yg*/
	C_Fragment = T;
}
char
SET_Xg_Yg(long Lay)
{
	long Z;
	long X;
	long Y;
	long x;
	long y;
	long A;
	XgDIV = XBSIZ;
	YgDIV = YBSIZ;

	if(MaSCOT_IO.I_XBCNT(Lay)==XBCNT&&MaSCOT_IO.I_YBCNT(Lay)==YBCNT)
	if(MaSCOT_IO.I_CXMIN(Lay)>=CXMIN&&MaSCOT_IO.I_CXMAX(Lay)<=CXMAX)
	if(MaSCOT_IO.I_CYMIN(Lay)>=CYMIN&&MaSCOT_IO.I_CYMAX(Lay)<=CYMAX)
	{
		return 0;
	}
	Z = 0;
	if(MaSCOT_IO.I_CXMIN(Lay)<CXMIN)Z|=0x1;
	if(MaSCOT_IO.I_CXMAX(Lay)>CXMAX)Z|=0x2;
	if(MaSCOT_IO.I_CYMIN(Lay)<CYMIN)Z|=0x4;
	if(MaSCOT_IO.I_CYMAX(Lay)>CYMAX)Z|=0x8;

	Out_of_Bound_Xmin = Z&0x1;
	Out_of_Bound_Xmax = Z&0x2;
	Out_of_Bound_Ymin = Z&0x4;
	Out_of_Bound_Ymax = Z&0x8;

	X = XBCNT+1;
	Y = YBCNT+1;

	if(Z&0x1)++X;
	if(Z&0x2)++X;
	if(Z&0x4)++Y;
	if(Z&0x8)++Y;

	XgCNT_2 = X-1;
	YgCNT_2 = Y-1;

	if(!ZXgCO1)ZXgCO1 = new long[X];
	if(!ZYgCO1)ZYgCO1 = new long[Y];

	A = (X-1)*(Y-1);

	ZXgCO2 = ZXgCO1;
	ZYgCO2 = ZYgCO1;

	if(Z&0x1)ZXgCO2 += 1;
	if(Z&0x4)ZYgCO2 += 1;
	X = 0;
	x = CXMIN;
	while(X<=XBCNT)ZXgCO2[X++] = x, x += XBSIZ;
	Y = 0;
	y = CYMIN;
	while(Y<=YBCNT)ZYgCO2[Y++] = y, y += YBSIZ;

	if(Z&0x1)ZXgCO1[0] =-LONG_MAX;
	if(Z&0x2)ZXgCO2[X] = LONG_MAX;

	if(Z&0x4)ZYgCO1[0] =-LONG_MAX;
	if(Z&0x8)ZYgCO2[Y] = LONG_MAX;

	if(!ZGaTBl)ZGaTBl = new GvBead*[A];
	memset(ZGaTBl, 0, A*sizeof(ZGaTBl[0]));

	return 1;
}
void
Destroy_GridStorage(void)
{
	delete ZXgCO1; ZXgCO1=0;
	delete ZYgCO1; ZYgCO1=0;
	delete ZGaTBl; ZGaTBl=0;
	ZGVCor._Release();
}
void
fzinit_print_total(void)
{
MASLog("\rTOTAL: input %8ld, output %8ld", fZiInpT, fZiOutT);
}
void
FragmPolygons(long n)
{
	STATUS = 1L;
	ZPBLIM = 63;
	IPOINT = IPOINT4;

	fZiInpT=fZiOutT=0;

	MaSCOT_IO.D_Link(n);
	MaSCOT_IO.SetAbsDelta(n);
	IEvaluater.__SetVT0LB0__();
MASLog("!!! SPLIT_POLYGONS STARTED ON <%s> !!!\n",MaSCOT_IO.I_Layer[n].Name);
MASLog("\tblocks-cnt on x %7ld, on y %7ld\n",XBCNT,YBCNT);
MASLog("\tblock-size on x %7ld, on y %7ld\n",XBSIZ,YBSIZ);
	while(C_Fragment!=LONG_MAX)
	{
		MaSCOT_IO.I_READ_P(0,1);
		if(ZLDLIM==0)
		{
			MaSCOT_IO.SetAbsDelta(n);
			continue;
		}
		LDMake_ClosedPolygon();
		SPLIT_POLYGON(0);
		++fZiInpT;
if(!(fZiInpT%117)||!(fZiOutT%137))fzinit_print_total();
	}
	fzinit_print_total();
	ZPBLIM = 8191;
	__FINISHEDNORMALLY__();
}
