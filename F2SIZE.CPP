#define F_SIZE_MODULE

#include "stdafx.h"

#pragma warning(disable : 26451)


/**/
#include<DOS.H>
#include<MATH.H>
#include<STDIO.H>
/**/
#include<STDLIB.H>
#include<CTYPE.H>
#include<STRING.H>
/**/
#include<LIMITS.H>
#include<PROCESS.H>
/**/
#include "fqeval.hpp"
#include "fxeval.hpp"
#include "scale.hpp"
extern void zabort(void);
extern void PrintOk(void);
const char*f2size="f2size";
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
char F_C_Prev;


char __iEx__;



extern long Delta_X;
extern long Delta_Y;
extern long Delta_X_2;
extern long Delta_Y_1;
extern long Delta_Y_2;

void
__PsPLINE__(LbBead* LB, double angleVariance)
{
	LvBead* LE;
	LvBead* LV;
	LvBead* LN;

	double A1;
	double A2;

	/*Only After Force_C_Polygonc and*/
	/*Dummyjunctionanalysis and S_FACTOR!=0.*/

	LE = LB->ZLBFirstLvBead;
	LN = LE->ZLVNextLvBead;
	LV = LE;
	A1 = COMPLVANGLE(LV);
	do
	{
		A2 = COMPLVANGLE(LN);
		if (_EQ_(A1, A2, angleVariance))
		{
			REMOVE_FROM_VT_CHAIN(LN);
			REMOVE_FROM_VT_CHAIN(LV);
			LN->ZLVMyLbBead = __LB0__;
			LN->ZLVMyVector = __VT0__;
			LV->ZLVMyVector = __VT0__;
			LB->ZLBFirstLvBead = LV;
			LV->ZLVPoint2 = LN->ZLVPoint2;
			LV->ZLVNextLvBead = LN->ZLVNextLvBead;
			A1 = COMPLVANGLE(LV);
			if (LN == LE)
			{
				return;
			}
		}
		else
		{
			LV = LN; A1 = A2;
		}
		LN = LV->ZLVNextLvBead;
	} while (LV != LE);
}



void
__INI_LvVt__(void)
{
	long LB;
	for (LB = 1; LB < ZLBLIM; ++LB)
	{
		LbBead* Lb = GetLbBead(LB);
		LvBead* Lv = Lb->ZLBFirstLvBead;
		LvBead* Le = Lv;
		if (Lv)
			do
			{
				Lv->ZLVMyVector = __VT0__;
				Lv = Lv->ZLVNextLvBead;
			} while (Lv && Lv != Le);
	}
	__VT0__ = GetVtBead(0);
	VtBead* __VT1__ = GetVtBead(1);
	memset(__VT0__, 0, sizeof(VtBead));
	memset(__VT1__, 0, sizeof(VtBead));
	__VT0__->ZVTMeInreverse = __VT1__;
	__VT1__->ZVTMeInreverse = __VT0__;
	memset(__LB0__, 0, sizeof(LbBead));
}




long UnloadLbVectors(std::vector<std::vector<long>> &Xs, std::vector<std::vector<long>> &Ys, bool run_linearizer = 0) {
	long originalLbCount = 0;
	for (long i = __iEx__ ? 2 : 1; i < ZLBLIM; ++i) {
		LbBead* Lb = GetLbBead(i);
		LvBead* LV = Lb->ZLBFirstLvBead;
		if (LV != 0) {
			LvBead* LE;
			LvBead* LN;

			LN = LV->ZLVNextLvBead;
			LE = LV;

			ZLDLIM = 0;
			do
			{
				long LD = ZLDLIM++;

				ZLDCO0[LD] = LV->ZLVPoint2->ZPTCO1;
				ZLDCO1[LD] = LV->ZLVPoint2->ZPTCO2;

				LV = LN;
				LN = LV->ZLVNextLvBead;

			} while (LV != LE);

			std::vector<long>& x = Xs[originalLbCount];
			std::vector<long>& y = Ys[originalLbCount];

			++originalLbCount;

			x.resize(ZLDLIM);
			y.resize(ZLDLIM);
			for (long j = 0; j < ZLDLIM; ++j) {
				x[j] = ZLDCO0[j];
				y[j] = ZLDCO1[j];
			}
			if (run_linearizer) {
				size_t stepSize = S_FACTOR > 1 ? (((size_t)1) << (S_FACTOR - 1)) : 1; 
				linearizeContour(x, y, stepSize, 5);
			}
		}
	}
	return originalLbCount; 
}

void LoadLbVectors(std::vector<std::vector<long>>& Xs, std::vector<std::vector<long>>& Ys, long originalLbCount, int log_graph = 0) {
	__INI_LvVt__();

	CleanZPTHASH();

	ZLBLIM = 1;
	ZLVLIM = 1;
	ZPTLIM = 1;
	ZVTLIM = 2;
	ZLDLIM = 0;

	for (long i = 0; i < originalLbCount; ++i) {
		std::vector<long>& x = Xs[i];
		std::vector<long>& y = Ys[i];
		ZLDLIM = x.size();
		for (long j = 0; j < ZLDLIM; ++j) {
			ZLDCO0[j] = x[j];
			ZLDCO1[j] = y[j];
		}
		if (log_graph) {
			graph_log_ZLDLIM("LoadmasterPhaseX", 0, 0);
		}
		STATUS = 1;
		char check_closure = 1;
		Loadmaster(check_closure);
	}
}



char __GrpNONE__;


void
__OVELSEG__(void) { 
	bool done = false;

	std::vector<std::vector<long>> Xs(ZLBLIM);
	std::vector<std::vector<long>> Ys(ZLBLIM);

	long originalLbCount = 0;

	int c = 100;
	while (!done && (c--)>=0) {
		long prevVTcount = ZVTLIM; 
		if (c < 99) {
			LoadLbVectors(Xs, Ys, originalLbCount); 
		}

		INTERSECTION_ANALYSIS();

		// here: unload all polygons to vectors; reload vectors on next iteration.

		originalLbCount = UnloadLbVectors(Xs, Ys, c < 99);

		POINT_VECTOR_ANALYSIS(2);

		for (long I = 1; I < ZLBLIM; ++I) {
			LbBead* Lb = GetLbBead(I);
			if (Lb->ZLBFirstLvBead)
				DUMMYJUNCTIONANALYSIS(Lb);
		}

		long K = 1;
		if (__iEx__) {
			K = 2;
			True_Stat = AND_NOT;
			__GrpNONE__ = 1;
			POINT_VECTOR_CLASS_2(GetLbBead(1), 8, 8);
			__GrpNONE__ = 0;
		}

		for (long I = K; I < ZLBLIM; ++I) {
			LbBead* Lb = GetLbBead(I);

			if (Lb->ZLBFirstLvBead) {
				Set_Rstat(Lb, 1);
			}
		}

		done = FORCE_C_POLYGONS();

		if (prevVTcount == ZVTLIM) {
			done = true; 
		}
	}


	long K = 1;
	if (!__iEx__ && !S_FACTOR) {
		K = ZLBLIM;
	}


	for (long I = K; I < ZLBLIM; ++I) {
		LbBead* Lb = GetLbBead(I);
		if (Lb->ZLBFirstLvBead) {
			DUMMYJUNCTIONANALYSIS(Lb);
		}
	}

	if (__iEx__) {
		for (long I = 1; I < ZLBLIM; ++I)
		{
			LbBead* Lb = GetLbBead(I);
			if (Lb->ZLBFirstLvBead) 
			{
				__PsPLINE__(Lb, 0.1);
			}
		}
	}
	__iEx__ = 0;

	True_Stat = O_R;
}





char Do_Open_Shapes; 



void
__READSH__(void)
{
	Do_Open_Shapes = MaSCOT_IO.I_READ_P(1,1);
	if(ZLDLIM) {
		if(!Do_Open_Shapes)
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
	}
	PLDLIM = ZLDLIM+1;
}




double
DEFINE_CSLOPE(long K, long I, long J)
{
	long X;
	long Y;
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
	long DX;
	long DY;
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
	if(_EQ_(DL, 0, 1E-7))
		return(0);

	X[0] = (B1*C2-B2*C1)/DL;
	Y[0] = (A2*C1-A1*C2)/DL;

	if(X[0]>LONG_MAX||Y[0]>LONG_MAX)
		return(0);
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
__UNLINTSTD__(std::ostringstream *ostr = 0)
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
	if (ostr) {
		long offsX = Delta_X_2 << S_FACTOR;
		long offsY = Delta_Y_2 << S_FACTOR;
		long cnt = 0;
		J = 0;
		while (J < ZLDLIM) {
			if (cnt++ > 0) {
				*ostr << ',';
			}
			*ostr << (offsX + (long)ZLDCO0[J]) << ',' << (offsY + (long)ZLDCO1[J]);
			++J;
		}
		*ostr << std::endl;
	}
	STATUS = 1;
	char check_closure = 1;
	Loadmaster(check_closure);
}
void
UNL_TOINTLAYSTANDART(char NO_CONT_2, double*X, double*Y, std::ostringstream* ostr = 0)
{
	long J;
	long I;
	long L;
	long s;
	_sAlong x;
	_sAlong y;
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
	J = DETERMINE_CLOCKSTATUS(x, y, L);
	if(J==0) {
		//for (long j = 0; j < L; ++j) {
		//	printf("x=%ld, y=%ld\r\n", (long)x[j], (long)y[j]);
		//}
		long x2 = x[2];
		long y2 = y[2];
		x[2] = x[3];
		y[2] = y[3];
		x[3] = x2;
		y[3] = y2;
		J = DETERMINE_CLOCKSTATUS(x, y, L);
		if (J == 0) {
			DEFINE_INTERSECTION(X + 5, Y + 5, X + 7, Y + 7, X, Y);
			x[2] = floor(X[0] * s + 0.5);
			y[2] = floor(Y[0] * s + 0.5);
			x[3] = x[4];
			y[3] = y[4];
			L = 4;
			J = DETERMINE_CLOCKSTATUS(x, y, L);
		}
		if (J == 0) {
			printf("rejected segment\r\n\r\n");
			return;
		}
		printf("fixed segment\r\n\r\n");
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
		if (!NO_CONT_2) {
			if((*X0)[I]!=x[0])NO_CONT_2 =1;
			else
			if((*X1)[I]!=x[3])NO_CONT_2 =1;
			else
			if((*Y0)[I]!=y[0])NO_CONT_2 =1;
			else
			if((*Y1)[I]!=y[3])NO_CONT_2 =1;
		}
		if(NO_CONT_2)
		{
			__UNLINTSTD__(ostr);
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
DEFINE_SEGMENT(char isNotFirstSegment, long J, long I, long K, std::ostringstream *ostr = 0)
{
	/*[0]-FIRST-DISPLACE-POINT!*/
	/*[1]-SECOND-DISP-POINT!*/
	/*[2]-NORMAL*/

	double IX = 0, IY = 0;
	double CAN = 0, MAN = 0;

	double X1[9] = { 0 }, X2[3] = { 0 }, Y1[9] = { 0 }, Y2[3] = { 0 };

	static double CAN_Prev = 0;
	long CANLess180 = 0;
	long SPT = 0; /*spline type*/
	static long CANLess180Prev = 0;

	//long SW1 = 0, SW2 = 0, SW3 = 0;


	auto swap_X1Y1 = [&X1, &Y1]() {
		d_swap(X1 + 4, X1 + 7);
		d_swap(Y1 + 4, Y1 + 7);
		d_swap(X1 + 5, X1 + 6);
		d_swap(Y1 + 5, Y1 + 6);
	};

	if (!isNotFirstSegment) {
		CAN_Prev = 0;
	}

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

		CANLess180 = 1;
	}
	else 
		MAN = CAN, CANLess180 = 0;

	F_C = 0;

	DEFINE_DISPLACE(J, I, X1, Y1);
	DEFINE_DISPLACE(I, K, X2, Y2);

	SPT = SPLINE_TYPE(MAN);

	if(SPT>0)
	{
		if (!DEFINE_INTERSECTION(X1, Y1, X2, Y2, &IX, &IY))
			SPT = 0;
		else
			IX -= ZLDCO4[I], IY -= ZLDCO5[I];
	}
	X1[4] = XELBOW +ZLDCO4[J];
	Y1[4] = YELBOW +ZLDCO5[J];
	X1[7] = ZLDCO4[J] -XELBOW;
	Y1[7] = ZLDCO5[J] -YELBOW;
	X1[5] = X1[1];
	Y1[5] = Y1[1];
	long Xi = ZLDCO4[I];
	long Yi = ZLDCO5[I];
	X1[6] = ((size_t)Xi<<1)-X1[1];
	Y1[6] = ((size_t)Yi<<1)-Y1[1];
	switch(SPT)
	{
		case 0: /*GORIZONTAL!NEEDED-BOX-GENERATING*/
		XELBOW = X1[2]*__SIZWID__;
		YELBOW = Y1[2]*__SIZWID__;
		if (CANLess180)
			XELBOW = -XELBOW, YELBOW = -YELBOW;
		if(CANLess180Prev)
		{
			swap_X1Y1();
		}
		break;
		case 1: /*NOT-ACUTE-ANGLE*/
		XELBOW = IX;
		YELBOW = IY;
		DEFINE_ELBOW(X1, Y1, I);
		if (CANLess180)
			XELBOW = -XELBOW, YELBOW = -YELBOW;
		if (CANLess180Prev && !CANLess180)
			CAN_Prev -= 360;
		if (!CANLess180Prev && CANLess180)
			CAN_Prev *= -1.0;

		if(F_C_Prev == 0)
			if ((MAN + CAN_Prev) < 0)
			{
				swap_X1Y1();
			}
		break;
		case 2: /*ACUTE-ANGLE*/
		case 3:
		/*Mirror Normals*/
		X1[2] =-X1[2];
		Y1[2] =-Y1[2];
		X2[2] =-X2[2];
		Y2[2] =-Y2[2];

		/*Inside Edges*/
		DEFINE_DISPLACE(J, I, X1, Y1);
		DEFINE_DISPLACE(I, K, X2, Y2);

		/*Check For Cover Make up*/
		if(!DEFINE_INTERSECTION(X1, Y1, X2, Y2, X1 + 3, Y1 + 3))
			zabort(f2size,__LINE__);

		//SW1 = POINTONSEGMENT(X1, Y1, X1[3], Y1[3]);
		//SW2 = POINTONSEGMENT(X2, Y2, X1[3], Y1[3]);
		//if(SPT == 3 && SW1&&SW2)
		//{
		//	XELBOW = IX;
		//	YELBOW = IY;
		//	DEFINE_ELBOW(X1, Y1, I);
		//	if (CANLess180)
		//		XELBOW = -XELBOW, YELBOW = -YELBOW;

		//	if(CANLess180Prev&&!CANLess180)CAN_Prev -= 360;
		//	if(!CANLess180Prev&& CANLess180)CAN_Prev *=-1.0;

		//	if (F_C_Prev == 0)
		//		if ((MAN + CAN_Prev) < 0)
		//		{
		//			swap_X1Y1();
		//		}
		//	break;
		//}

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

		if (CANLess180)
			XELBOW = -XELBOW, YELBOW = -YELBOW;

		if(CANLess180Prev!= CANLess180)
		{
			swap_X1Y1();
		}
		F_C = 1;
		break;
		default: zabort(f2size,__LINE__);
	}
	X1[8] = X1[4];
	Y1[8] = Y1[4];

	if (J > 0) {
		UNL_TOINTLAYSTANDART(F_C, X1, Y1, ostr);
		if (F_C && isNotFirstSegment) {
			UNL_TOINTLAYSTANDART(1, X_C, Y_C, ostr);
		}
	}

	CANLess180Prev = CANLess180;

	F_C_Prev = F_C;
	CAN_Prev = CAN;
}
void
Shapes_Collar_Generator(int log_graph = 0)
{
	long S;
	long T;
	long Q;

	if(PLDLIM<2)
	{
		return;
	}
	Delta_X_2 = PXMIN -= __SIZWID__;
	Delta_Y_2 = PYMIN -= __SIZWID__;
	PXMAX += __SIZWID__;
	PYMAX += __SIZWID__;

	for(long j = 1; j<PLDLIM; ++j)
	{
		ZLDCO4[j] -= PXMIN;
		ZLDCO5[j] -= PYMIN;
	}
	PXMAX -= PXMIN;
	PYMAX -= PYMIN;

	PXMIN = PYMIN = 0;
	CleanZPTHASH();

	long isNotFirstSegment = 0;

	if(Do_Open_Shapes)
	{
		Q = 1;
		S = 2;
		T = 3;
		isNotFirstSegment = 1;
	}
	else
	{
		/*LAST POINT .EQ. FIRST POINT ALWAYS.*/
		/*DUPLICATE POINTS NEVER TAKE PLACE.*/
		Q = 0;
		S = 1;
		T = 2;
		isNotFirstSegment = 0;

		long j = PLDLIM - 2;
		ZLDCO4[Q] = ZLDCO4[j];
		ZLDCO5[Q] = ZLDCO5[j];
		j = PLDLIM++;
		ZLDCO4[j] = ZLDCO4[T];
		ZLDCO5[j] = ZLDCO5[T];
	}
	/*SCALE POINTS*/
	long maxJ = PYMAX>PXMAX? PYMAX: PXMAX;
	S_FACTOR = 0;
	do
	{
		++S_FACTOR;
	}
	while(((maxJ<<S_FACTOR)>>S_FACTOR)== maxJ);
	S_FACTOR--;
	if(S_FACTOR<0)S_FACTOR = 0;
	if(S_FACTOR>9)S_FACTOR = 9;

	PXMAX <<= S_FACTOR;
	PYMAX <<= S_FACTOR;
	PXMIN <<= S_FACTOR;
	PYMIN <<= S_FACTOR;

	DEFINE_NORMALFROM(Q, S);

	XELBOW = NXFROM*__SIZWID__;
	YELBOW = NYFROM*__SIZWID__;

	ZLBLIM = 1;
	ZLVLIM = 1;
	ZPTLIM = 1;
	ZVTLIM = 2;
	ZLDLIM = 0;

	__iEx__= 0;

	C_Layer = Do_Open_Shapes? 1: 0;

	std::ostringstream ostr; 
	std::ostringstream *pOstr = &ostr;
	if (!log_graph) {
		pOstr = 0; 
	}

	DEFINE_SEGMENT(0, Q, S, T, pOstr);

	if(Do_Open_Shapes) {
		if(ZLDLIM)
			__iEx__ = 1;

		__UNLINTSTD__(pOstr);

		ZLDLIM = 0;
		if (F_C) {
			UNL_TOINTLAYSTANDART(1, X_C, Y_C, pOstr);
		}
	}

	C_Layer = 0;

	F_C_Prev = 0;
	while(T<PLDLIM) {
		Q = S; 
		S = T++; 

		DEFINE_SEGMENT(1, Q, S, T, pOstr);
	}

	__UNLINTSTD__(pOstr);

	if (pOstr) {
		WriteStream2file(pOstr, "Shapes");
	}
	ostr = std::ostringstream();


	std::vector<std::vector<long>> Xs(ZLBLIM);
	std::vector<std::vector<long>> Ys(ZLBLIM);
	long count = 0;


	if (Do_Open_Shapes ? (ZLBLIM > 3) : (ZLBLIM > 2)) {

		__OVELSEG__();

		for (long i = __iEx__ ? 2 : 1; i < ZLBLIM; ++i) {
			LbBead* Lb = GetLbBead(i);
			if (Lb->ZLBFirstLvBead != 0 && LbBead_Unload2InputXY(Lb, 0, log_graph)) {
				std::vector<long> &x = Xs[count];
				std::vector<long> &y = Ys[count];
				++count; 
				x.resize(ZLDLIM);
				y.resize(ZLDLIM);
				for (long j = 0; j < ZLDLIM; ++j) {
					x[j] = ZLDCO0[j];
					y[j] = ZLDCO1[j];
				}
				if (S_FACTOR > 0) {
					linearizeContour(x, y, static_cast<size_t>(1) << (S_FACTOR), 5);
				}
			}
		}

		LoadLbVectors(Xs, Ys, count, log_graph); 

		__OVELSEG__();
	}
	else {
		__INI_LvVt__();
	}

	if (log_graph) {
		graph_log_polygons("ShapesCombined", Delta_X_2 << S_FACTOR, Delta_Y_2 << S_FACTOR);
	}
		

	Back_ScalePoints();

	S_FACTOR = 0;

	count = 0;
	for (long j = 0; j < 2 && count == 0; ++j) {
		for (long i = 1; i < ZLBLIM; ++i) {
			LbBead* Lb = GetLbBead(i);
			if (Lb->ZLBFirstLvBead != 0 && LbBead_Unload2InputXY(Lb, 0, log_graph)) {
				++Total_Shapes;
				long offsX = Delta_X_2 << S_FACTOR;
				long offsY = Delta_Y_2 << S_FACTOR;
				for (long j = 0; j < ZLDLIM; ++j) {
					ZLDCO0[j] += offsX;
					ZLDCO1[j] += offsY;
				}
				char do_change_chip_dimensions = 1;
				MaSCOT_IO.Dix_output(do_change_chip_dimensions, 0, Do_Open_Shapes);
				++count; 
			}
		}
		if (count == 0) {
			count = UnloadLbVectors(Xs, Ys, true);
			LoadLbVectors(Xs, Ys, count);
			count = 0; 

			__OVELSEG__();
		}
	}
}



#define O_L MaSCOT_IO.O_Layer


void FinishedNormally(void);


void
F_SIZE_COLLARS(F_SIZE_JOB&JOB) {
	//fprintf(stderr, "\n!!! Collars Data Base Generater Started on %s !!!\n",JOB.Name);

	__SIZWID__ = JOB.Size;
	__SISIGN__ = JOB.Sign;

	memcpy(MaSCOT_IO.I_Layer[0].Name, JOB.Name, 20);
	MaSCOT_IO.ILayer_Cnt = 1;
	MaSCOT_IO.Activate_ILayers();

	OutDeltaX = 0;
	OutDeltaY = 0;

	MaSCOT_IO.INIT_ILayer(0);


	memset(MaSCOT_IO.O_Layer.Name, 0, 20);
	memcpy(MaSCOT_IO.O_Layer.Name, MaSCOT_IO._header.TempP, 3);

	fprintf(stderr, "!!! Output Layer will be %s !!!\n", MaSCOT_IO.O_Layer.Name);
	MaSCOT_IO.INIT_OLayer();


	C_Fragment = 0;
	MaSCOT_IO.O_Layer._lIdentity.lastWrittenFragment =-1;
	MaSCOT_IO.D_Link(0);


	ZPBLIM = 127;

	CXMIN=+LONG_MAX;
	CYMIN=+LONG_MAX;
	CXMAX=-LONG_MAX;
	CYMAX=-LONG_MAX;
	IEvaluater.__SetVT0LB0__();

	Total_Shapes = 0; 

	for(__READSH__(); !feof(MaSCOT_IO.I_Layer[0].File); __READSH__()) {
		if(PLDLIM<2) {
			continue;
		}
		Shapes_Collar_Generator(JOB.Do_log_graph);
	}

	ZPBLIM = 8191;


	if(CXMIN==+LONG_MAX)CXMIN = 0;
	if(CYMIN==+LONG_MAX)CYMIN = 0;
	if(CXMAX==-LONG_MAX)CXMAX = 0;
	if(CYMAX==-LONG_MAX)CYMAX = 0;

	MaSCOT_IO.SetMinMax();

	MaSCOT_IO.O_Layer._lIdentity.layer_STATUS = Total_Shapes <= 1;

	MaSCOT_IO.O_Layer._lIdentity.CXMIN = CXMIN;
	MaSCOT_IO.O_Layer._lIdentity.CXMAX = CXMAX;
	MaSCOT_IO.O_Layer._lIdentity.CYMIN = CYMIN;
	MaSCOT_IO.O_Layer._lIdentity.CYMAX = CYMAX;

	MaSCOT_IO.O_Layer._lIdentity.XBCNT = 1;
	MaSCOT_IO.O_Layer._lIdentity.YBCNT = 1;
	MaSCOT_IO.O_Layer._lIdentity.XBSIZ = CXMAX-CXMIN;
	MaSCOT_IO.O_Layer._lIdentity.YBSIZ = CYMAX-CYMIN;


	MaSCOT_IO._fileIdentity.Delta_X = 0; // MaSCOT_IO._header.CXMIN;
	MaSCOT_IO._fileIdentity.Delta_Y = 0; // MaSCOT_IO._header.CYMIN;

	char do_save_identity = 1;
	MaSCOT_IO.Dix_close(do_save_identity);


	Do_Open_Shapes = 0;

	MaSCOT_IO.renameoutput();
	MaSCOT_IO.Close_ILayer(0);

	IEvaluater._ReleaseStorage();

	FinishedNormally();
}
