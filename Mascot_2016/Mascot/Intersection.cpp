/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#include "stdafx.h" 

#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "fqeval.hpp"
#include "fxeval.hpp"

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
long _Lf_; // leaf number found in the intersection SearchTree for current _Lv_

long threshold_xmin; 

LvBead*_Lv_;
VtBead*_Vt_; // last inserted vector; made global for debugging purposes.

char Lv_isInreverse;
char Vt_isInreverse;

char Lv_isVertical;
char Lv_isHorizontal;
char Lv_isOrtogonal;

double Lv_Y21;
double Lv_X21;
double Lv_D21;

char I_Type; // intersection type; made global for debugging purposes.

/*
 Helpers to establish the chain of added vectors when leaf is created or updated. 
 */
VtBead**Lf_VtChainFirst;
VtBead**Lf_VtChainLast;
unsigned short*Lf_AddedChains;
unsigned short Lf_AddedChainsLim;

/*
 vector SearchTree.
 */
VtBead**Lf_First; // a begin of a vector-chain for each leaf. 
unsigned short*Lf_Upper;
unsigned short*Lf_Lower;
long*Lf_Ymidl; // Y-value that splits the leaf in half.
char*Lf_Haschildrens;

VtBead**Lf_SetASideFirst; // a begin of a processed vector-chain (those that are below the current min-X) for each leaf
VtBead**Lf_SetASideLast; 

char Lf_Factor;

unsigned short LF_LIM;
unsigned short Tree_next_pos;

char hash_Initialized;

VtBead*__VT0__;
LbBead*__LB0__;

_IEvaluater IEvaluater;



extern long Delta_X;
extern long Delta_Y;


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

	ZLDLIM = 0;
	ZLBLIM = 0;
	ZLVLIM = 0;
	ZPTLIM = 0;
	ZVTLIM = 0;
	PTRAux_2Lim = 0;
	PTRAux_1Lim = 0;
	INDAux_3Lim = 0;
}
void _IEvaluater::__SetVT0LB0__(void)
{
	__VT0__ = GetVtBead(0);
	__LB0__ = GetLbBead(0);
	memset(__VT0__, 0, sizeof(VtBead)<<1);
	memset(__LB0__, 0, sizeof(LbBead));
}
_IEvaluater::_IEvaluater()
{
	ZPTHASH = new PtBead*[ZPBLIM+1];
	memset(ZPTHASH, 0, (ZPBLIM+1)*sizeof(PtBead*));
	ZPTHASH_Static = new PtBead*[ZPBLIM+1];
	memset(ZPTHASH_Static, 0, (ZPBLIM+1)*sizeof(PtBead*));

	LF_LIM = 128;
	Lf_Haschildrens = (char*)new long long*[LF_LIM*10*sizeof(long long*)];
	Lf_VtChainFirst = (VtBead**)(Lf_Haschildrens+LF_LIM);
	Lf_VtChainLast = Lf_VtChainFirst+LF_LIM;
	Lf_First = Lf_VtChainLast+LF_LIM;
	Lf_SetASideFirst = Lf_First+LF_LIM; 
	Lf_SetASideLast = Lf_SetASideFirst+LF_LIM; 
	Lf_Upper = (unsigned short*)(Lf_SetASideLast+LF_LIM);
	Lf_Lower = Lf_Upper+LF_LIM;
	Lf_AddedChains = Lf_Lower+LF_LIM;
	Lf_Ymidl = (long*)(Lf_AddedChains+LF_LIM);
	Lf_Factor = 1;
	while(LF_LIM>>Lf_Factor) ++Lf_Factor;
	--Lf_Factor;

	__SetVT0LB0__();
}
_IEvaluater::~_IEvaluater()
{
	delete ZPTHASH;
	delete ZPTHASH_Static;
	delete Lf_Haschildrens;

	_ReleaseStorage();
}
void _IEvaluater::_RestoreStaticConfiguration() { 
	for(unsigned short Lf = 0; Lf < LF_LIM; ++Lf) { 
		VtBead*Vt_Next = 0;
		VtBead*Vt_Last = 0;
		VtBead*Vt = Lf_First[Lf];
		while(Vt) { 
			Vt_Next = Vt->ZVTNextInTreeVtBead; 
			if(Vt->ZVTMeInreverse == 0) { 
				if(Vt_Last) { 
					Vt_Last->ZVTNextInTreeVtBead = Vt_Next; 
				} 
				else { 
					Lf_First[Lf] = Vt_Next; 
				} 
			} 
			else { 
				Vt_Last = Vt; 
			} 
			Vt = Vt_Next; 
		} 
	} 
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
		Lf_VtChainLast[I] = 0;
		Lf_First[I] = 0;
		Lf_SetASideFirst[I] = 0; 
		Lf_SetASideLast[I] = 0; 
		Lf_AddedChains[I] = 0;
	}
	if(!hash_Initialized) {
		for(I = 0; I<LF_LIM; ++I) {
			Lf_Upper[I] = 0;
			Lf_Lower[I] = 0;
		}
		Init_Vt_Findtree(PYMIN, PYMAX, 0, 1);
		hash_Initialized = 1;
	}
	Lf_AddedChainsLim = 0;
}
void Into_Lv_Chain(LvBead*Lv, VtBead*Vt)
{
	if(Lv->ZLVMyVector == Vt) 
	{ 
		return; 
	} 

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

	if(Lf_VtChainLast[Lf]==0)
	{
		Lf_VtChainLast[Lf] = Vt;
		Lf_AddedChains[Lf_AddedChainsLim++] = Lf;
	}

	Vt->ZVTNextInTreeVtBead = Lf_VtChainFirst[Lf];
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

	H_Insert(Lf, _Vt_ = Vt);
}
void Remove_Vt(unsigned short Lf, VtBead*Vt) {
	if(Vt_Last ==__VT0__) {
		Lf_First[Lf] = Vt->ZVTNextInTreeVtBead;
	}
	else { 
		Vt_Last->ZVTNextInTreeVtBead = Vt->ZVTNextInTreeVtBead; 	
	} 
	H_INSERT(Lf, Vt);
}
void Split_Vt(unsigned short Lf, VtBead*Vt, PtBead*Pt)
{
	H_INSERT(Lf, SPLITVT(Vt, Pt)); // adds new segment to inactive chain
	Remove_Vt(Lf, Vt); // adds old segment to inactive chain
}
char Find_Intersections(unsigned short Lf) // returns 1, if coincidence was found
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

	PtBead*Int_P = 0;
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

	memset(__VT0__, 0, sizeof(VtBead)<<1);

	Vt = *(Lf_First+Lf);
	for(Vt_Last = __VT0__; Vt; Vt_Last = Vt, Vt = Vt->ZVTNextInTreeVtBead) {
		__VT0__->ZVTNextInTreeVtBead = Vt->ZVTNextInTreeVtBead; 

		P3 = Vt->ZVTCO1;
		X3 = P3->ZPTCO1;

		if(X3>Lv_Xmax)
		{
			continue;
		}
		P4 = Vt->ZVTCO2;
		X4 = P4->ZPTCO1;

		if(X4<threshold_xmin) { 
			// This is where a vector gets removed from the search tree, 
			//  because all next Lv will be to the right of it. 
			// The removed vector is placed on a secondary tree. 
			// The secondary tree will be merged with the main tree when intersection analysis finishes. 
			if(Lf_SetASideLast[Lf]) { 
				Lf_SetASideLast[Lf]->ZVTNextInTreeVtBead = Vt; 
			} 
			else { 
				Lf_SetASideFirst[Lf] = Vt; 
			} 
			Lf_SetASideLast[Lf] = Vt; 

			if(Lf_First[Lf] == Vt) {
				Lf_First[Lf] = Vt->ZVTNextInTreeVtBead;
			} 

			Vt_Last->ZVTNextInTreeVtBead = Vt->ZVTNextInTreeVtBead;
			Vt->ZVTNextInTreeVtBead = 0; 

			Vt = Vt_Last; 
			continue;
		}
		if(X4<Lv_Xmin)
		{
			continue;
		}
		Y4 = P4->ZPTCO2;
		Y3 = P3->ZPTCO2;

		if(Y3>Y4) Vt_Ymin = Y4, Vt_Ymax = Y3;
		else Vt_Ymin = Y3, Vt_Ymax = Y4;

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
			if(Vt_isVertical&&Lv_isVertical)goto IntCoincidentVectors;
			if(Vt_isHorizontal&&Lv_isHorizontal)goto IntCoincidentVectors;
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
IntCoincidentVectors:
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
					Into_Lv_Chain(_Lv_, Vt); // exit point
					return 2; 
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
				Int_X = (long)floor((B1*C2-B2*C1)/DL +0.5 -0.5E-7);
				Int_Y = (long)floor((A2*C1-A1*C2)/DL +0.5 -0.5E-7);
				Int_P = IPOINT(Int_X, Int_Y);
				I_Type = 9;
			}
		}
		switch(I_Type)
		{
		case 1:
			Split_Vt(Lf, Vt, _P1_);

			Vt = Vt_Last;

			continue;
		case 2:
			Split_Vt(Lf, Vt, _P2_);

			Vt = Vt_Last;

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

			Vt = Vt_Last; 

			SPLITLV(_Lv_, Int_P);
			_P2_ = Int_P;
			_X2_ = Int_X;
			_Y2_ = Int_Y;
			break;

		case 9:
			if(Int_P!=P3&&Int_P!=P4)
			{
				Split_Vt(Lf, Vt, Int_P);

				Vt = Vt_Last;
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

		case 8: { 
				LvBead*Ls;
				VtBead*Vs;
				VtBead*Vt_S[3];
				long Vt_L = 0;

				S3 = (Delta_Y = Lv_Ymax-Lv_Ymin) > (Delta_X = Lv_Xmax-Lv_Xmin);

				S1 = S3? Y4 < Y3: X4 < X3;
				S2 = S3? _Y2_<_Y1_: _X2_<_X1_;

				S1 = S1!=S2;
				if(S1) {
					PtBead*P0=P3; P3=P4; P4=P0;
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
						Vt_S[Vt_L++] = Vs = SPLITVT(Vs, _P1_);
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
						Vt_S[Vt_L++] = VT = SPLITVT(Vs, _P2_);
						if(S1)Vs = VT;
					}
				}
				if(S1)Vs = Vs->ZVTMeInreverse;
				Into_Lv_Chain(Ls, Vs);

				if(Vt_L)
				{
					while(Vt_L--) { 
						H_INSERT(Lf, Vt_S[Vt_L]); 
					} 

					Remove_Vt(Lf, Vt);
					Vt = Vt_Last;
				}
				if(_Lv_==Ls)
				{
					return 1; // exit point
				}
				_P2_ = _Lv_->ZLVPoint2;
				_X2_ = _P2_->ZPTCO1;
				_Y2_ = _P2_->ZPTCO2; 
			}
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
	char rc = 0; 
	if(Lf_First[Lf])
	{
		if((rc = Find_Intersections(Lf))) 
		{ 
			return rc;
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
		if((rc = TreeIntersect_Lv(Lf_Lower[Lf]))) 
		{ 
			return rc;
		}
	}
	if(Lv_Ymax>=Lf_Ymidl[Lf])
	{
		if((rc = TreeIntersect_Lv(Lf_Upper[Lf]))) 
		{ 
			return rc;
		} 
	}
	return 0;
}
void Initiate_Intersect_Lv(LvBead*Lv) { 
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
} 
void Intersect_Lv(LvBead*Lv)
{
	VtBead*V3;
	VtBead*V4;

	Initiate_Intersect_Lv(Lv); 

	int done = 0;
	if((done = TreeIntersect_Lv(0))) {
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

	H_Insert((unsigned short)(_Lf_< 0? 0: _Lf_), _Vt_);
}
void Lf_MergeChains() { 
	for(long j = 0; j<Lf_AddedChainsLim; ++j)
	{
		long Lf = Lf_AddedChains[j];

		Lf_VtChainLast[Lf]->ZVTNextInTreeVtBead = Lf_First[Lf];
		Lf_First[Lf] = Lf_VtChainFirst[Lf];

		Lf_VtChainLast[Lf] = Lf_VtChainFirst[Lf] = 0;
	}

	Lf_AddedChainsLim = 0;
} 
void INTERSECTION_ANALYSIS(bool preloaded, int firts_lb)
{
	if(!preloaded) { 
		Init_Vt_FindTree();
	} 

	Lv_Sort(firts_lb); // sorts LvBeads into PTRAux_1. 

	for(long Lvsorted = 1; Lvsorted<PTRAux_1Lim; ++Lvsorted)
	{
		LvBead*Lv = (LvBead*)PTRAux_1[Lvsorted];
		LvBead*LVNext = Lv->ZLVNextLvBead;

		PtBead*P1 = Lv->ZLVPoint1;
		PtBead*P2 = Lv->ZLVPoint2;

		long X1 = P1->ZPTCO1;
		long X2 = P2->ZPTCO1;

		threshold_xmin = X1<X2? X1: X2;

		do
		{
			Intersect_Lv(Lv);
			Lv = Lv->ZLVNextLvBead;
		}
		while(Lv!=LVNext);

		Lf_MergeChains();

//		FileLog(itostdstring(ZVTLIM) + "\r\n"); 
	}

	for(int Lf = 0; Lf < LF_LIM; ++Lf) { 
		if(Lf_SetASideFirst[Lf]) { 
			Lf_SetASideLast[Lf]->ZVTNextInTreeVtBead = Lf_First[Lf]; 
			Lf_First[Lf] = Lf_SetASideFirst[Lf]; 

			Lf_SetASideFirst[Lf] = Lf_SetASideLast[Lf] = 0; 
		} 
	} 

//	FileLog(itostdstring(ZVTLIM) + "\r\n", true); 
}
