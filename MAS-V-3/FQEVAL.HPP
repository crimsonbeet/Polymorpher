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
extern char Lf_Factor;
extern unsigned short Lf_Lim;
extern unsigned short LF_LIM;
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
#define FETCHSUCCESSOR(VT) ((VT)->ZVTNxt?(VT)->ZVTNxt:(VT)->ZVTCO1->ZPTCO3)
#define _EQ_(VA,VL,EN) (((VA)>(VL)-(EN))&&((VA)<(VL)+(EN)))
#define FetchReverseLv(LV) ((LV)->ZLVCO2->ZVTCO6->ZVTCO5)
/**/
extern void(*zAbort_proc)(void);
void zAbort(char*ids, long lin);
PtBead*__GPT__(long Pt);
LvBead*__GLV__(long Lv);
LbBead*__GLB__(long Lb);
VtBead*__GVT__(long Vt);
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
void INTERSECTION_ANALYSIS(void);
void POINT_VECTOR_ANALYSIS(long first);
void DUMMYJUNCTIONANALYSIS(LbBead*LB);
void Check_Closure(void);
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
LbBead*Loadmaster(void);
void SET_INSIDE_ALL_1(void);
void POINT_VECTOR_CLASS_1(LbBead*LB);
void POINT_VECTOR_CLASS_2(LbBead*LB, long SX, long SY);
void Classify_Main(void);
void Set_Rstat(LbBead*I, char or);
void __INI_LvVt__(void);
#endif
