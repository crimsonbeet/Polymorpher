#include "stdafx.h" 

#include<MATH.H>
/**/
#ifdef CHAR
#undef CHAR
#endif
#ifdef SHORT
#undef SHORT
#endif
#define CHAR unsigned char
#define SHORT unsigned short
/**/
extern long*ZLDCO0; /*X-COORDINATE*/
extern long*ZLDCO1; /*Y-COORDINATE*/
extern long*ZLDCO2; /*X-COORDINATE*/
extern long*ZLDCO3; /*Y-COORDINATE*/
extern long*ZPTCO1; /*X-COORDINATE*/
extern long*ZPTCO2; /*Y-COORDINATE*/
extern char*ZLBCO7; /*ALREADY-CLASSIFIED-FLAG*/
extern CHAR*ZPVCO3; /*PSEUDO-SLOPE*/
extern char*ZVTCO8; /*TRAVERSAL-STATUS*/
extern char*ZLBCO1; /*POLYGON-RIGHT/LEFT-STATUS*/
extern CHAR*ZVTCO4; /*RIGHT-STATUS*/
extern CHAR*ZLBCO4; /*LAYER-CODE*/
extern SHORT*ZLbXx1; /*MIN-POINT-ON-POLYGON(AFTER-LOAD-MASTER)*/
extern SHORT*ZLbXx2; /*MAX-X-POINT-ON-POLYGON*/
extern SHORT*ZLbYy1; /*MIN-Y-POINT-ON-POLYGON*/
extern SHORT*ZLbYy2; /*MAX-Y-POINT-ON-POLYGON*/
extern SHORT*ZLBCO3; /*POINTER-TO-THE-FIRST-VECTOR-BEAD*/
extern SHORT*ZLBCO6; /*POLYGON-NUMBER*/
extern SHORT*ZLbNxt; /*NEXT-HASH-POLYGON*/
extern SHORT*ZLVCO1; /*NEXT-POLYGON-VECTOR(LV)-BEAD(CHAIN HEAD IS ZLBCO3!CIRCULAR)*/
extern SHORT*ZLVCO2; /*VECTOR-NUMBER*/
extern SHORT*ZLVCO3; /*NEXT-COINCIDENT-VECTOR-PTR(CHAIN HEAD IS ZVTCO5)*/
extern SHORT*ZLVCO5; /*FIRST-POINT*/
extern SHORT*ZLVCO6; /*SECOND-POINT*/
extern SHORT*ZLVCO7; /*PTR-TO-POLYGON-BEAD*/
extern SHORT*ZLVCO8; /*PTR-TO-CLASS-POLYGON*/
extern SHORT*ZPVCO5; /*POINTER-TO-NEXT-PV-BEAD-FOR-THIS-POINT(COUNTERCLOCKWISE)*/
extern SHORT*ZPTCO3; /*PTR-TO-PV-BEAD(CHAIN HEAD)*/
extern SHORT*ZPTCO5; /*NEXT-X-POINTER-IN-Y-CHAIN*/
extern SHORT*ZVTCO1; /*FIRST-POINT-NUMBER*/
extern SHORT*ZVTCO2; /*SECOND-POINT-NUMBER*/
extern SHORT*ZVTCO5; /*PTR-TO-FIRST-POLYGON-VECTOR-BEAD-CHAIN-OF-COINCIDENT-VECTORS(IS NEVER ZERO)*/
extern SHORT*ZVTCO6; /*ASSOCIATED-VECTOR(REVERSE DIRECTION)*/
extern SHORT*ZJSCO0; /*LV-WILL-BE-PROCESSED-BY-SET-INSIDE*/
extern SHORT*ZJSCO1; /*ENCLOSED-LB*/
extern double*VERCOX;
extern double*VERCOY;
extern double*VERSTW;
extern double*VERENW;
/**/
extern long VERMAX;
extern long ZLSLIM;
extern long ZLDLIM;
extern long ZLDMAX;
extern long ZLBLIM;
extern long ZLBMAX;
extern long ZLVLIM;
extern long ZLVMAX;
extern long ZPTLIM;
extern long ZPTMAX;
extern long ZYPLIM;
extern long ZYPMAX;
extern long ZPVLIM;
extern long ZPVMAX;
extern long ZVTLIM;
extern long ZVTMAX;
extern long ZJSLIM;
extern long ZJSMAX;
/**/
extern long CXMAX;
extern long CXMIN;
extern long CYMAX;
extern long CYMIN;
extern long PXMAX;
extern long PXMIN;
extern long PYMAX;
extern long PYMIN;
/**/
extern SHORT*ZPBCOR;
extern long PBLIM;
extern long PXCNT;
extern long PYCNT;
extern long XPDIV;
extern long YPDIV;
extern long PXMAX;
extern long PYMAX;
extern long PXMIN;
extern long PYMIN;
/**/
extern long Zptlim_After_Loadmaster;
/**/
void Determine_Point_Base(void);
/**/
long J_P_S;
long B_S_Lv_Only;
long S_FACTOR = 8;
char Without_Scale;
/**/
void
Back_Scale_1(void)
{
	register long p, l;
	double s;
	l = Zptlim_After_Loadmaster;

	s = 1L<<S_FACTOR;
	for(p = 1; p<l; ++p)
	{
	ZPTCO1[p] >>= S_FACTOR;
	ZPTCO2[p] >>= S_FACTOR;
	}
	for(p = l; p<ZPTLIM; ++p)
	{
	ZPTCO1[p] = (long)floor((double)ZPTCO1[p]/s +0.5);
	ZPTCO2[p] = (long)floor((double)ZPTCO2[p]/s +0.5);
	}
	XPDIV >>= S_FACTOR;
	YPDIV >>= S_FACTOR;
	PXMAX >>= S_FACTOR;
	PYMAX >>= S_FACTOR;
	PXMIN >>= S_FACTOR;
	PYMIN >>= S_FACTOR;
}
void
Back_Scale_2(void)
{
long X, Y, B, P;
register long p;
register long x, y;
register char f;

	Determine_Point_Base();
	f = 0;
	for(p = 1; p<ZPTLIM; ++p)
	{
	if(ZPTCO3[p])
	{
	/* REMAINED AFTER PRECEDED ITERATION. */
	/* MUST BE SKIPPED. */
	continue;
	}
	y = ZPTCO2[p];
	x = ZPTCO1[p];

	Y = ((y-PYMIN)/YPDIV);
	X = ((x-PXMIN)/XPDIV);

	if(X<0)X = 0;
	else if(X>PXCNT-1)X = PXCNT-1;

	if(Y<0)Y = 0;
	else if(Y>PYCNT-1)Y = PYCNT-1;

	B = X*PYCNT + Y, P = ZPBCOR[B];
	while(P)
	{
	if(ZPTCO1[P]==x && ZPTCO2[P]==y) break;
	P = ZPTCO5[P];
	}
	if(!P)
	{
	ZPTCO5[p] = ZPBCOR[B];
	ZPBCOR[B] = p;
	ZPTCO3[p] = 0;
	}
	else ZPTCO3[p] = P, f = 1;
	}
	J_P_S = f;
}
void
Back_Scale_4(void)
{
register long v, e, o;
long A, B, D;
	for(D = 1; D<ZLBLIM; ++D)
	{
	v = e = ZLBCO3[D];

	if(!v)continue;
	do
	{
	A = ZLVCO5[v];
	B = ZLVCO6[v];
	if(A==B) v = ZLVCO1[v];
	}
	while(v&&(A==B)&&(v!=e));
	if(A==B)
	{
	v = ZLBCO3[D], ZLBCO3[D] = 0;
	do
	{
	A = ZLVCO2[v];
	if(A)
	{
	B = ZVTCO6[A];
	ZVTCO6[A] = 0;
	ZVTCO6[B] = 0;
	}
	v = ZLVCO1[v];
	}
	while(v&&(v!=e));
	continue;
	}
	else if(v!=e) ZLBCO3[D] = v;
	e = v;
	o = e;
	v = ZLVCO1[v];
	do
	{
	A = ZLVCO5[v], B = ZLVCO6[v];
	if(A==B)
	{
	A = ZLVCO2[v];
	if(A)
	{
	B = ZVTCO6[A];
	ZVTCO6[A] = 0;
	ZVTCO6[B] = 0;
	}
	ZLVCO1[o] = ZLVCO1[v];
	}
	else o = v;
	v = ZLVCO1[v];
	}
	while(v&&(e!=v));
	}
}
void
Relink_Lv_Pt(long v)
{
long l;
	for(l = ZVTCO5[v]; l; l = ZLVCO3[l])
	{
	ZLVCO5[l] = ZVTCO1[v];
	ZLVCO6[l] = ZVTCO2[v];
	}
}
void
Back_Scale_5(void)
{
register long P1, P2;
register long vt, pt;

	for(vt = 1; vt<ZLVLIM; vt++)
	{
	P1 = ZLVCO5[vt];
	P2 = ZLVCO6[vt];

	pt = ZPTCO3[P1];
	if(pt)
	{
	ZLVCO5[vt] = pt;
	}
	pt = ZPTCO3[P2];
	if(pt)
	{
	ZLVCO6[vt] = pt;
	}
	}
}
void
Back_Scale_3(void)
{
long V4, P1, P2;

register long vt, pt;
register char FA, FB, fg;

	if(B_S_Lv_Only)
	{
	Back_Scale_5();
	return;
	}
	fg = 0;
	for(vt = 2; vt<ZVTLIM; vt+=2)
	{
	FA = FB = 0;

	V4 = ZVTCO6[vt];
	if(V4){}
	else continue;

	P1 = ZVTCO1[vt];
	P2 = ZVTCO2[vt];

	pt = ZPTCO3[P1];
	if(pt)
	{
	ZVTCO1[vt] = ZVTCO2[V4] = pt;
	FA = 1;
	}
	pt = ZPTCO3[P2];
	if(pt)
	{
	ZVTCO2[vt] = ZVTCO1[V4] = pt;
	FB = 1;
	}
	if(FA||FB)
	{
	Relink_Lv_Pt(vt);
	Relink_Lv_Pt(V4);
	fg = 1;
	}
	}
	J_P_S = fg;
}
long
Back_Scale_0(void)
{
long p;
	J_P_S = 0;

	if(Without_Scale)return(J_P_S);

	for(p = 1; p<ZPTLIM; ++p)
	ZPTCO3[p] = 0;

	Back_Scale_1();
	Back_Scale_2();

	if(J_P_S)
	Back_Scale_3();

	if(J_P_S)
	Back_Scale_4();

	for(p = 1; p<ZPTLIM; ++p)
	ZPTCO3[p] = 0;

	return(J_P_S);
}
