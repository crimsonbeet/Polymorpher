/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#ifndef _EVALUATER_
#define _EVALUATER_ 1
//#include "tom_aray.hpp"
#include "MemoryAllocator.h"
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
	PtBead*ZPTNxt;/*next Pt Bead in the hash class.*/

	PtBead*ZPTAux;/*auxiliary pointer used by scale functions.*/
};
struct VtBead
{
	char ZVTAux;/*traversal status.*/
	unsigned char ZVTStatus;/*color of the area to the right of vector.*/
	long ZVTPos;/*pos in ZVtTBl.*/

	PtBead*ZVTCO1;/*first point.*/
	PtBead*ZVTCO2;/*second point.*/
	LvBead*ZVTFirstCoincident;/*FIRST POLYGON VECTOR BEAD CHAIN OF COINCIDENT VECTORS.*/
	VtBead*ZVTMeInreverse;/*ASSOCIATED VECTOR Of REVERSE DIRECTION.*/

	VtBead*ZVTNextSortedVtBead;/*POINTER TO NEXT Vt BEAD FOR THIS POINT(COUNTERCLOCKWISE) (CHAIN HEAD IS ZPTFirstSortedVtBead).*/
	VtBead*ZVTNextInTreeVtBead; 
};
struct LvBead
{
	LvBead*ZLVNextLvBead;/*NEXT BEAD OF POLYGON VECTOR(LV) (CHAIN HEAD IS ZLBFirstLvBead) (CIRCULAR)*/
	VtBead*ZLVMyVector;/*associated Vt VECTOR.*/
	LvBead*ZLVNextConcident;/*NEXT COINCIDENT VECTOR PTR(CHAIN HEAD IS ZVTFirstCoincident).*/
	PtBead*ZLVPoint1;/*first point.*/
	PtBead*ZLVPoint2;/*second point.*/
	LbBead*ZLVMyLbBead;/*pointer to polygon bead.*/
	LbBead*ZLVAux;/*auxiliary pointer.*/
};
struct LbBead // represents a polygon
{
	unsigned long ZLBLayer; // layer number
	long ZLBStatus;
	LvBead*ZLBFirstLvBead; /*POINTER TO THE FIRST VECTOR BEAD.*//*will point to the vector with lowest left point if is not an open shape*/
	PtBead*ZLBXym; /*lower left point pointer.*/
	LbBead*ZLBNxt; /*next in class.*/
	LbBead*ZLBGrp; /*pointer to the first in class.*/
};
struct _zAvtbl: public XmemoryAllocator<VtBead>
{ 
};
struct _zAltbl: public XmemoryAllocator<LvBead>
{
};
struct _zAbtbl: public XmemoryAllocator<LbBead>
{
};
struct _zAptbl: public XmemoryAllocator<PtBead>
{
};
struct F_SIZE_JOB
{
	char Name[20];
	long Size;
	char Sign;
	int Do_log_graph; 
};
class F_EVAL_JOB
{
public:
	char O_name[20];
	char I_name[4][20];
	long Function;

	char classifyType; 

	int ProcessArguments(short N, const char*A[]); 
	void PerformFunction(int log_graph = 0); 

	F_EVAL_JOB(short N, const char*A[], int log_graph = 0);
	F_EVAL_JOB(); 
};
class _IEvaluater
{
public:
	_IEvaluater();
	~_IEvaluater();
	void __SetVT0LB0__();
	void _ReleaseStorage();
	void _RestoreStaticConfiguration(); 
};
extern _IEvaluater IEvaluater;
/**/
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
extern unsigned long LYCODE[16]; /*LAYER-CODE*/
extern unsigned long LYMASK[16]; /*LAYER-MASKA*/
/**/
extern long Target_Function; /*FUNCTION-CODE*/
/**/
extern long PXMAX; // used by grid, size module
extern long PYMAX;
extern long PXMIN;
extern long PYMIN;
/**/
extern char PV_Error;
/**/
extern PtBead**ZPTHASH;
extern PtBead**ZPTHASH_Static; 
extern long ZPBLIM;
/**/
extern long STATUS; // current polygon's status: +1 - counterclockwise, -1 - clockwise. 
extern unsigned long C_Layer; // current layer code.
/**/
extern VtBead*__VT0__;
extern LbBead*__LB0__;
/**/
#define MFunction_OverlapElim 0
#define MFunction_AndNot 1
#define MFunction_And 2
#define MFunction_Xor 3
#define MFunction_Or 4
#define MFunction_Not 5
#define MFunction_Area 6
#define MFunction_Size 7
#define MFunction_ActivateLayer 8
#define MFunction_Intersect 9
#define MFunction_Nothing 10
#define MFunction_5009 11
/**/
#define MFunction_AndOne -1
#define MFunction_XorOne -2
/**/
#define FETCHSUCCESSOR(VT) ((VT)->ZVTNextSortedVtBead?(VT)->ZVTNextSortedVtBead:(VT)->ZVTCO1->ZPTFirstSortedVtBead)
#define _EQ_(VA,VL,EN) (((VA)>(VL)-(EN))&&((VA)<(VL)+(EN)))
LvBead* FetchReverseLv(LvBead* LV); 
/**/
typedef LvBead*compsplitlv(LvBead*, VtBead*, VtBead*, PtBead*);
extern compsplitlv*COMPSPLITLV;
/**/
typedef LvBead*splitlv(LvBead*, PtBead*);
extern splitlv*SPLITLV;
/**/
extern void(*zabort_proc)(void);
void zabort(const char*ids, long lin);
void zabort(void);
void MASLog(const char * format, ...); 
void FinishedNormally(void);
void __FINISHEDNORMALLY__(void); 
PtBead*GetPtBead(long Pt);
LvBead*GetLvBead(long Lv);
LbBead*GetLbBead(long Lb);
VtBead*GetVtBead(long Vt);
char VtIsEven(VtBead*Vt);
void GetVtPair(VtBead*&V3, VtBead*&V4);
typedef PtBead*ipoint(long, long);
extern ipoint*IPOINT;
PtBead*IPOINT0(long X, long Y);
PtBead*IPOINT4(long X, long Y);
PtBead*IPOINT5(long X, long Y);
void l_swap(long&A, long&B);
void COMBINE_C_POLYGONS(void);
void CleanZPTHASH();
void Relink_Lv_Pt(VtBead*v); 
double Eval_Angle(double DX, double DY);
void REMOVE_FROM_VT_CHAIN(LvBead*LV);
LvBead*CLEAR_LB_PTR(LvBead*Lv, LvBead*Ln)/*From Lv To Ln*/; 
double COMPLVANGLE(LvBead*L);
double COMPLDANGLE(long L);
double COMPANGLE(long*x, long*y, long L);
long DETERMINE_CLOCKSTATUS(LbBead*B, double*A = nullptr);
long DETERMINE_CLOCKSTATUS(_sAlong& x, _sAlong& y, long lim, double* A = nullptr);
VtBead*SPLITVT(VtBead*V3, PtBead*IP);
VtBead*INSERTV(PtBead*P1, PtBead*P2, LvBead*LV);
void Classify_Polygons(char classifyType = 0); 
void INTERSECTION_ANALYSIS(bool preloaded = false, int first_lb = 1);
char POINT_VECTOR_ANALYSIS(long first, char F_EVAL_INPROGRESS = 1, char F_FULLANALYSIS_INPROGRESS = 0);
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
bool FORCE_C_POLYGONS(char F_EVAL_INPROGRESS = 1);
long LbBead_Unload2InputXY(LbBead*LB, char run_linearizer = 1, int log_graph = 0);
LbBead*Loadmaster(char check_closure = 0, char open_shape = 0);
char POINT_VECTOR_CLASS_1(LbBead*LB);
void POINT_VECTOR_CLASS_2(LbBead*LB, long LayerCode, long MaskCode/*when and or xor on itself*/);
void Classify_Main(void);
void Set_Rstat(LbBead*I, char call_flavor);
void F_AREA_MAIN(const char* Name);

void LFuInitialize(const char*outLayer_name);
int LFuEvaluater(long Function, int log_graph = 0, char classifyType = 0/*0 - main, 1 - quick*/);
bool LFuEvaluaterLoop(long Function, int log_graph = 0, char classifyType = 0);
void SetNxtFrag(long Function);

LvBead* FETCHREVERSE(LbBead* LB, LvBead* LV);

void F_SIZE_SHAPES(F_SIZE_JOB& JOB);
void F_SIZE_COLLARS(F_SIZE_JOB& JOB);

void linearizeContour(std::vector<long>& x, std::vector<long>& y, double stepSize, const size_t maxSegmentSize); // external



#endif
