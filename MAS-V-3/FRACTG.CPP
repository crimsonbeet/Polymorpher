#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "fractm.hpp"

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#endif
const char * fractg = "FRACTG";
/*Object Line (Converted and Redirected if needed).*/
/*Direction from Left to Right.*/
#define objlx1 (ObjlObj.x1)
#define objly1 (ObjlObj.y1)
#define objlx2 (ObjlObj.x2)
#define objly2 (ObjlObj.y2)

#define objlx3 (ObjlCow.x1)
#define objly3 (ObjlCow.y1)
#define objlx4 (ObjlCow.x2)
#define objly4 (ObjlCow.y2)

#define zz_min(A,B) ((A)<(B) ?(A): (B))
#define zz_max(A,B) ((A)>(B) ?(A): (B))
#define zz_eqv(A,B,C) ((A)>(B)? ((A)-(B)<=(C)): ((B)-(A)<=(C)))

long
TO_FLS(M_5009_JOB*JOB)
{
	return JOB->FMin;
}
char
IS_FLS(long Delta,M_5009_JOB*JOB)
{
	return Delta>=JOB->FMin;
}
char
IS_033(long D, long d, char X)
{
	char R;
	R = d<(D*(1.0/3.0));
	if(X)R&=d<200;
	return R;
}
char
IS_011(long D, long d, char X)
{
	char R;
	R = d<(D*0.11);
	if(X)R&=d<Max_Flash_Size;
	return R;
}
char
IS_066(long D, long d, char X)
{
	char R;
	R = d<(D*(2.0/3.0));
	if(X)R&=d<200;
	return R;
}
char
IS_050(long D, long d, char X)
{
	char R;
	R = d<(D>>2);
	if(X)R&=d<200;
	return R;
}
char
OBJ_INFLINE_OVER_WINDOW(long W[4]/*Xmin,Xmax,Ymin,Ymax*/)
{
	double A;
	double B;
	double C;
	double X;
	double Y;
	double Z;
	char I;
	char M;
	char K;
#define X1 ObjlObj.x1
#define Y1 ObjlObj.y1
#define X2 ObjlObj.x2
#define Y2 ObjlObj.y2
	A = Y1-Y2;
	B = X2-X1;
	C = (double)X1*Y2  - (double)Y1*X2;
	X = W[0];
	Y = W[2];
	Z = A*X + B*Y + C;
	M = Z>0? 1: Z<0? -1: 0;
	for(I = 1; I<4; ++I)
	{
		if(I<2) X = W[0], Y = W[I+2];
		else X = W[1], Y = W[I];
		Z = A*X + B*Y + C;
		K = Z>0? 1: Z<0? -1: 0;
		if(M*K<0)return 1;
		if(K)M = K;
	}
	return 0;
#undef X1
#undef X2
#undef Y1
#undef Y2
}
char
COWAN_POINT(long X, long Y, long W[4])
{
	char T = 0;
	if(X<W[0])T |= 1;
	if(X>W[1])T |= 2;
	if(Y<W[2])T |= 4;
	if(Y>W[3])T |= 8;
	return T;
}
char
COWAN_SPLIT(long Window[4]/*Xmin,Xmax,Ymin,Ymax*/)
{
	char T1_COD;
	char T2_COD;
	char V;
	char F;
	char I;
	long D;
	double RSLOPE;
	double DSLOPE;
	F = zz_max(objlx1, objlx2)<=Window[0];
	if(!F)
	F = zz_min(objlx1, objlx2)>=Window[1];
	if(!F)
	F = objly2<=Window[2];
	if(!F)
	F = objly1>=Window[3];
	if(F)return cowFULinside=0;

	T1_COD = COWAN_POINT(objlx1, objly1, Window);
	T2_COD = COWAN_POINT(objlx2, objly2, Window);
	cowFULinside = !T1_COD&&!T2_COD;
	if(cowFULinside)
	{
		ObjlCow = ObjlObj;
		return 1;
	}
	if(objlx2-objlx1==0)
	{
		objlx3 = objlx4 = objlx2;
		objly3 = zz_max(objly1, Window[3]);
		objly4 = zz_min(objly2, Window[4]);
		if(objly3>=objly4)zAbort(fractg,__LINE__);
		return 1;
	}
	D = objly2-objly1;
	if(D==0)
	{
		objly3 = objly4 = objly2;
		objlx3 = zz_max(objlx1, Window[0]);
		objlx4 = zz_min(objlx2, Window[1]);
		if(objlx3>=objlx4)zAbort(fractg,__LINE__);
		return 1;
	}
	ObjlCow = ObjlObj;
	V = !T1_COD&&!T2_COD? 1: T1_COD&T2_COD? 0: -1;
	if(V>=0)return V;

	DSLOPE = ((double)D)/((double)objlx2-objlx1);
	RSLOPE = 1.0/DSLOPE;
	for(I = 0; I<4; ++I)
	{
		while((T1_COD&(1<<I))==(T2_COD&(1<<I)))
		{
			++I;
			if(I>=4)break;
		}
		if(I>=4)break;
		if((T1_COD&(1<<I))==0)
		{
			long_swap(objlx3, objlx4);
			long_swap(objly3, objly4);
		}
		if(I<=1)
		{
			objly3 += DSLOPE*(Window[I]-objlx3);
			objlx3 = Window[I];
		}
		else
		{
			objlx3 += RSLOPE*(Window[I]-objly3);
			objly3 = Window[I];
		}
		T1_COD = COWAN_POINT(objlx3, objly3, Window);
		T2_COD = COWAN_POINT(objlx4, objly4, Window);

		V = !T1_COD&&!T2_COD? 1: T1_COD&T2_COD? 0: -1;
		if(V>=0)break;
	}
	return V>0;
}
/*It modifies each flash from Local List of source line.*/
/*Input: Converted Object Line as externaly defined values and*/
/*local list header (S_L_PTR) for source line.*/
/*No any check will be done.*/
/*Output: New Local List.*/
unsigned
MdfLocFlsLis(unsigned S_L_PTR, M_5009_JOB*JOB)
{
	unsigned F; /*Current Flash Index*/
	unsigned Z; /*Current Flash Index*/
	unsigned Pre_F;
	unsigned Aux_F;
	unsigned Nxt_F;

	long X_Flash[4];

#define x_1 (X_Flash[0])
#define x_2 (X_Flash[1])
#define y_2 (X_Flash[3])

	long DY_4_3;
	long DX_4_3;
	long DX_2_1;
	long DX_3_1;
	long DX_2_4;

	char CASE_X;
	char CASE_1;
	char CASE_2;

	char CASE_3;
	char CASE_4;

	char CASE_5;
	char CASE_6;

	char WAVE_D; /*Down*/
	char WAVE_U; /*Up*/
	char WAVE_0;

	char BREAK_LOOP;

	/*Boundary-Box for Object-line.*/
	long X3;
	long X4;
	long Y3;
	long Y4;

	long XL;
	long XR;
	long YL;
	long YR;
	long DXSTEP;
	long DYSTEP;
	long RWM_X2;
	long YR_FIRST;

	long DY_R_3;
	long DX_R_3;

	unsigned RWM_F1;
	unsigned RWM_F2;

	double RSLOPE;
	double DSLOPE;

	double __1Z1__;

	X_Flash[2] = 0;
	Pre_F = 0;
	for(F = S_L_PTR; F; Pre_F = Aux_F, F = Nxt_F)
	{
		x_1 = FLSFX1[F];
		x_2 = FLSFX2[F];
		y_2 = FLSFY2[F];

		Aux_F = F;
		Nxt_F = FLSNXT[F];

		if(y_2<=JOB->FMin)continue;
		if(x_1==x_2)continue;

		if(!COWAN_SPLIT(X_Flash))continue;
		if(objlx3>objlx4)
		{
			long_swap(objlx3, objlx4);
			long_swap(objly3, objly4);
		}
		X3 = zz_min(objlx3, objlx4);
		X4 = zz_max(objlx3, objlx4);
		Y3 = zz_min(objly3, objly4);
		Y4 = zz_max(objly3, objly4);

		DX_4_3 = X4-X3;
		DY_4_3 = Y4-Y3;

		DX_2_1 = x_2-x_1;
		DX_3_1 = X3-x_1;
		DX_2_4 = x_2-X4;

		CASE_1 = 0;
		CASE_2 = 0;

		WAVE_D = 0;
		WAVE_U = 0;
		WAVE_0 = 0;

		if(IS_066(DX_2_1, DX_4_3, 0)) CASE_1 = 1;
		else CASE_2 = 1;

		if(!IS_066(y_2, Y3, 0)||(CASE_2&&Y3==Y4))
		{
			FLSFY2[F] = Y3;
			continue;
		}
		WAVE_0 = CASE_1;

		if(objly4<objly3) WAVE_D = CASE_2;
		else
		if(objly4>objly3) WAVE_U = CASE_2;
		else WAVE_0 = 1;

		/*Start of Steps Passing*/

		/*INPUT:*/
		RWM_F1 = 0;
		RWM_F2 = 0;

		CASE_X = WAVE_0;
		if(!CASE_X)
		{
			CASE_X = (!IS_FLS(DY_4_3,JOB))||(!IS_FLS(DX_4_3,JOB));
		}
		if(CASE_X)goto END_OF_STEPS_PASSING;
		/*OUTPUT:*/
		/*RWM_F1, RWM_F2*/
		/*RWM_X2, RWM_X2*/

		/*FROM LEFT UPPER POINT*/
		XR = WAVE_D? X3: X4;
		YR = Y4;

		RSLOPE = ((double)DX_4_3)/((double)DY_4_3);
		DSLOPE = ((double)DY_4_3)/((double)DX_4_3);
		__1Z1__ = RSLOPE*tan(atan(DSLOPE)/2);

		DX_R_3 = 0;
		do
		{
			if(RWM_F1)
			YR = floor(Y4-(DX_R_3)*DSLOPE+0.5);

			XL = XR;
			YL = YR;

			DYSTEP = floor(YL-YL*__1Z1__/*+0.5*/);

			DXSTEP = DYSTEP*RSLOPE;
			if(!IS_FLS(DXSTEP,JOB))
			{
				DXSTEP = TO_FLS(JOB);
				DYSTEP = ceil(DXSTEP*DSLOPE);
			}
			YR = YL-DYSTEP;
			XR = XL+(WAVE_D? DXSTEP: -DXSTEP);

			DY_R_3 = YR-Y3;
			DX_R_3 += DXSTEP;

			BREAK_LOOP = 0;

			BREAK_LOOP |= !IS_FLS(DX_4_3-DX_R_3,JOB);
			BREAK_LOOP |= !IS_FLS(DY_R_3,JOB);

			if(BREAK_LOOP)
			{
				RWM_X2 = XL;
				break;
			}
			Z = __popFls__();

			FLSFX1[Z] = WAVE_U? XR: XL;
			FLSFX2[Z] = WAVE_D? XR: XL;
			FLSFY2[Z] = YR;

			if(!RWM_F1)
			RWM_F1 = RWM_F2 = Z, FLSNXT[Z] = 0, YR_FIRST=YR;
			else
			if(WAVE_U)
			FLSNXT[Z] = RWM_F2;
			else
			FLSNXT[RWM_F2] = Z;

			RWM_F2 = Z;
		}
		while(1);

		if(WAVE_U){unsigned Z=RWM_F1; RWM_F1=RWM_F2; RWM_F2=Z;}

		END_OF_STEPS_PASSING:

		CASE_3 = 0;
		CASE_4 = 0;

		CASE_5 = DX_3_1!=0;
		CASE_6 = DX_2_4!=0;

		if(!WAVE_0&&RWM_F1==0)
		/*No any Steps done on CASE_2*/
		/*Note: Changing of WAVE_U, WAVE_D, WAVE_0.*/
		WAVE_U = WAVE_D = 0, WAVE_0 = 1;

		if(!IS_011(DX_2_1, DX_3_1, 0)&&IS_FLS(DX_3_1,JOB)) CASE_3 = 1;
		else
		CASE_3 = WAVE_U;

		if(!IS_011(DX_2_1, DX_2_4, 0)&&IS_FLS(DX_2_4,JOB)) CASE_4 = 1;
		else
		CASE_4 = WAVE_D;

		/*Note: Changing of CASE_5 and CASE_6.*/

		CASE_5 &= !CASE_3&&WAVE_D;
		CASE_6 &= !CASE_4&&WAVE_U;

		if(CASE_3)
		{
			Aux_F = __popFls__();

			FLSNXT[Aux_F] = F;
			if(Pre_F)
			FLSNXT[Pre_F] = Aux_F;
			else
			S_L_PTR = Aux_F;

			Pre_F = Aux_F;
			FLSFX1[Aux_F] = x_1;
			FLSFX2[Aux_F] = WAVE_U? RWM_X2: X3;
			FLSFY2[Aux_F] = CASE_1? y_2: objly3;
		}
		else
		if(CASE_5) FLSFX1[RWM_F1] = x_1;

		if(CASE_4)
		{
			Nxt_F = __popFls__();

			FLSNXT[Nxt_F] = FLSNXT[F];
			FLSNXT[F] = Nxt_F;

			FLSFX1[Nxt_F] = WAVE_D? RWM_X2: X4;
			FLSFX2[Nxt_F] = x_2;
			FLSFY2[Nxt_F] = CASE_1? y_2: objly4;
		}
		else
		if(CASE_6) FLSFX2[RWM_F2] = x_2;

		if(CASE_1)
		{
			if(IS_FLS(Y4-1,JOB)&&!IS_FLS(Y3,JOB))CASE_2 = 1; /*Will be passed on this Flash*/
			else
			if(!IS_033(DX_2_1, DX_4_3, 0))CASE_2 = 1;

			FLSFY2[F] = CASE_2? Y4: Y3;
			FLSFX1[F] = CASE_3? X3: x_1;
			FLSFX2[F] = CASE_4? X4: x_2;

			if(CASE_2)Aux_F = Pre_F, Nxt_F = F;
			else
			if(CASE_4)Aux_F = Nxt_F, Nxt_F = FLSNXT[Nxt_F];
			continue;
		}
		if(!CASE_2)zAbort(fractg,__LINE__);

		if(WAVE_0) /*No Any Steps was done*/
		{
			FLSFY2[F] = Y3;
			FLSFX1[F] = CASE_3? X3: x_1;
			FLSFX2[F] = CASE_4? X4: x_2;

			if(CASE_4)Aux_F = Nxt_F, Nxt_F = FLSNXT[Nxt_F];
			continue;
		}
		if(Pre_F)FLSNXT[Pre_F] = RWM_F1;
		else
		S_L_PTR = RWM_F1;

		FLSNXT[RWM_F2] = Nxt_F;

		Aux_F = RWM_F2;
		__pushFls__(F);
		continue;
	}
	return S_L_PTR;
#undef x_1
#undef x_2
#undef y_2
}
