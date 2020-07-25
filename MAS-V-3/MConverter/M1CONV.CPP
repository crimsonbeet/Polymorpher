#include "stdafx.h" 

#define M_CONV_MODULE
/**/
#include<DOS.H>
#include<MATH.H>
#include<STDIO.H>
/**/
#ifdef __BORLANDC__
#include<DIR.H>
#include<ALLOC.H>
#endif
/**/
#include<STDLIB.H>
#include<CTYPE.H>
/**/
#include<STRING.H>
#ifndef stricmp
#define stricmp strcmp
#endif
/**/
#ifdef __BORLANDC__
#include<VALUES.H>
#else
#include<FLOAT.H>
#define MAXFLOAT FLT_MAX
#endif
/**/
#include<LIMITS.H>
#include<PROCESS.H>
/**/
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
extern long STATUS;
extern char S_Free;
/**/
void ZOUTABORT(void);
void ZSWAB(long*, long*);
long ZLBNEXT(void);
long ZLVNEXT(void);
void INTERSECTION_ANALYSIS(void);
void POINT_VECTOR_ANALYSIS(long);
void ZZ_SET_RSTAT(long);
void FORCE_C_POLYGONS(void);
void ZZ_EXPLODE_CHAIN(void);
long INSERTV(long, long, long);
void Z_Check_Closure(void);
void Z_FEval_Storage(void);
void Determine_Point_Base(void);
long IPOINT3(long, long);
long SPLITVT(long, long);
/**/
long ZXgCO0[3]; /*GRID-X-BOUNDARY-TABLE*/
long ZYgCO0[3]; /*GRID-Y-BOUNDARY-TABLE*/
SHORT ZgACO2[9]; /*POINTER-TO-FIRST-gV-BEAD-FOR-THIS-BLOCK*/
SHORT*ZgVCO1; /*LINK-TO-THE-NEXT-gV-BEAD*/
SHORT*ZgVCO2; /*FIRST-J-POINT*/
SHORT*ZgVCO3; /*SECOND-J-POINT*/

long ZgVLIM;
long ZgVMAX;
/**/
long XgDIV; /*GRID-BLOCK-SIZE-UNITS-WIDE*/
long YgDIV; /*GRID-BLOCK-SIZE*/
long XgCNT; /*TOTAL-BLOCKS-IN-X-DIRECTION*/
long YgCNT; /*TOTAL-BLOCKS-IN-Y-DIRECTION*/
/**/
long SP_YEND; /*END-Yg-FOR-LOOP*/
long SP_YBEg; /*START-Yg*/
long SP_YSTA; /*Y-COORD-SETTED-BY-PREVIOUSLY-COMPUTATING*/
long SP_XSTA; /*X-COORD*/
long SP_XFIR; /*FIRST-X-COORD-OF-A-VECTOR*/
long SP_YFIR; /*FIRST-Y-COORD*/
long VTMATCH; /*RIGHT-VT-VECTOR-MATCHED-LV-VECTOR*/
long SP_SWIT; /*VECTOR-WAS-SWITCHED-FLAG*/
long XSECOND; /*SECOND-X*/
long YSECOND; /*SECOND-Y*/
long SP_XMIN; /*MINIMAL-Xg-FOR-THIS-POLY*/
long SP_XMAX; /*MAX-Xg*/
long SP_YMIN; /*MINIMAL-Yg-FOR-THIS-VECTOR*/
long SP_YMAX; /*MAX-Yg*/

long XROUND;
long YROUND;
double SP_SLOP; /*SLOPE-OF-A-VECTOR*/
/**/
#define EVALUATEX(Y) (floor(((double)((Y)-SP_YFIR))/SP_SLOP+SP_XFIR+0.5))
#define EVALUATEY(X) (floor(SP_SLOP*((X)-SP_XFIR)+SP_YFIR+0.5))
char SP_SX;
char SP_SY;
long
ZgVNEXT(void)
{
if((ZgVLIM+1)>=ZgVMAX)ZOUTABORT();
return(ZgVLIM++);
}
void
DETERMINEGRIDVECTOR(long Xg, long Yg)
{
	long VT;
	long gV;
	long gB;
	long IX;
	long IY;
	long JY;
	long JX;

	long IY0;
	long IX1;
	long IY1;

	IX1 = ZXgCO0[Xg];
	IY1 = ZYgCO0[Yg], IY0 = ZYgCO0[Yg-1];

	IX = SP_XSTA;
	IY = SP_YSTA;

	if(!SP_SX) JX = IX, JY = YSECOND<IY1 ? YSECOND : IY1;
	else
	if(!SP_SY) JY = IY, JX = XSECOND<IX1 ? XSECOND : IX1;
	else
	{
	if(IX1<XSECOND) JX = IX1, JY = EVALUATEY(JX);
	else JX = XSECOND, JY = YSECOND;
	if(SP_SY==+1)
	{
	if(JY>IY1) JY = IY1, JX = EVALUATEX(JY);
	}
	else
	if(JY<IY0) JY = IY0, JX = EVALUATEX(JY);
	}
	SP_XSTA = JX, SP_YSTA = JY;
	if(JX==IX&&JY==IY)
	{
	return;
	}
	VT = VTMATCH;
	if(JX!=XSECOND||JY!=YSECOND)
	{
	long IP;
	IP = IPOINT3(JX, JY);
	VTMATCH = SPLITVT(VT, IP);
	}
	if(SP_SWIT)
	{
	VT = ZVTCO6[VT];
	}
	if(IX==JX&&JX==IX1&&Xg!=XgCNT)
	{
	Xg += XROUND;
	}
	gV = ZgVNEXT(), gB = (Xg-1)*YgCNT+Yg-1;
	ZgVCO2[gV] = ZVTCO1[VT];
	ZgVCO3[gV] = ZVTCO2[VT];
	ZgVCO1[gV] = ZgACO2[gB];
	ZgACO2[gB] = gV;
}
void
Set_Ag_Flags(long Xg)
{
	long Yg;
	long LP;
	long SI;

	if(SP_XFIR==XSECOND) SP_SX = 0;
	else SP_SX =+1;
	if(SP_YFIR<YSECOND) SP_SY =+1;
	else if(SP_YFIR>YSECOND) SP_SY =-1;
	else SP_SY = 0;

	if(!SP_SX&&!SP_SY) return;

	SI = SP_YBEg<SP_YEND? +1: -1;
	LP = (SI*(SP_YEND-SP_YBEg))+1;

	for(Yg=SP_YBEg; LP; --LP, Yg+=SI)
	{
	DETERMINEGRIDVECTOR(Xg, Yg);
	}
}
void
SPLIT_gVECTOR_2(long LV)
{
char SW;
long X1, X2, Y1, Y2, S2, DX, DY;
long Xg, IX, IY, Xg1, Xg2, Yg1, Yg2;
long XR, YR, XB, YB;
	X1 = ZPTCO1[ZLVCO5[LV]];
	Y1 = ZPTCO2[ZLVCO5[LV]];
	Y2 = ZPTCO2[ZLVCO6[LV]];
	X2 = ZPTCO1[ZLVCO6[LV]];
	DX = X2-X1;
	DY = Y2-Y1;
	SW = 0;
	YROUND = XROUND = 0;
	if(!DX) /*X1-Eg-X2*/
	{
	if(Y1>Y2) S2 =-STATUS, ZSWAB(&Y1, &Y2);
	else S2 = STATUS;
	XR = S2==+1;
	YR = 1;
	}
	else /*X1-NE-X2*/
	{
	if(X1>X2) S2 =-STATUS, ZSWAB(&X1, &X2), ZSWAB(&Y1, &Y2);
	else S2 = STATUS;
	XR = 1;
	if(!DY) /*Y1-Eg-Y2*/ YR = S2==-1;
	else /*Y1-NE-Y2-(ALSO-X1-NE-X2)*/
	{
	YR = 1;
	if(Y2<Y1) /*NEGATIVE-SLOPE*/
	{
	ZSWAB(&Y1, &Y2);
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
	YB = ZYgCO0[++Yg1];

	if(YR&&YB==Y1)
	YB = ZYgCO0[++Yg1];

	Yg2 = Yg1;
	while(Y2>YB)
	YB = ZYgCO0[++Yg2];

	Xg1 = 1;
	while(X1>XB)
	XB = ZXgCO0[++Xg1];

	if(XR&&X1==XB)
	XB = ZXgCO0[++Xg1];

	Xg2 = Xg1;
	while(X2>XB)
	XB = ZXgCO0[++Xg2];

	if(SW) ZSWAB(&Y1, &Y2), ZSWAB(&Yg1, &Yg2);
	if(Xg1<SP_XMIN) SP_XMIN = Xg1;
	if(Yg1<SP_YMIN) SP_YMIN = Yg1;
	if(Yg2>SP_YMAX) SP_YMAX = Yg2;
	if(Yg2<SP_YMIN) SP_YMIN = Yg2;
	if(Yg1>SP_YMAX) SP_YMAX = Yg1;
	if(Xg2>SP_XMAX) SP_XMAX = Xg2;
	SP_XFIR = SP_XSTA = X1;
	SP_YFIR = SP_YSTA = Y1;
	XSECOND = X2;
	YSECOND = Y2;
	SP_SWIT = S2!=STATUS;
	VTMATCH = SP_SWIT? ZVTCO6[ZLVCO2[LV]]: ZLVCO2[LV];
	SP_SLOP = DX? (DY? ((double)DY)/DX: 0): LONG_MAX;
	/*FOR-EACH-BUT-THE-LAST-Xg-VALUE*/
	SP_YEND = SP_YBEg = Yg1;
	if(Yg1==Yg2)
	for(Xg=Xg1; Xg<Xg2; ++Xg) Set_Ag_Flags(Xg);
	else
	for(Xg=Xg1; Xg<Xg2; ++Xg)
	{
	IX = ZXgCO0[Xg];
	IY = floor(SP_SLOP*(IX-X1)+Y1+0.5);
	if(SW) /*NEGATIVE-SLOPE*/
	{
	YB = ZYgCO0[SP_YEND-1];
	while(IY<YB) YB = ZYgCO0[--SP_YEND-1];
	if(IY==YB&&S2==+1) --SP_YEND;
	}
	else
	{
	YB = ZYgCO0[SP_YEND];
	while(IY>YB) YB = ZYgCO0[++SP_YEND];
	if(IY==YB&&S2==-1) ++SP_YEND;
	}
	Set_Ag_Flags(Xg);
	SP_YBEg = SP_YEND;
	}
	/*NOW-FOR-THE-LAST-Xg-VALUE*/
	Xg = Xg2;
	SP_YEND = Yg2;
	Set_Ag_Flags(Xg);
}
void
DEFINELBGRIDDATA(long LB)
{
long LE, LV, LN;
	LE = LV = ZLBCO3[LB];
	if(LE) do
	{
	LN = ZLVCO1[LV];
	SPLIT_gVECTOR_2(LV);
	LV = LN;
	}
	while(LV!=LE);
}
void
LOADGRIDBOUNDARY(long Xg, long Yg)
{
	ZLDLIM = 5;
	ZLDCO1[1]=ZLDCO1[2]=ZYgCO0[Yg];
	ZLDCO1[0]=ZLDCO1[3]=ZYgCO0[Yg-1];
	ZLDCO0[2]=ZLDCO0[3]=ZXgCO0[Xg];
	ZLDCO0[0]=ZLDCO0[1]=ZXgCO0[Xg-1];
	ZLDCO0[4] = ZLDCO0[0];
	ZLDCO1[4] = ZLDCO1[0];
}
void
LOADGRIDVECTORS(long gB)
{
	long gV;
	long P1;
	long P2;
	long LV;
	long LB;
	for(gV = ZgACO2[gB]; gV; gV = ZgVCO1[gV])
	{
	P1 = ZgVCO2[gV];
	P2 = ZgVCO3[gV];

	ZLBCO7[LB = ZLBNEXT()] = 0;
	ZLBCO4[LB] = 0;

	ZLBCO3[LB] = ZLVLIM;
	ZLBCO1[LB] = STATUS;
	LV = ZLVNEXT();
	ZLVCO7[LV] = LB;
	ZLVCO5[LV] = P1;
	ZLVCO6[LV] = P2;
	ZLVCO2[LV] = 0L;
	ZLVCO1[LV] = LV;
	ZLVCO3[LV] = 0L;
	ZLVCO8[LV] = 0L;
	ZLVCO1[LV] = LV;
	ZLbXx1[LB] = P1;
	ZLbYy1[LB] = P1;
	ZLbYy2[LB] = P1;
	ZLbXx2[LB] = P1;
	}
}
void
CLEAR_PV_POINTERS(void)
{
	long I;
	for(I = 0; I<ZPTLIM; ++I)
	ZPTCO3[I] = 0;
}
void
FULLANALYSIS(long X, long Y)
{
	long B, M, S, L;
	ZLBLIM=ZLVLIM=1;
	ZVTLIM = 2;
	B = (X-1)*YgCNT+(Y-1);
	LOADGRIDVECTORS(B);

	S = STATUS;
	STATUS = 1;
	S_Free = 0;

	L = ZLBLIM;
	LOADGRIDBOUNDARY(X, Y);
	Z_FEval_Storage();
	CLEAR_PV_POINTERS();

	INTERSECTION_ANALYSIS();
	POINT_VECTOR_ANALYSIS(2);

	for(M = 1; M<L; ++M)
	ZZ_SET_RSTAT(M);

	FORCE_C_POLYGONS();
	ZZ_EXPLODE_CHAIN();

	STATUS = S;
}
void
INSERT_VECTORS(long L)
{
long E, V;
	V = E = ZLBCO3[L];
	do
	{
	INSERTV(ZLVCO5[V], ZLVCO6[V], V);
	V = ZLVCO1[V];
	}
	while(V!=E);
}
void
SET_Xg_Yg(void)
{
	long X;
	long Y;
	long P;
	long D;
	long V;
	long Z;
	long I;

	long*Zldcor;
	ZgVCO1 = (SHORT*)VERCOX;
	ZgVCO2 = (SHORT*)VERCOY;
	ZgVCO3 = (SHORT*)VERSTW;

	ZgVMAX = VERMAX*4;
	ZgVLIM = 1;

	PXMAX = PXMIN = ZLDCO0[0];
	PYMAX = PYMIN = ZLDCO1[0];

	for(P = 1; P<ZLDLIM; ++P)
	{
	if(PXMIN>ZLDCO0[P])PXMIN = ZLDCO0[P];
	if(PXMAX<ZLDCO0[P])PXMAX = ZLDCO0[P];
	if(PYMIN>ZLDCO1[P])PYMIN = ZLDCO1[P];
	if(PYMAX<ZLDCO1[P])PYMAX = ZLDCO1[P];
	}
	Determine_Point_Base();
	X = PXMAX-PXMIN;
	Y = PYMAX-PYMIN;
	YgCNT = 1;
	XgCNT = 1;
	if(X>Y)
	{
	XgCNT++;
	P = Z =((D = X)>>1) + PXMIN;
	Zldcor = ZLDCO0;
	}
	else
	{
	YgCNT++;
	P = Z =((D = Y)>>1) + PYMIN;
	Zldcor = ZLDCO1;
	}
	for(I = 0; I<ZLDLIM; ++I)
	{
	V = Zldcor[I] - Z;
	if(V<0)V = -V;
	if(V<D)
	{
	D = V;
	P = Zldcor[I];
	}
	}
	ZXgCO0[0] = PXMIN;
	ZXgCO0[XgCNT] = PXMAX;

	ZYgCO0[0] = PYMIN;
	ZYgCO0[YgCNT] = PYMAX;

	if(X>Y)ZXgCO0[1] = P;
	else ZYgCO0[1] = P;

	CXMIN = PXMIN;
	CYMIN = PYMIN;
	CXMAX = PXMAX;
	CYMAX = PYMAX;
}
void
SPLIT_POLYGON(void)
{
	long X;
	long Y;
	long B;
	ZLBLIM=ZLVLIM=ZPTLIM=ZYPLIM=ZgVLIM=1;
	ZVTLIM = 2;
	STATUS = 1;

	SET_Xg_Yg();

	SP_XMIN=XgCNT, SP_XMAX=0;
	SP_YMIN=YgCNT, SP_YMAX=0;

	Z_Check_Closure();
	Z_FEval_Storage();
	INSERT_VECTORS(1);

	DEFINELBGRIDDATA(1);

	for(Y = SP_YMIN; Y<=SP_YMAX; ++Y)
	{
		for(X = SP_XMIN; X<=SP_XMAX; ++X)
		{
			B = (X-1)*YgCNT+(Y-1);
			if(ZgACO2[B])
			{
				FULLANALYSIS(X, Y);
				ZgACO2[B] = 0;
			}
		} /*END-LOOP-OVER-Xg*/
	} /*END-LOOP-OVER-Yg*/
}
