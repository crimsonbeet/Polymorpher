#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif
#ifndef LONG_MAX
#define LONG_MAX ((1l<<30)+((1l<<30)-1))
#endif

#include "tom_aray.hpp"
/**/
struct VtBead;
struct LvBead;
struct LbBead;
struct PtBead
{
	long ZPTCO1;/*x.*/
	long ZPTCO2;/*y*/

	VtBead*ZPTCO3;/*first Vt bead for this point(chain head).*/
	PtBead*ZPTNxt;/*second Pt Bead in hash class.*/
}	;
struct VtBead
{
	char ZVTAux;/*traversal status.*/
	long ZVTPos;/*pos in ZVtTBl.*/
	unsigned char ZVTCO4;/*right status.*/

	PtBead*ZVTCO1;/*first point.*/
	PtBead*ZVTCO2;/*second point.*/
	LvBead*ZVTCO5;/*FIRST POLYGON VECTOR BEAD CHAIN OF COINCIDENT VECTORS.*/
	VtBead*ZVTCO6;/*ASSOCIATED VECTOR Of REVERSE DIRECTION.*/
	VtBead*ZVTNxt;/*POINTER TO NEXT Vt BEAD FOR THIS POINT(COUNTERCLOCKWISE).*/
}	;
struct LvBead
{
	LvBead*ZLVCO1;/*NEXT POLYGON VECTOR(LV) BEAD(CHAIN HEAD IS ZLBCO3!CIRCULAR)*/
	VtBead*ZLVCO2;/*associated Vt VECTOR.*/
	LvBead*ZLVCO3;/*NEXT COINCIDENT VECTOR PTR(CHAIN HEAD IS ZVTCO5).*/
	PtBead*ZLVCO5;/*first point.*/
	PtBead*ZLVCO6;/*second point.*/
	LbBead*ZLVCO7;/*pointer to polyogin bead.*/
	LbBead*ZLVAux;/*auxiliary pointer.*/
}	;
struct LbBead
{
	unsigned char ZLBCO4;
	char ZLBCO1;
	LvBead*ZLBCO3; /*POINTER TO THE FIRST VECTOR BEAD.*/
	PtBead*ZLBXym; /*lower left point pointer.*/
	LbBead*ZLBNxt; /*next in class.*/
	LbBead*ZLBGrp; /*pointer to the first in class.*/
}	;
class _zAvtbl: public _rCash
{
public:
	_zAvtbl():_rCash(4096/sizeof(VtBead), 512){};
	~_zAvtbl(){};
	VtBead& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 4096/sizeof(VtBead));
		}
		return *((VtBead*)aHot+(i-lo));
	}
}	;
class _zAltbl: public _rCash
{
public:
	_zAltbl():_rCash(4096/sizeof(LvBead), 256){};
	~_zAltbl(){};
	LvBead& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 4096/sizeof(LvBead));
		}
		return *((LvBead*)aHot+(i-lo));
	}
}	;
class _zAbtbl: public _rCash
{
public:
	_zAbtbl():_rCash(4096/sizeof(LbBead), 128){};
	~_zAbtbl(){};
	LbBead& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 4096/sizeof(LbBead));
		}
		return *((LbBead*)aHot+(i-lo));
	}
}	;
class _zAptbl: public _rCash
{
public:
	_zAptbl():_rCash(4096/sizeof(PtBead), 256){};
	~_zAptbl(){};
	PtBead& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 4096/sizeof(PtBead));
		}
		return *((PtBead*)aHot+(i-lo));
	}
}	;
extern void zabort(void);
char*fqeval = "fqeval";
void(*zAbort_proc)(void);
void
zAbort(char*ids, long lin)
{
      fprintf(stderr, "\nModule %s: %ld", ids, lin);
      if(zAbort_proc)
      zAbort_proc();
      zabort();
}
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
_zAvoid PTRAux_1;
_zAvoid PTRAux_2;
_sAlong INDAux_3;
_zAvtbl ZVtTBl;
_zAltbl ZLvTBl;
_zAbtbl ZLbTBl;
_zAptbl ZPtTBl;
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
PtBead**ZPBCOR;
#ifndef __BORLANDC__
long ZPBLIM = 8191;
long __ZPBLIM__ = 8191;
#else
long ZPBLIM = 1023;
long __ZPBLIM__ = 1023;
#endif
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
long Lv_Ymin;
long Lv_Ymax;
long Lv_Xmin;
long Lv_Xmax;

long Vt_Ymax;
long Vt_Ymin;
VtBead*Vt_Last;
VtBead*Vt_Auxx;

PtBead*_P1_;
PtBead*_P2_;
long _X1_;
long _Y1_;
long _X2_;
long _Y2_;
char _In_;
long _Lf_;

LvBead*_Lv_;
VtBead*_Vt_;

char Lv_Swi;
char Vt_Swi;

double Lv_Y21;
double Lv_X21;
double Lv_D21;
/**/
char Lv_Ver;
char Lv_Gor;
char Lv_Ort;
char I_Type;
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
VtBead**Lf_Rolld;
VtBead**Lf_ROLLD;
VtBead**Lf_First;
unsigned short*Lf_Stack;
unsigned short*Lf_Upper;
unsigned short*Lf_Lower;

long*Lf_Ymidl;
char*Lf_Sdown;

char Lf_Factor;

unsigned short Lf_Lim;
unsigned short LF_LIM;
unsigned short Tree_next_pos;

char Initialized_hash;
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
#define FETCHSUCCESSOR(VT) ((VT)->ZVTNxt?(VT)->ZVTNxt:(VT)->ZVTCO1->ZPTCO3)
#define _EQ_(VA,VL,EN) (((VA)>(VL)-(EN))&&((VA)<(VL)+(EN)))
#define FetchReverseLv(LV) ((LV)->ZLVCO2->ZVTCO6->ZVTCO5)
/**/
PtBead*__GPT__(long Pt)
{
	PtBead*pt = &ZPtTBl[Pt];
	return pt;
}
LvBead*__GLV__(long Lv)
{
	LvBead*lv = &ZLvTBl[Lv];
	return lv;
}
LbBead*__GLB__(long Lb)
{
	LbBead*lb = &ZLbTBl[Lb];
	return lb;
}
VtBead*__GVT__(long Vt)
{
	VtBead*vt = &ZVtTBl[Vt];
	return vt;
}
VtBead*__VT0__;
LbBead*__LB0__;
union AUXObj
{
	long L;
	void*P;
}	;
typedef char GT(AUXObj, AUXObj, char);
GT * Gt;
typedef void GT_SWAP(long, long);
GT_SWAP * GtSwap;
typedef AUXObj GT_GET(long);
GT_GET * GtGetVal;
long
GetMinXLv(LvBead*Lv)
{
	register long X1=(Lv->ZLVCO5)->ZPTCO1;
	register long X2=(Lv->ZLVCO6)->ZPTCO1;
	return X1<X2? X1: X2;
}
AUXObj GtGetXLv(long k)
{
	AUXObj T;
	T.L = GetMinXLv((LvBead*)PTRAux_1[k]);
	return T;
}
void
GtSwapLv(long k, long m)
{
	void*&K = PTRAux_1[k];
	void*&M = PTRAux_1[m];
	void*T=K;K=M;M=T;
}
char
GtXLv(AUXObj k, AUXObj m, char f)
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
void
GtSwapVt(long k, long m)
{
	void*&K = PTRAux_1[k];
	void*&M = PTRAux_1[m];
	void*T=K;K=M;M=T;
}
char
GtYVt(AUXObj k, AUXObj m, char f)
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
char
GtyVt(AUXObj k, AUXObj m, char f)
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
void
GtSwapLb(long k, long m)
{
	void*&K = PTRAux_2[k];
	void*&M = PTRAux_2[m];
	void*T=K;K=M;M=T;
}
char
GtYLb(AUXObj k, AUXObj m, char f)
{
	register LbBead*i;
	register LbBead*j;
	if(!f)
	{
		i = (LbBead*)PTRAux_2[k.L];
		j = (LbBead*)PTRAux_2[m.L];
	}
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
char
GtyLb(AUXObj k, AUXObj m, char f)
{
	register LbBead*i;
	register LbBead*j;
	if(!f)
	{
		i = (LbBead*)PTRAux_2[k.L];
		j = (LbBead*)PTRAux_2[m.L];
	}
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
char
GtxLb(AUXObj k, AUXObj m, char f)
{
	register LbBead*i;
	register LbBead*j;
	if(!f)
	{
		i = (LbBead*)PTRAux_2[k.L];
		j = (LbBead*)PTRAux_2[m.L];
	}
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
void
Quick_Sort(long l, long r)
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
class _IEvaluater
{
public:
      _IEvaluater();
      ~_IEvaluater();
      void __SetVT0LB0__(void);
      void _ReleaseStorage(void);
}     ;
void _IEvaluater::_ReleaseStorage(void)
{
	ZVtTBl._Release();
	ZLvTBl._Release();
	ZLbTBl._Release();
	ZPtTBl._Release();
	ZLDCO0._Release();
	ZLDCO1._Release();
	ZLDCO2._Release();
	ZLDCO3._Release();
	ZLDCO4._Release();
	ZLDCO5._Release();

	PTRAux_1._Release();
	PTRAux_2._Release();
	INDAux_3._Release();
}
void _IEvaluater::__SetVT0LB0__(void)
{
      __VT0__ = __GVT__(0);
      __LB0__ = __GLB__(0);
      memset(__VT0__, 0, sizeof(VtBead)<<1);
      memset(__LB0__, 0, sizeof(LbBead)<<1);
}
_IEvaluater::_IEvaluater()
{
      ZPBCOR = new PtBead*[ZPBLIM+1];
      memset(ZPBCOR, 0, (ZPBLIM+1)*sizeof(PtBead*));

      LF_LIM = 512;
      Lf_Sdown = new char[LF_LIM*25];
      Lf_Rolld = (VtBead**)(Lf_Sdown+LF_LIM);
      Lf_ROLLD = Lf_Rolld+LF_LIM;
      Lf_First = Lf_ROLLD+LF_LIM;
      Lf_Upper = (unsigned short*)(Lf_First+LF_LIM);
      Lf_Lower = Lf_Upper+LF_LIM;
      Lf_Stack = Lf_Lower+LF_LIM;
      Lf_Ymidl = (long*)(Lf_Stack+(LF_LIM<<1));
      Lf_Factor = 1;
      while(LF_LIM>>Lf_Factor)
      ++Lf_Factor;
      --Lf_Factor;
      __SetVT0LB0__();
}
_IEvaluater::~_IEvaluater()
{
      delete ZPBCOR;
      delete Lf_Sdown;
      _ReleaseStorage();
}
_IEvaluater IEvaluater;
void l_swap(long&A, long&B)
{
long T=A;A=B;B=T;
}
char
COMBIN(LbBead*LB)
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
	L1 = L0 = LB->ZLBCO3;
	L8 = FetchReverseLv(L1);
	B3 = L8? L8->ZLVCO7: (B1=0);
	if(L8)
	do /*FIND-INITIAL-VECTOR*/
	{
		L1 = L1->ZLVCO1;
		L8 = FetchReverseLv(L1);
		B1 = L8? L8->ZLVCO7: 0;
	}
	while((L1!=L0)&&(B3==B1));
	/*FULLY-COINCIDENCE?*/
	if(B3&&(L0==L1))return(2);
	L0 = L1;
	B3 = LB;
	do /*FIND-L3-L2-(L0-IS-SECOND-FOR-LB)*/
	{
		L2 = L0;
		L0 = L0->ZLVCO1;
		L3 = FetchReverseLv(L0);
		if(L3)B3 = L3->ZLVCO7;
	}
	while((B3==LB)&&(L1!=L0));
	/*NON-COINCIDENT?*/
	if(L1==L0)return(0);
	do /*FIND-LAST-COINCIDENT-WITH-B3(L6)*/
	{
		L6 = L0;
		L0 = L0->ZLVCO1;
		L7 = FetchReverseLv(L6);
		L8 = FetchReverseLv(L0);
		B1 = L8? L8->ZLVCO7: 0;
		if(B1!=B3)break;
		L8 = L8->ZLVCO1;
		if(L8!=L7)break;
	}
	while(1);
	L9 = L6->ZLVCO1;
	L4 = L3->ZLVCO1;
	L0 = L4;
	while(L0!=L7)
	{
		L5 = L0;
		L0->ZLVCO7 = LB;
		L0 = L0->ZLVCO1;
	}
	if(L4==L7)
	L2->ZLVCO1 = L9;
	else
	{
		L2->ZLVCO1 = L4;
		L5->ZLVCO1 = L9;
	}
	B3->ZLBCO3 = 0L;
	LB->ZLBCO3 = L2; /*Set New Origin*/
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
	while(L<ZLBLIM)__GLB__(L++)->ZLBCO1 = 1;

	for(F = 1, L = 1; F; L = 1)
	for(F = 0; L<ZLBLIM; L+= 1)
	{
		LbBead*B = __GLB__(L);
		if(B->ZLBCO1!=1||!B->ZLBCO3)
		{
		/*POLYGON IS FULLY*/
		/*OUTSIDE*/
		/*OR IT WAS COMBINED BEFORE. */
			continue;
		}
		B->ZLBCO1 = S = COMBIN(B);
		if(S)F = 1;
	}
	L = 1;
	while(L<ZLBLIM)__GLB__(L++)->ZLBCO1 = 1;
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
	__GPT__(ZPTLIM)->ZPTCO1 = X;
	__GPT__(ZPTLIM)->ZPTCO2 = Y;

	P = __GPT__(i=1);
	while(P->ZPTCO1!=X||P->ZPTCO2!=Y)
	{
		P = __GPT__(++i);
	}
	if(i==ZPTLIM)__GPT__(ZPTLIM++)->ZPTCO3 = 0;
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
	P = __GPT__(ZPTLIM++);
	P->ZPTNxt = ZPBCOR[h];
	ZPBCOR[h] = P;
	P->ZPTCO1 = X;
	P->ZPTCO2 = Y;
	P->ZPTCO3 = 0;
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
	P = __GPT__(ZPTLIM++);
	P->ZPTNxt = ZPBCOR[h];
	ZPBCOR[h] = P;
	P->ZPTCO1 = X;
	P->ZPTCO2 = Y;
	P->ZPTCO3 = 0;
	return(P);
}
typedef PtBead*ipoint(long, long);
ipoint*IPOINT = IPOINT4;
double Eval_Angle
(double DX, double DY)
{
	double AN;
	if(DX==0.0)
	AN = DY>0.0? 90.0: DY<0.0? 270.0: 0.0;
	else
	if(DY==0.0)
	AN = DX>0.0? 0.0: 180.0;
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
	A = L->ZLVCO5;
	B = L->ZLVCO6;
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
	E = B->ZLBCO3;

	SS_Fl = 0;
	if(!E)return(STATUS=0);

	L = F = COMPLVANGLE(E);
	T = 0;
	for(V = E->ZLVCO1; V!=E; V = V->ZLVCO1)
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
LvBead*SPLITLV_M(LvBead*L3, PtBead*IP);
LvBead*COMPSPLITLV_M(LvBead*L3, VtBead*V3, VtBead*V4, PtBead*IP);
typedef LvBead*compsplitlv(LvBead*, VtBead*, VtBead*, PtBead*);
compsplitlv*COMPSPLITLV = COMPSPLITLV_M;
typedef LvBead*splitlv(LvBead*, PtBead*);
splitlv*SPLITLV = SPLITLV_M;
LvBead*SPLITLV_M(LvBead*L3, PtBead*IP)
{
	LvBead*L4 = __GLV__(ZLVLIM++);
	L4->ZLVCO2 = 0;
	L4->ZLVCO3 = 0;
	L4->ZLVCO1 = L3->ZLVCO1;
	L3->ZLVCO1 = L4;
	L4->ZLVCO7 = L3->ZLVCO7;
	L4->ZLVCO6 = L3->ZLVCO6;
	L3->ZLVCO6 = IP;
	L4->ZLVCO5 = IP;
	return(L4);
}
LvBead*COMPSPLITLV_M(LvBead*L3, VtBead*V3, VtBead*V4, PtBead*IP)
{
	LvBead*L4; /*SECOND-PART-OF-L3*/
	LvBead*L7; /*COINCINDENT-FOR-L3*/
	LvBead*L8; /*COINCINDENT-FOR-L4*/
	L7 = L3->ZLVCO3;
	L8 = L7 ? COMPSPLITLV_M(L7, V3, V4, IP) : 0;
	L4 = SPLITLV_M(L3, IP);
	L4->ZLVCO2 = V4; /*SET-VECTOR-NUMBER*/
	L4->ZLVCO3 = L8; /*SET-NEXT-COINCINDENT-VECTOR*/
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
	V3 = __GVT__(ZVTLIM++);
	V4 = __GVT__(ZVTLIM++);

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
	V4 = V3->ZVTCO6;
	GetVtPair(V5, V6);
	if(!VtIsEven(V3))
	{
		VtBead*V0=V5;V5=V6;V6=V0;
	}
	V3->ZVTCO2 = V4->ZVTCO2 = IP;
	V5->ZVTCO1 = V6->ZVTCO1 = IP;
	V5->ZVTCO2 = V4->ZVTCO1;
	V6->ZVTCO2 = V3->ZVTCO1;
	V5->ZVTCO6 = V4;
	V6->ZVTCO6 = V3;
	V3->ZVTCO6 = V6;
	V4->ZVTCO6 = V5;
	L3 = V3->ZVTCO5; /*CHAIN-HEAD-COINCIDENT-VECTORS*/
	L4 = V4->ZVTCO5; /*CHAIN-HEAD-FOR-REVERSE-DIRECTION*/
	L5 = L3 ? COMPSPLITLV(L3, V3, V5, IP) : 0;
	L6 = L4 ? COMPSPLITLV(L4, V4, V6, IP) : 0;
	V6->ZVTCO5 = L6;
	V5->ZVTCO5 = L5;
	V5->ZVTCO4 = V3->ZVTCO4;
	V6->ZVTCO4 = V4->ZVTCO4;
	V6->ZVTAux = V5->ZVTAux = 0;
	V6->ZVTNxt = V5->ZVTNxt = 0;
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
		LV->ZLVCO2 = V3;
	}
	V3->ZVTCO5 = LV, V4->ZVTCO5 = 0;
	V3->ZVTCO6 = V4;
	V4->ZVTCO6 = V3;
	V4->ZVTCO2 = V3->ZVTCO1 = P1;
	V4->ZVTCO1 = V3->ZVTCO2 = P2;
	V4->ZVTCO4 = V3->ZVTCO4 = 0;
	V3->ZVTAux = V4->ZVTAux = 0;
	V3->ZVTNxt = V4->ZVTNxt = 0;
	return(V3);
}
void
Init_Vt_Findtree
(long Y1, long Y2, unsigned short P, char L)
{
	long Y3;
	Y3 = ((Y2-Y1)>>1)+Y1;

	if(L>=Lf_Factor)
	{
		return;
	}
	Lf_Upper[P] = Tree_next_pos++;
	Lf_Lower[P] = Tree_next_pos++;

	Lf_Ymidl[P] = Y3;

	Init_Vt_Findtree(Y3, Y2, Lf_Upper[P], L+1);
	Init_Vt_Findtree(Y1, Y3, Lf_Lower[P], L+1);
}
void Init_Vt_FindTree(void)
{
	unsigned short I;
	Tree_next_pos = 1;

	for(I = 0; I<LF_LIM; ++I)
	{
		Lf_Sdown[I] = 0;
		Lf_Rolld[I] = 0;
		Lf_First[I] = 0;
		Lf_Stack[I] = 0;
	}
	if(!Initialized_hash)
	{
		for(I = 0; I<LF_LIM; ++I)
		{
			Lf_Upper[I] = 0;
			Lf_Lower[I] = 0;
		}
		Init_Vt_Findtree(PYMIN, PYMAX, 0, 1);
	}
	Lf_Lim = 0;
	Initialized_hash = F_EVAL_MODULE? 1: 0;
}
void Into_Lv_Chain(LvBead*Lv, VtBead*Vt)
{
	Lv->ZLVCO2 = Vt;
	Lv->ZLVCO3 = Vt->ZVTCO5;
	Vt->ZVTCO5 = Lv;
}
void H_Insert(unsigned short Lf)
{
H_Insert_Start:
	if(!Lf_Upper[Lf])
	{
		goto H_Insert_Exit;
	}
	else
	if(Vt_Ymin>Lf_Ymidl[Lf])
	{
		Lf_Sdown[Lf] = 1;
		Lf = Lf_Upper[Lf];
		goto H_Insert_Start;
	}
	else
	if(Vt_Ymax<Lf_Ymidl[Lf])
	{
		Lf_Sdown[Lf] = 1;
		Lf = Lf_Lower[Lf];
		goto H_Insert_Start;
	}
H_Insert_Exit:
	if(Lf_Rolld[Lf]==0)
	{
		Lf_ROLLD[Lf] = _Vt_;
		Lf_Stack[Lf_Lim++] = Lf;
		if(Lf_Lim==(LF_LIM<<1))
		{
			zAbort(fqeval,__LINE__);
		}
	}
	_Vt_->ZVTNxt = Lf_Rolld[Lf];
	Lf_Rolld[Lf] = _Vt_;
	return;
}
void H_INSERT(unsigned short Lf)
{
	Vt_Ymin =(_Vt_)->ZVTCO1->ZPTCO2;
	Vt_Ymax =(_Vt_)->ZVTCO2->ZPTCO2;

	if(Vt_Ymin>Vt_Ymax)
	{
		l_swap(Vt_Ymin, Vt_Ymax);
	}
	H_Insert(Lf);
}
void
Split_Vt(short Lf, VtBead*Vt, PtBead*Pt)
{
	_Vt_ = SPLITVT(Vt, Pt);
	H_INSERT(Lf);
}
void
Remove_Vt(short Lf, VtBead*Vt)
{
	VtBead*VT = Vt->ZVTNxt;
	Vt_Last->ZVTNxt = VT;
	if(Vt_Last==__VT0__)
	{
		Lf_First[Lf] = VT;
	}
	_Vt_ = Vt;
	H_INSERT(Lf);
}
void
Find_Intsect(unsigned short Lf)
{
	double&X21=Lv_X21;
	double&Y21=Lv_Y21;
	double&D21=Lv_D21;

	long A;
	long B;

	long Y4;
	long X4;
	long Y3;
	long X3;

	LvBead*Ls;
	VtBead*Vs;
	VtBead*Vt;
	VtBead*VT;
	PtBead*P3;
	PtBead*P4;

	char S1;
	char S2;
	char S3;

	char H132;
	char H142;
	char H314;
	char H324;
	char DF21;
	char DF43;
	char Cond;

	PtBead*Int_P;
	long Int_Y;
	long Int_X;

	char Vt_Ver;
	char Vt_Gor;

	double X31;
	double Y31;
	double X41;
	double Y41;
	double X23;
	double Y23;
	double X43;
	double Y43;
	double D43;

	double S132;
	double S142;
	double S314;
	double S324;

	__VT0__->ZVTNxt = 0;

	Vt = *(Lf_First+Lf);
	for(Vt_Last = __VT0__; Vt; Vt_Last = Vt_Auxx, Vt = Vt->ZVTNxt)
	{
		Vt_Auxx = Vt;

		P3 = Vt->ZVTCO1;
		X3 = P3->ZPTCO1;

		if(X3>Lv_Xmax)
		{
			continue;
		}
		P4 = Vt->ZVTCO2;
		X4 = P4->ZPTCO1;

		if(X4<Sh_Xmin)
		{
			if(Vt_Last!=__VT0__)
			{
				Vt_Last->ZVTNxt = Vt->ZVTNxt;
				Vt_Auxx = Vt_Last;
				continue;
			}
			Lf_First[Lf] = Vt->ZVTNxt;
			Vt_Auxx = __VT0__;
			continue;
		}
		if(X4<Lv_Xmin)
		{
			continue;
		}
		Y4 = P4->ZPTCO2;
		Y3 = P3->ZPTCO2;

		if(Y3>Y4)
		Vt_Ymin = Y4, Vt_Ymax = Y3;
		else
		Vt_Ymin = Y3, Vt_Ymax = Y4;

		if(Vt_Ymin>Lv_Ymax)
		{
			continue;
		}
		if(Vt_Ymax<Lv_Ymin)
		{
			continue;
		}
		Vt_Swi = X3==X4&&Y3>Y4;
		I_Type = 0;

		Vt_Ver = 0;
		Vt_Gor = 0;

		if(Lv_Ort)
		{
			Vt_Ver = X3==X4;
			Vt_Gor = Y3==Y4;
			if(Vt_Ver&&Lv_Ver)goto IntAnalCoincedence;
			if(Vt_Gor&&Lv_Gor)goto IntAnalCoincedence;
			if(_P1_==P3||_P2_==P3)
			{
				continue;
			}
			if(_P1_==P4||_P2_==P4)
			{
				continue;
			}
		}
		X31 = X3-_X1_;
		Y31 = Y3-_Y1_;
		X41 = X4-_X1_;
		Y41 = Y4-_Y1_;
		X23 = _X2_-X3;
		Y23 = _Y2_-Y3;
		X43 = X4-X3;
		Y43 = Y4-Y3;
		D43 = ((Vt_Ymax-Vt_Ymin)+X43)/(2.0);
		S132 = X31 *Y21 - Y31 *X21;
		S142 = X41 *Y21 - Y41 *X21;
		S314 = X43 *Y31 - Y43 *X31;
		S324 = X23 *Y43 - Y23 *X43;

		H132 = S132 >D21 ? 1 : S132 <-D21 ? -1 : 0;
		H142 = S142 >D21 ? 1 : S142 <-D21 ? -1 : 0;
		H314 = S314 >D43 ? 1 : S314 <-D43 ? -1 : 0;
		H324 = S324 >D43 ? 1 : S324 <-D43 ? -1 : 0;

		DF21 = (H132==-H142)&&(H132);
		DF43 = (H324==-H314)&&(H324);

		if(DF21&&DF43)I_Type = 5;
		else
		if(!H314&&!H324&&!H132&&!H142)
		{
IntAnalCoincedence:
			if(Lv_Swi!=Vt_Swi)
			{
				if(_P1_==P3||_P2_==P4)continue;
				if(P3==_P2_&&P4==_P1_)I_Type = 6;
			}
			else
			{
				if(_P1_==P4||_P2_==P3)continue;
				if(P3==_P1_&&P4==_P2_)I_Type = 7;
			}
			switch(I_Type)
			{
				case 6:
				Vt = Vt->ZVTCO6;
				case 7:
				Into_Lv_Chain(_Lv_, Vt);
				_In_ = 1;
				return;
			}
			I_Type = 8;
		}
		else
		{
			char Int_0;
			char Int_1;
			char Int_2;
			char Int_3;
			char Int_4;
			DF21 = ((S132>0)&&(S142<0))||((S132<0)&&(S142>0));
			DF43 = ((S324>0)&&(S314<0))||((S324<0)&&(S314>0));
			Int_0 = 0;
			if(DF21 && !H314) I_Type = 1, ++Int_0, Int_1 = 1;
			else Int_1 = 0;
			if(DF21 && !H324) I_Type = 2, ++Int_0, Int_2 = 1;
			else Int_2 = 0;
			if(DF43 && !H132) I_Type = 3, ++Int_0, Int_3 = 1;
			else Int_3 = 0;
			if(DF43 && !H142) I_Type = 4, ++Int_0, Int_4 = 1;
			else Int_4 = 0;
			if(Int_0==0)
			{
				if(DF21&&DF43) I_Type = 9;
				else continue;
			}
			else
			if(Int_0>1)
			{
				if(Int_1&&Int_2)I_Type = 8;
				else
				if(Int_3&&Int_4)I_Type = 8;
				else
				I_Type = 9;
			}
		}
		if(I_Type==5||I_Type==9)
		{
			if(Vt_Gor||Vt_Ver)
			{
				I_Type = 5;
				Int_X = Vt_Ver? X3: Lv_Xmin;
				Int_Y = Vt_Ver? Lv_Ymin: Y3;
				Int_P = IPOINT(Int_X, Int_Y);
			}
			else
			{
				double DL;
				double A1;
				double B1;
				double C1;
				double A2;
				double B2;
				double C2;
				A1 = -Y21;
				B1 = +X21;
				C1 = (double)_X1_*_Y2_ - (double)_X2_*_Y1_;
				A2 = -Y43;
				B2 = +X43;
				C2 = (double)X3*Y4 - (double)X4*Y3;
				DL = A1*B2 - B1*A2;
				Int_X = floor((B1*C2-B2*C1)/DL +0.5 -0.5E-7);
				Int_Y = floor((A2*C1-A1*C2)/DL +0.5 -0.5E-7);
				Int_P = IPOINT(Int_X, Int_Y);
				I_Type = 9;
			}
		}
IntAnalIntersect:
		switch(I_Type)
		{
			case 1:
			Split_Vt(Lf, Vt, _P1_);

			Remove_Vt(Lf, Vt);
			Vt_Auxx = Vt = Vt_Last;

			continue;
			case 2:
			Split_Vt(Lf, Vt, _P2_);

			Remove_Vt(Lf, Vt);
			Vt_Auxx = Vt = Vt_Last;

			continue;
			case 3:
			SPLITLV(_Lv_, P3);
			_P2_ = P3;
			_X2_ = X3;
			_Y2_ = Y3;
			break;
			case 4:
			SPLITLV(_Lv_, P4);
			_P2_ = P4;
			_X2_ = X4;
			_Y2_ = Y4;
			break;
			case 5:
			Split_Vt(Lf, Vt, Int_P);

			Remove_Vt(Lf, Vt);
			Vt_Auxx = Vt = Vt_Last;
			SPLITLV(_Lv_, Int_P);
			_P2_ = Int_P;
			_X2_ = Int_X;
			_Y2_ = Int_Y;
			break;

			case 9:
			if(Int_P!=P3&&Int_P!=P4)
			{
				Split_Vt(Lf, Vt, Int_P);

				Remove_Vt(Lf, Vt);
				Vt_Auxx = Vt = Vt_Last;
			}
			if(Int_P==_P1_||Int_P==_P2_)
			{
				continue;
			}
			SPLITLV(_Lv_, Int_P);
			_P2_ = Int_P;
			_X2_ = Int_X;
			_Y2_ = Int_Y;
			break;

			case 8:
			VtBead*Vt_S[3];
			long Vt_L =0;
			Delta_Y = Lv_Ymax-Lv_Ymin;
			Delta_X = Lv_Xmax-Lv_Xmin;

			S3 = Delta_Y>Delta_X;

			S1 = S3? Y4<Y3: X4<X3;
			S2 = S3? _Y2_<_Y1_: _X2_<_X1_;

			S1 = S1!=S2;
			if(S1)
			{
				PtBead*P0=P3;
				P3=P4;
				P4=P0;
			}
			Ls = _Lv_;
			Vs = Vt;

			if(_P1_!=P3)
			{
				if(S3) A = _Y1_, B = P3->ZPTCO2;
				else   A = _X1_, B = P3->ZPTCO1;
				if(S2) Cond = A>=B;
				else   Cond = A<=B;
				if(Cond)
				{
					Ls = SPLITLV(Ls, P3);
				}
				else
				{
					Vs = SPLITVT(Vs, _P1_);
					Vt_S[Vt_L++] = Vs;
					if(S1)Vs = Vt;
				}
			}
			if(_P2_!=P4)
			{
				if(S3) A = _Y2_, B = P4->ZPTCO2;
				else   A = _X2_, B = P4->ZPTCO1;
				if(S2) Cond = A<=B;
				else   Cond = A>=B;
				if(Cond)
				{
					SPLITLV(Ls, P4);
				}
				else
				{
					VT = SPLITVT(Vs, _P2_);
					Vt_S[Vt_L++] = VT;
					if(S1)Vs = VT;
				}
			}
			if(S1)Vs = Vs->ZVTCO6;
			Into_Lv_Chain(Ls, Vs);

			if(Vt_L)
			{
				while(Vt_L--)
				_Vt_ = Vt_S[Vt_L], H_INSERT(Lf);

				Remove_Vt(Lf, Vt);
				Vt_Auxx = Vt = Vt_Last;
			}
			if(_Lv_==Ls)
			{
				_In_ = 1;
				return;
			}
			_P2_ = _Lv_->ZLVCO6;
			_X2_ = _P2_->ZPTCO1;
			_Y2_ = _P2_->ZPTCO2;
			break;
		}
		if(_X1_<_X2_)Lv_Xmin = _X1_,Lv_Xmax = _X2_;
		else Lv_Xmin = _X2_,Lv_Xmax = _X1_;

		if(_Y1_<_Y2_)Lv_Ymin = _Y1_,Lv_Ymax = _Y2_;
		else Lv_Ymin = _Y2_,Lv_Ymax = _Y1_;

		Y21 = _Y2_ - _Y1_;
		X21 = _X2_ - _X1_;

		Lv_Swi = (_X1_>_X2_)||((_X1_==_X2_)&&(_Y1_>_Y2_));

		D21 = ((double)(Lv_Ymax-Lv_Ymin)+(Lv_Xmax-Lv_Xmin))/(2.0);
	}
}
void __IntSLv__(unsigned short Lf)
{
	if(Lf_First[Lf])
	{
		Find_Intsect(Lf);
		if(_In_)return;
	}
	if(!Lf_Sdown[Lf])return;

	if(_Lf_<0)
	{
		char Lst;
		char Ust;
		Ust = Lv_Ymax>=Lf_Ymidl[Lf];
		Lst = Lv_Ymin<=Lf_Ymidl[Lf];

		if(Ust&&Lst)_Lf_ = Lf;
	}
	if(Lv_Ymin<=Lf_Ymidl[Lf])
	{
		__IntSLv__(Lf_Lower[Lf]);
		if(_In_)return;
	}
	if(Lv_Ymax>=Lf_Ymidl[Lf])
	{
		__IntSLv__(Lf_Upper[Lf]);
	}
	return;
}
void Intsect_Lv(LvBead*Lv)
{
	VtBead*V3;
	VtBead*V4;
	_Lv_ = Lv;

	_P1_ = _Lv_->ZLVCO5;
	_P2_ = _Lv_->ZLVCO6;
	_X1_ = _P1_->ZPTCO1;
	_X2_ = _P2_->ZPTCO1;
	_Y1_ = _P1_->ZPTCO2;
	_Y2_ = _P2_->ZPTCO2;

	Lv_Swi = (_X1_>_X2_)||((_X1_==_X2_)&&(_Y1_>_Y2_));

	if(_X1_<_X2_)Lv_Xmin = _X1_,Lv_Xmax = _X2_;
	else Lv_Xmin = _X2_,Lv_Xmax = _X1_;

	if(_Y1_<_Y2_)Lv_Ymin = _Y1_,Lv_Ymax = _Y2_;
	else Lv_Ymin = _Y2_,Lv_Ymax = _Y1_;

	Lv_Y21 = _Y2_ - _Y1_;
	Lv_X21 = _X2_ - _X1_;

	Lv_D21 = ((double)(Lv_Ymax-Lv_Ymin)+(Lv_Xmax-Lv_Xmin))/(2.0);

	Lv_Gor = Lv_Ymin==Lv_Ymax;
	Lv_Ver = Lv_Xmin==Lv_Xmax;
	Lv_Ort = Lv_Ver || Lv_Gor;
	_Lf_ =-1;
	_In_ = 0;

	__IntSLv__(0);
	if(_In_)
	{
		return;
	}
	_In_ = 1;

	GetVtPair(V3, V4);

	_Vt_ = V3;

	if(Lv_Swi)
	{
		VtBead*V0=V3;V3=V4;V4=V0;
	}
	V4->ZVTCO5 = 0;
	V3->ZVTCO5 = _Lv_;

	Lv->ZLVCO2 = V3;
	V3->ZVTCO6 = V4;
	V4->ZVTCO6 = V3;

	V4->ZVTCO2 = V3->ZVTCO1 = _P1_;
	V4->ZVTCO1 = V3->ZVTCO2 = _P2_;

	V4->ZVTCO4 = V3->ZVTCO4 = 0;
	V3->ZVTAux = V4->ZVTAux = 0;

	Vt_Ymin = Lv_Ymin;
	Vt_Ymax = Lv_Ymax;

	H_Insert(_Lf_<0? 0: _Lf_);
}
void Lv_Sort(void)
{
	LbBead*Lb;
	LvBead*Le;
	LvBead*Lv;
	long Vt=0;
	long Ls=0;
	for(Ls = 1; Ls<ZLBLIM; ++Ls)
	{
		Lb = __GLB__(Ls);
		Le = Lb->ZLBCO3;
		if(!Le)continue;
		Lv = Le;
		do
		{
			++Vt;

			Lv->ZLVCO2 = 0L;
			Lv->ZLVCO3 = 0L;
			PTRAux_1[Vt] = (void*)Lv;
			Lv = Lv->ZLVCO1;
		}
		while(Lv&&Lv!=Le);
	}
	Gt = GtXLv;
	GtSwap = GtSwapLv;
	GtGetVal = GtGetXLv;
	if(Vt>1)Quick_Sort(1l, Vt);

	PTRAux_1Lim = Vt+1;
}
char CmplxIntrsAlwd;
char CmplxIntrsSett;
void
INTERSECTION_ANALYSIS(void)
{
	long Ls;
	long Lf;
	long LF;
	long X1;
	long X2;
	VtBead*Vt;
	VtBead*VT;
	PtBead*P1;
	PtBead*P2;
	LvBead*Lv;

	LvBead*Lnxt;
	Init_Vt_FindTree(), Lv_Sort();

	for(Ls = 1; Ls<PTRAux_1Lim; ++Ls)
	{
		Lv = (LvBead*)PTRAux_1[Ls];
		P1 = Lv->ZLVCO5;
		P2 = Lv->ZLVCO6;
		X1 = P1->ZPTCO1;
		X2 = P2->ZPTCO1;
		Sh_Xmin = X1<X2? X1: X2;

		Lnxt = Lv->ZLVCO1;
		if(CmplxIntrsAlwd&&(ZLVLIM-PTRAux_1Lim>(Ls<<3)))
		{
			CmplxIntrsSett=1;
			Intsect_Lv(Lv);
		}
		else
		do
		{
			if(!Lv->ZLVCO2)
			{
				Intsect_Lv(Lv);
			}
			Lv = Lv->ZLVCO1;
		}
		while(Lv!=Lnxt);
		for(LF = 0; LF<Lf_Lim; ++LF)
		{
			Lf = Lf_Stack[LF];
			Vt = Lf_Rolld[Lf];
			VT = Lf_ROLLD[Lf];

			VT->ZVTNxt = Lf_First[Lf];
			Lf_First[Lf] = Vt;
			Lf_Rolld[Lf] = 0L;
		}
		Lf_Lim = 0;
	}
}
void
MERGELVCHAIN_M(VtBead*VT, VtBead*PV)
{
	LvBead*LQ;
	LvBead*LS;
	LvBead*LH;
	LvBead*LM;
	LH = VT->ZVTCO5;
	LM = PV->ZVTCO5;
	for(LS = LQ = LH; LQ; LQ = LQ->ZLVCO3)
	{
		LS = LQ, LQ->ZLVCO2 = PV;
	}
	if(LS)LS->ZLVCO3 = LM, PV->ZVTCO5 = LH;
}
typedef void mergelvchain(VtBead*, VtBead*);
mergelvchain*MERGELVCHAIN = MERGELVCHAIN_M;
void
SUBS_PV_BEAD(VtBead*Old, VtBead*New, char z)
{
	PtBead*PT;
	VtBead*VT;
	PT = Old->ZVTCO1;
	if(PT->ZPTCO3==Old)
	{
		PT->ZPTCO3 = New;
		if(z)
		{
			New->ZVTNxt = Old->ZVTNxt;
		}
		return;
	}
	for(VT = PT->ZPTCO3; VT; VT = VT->ZVTNxt)
	{
		if(VT->ZVTNxt!=Old)
		{
			continue;
		}
		if(z)
		{
			New->ZVTNxt = Old->ZVTNxt;
		}
		VT->ZVTNxt = New;
		return;
	}
}
void
ERASEVTBEAD(VtBead*VT, VtBead*PV)
{
	VtBead*Vt;
	VtBead*Pv;
	Vt = VT->ZVTCO6;
	Pv = PV->ZVTCO6;
	if(Vt->ZVTPos<VT->ZVTPos)
	{
		PV->ZVTCO6 = Vt;
		Vt->ZVTCO6 = PV;
		if(Pv->ZVTPos<VT->ZVTPos)
		{
			SUBS_PV_BEAD(Vt, Vt->ZVTNxt, 0);
		}
		SUBS_PV_BEAD(Pv, Vt, 1);

		VtBead*Tm=Vt;Vt=Pv;Pv=Tm;
	}
	MERGELVCHAIN(VT, PV);
	MERGELVCHAIN(Vt, Pv);
	if(S_Free)
	{
		PV->ZVTCO4 |= VT->ZVTCO4;
		Pv->ZVTCO4 |= Vt->ZVTCO4;
	}
	else
	if(!F_EVAL_MODULE)
	{
		PV->ZVTCO4 = 0;
		Pv->ZVTCO4 = 0;
	}
	/*
	memset(VT, 0, sizeof(VtBead));
	memset(Vt, 0, sizeof(VtBead));
	*/
	VT->ZVTCO6 = 0;
	Vt->ZVTCO6 = 0;
}
void
split_vt_pv
(VtBead*vt, VtBead*pv, long y2, long y4, PtBead*p2, PtBead*p4)
{
	if(y2<y4)SPLITVT(pv, p2);
	else SPLITVT(vt, p4);
}
long Pass_Count;
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
	Pass_Count = 0;
	for(Vt=first; Vt<ZVTLIM; ++Vt)
	{
		VT = __GVT__(Vt);

		if(!VT->ZVTCO6)
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

		PV = PT->ZPTCO3;
		PS = 0;
		ER = 0;
		for(ST = 1; PV!=0 && ST; ++Pass_Count)
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
			if(ST)PS = PV, PV = PV->ZVTNxt;
			else if(ER)PV_Error = 1;
		}
		if(ER==0)
		{
			VT->ZVTNxt = PV;
			if(PS)PS->ZVTNxt = VT;
			else  PT->ZPTCO3 = VT;
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
	LE = VT->ZVTCO5;
	while(LE&&(LE->ZLVCO7!=LB))
	{
		LE = LE->ZLVCO3;
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
	VR = (LV->ZLVCO2)->ZVTCO6;
	LS = VR->ZVTCO5;
	while(LS)
	{
		if(LS->ZLVCO7==LB)LR = LS, ++EC;
		LS = LS->ZLVCO3;
	}
	if(EC<=1)return LR;
/*GET-UP-DIRECT-SUCCESSOR*/
	LS = LV->ZLVCO1;
	while(LS->ZLVCO2!=VR)
	{
		LS = LS->ZLVCO1;
	}
	return LS;
}
void
REMOVE_FROM_VT_CHAIN(LvBead*LV)
{
	VtBead*VT;
	LvBead*Lv;
	VT = LV->ZLVCO2;
	Lv = VT->ZVTCO5;
	if(Lv==LV)
	{
		VT->ZVTCO5 = LV->ZLVCO3;
		return;
	}
	while(Lv)
	{
		if(Lv->ZLVCO3==LV)
		{
			Lv->ZLVCO3 = LV->ZLVCO3;
			return;
		}
		Lv = Lv->ZLVCO3;
	}
}
LvBead*CLEAR_LB_PTR(LvBead*Lv, LvBead*Ln)/*From Lv To Ln*/
{
	LvBead*LE;
	LvBead*LS;
	DummyClip = 1;
	LS = Lv;
	REMOVE_FROM_VT_CHAIN(Ln);
	for(Ln->ZLVCO7 = 0; LS!=Ln; LS = LS->ZLVCO1)
	{
		LS->ZLVCO7 = 0;
		REMOVE_FROM_VT_CHAIN(LS);
	}
	LE = Ln->ZLVCO1;
	Ln->ZLVCO1 = Lv;
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
		LV = LV->ZLVCO1;
		if(LV==LN)ER = 2;
		else
		{
			LN = FETCHREVERSE(LB, LV);
			ER = LN ? 1 : 0;
			while(ER&&(LN->ZLVCO1!=LS))
			{
				ER = DUMMYJUNCTION(LB, LV);
				if(ER)
				{
					LF->ZLVCO1 = CLEAR_LB_PTR(LV, LN);
					LV = LF->ZLVCO1;
					LN = FETCHREVERSE(LB, LV);
					ER = LN ? 1 : 0;
				}
			}
			if(!ER)DJ_FIRSTFR_LV = LV->ZLVCO1;
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

	LE = LV = LB->ZLBCO3;

	LN = FETCHREVERSE(LB, LV);
	LM = FETCHREVERSE(LB, LZ=LV->ZLVCO1);

	if(LN||LM)
	do
	{
		LN = FETCHREVERSE(LB, LV=LZ->ZLVCO1);
		LM = FETCHREVERSE(LB, LZ=LV->ZLVCO1);
	}
	while((LN||LM)&&!(badpoly=(LV==LE||LZ==LE)));
	else badpoly = 0;

	if(badpoly)
	{
		CLEAR_LB_PTR(LE->ZLVCO1, LE);
		LB->ZLBCO3 = 0;
		PV_Error = 2;
		return 0;
	}
	return(LB->ZLBCO3=LZ);
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
	LN = LV->ZLVCO1;
	do /*For Each Non Coincidenced LV*/
	{
		DJ_FIRSTFR_LV = LN->ZLVCO1;

		if(FetchReverseLv(LN))
		{
			while(DUMMYJUNCTION(LB, LN))
			{
				LN = CLEAR_LB_PTR(LN, DJ_REVERSE_LV);
				LV->ZLVCO1 = LN;
				DJ_FIRSTFR_LV = LN->ZLVCO1;
			}
			do
			{
				LV = LN;
				LN = LV->ZLVCO1;
			}
			while(LV!=LE&&LN!=DJ_FIRSTFR_LV);
		}
		else
		LV = LN,
		LN = LV->ZLVCO1;
	}
	while(LV!=LE);
}
void
Check_Closure(void)
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
	LB = __GLB__(ZLBLIM++);
	LE = __GLV__(ZLVLIM);
	LB->ZLBCO3 = LE;
	ZLDLIM = 0;
	do
	{
		V3->ZVTAux = 1;
		LV = __GLV__(ZLVLIM++);

		LV->ZLVCO3 = 0;
		LV->ZLVCO2 = V3;
		LV->ZLVCO7 = LB;
		LV->ZLVCO5 = V3->ZVTCO1;
		LV->ZLVCO6 = V3->ZVTCO2;
		LE->ZLVCO1 = V3->ZVTCO5 = LV;
		LE = LV;
		V4 = V3;
		V3 = FETCHSUCCESSOR(V3->ZVTCO6);
		if(V4->ZVTCO6==V3)DJ = 1;
	}
	while(V3!=VT);
	LE = LV->ZLVCO1 = LB->ZLBCO3;
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
	LB->ZLBCO3 = 0;
	LV = LE;
	do
	{
		REMOVE_FROM_VT_CHAIN(LV);
		LV = LV->ZLVCO1;
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
	for(Vt=__GVT__(V=0); V<ZVTLIM; Vt=__GVT__(++V))
	{
		Vt->ZVTAux = 0;
		Vt->ZVTCO5 = 0;
	}
	for(Vt=__GVT__(V=2); V<ZVTLIM; Vt=__GVT__(++V))
	{
		if(!Vt->ZVTCO6||Vt->ZVTAux)
		{
			continue;
		}
		if(!True_Stat(Vt->ZVTCO4))
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

	LV = LB->ZLBCO3;
	LN = LV->ZLVCO1;
	V1 = LV->ZLVCO2->ZVTCO6;
	V2 = LN->ZLVCO2->ZVTCO6;
	if(CheckItSelf(LB, V1)||CheckItSelf(LB, V2))
	{
		LV = SET_INITIAL_VECTOR(LB);
		if(!LV)return(0);
	}
	LE = LV;
	LN = LV->ZLVCO1;
	A1 = 361.0;
	ZLDLIM = 0;
	do
	{
		DJ_FIRSTFR_LV = LN->ZLVCO1;

		if(FetchReverseLv(LN))
		while(DUMMYJUNCTION(LB, LN))
		{
			LN = CLEAR_LB_PTR(LN, DJ_REVERSE_LV);
			LV->ZLVCO1 = LN;
			DJ_FIRSTFR_LV = LN->ZLVCO1;
		}
		LD = (ZLDLIM++);

		ZLDCO0[LD] = LV->ZLVCO6->ZPTCO1;
		ZLDCO1[LD] = LV->ZLVCO6->ZPTCO2;

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
		LN = LV->ZLVCO1;
		while(LN!=DJ_FIRSTFR_LV&&LV!=LE)
		{
			LD = (ZLDLIM++);

			ZLDCO0[LD] = LV->ZLVCO6->ZPTCO1;
			ZLDCO1[LD] = LV->ZLVCO6->ZPTCO2;
			A1 = COMPLVANGLE(LV);
			LV = LN;
			LN = LV->ZLVCO1;
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
	      LbBead*Lb=__GLB__(LB);
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
      LbGrp_Mdfy(Lv_0->ZLVCO7);

      Vt_1 = Lv_0->ZLVCO2;
      Vt_2 = Vt_1->ZVTCO6;
      do
      {
	      Vt_1->ZVTCO4|= Si_St;
	      Vt_2->ZVTCO4|= Si_St;
	      Lv_0->ZLVAux = Si_Lb;

	      Lv_0 = Lv_0->ZLVCO1;
	      Vt_1 = Lv_0->ZLVCO2;

	      Js = PTRAux_1Lim;
	      for(Vt_3 = Vt_1->ZVTCO1->ZPTCO3; Vt_3; Vt_3 = Vt_3->ZVTNxt)
	      {
		      Lv_1 = Vt_3->ZVTCO5;
		      if(!Lv_1)continue;

		      Lv_2 = Lv_1;
		      do
		      if(Si_Lb==Lv_2->ZLVCO7)
		      return;
		      while((Lv_2 = Lv_2->ZLVCO3)!=0);

		      if(Vt_3==Vt_2)continue;
		      if(Vt_3==Vt_1)continue;

		      if(Lv_1->ZLVAux!=Si_Lb)PTRAux_1[++Js] = (void*)Lv_1;
	      }
	      PTRAux_1Lim = Js;
	      Vt_2 = Vt_1->ZVTCO6;
      }
      while(Lv_0->ZLVAux!=Si_Lb);
}
void
SET_LbGrp(LvBead*v)
{
      while(v)
      {
	      LbBead*p=v->ZLVCO7;
	      v = v->ZLVCO3;
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
		VR = PV->ZVTCO6;

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
	      VR = PV->ZVTCO6;
	      LS = PV->ZVTCO5;

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
	      SET_LbGrp(PV->ZVTCO5);
	}
}
void
POINT_VECTOR_CLASS_2
(LbBead*LB, long SX, long SY)
{
      VtBead*V3;
      VtBead*V4;
      VtBead*V5;
      VtBead*V6;
      LvBead*LE;
      LvBead*LV;
      LvBead*LN;

/*After DUMMYJUNCTIONANALYSIS Only*/

      LV = LB->ZLBCO3;
      LE = LV;

      LN = LV->ZLVCO1;
      V3 = LV->ZLVCO2;
      V4 = V3->ZVTCO6;

      Si_Lb = LB;
      Si_St = SY;

      Si_LbGrp = LB->ZLBGrp;
      Si_LbNxt = Si_LbGrp!=LB||LB->ZLBNxt!=0;

      PV_Error = 0;
      /*For Each LV*/
      do
      {
	      SET_LbGrp(V3->ZVTCO5);
	      SET_LbGrp(V4->ZVTCO5);

	      V5 = LN->ZLVCO2;
	      V6 = V5->ZVTCO6;

	      SI_V1 = V4;
	      SI_V2 = V5;

	      SET_INSIDE_ALL_2();

	      if(!V4->ZVTCO5&&!V6->ZVTCO5)
	      {
		       SET_INSIDE_ALL_3();
	      }
	      if(Target_Function<0&&(V3->ZVTCO4&SX))
	      V3->ZVTCO4 |= SY;
	      V3->ZVTCO4 |= SX;

	      if(SI_Error)
	      {
		      PV_Error = 1;
		      do
		      {
			      LV = LN;
			      LN = LV->ZLVCO1;
			      V3 = LV->ZLVCO2;
			      V4 = V3->ZVTCO6;
		      }
		      while(CheckItSelf(LB, V4));
	      }
	      else V3 = V5, V4 = V6, LV = LN, LN = LV->ZLVCO1;
      }
      while(LV!=LE);
}
LbBead*Loadmaster(void)
{
	LbBead*LB;
	LvBead*LV;
	LvBead*LE;
	LvBead*LM=0;
	PtBead*P1;
	PtBead*P2;
	PtBead*PM;
	long Lim_1=ZLBLIM;
	long Lim_2=ZLVLIM;
	long PCOUNT=1;
	long XM, YM, LD, X, Y;

	LB = __GLB__(ZLBLIM++);
	LB->ZLBCO4 = C_Layer;

	LB->ZLBCO3 = __GLV__(ZLVLIM);
	LB->ZLBCO1 = STATUS;
	XM=ZLDCO0[0];
	YM=ZLDCO1[0];
	PM=P1=P2=IPOINT(X=XM, Y=YM);
	for(LD=1, LE=LV=LB->ZLBCO3; LD<ZLDLIM; ++LD, P1=P2, LE=LV)
	{
		long x = ZLDCO0[LD];
		long y = ZLDCO1[LD];
		if(x==X&&y==Y)
		{
			continue;
		}
		++PCOUNT;
		P2 = IPOINT(X=x, Y=y);
		LV = __GLV__(ZLVLIM++);
		LV->ZLVCO7 = LB;
		LV->ZLVCO5 = P1;
		LV->ZLVCO6 = P2;
		LV->ZLVCO2 = 0L;
		LE->ZLVCO1 = LV;
		LV->ZLVCO3 = 0L;
		if(X<XM? 1: X>XM? 0: Y<YM? 1: 0)XM=X, YM=Y, PM=P2, LM=LV;
	}
	LV->ZLVCO1 = LB->ZLBCO3;
	LB->ZLBXym = PM;
	if(LM&&!Open_Shape)
	{
		LB->ZLBCO3 = LM;
	}
	if(Open_Shape||PCOUNT>3)
	{
		return LB;
	}
	ZLBLIM = Lim_1,
	ZLVLIM = Lim_2;
	return 0;
}
void
iniPvClass(void)
{
	long i;
	PTRAux_2Lim = ZLBLIM;
	for(i = 1; i<ZLVLIM; ++i)
	{
		__GLV__(i)->ZLVAux = 0;
	}
	for(i = 1; i<ZLBLIM; ++i)
	{
		PTRAux_2[i] = (void*)__GLB__(i);
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
		VtBead*Vt = __GVT__(i);
		VtBead*Vr = __GVT__(m);
		VtBead*VT;
		VT = VtPointUp(Vt)? Vt: Vr;
		if(VT->ZVTCO6)
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
		VtBead*Vt = __GVT__(i);
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
		LbBead*Lb = __GLB__(i);
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
			V3 = V3->ZVTCO6;
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
			zAbort(fqeval,__LINE__);
		}
	}
	Sh_Vt = VT;
	Sh_Ip = IP;
	Sh_Xdir = XO;
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
		V3 = VtPointUp(Sh_Vt)? Sh_Vt: Sh_Vt->ZVTCO6;
	}
	else
	{
		if(!Sh_Ip)
		{
			return 1;
		}
		V5 = Sh_Ip->ZPTCO3;
		if(!VtPointUp(V5))
		{
			do
			V3 = V5, V5 = V5->ZVTNxt;
			while(V5);
			V3 = V3->ZVTCO6;
		}
		else V3 = V5;
	}
	V3->ZVTAux = 1;
	if(!V3->ZVTCO4)
	{
		return 0;
	}
	for(V5 = V3; 1; V5 = FETCHSUCCESSOR(V5))
	{
		Lv = V5->ZVTCO5;
		if(!Lv)
		{
			V5 = V5->ZVTCO6;
			Lv = V5->ZVTCO5;
		}
		if(Lv)break;
	}
	Gr_Com = Lv->ZLVCO7->ZLBGrp;

	IN = MAKE_MASK(V3->ZVTCO4);
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
		Lv = LB->ZLBCO3;
		Le = Lv;
		do
		{
			(V3=Lv->ZLVCO2)->ZVTCO4 |=IN;
			(V3=V3->ZVTCO6)->ZVTCO4 |=IN;

			Lv = Lv->ZLVCO1;
		}
		while(Lv!=Le);
	}
	while(Nx);
	LB->ZLBNxt = Gr_Com->ZLBNxt;
	Gr_Com->ZLBNxt = Gr;
	return 2;
}
void
POINT_VECTOR_CLASS_1(LbBead*LB)
{
	VtBead*V4;
	VtBead*V5;
	VtBead*V6;
	LvBead*LE;
	LvBead*LV;
	LvBead*LN;

/*After DUMMYJUNCTIONANALYSIS Only*/

	LV = LB->ZLBCO3;
	LE = LV;

	LN = LV->ZLVCO1;
	V4 = LV->ZLVCO2;
	V4 = V4->ZVTCO6;

	Si_Lb = LB;
	do
	{
		V5 = LN->ZLVCO2;
		V6 = V5->ZVTCO6;


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
		else V4 = V6, LV = LN, LN = LV->ZLVCO1;
	}
	while(LV!=LE);
}
void PrintOk(void);
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
Set_Rstat(LbBead*I, char z)
{
	LvBead*S;
	LvBead*K;
	VtBead*J;
	VtBead*R;
	char T;
	S = I->ZLBCO3;
	if(S==0)
	{
		return;
	}
	K = S;
	T = I->ZLBCO1;
	do
	{
		J = K->ZLVCO2;
		R = J->ZVTCO6;
		K = K->ZLVCO1;
		if(z==7)
		{
			char f_1=CheckItSelf(I, J);
			char f_2=CheckItSelf(I, R);
			if(f_1&&f_2)continue;
		}
		if(T==-1)J=R;
		if(z)J->ZVTCO4|=1;
		else J->ZVTCO4 =1;
	}
	while(K!=S);
}
void
__INI_LvVt__(void)
{
	long LB;
	for(LB=1; LB<ZLBLIM; ++LB)
	{
		LbBead*Lb=__GLB__(LB);
		LvBead*Lv= Lb->ZLBCO3;
		LvBead*Le= Lv;
		if(Lv)
		do
		{
			Lv->ZLVCO2 =__VT0__;
			Lv=Lv->ZLVCO1;
		}
		while(Lv&&Lv!=Le);
	}
	__VT0__=__GVT__(0);
	VtBead*__VT1__=__GVT__(1);
	memset(__VT0__, 0, sizeof(VtBead));
	memset(__VT1__, 0, sizeof(VtBead));
	__VT0__->ZVTCO6 =__VT1__;
	__VT1__->ZVTCO6 =__VT0__;
	memset(__LB0__, 0, sizeof(LbBead));
}
