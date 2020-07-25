#include "stdafx.h" 

#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "fqeval.hpp"

#include "QuickSort.h"


long Lv_Ymin;
long Lv_Ymax;
long Lv_Xmin;
long Lv_Xmax;

long Vt_Ymax;
long Vt_Ymin;

VtBead*Vt_Last;

PtBead*_P1_;
PtBead*_P2_;
long _X1_;
long _Y1_;
long _X2_;
long _Y2_;
long _Lf_; // found leaf number in the intersection FindTree

long threshold_xmin; 

LvBead*_Lv_;
VtBead*_Vt_;

char Lv_isInreverse;
char Vt_isInreverse;

char Lv_isVertical;
char Lv_isHorizontal;
char Lv_isOrtogonal;

double Lv_Y21;
double Lv_X21;
double Lv_D21;

char I_Type; // intersection type

VtBead**Lf_VtChainFirst;
VtBead**Lf_VtChainLast;
VtBead**Lf_First;
unsigned short*Lf_CreatedLeaves;
unsigned short*Lf_Upper;
unsigned short*Lf_Lower;

long*Lf_Ymidl;
char*Lf_Haschildrens;

char Lf_Factor;

unsigned short Lf_CreatedLeavesLim;
unsigned short LF_LIM;
unsigned short Tree_next_pos;

char Initialized_hash;

VtBead*__VT0__;
LbBead*__LB0__;

_IEvaluater IEvaluater;


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
      __VT0__ = GetVtBead(0);
      __LB0__ = GetLbBead(0);
      memset(__VT0__, 0, sizeof(VtBead)<<1);
      memset(__LB0__, 0, sizeof(LbBead)<<1);
}
_IEvaluater::_IEvaluater()
{
      ZPBCOR = new PtBead*[ZPBLIM+1];
      memset(ZPBCOR, 0, (ZPBLIM+1)*sizeof(PtBead*));

      LF_LIM = 512;
      Lf_Haschildrens = new char[LF_LIM*25];
      Lf_VtChainFirst = (VtBead**)(Lf_Haschildrens+LF_LIM);
      Lf_VtChainLast = Lf_VtChainFirst+LF_LIM;
      Lf_First = Lf_VtChainLast+LF_LIM;
      Lf_Upper = (unsigned short*)(Lf_First+LF_LIM);
      Lf_Lower = Lf_Upper+LF_LIM;
      Lf_CreatedLeaves = Lf_Lower+LF_LIM;
      Lf_Ymidl = (long*)(Lf_CreatedLeaves+(LF_LIM<<1));
      Lf_Factor = 1;
      while(LF_LIM>>Lf_Factor) ++Lf_Factor;
      --Lf_Factor;

      __SetVT0LB0__();
}
_IEvaluater::~_IEvaluater()
{
      delete ZPBCOR;
      delete Lf_Haschildrens;

      _ReleaseStorage();
}


void Init_Vt_Findtree(long Y1, long Y2, unsigned short P, char L)
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
	long I;
	Tree_next_pos = 1;

	for(I = 0; I<LF_LIM; ++I)
	{
		Lf_Haschildrens[I] = 0;
		Lf_VtChainFirst[I] = 0;
		Lf_First[I] = 0;
		Lf_CreatedLeaves[I] = 0;
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
	Lf_CreatedLeavesLim = 0;
	Initialized_hash = F_EVAL_MODULE? 1: 0;
}
void Into_Lv_Chain(LvBead*Lv, VtBead*Vt)
{
	Lv->ZLVMyVector = Vt; 

	Lv->ZLVNextConcident = Vt->ZVTFirstCoincident;
	Vt->ZVTFirstCoincident = Lv;
}
void H_Insert(unsigned short Lf, VtBead*Vt)
{
	while(Lf_Upper[Lf])
	{
		if(Vt_Ymin>Lf_Ymidl[Lf])
		{
			Lf_Haschildrens[Lf] = 1;
			Lf = Lf_Upper[Lf];
		}
		else
		if(Vt_Ymax<Lf_Ymidl[Lf])
		{
			Lf_Haschildrens[Lf] = 1;
			Lf = Lf_Lower[Lf];
		}
		else { 
			break; 
		} 
	}

	if(Lf_VtChainFirst[Lf]==0)
	{
		Lf_VtChainLast[Lf] = Vt;
		Lf_CreatedLeaves[Lf_CreatedLeavesLim++] = Lf;
	}

	Vt->ZVTNextSortedVtBead = Lf_VtChainFirst[Lf];
	Lf_VtChainFirst[Lf] = Vt;
}
void H_INSERT(unsigned short Lf, VtBead*Vt)
{
	Vt_Ymin =Vt->ZVTCO1->ZPTCO2;
	Vt_Ymax =Vt->ZVTCO2->ZPTCO2;

	if(Vt_Ymin>Vt_Ymax)
	{
		l_swap(Vt_Ymin, Vt_Ymax);
	}

	H_Insert(Lf, Vt);
}
void Split_Vt(short Lf, VtBead*Vt, PtBead*Pt)
{
	H_INSERT(Lf, _Vt_ = SPLITVT(Vt, Pt));
}
void Remove_Vt(short Lf, VtBead*Vt)
{
	if(Vt_Last==__VT0__)
	{
		Lf_First[Lf] = Vt_Last->ZVTNextSortedVtBead = Vt->ZVTNextSortedVtBead;
	}
	H_INSERT(Lf, _Vt_ = Vt);
}
char Find_Intsect(unsigned short Lf)
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

	VtBead*Vt_Aux;

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

	char Vt_isVertical;
	char Vt_isHorizontal;

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

	__VT0__->ZVTNextSortedVtBead = 0;

	Vt = *(Lf_First+Lf);
	for(Vt_Last = __VT0__; Vt; Vt_Last = Vt_Aux, Vt = Vt->ZVTNextSortedVtBead)
	{
		Vt_Aux = Vt;

		P3 = Vt->ZVTCO1;
		X3 = P3->ZPTCO1;

		if(X3>Lv_Xmax)
		{
			continue;
		}
		P4 = Vt->ZVTCO2;
		X4 = P4->ZPTCO1;

		if(X4<threshold_xmin)
		{
			if(Vt_Last!=__VT0__)
			{
				Vt_Last->ZVTNextSortedVtBead = Vt->ZVTNextSortedVtBead;
				Vt_Aux = Vt_Last;
				continue;
			}
			Lf_First[Lf] = Vt->ZVTNextSortedVtBead;
			Vt_Aux = __VT0__;
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
		I_Type = 0;

		Vt_isInreverse = X3==X4&&Y3>Y4;
		Vt_isVertical = 0;
		Vt_isHorizontal = 0;

		if(Lv_isOrtogonal)
		{
			Vt_isVertical = X3==X4;
			Vt_isHorizontal = Y3==Y4;
			if(Vt_isVertical&&Lv_isVertical)goto IntCoincidence;
			if(Vt_isHorizontal&&Lv_isHorizontal)goto IntCoincidence;
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
IntCoincidence:
			if(Lv_isInreverse!=Vt_isInreverse)
			{
				if(_P1_==P3||_P2_==P4)continue;
				if(_P1_==P4&&_P2_==P3)I_Type = 6;
			}
			else
			{
				if(_P1_==P4||_P2_==P3)continue;
				if(_P1_==P3&&_P2_==P4)I_Type = 7;
			}
			switch(I_Type)
			{
				case 6:
					Vt = Vt->ZVTMeInreverse;
				case 7:
					Into_Lv_Chain(_Lv_, Vt);
// exit point
					return 1;
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
			if(Vt_isHorizontal||Vt_isVertical)
			{
				I_Type = 5;
				Int_X = Vt_isVertical? X3: Lv_Xmin;
				Int_Y = Vt_isVertical? Lv_Ymin: Y3;
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
		switch(I_Type)
		{
		case 1:
			Split_Vt(Lf, Vt, _P1_);

			Remove_Vt(Lf, Vt);
			Vt_Aux = Vt = Vt_Last;

			continue;
		case 2:
			Split_Vt(Lf, Vt, _P2_);

			Remove_Vt(Lf, Vt);
			Vt_Aux = Vt = Vt_Last;

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
			Vt_Aux = Vt = Vt_Last;
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
				Vt_Aux = Vt = Vt_Last;
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
			if(S1)Vs = Vs->ZVTMeInreverse;
			Into_Lv_Chain(Ls, Vs);

			if(Vt_L)
			{
				while(Vt_L--) { 
					H_INSERT(Lf, _Vt_ = Vt_S[Vt_L]); 
				} 

				Remove_Vt(Lf, Vt);
				Vt_Aux = Vt = Vt_Last;
			}
			if(_Lv_==Ls)
			{
// exit point
				return 1;
			}
			_P2_ = _Lv_->ZLVPoint2;
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

		Lv_isInreverse = (_X1_>_X2_)||((_X1_==_X2_)&&(_Y1_>_Y2_));

		D21 = ((double)(Lv_Ymax-Lv_Ymin)+(Lv_Xmax-Lv_Xmin))/(2.0);
	}

	return 0; 
}
char TreeIntersect_Lv(unsigned short Lf)
{
	if(Lf_First[Lf])
	{
		if(Find_Intsect(Lf)) 
		{ 
			return 1;
		}
	}

	if(!Lf_Haschildrens[Lf])
	{ 
		return 0;
	}

	if(_Lf_<0)
	{
		if((Lv_Ymax>=Lf_Ymidl[Lf])&&(Lv_Ymin<=Lf_Ymidl[Lf]))_Lf_ = Lf;
	}

	if(Lv_Ymin<=Lf_Ymidl[Lf])
	{
		if(TreeIntersect_Lv(Lf_Lower[Lf])) 
		{ 
			return 1;
		}
	}
	if(Lv_Ymax>=Lf_Ymidl[Lf])
	{
		if(TreeIntersect_Lv(Lf_Upper[Lf])) 
		{ 
			return 1;
		} 
	}
	return 0;
}
void Intsect_Lv(LvBead*Lv)
{
	VtBead*V3;
	VtBead*V4;

	_Lv_ = Lv;

	_P1_ = _Lv_->ZLVPoint1;
	_P2_ = _Lv_->ZLVPoint2;
	_X1_ = _P1_->ZPTCO1;
	_X2_ = _P2_->ZPTCO1;
	_Y1_ = _P1_->ZPTCO2;
	_Y2_ = _P2_->ZPTCO2;

	Lv_isInreverse = (_X1_>_X2_)||((_X1_==_X2_)&&(_Y1_>_Y2_));

	if(_X1_<_X2_)Lv_Xmin = _X1_,Lv_Xmax = _X2_;
	else Lv_Xmin = _X2_,Lv_Xmax = _X1_;

	if(_Y1_<_Y2_)Lv_Ymin = _Y1_,Lv_Ymax = _Y2_;
	else Lv_Ymin = _Y2_,Lv_Ymax = _Y1_;

	Lv_Y21 = _Y2_ - _Y1_;
	Lv_X21 = _X2_ - _X1_;

	Lv_D21 = ((double)(Lv_Ymax-Lv_Ymin)+(Lv_Xmax-Lv_Xmin))/(2.0);

	Lv_isHorizontal = Lv_Ymin==Lv_Ymax;
	Lv_isVertical = Lv_Xmin==Lv_Xmax;
	Lv_isOrtogonal = Lv_isVertical || Lv_isHorizontal;

	_Lf_ = -1;

	if(TreeIntersect_Lv(0))
	{
		return;
	}

	GetVtPair(V3, V4);

	_Vt_ = V3;

	if(Lv_isInreverse)
	{
		VtBead*V0=V3;V3=V4;V4=V0;
	}
	V4->ZVTFirstCoincident = 0;
	V3->ZVTFirstCoincident = _Lv_;

	Lv->ZLVMyVector = V3;

	V3->ZVTMeInreverse = V4;
	V4->ZVTMeInreverse = V3;

	V4->ZVTCO2 = V3->ZVTCO1 = _P1_;
	V4->ZVTCO1 = V3->ZVTCO2 = _P2_;

	V4->ZVTStatus = V3->ZVTStatus = 0;
	V3->ZVTAux = V4->ZVTAux = 0;

	Vt_Ymin = Lv_Ymin;
	Vt_Ymax = Lv_Ymax;

	H_Insert((_Lf_<0? 0: _Lf_), _Vt_);
}
void INTERSECTION_ANALYSIS(void)
{
	Init_Vt_FindTree();

	Lv_Sort(); // sorts LvBeads into PTRAux_1. 

	for(long Lvsorted = 1; Lvsorted<PTRAux_1Lim; ++Lvsorted)
	{
		LvBead*Lv = (LvBead*)PTRAux_1[Lvsorted];

		LvBead*LVNext = Lv->ZLVNextLvBead;

		PtBead*P1 = Lv->ZLVPoint1;
		PtBead*P2 = Lv->ZLVPoint2;
		long X1 = P1->ZPTCO1;
		long X2 = P2->ZPTCO1;

		threshold_xmin = X1<X2? X1: X2;

		if(ComplexIntersectionAllowed&&(ZLVLIM-PTRAux_1Lim>(Lvsorted<<3)))
		{
			RepeatComplexIntersection=1;
			Intsect_Lv(Lv);
		}
		else
		do
		{
			if(!Lv->ZLVMyVector)
			{
				Intsect_Lv(Lv);
			}
			Lv = Lv->ZLVNextLvBead;
		}
		while(Lv!=LVNext);

		for(long j = 0; j<Lf_CreatedLeavesLim; ++j)
		{
			long Lf = Lf_CreatedLeaves[j];

			Lf_VtChainLast[Lf]->ZVTNextSortedVtBead = Lf_First[Lf];
			Lf_First[Lf] = Lf_VtChainFirst[Lf];

			Lf_VtChainFirst[Lf] = 0;
		}

		Lf_CreatedLeavesLim = 0;
	}
}
