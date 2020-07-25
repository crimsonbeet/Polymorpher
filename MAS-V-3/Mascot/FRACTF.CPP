#include "stdafx.h" 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tom_aray.hpp"
#include "fqeval.hpp"
#include "fxeval.hpp"
#include "mgsort.hpp"
#include "m_alcr.hpp"
#include "dxbout.hpp"
#include "fzinit.hpp"
void zAbort(char*ids, long lin);
char*fractf="fractf";
char cowFULinside; /*indicator of obj-line is fully inside window.*/
/*Work ObjLine.*/
Obj_Line ObjlObj;
/*Splitted Object Line*/
Obj_Line ObjlCow;
Obj_Line*ObjlRot;
unsigned ObjlMax;
unsigned ObjlLim;

FILE*Pat_All_Angle;
FILE*Pat_ORtogonal;

/*For each source line We will convert coordinate system into*/
/*a one in wich the source line is parallel to the x axis.*/
/*Inside of the source line always points up.*/
/*After then origin of coor.sys. are resided to left point of*/
/*source line.*/
/*Thus Flash is a Rotated Box with bottom on the source line*/
/*and x-y-values are related to left point of source line.*/
/*And each object line must be converted into source-*/
/*coordinate system.*/
double Angle_R;
double SLRM_COS;
double SLRM_SIN;
double sine;
double cosine;
long Obj_Delta_X;
long Obj_Delta_Y;
/*Thus for each source line rotation matrix must present.*/
/*Flash-Boxes*/
struct FLashb
{
	long x1;
	long x2;
	long y1;
	long y2;
	char st;
	FLashb*Nxt;
}       ;
class _zbFlsh: public _rCash
{
public:
	_zbFlsh():_rCash(4096/sizeof(FLashb), 64){};
	~_zbFlsh(){};
	FLashb& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 4096/sizeof(FLashb));
		}
		return *((FLashb*)aHot+(i-lo));
	}
}       ;
class FLash_Buffer
{
	_zbFlsh B;

	FLashb*STak;
	long Lim;
	public:
	void push(FLashb*f);
	FLashb*pop(void);
	FLashb*__popFls__(void);
	void __pushFls__(FLashb*f){push(f);}
	FLash_Buffer(){STak=0;Lim=0;}
}       ;
void FLash_Buffer::push(FLashb*f)
{
	f->Nxt = STak;
	STak = f;
}
FLashb*FLash_Buffer::pop(void)
{
	FLashb*z=STak;
	if(z)
	{
		STak = STak->Nxt;
	}
	return z;
}
FLashb*FLash_Buffer::__popFls__(void)
{
	FLashb*z=pop();
	if(!z)
	{
		z=&B[Lim++];
	}
	z->st = 0;
	return z;
}
void
__iniFls__(void)
{
	ObjlRot = (Obj_Line*)__malloc__(256*sizeof(Obj_Line));
	ObjlMax = 256;
	chnl = (long*)__malloc__(256*sizeof(long));
	cmax = 256;
}
void
__delFls__(void)
{
	__free__(chnl);
	__free__(ObjlRot);
}
struct M_5009_JOB
{
	char Name[20];
	long FMin;
	long FMax;
	long HMax;
	long Strip;
	char Name_1[20];
	char Name_2[20];
	char Name_3[20];
}       ;
struct OTELLO_JOB
{
	char Name[20];
	long FSiz;
	long STep;
	char Name_1[20];
	char Name_2[20];
	char Name_3[20];
}       ;
struct Buf_Obj_Line
{
	Obj_Line Obj;
	Buf_Obj_Line*Nxt;
}       ;
class _zbObjl: public _rCash
{
public:
	_zbObjl():_rCash(4096/sizeof(Buf_Obj_Line), 128){};
	~_zbObjl(){};
	Buf_Obj_Line& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 4096/sizeof(Buf_Obj_Line));
		}
		return *((Buf_Obj_Line*)aHot+(i-lo));
	}
}       ;
struct Object_Buffer
{
	Buf_Obj_Line*CSrc;
	Buf_Obj_Line*LSrc;
	Buf_Obj_Line*CObj;
	Buf_Obj_Line*LObj;
	Buf_Obj_Line*FObj;
	Buf_Obj_Line*STak;
	long BLim;
	char BEof;
	char*Pat_Name;

	_zbObjl __B__;
	Buf_Obj_Line*pop(void);
	void push(Buf_Obj_Line*Obj);
	~Object_Buffer();
	Object_Buffer(M_5009_JOB&JOB);
}       ;
void
READ_OBJ_LINE(Object_Buffer*B)
{
	if(B->BEof)
	{
		B->LObj->Nxt = B->LObj;
		return;
	}
	Buf_Obj_Line*z=B->pop();
	if(!z)
	{
		z=&B->__B__[B->BLim++];
	}
	if(!MaSCOT_IO.ZDREAD_L(z->Obj))
	{
		B->LSrc = z;
		B->BEof = 1;
	}
	z->Nxt = 0;
	if(B->LObj)
	{
		B->LObj->Nxt = z;
	}
	else
	{
		B->FObj = z;
		B->CObj = z;
		B->CSrc = z;
	}
	B->LObj = z;
}
Obj_Line*
SETNEXT_SRC_LINE(Object_Buffer*B)
{
	if(!B->CSrc->Nxt)
	{
		READ_OBJ_LINE(B);
	}
	B->CSrc = B->CSrc->Nxt;
	return &B->CSrc->Obj;
}
Obj_Line*
SETNEXT_OBJ_LINE(Object_Buffer*B)
{
	if(!B->CObj->Nxt)
	{
		READ_OBJ_LINE(B);
	}
	B->CObj = B->CObj->Nxt;
	return &B->CObj->Obj;
}
Buf_Obj_Line*Object_Buffer::pop(void)
{
	Buf_Obj_Line*z=STak;
	if(z)
	{
		STak = STak->Nxt;
	}
	return z;
}
void Object_Buffer::push(Buf_Obj_Line*Obj)
{
	Obj->Nxt = STak;
	STak = Obj;
}
Object_Buffer::Object_Buffer(M_5009_JOB&JOB)
{
	memcpy(I_Layer[0].Name, JOB.Name, 20);
	ILayer_Cnt= 1;
	MaSCOT_IO.Find_ILayer();
	OutDeltaX = 0;
	OutDeltaY = 0;
	MaSCOT_IO.Make_PLT_File_Name(I_Layer[0].PdbN, I_Layer[0].LX_S.PDBI);
	if(!FindxFile(I_Layer[0].PdbN))
	{
		zAbort(fractf,__LINE__);
	}
	MaSCOT_IO.Init_ILayer_File(I_Layer);
	memset(O_Layer.BdbN, 0, 20);
	memcpy(O_Layer.BdbN, I_Layer[0].PdbN, 15);
	MaSCOT_IO.INIT_OLayer();
	tmpnam(JOB.Name_1);
	Pat_Name = JOB.Name_1;
	Pat_All_Angle = __open__(JOB.Name_1, "wb");
	__write__(&MaSCOT_IO.FP_S, 1, sizeof(MaSCOT_IO.FP_S), Pat_All_Angle);
	__iniFls__();
	__ZLDFI__= 0;
	__ZLDLA__= 2;
	LSrc=0;
	CSrc=0;
	CObj=0;
	LObj=0;
	FObj=0;
	STak=0;
	BLim=0;
	BEof=0;
	CFragment = 0;
	XFragment =-1;
	MaSCOT_IO.D_Link(0);
	MaSCOT_IO.Dix_Out_Cnt = 0;

	READ_OBJ_LINE(this);
}
Object_Buffer::~Object_Buffer()
{
	__delFls__();
	__ZLDFI__= 0;
	__ZLDLA__= 0;
	STATUS = 1;
	MaSCOT_IO.Dix_close();
	MaSCOT_IO.Close_ILayer(0);
	__close__(Pat_All_Angle, Pat_Name);
	__unlink__(O_Layer.BdbN);
	__rename__(O_Layer.PdbN, O_Layer.BdbN);
}
Obj_Line*
SET_OBJ_LINE_PTR_1(long STRIP1_Y, Object_Buffer*B)
{
	Buf_Obj_Line*Obj = B->FObj;
	B->CObj = B->FObj;
	while(Obj->Obj.y2<=STRIP1_Y)
	{
		SETNEXT_OBJ_LINE(B);
		B->push(Obj);
		B->FObj = B->CObj;
		Obj = B->FObj;
	}
	return &Obj->Obj;
}
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef LONG_MAX
#define LONG_MAX (((long)1l<<30)+(((long)1l<<30)-1))
#endif
char*fractg = "FRACTG";
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
	if(X)R&=d<200;
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
}
#undef X1
#undef X2
#undef Y1
#undef Y2
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
	F = max(objlx1, objlx2)<=Window[0];
	if(!F)
	F = min(objlx1, objlx2)>=Window[1];
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
		objly3 = max(objly1, Window[3]);
		objly4 = min(objly2, Window[4]);
		if(objly3>=objly4)zAbort(fractg,__LINE__);
		return 1;
	}
	D = objly2-objly1;
	if(D==0)
	{
		objly3 = objly4 = objly2;
		objlx3 = max(objlx1, Window[0]);
		objlx4 = min(objlx2, Window[1]);
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
			l_swap(objlx3, objlx4);
			l_swap(objly3, objly4);
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
/*local list header (S_L_PTR) for source line*/
/*and sine and cosine of rot-matrix.*/
/*No any check will be done.*/
/*Output: New Local List.*/
FLashb*
MdfLocFlsLis(FLashb*S_L_PTR, FLash_Buffer*B, M_5009_JOB&JOB)
{
	FLashb*F; /*Current Flash Index*/
	FLashb*Z; /*Current Flash Index*/
	FLashb*Pre_F;
	FLashb*Aux_F;
	FLashb*Nxt_F;

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
	long Hr;
	char Fl = 0;
	long DXSTEP;
	long DYSTEP;
	long RWM_X2;

	long DY_R_3;
	long DX_R_3;

	FLashb*RWM_F1;
	FLashb*RWM_F2;

	double RSLOPE;
	double DSLOPE;

	double __1Z1__;

	X_Flash[2] = 0;
	Pre_F = 0;
	for(F = S_L_PTR; F; Pre_F = Aux_F, F = Nxt_F)
	{
		x_1 = F->x1;
		x_2 = F->x2;
		y_2 = F->y2;

		Aux_F = F;
		Nxt_F = F->Nxt;

		if(y_2<=JOB.FMin)continue;
		if(x_1==x_2)continue;

		if(!COWAN_SPLIT(X_Flash))continue;
		if(objlx3>objlx4)
		{
			l_swap(objlx3, objlx4);
			l_swap(objly3, objly4);
		}
		X3 = min(objlx3, objlx4);
		X4 = max(objlx3, objlx4);
		Y3 = min(objly3, objly4);
		Y4 = max(objly3, objly4);

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

		Hr = floor(DX_2_1*sine*cosine+0.5);
		if(Y3>=(Hr>>1)||(CASE_2&&Y3==Y4))
		{
			F->y2 = Y3;
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
			CASE_X |= DY_4_3<JOB.FMin;
			CASE_X |= DX_4_3<JOB.FMin;
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
			{
				YR = floor(Y4-(DX_R_3)*DSLOPE+0.5);
			}
			XL = XR;
			YL = YR;

			DYSTEP = floor(YL-YL*__1Z1__);

			DXSTEP = DYSTEP*RSLOPE;
			if(DXSTEP<JOB.FMin)
			{
				DXSTEP = JOB.FMin;
				DYSTEP = ceil(DXSTEP*DSLOPE);
			}
			Hr = floor(DXSTEP*sine*cosine+0.5);
			YR = YL-DYSTEP;
			Fl = 0;
			if(Hr<YR&&Hr)
			{
				double Rel = sqrt((double)YR/Hr);
				YR = floor(Hr*Rel+0.5);
				Fl = 1;
			}
			if(Hr>YR&&YR)
			{
				long Rel = ceil((double)Hr/YR);
				YR = Hr/Rel;
				Fl = Rel>2? 2: 1;
			}
			if(Fl)
			{
			/*YR is not being elevated.*/
				DXSTEP = (YL-YR)*RSLOPE;
				if(Fl==2&&Y3==0)
				{
					if(IS_066(DX_4_3, DX_R_3, 0))
					{
						Fl = 0;
					}
				}
				else		Fl = 0;
			}
			XR = XL+(WAVE_D? DXSTEP: -DXSTEP);

			DY_R_3 = YR-Y3;
			DX_R_3 += DXSTEP;

			BREAK_LOOP = Fl;

			BREAK_LOOP |= DX_4_3-DX_R_3<JOB.FMin;
			BREAK_LOOP |= DY_R_3<JOB.FMin;

			if(BREAK_LOOP)
			{
				RWM_X2 = XL;
				break;
			}
			Z = B->__popFls__();

			Z->x1 = WAVE_U? XR: XL;
			Z->x2 = WAVE_D? XR: XL;
			Z->y2 = YR;
			Z->st = WAVE_U? 1: 2;

			if(!RWM_F1)
			{
				RWM_F1 = RWM_F2 = Z, Z->Nxt = 0;
			}
			else
			if(WAVE_U)
			{
				Z->Nxt = RWM_F2;
			}
			else
			{
				RWM_F2->Nxt = Z;
			}
			RWM_F2 = Z;
		}
		while(1);

		if(WAVE_U){FLashb*Z=RWM_F1; RWM_F1=RWM_F2; RWM_F2=Z;}

		END_OF_STEPS_PASSING:

		CASE_3 = 0;
		CASE_4 = 0;

		CASE_5 = DX_3_1!=0;
		CASE_6 = DX_2_4!=0;

		if(!WAVE_0&&RWM_F1==0)
		/*No any Steps done on CASE_2*/
		/*Note: Changing of WAVE_U, WAVE_D, WAVE_0.*/
		WAVE_U = WAVE_D = 0, WAVE_0 = 1;

		if(!IS_011(DX_2_1, DX_3_1, 0)&&DX_3_1>JOB.FMin) CASE_3 = 1;
		else
		CASE_3 = WAVE_U;

		if(!IS_011(DX_2_1, DX_2_4, 0)&&DX_2_4>JOB.FMin) CASE_4 = 1;
		else
		CASE_4 = WAVE_D;

		/*Note: Changing of CASE_5 and CASE_6.*/

		CASE_5 &= !CASE_3&&WAVE_D;
		CASE_6 &= !CASE_4&&WAVE_U;

		if(CASE_3)
		{
			Aux_F = B->__popFls__();

			Aux_F->Nxt = F;
			if(Pre_F)
			Pre_F->Nxt = Aux_F;
			else
			S_L_PTR = Aux_F;

			Pre_F = Aux_F;
			Aux_F->x1 = x_1;
			Aux_F->x2 = WAVE_U? RWM_X2: X3;
			Aux_F->y2 = CASE_1? y_2: objly3;
			Aux_F->st = F->st|1;
			F->st |= 2;
		}
		else
		if(CASE_5) RWM_F1->x1 = x_1;

		if(CASE_4)
		{
			Nxt_F = B->__popFls__();

			Nxt_F->Nxt = F->Nxt;
			F->Nxt = Nxt_F;

			Nxt_F->x1 = WAVE_D? RWM_X2: X4;
			Nxt_F->x2 = x_2;
			Nxt_F->y2 = CASE_1? y_2: objly4;
			Nxt_F->st = F->st|2;
			F->st |= 1;
		}
		else
		if(CASE_6) RWM_F2->x2 = x_2;

		if(CASE_1)
		{
			if(Y4>JOB.FMin)CASE_2 = 1; /*Will be passed on this Flash*/
			else	       CASE_2 = 0;

			if(CASE_2)
			{
				char inf_loop=1;
				inf_loop &= Y4==y_2? 1:0;
				inf_loop &= !CASE_3&&!CASE_4? 1:0;
				if(inf_loop)CASE_2 = 0;
			}
			F->y2 = CASE_2? Y4: Y3;
			F->x1 = CASE_3? X3: x_1;
			F->x2 = CASE_4? X4: x_2;

			if(CASE_2)Aux_F = Pre_F, Nxt_F = F;
			else
			if(CASE_4)Aux_F = Nxt_F, Nxt_F = Nxt_F->Nxt;
			continue;
		}
		if(!CASE_2)zAbort(fractg,__LINE__);

		if(WAVE_0) /*No Any Steps was done*/
		{
			F->y2 = Y3;
			F->x1 = CASE_3? X3: x_1;
			F->x2 = CASE_4? X4: x_2;

			if(CASE_4)Aux_F = Nxt_F, Nxt_F = Nxt_F->Nxt;
			continue;
		}
		if(Pre_F)Pre_F->Nxt = RWM_F1;
		else
		S_L_PTR = RWM_F1;

		RWM_F2->Nxt = Nxt_F;

		Aux_F = RWM_F2;
		B->__pushFls__(F);
		continue;
	}
	return S_L_PTR;
}
#undef x_1
#undef x_2
#undef y_2
#define y1 (ObjlObj.y1)
#define y2 (ObjlObj.y2)
#define x1 (ObjlObj.x1)
#define x2 (ObjlObj.x2)
void
DPOINT_ROTATE(double&X, double&Y)
{
	double T = X;
	X = T*SLRM_COS-Y*SLRM_SIN;
	Y = T*SLRM_SIN+Y*SLRM_COS;
}
void
LPOINT_ROTATE(long&X, long&Y)
{
	long T = X;
	X = floor(T*SLRM_COS-Y*SLRM_SIN+0.5);
	Y = floor(T*SLRM_SIN+Y*SLRM_COS+0.5);
}
void
CONVERT_OBJ_LINE(char&r_stat)
{
	x1 -= Obj_Delta_X;
	x2 -= Obj_Delta_X;
	y1 -= Obj_Delta_Y;
	y2 -= Obj_Delta_Y;
	LPOINT_ROTATE(x1, y1);
	LPOINT_ROTATE(x2, y2);
	if(y1>y2||(y1==y2&&x1>x2))
	{
		r_stat = r_stat? 0: 1;
		l_swap(y1, y2);
		l_swap(x1, x2);
	}
}
#undef y1
#undef y2
#undef x1
#undef x2
double
compute_cut_x(double X1, long Y1, long X2, long Y2, long Y)
{
	double X;
	double a;
	double b;
	double c;
	double d;
	a = Y1-Y2;
	b = X2-X1;
	c = (double)(X1)*Y2 - (double)(X2)*Y1;
	d = -(Y*b + c);
	X = a!=0? d/a: X1;
	return X;
}
void
__mergefls__(FLashb*F_I, FLash_Buffer*F)
{
	FLashb*f_1;
	FLashb*f_2;
	f_1 = F_I;
	for(f_2=f_1->Nxt; f_2; f_1=f_2,f_2=f_1->Nxt)
	{
		long&h1 = f_1->y2;
		long h2 = f_2->y2;
		if(_EQ_(h1,h2,1))
		{
			if(h1>h2)h1=h2;
			f_1->x2 = f_2->x2;
			f_1->Nxt = f_2->Nxt;

			F->__pushFls__(f_2);
			f_2=f_1;
		}
	}
}
char
__mgt__f(long a, long b, char f)
{
	long*c_a = f==1? &a: (long*)incr_far(chnl, a);
	long*c_b = f==2? &b: (long*)incr_far(chnl, b);
	Obj_Line*b_a = (Obj_Line*)incr_far(ObjlRot, (*c_a));
	Obj_Line*b_b = (Obj_Line*)incr_far(ObjlRot, (*c_b));
	return GTObj_Stat(b_a,b_b)>1;
}
extern DxbOutPut DxbOut;
extern char dbgoutdxb;
void
__DXBFL__(double Fw, double*Fx, double*Fy, char*s)
{
	long L;
	if(!dbgoutdxb)
	{
		return;
	}
	if(Fx==0||Fy==0)
	{
		DxbOut.write_eof();
		return;
	}
	if(!DxbOut.DXB)
	{
		DxbOut.nam = "$cmd.dxb";
		DxbOut.DXB = Fopen(DxbOut.nam, "wb");
		DxbOut.write_header();
		DxbOut.set_double();
	}
	if(s)
	{
		DxbOut.write_layer(s);
	}
	DxbOut.write_pline(0);
	DxbOut.write_width(Fw, Fw);
	for(L = 0; L<2; ++L)
	{
		DxbOut.write_vertex(Fx[L], Fy[L]);
	}
	DxbOut.write_seqend();
}
void __LinZLD__(long x1, long y1, long x2, long y2)
{
	ZLDLIM = 2;
	if(y1>y2||(y1==y2&&x1>x2))
	{
		l_swap(y1, y2);
		l_swap(x1, x2);
	}
	ZLDCO0[0] = x1;
	ZLDCO1[0] = y1;
	ZLDCO0[1] = x2;
	ZLDCO1[1] = y2;
	MaSCOT_IO.Dix_output_line();
	return;
}
void
__putFlash__(FLashb*Flash, long zMin, long zMax, long Strip, FILE*PatFile)
{
	char Wd_is_Dx=1;
	double __Alpha__=0;
	char Rotate=1;
	if(!(Angle_R!=0))
	{
		Rotate=0;
	}
	else
	if(Angle_R<90)
	{
		__Alpha__= 90;
		Wd_is_Dx = 0;
	}
	else
	if(Angle_R<180)
	{
		__Alpha__= 180;
		Wd_is_Dx = 1;
	}
	else
	if(Angle_R<270)
	{
		__Alpha__= 270;
		Wd_is_Dx = 0;
	}
	else
	{
		__Alpha__= 360;
		Wd_is_Dx = 1;
	}
	__Alpha__ -= Angle_R;
	long i;
	long j;
	long yCnt;
	long xCnt;
	long dy = Flash->y2-Flash->y1;
	long dx = Flash->x2-Flash->x1;
	yCnt = (dy-1)/zMax;
	++yCnt;
	dy = dy/yCnt;
	xCnt = (dx-1)/zMax;
	++xCnt;
	dx = dx/xCnt;
	char st;
	long Y1 = Flash->y1;
	long Dy;
	long Dx;
	for(j=0; j<yCnt; ++j)
	{
		long y1 = Y1;
		long y2 = y1+dy;
		if(j+1<yCnt)Dy = dy, st = 8; /*ext. up*/
		else
		{
			st = j==0? Flash->st&0xc: 4; /*ext. down*/
			y2 = Flash->y2;
			Dy = y2-y1;
		}
		if(Dy<zMin)
		{
			long z = zMin-Dy;
			Dy = zMin;
			if((st&4)!=0)y1 -= z;
			else
			if((st&8)!=0)y2 += z;
		}
		else
		if(((Dy>>1)<<1)!=Dy)
		{
			++Dy;
			if((st&4)!=0)--y1;
			else
			if((st&8)!=0)++y2;
		}
		Y1 += dy;
		long X1 = Flash->x1;
		for(i=0; i<xCnt; ++i)
		{
			long x1 = X1;
			long x2 = x1+dx;
			if(i+1<xCnt)Dx = dx, st |= 2; /*ext. right*/
			else
			{
				st |= i==0? Flash->st&0x3: 1; /*ext. left*/
				x2 = Flash->x2;
				Dx = x2-x1;
			}
			if(Dx<zMin)
			{
				long z = zMin-Dx;
				Dx = zMin;
				if((st&1)!=0)x1 -= z;
				else
				if((st&2)!=0)x2 += z;
			}
			else
			if(((Dx>>1)<<1)!=Dx)
			{
				++Dx;
				if((st&1)!=0)--x1;
				else
				if((st&2)!=0)++x2;
			}
			double Cx = x1 + ((double)(x2-x1))/2.0;
			double Cy = y1 + ((double)(y2-y1))/2.0;
			X1 += dx;
			Flashp Pat;
			Pat.Wd = (unsigned short)(Wd_is_Dx? Dx: Dy);
			Pat.Hg = (unsigned short)(Wd_is_Dx? Dy: Dx);
			Pat.An = 0;
			Pat.Zn = 0;
			if(Rotate)
			{
				Pat.An = (unsigned short)floor(__Alpha__*10 +0.5);
				DPOINT_ROTATE(Cx, Cy);
				Cx += Obj_Delta_X;
				Cy += Obj_Delta_Y;
			}
			Pat.Cx = (long)floor(Cx+0.5);
			Pat.Cy = (long)floor(Cy+0.5);
			Pat.Zn = (Pat.Cy-CYMIN-1)/Strip +1;
			if(Pat.Zn<1)Pat.Zn = 1;
			fwrite(&Pat, 1, sizeof(Flashp), PatFile);
			__error__(PatFile, 0);
		}
	}
}
void
OBJ_LIN_FLASH_EXPLOD(Object_Buffer*B, FLash_Buffer*F, M_5009_JOB*JOB)
{
	long STRIP0_Y;
	long STRIP1_Y;
	long STRIP3_Y;
	long STRIP2_X;
	long STRIP3_X;

	long X_F[4];

	long rX;
	long rY;
	long Hr;
	long Hm;

	FLashb*F_I;
	FLashb*f_1;
	FLashb*f_2;

	char status;
	char Status;
	char r_stat;
	char R_stat;
	if(!B->CSrc)
	{
		zAbort(fractf,__LINE__);
	}
	Obj_Line*Src = &B->CSrc->Obj;

	status = Src->status;
	r_stat = status? 1: 0;
	STATUS = r_stat? 1:-1;

	if(Src->x1==Src->x2||Src->y1==Src->y2)
	{
		__LinZLD__(Src->x1,Src->y1,Src->x2,Src->y2);
		return;
	}
	switch(r_stat)/*evaluate displacement and rotation*/
	{
		case 1: /*left edge*/
		Obj_Delta_X = Src->x2;
		Obj_Delta_Y = Src->y2;
		rX = Src->x1 - Obj_Delta_X;
		rY = Src->y1 - Obj_Delta_Y;
		break;

		case 0: /*right edge*/
		Obj_Delta_X = Src->x1;
		Obj_Delta_Y = Src->y1;
		rX = Src->x2 - Obj_Delta_X;
		rY = Src->y2 - Obj_Delta_Y;
		break;
		default: zAbort(fractf,__LINE__);
	}
	Angle_R = 360.0-Eval_Angle((double)rX, (double)rY);

	SLRM_SIN = sin(Angle_R*(M_PI/180.0));
	SLRM_COS = cos(Angle_R*(M_PI/180.0));
	sine = SLRM_SIN>0? SLRM_SIN: -SLRM_SIN;
	cosine = SLRM_COS>0? SLRM_COS: -SLRM_COS;

	LPOINT_ROTATE(rX, rY);

	Hm = JOB->HMax;
	Hr = ceil(rX*sine*cosine);

	F_I = F->__popFls__();

	F_I->Nxt = 0;
	X_F[0] = F_I->x1 = 0l;
	X_F[1] = F_I->x2 = rX;

	X_F[3] = F_I->y2 = Hr;
	X_F[2] = 0;

	STRIP3_Y = Src->y2+Hr;
	STRIP1_Y = Src->y1-Hr;
	STRIP0_Y = Src->y1-Hm;
	STRIP2_X = min(Src->x1,Src->x2)-Hr;
	STRIP3_X = max(Src->x2,Src->x1)+Hr;
	Obj_Line*Obj;
	Obj = SET_OBJ_LINE_PTR_1(STRIP0_Y, B);
	for(ObjlLim = 0; Obj->y1<STRIP3_Y; Obj = SETNEXT_OBJ_LINE(B))
	{
		if(Obj->y2<STRIP1_Y)
		{
			continue;
		}
		if(max(Obj->x1,Obj->x2)<STRIP2_X)
		{
			continue;
		}
		if(min(Obj->x2,Obj->x1)>STRIP3_X)
		{
			continue;
		}
		Status = Obj->status;
		R_stat = Status;
#define y1 (ObjlObj.y1)
#define y2 (ObjlObj.y2)
#define x1 (ObjlObj.x1)
#define x2 (ObjlObj.x2)
		ObjlObj = (*Obj);
		CONVERT_OBJ_LINE(R_stat);
		if(x1==x2)
		{
			continue;
		}
		if(x2<x1)
		{
			if(R_stat)continue;
		}
		else
		if(!R_stat)continue;
#undef y1
#undef y2
#undef x1
#undef x2
		if(COWAN_SPLIT(X_F))
		{
#define yc1 (ObjlCow.y1)
#define yc2 (ObjlCow.y2)
#define xc1 (ObjlCow.x1)
#define xc2 (ObjlCow.x2)
			if(ObjlLim>=ObjlMax-1)
			{
				zAbort(fractf,__LINE__);
			}
			if(yc1>yc2||(yc1==yc2&&xc1>xc2))
			{
				l_swap(yc1, yc2);
				l_swap(xc1, xc2);
			}
			ObjlCow.status = R_stat;
			chnl[ObjlLim] = ObjlLim;
			*(Obj_Line*)incr_far(ObjlRot, ObjlLim++) = ObjlCow;
		}
	}
	if(ObjlLim>1)
	{
		__mgt__ = __mgt__f;
		fastquicksort(0, ObjlLim-1);
		__mgt__ = __mgt__2;
	}
	unsigned i;
	unsigned j;
	for(i = 0; i<ObjlLim; ++i)
	{
		j = *((long*)incr_far(chnl, i));
		ObjlObj = *((Obj_Line*)incr_far(ObjlRot, j));
		F_I = MdfLocFlsLis(F_I, F, *JOB);
	}
#undef yc1
#undef yc2
#undef xc1
#undef xc2
	f_1 = 0;
	for(f_2 = F_I; f_2; f_1 = f_2)
	{
		long&h2 = f_2->y2;
		f_2 = f_2->Nxt;
		if(!f_1||!f_2)
		{
			if(!h2)continue;
		}
		if(h2<JOB->FMin)
		{
			h2=JOB->FMin;
		}
	}
	__mergefls__(F_I, F);
	for(f_2 = F_I; f_2; f_2 = f_2->Nxt)
	{
		long dx = f_2->x2-f_2->x1;
		long&y2 = f_2->y2;
		long h2 = ceil(dx*sine*cosine);
		if(h2<JOB->FMin)
		{
			h2=JOB->FMin;
		}
		if(h2<y2)
		{
			y2 = h2;
		}
	}
	__mergefls__(F_I, F);

	SLRM_SIN *= -1;
	f_1 = 0;
	for(f_2 = F_I; f_2; f_1=f_2, f_2=f_2->Nxt)
	{
		double x0 = f_2->x1;
		double x1 = x0;
		double x2;
		double x3 = f_2->x2;
		double x4;
		double x5;
		double dx = x3-x1;
		double y2 = f_2->y2;
		double y4;
		double y5;
		double h2 = dx*sine*cosine;
		long nx = y2!=0? ceil(h2/y2): 1;
		double Dx = dx/nx;
		double Fx[5];
		double Fy[5];
		x4 = x1;
		y4 = 0;
		DPOINT_ROTATE(x4, y4);
		x4 = floor(x4+Obj_Delta_X+0.5);
		y4 = floor(y4+Obj_Delta_Y+0.5);
		Fx[4] = Fx[0] = x4;
		Fy[4] = Fy[0] = y4;
		do
		{
			x2 = ceil(x1+Dx);
			if(x2>x3)x2=x3;
			x1 = x2;

			x5 = x2;
			y5 = 0;
			DPOINT_ROTATE(x5, y5);
			x5 = floor(x5+Obj_Delta_X+0.5);
			y5 = floor(y5+Obj_Delta_Y+0.5);
			__LinZLD__(x4,y4,x5,y5);
			x4 = x5;
			y4 = y5;
		}
		while(x2<x3);
		Fx[1] = x5;
		Fy[1] = y5;
		if(y2==0)
		{
			continue;
		}
		f_2->y1 = 0;
	__putFlash__(f_2, JOB->FMin, JOB->FMax, JOB->Strip, Pat_All_Angle);
		double Fw = y2/2.0;
		Fx[2] = x0;
		Fy[2] = Fw;
		Fx[3] = x3;
		Fy[3] = Fw;
		for(i = 2; i<4; ++i)
		{
			DPOINT_ROTATE(Fx[i], Fy[i]);
			Fx[i] += Obj_Delta_X;
			Fy[i] += Obj_Delta_Y;
		}
		__DXBFL__(Fw*2, Fx+2, Fy+2, "F");
	}
	while(F_I)
	{
		f_1 = F_I->Nxt;
		F->__pushFls__(F_I);
		F_I = f_1;
	}
}
void
M_5009_ALL_ANGLE(M_5009_JOB&JOB)
{
	Object_Buffer bSTrip(JOB);
	FLash_Buffer Flash;
	while(bSTrip.CSrc!=bSTrip.LSrc)
	{
		OBJ_LIN_FLASH_EXPLOD(&bSTrip, &Flash, &JOB);
		SETNEXT_SRC_LINE(&bSTrip);
	}
/*Closing O_Layer by destructor.*/
/*Deleting All memory allocation by destructor.*/
}
char*fboxes="fboxes";
#define bcxm CSrc.x2
#define bcxl CSrc.x1
#define bcyl CSrc.y1
#define bcym CSrc.y2
#define bckd (((double)(bcxl-bcxm))/((double)(bcyl-bcym)))

#define Objl_Rstatus 1 /*IT'S LEFT EDGE AND RSTAT = 1*/
#define Objl_Lstatus 0 /*IT'S RIGHT EDGE AND RSTAT = 0*/

#define NDir (CDir->Nxt)

#define nlyl (NDir->LY1)
#define nlym (NDir->LY2)
#define nlxl (NDir->LX1)
#define nlxm (NDir->LX2)
#define nryl (NDir->RY1)
#define nrym (NDir->RY2)
#define nrxl (NDir->RX1)
#define nrxm (NDir->RX2)
#define nlxi (NDir->LXi)
#define nrxi (NDir->RXi)

#define olyl (ODir->LY1)
#define olym (ODir->LY2)
#define olxl (ODir->LX1)
#define olxm (ODir->LX2)
#define oryl (ODir->RY1)
#define orym (ODir->RY2)
#define orxl (ODir->RX1)
#define orxm (ODir->RX2)
#define olxi (ODir->LXi)
#define orxi (ODir->RXi)

#define flyl (CDir->LY1)
#define flym (CDir->LY2)
#define flxl (CDir->LX1)
#define flxm (CDir->LX2)
#define fryl (CDir->RY1)
#define frym (CDir->RY2)
#define frxl (CDir->RX1)
#define frxm (CDir->RX2)
#define flxi (CDir->LXi)
#define frxi (CDir->RXi)

#define flkd (((double)(flxl-flxm))/((double)(flyl-flym)))
#define frkd (((double)(frxl-frxm))/((double)(fryl-frym)))

struct Dir_Line
{
	long LXi; /*initial LOWER X*/
	long RXi; /*initial LOWER X*/
	double LX1; /*LOWER X*/
	double RX1; /*LOWER X*/
	long LY1; /*LOWER Y*/
	long LX2; /*UPPER X*/
	long LY2; /*UPPER Y*/
	long RY1; /*LOWER Y*/
	long RX2; /*UPPER X*/
	long RY2; /*UPPER Y*/
	Dir_Line*Nxt;
}	;
class _zbDirl: public _rCash
{
public:
	_zbDirl():_rCash(4096/sizeof(Dir_Line), 128){};
	~_zbDirl(){};
	Dir_Line& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 4096/sizeof(Dir_Line));
		}
		return *((Dir_Line*)aHot+(i-lo));
	}
}	;
class Dir_Trapezoid
{
	long BLim;
	long OLim;
	char Beof;
	char BEof;
	long FCnt;
	long fcyl;
	char mfdb;
	char*Pat_Name;

	Obj_Line CSrc;

	Dir_Line*CDir;
	Dir_Line*ODir;

	Dir_Line*FDir;
	Dir_Line*STak;

	_zbDirl __B__;
	FLash_Buffer __F__;
	FLashb*__t__;
	FLashb*__l__;
	FLashb*__p__;

	char EM5009;
	char OTELLO;

	long FMin;
	long FMax;
	long zStrip;

	Dir_Line*pop(void);
	void push(Dir_Line*Dir);
	void awaycrn(void);
	void getsrc(void);
	Dir_Line*put(void);
	void terminate(long Y);
	void em5009_terminate(long ym);
	void otello_putFlash(long x1,long x2,long y1,long y2);
	void otello_terminate(long ym);
	unsigned analize(void);
	unsigned merge(void);
	FLashb*__putBox__(FLashb*p, FLashb*f);
	void put_after(FLashb*p, FLashb*f, FLashb*z);
public:
	void makeboxes(void);
	~Dir_Trapezoid();
	Dir_Trapezoid(M_5009_JOB&JOB);
	Dir_Trapezoid(OTELLO_JOB&JOB);
}	;
void Dir_Trapezoid::getsrc(void)
{
	if(Beof)
	{
		BEof = 1;
		return;
	}
	do
	{
		char z;
		if(OTELLO)z=mg_get_otello_obj(CSrc, OLim++);
		else z=MaSCOT_IO.ZDREAD_L(CSrc);
		if(!z)
		{
			Beof = 1;
			return;
		}
		if(bcxm==bcxl)
		{
			return;
		}
	}
	while(bcym==bcyl);
}
void Dir_Trapezoid::awaycrn(void)
{
	Dir_Line*Nxt;
	if(ODir==0)
	Nxt = (FDir = CDir->Nxt);
	else
	Nxt = (ODir->Nxt = CDir->Nxt);
	push(CDir);
	CDir = Nxt;
}
Dir_Line*Dir_Trapezoid::put(void)
{
	Dir_Line*z=pop();
	if(!z)
	{
		z=&__B__[BLim++];
	}
	if(ODir!=0)
	{
		z->Nxt = ODir->Nxt;
		ODir->Nxt = z;
	}
	else
	{
		z->Nxt = FDir;
		FDir = z;
	}
	return z;
}
Dir_Line*Dir_Trapezoid::pop(void)
{
	Dir_Line*z=STak;
	if(z)
	{
		STak = STak->Nxt;
	}
	return z;
}
void Dir_Trapezoid::push(Dir_Line*Dir)
{
	Dir->Nxt = STak;
	STak = Dir;
}
Dir_Trapezoid::Dir_Trapezoid(OTELLO_JOB&JOB)
{
	EM5009 = 0;
	OTELLO = 1;
	FMax = JOB.FSiz;
	FMin = 1;
	zStrip = JOB.STep;
	Angle_R = 0;
	CDir=0;
	ODir=0;
	FDir=0;
	STak=0;
	BLim=0;
	OLim=0;
	BEof=0;
	Beof=0;
	FCnt=0;
	__t__ = 0;
	__l__ = 0;
	__p__ = 0;
	getsrc();
}
Dir_Trapezoid::Dir_Trapezoid(M_5009_JOB&JOB)
{
	EM5009 = 1;
	OTELLO = 0;
	memcpy(I_Layer[0].Name, JOB.Name, 20);
	ILayer_Cnt= 1;
	MaSCOT_IO.Find_ILayer();
	OutDeltaX = 0;
	OutDeltaY = 0;
	MaSCOT_IO.Make_PLT_File_Name(I_Layer[0].PdbN, I_Layer[0].LX_S.PDBI);
	if(!FindxFile(I_Layer[0].PdbN))
	{
		zAbort(fractf,__LINE__);
	}
	MaSCOT_IO.Init_ILayer_File(I_Layer);
	tmpnam(JOB.Name_2);
	Pat_Name = JOB.Name_2;
	Pat_ORtogonal = __open__(JOB.Name_2, "wb");
	__write__(&MaSCOT_IO.FP_S, 1, sizeof(MaSCOT_IO.FP_S), Pat_ORtogonal);
	FMin = JOB.FMin;
	FMax = JOB.FMax;
	zStrip = JOB.Strip;
	Angle_R = 0;
	CDir=0;
	ODir=0;
	FDir=0;
	STak=0;
	BLim=0;
	OLim=0;
	BEof=0;
	Beof=0;
	FCnt=0;
	__t__ = 0;
	__l__ = 0;
	__p__ = 0;
	CFragment = 0;
	XFragment =-1;
	MaSCOT_IO.D_Link(0);
	getsrc();
}
Dir_Trapezoid::~Dir_Trapezoid()
{
	if(OTELLO)return;
	MaSCOT_IO.Close_ILayer(0);
	__unlink__(I_Layer[0].PdbN);
	__DXBFL__(0,0,0,0);
	__close__(Pat_ORtogonal, Pat_Name);
}
FLashb*Dir_Trapezoid::__putBox__(FLashb*p, FLashb*f)
{
	double Fx[2], Fy[2], Fw;
	__putFlash__(f, FMin, FMax, zStrip, Pat_ORtogonal);
	Fw = (f->y2-f->y1)/(double)2;
	Fx[0] = f->x1;
	Fx[1] = f->x2;
	Fy[0] = f->y1+Fw;
	Fy[1] = f->y1+Fw;
	__DXBFL__(Fw*2, Fx, Fy, "B");
	FLashb*z=f->Nxt;
	if(p)
	{
		p->Nxt = z;
	}
	else
	{
		__t__ = z;
	}
	__F__.__pushFls__(f);
	return z;
}
void
Dir_Trapezoid::put_after(FLashb*p, FLashb*f, FLashb*z)
{
	if(p)
	{
		p->Nxt = f;
	}
	else
	{
		__t__ = f;
	}
	f->Nxt = z;
}
class FlshOptmzdBfr
{
	long Pos;
	long Len;
	char Ovr;
public:
	long FlsCnt;
	Flashp*Bfr;
	void PutFls(Flashp&Fp);
	void RstBfr(void);
	~FlshOptmzdBfr();
	FlshOptmzdBfr();
}	;
FlshOptmzdBfr*ExtFlshBfr;
FlshOptmzdBfr::~FlshOptmzdBfr()
{
	delete Bfr;
}
FlshOptmzdBfr::FlshOptmzdBfr()
{
	Bfr=new Flashp[256];
	Len=256;
	Pos=0;
	Ovr=0;
	FlsCnt=0;
}
void FlshOptmzdBfr::PutFls(Flashp&Fp)
{
	if(Pos>=Len)
	{
		Flashp*t;
		t=new Flashp[Len<<=1];
		memcpy(t, Bfr, Pos*sizeof(Flashp));
		delete Bfr;
		Bfr=t;
		Ovr=1;
	}
	++FlsCnt;
	Bfr[Pos++] = Fp;
}
void FlshOptmzdBfr::RstBfr(void)
{
	if(Ovr)
	{
		delete Bfr;
		Bfr=new Flashp[Len];
	}
	Pos=0;
	Ovr=0;
	FlsCnt=0;
}
void
Dir_Trapezoid::otello_putFlash(long x1,long x2,long y1,long y2)
{
	Flashp Pat;
	if(UN001!=1)
	{
		x1=floor(x1*STSIZ+0.5);
		x2=floor(x2*STSIZ+0.5);
		y1=floor(y1*STSIZ+0.5);
		y2=floor(y2*STSIZ+0.5);
	}
	x1<<=1;
	x2<<=1;
	y1<<=1;
	y2<<=1;
	Pat.Wd = x2-x1;
	Pat.Hg = y2-y1;
	Pat.An = 0;
	Pat.Zn = 0;
	Pat.Cx = x1+(Pat.Wd>>1);
	Pat.Cy = y1+(Pat.Hg>>1);
	if(Pat.Wd==0)Pat.Wd=2;
	if(Pat.Hg==0)Pat.Hg=2;
	Pat.Zn = (Pat.Cy)/zStrip+1;
	ExtFlshBfr->PutFls(Pat);
}
void
Dir_Trapezoid::otello_terminate(long ym)
{
	long oly1 = flyl;
	long oly2 = fryl;
	long olx1 = flxl;
	long olx2 = frxl;
	long dly1 = flym - flyl;
	long dly2 = frym - fryl;
	long dlx1 = flxm - flxl;
	long dlx2 = frxm - frxl;
	if(flyl!=fryl||(dly1<=0||dly2<=0))
	{
		zAbort(fractf,__LINE__);
	}
	if(!dlx2&&!dlx1)
	{
		otello_putFlash(olx1,olx2,oly1,ym);
		return;
	}
	long dsy1 = 0,    dsy2 = 0;
	long dsx1 = 0,    dsx2 = 0;
	long erx1 = 0,    ery1 = 0;
	long erx2 = 0,    ery2 = 0;
	long dst1;
	long dst2;
	long x1;
	long x2;
	long y1;
	long y2;

	int i = 0;
	int k = 0;
	int j = 0;
	int inx1 = 0;
	int inx2 = 0;
	char hadincrx1 = 0;
	char hadincrx2 = 0;
	char semaphore = 0;

	if(dlx1>0)
	{
		inx1 =  1;
	}
	else
	if(dlx1<0)
	{
		inx1 = -1;
	}
	if(dlx2>0)
	{
		inx2 =  1;
	}
	else
	if(dlx2<0)
	{
		inx2 = -1;
	}
	if(dlx1<0)dlx1 =-dlx1;
	if(dlx2<0)dlx2 =-dlx2;
	char onx1;
	char onx2;
	if(dlx1>dly1)
	{
		onx1 = 1;
		dst1 = dlx1;
	}
	else
	{
		onx1 = 0;
		dst1 = dly1;
	}
	if(dlx2>dly2)
	{
		onx2 = 1;
		dst2 = dlx2;
	}
	else
	{
		onx2 = 0;
		dst2 = dly2;
	}
	if(dlx1==0)dlx1 = dly1;
	if(dlx2==0)dlx2 = dly2;

	while(!semaphore)
	{
		semaphore = (oly1==ym)&&(oly2==ym);
		if(dsy1==dsy2&&dsy1)
		{
			if(onx2)
			{
				i = dsx2>=zStrip;
			}
			else
			{
				i = inx2!=0? dsy2>=zStrip&&dsx2: 0;
			}
			if(onx1)
			{
				j = dsx1>=zStrip;
			}
			else
			{
				j = inx1!=0? dsy1>=zStrip&&dsx1: 0;
			}
			k = i||j;
		}
		else	k = 0;
		if(k||semaphore)
		{
			if(semaphore)
			{
				long z;
				z = (erx1+dlx1)>dst1;
				if(!hadincrx1&&z)
				{
					olx1 += inx1;
				}
				z = (erx2+dlx2)>dst2;
				if(!hadincrx2&&z)
				{
					olx2 += inx2;
				}
			}
			if(!dsy1)dsy1 = 1;
			y1 = oly1-dsy1;
			y2 = oly1;
			if(dsx1>1)dsx1>>=1;
			if(dsx2>1)dsx2>>=1;
			x1 = olx1 - dsx1*inx1;
			x2 = olx2 - dsx2*inx2;
			otello_putFlash(x1,x2,y1,y2);
			dsx1 = 0;
			dsx2 = 0;
			dsy1 = 0;
			dsy2 = 0;
		}
		if(oly1!=ym&&dsy1<=dsy2)
		{
			erx1 += dlx1;
			ery1 += dly1;
			if(erx1>dst1)
			{
				hadincrx1 =1;
				erx1 -= dst1;
				olx1 += inx1;
				++dsx1;
			}
			else	hadincrx1 =0;
			if(ery1>dst1)
			{
				ery1 -= dst1;
				++oly1;
				++dsy1;
			}
		}
		if(oly2!=ym&&dsy2<dsy1)
		{
			erx2 += dlx2;
			ery2 += dly2;
			if(erx2>dst2)
			{
				hadincrx2 =1;
				erx2 -= dst2;
				olx2 += inx2;
				++dsx2;
			}
			else	hadincrx2 =0;
			if(ery2>dst2)
			{
				ery2 -= dst2;
				++oly2;
				++dsy2;
			}
		}
	}
}
void
Dir_Trapezoid::em5009_terminate(long ym)
{
	long xl=max(flxi, flxm);
	long xm=min(frxi, frxm);
	if(xl<xm)
	{
		long yl=max(flyl, fryl);
		if(ym>yl)
		{
			FLashb*f=__F__.__popFls__();
			f->x1 = xl;
			f->x2 = xm;
			f->y1 = yl;
			f->y2 = ym;
			f->st = 0;
			FLashb*z=0;
			FLashb*p=0;
			FLashb*t=__t__;
			if(__l__)
			{
				if(f->x1>=__l__->x1)
				{
					t=__l__;
					p=__p__;
				}
			}
			long&x1 = f->x1;
			long&x2 = f->x2;
			long&y1 = f->y1;
			long&y2 = f->y2;
			for(z=t; z; p = z, z = z? z->Nxt: __t__)
			{
				long&x3 = z->x1;
				long&x4 = z->x2;
				long&y3 = z->y1;
				long&y4 = z->y2;
				if(y4<y1&&x1<=x4&&x2>=x3)
				{
					__putBox__(p, z);
					z=p;
					continue;
				}
				if(x2<=x3)
				{
				/*insert outside loop.*/
					break;
				}
				if(x1>=x4)
				{
					continue;
				}
				if(x3>=x1&&x4<=x2)
				{
				/*mode: cut upper(f)*/
					if(y4!=y1)
					{
						zAbort(fboxes,__LINE__);
					}
					long dyf = y2-y1;
					long dxf = x2-x1;
					long dxl = x3-x1;
					long dxr = x2-x4;
					char fl = dyf<(dxf>>4);
					char LCut = dxl&&fl;
					char RCut = dxr&&fl;
					char Left = dxl==0||LCut;
					char Right = dxr==0||RCut;
					if(Left&&Right)
					{
						FLashb*x;
						if(!LCut)goto mcunlc;
						x=__F__.__popFls__();
						*x = *f;
						x->x2 = x3;
						x1 = x3;
						x->st = 1; /*to right.*/
						put_after(p, x, z);
						p = x;
						/*
						xf  or xff
						0z     0z0
						*/
						mcunlc:
						if(!RCut)goto mcunrc;
						x1 = x4;
						y4 = y2;
						f->st |= 2; /*to left.*/
						/*
						zf  or xzf
						z0     0z0
						*/
						continue;

						mcunrc:
						y4 = y2;
						__F__.__pushFls__(f);
						f = 0;
						/*
						f  or xf
						z     0z
						*/
						break;
					}
					__putBox__(p, z);
					z=p;
				}
				else
				if(x1>=x3&&x2<=x4)
				{
				/*mode: cut lower(z)*/
					if(y4!=y1)
					{
						zAbort(fboxes,__LINE__);
					}
					long dyz = y4-y3;
					long dxl = x1-x3;
					long dxr = x4-x2;
					long dxf = x2-x1;
					long dYz = dyz>>4;
					char LCut = dxl&&dxl>dYz;
					char RCut = dxr&&dxr>dYz;
					char fl = dxf>dYz;
					if(!fl)
					{
						LCut = RCut = 0;
					}
					char Left = dxl==0||LCut;
					char Right = dxr==0||RCut;
					if(Left&&Right)
					{
						FLashb*x=0;
						if(!LCut)goto mclnlc;
						x=__F__.__popFls__();
						*x = *z;
						x->st = 1; /*to right.*/
						x->x2 = x1;
						x3 = x1;
						put_after(p, x, z);
						p = x;

						mclnlc:
						if(!RCut)goto mclnrc;
						z->st |= 2; /*to left.*/
						x3 = x2;
						y1 = y3;
						/*
						0f0  or f0
						x1z     1z
						*/
					/*insert outside loop.*/
						goto mclend;

						mclnrc:
						/*
						0f  or f
						xz     z
						*/
						y4 = y2;
						__F__.__pushFls__(f);
						f = 0;

						mclend: break;
					}
					f->st |= 4; /*down.*/
					z=__putBox__(p, z);
					break;
				}
				else
				if(x1<x3)
				{
				/*insert outside loop.*/
					break;
				}
			}
			if(f)
			{
				put_after(p, f, z);
			}
			__l__ = p? p->Nxt: 0;
			__p__ = p;
		}
	}
}
void
Dir_Trapezoid::terminate(long ym)
{
	char r;
	char l;
	l = (flyl!=flym? 1: 0)+(flxl!=flxm? 7: 0);
	r = (fryl!=frym? 1: 0)+(frxl!=frxm? 7: 0);
	if(l&&r)
	{
		if(EM5009)
		{
			em5009_terminate(ym);
		}
		else
		if(OTELLO)
		{
			otello_terminate(ym);
		}
		else
		{
			zAbort(fractf,__LINE__);
		}
	}
	if(l==8)
	{
		if(flym!=ym)
		flxl = compute_cut_x(flxl, flyl, flxm, flym, ym);
		else
		flxl = (flxi = flxm);
	}
	if(r==8)
	{
		if(frym!=ym)
		frxl = compute_cut_x(frxl, fryl, frxm, frym, ym);
		else
		frxl = (frxi = frxm);
	}
	flyl = ym;
	fryl = ym;
}
unsigned
Dir_Trapezoid::analize(void)
{
/*NON HORIZONTAL OBJ-LINE MUST BE.*/
/*NO ANY JUNCTION LINES MUST BE.*/
	register char R_F;
	R_F = bcyl!=fcyl;
	if(R_F)
	{
		return 0; /*REWIND FRONTS*/
	}
	R_F = CDir==0;
	if(R_F)
	{
		return 1; /*IT IS END OF FRONTS*/
	}
	R_F = flym==flyl&&frym==fryl;
	if(R_F)
	{
		return 2; /*THIS FRONT WAS MERELY TERMINATED*/
	}
	R_F = fcyl!=(flyl==flym? fryl: flyl);
	if(R_F)
	{
		if(flym==flyl)return 3;
		if(frym==fryl)return 7;

		if(bcyl<flym&&bcyl<frym)
		{
			double ffxl=min(flxl, flxm);
			double ffxm=max(frxl, frxm);

			if(bcxl<=ffxl)return 4;
			if(bcxl>=ffxm)return 5;

			ffxl = compute_cut_x(flxl, flyl, flxm, flym, fcyl);
			ffxm = compute_cut_x(frxl, fryl, frxm, frym, fcyl);

			if(bcxl<=ffxl)return 4;
			if(bcxl>=ffxm)return 5;
		}
		terminate(min(fcyl, min(flym, frym)));
		return analize();
	}
	R_F = flym==flyl;
	if(R_F)
	{
		return 3;
	}
	/*THIS IS SPECIAL CASE FOR MERGENING*/
	R_F = frym==fryl;
	if(R_F)
	{
		R_F = bcxl==frxm&&bcym==frym;
		if(R_F)
		{
			return 8;
		}
		R_F = bcxl>=flxl;
		if(R_F)
		{
			return 7;
		}
	}
	/*ELSE IT IS A "LEFT" CASE AND SUBMITTED TO SYMPLE PASS*/
	R_F = bcxl<flxl;
	if(R_F)
	{
		return 4;
	}
	R_F = bcxl==flxl;
	if(R_F)
	return bckd<flkd? 4: (bcxl==frxl? (bckd>frkd? 5: 6): 6);

	R_F = bcxl<frxl;
	if(R_F)
	{
		return 6;
	}
	return 5;
}
unsigned
Dir_Trapezoid::merge(void)
{
	unsigned error=0;
	Dir_Line*o_l_d = ODir;
	Dir_Line*c_r_n = CDir;
	unsigned endloop=0;
	ODir = CDir;
	CDir = NDir;
	while(!endloop)
	{
		if(error)
		{
			break;
		}
		switch(analize())
		{
			case 6: case 0: error = 1;
			case 2:
			awaycrn();
			break;
			case 7:
			error = merge();
			endloop = 1;
			break;
			case 3:
			mfdb=1;
			oryl = fryl;
			orym = frym;
			orxl = frxl;
			orxm = frxm;
			orxi = frxi;
			awaycrn();
			endloop = 1;
			break;
			default:
			endloop = 1;
			break;
		}
	}
	ODir = o_l_d;
	CDir = c_r_n;
	return error;
}
void
Dir_Trapezoid::makeboxes(void)
{
	unsigned nlop = 0;
	unsigned error = 0;

	fcyl=-LONG_MAX;
	while(!BEof)
	{
		if(nlop>1024)
		{
			error = 1;
			break;
		}
		switch(analize())
		{
			case 0:
			fcyl = bcyl;
			CDir = FDir;
			ODir = 0;
			nlop = 0;
			break;
			case 1:
			fcyl = bcyl;
			case 4:
			CDir = put();
			flxi = bcxl;
			flxl = bcxl;
			flyl = bcyl;
			flxm = bcxm;
			flym = bcym;
			frxl = LONG_MAX;
			fryl = bcyl;
			frxm = LONG_MAX;
			frym = bcyl;
			getsrc();
			nlop = 0;
			break;
			case 2:
			if(bcyl!=flyl||bcxl!=flxl)
			{
				awaycrn();
				nlop = 0;
				break;
			}
			case 3:
			flxi = bcxl;
			flxl = bcxl;
			flyl = bcyl;
			flxm = bcxm;
			flym = bcym;
			getsrc();
			nlop = 0;
			break;
			case 5:
			ODir = CDir;
			CDir = NDir;
			nlop = 0;
			break;
			case 6:
			CDir = put();
			flxi = nlxi;
			flxl = nlxl;
			flyl = nlyl;
			flxm = nlxm;
			flym = nlym;
			nlym = nlyl;
			frxi = bcxl;
			frxl = bcxl;
			fryl = bcyl;
			frxm = bcxm;
			frym = bcym;
			getsrc();
			nlop = 0;
			break;
			case 7:
			++nlop;
			mfdb=0, error = merge();
			if(error)
			{
				break;
			}
			if(mfdb||(fryl!=frym||fcyl!=flyl))
			{
				break;
			}
			case 8:
			frxi = bcxl;
			frxl = bcxl;
			fryl = bcyl;
			frxm = bcxm;
			frym = bcym;
			getsrc();
			nlop = 0;
			break;
			default: error = 1;
		}
		if(error)
		{
			break;
		}
	}
	if(error)zAbort(fboxes,__LINE__);
	if(EM5009)
	{
		FLashb*p=__t__;
		while(p)
		{
			p=__putBox__(0, p);
		}
	}
}
void
M_5009_TRAPEZOID(M_5009_JOB&JOB)
{
	Dir_Trapezoid Trapezoid(JOB);
	Trapezoid.makeboxes();
}
static unsigned char tab_ebcdic[] =
{
	0x00,0x01,0x02,0x03,0x37,0x2d,0x2e,0x2f,
	0x16,0x05,0x15,0x0b,0x0c,0x0d,0x0e,0x0f,

	0x10,0x11,0x12,0x00,0x3c,0x3d,0x32,0x26,
	0x18,0x19,0x3f,0x27,0x1c,0x1d,0x1e,0x1f,

	0x40,0x5a,0x7f,0x7b,0x5b,0x6c,0x50,0x7d,
	0x4d,0x5d,0x5c,0x4e,0x6b,0x60,0x4b,0x61,

	0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,
	0xf8,0xf9,0x7a,0x5e,0x4c,0x7e,0x6e,0x6f,

	0x7c,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
	0xc8,0xc9,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,

	0xd7,0xd8,0xd9,0xe2,0xe3,0xe4,0xe5,0xe6,
	0xe7,0xe8,0xe9,0x00,0xe0,0x00,0x5f,0x6d,

	0x79,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
	0x88,0x89,0x91,0x92,0x93,0x94,0x95,0x96,

	0x97,0x98,0x99,0xa2,0xa3,0xa4,0xa5,0xa6,
	0xa7,0xa8,0xa9,0xc0,0x6a,0xd0,0xa1,0x07
}	;
unsigned char ASCII_to_EBCDIC (unsigned char c)
{
	return tab_ebcdic[c];
}
struct MANN3000BOX
{
	long An;
	long Cx;
	long Cy;
	long Dx;
	long Dy;
	FILE*man;
	void put(Flashp*b);
	MANN3000BOX(FILE*z){An=Cx=Cy=Dx=Dy=LONG_MAX; man=z;}
}	;
void MANN3000BOX::put(Flashp*b)
{
	unsigned i;
	char str[80];
	char Str[80];
	str[0] = 0;
	if(Cx!=b->Cx)
	{
		sprintf(Str, "X%ld", Cx = b->Cx);
		strcat(str, Str);
	}
	if(Cy!=b->Cy)
	{
		sprintf(Str, "Y%ld", Cy = b->Cy);
		strcat(str, Str);
	}
	if(Dx!=(long)b->Wd)
	{
		sprintf(Str, "W%ld", Dx = b->Wd);
		strcat(str, Str);
	}
	if(Dy!=(long)b->Hg)
	{
		sprintf(Str, "H%ld", Dy = b->Hg);
		strcat(str, Str);
	}
	if(An!=(long)b->An)
	{
		sprintf(Str, "A%ld", An = b->An);
		strcat(str, Str);
	}
	strcat(str,";");
	for(i=0; str[i]; ++i)
	{
		str[i] = ASCII_to_EBCDIC(str[i]);
	}
	if(i>1)
	{
		if(!man)zAbort(fboxes,__LINE__);
		fwrite(str, 1, i, man);
	}
}
void	M_5009_SORTSTRIP(char*s_name)
{
	pospoly	= pospoly_3;
	READRUN	= READRUN_3;
	__mGt__	= __mGt__3;
	__mGe__	= __mGe__3;
	__mgt__	= __mgt__3;
	zsmove	= zsmove_3;
	ZSMOVE	= ZSMOVE_3;
	memcpy(s_name,mg_sort(s_name),15);
}
long	M_5009_PATFILE(char*name, MANN3000BOX&MANN3000)
{
	FILE*file;
	file = __open__(name, "rb");
	long total=0;
	__read__(&MaSCOT_IO.FP_S, sizeof(MaSCOT_IO.FP_S), 1, file);
	do
	{
		Flashp p;
		long k=fread(&p, sizeof(p), 1, file);
		if(k!=1)
		{
			break;
		}
		MANN3000.put(&p);
		++total;
	}
	while(1);
	__close__(file, name);
	__unlink__(name);
	return total;
}
#include "f-size.hpp"
void
MANN300_PUTSTR(const char*str, FILE*man)
{
	char Str[80];
	long i;
	for(i=0; str[i]; ++i)Str[i] = ASCII_to_EBCDIC(str[i]);
	fwrite(Str, 1, i, man);
}
extern FILE*Rniirm;
extern char*Nniirm;
void
M_5009_MAIN(char Name[20], long FMin, long FMax, long HMax)
{
fprintf(stderr, "\n!!! <MANN-3000 Postprocessor> Started on %s !!!\n",Name);
fprintf(Rniirm, "\n!!! <MANN-3000 Postprocessor> Started on %s !!!\n",Name);
	M_5009_JOB JOB;
	JOB.Strip = FMax>>1;
	if(JOB.Strip<4)JOB.Strip = 4;
	memcpy(JOB.Name, Name, 20);
	JOB.FMin = FMin;
	JOB.FMax = FMax;
	JOB.HMax = HMax;

	F_SIZE_JOB JOB_1;
	memcpy(JOB_1.Name, Name, 20);
	JOB_1.Size = 0;
	F_SIZE_SHAPES(JOB_1);

	__mGt__	= __mGt__2;
	__mGe__	= __mGe__2;
	__mgt__	= __mgt__2;
	zsmove	= zsmove_2;
	pospoly	= pospoly_2;
	READRUN	= READRUN_2;
	ZSMOVE	= ZSMOVE_2;
	memcpy(O_Layer.PdbN,mg_sort(O_Layer.BdbN),15);
	__rename__(O_Layer.PdbN,O_Layer.BdbN);

	M_5009_ALL_ANGLE(JOB);
	memcpy(O_Layer.PdbN,mg_sort(O_Layer.BdbN),15);
	__rename__(O_Layer.PdbN,O_Layer.BdbN);

	M_5009_TRAPEZOID(JOB);

	M_5009_SORTSTRIP(JOB.Name_1);
	M_5009_SORTSTRIP(JOB.Name_2);

	memset(JOB.Name_3, 0, 20);
	memcpy(JOB.Name_3, JOB.Name, 8);
	strcat(JOB.Name_3, ".pat");
	FILE*man = __open__(JOB.Name_3, "wb");

	MANN3000BOX MANN3000(man);

	long boxes=M_5009_PATFILE(JOB.Name_2, MANN3000);
	long flshs=M_5009_PATFILE(JOB.Name_1, MANN3000);

	MANN300_PUTSTR("$", man);
	__close__(man, JOB.Name_3);
fprintf(stderr, "\n!!! Total boxes = %ld", boxes);
fprintf(stderr, "\n!!! Total all angle flashes = %ld", flshs);
fprintf(stderr, "\n!!! Total flashes = %ld", boxes+flshs);
fprintf(Rniirm, "\n!!! Total boxes = %ld", boxes);
fprintf(Rniirm, "\n!!! Total all angle flashes = %ld", flshs);
fprintf(Rniirm, "\n!!! Total flashes = %ld", boxes+flshs);
	__close__(Rniirm,Nniirm);
exit(0);
}
void __FR2__(OTELLO_JOB&JOB)
{
	Dir_Trapezoid Trapezoid(JOB);
	long Xb = CFragment/YBCNT;
	long Yb = CFragment-(Xb*YBCNT);
	OutDeltaX = CXMIN+Xb*XBSIZ;
	OutDeltaY = CYMIN+Yb*YBSIZ;
	Trapezoid.makeboxes();
	OutDeltaX = 0;
	OutDeltaY = 0;
}
void __FR1__(void)
{
	long i;
	for(i=2; i<ZVTLIM; i+=2)
	{
		VtBead*Vt =GetVtBead(i);
		if(Vt->ZVTMeInreverse==0)
		{
	/*Possible horizontal or junction.See  long __SI1__(char)*/
			continue;
		}
		PtBead*P1 = Vt->ZVTCO1;
		PtBead*P2 = Vt->ZVTCO2;
		long x1 = P1->ZPTCO1;
		long y1 = P1->ZPTCO2;
		long x2 = P2->ZPTCO1;
		long y2 = P2->ZPTCO2;
		char st = Vt->ZVTFirstCoincident!=0;
		mg_put_otello_obj(x1,y1,x2,y2,st);
	}
}
void __IVE__(void)
{
	long i;
	void INSERT_VECTORS(LbBead*Lb);
	for(i=1; i<ZLBLIM; ++i)
	{
		LbBead*Lb=GetLbBead(i);
		INSERT_VECTORS(Lb);
	}
}
long Get_Otellofrgmntnmbr(long Fr)
{
	long Xb = Fr/YBCNT;
	long Yb = Fr-(Xb*YBCNT);
	if(Yb!=((Yb>>1)<<1))
	{
		Xb = XBCNT-Xb-1;
	}
	return Yb*XBCNT+Xb;
}
void Otello_wrthdr(FILE*Uml)
{
	long Dx=CXMAX-CXMIN;
	long Dy=CYMAX-CYMIN;
	long Fx=XBSIZ;
	long Fy=YBSIZ;
	struct
	{
		unsigned short Dx;
		unsigned short Dy;
		unsigned short Fx;
		unsigned short Fy;
		unsigned short Xc;
		unsigned short Yc;
	}	header;
	header.Dx=(Dx/UN001)/10;
	header.Dy=(Dy/UN001)/10;
	header.Fx=(Fx/UN001)/10;
	header.Fy=(Fy/UN001)/10;
	header.Xc=XBCNT;
	header.Yc=YBCNT;
	fwrite(&header, sizeof(header), 1, Uml);
}
void Otello_wrtflsh(Flashp&Fl, FILE*Uml, long FMax)
{
	unsigned short Wd=Fl.Wd>>1;
	unsigned short Hg=Fl.Hg>>1;
	unsigned short Ln;
	unsigned short m[4];
	m[0]=(unsigned short)(Fl.Cx-Wd)>>1;
	m[1]=(unsigned short)(Fl.Cy-Hg)>>1;
	m[0]|=0x8000;
	if(Wd>FMax||Hg>FMax)
	{
		m[2]=(unsigned short)Wd;
		m[3]=(unsigned short)Hg;
		m[1]|=0x8000;
		Ln=8;
	}
	else
	{
		m[2]=(unsigned short)Wd;
		m[2]|=(unsigned short)(Hg<<8);
		Ln=6;
	}
	fwrite(m, Ln, 1, Uml);
}
void Otello_eofrgmnt(long&XFragment, long Fr, FILE*Dst)
{
	unsigned short Eofr=0xe000;
	while(XFragment!=Fr)
	{
		if(XFragment++>Fr)
		{
			zAbort(fractf,__LINE__);
		}
		fwrite(&Eofr, 2, 1, Dst);
	}
}
extern char*OvrlpMstPrf;
void DspNmbrFrgmnt(long Fr);
void QSORT_STARTED(char f);
void LoadFragment(void);
long __SI1__(char Hr);
void __FINISHEDNORMALLY__(void);
void PrintOk(void);
void
OTELLO_MAIN(char Name[20], long BSiz, long FSiz, long STep)
{
/*Parameters in internal units.*/
	OTELLO_JOB JOB;
	__MGSEQ__ Sq;

	MaSCOT_IO.Find_ILayer();
	if(!(I_Layer[0].LX_S.STATUS&1))
	{
		fputs(OvrlpMstPrf, stderr);
		zAbort(fractf,__LINE__);
	}
	memcpy(JOB.Name, Name, 20);
	JOB.FSiz = FSiz;
	JOB.STep = STep;

	XBSIZ = BSiz;
	YBSIZ = BSiz;

	XBCNT = (CXMAX-CXMIN+XBSIZ)/XBSIZ;
	YBCNT = (CYMAX-CYMIN+YBSIZ)/YBSIZ;

	XBSIZ = (CXMAX-CXMIN+XBCNT)/XBCNT;
	YBSIZ = (CYMAX-CYMIN+YBCNT)/YBCNT;
	XBSIZ = ceil((double)XBSIZ/UN001);
	YBSIZ = ceil((double)YBSIZ/UN001);
	while(XBSIZ%10)++XBSIZ;
	while(YBSIZ%10)++YBSIZ;
	XBSIZ*=UN001;
	YBSIZ*=UN001;
	MaSCOT_IO.INIT_ILayer(0);
	SET_Xg_Yg(0);
	memcpy(O_Layer.Name, I_Layer[0].Name, 20);
	MaSCOT_IO.INIT_OLayer();
	FragmPolygons(0);
	MaSCOT_IO.Dix_close();
	MaSCOT_IO.Close_ILayer(0);
	UPSet_Xg_Yg();
	IEvaluater._ReleaseStorage();

	__mGt__	= __mGt__4;
	__mGe__	= __mGe__4;
	__mgt__	= __mgt__4;
	zsmove	= zsmove_1;
	pospoly	= pospoly_1;
	READRUN	= READRUN_1;
	ZSMOVE	= ZSMOVE_1;
	memcpy(O_Layer.PdbN,mg_sort(O_Layer.PdbN),15);

	MaSCOT_IO.Make_BDB_File_Name(I_Layer[0].PdbN, I_Layer[0].LX_S.PDBI);
	Delete_File(I_Layer[0].PdbN);
	__rename__(O_Layer.PdbN, I_Layer[0].PdbN);

	if(!FindxFile(I_Layer[0].PdbN))
	{
		zAbort(fractf,__LINE__);
	}
	MaSCOT_IO.Init_ILayer_File(I_Layer);
	I_Layer[0].Fragment = 0;
	MaSCOT_IO.D_Link(0);
	CFragment = 0;
	XFragment = 0;
	ILayer_Cnt =1;
	FlshOptmzdBfr FoB;
	ExtFlshBfr=&FoB;
	memcpy(JOB.Name_2, JOB.Name, 20);
	strcat(JOB.Name_2, ".UML");
	FILE*Uml=__open__(JOB.Name_2, "wb");
	Otello_wrthdr(Uml);
fprintf(stderr,"\n!!!PREPARE-UML FOR OTELLO STARTED\n");
fprintf(stderr,"\toutput file <%s>\n",JOB.Name_2);
	while(CFragment!=LONG_MAX)
	{
		long Fr=Get_Otellofrgmntnmbr(CFragment);
		DspNmbrFrgmnt(Fr);
		Otello_eofrgmnt(XFragment, Fr, Uml);
		ZLBLIM = 1;
		ZLVLIM = 1;
		ZPTLIM = 1;
		ZVTLIM = 2;
		STATUS =+1;
		CleanZPBCOR(ZPBLIM+1);
		LoadFragment(); /*Load Fragment*/
		__IVE__(); /*Allocate Vt for each Lv*/
		if(ZVTLIM==2)
		{
		CFragment = I_Layer[0].Fragment;
			continue;
		}
		POINT_VECTOR_ANALYSIS(2);
		long LCnt=__SI1__(1); /*Remove junctions and horizontals.*/
		Sq.Buf=0;
		mg_init_for_otello(LCnt>>1, &Sq);
		__FR1__(); /*Load Vts to Obj_Lines for sort*/
		__mgt__=__mgt__2;
		QSORT_STARTED(0);
		IEvaluater._ReleaseStorage();
		__FR2__(JOB); /*Build Flashes*/
		mg_destroy_from_otello();

		Sq.Buf=(char*)FoB.Bfr;
		mg_init_for_otello(FoB.FlsCnt, &Sq);
		__mgt__=__mgt__3;
		QSORT_STARTED(0);
		Sq.pos =0;
		Flashp Fl;
		while(mg_get_otello_fls(Fl, Sq.pos++))
		{
			Otello_wrtflsh(Fl, Uml, FSiz);
		}
		__error__(Uml, JOB.Name_2);
		Sq.Buf=0;
		mg_destroy_from_otello();
		FoB.RstBfr();
		CFragment = I_Layer[0].Fragment;
	}
	CFragment = (XBCNT*YBCNT)-1;
	Otello_eofrgmnt(XFragment, CFragment, Uml);
	__close__(Uml, JOB.Name_2);
	MaSCOT_IO.Close_ILayer(0);
	__unlink__(I_Layer[0].PdbN);
	MaSCOT_IO.SetMinMax();
	__FINISHEDNORMALLY__();
	exit(0);
}
