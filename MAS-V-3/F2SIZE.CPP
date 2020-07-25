#define F_SIZE_MODULE
/**/
#include<DOS.H>
#include<MATH.H>
#include<STDIO.H>
#ifdef __BORLANDC__
#include<DIR.H>
#include<ALLOC.H>
#endif
/**/
#include<STDLIB.H>
#include<CTYPE.H>
#include<STRING.H>
/**/
#ifdef __BORLANDC__
#include<VALUES.H>
#endif
/**/
#include<LIMITS.H>
#include<PROCESS.H>
/**/
#include "fqeval.hpp"
#include "fxeval.hpp"
#include "_scale.hpp"
#include "tom_aray.hpp"
extern void zabort(void);
extern void __DXBLD__(char closure, char*s);
extern void PrintOk(void);
char*f2size="f2size";
/**/
long Total_Collar;
extern long Total_Shapes;
long PLDLIM;

double __SIZWID__;
char   __SISIGN__;

double NXFROM;
double NYFROM;
double XELBOW;
double YELBOW;

double X_C[9];
double Y_C[9];

char F_C;
char F_C_OLD;
char S_C_F_S;

char __iEx__;

char NO_CONT_1;
char NO_CONT_2;
void
__PsPLINE__(LbBead*LB)
{
	LvBead*LE;
	LvBead*LV;
	LvBead*LN;

	double A1;
	double A2;

/*Only After Force_C_Polygonc and*/
/*Dummyjunctionanalysis and S_FACTOR!=0.*/

	LE = LB->ZLBCO3;
	LN = LE->ZLVCO1;
	LV = LE;
	A1 = COMPLVANGLE(LV);
	do
	{
		A2 = COMPLVANGLE(LN);
		if(_EQ_(A1, A2, 0.1))
		{
			__bSb__ = 1;
			REMOVE_FROM_VT_CHAIN(LN);
			REMOVE_FROM_VT_CHAIN(LV);
			LN->ZLVCO7 = __LB0__;
			LN->ZLVCO2 = __VT0__;
			LV->ZLVCO2 = __VT0__;
			LB->ZLBCO3 = LV;
			LV->ZLVCO6 = LN->ZLVCO6;
			LV->ZLVCO1 = LN->ZLVCO1;
			A1 = COMPLVANGLE(LV);
			if(LN==LE)
			{
				return;
			}
		}
		else
		{
			LV = LN;A1 = A2;
		}
		LN = LV->ZLVCO1;
	}
	while(LV!=LE);
}
void
__OVELSEG__(void)
{
	long I;
	long K;
	ZVTLIM = 2;
	INTERSECTION_ANALYSIS();
	POINT_VECTOR_ANALYSIS(2);
	for(I = 1; I<ZLBLIM; ++I)
	{
		LbBead*Lb = __GLB__(I);
		if(Lb->ZLBCO3)
		DUMMYJUNCTIONANALYSIS(Lb);
	}
	if(__iEx__)
	{
		K = 2;
		True_Stat = AND_NOT;
		__GrpNONE__= 1;
		POINT_VECTOR_CLASS_2(__GLB__(1), 8, 8);
		__GrpNONE__= 0;
	}
	else	K = 1;
	for(I = K; I<ZLBLIM; ++I)
	{
		LbBead*Lb = __GLB__(I);
		if(Lb->ZLBCO3)Set_Rstat(Lb, 1);
	}
	FORCE_C_POLYGONS();
	if(!__iEx__&&!S_FACTOR)
	{
		K = ZLBLIM;
	}
	else	K = 1;
	for(I = K; I<ZLBLIM; ++I)
	{
		LbBead*Lb = __GLB__(I);
		if(Lb->ZLBCO3)
		DUMMYJUNCTIONANALYSIS(Lb);
	}
	__bSb__ = 0;
	if(__iEx__)
	for(I = 1; I<ZLBLIM; ++I)
	{
		LbBead*Lb = __GLB__(I);
		if(Lb->ZLBCO3)
		{
			__PsPLINE__(Lb);
		}
	}
	True_Stat = O_R;
	__iEx__ = 0;
}
void
__coOut__(long j)
{
	long i;
	for(i = j; i<ZLBLIM; ++i)
	{
		LbBead*Lb=__GLB__(i);
		if(Lb->ZLBCO3==0)
		{
			continue;
		}
		if(!__UNL__(Lb))
		{
			continue;
		}
		++Total_Shapes;
		for(j = 0; j<ZLDLIM; ++j)
		{
			ZLDCO0[j] += Delta_X_2;
			ZLDCO1[j] += Delta_Y_2;
		}
		MaSCOT_IO.DIX_OpenS = Open_Shape;
		MaSCOT_IO.Rewrite_FP_S = 1;
		MaSCOT_IO.Dix_output();
		MaSCOT_IO.Rewrite_FP_S = 0;
	}
}
void
__READSH__(void)
{
	__INTOPLD__= 1;
	SetUp_Min_Max= 1;
	MaSCOT_IO.ZDREAD_P();
	if(ZLDLIM)
	if(!Open_Shape)
	{
		long x = ZLDCO4[1];
		long y = ZLDCO5[1];
		if(x!=ZLDCO4[ZLDLIM]||y!=ZLDCO5[ZLDLIM])
		{
			++ZLDLIM;
			ZLDCO4[ZLDLIM]=x;
			ZLDCO5[ZLDLIM]=y;
		}
	}
	PLDLIM = ZLDLIM+1;
	__INTOPLD__= 0;
	SetUp_Min_Max= 0;
}
double
DEFINE_CSLOPE(long K, long I, long J)
{
	double X;
	double Y;
	double A;
	X = ZLDCO4[J]-ZLDCO4[I];
	Y = ZLDCO5[J]-ZLDCO5[I];
	A = Eval_Angle(X, Y);
	X = ZLDCO4[K]-ZLDCO4[I];
	Y = ZLDCO5[K]-ZLDCO5[I];
	A = A + 360-Eval_Angle(X, Y);
	if(A>360)A-=360;
	return(A);
}
long
SPLINE_TYPE(double A)
{
	long T;
	double D =/*__SISIGN__<0? 45:*/ 15;
	if(_EQ_(A, 180, 0.1)) T = 0;
	else
	if(A<=270) T = 1;
	else
	if(_EQ_(A, 360, D)) T = 2;
	else T = 3;
	return(T);
}
void
DEFINE_DISPLACE(long I, long K, double*X, double*Y)
{
	X[0] = X[2]*__SIZWID__+ZLDCO4[I];
	Y[0] = Y[2]*__SIZWID__+ZLDCO5[I];
	X[1] = X[2]*__SIZWID__+ZLDCO4[K];
	Y[1] = Y[2]*__SIZWID__+ZLDCO5[K];
}
void DEFINE_NORMALFROM(long I, long K)
{
	double SZ;
	double DX;
	double DY;
	double NX;
	double NY;
	DX = ZLDCO4[K]-ZLDCO4[I];
	DY = ZLDCO5[K]-ZLDCO5[I];
	if(DX!=0&&DY!=0)
	{
		SZ = sqrt(DX*DX+DY*DY);
		NX = (+DY)/SZ;
		NY = (-DX)/SZ;
	}
	else
	{
		NX = DY? (DY>0? 1: -1): 0;
		NY = DX? (DX>0? -1: 1): 0;
	}
	NXFROM = NX;
	NYFROM = NY;
}
char
DEFINE_INTERSECTION
(double*X1, double*Y1, double*X2, double*Y2, double*X, double*Y)
{
	double DL, A1, B1, C1, A2, B2, C2;
	A1 = Y1[0]-Y1[1];
	B1 = X1[1]-X1[0];
	C1 = X1[0]*Y1[1]-X1[1]*Y1[0];

	A2 = Y2[0]-Y2[1];
	B2 = X2[1]-X2[0];
	C2 = X2[0]*Y2[1]-X2[1]*Y2[0];

	DL = A1*B2 - B1*A2;
	if(_EQ_(DL, 0, 1E-7))return(0);

	X[0] = (B1*C2-B2*C1)/DL;
	Y[0] = (A2*C1-A1*C2)/DL;
	if(X[0]>LONG_MAX||Y[0]>LONG_MAX)return(0);
	return(1);
}
void
DEFINE_ELBOW
(double*X1, double*Y1, long I)
{
	X1[5] = XELBOW + ZLDCO4[I];
	Y1[5] = YELBOW + ZLDCO5[I];
	X1[6] = ZLDCO4[I] - XELBOW;
	Y1[6] = ZLDCO5[I] - YELBOW;
}
long
POINTONSEGMENT
(double*X1, double*Y1, double X, double Y)
{
	double XMIN, YMIN, XMAX, YMAX;
	XMIN = X1[1];
	YMIN = Y1[1];
	if(XMIN>X1[0]) XMAX = XMIN, XMIN = X1[0];
	else XMAX = X1[0];
	if(YMIN>Y1[0]) YMAX = YMIN, YMIN = Y1[0];
	else YMAX = Y1[0];
	XMIN -= 0.5;
	XMAX += 0.5;
	YMIN -= 0.5;
	YMAX += 0.5;
	return((X>=XMIN&&X<=XMAX)&&(Y>=YMIN&&Y<=YMAX));
}
void
__UNLINTSTD__(void)
{
	long I;
	long J;
	J = ZLDLIM;
	if(J==0)
	{
		return;
	}
	while(J--)
	{
		I = ZLDLIM++;
		ZLDCO0[I] = ZLDCO2[J];
		ZLDCO1[I] = ZLDCO3[J];
	}
	STATUS = 1;
	Check_Closure();
	Loadmaster();
}
void
UNL_TOINTLAYSTANDART(double*X, double*Y)
{
	long J;
	long I;
	long L;
	long s;
	long x[5];
	long y[5];
	_sAlong*X0 = &ZLDCO0;
	_sAlong*X1 = &ZLDCO2;
	_sAlong*Y0 = &ZLDCO1;
	_sAlong*Y1 = &ZLDCO3;
	s = 1L<<S_FACTOR;
	L = 0;
	for(J = 4; J<9; ++J)
	{
		I = L++;
		x[I] = floor(X[J]*s+0.5);
		y[I] = floor(Y[J]*s+0.5);
	}
	J = DETERMINE_STAT_3(x, y, L);
	if(J==0)
	{
		DEFINE_INTERSECTION(X+5, Y+5, X+7, Y+7, X, Y);
		x[2] = floor(X[0]*s+0.5);
		y[2] = floor(Y[0]*s+0.5);
		x[3] = x[2];
		y[3] = y[2];
		J = DETERMINE_STAT_3(x, y, L);
		if(J==0)
		{
			return;
		}
	}
	if(J!=1)
	{
		_sAlong*X2 = X0;
		_sAlong*Y2 = Y0;
		X0 = X1, X1 = X2;
		Y0 = Y1, Y1 = Y2;
	}
	if(ZLDLIM!=0)
	{
		I = ZLDLIM-1;
		J = 0;
		if((*X0)[I]!=x[0])J=1;
		else
		if((*X1)[I]!=x[3])J=1;
		else
		if((*Y0)[I]!=y[0])J=1;
		else
		if((*Y1)[I]!=y[3])J=1;
		if(J||NO_CONT_2)
		{
			__UNLINTSTD__();
			ZLDLIM = 0;
		}
	}
	if(ZLDLIM==0)
	{
		I = ZLDLIM++;
		(*X0)[I] = x[0];
		(*X1)[I] = x[3];
		(*Y0)[I] = y[0];
		(*Y1)[I] = y[3];
	}
	I = ZLDLIM++;
	(*X0)[I] = x[1];
	(*X1)[I] = x[2];
	(*Y0)[I] = y[1];
	(*Y1)[I] = y[2];
	STATUS = 0;
}
void
d_swap(double*A, double*B)
{
double T = *A; *A = *B, *B = T;
}
void
DEFINE_SEGMENT(char S, long J, long I, long K)
{
double IX, IY, X1[9], X2[3], Y1[9], Y2[3], CAN, MAN;
/*[0]-FIRST-DISPLACE-POINT!*/
/*[1]-SECOND-DISP-POINT!*/
/*[2]-NORMAL*/
	static double CAN_OLD;
	long SW0, SW1, SW2, SW3, SPT;/*SPECIAL-FLAGS*/
	static long SW4;
	NO_CONT_1 = 0;
	if(S==0)CAN_OLD = 0;
	X1[2] = NXFROM;
	Y1[2] = NYFROM;
	DEFINE_NORMALFROM(I, K);
	X2[2] = NXFROM;
	Y2[2] = NYFROM;
	CAN = DEFINE_CSLOPE(J, I, K);
	if(CAN<180)
	{
		X1[2] =-X1[2], Y1[2] =-Y1[2];
		X2[2] =-X2[2], Y2[2] =-Y2[2];
		XELBOW =-XELBOW;
		YELBOW =-YELBOW;
		MAN = 360-CAN;
		SW0 = 1;
	}
	else MAN = CAN, SW0 = 0;

	SPT = 0;
	F_C = 0;
	if(!SPT)SPT = SPLINE_TYPE(MAN);

	DEFINE_DISPLACE(J, I, X1, Y1);
	DEFINE_DISPLACE(I, K, X2, Y2);

	if(SPT>0)
	{
		SW3 = DEFINE_INTERSECTION(X1, Y1, X2, Y2, &IX, &IY);
		if(!SW3)SPT = 0;
		else
		IX -= ZLDCO4[I],IY -= ZLDCO5[I];
	}
	X1[4] = XELBOW +ZLDCO4[J];
	Y1[4] = YELBOW +ZLDCO5[J];
	X1[7] = ZLDCO4[J] -XELBOW;
	Y1[7] = ZLDCO5[J] -YELBOW;
	X1[5] = X1[1];
	Y1[5] = Y1[1];
	long Xi = ZLDCO4[I];
	long Yi = ZLDCO5[I];
	X1[6] = (Xi<<1)-X1[1];
	Y1[6] = (Yi<<1)-Y1[1];
	switch(SPT)
	{
		case 0: /*GORIZONTAL!NEEDED-BOX-GENERATING*/
		SW1 = SW2 = 1;
		XELBOW = X1[2]*__SIZWID__;
		YELBOW = Y1[2]*__SIZWID__;
		if(SW0)
		XELBOW =-XELBOW, YELBOW =-YELBOW;
		if(SW4)
		{
			d_swap(X1+4, X1+7);
			d_swap(Y1+4, Y1+7);
			d_swap(X1+5, X1+6);
			d_swap(Y1+5, Y1+6);
		}
		break;
		case 1: /*NOT-ACUTE-ANGLE*/
		SW1 =-1, SW2 =+1;
		XELBOW = IX;
		YELBOW = IY;
		DEFINE_ELBOW(X1, Y1, I);
		if(SW0)
		XELBOW =-XELBOW, YELBOW =-YELBOW;
		if(SW4&&!SW0)CAN_OLD -= 360;
		if(!SW4&&SW0)CAN_OLD *=-1.0;

		if(!F_C_OLD)
		if((MAN+CAN_OLD)<0)
		{
			d_swap(X1+4, X1+7);
			d_swap(Y1+4, Y1+7);
			d_swap(X1+5, X1+6);
			d_swap(Y1+5, Y1+6);
		}
		break;
		case 2: /*ACUTE-ANGLE*/
		case 3:
		/*Mirror Normals*/
		X1[2] =-X1[2];
		Y1[2] =-Y1[2];
		X2[2] =-X2[2];
		Y2[2] =-Y2[2];

		/*Insisde Edges*/
		DEFINE_DISPLACE(J, I, X1, Y1);
		DEFINE_DISPLACE(I, K, X2, Y2);

		/*Check For Cover Making*/
		SW3 = DEFINE_INTERSECTION(X1, Y1, X2, Y2, X1+3, Y1+3);
		if(!SW3)zAbort(f2size,__LINE__);
		SW1 = POINTONSEGMENT(X1, Y1, X1[3], Y1[3]);
		SW2 = POINTONSEGMENT(X2, Y2, X1[3], Y1[3]);
		if(SW1&&SW2&&SPT!=2)
		{
			XELBOW = IX;
			YELBOW = IY;
			DEFINE_ELBOW(X1, Y1, I);
			if(SW0)
			XELBOW =-XELBOW, YELBOW =-YELBOW;

			if(SW4&&!SW0)CAN_OLD -= 360;
			if(!SW4&&SW0)CAN_OLD *=-1.0;

			if(!F_C_OLD)
			if((MAN+CAN_OLD)<0)
			{
				d_swap(X1+4, X1+7);
				d_swap(Y1+4, Y1+7);
				d_swap(X1+5, X1+6);
				d_swap(Y1+5, Y1+6);
			}
			break;
		}
		NO_CONT_1 = 1;
		X2[2] =-X2[2];
		Y2[2] =-Y2[2];
		XELBOW = X2[2]*__SIZWID__;
		YELBOW = Y2[2]*__SIZWID__;

		X_C[5] = X1[5]+XELBOW;
		Y_C[5] = Y1[5]+YELBOW;
		X_C[6] = Xi+XELBOW;
		Y_C[6] = Yi+YELBOW;
		X_C[7] = Xi;
		Y_C[7] = Yi;
		X_C[8] = X_C[4] = X1[5];
		Y_C[8] = Y_C[4] = Y1[5];

		if(SW0)
		XELBOW =-XELBOW, YELBOW =-YELBOW;
		if(SW4!=SW0)
		{
			d_swap(X1+4, X1+7);
			d_swap(Y1+4, Y1+7);
			d_swap(X1+5, X1+6);
			d_swap(Y1+5, Y1+6);
		}
		F_C = 1;
		break;
		default: zAbort(f2size,__LINE__);
	}
	X1[8] = X1[4];
	Y1[8] = Y1[4];
	if(S)
	UNL_TOINTLAYSTANDART(X1, Y1);

	SW4 = SW0;
	F_C_OLD = F_C;
	CAN_OLD = CAN;
	NO_CONT_2 = NO_CONT_1;
}
void
Shapes_Collar_Generater(void)
{
	long S;
	long K;
	long T;
	long I;
	long J;
	long Q;
	long Z;

	if(PLDLIM<2)
	{
		return;
	}
	Delta_X_2 = PXMIN -= __SIZWID__;
	Delta_Y_2 = PYMIN -= __SIZWID__;
	PXMAX += __SIZWID__;
	PYMAX += __SIZWID__;

	for(I = 1; I<PLDLIM; ++I)
	{
		ZLDCO4[I] -= PXMIN;
		ZLDCO5[I] -= PYMIN;
	}
	PXMAX -= PXMIN;
	PYMAX -= PYMIN;

	PXMIN = PYMIN = 0;
	CleanZPBCOR(ZPBLIM+1);

	if(!Open_Shape)
	{
/*LAST POINT .EQ. FIRST POINT ALWAYS.*/
/*DUPLICATE POINTS NEVER TAKES PLACE.*/
		K = PLDLIM -2;
		ZLDCO4[0] = ZLDCO4[K];
		ZLDCO5[0] = ZLDCO5[K];
		Q = 0;
		S = 1;
		T = 2;
		I = PLDLIM++;
		ZLDCO4[I] = ZLDCO4[T];
		ZLDCO5[I] = ZLDCO5[T];
	}
	else
	{
		Q = 1;
		S = 2;
		T = 3;
	}
	Z = S;
	/*SCALE POINTS*/
	J = PYMAX>PXMAX? PYMAX: PXMAX;
	S_FACTOR = 0;
	do
	{
		++S_FACTOR;
	}
	while(((J<<S_FACTOR)>>S_FACTOR)==J);
	S_FACTOR--;
	if(S_FACTOR<0)S_FACTOR = 0;
	if(S_FACTOR>9)S_FACTOR = 9;

	PXMAX <<= S_FACTOR;
	PYMAX <<= S_FACTOR;
	PXMIN <<= S_FACTOR;
	PYMIN <<= S_FACTOR;

	DEFINE_NORMALFROM(Q, Z);

	XELBOW = NXFROM*__SIZWID__;
	YELBOW = NYFROM*__SIZWID__;

	ZLBLIM = 1;
	ZLVLIM = 1;
	ZPTLIM = 1;
	ZVTLIM = 2;
	ZLDLIM = 0;
	__iEx__= 0;

	J = Open_Shape!=0;

	S_C_F_S = J; /*First Segment*/
	C_Layer = J;
	DEFINE_SEGMENT(J, S-1, S, T);

	NO_CONT_2 = 1;
	if(S_C_F_S)
	{
		if(ZLDLIM)__iEx__ = 1;
		__UNLINTSTD__();
		ZLDLIM = 0;
		if(F_C)
		UNL_TOINTLAYSTANDART(X_C, Y_C);
	}
	C_Layer = 0;
	S_C_F_S = 0; /*Not First Segment*/
	F_C_OLD = 0;

	K = S, S = T++;

	while(T<PLDLIM)
	{
		DEFINE_SEGMENT(1, K, S, T);
		if(F_C)
		{
			NO_CONT_2 = 1;
			UNL_TOINTLAYSTANDART(X_C, Y_C);
		}
		K = S, S = T++;
	}
	__UNLINTSTD__();

	if(J)I = ZLBLIM>3;
	else I = ZLBLIM>2;

	__pAm__ = ZPTLIM;

	if(I)__OVELSEG__();
	else __INI_LvVt__();

	__BSC__();

	if(__bSb__)
	{
		ZVTLIM = 2;
		__bSb__= 0;
		__INI_LvVt__();
	}
	S_FACTOR = 0;
	if(__xSx__)__OVELSEG__();
	__coOut__(__iEx__? 2: 1);
}
#define O_L O_Layer
void __FINISHEDNORMALLY__(void);
struct F_SIZE_JOB
{
	char Name[20];
	long Size;
	char Sign;
}	;
void
F_SIZE_COLLARS(F_SIZE_JOB&JOB)
{
fprintf(stderr, "\n!!! Collars Data Base Generater Started on %s !!!\n",JOB.Name);

	__SIZWID__ = JOB.Size;
	__SISIGN__ = JOB.Sign;
	memcpy(I_Layer[0].Name, JOB.Name, 20);
	ILayer_Cnt= 1;
	MaSCOT_IO.Find_ILayer();
	OutDeltaX = 0;
	OutDeltaY = 0;
	MaSCOT_IO.Make_BDB_File_Name(I_Layer[0].PdbN, I_Layer[0].LX_S.PDBI);
	if(!FindxFile(I_Layer[0].PdbN))
	{
		zAbort(f2size,__LINE__);
	}
	MaSCOT_IO.Init_ILayer_File(I_Layer);
	memset(O_Layer.Name, 0, 20);
	memcpy(O_Layer.Name, MaSCOT_IO.LZ_S.TempP, 3);
fprintf(stderr, "\n!!! Output Layer will be %s !!!\n", O_Layer.Name);
	MaSCOT_IO.INIT_OLayer();

	CFragment = 0;
	XFragment =-1;
	MaSCOT_IO.D_Link(0);
	ZPBLIM = 127;
	CXMIN=+LONG_MAX;
	CYMIN=+LONG_MAX;
	CXMAX=-LONG_MAX;
	CYMAX=-LONG_MAX;
IEvaluater.__SetVT0LB0__();
	for(__READSH__(); !MaSCOT_IO.DFile_Eof; __READSH__())
	{
		if(PLDLIM<2)
		{
			continue;
		}
		Shapes_Collar_Generater();
	}
	ZPBLIM = __ZPBLIM__;
	if(CXMIN==+LONG_MAX)CXMIN = 0;
	if(CYMIN==+LONG_MAX)CYMIN = 0;
	if(CXMAX==-LONG_MAX)CXMAX = 0;
	if(CYMAX==-LONG_MAX)CYMAX = 0;
	MaSCOT_IO.FP_S.Delta_X = MaSCOT_IO.LZ_S.CXMIN;
	MaSCOT_IO.FP_S.Delta_Y = MaSCOT_IO.LZ_S.CYMIN;

	O_L.LX_S.STATUS = 0;
	O_L.LX_S.CXMIN = CXMIN;
	O_L.LX_S.CXMAX = CXMAX;
	O_L.LX_S.CYMIN = CYMIN;
	O_L.LX_S.CYMAX = CYMAX;

	O_L.LX_S.XBCNT = 1;
	O_L.LX_S.YBCNT = 1;
	O_L.LX_S.XBSIZ = CXMAX-CXMIN;
	O_L.LX_S.YBSIZ = CYMAX-CYMIN;

	MaSCOT_IO.SetMinMax();

	MaSCOT_IO.Rewrite_FP_S = 1;
	MaSCOT_IO.Dix_close();
	MaSCOT_IO.DIX_OpenS=0;
	MaSCOT_IO.Rewrite_FP_S = 0;

	MaSCOT_IO.renameoutput();
	MaSCOT_IO.Close_ILayer(0);
	IEvaluater._ReleaseStorage();
	Open_Shape = 0;
__FINISHEDNORMALLY__();
}
