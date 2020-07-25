#ifndef _EVALUATER_
#define _EVALUATER_ 1
#include "tom_aray.hpp"
#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif
#ifndef LONG_MAX
#define LONG_MAX ((1l<<30)+((1l<<30)-1))
#endif
/**/
struct VtBead;
struct LvBead;
struct LbBead;
struct PtBead
{
	long ZPTCO1;/*x*/
	long ZPTCO2;/*y*/

	VtBead*ZPTFirstSortedVtBead;/*first Vt bead for this point(chain head).*/
	PtBead*ZPTNxt;/*second Pt Bead in the hash class.*/

	PtBead*ZPTAux;/*auxiliary pointer used by scale functions.*/
}	;
struct VtBead
{
	char ZVTAux;/*traversal status.*/
	long ZVTPos;/*pos in ZVtTBl.*/
	unsigned char ZVTStatus;/*right status.*/

	PtBead*ZVTCO1;/*first point.*/
	PtBead*ZVTCO2;/*second point.*/
	LvBead*ZVTFirstCoincident;/*FIRST POLYGON VECTOR BEAD CHAIN OF COINCIDENT VECTORS.*/
	VtBead*ZVTMeInreverse;/*ASSOCIATED VECTOR Of REVERSE DIRECTION.*/

	VtBead*ZVTNextSortedVtBead;/*POINTER TO NEXT Vt BEAD FOR THIS POINT(COUNTERCLOCKWISE) (CHAIN HEAD IS ZPTFirstSortedVtBead).*/
}	;
struct LvBead
{
	LvBead*ZLVNextLvBead;/*NEXT BEAD OF POLYGON VECTOR(LV) (CHAIN HEAD IS ZLBFirstLvBead!CIRCULAR)*/
	VtBead*ZLVMyVector;/*associated Vt VECTOR.*/
	LvBead*ZLVNextConcident;/*NEXT COINCIDENT VECTOR PTR(CHAIN HEAD IS ZVTFirstCoincident).*/
	PtBead*ZLVPoint1;/*first point.*/
	PtBead*ZLVPoint2;/*second point.*/
	LbBead*ZLVMyLbBead;/*pointer to polygon bead.*/
	LbBead*ZLVAux;/*auxiliary pointer.*/
}	;
struct LbBead // represents a polygon
{
	unsigned char ZLBCO4; // layer number
	char ZLBCO1;
	LvBead*ZLBFirstLvBead; /*POINTER TO THE FIRST VECTOR BEAD.*//*will point to the vector with lowest left point if is not an open shape*/
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
class _IEvaluater
{
public:
      _IEvaluater();
      ~_IEvaluater();
      void __SetVT0LB0__(void);
      void _ReleaseStorage(void);
}     ;
extern _IEvaluater IEvaluater;
/**/
extern char __GrpNONE__;
extern long S_Factor;
extern long S_FACTOR;
/**/
extern _sAlong ZLDCO0; /*X*/
extern _sAlong ZLDCO1; /*Y*/
extern _sAlong ZLDCO2; /*X*/
extern _sAlong ZLDCO3; /*Y*/
extern _sAlong ZLDCO4; /*X*/
extern _sAlong ZLDCO5; /*Y*/
extern _zAvoid PTRAux_1;
extern _zAvoid PTRAux_2;
extern _sAlong INDAux_3;
extern _zAvtbl ZVtTBl;
extern _zAltbl ZLvTBl;
extern _zAbtbl ZLbTBl;
extern _zAptbl ZPtTBl;
/**/
extern long ZLDLIM;
extern long ZLBLIM;
extern long ZLVLIM;
extern long ZPTLIM;
extern long ZVTLIM;
extern long PTRAux_2Lim;
extern long PTRAux_1Lim;
extern long INDAux_3Lim;
/**/
extern long LYCODE[10]; /*LAYER-CODE*/
extern long LYMASK[10]; /*LAYER-MASKA*/
/**/
extern long Target_Function; /*FUNCTION-CODE*/
#define OVRELIM 0
#define ZANDNOT 1
#define ZZZANDJ 2
#define ZZZXORJ 3
#define ZZZZORJ 4
#define ZZZNOTJ 5
#define ZZZSIZE 6
#define ZZZ5009 7
#define ZZZZ559 8
#define ZOTELLO 9
#define ZZZPDIF 10
#define ZZZ5089 11
#define ZZZAREA 12
#define ZZDUMMY 14

#define ZZZAND1 -1
#define ZZZXOR1 -2
/**/
extern char SI_Error;
extern char PV_Error;
/**/
extern PtBead**ZPBCOR;
extern long ZPBLIM;
extern long __ZPBLIM__;
extern unsigned short PBLIM;
extern unsigned short PXCNT;
extern unsigned short PYCNT;
extern long XPDIV;
extern long YPDIV;
extern long PXMAX;
extern long PYMAX;
extern long PXMIN;
extern long PYMIN;
/**/
extern char Closed_Polygon;
/**/
extern long Delta_X_2;
extern long Delta_Y_1;
extern long Delta_Y_2;
extern long Delta_X;
extern long Delta_Y;
/**/
extern long Sized_X;
extern long Sized_Y;
/**/
extern char S_Free;
extern long STATUS;
extern long C_Layer;
extern char DummyClip;
extern char Open_Shape;
extern char BuildSLine;
/**/
extern char M_CONV_MODULE;
extern char F_EVAL_MODULE;
extern char F_INIT_MODULE;
/**/
extern char ComplexIntersectionAllowed;
extern char RepeatComplexIntersection;
/**/
#define FETCHSUCCESSOR(VT) ((VT)->ZVTNextSortedVtBead?(VT)->ZVTNextSortedVtBead:(VT)->ZVTCO1->ZPTFirstSortedVtBead)
#define _EQ_(VA,VL,EN) (((VA)>(VL)-(EN))&&((VA)<(VL)+(EN)))
#define FetchReverseLv(LV) ((LV)->ZLVMyVector->ZVTMeInreverse->ZVTFirstCoincident)
/**/
typedef LvBead*compsplitlv(LvBead*, VtBead*, VtBead*, PtBead*);
typedef LvBead*splitlv(LvBead*, PtBead*);
/**/
extern compsplitlv*COMPSPLITLV;
extern splitlv*SPLITLV;
/**/
typedef void mergelvchain(VtBead*, VtBead*);
/**/
extern mergelvchain*MERGELVCHAIN;
/**/
extern void(*zAbort_proc)(void);
void zAbort(char*ids, long lin);
PtBead*GetPtBead(long Pt);
LvBead*GetLvBead(long Lv);
LbBead*GetLbBead(long Lb);
VtBead*GetVtBead(long Vt);
extern VtBead*__VT0__;
extern LbBead*__LB0__;
char VtIsEven(VtBead*Vt);
void GetVtPair(VtBead*&V3, VtBead*&V4);
typedef PtBead*ipoint(long, long);
extern ipoint*IPOINT;
PtBead*IPOINT0(long X, long Y);
PtBead*IPOINT4(long X, long Y);
PtBead*IPOINT5(long X, long Y);
void l_swap(long&A, long&B);
void COMBINE_C_POLYGONS(void);
void CleanZPBCOR(unsigned short Cnt);
double Eval_Angle(double DX, double DY);
void REMOVE_FROM_VT_CHAIN(LvBead*LV);
double COMPLVANGLE(LvBead*L);
double COMPLDANGLE(long L);
double COMPANGLE(long*x, long*y, long L);
long DETERMINE_STAT_1(void);
long DETERMINE_STAT_2(LbBead*B);
long DETERMINE_STAT_3(long*x, long*y, long lim);
VtBead*SPLITVT(VtBead*V3, PtBead*IP);
VtBead*INSERTV(PtBead*P1, PtBead*P2, LvBead*LV);
void Classify_Polygons(char quickclassify); 
void INTERSECTION_ANALYSIS(void);
void POINT_VECTOR_ANALYSIS(long first);
void DUMMYJUNCTIONANALYSIS(LbBead*LB);
void LDMake_ClosedPolygon(void);
typedef char compare(unsigned char);
char XOR_1(unsigned char S);
char AND_1(unsigned char S);
char AN_D(unsigned char S);
char O_R(unsigned char S);
char AND_NOT(unsigned char S);
char OVR_ELIM(unsigned char S);
char X_OR(unsigned char S);
char NO_T(unsigned char S);
extern compare *True_Stat;
extern compare *Cmp_Modules[6];
void FORCE_C_POLYGONS(void);
long __UNL__(LbBead*LB);
LbBead*Loadmaster(char check_closure = 0);
void SET_INSIDE_ALL_1(void);
void POINT_VECTOR_CLASS_1(LbBead*LB);
void POINT_VECTOR_CLASS_2(LbBead*LB, long SX, long SY);
void Classify_Main(void);
void Set_Rstat(LbBead*I, char or);

#endif
