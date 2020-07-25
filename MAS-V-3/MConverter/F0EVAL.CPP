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
#endif
/**/
#include<LIMITS.H>
#include<PROCESS.H>


#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510
#endif

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
long*ZLDCO0; /*X-COORDINATE*/
long*ZLDCO1; /*Y-COORDINATE*/
long*ZLDCO2; /*X-COORDINATE*/
long*ZLDCO3; /*Y-COORDINATE*/
long*ZPTCO1; /*X-COORDINATE*/
long*ZPTCO2; /*Y-COORDINATE*/
char*ZLBCO7; /*ALREADY-CLASSIFIED-FLAG*/
CHAR*ZPVCO3; /*PSEUDO-SLOPE*/
char*ZVTCO8; /*TRAVERSAL-STATUS*/
char*ZLBCO1; /*POLYGON-RIGHT/LEFT-STATUS*/
CHAR*ZVTCO4; /*RIGHT-STATUS*/
CHAR*ZLBCO4; /*LAYER-CODE*/
SHORT*ZLbXx1; /*MIN-POINT-ON-POLYGON(AFTER-LOAD-MASTER)*/
SHORT*ZLbXx2; /*MAX-X-POINT-ON-POLYGON*/
SHORT*ZLbYy1; /*MIN-Y-POINT-ON-POLYGON*/
SHORT*ZLbYy2; /*MAX-Y-POINT-ON-POLYGON*/
SHORT*ZLBCO3; /*POINTER-TO-THE-FIRST-VECTOR-BEAD*/
SHORT*ZLBCO6; /*POLYGON-NUMBER*/
SHORT*ZLbNxt; /*NEXT-HASH-POLYGON*/
SHORT*ZLVCO1; /*NEXT-POLYGON-VECTOR(LV)-BEAD(CHAIN HEAD IS ZLBCO3!CIRCULAR)*/
SHORT*ZLVCO2; /*VECTOR-NUMBER*/
SHORT*ZLVCO3; /*NEXT-COINCIDENT-VECTOR-PTR(CHAIN HEAD IS ZVTCO5)*/
SHORT*ZLVCO5; /*FIRST-POINT*/
SHORT*ZLVCO6; /*SECOND-POINT*/
SHORT*ZLVCO7; /*PTR-TO-POLYGON-BEAD*/
SHORT*ZLVCO8; /*PTR-TO-CLASS-POLYGON*/
SHORT*ZPVCO5; /*POINTER-TO-NEXT-PV-BEAD-FOR-THIS-POINT(COUNTERCLOCKWISE)*/
SHORT*ZPTCO3; /*PTR-TO-PV-BEAD(CHAIN HEAD)*/
SHORT*ZPTCO5; /*NEXT-X-POINTER-IN-Y-CHAIN*/
SHORT*ZVTCO1; /*FIRST-POINT-NUMBER*/
SHORT*ZVTCO2; /*SECOND-POINT-NUMBER*/
SHORT*ZVTCO5; /*PTR-TO-FIRST-POLYGON-VECTOR-BEAD-CHAIN-OF-COINCIDENT-VECTORS(IS NEVER ZERO)*/
SHORT*ZVTCO6; /*ASSOCIATED-VECTOR(REVERSE DIRECTION)*/
SHORT*ZJSCO0; /*LV-WILL-BE-PROCESSED-BY-SET-INSIDE*/
SHORT*ZJSCO1; /*ENCLOSED-LB*/

long**LLDCOR[3] = {&ZLDCO0, &ZLDCO1, NULL};
long**Lldcor[3] = {&ZLDCO2, &ZLDCO3, NULL};
long**LPTCOR[3] = {&ZPTCO1, &ZPTCO2, NULL};

char**CVTCOR[3] = {(char**)&ZVTCO4, &ZVTCO8, NULL};
char**CLBCOR[4] = {&ZLBCO1, (char**)&ZLBCO4, &ZLBCO7, NULL};

SHORT**ILBCOR[9] = {&ZLbXx1, &ZLBCO3, &ZLBCO6, &ZLbXx2, &ZLbYy1, &ZLbYy2, &ZLbNxt, NULL};
SHORT**ILVCOR[8] = {&ZLVCO1, &ZLVCO2, &ZLVCO3, &ZLVCO5, &ZLVCO6, &ZLVCO7, &ZLVCO8, NULL};
SHORT**IVTCOR[7] = {&ZVTCO1, &ZVTCO2, &ZVTCO5, &ZVTCO6, &ZPVCO5, NULL};
SHORT**IPTCOR[3] = {&ZPTCO3, &ZPTCO5, NULL};
SHORT**IJSCOR[3] = {&ZJSCO0, NULL};
/**/
long ZLSLIM;
long ZLDLIM;
long ZLDMAX;
long ZLBLIM;
long ZLBMAX;
long ZLVLIM;
long ZLVMAX;
long ZPTLIM;
long ZPTMAX;
long ZYPLIM;
long ZYPMAX;
long ZPVLIM;
long ZPVMAX;
long ZVTLIM;
long ZVTMAX;
long ZJSLIM;
long ZJSMAX;
/**/
#ifdef M_CONV_MODULE
#define Z_LD_Wt 0.8
#define Z_LB_Wt 0.5
#define Z_LV_Wt 1.0
#define Z_VT_Wt 2.0
#define Z_PT_Wt 1.0
#define Z_JS_Wt 0.1
#define Z_gV_Wt 1.0
#define Z_VX_Wt 0.8
#else
#define Z_LD_Wt 0.5
#define Z_LB_Wt 0.5
#define Z_LV_Wt 2.3
#define Z_VT_Wt 4.0
#define Z_PT_Wt 1.7
#define Z_JS_Wt 0.1
#define Z_gV_Wt 2.3
#define Z_VX_Wt 0.5
#endif

#define Z_gV_Sz 6 *Z_gV_Wt
#define Z_LD_Sz 8 *Z_LD_Wt
#define Z_VX_Sz 32*Z_VX_Wt
#define Z_LB_Sz 3 *Z_LB_Wt + 14*Z_LB_Wt
#define Z_LV_Sz 14*Z_LV_Wt
#define Z_VT_Sz 2 *Z_VT_Wt + 10*Z_VT_Wt
#define Z_PT_Sz 8 *Z_PT_Wt + 4 *Z_PT_Wt
#define Z_JS_Sz 2 *Z_JS_Wt
/**/
long LYCODE[10]; /*LAYER-CODE*/
long LYMASK[10]; /*LAYER-MASKA*/
/**/
long Target_Function; /*FUNCTION-CODE*/
#define OVRELIM 0
#define ZANDNOT 1
#define ZZZANDJ 2
#define ZZZXORJ 3
#define ZZZZORJ 4
#define ZZZNOTJ 5

#define ZZZAND1 -1
#define ZZZXOR1 -2
/**/
long SI_LB, SI_V1, SI_V2, SI_ST;
long SI_LE;
char SI_FL, SI_IX_FL, SI_PA_FL;
/**/
char SI_Error;
char PV_Error;
/**/
long UN001;
long UN002;
long EMSIZ;
long LVCNT;
long BLCNT;
long UBSIZ;
long XBSIZ;
long YBSIZ;
long XBCNT; /*TOTAL-BLOCKS-IN-X-DIRECTION*/
long YBCNT; /*TOTAL-BLOCKS-IN-Y-DIRECTION*/
long CXMIN; /*MIN-X-COORD-ON-CHIP*/
long CXMAX; /*MAX-X-COORD*/
long CYMIN; /*MIN-Y-COORD-ON-CHIP*/
long CYMAX; /*MAX-Y-COORD*/
/**/
double ZZ_DELTA_X;
double ZZ_DELTA_Y;
/**/
double STSIZ;
/**/
char S_Free;
long STATUS;
long C_Layer;
long Z_JFlag;
long S_Factor;
long ZZ_RP_KB;
char DummyClip;
long Z_Fragment;
char Ia_Complex;
long Z_Full_Scan;
long Z_User_Block;
long Z_Layers_Cnt;
char Z_Open_Shape;
char Dix_Rewrite_FP_S;
char Roll_Back_Intsect;
long Zptlim_After_Loadmaster;
/**/
SHORT*ZPBCOR;
long PBLIM;
long PXCNT;
long PYCNT;
long XPDIV;
long YPDIV;
long PXMAX;
long PYMAX;
long PXMIN;
long PYMIN;
/**/
char Ia_Flag;
char Without_scalling;
/**/
long Lb_Ymin;
long Lb_Ymax;

long Lv_Ymin;
long Lv_Ymax;
long Lv_Xmin;
long Lv_Xmax;
long Lv_This;

long Vt_Ymax;
long Vt_Ymin;
long Vt_Last;
long Vt_Auxx;

long Lv_Pt1;
long Lv_Pt2;
long Lv_Xx1;
long Lv_Yy1;
long Lv_Xx2;
long Lv_Yy2;

char Lv_Swi;
char Vt_Swi;

double Lv_Y21;
double Lv_X21;

double Lv_De21;
/**/
char Lv_Ins;
char Lv_Spl;
char Int_Type;
/**/
char Closed_Polygon;
/**/
long Delta_X;
long Delta_Y;
/**/
long Sized_X;
long Sized_Y;
/**/
SHORT*Lf_Rolld;
SHORT*Lf_ROLLD;
SHORT*Lf_First;
SHORT*Lf_Stack;
SHORT*Lf_Upper;
SHORT*Lf_Lower;

long*Lf_Ymidl;
char*Lf_Sdown;
char*Lf_Flags;

char Lf_Factor;

SHORT Lf_Lim;
SHORT LF_LIM;
SHORT Tree_next_pos;

char Initialized_hash;
/**/
long Sh_Xmin;
long Sh_Xmax;
long Sh_Ymin;
long Sh_Ymax;

long Lv_Ptr;
long Vt_Ptr;
long Lf_Ptr;
/**/
long DJ_REVERSE_LV;
long DJ_FIRSTFR_LV;
/**/
long DIX_Fragment;
long DIX_OpenS;
long DIX_Layer;
long Dix_Layer;

char*Dix_Out_Buf;
long Dix_Out_Pos;
long Dix_Out_Len;

FILE*Dix_File;
char*Dix_Name;
/**/
char*Zd_File_Buf;
long Zd_File_Ptr;
long Zd_File_Eof;
long Zd_File_Lim;

FILE*Zd_File;
long*Zd_Fragment = &DIX_Fragment;
long*Zd_Layer = &Dix_Layer;

long Zd_Delta_X;
long Zd_Delta_Y;
/**/
typedef struct
{
char Id_String[32];

long Delta_X; /*In Units*/
long Delta_Y; /*In Units*/
} FP_Struct;
typedef struct
{
long EMSIZ; /*In PAGES BY 1K*/
long UN001; /*Units_Per_Micron*/
long UN003; /*Delta Window In Units*/
long CXMIN; /*In Units*/
long CXMAX; /*In Units*/
long CYMIN; /*In Units*/
long CYMAX; /*In Units*/
long XBSIZ; /*In Units*/
long YBSIZ; /*In Units*/
long XBCNT;
long YBCNT;
long LVCNT; /*Vectors Count on Block*/
long FiLID;
char FileP[4];
char Err1P[4];
char PlotP[4];
char TempP[4];
} LZ_Struct;
/**/
typedef struct
{
long STATUS;

long CXMIN;
long CXMAX;
long CYMIN;
long CYMAX;

long XBCNT;
long YBCNT;
long XBSIZ;
long YBSIZ;

long ER1I;
long PLTI;
long BDBI;
long PDBI;
} LX_Struct;
typedef struct
{
FILE*Fil;
char*Buf;
long Buf_Len;
long Buf_Pos;
char Buf_Eof;
} LF_Struct;
typedef struct
{
long Fragment;

LX_Struct LX_S;
LF_Struct File;

char Name[20];
char PdbN[20];
char BdbN[20];
char Er1N[20];
char PltN[20];

long Delta_X;
long Delta_Y;

long Lcode;
long Lmask;

char Exist;

FILE*ER1;
FILE*ER2;
} FX_Struct;

LZ_Struct LZ_S;
LX_Struct LX_S;
FX_Struct Z_Layer[4], O_Layer;

FP_Struct FP_S = {"Mascot V2.0 Internal Storage\n", 0, 0};

fpos_t FPOS_1 = sizeof(LZ_S)+sizeof(FP_S);
fpos_t FPOS_2 = sizeof(FP_S);
fpos_t FPOS_3;

char LZ_S_Changed;

FILE*LZ_S_FILE;
FILE*LX_S_FILE;
char*LZ_S_NAME;
char*LX_S_NAME;
/**/
long S_BPOS;
long S_BLEN;
char*S_BTOP;
/**/
#define FETCHSUCCESSOR(VT) (ZPVCO5[(VT)]?ZPVCO5[(VT)]:ZPTCO3[ZVTCO1[(VT)]])
#define ENVEQ(VA,VL,EN) (((VA)>(VL)-(EN))&&((VA)<(VL)+(EN)))
#define zznoeven(V) ((V)-(((V)>>1)<<1))
#define FetchReverseLv(LV) (ZVTCO5[ZVTCO6[ZLVCO2[(LV)]]])
/**/
void Zswab(long * A, long * B)
{
long T = *A; *A = *B; *B = T;
}
void ZSWAB(long*A, long*B)
{
long T = *A; *A = *B; *B = T;
}
FILE*ERRFIL;
char*ERRNAM;
void bad_poly_out(char closure, char*s);
void zabort(void);
void Print_Note_Message(void)
{
fprintf(stderr, "\n");
}
void
ZOUTABORT(void)
{
	Print_Note_Message();
	fprintf(stderr, "\nOUT OF STORAGE");
	Print_Note_Message();
#ifdef F_EVAL_MODULE
	fprintf(stderr, "\nNOTE!Try to CHANGE BLOCKS COUNT");
#endif
	zabort();
}
void
ZALLOCATE(void**XX, long L)
{
	long I[2] = {0, 0};
	if(((*XX)=malloc(L))==NULL)
	{
	Print_Note_Message();
	fprintf(stderr,"\nOUT OF MEMORY");
	Print_Note_Message();
	zabort();
	}
	if(L>8)L = 8;
	memcpy((*XX), I, L);
}
void Z_FREE(void**XX)
{
if(*XX)free(*XX);
}
void ZXXFREE(void**XX[])
{
	long I=0;
	while(XX[I]!=NULL) Z_FREE(XX[I++]);
}
#ifdef __BORLANDC__
void
ZXXALLOC(X, L)
void**X[]; long L;
{
long I=0;
while(X[I]!=NULL) ZALLOCATE(X[I++], L);
}
#else
void
ZXXALLOC(void**X[], long L)
{
long I=0;
while(X[I]!=NULL) ZALLOCATE(X[I++], L);
}
#endif
void
ZLDALLOC(long L)
{
#ifdef __BORLANDC__
if(L>16000)L = 16000;
#else
if(L>64000)L = 64000;
#endif
ZLDLIM = 0;
ZLDMAX = L;

ZXXALLOC((void***)LLDCOR, L*4);
}
void
ZldALLOC(long L)
{
ZLDLIM = 0;
ZLDMAX = L;

ZXXALLOC((void***)Lldcor, L*4);
}
void
ZLBALLOC(long L)
{
#ifdef __BORLANDC__
if(L>32000)L = 32000;
#else
if(L>64000)L = 64000;
#endif
ZLBLIM = 1;
ZLBMAX = L;

ZXXALLOC((void***)ILBCOR, L*2);
ZXXALLOC((void***)CLBCOR, L*1);
}
void
ZLVALLOC(long L)
{
#ifdef __BORLANDC__
if(L>32000)L = 32000;
#else
if(L>64000)L = 64000;
#endif
ZLVLIM = 1;
ZLVMAX = L;

ZXXALLOC((void***)ILVCOR, L*2);
}
void
ZVTALLOC(long L)
{
#ifdef __BORLANDC__
if(L>32000)L = 32000;
#else
if(L>64000)L = 64000;
#endif
ZVTLIM = 2;
ZVTMAX = L;

ZXXALLOC((void***)IVTCOR, L*2);
ZXXALLOC((void***)CVTCOR, L*1);
}
void
ZPTALLOC(long L)
{
#ifdef __BORLANDC__
if(L>32000)L = 32000;
#else
if(L>64000)L = 64000;
#endif
ZPTLIM = 1;
ZPTMAX = L;

ZXXALLOC((void***)LPTCOR, L*4);
ZXXALLOC((void***)IPTCOR, L*2);
}
void
ZJSALLOC(long L)
{
if(L>16000)L = 16000;
ZJSLIM = 0;
ZJSMAX = L;

ZXXALLOC((void***)IJSCOR, L*2);
}
long
ZVTNEXT(void)
{
	if((ZVTLIM+1)>=ZVTMAX) ZOUTABORT();
	return(ZVTLIM++);
}
long
ZLVNEXT(void)
{
	if((ZLVLIM+1)>=ZLVMAX) ZOUTABORT();
	return(ZLVLIM++);
}
long
ZLBNEXT(void)
{
	if((ZLBLIM+1)>=ZLBMAX) ZOUTABORT();
	return(ZLBLIM++);
}
long
ZYPNEXT(void)
{
	if((ZYPLIM+1)>=ZYPMAX) ZOUTABORT();
	return(ZYPLIM++);
}
long
ZPTNEXT(void)
{
	if((ZPTLIM+1)>=ZPTMAX) ZOUTABORT();
	return(ZPTLIM++);
}
long
ZLDNEXT(void)
{
	if((ZLDLIM+1)>=ZLDMAX) ZOUTABORT();
	return(ZLDLIM++);
}
char
COMBIN(long LB)
{
long L0; /*AUXILIARY-LV-POINTER*/
long L1; /*INITIAL-VECTOR*/
long L2; /*LB-VECTOR-PRECEEDING-COINCIDENCE*/
long L3; /*LAST-COINCIDENT-B3-VECTOR*/
long L4; /*B3-VECTOR-AFTER-L3*/
long L5; /*B3-VECTOR-PRECEEDING-COINCIDENCE*/
long L6; /*LAST-COINCIDENT-LB-VECTOR*/
long L7; /*FIRST-COINCIDENT-B3-VECTOR(L6-COINCIDENCE)*/
long L8; /*AUXILIARY-LV-POINTER*/
long L9; /*LB-VECTOR-AFTER-L6*/
long B3; /*(COINCIDENT-LB)-POINTER*/
long B1; /*AUXILIARY-LB-POINTER*/
	L1 = L0 = ZLBCO3[LB];
	L8 = FetchReverseLv(L1);
	B3 = L8 ? ZLVCO7[L8] : (B1=0);
	if(L8)
	do /*FIND-INITIAL-VECTOR*/
	{
	L1 = ZLVCO1[L1];
	L8 = FetchReverseLv(L1);
	B1 = L8 ? ZLVCO7[L8] : 0;
	}
	while((L1!=L0)&&(B3==B1));
	/*FULLY-COINCIDENCE?*/
	if(B3&&(L0==L1))return(2);
	L0 = L1;
	B3 = LB;
	do /*FIND-L3-L2-(L0-IS-SECOND-FOR-LB)*/
	{
	L2 = L0;
	L0 = ZLVCO1[L0];
	L3 = FetchReverseLv(L0);
	if(L3) B3 = ZLVCO7[L3];
	}
	while((B3==LB)&&(L1!=L0));
	/*NON-COINCIDENT?*/
	if(L1==L0)return(0);
	do /*FIND-LAST-COINCIDENT-WITH-B3(L6)*/
	{
	L6 = L0;
	L0 = ZLVCO1[L0];
	L7 = FetchReverseLv(L6);
	L8 = FetchReverseLv(L0);
	B1 = L8 ? ZLVCO7[L8] : 0;
	if(B1!=B3)break;
	L8 = ZLVCO1[L8];
	if(L8!=L7)break;
	}
	while(1);
	L9 = ZLVCO1[L6];
	L4 = ZLVCO1[L3];
	L0 = L4;
	while(L0!=L7)
	{
	L5 = L0;
	ZLVCO7[L0] = LB;
	L0 = ZLVCO1[L0];
	}
	if(L4==L7)
	ZLVCO1[L2] = L9;
	else
	{
	ZLVCO1[L2] = L4;
	ZLVCO1[L5] = L9;
	}
	ZLBCO3[B3] = 0L;
	ZLBCO3[LB] = L2; /*Set New Origin*/
	return(1);
}
void
COMBINE_C_POLYGONS(void)
{
long L;
char F, S;
	if(ZLBLIM<3) return;

	L = 1;
	while(L<ZLBLIM) ZLBCO7[L++] = 1;

	for(F = 1, L = 1; F; L = 1)
	for(F = 0; L<ZLBLIM; L+= 1)
	{
	if(ZLBCO7[L]!=1||!ZLBCO3[L])
	{
	/*POLYGON IS FULLY*/
	/*OUTSIDE*/
	/*OR IT WAS COMBINED BEFORE. */
	continue;
	}
	ZLBCO7[L] = S = COMBIN(L);
	if(S) F = 1;
	}
}
typedef long ipoint(long, long);
ipoint*IPOINT;
/**/
void Determine_Point_Base(void)
{
long I;
	XPDIV = (PXMAX-PXMIN+PXCNT)/PXCNT;
	YPDIV = (PYMAX-PYMIN+PYCNT)/PYCNT;
	for(I=0; I<PBLIM; ++I) ZPBCOR[I] = 0;
}
long IPOINT0(long X, long Y)
{
register int P;
	ZPTCO1[ZPTLIM] = X;
	ZPTCO2[ZPTLIM] = Y;
	P = 1;
	while(ZPTCO1[P]!=X||ZPTCO2[P]!=Y) ++P;
	if(P==ZPTLIM) ZPTCO3[ZPTNEXT()] = 0;
	return(long)P;
}
long IPOINT3(long X, long Y)
{
register int P;
register long B, x, y;
	y = ((Y-PYMIN)/YPDIV)+1;
	x = ((X-PXMIN)/XPDIV)+1;

	if(x<1) x = 1;
	else if(x>PXCNT) x = PXCNT;
	if(y<1) y = 1;
	else if(y>PYCNT) y = PYCNT;

	B = (--x)*PYCNT+(--y);
	P = ZPBCOR[B];
	while(P)
	{
	if(ZPTCO1[P]==X&&ZPTCO2[P]==Y)return(P);
	P = ZPTCO5[P];
	}
	P = ZPTNEXT();
	ZPTCO5[P] = ZPBCOR[B];
	ZPBCOR[B] = P;
	ZPTCO1[P] = X;
	ZPTCO2[P] = Y;
	ZPTCO3[P] = 0;
	return(P);
}
long IPOINT1(long X, long Y)
{
register SHORT P;
long B, x, y;
	y = ((Y-PYMIN)/YPDIV);
	x = ((X-PXMIN)/XPDIV);

	B = PYCNT*x + y, P = ZPBCOR[B];
	while(P)
	{
	if(ZPTCO1[P]==X&&ZPTCO2[P]==Y)return(P);
	P = ZPTCO5[P];
	}
	P = ZPTNEXT();
	ZPTCO5[P] = ZPBCOR[B];
	ZPBCOR[B] = P;
	ZPTCO1[P] = X;
	ZPTCO2[P] = Y;
	ZPTCO3[P] = 0;
	return(P);
}
double Eval_Angle
(double DX, double DY)
{
double AN;
	if(DX==0.0)
	AN = DY>0.0 ? 90.0 : DY<0.0 ? 270.0 : 0.0;
	else if(DY==0.0)
	AN = DX>0.0 ? 0.0 : 180.0;
	else
	{
	AN = atan(DY/DX)*(180.0/M_PI);
	if(DX<0.0) AN = 180.0+AN;
	else AN = DY>0.0 ? AN : 360.0+AN;
	}
	return(AN);
}
double sumangles;
char SS_Fl;
double SUB_ANGLES(double A0, double A1)
{
	double A2;
	if(SS_Fl)
	{
	SS_Fl = 0;
	return(0);
	}
	A2 = A0-A1;
	if(A2>180.0) A2 -= 360.0;
	else if(A2<-180.0) A2 += 360.0;
	return(A2);
}
double COMPLVANGLE(long L)
{
SHORT A, B;
double X, Y;
	A = ZLVCO5[L];
	B = ZLVCO6[L];
	X = ZPTCO1[B]-ZPTCO1[A];
	Y = ZPTCO2[B]-ZPTCO2[A];
	if(X==0&&Y==0)
	{
	SS_Fl = 1;
	return(0);
	}
	return(Eval_Angle(X, Y));
}
double COMPLDANGLE(long L)
{
double X, Y;
	X = ZLDCO0[L]-ZLDCO0[L-1];
	Y = ZLDCO1[L]-ZLDCO1[L-1];
	if(X==0&&Y==0)
	{
	SS_Fl = 1;
	return(0);
	}
	return(Eval_Angle(X, Y));
}
long DETERMINE_STAT_1(void)
{
long S, D;
double F, L, C, T;
	SS_Fl = 0;
	if(ZLDLIM<3)return(STATUS=0);

	L = F = COMPLDANGLE(1);
	T = 0;
	for(D = 2; D<ZLDLIM; ++D, L = C)
	{
	T += SUB_ANGLES(L, (C = COMPLDANGLE(D), SS_Fl? (C = L):C));
	}
	T += SUB_ANGLES(L, F);
	if(T>0.0) S = ENVEQ(T, 360.0, 0.001) ? 1: 0;
	else S = ENVEQ(T,-360.0, 0.001) ?-1: 0;
	STATUS = S;
sumangles = T;
	return(S);
}
long DETERMINE_STAT_2(long B)
{
long S, E, V;
double F, L, C, T;
	E = ZLBCO3[B];

	SS_Fl = 0;
	if(!E)return(STATUS=0);

	L = F = COMPLVANGLE(E);
	T = 0;
	for(V = ZLVCO1[E]; V!=E; V = ZLVCO1[V], L = C)
	{
	T += SUB_ANGLES(L, (C = COMPLVANGLE(V), SS_Fl? (C = L):C));
	}
	T += SUB_ANGLES(L, F);
	if(T>0.0) S = ENVEQ(T, 360.0, 0.001) ? 1: 0;
	else S = ENVEQ(T,-360.0, 0.001) ?-1: 0;
	STATUS = S;
sumangles = T;
	return(S);
}
void Z_FEval_Storage(void)
{
long LB, LV, LE, P1, P2, PM, PA, PB, PD, PCOUNT=1;
long XM, XD, YM, YA, YB, LD, X, Y;
	ZLBCO7[LB = ZLBNEXT()] = 0;
	ZLBCO4[LB] = C_Layer;

	ZLBCO3[LB] = ZLVLIM;
	ZLBCO1[LB] = STATUS;
	XM=XD=ZLDCO0[0];
	YA=YB=YM=ZLDCO1[0];
	PA=PB=PD=PM=P1=P2=IPOINT(X=XM, Y=YM);
	for(LD=1, LE=LV=ZLBCO3[LB]; LD<ZLDLIM; ++LD, P1=P2, LE=LV)
	{
	if(ZLDCO0[LD]==X&&ZLDCO1[LD]==Y) continue;
	P2 = IPOINT((X=ZLDCO0[LD]), (Y=ZLDCO1[LD]));
	PCOUNT += 1, LV = ZLVNEXT();
	ZLVCO7[LV] = LB;
	ZLVCO5[LV] = P1;
	ZLVCO6[LV] = P2;
	ZLVCO2[LV] = 0L;
	ZLVCO1[LE] = LV;
	ZLVCO3[LV] = 0L;
	ZLVCO8[LV] = 0L;
	if(X<XM? 1: X>XM? 0: Y<YM? 1: 0) XM=X, YM=Y, PM=P2;
	if(X>XD) XD=X, PD=P2;
	if(Y<YA) YA=Y, PA=P2;
	if(Y>YB) YB=Y, PB=P2;
	}
	ZLVCO1[LV] = ZLBCO3[LB];
	ZLbXx1[LB] = PM;
	ZLbYy1[LB] = PA;
	ZLbYy2[LB] = PB;
	ZLbXx2[LB] = PD;
	if(Z_Open_Shape)return;
	if(PCOUNT<4)--ZLBLIM, ZLVLIM = ZLBCO3[LB];
}
#ifndef F_INIT_MODULE
#define zswab(A,B,C) ((C)=ZLBCO6[(A)],ZLBCO6[(A)]=ZLBCO6[(B)],ZLBCO6[(B)]=(C))
void QLb_Sort(long sl, long sr)
{
long k, j, w, x;
	if((sr-sl)<11) /*SHACKERSORT*/
	{
	while(sl<sr)
	{
	for(j=sr, k=sr; sl<j; j--)
	if(ZPTCO1[ZLbXx1[ZLBCO6[j-1]]] > ZPTCO1[ZLbXx1[ZLBCO6[j]]])
	{
	x = ZLBCO6[j];
	do ZLBCO6[j] = ZLBCO6[j-1], j-- ;
	while((sl<j) && (ZPTCO1[ZLbXx1[ZLBCO6[j-1]]] > ZPTCO1[ZLbXx1[x]]));
	ZLBCO6[j] = x, k = j;
	}
	sl = k+1;
	for(j=sl; j<sr; j++)
	if(ZPTCO1[ZLbXx1[ZLBCO6[j]]] > ZPTCO1[ZLbXx1[ZLBCO6[j+1]]])
	{
	x = ZLBCO6[j] ;
	do ZLBCO6[j] = ZLBCO6[j+1], j++ ;
	while((j<sr) && (ZPTCO1[ZLbXx1[x]] > ZPTCO1[ZLbXx1[ZLBCO6[j+1]]]));
	ZLBCO6[j] = x, k = j;
	}
	sr = k-1;
	}
	}
	else /*QUICKSORT*/
	{
	x = (sl + sr)/2 ;
	if(ZPTCO1[ZLbXx1[ZLBCO6[sl]]] > ZPTCO1[ZLbXx1[ZLBCO6[x]]]) zswab(sl, x, w);
	if(ZPTCO1[ZLbXx1[ZLBCO6[x]]] > ZPTCO1[ZLbXx1[ZLBCO6[sr]]]) zswab(sr, x, w);
	if(ZPTCO1[ZLbXx1[ZLBCO6[sl]]] > ZPTCO1[ZLbXx1[ZLBCO6[x]]]) zswab(sl, x, w);
	x = ZLBCO6[x];
	for(k=sl+1, j=sr-1; k<j;)
	{
	while(ZPTCO1[ZLbXx1[x]] > ZPTCO1[ZLbXx1[ZLBCO6[k]]]) ++k;
	while(ZPTCO1[ZLbXx1[ZLBCO6[j]]] > ZPTCO1[ZLbXx1[x]]) --j;
	if(k<j) zswab(k, j, w ), ++k, --j;
	}
	if(sl<j) QLb_Sort(sl, j);
	if(k<sr) QLb_Sort(k, sr);
	}
}
#undef zswab
#endif
typedef long splitlv(long, long);
splitlv*SPLITLV;
long SPLITLV_M(long L3, long IP)
{
long L4 = ZLVNEXT();
	ZLVCO2[L4] = 0;
	ZLVCO3[L4] = 0;
	ZLVCO8[L4] = 0;
	ZLVCO1[L4] = ZLVCO1[L3];
	ZLVCO1[L3] = L4;
	ZLVCO7[L4] = ZLVCO7[L3];
	ZLVCO6[L4] = ZLVCO6[L3];
	ZLVCO6[L3] = IP;
	ZLVCO5[L4] = IP;
	return(L4);
}
typedef long compsplitlv(long, long, long, long);
compsplitlv*COMPSPLITLV;
long COMPSPLITLV_M(long L3, long V3, long V4, long IP)
{
long L4; /*SECOND-PART-OF-L3*/
long L7; /*COINCINDENT-FOR-L3*/
long L8; /*COINCINDENT-FOR-L4*/
	L7 = ZLVCO3[L3];
	L8 = L7 ? COMPSPLITLV(L7, V3, V4, IP) : 0;
	L4 = SPLITLV(L3, IP);
	ZLVCO2[L4] = V4; /*SET-VECTOR-NUMBER*/
	ZLVCO3[L4] = L8; /*SET-NEXT-COINCINDENT-VECTOR*/
	return(L4);
}
long SPLITVT(long V3, long IP)
{
long V4; /*V3-OLD-REVERSE*/
long V5; /*V4-NEW-REVERSE-OR-V3-SECOND-PART*/
long V6; /*V3-NEW-REVERSE-OR-V4-SECOND-PART*/
long L3;
long L4;
long L5;
long L6;
	V4 = ZVTCO6[V3];
	V5 = ZVTNEXT();
	V6 = ZVTNEXT();
	if(V3-((V3>>1)<<1)) Zswab(&V5, &V6);
	ZVTCO2[V3] = ZVTCO2[V4] = IP;
	ZVTCO1[V5] = ZVTCO1[V6] = IP;
	ZVTCO2[V5] = ZVTCO1[V4];
	ZVTCO2[V6] = ZVTCO1[V3];
	ZVTCO6[V5] = V4;
	ZVTCO6[V6] = V3;
	ZVTCO6[V3] = V6;
	ZVTCO6[V4] = V5;
	L3 = ZVTCO5[V3]; /*CHAIN-HEAD-COINCIDENT-VECTORS*/
	L4 = ZVTCO5[V4]; /*CHAIN-HEAD-FOR-REVERSE-DIRECTION*/
	L5 = L3 ? COMPSPLITLV(L3, V3, V5, IP) : 0;
	L6 = L4 ? COMPSPLITLV(L4, V4, V6, IP) : 0;
	ZVTCO5[V6] = L6;
	ZVTCO5[V5] = L5;
	ZVTCO4[V5] = ZVTCO4[V3];
	ZVTCO4[V6] = ZVTCO4[V4];
	ZVTCO8[V6] = ZVTCO8[V5] = 0;
	return(V5);
}
long
INSERTV(long P1, long P2, long LV)
{
long V3;
long V4;
long X1;
long X2;
long Y1;
long Y2;
	V3 = ZVTNEXT();
	V4 = ZVTNEXT();
	X1 = ZPTCO1[P1];
	X2 = ZPTCO1[P2];
	Y1 = ZPTCO2[P1];
	Y2 = ZPTCO2[P2];
	if((X1>X2)||((X1==X2)&&(Y1>Y2)))Zswab(&V3, &V4);
	if(LV) ZLVCO2[LV] = V3;
	ZVTCO5[V3] = LV, ZVTCO5[V4] = 0;
	ZVTCO6[V3] = V4;
	ZVTCO6[V4] = V3;
	ZVTCO2[V4] = ZVTCO1[V3] = P1;
	ZVTCO1[V4] = ZVTCO2[V3] = P2;
	ZVTCO4[V4] = ZVTCO4[V3] = 0;
	ZVTCO8[V3] = ZVTCO8[V4] = 0;
	return(V3);
}
#ifndef F_INIT_MODULE
void Init_Vt_hash_tree
(long Y1, long Y2, SHORT P, char L)
{
long Y3;
	Y3 = ((Y2-Y1)>>1)+Y1;

	if(L>=Lf_Factor)return;

	Lf_Upper[P] = Tree_next_pos++;
	Lf_Lower[P] = Tree_next_pos++;

	Lf_Ymidl[P] = Y3;

	Init_Vt_hash_tree(Y3, Y2, Lf_Upper[P], L+1);
	Init_Vt_hash_tree(Y1, Y3, Lf_Lower[P], L+1);
}
void Init_Vt_Hash_Tree(void)
{
SHORT I;
	Tree_next_pos = 1;

	for(I = 0; I<LF_LIM; ++I)
	{
	Lf_Sdown[I] = 0;
	Lf_Rolld[I] = 0;
	Lf_First[I] = 0;
	Lf_Stack[I] = 0;
	}
	if(Initialized_hash){}
	else
	{
	for(I = 0; I<LF_LIM; ++I)
	{
	Lf_Upper[I] = 0;
	Lf_Lower[I] = 0;
	}
	Init_Vt_hash_tree(PYMIN, PYMAX, 0, 1);
	}
	Lf_Lim = 0;
#ifdef F_EVAL_MODULE
	Initialized_hash = 1;
#else
	Initialized_hash = 0;
#endif
	return;
}
#define ZLvNum ZLVCO8
#define ZLbNum ZLBCO6
#define ZLbLvp ZLBCO3
#define ZLvNxt ZLVCO1
#define ZVtNxt ZPVCO5
#define ZLvPt1 ZLVCO5
#define ZLvPt2 ZLVCO6
#define ZVtPt1 ZVTCO1
#define ZVtPt2 ZVTCO2
#define ZPtXxx ZPTCO1
#define ZPtYyy ZPTCO2
#define ZVtRev ZVTCO6
#define ZLvVtp ZLVCO2
#define ZLvLvp ZLVCO3
#define ZVtLvp ZVTCO5
#define ZVtRst ZVTCO4
#define ZVtTst ZVTCO8
void Into_Lv_Chain(long Lv, long Vt)
{
	ZLVCO2[Lv] = Vt;
	ZLVCO3[Lv] = ZVTCO5[Vt];
	ZVTCO5[Vt] = Lv;
}
void H_Insert(SHORT Lf)
{
	while(1)
	{
	if(!Lf_Upper[Lf]){}
	else
	if(Vt_Ymin>Lf_Ymidl[Lf])
	{
	Lf_Sdown[Lf] = 1;
	Lf = Lf_Upper[Lf];
	continue;
	}
	else
	if(Vt_Ymax<Lf_Ymidl[Lf])
	{
	Lf_Sdown[Lf] = 1;
	Lf = Lf_Lower[Lf];
	continue;
	}
	if(Lf_Rolld[Lf]==0)
	{
	Lf_ROLLD[Lf] = Vt_Ptr;
	Lf_Stack[Lf_Lim++] = Lf;
	}
	ZVtNxt[Vt_Ptr] = Lf_Rolld[Lf];
	Lf_Rolld[Lf] = Vt_Ptr;
	return;
	}
}
void H_INSERT(SHORT Lf)
{
	long P3;
	long P4;
	P3 = ZVtPt1[Vt_Ptr];
	P4 = ZVtPt2[Vt_Ptr];

	Vt_Ymin = ZPtYyy[P3];
	Vt_Ymax = ZPtYyy[P4];

	if(Vt_Ymin>Vt_Ymax)
	{
	register long Z = Vt_Ymin;
	Vt_Ymin = Vt_Ymax;
	Vt_Ymax = Z;
	}
	H_Insert(Lf);
}
long Split_Vt(SHORT Lf, long Vt, long Pt)
{
	Vt_Ptr = SPLITVT(Vt, Pt);

	H_INSERT(Lf);

	return(Vt_Ptr);
}
void Remove_Vt(SHORT Lf, long Vt)
{
	long VT = ZVtNxt[Vt];

	ZVtNxt[Vt_Last] = VT;
	if(Vt_Last==0) Lf_First[Lf] = VT;

	Vt_Ptr = Vt;

	H_INSERT(Lf);
}
#define Lv Lv_Ptr

#define P1 Lv_Pt1
#define P2 Lv_Pt2
#define X1 Lv_Xx1
#define Y1 Lv_Yy1
#define X2 Lv_Xx2
#define Y2 Lv_Yy2

#define Y21 Lv_Y21
#define X21 Lv_X21

#define DE21 Lv_De21
void Find_Intsect(SHORT Lf)
{
static long A;
static long B;

static long Y4;
static long X4;
static long Y3;
static long X3;
static long Ls;
static long Vs;
static long Vt;
static long VT;
static long P3;
static long P4;

static char S1;
static char S2;
static char S3;

static char H132;
static char H142;
static char H314;
static char H324;
static char DF21;
static char DF43;
static char Cond;

static long Int_P;
static long Int_Y;
static long Int_X;

static double X31;
static double Y31;
static double X41;
static double Y41;
static double X23;
static double Y23;
static double X43;
static double Y43;

static double S132;
static double S142;
static double S314;
static double S324;
static double DE43;

	Vt = Lf_First[Lf];

	for(Vt_Last = 0; Vt; Vt_Last = Vt_Auxx, Vt = ZVtNxt[Vt])
	{
	Vt_Auxx = Vt;

	P3 = ZVtPt1[Vt];
	X3 = ZPtXxx[P3];

	if(X3>Lv_Xmax)
	{
	continue;
	}
	P4 = ZVtPt2[Vt];
	X4 = ZPtXxx[P4];

	if(X4<Sh_Xmin)
	{
	if(Vt_Last)
	{
	ZVtNxt[Vt_Last] = ZVtNxt[Vt];
	Vt_Auxx = Vt_Last;
	continue;
	}
	Lf_First[Lf] = ZVtNxt[Vt];
	Vt_Auxx = 0;
	continue;
	}
	if(X4<Lv_Xmin)
	{
	continue;
	}
	Y4 = ZPtYyy[P4];
	Y3 = ZPtYyy[P3];

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
	Vt_Swi = X3==X4&&Y3>Y4;

	Int_Type = 0;

	X31 = X3-X1;
	Y31 = Y3-Y1;
	X41 = X4-X1;
	Y41 = Y4-Y1;
	X23 = X2-X3;
	Y23 = Y2-Y3;
	X43 = X4-X3;
	Y43 = Y4-Y3;
	S132 = X31 *Y21 - Y31 *X21;
	S142 = X41 *Y21 - Y41 *X21;
	S314 = Y31 *X43 - X31 *Y43;
	S324 = X23 *Y43 - Y23 *X43;
	DE43 = ((Vt_Ymax-Vt_Ymin)+X43)/(2.0);

	H132 = S132 >DE21 ? 1 : S132 <-DE21 ? -1 : 0;
	H142 = S142 >DE21 ? 1 : S142 <-DE21 ? -1 : 0;
	H314 = S314 >DE43 ? 1 : S314 <-DE43 ? -1 : 0;
	H324 = S324 >DE43 ? 1 : S324 <-DE43 ? -1 : 0;

	DF21 = (H132==-H142)&&(H132);
	DF43 = (H324==-H314)&&(H324);

	if(DF21&&DF43)Int_Type = 5;
	else
	if(!H314&&!H324&&!H132&&!H142)
	{
	if(Lv_Swi!=Vt_Swi)
	{
	if(P1==P3||P2==P4)continue;
	if(P3==P2&&P4==P1)Int_Type = 6;
	}
	else
	{
	if(P1==P4||P2==P3)continue;
	if(P3==P1&&P4==P2)Int_Type = 7;
	}
	switch(Int_Type)
	{
	case 6:
	Vt = ZVtRev[Vt];
	case 7:
	Into_Lv_Chain(Lv, Vt);
	Lv_Ins = 1;
	return;
	}
	Int_Type = 8;
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
	if(DF21 && !H314) Int_Type = 1, ++Int_0, Int_1 = 1;
	else Int_1 = 0;
	if(DF21 && !H324) Int_Type = 2, ++Int_0, Int_2 = 1;
	else Int_2 = 0;
	if(DF43 && !H132) Int_Type = 3, ++Int_0, Int_3 = 1;
	else Int_3 = 0;
	if(DF43 && !H142) Int_Type = 4, ++Int_0, Int_4 = 1;
	else Int_4 = 0;
	if(Int_0==0)
	{
	if(DF21&&DF43) Int_Type = 9;
	else continue;
	}
	else
	if(Int_0>1)
	{
	if(Int_1&&Int_2)Int_Type = 8;
	else
	if(Int_3&&Int_4)Int_Type = 8;
	else
	Int_Type = 9;
	}
	}
	if(Int_Type==5||Int_Type==9)
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
	C1 = (double)X1*Y2 - (double)X2*Y1;
	A2 = -Y43;
	B2 = +X43;
	C2 = (double)X3*Y4 - (double)X4*Y3;
	DL = A1*B2 - B1*A2;
	Int_X = floor((B1*C2-B2*C1)/DL +0.5 -0.5E-7);
	Int_Y = floor((A2*C1-A1*C2)/DL +0.5 -0.5E-7);
	Int_P = IPOINT(Int_X, Int_Y);
Int_Type = 9;
	}
	switch(Int_Type)
	{
	case 1:
	Split_Vt(Lf, Vt, P1);

	Remove_Vt(Lf, Vt);
	Vt_Auxx = Vt = Vt_Last;

	continue;
	case 2:
	Split_Vt(Lf, Vt, P2);

	Remove_Vt(Lf, Vt);
	Vt_Auxx = Vt = Vt_Last;

	continue;
	case 3:
	/*
	Lv_Spl = 1;
	*/
	SPLITLV(Lv, P3);
	P2 = P3;
	X2 = X3;
	Y2 = Y3;
	break;
	case 4:
	/*
	Lv_Spl = 1;
	*/
	SPLITLV(Lv, P4);
	P2 = P4;
	X2 = X4;
	Y2 = Y4;
	break;
	case 5:
	Split_Vt(Lf, Vt, Int_P);

	Remove_Vt(Lf, Vt);
	Vt_Auxx = Vt = Vt_Last;
	/*
	Lv_Spl = 1;
	*/
	SPLITLV(Lv, Int_P);
	P2 = Int_P;
	X2 = Int_X;
	Y2 = Int_Y;
	break;

	case 9:
	if(Int_P!=P3&&Int_P!=P4)
	{
	Split_Vt(Lf, Vt, Int_P);

	Remove_Vt(Lf, Vt);
	Vt_Auxx = Vt = Vt_Last;
	}
	if(Int_P==P1||Int_P==P2)
	{
	continue;
	}
	/*
	Lv_Spl = 1;
	*/
	SPLITLV(Lv, Int_P);
	P2 = Int_P;
	X2 = Int_X;
	Y2 = Int_Y;
	break;

	case 8:
	{
	long Vt_S[3];
	long Vt_L =0;
	Delta_Y = Lv_Ymax-Lv_Ymin;
	Delta_X = Lv_Xmax-Lv_Xmin;

	S3 = Delta_Y>Delta_X;

	S1 = S3? Y4<Y3: X4<X3;
	S2 = S3? Y2<Y1: X2<X1;

	S1 = S1!=S2;
	if(S1)
	Zswab(&P3, &P4);

	Ls = Lv;
	Vs = Vt;

	if(P1!=P3)
	{
	if(S3) A = Y1, B = ZPtYyy[P3];
	else A = X1, B = ZPtXxx[P3];
	if(S2) Cond = A>=B;
	else Cond = A<=B;
	if(Cond)
	{
	/*
	Lv_Spl = 1;
	*/
	Ls = SPLITLV(Ls, P3);
	}
	else
	{
	Vs = SPLITVT(Vs, P1);
	Vt_S[Vt_L++] = Vs;
	if(S1) Vs = Vt;
	}
	}
	if(P2!=P4)
	{
	if(S3) A = Y2, B = ZPtYyy[P4];
	else A = X2, B = ZPtXxx[P4];
	if(S2) Cond = A<=B;
	else Cond = A>=B;
	if(Cond)
	{
	/*
	Lv_Spl = 1;
	*/
	SPLITLV(Ls, P4);
	}
	else
	{
	VT = SPLITVT(Vs, P2);
	Vt_S[Vt_L++] = VT;
	if(S1) Vs = VT;
	}
	}
	if(S1) Vs = ZVtRev[Vs];
	Into_Lv_Chain(Ls, Vs);

	if(Vt_L)
	{
	while(Vt_L--)
	Vt_Ptr = Vt_S[Vt_L], H_INSERT(Lf);

	Remove_Vt(Lf, Vt);
	Vt_Auxx = Vt = Vt_Last;
	}
	P2 = ZLvPt2[Lv];
	X2 = ZPtXxx[P2];
	Y2 = ZPtYyy[P2];
	if(Lv==Ls)
	{
	Lv_Ins = 1;
	return;
	}
	P2 = ZLvPt2[Lv];
	X2 = ZPtXxx[P2];
	Y2 = ZPtYyy[P2];
	break;
	}
	}
	/*End Switch*/

	if(X1<X2)
	Lv_Xmin = X1,
	Lv_Xmax = X2;
	else
	Lv_Xmin = X2,
	Lv_Xmax = X1;

	if(Y1<Y2)
	Lv_Ymin = Y1,
	Lv_Ymax = Y2;
	else
	Lv_Ymin = Y2,
	Lv_Ymax = Y1;

	Y21 = Y2 - Y1;
	X21 = X2 - X1;

	Lv_Swi = (X1>X2)||((X1==X2)&&(Y1>Y2));

	DE21 = ((double)(Lv_Ymax-Lv_Ymin)+(Lv_Xmax-Lv_Xmin))/(2.0);
	}
}
#undef P1
#undef P2
#undef X1
#undef Y1
#undef X2
#undef Y2
#undef Lv
void IntSect_Lv(short Lf)
{
	if(Lf_First[Lf])
	{
	Find_Intsect(Lf);
	if(Lv_Ins)return;
	}
	if(!Lf_Sdown[Lf])return;

	if(Lf_Ptr<0)
	{
	char Lst;
	char Ust;
	Ust = Lv_Ymax>=Lf_Ymidl[Lf];
	Lst = Lv_Ymin<=Lf_Ymidl[Lf];

	if(Ust&&Lst)Lf_Ptr = Lf;
	}
	if(Lv_Ymin<=Lf_Ymidl[Lf])
	{
	IntSect_Lv(Lf_Lower[Lf]);
	if(Lv_Ins)return;
	}
	if(Lv_Ymax>=Lf_Ymidl[Lf])
	{
	IntSect_Lv(Lf_Upper[Lf]);
	}
	return;
}
void Intsect_Lv(long Lv)
{
long V3;
long V4;
	Lv_Ptr = Lv;

	Lv_Pt1 = ZLvPt1[Lv];
	Lv_Pt2 = ZLvPt2[Lv];

	Lv_Xx1 = ZPtXxx[Lv_Pt1];
	Lv_Xx2 = ZPtXxx[Lv_Pt2];
	Lv_Yy1 = ZPtYyy[Lv_Pt1];
	Lv_Yy2 = ZPtYyy[Lv_Pt2];

	Lv_Swi = (Lv_Xx1>Lv_Xx2)||((Lv_Xx1==Lv_Xx2)&&(Lv_Yy1>Lv_Yy2));

	if(Lv_Xx1<Lv_Xx2)
	Lv_Xmin = Lv_Xx1,
	Lv_Xmax = Lv_Xx2;
	else
	Lv_Xmin = Lv_Xx2,
	Lv_Xmax = Lv_Xx1;

	if(Lv_Yy1<Lv_Yy2)
	Lv_Ymin = Lv_Yy1,
	Lv_Ymax = Lv_Yy2;
	else
	Lv_Ymin = Lv_Yy2,
	Lv_Ymax = Lv_Yy1;

	Lv_Y21 = Lv_Yy2 - Lv_Yy1;
	Lv_X21 = Lv_Xx2 - Lv_Xx1;

	Lv_De21 = ((double)(Lv_Ymax-Lv_Ymin)+(Lv_Xmax-Lv_Xmin))/(2.0);
	do
	{
	Lf_Ptr =-1;
	Lv_Ins = 0;
	Lv_Spl = 0;

	IntSect_Lv(0);

	if(Lv_Ins)return;
	}
	while(Lv_Spl);
	Lv_Ins = 1;

	V3 = ZVTNEXT();
	V4 = ZVTNEXT();

	Vt_Ptr = V3;

	if(Lv_Swi)Zswab(&V3, &V4);

	ZVtLvp[V4] = 0;
	ZVtLvp[V3] = Lv_Ptr;

	ZLvVtp[Lv] = V3;
	ZVtRev[V3] = V4;
	ZVtRev[V4] = V3;

	ZVtPt2[V4] = ZVtPt1[V3] = Lv_Pt1;
	ZVtPt1[V4] = ZVtPt2[V3] = Lv_Pt2;

	ZVtRst[V4] = ZVtRst[V3] = 0;
	ZVtTst[V3] = ZVtTst[V4] = 0;

	Vt_Ymin = Lv_Ymin;
	Vt_Ymax = Lv_Ymax;

	if(Lf_Ptr<0)Lf_Ptr = 0;

	H_Insert(Lf_Ptr);
}
#define Lv_swab(A,B,C) ((C)=ZLvNum[(A)],ZLvNum[(A)]=ZLvNum[(B)],ZLvNum[(B)]=(C))
void QLv_Sort(SHORT sl, SHORT sr)
{
SHORT k, j;
register SHORT w, x;
	if((sr-sl)<17) /*SHACKERSORT*/
	{
	while(sl<sr)
	{
	for(j=sr, k=sr; sl<j; j--)
	if(ZPtXxx[ZVtPt1[ZLvNum[j-1]]] > ZPtXxx[ZVtPt1[ZLvNum[j]]])
	{
	x = ZLvNum[j];
	do
	ZLvNum[j] = ZLvNum[j-1], j-- ;
	while((sl<j) && (ZPtXxx[ZVtPt1[ZLvNum[j-1]]] > ZPtXxx[ZVtPt1[x]]));
	ZLvNum[j] = x, k = j;
	}
	sl = k+1;
	for(j=sl; j<sr; j++)
	if(ZPtXxx[ZVtPt1[ZLvNum[j]]] > ZPtXxx[ZVtPt1[ZLvNum[j+1]]])
	{
	x = ZLvNum[j] ;
	do
	ZLvNum[j] = ZLvNum[j+1], j++ ;
	while((j<sr) && (ZPtXxx[ZVtPt1[x]] > ZPtXxx[ZVtPt1[ZLvNum[j+1]]]));
	ZLvNum[j] = x, k = j;
	}
	sr = k-1;
	}
	}
	else /*QUICKSORT*/
	{
	x = (sl + sr)/2 ;

	if(ZPtXxx[ZVtPt1[ZLvNum[sl]]] > ZPtXxx[ZVtPt1[ZLvNum[x]]])
	Lv_swab(sl, x, w);
	if(ZPtXxx[ZVtPt1[ZLvNum[x]]] > ZPtXxx[ZVtPt1[ZLvNum[sr]]])
	Lv_swab(sr, x, w);
	if(ZPtXxx[ZVtPt1[ZLvNum[sl]]] > ZPtXxx[ZVtPt1[ZLvNum[x]]])
	Lv_swab(sl, x, w);

	x = ZLvNum[x];

	for(k = sl+1, j = sr-1; k<j;)
	{
	while(ZPtXxx[ZVtPt1[x]] > ZPtXxx[ZVtPt1[ZLvNum[k]]]) ++k;
	while(ZPtXxx[ZVtPt1[ZLvNum[j]]] > ZPtXxx[ZVtPt1[x]]) --j;
	if(k<j)
	Lv_swab(k, j, w), ++k, --j;
	}
	if(sl<j) QLv_Sort(sl, j);
	if(k<sr) QLv_Sort(k, sr);
	}
}
#undef Lv_swab
void Zz_Lv_Sort(void)
{
long Ls;
long Le;
long Lv;
long Vt;
long P1;
long P2;
long X1;
long X2;

	Vt = 0;
	for(Ls = 1; Ls<ZLBLIM; ++Ls)
	{
	if(ZLbLvp[Ls]==0)continue;

	Le = ZLbLvp[Ls];
	Lv = Le;
	do
	{
	P1 = ZLvPt1[Lv];
	P2 = ZLvPt2[Lv];

	X1 = ZPtXxx[P1];
	X2 = ZPtXxx[P2];

	if(X2<X1) P1 = P2;

	++Vt;

	ZVtPt1[Lv] = P1;
	ZLvVtp[Lv] = 0L;
	ZLvLvp[Lv] = 0L;
	ZLvNum[Vt] = Lv;

	Lv = ZLvNxt[Lv];
	}
	while(Lv&&Lv!=Le);
	}
	if(Vt>1)
	QLv_Sort((SHORT)1, (SHORT)Vt);
	ZLSLIM = Vt+1;
}
void INTERSECTION_ANALYSIS(void)
{
	long Ls;
	long Le;
	long Vt;
	long VT;
	long X1;
	long X2;
	long P1;
	long P2;
	long Lv;
	long Lf;
	long LF;

	long Lnxt;
	Ia_Flag = 1;
	Ia_Complex = 0;

	Init_Vt_Hash_Tree();

#ifdef F_EVAL_MODULE
	fprintf(stderr, "Sort..");
#endif
	Zz_Lv_Sort();
#ifdef F_EVAL_MODULE
	fprintf(stderr, "Ok!");
#endif

	Le = ZLSLIM;
	for(Ls = 1; Ls<Le; ++Ls)
	{
	Lv = ZLvNum[Ls];

	P1 = ZLvPt1[Lv];
	P2 = ZLvPt2[Lv];

	X1 = ZPtXxx[P1];
	X2 = ZPtXxx[P2];

	if(X1<X2)
	Sh_Xmin = X1;
	else
	Sh_Xmin = X2;

	Lnxt = ZLvNxt[Lv];
	do
	{
	if(!ZLvVtp[Lv])Intsect_Lv(Lv);
	Lv = ZLvNxt[Lv];
	}
	while(Lv!=Lnxt);
	for(LF = 0; LF<Lf_Lim; ++LF)
	{
	Lf = Lf_Stack[LF];

	Vt = Lf_Rolld[Lf];
	VT = Lf_ROLLD[Lf];

	ZVtNxt[VT] = Lf_First[Lf];
	Lf_First[Lf] = Vt;

	Lf_Rolld[Lf] = 0L;
	}
	Lf_Lim = 0;
	}
	/*End Loop For Each Lv*/
#ifdef F_EVAL_MODULE
	for(Lv = 1; Lv<ZLSLIM; ++Lv)
	{
	ZLvNum[Lv] = 0;
	}
#endif
	Ia_Flag = 0;
}
#undef ZLvNum
#undef ZLbNum
#undef ZLbLvp
#undef ZLvNxt
#undef ZVtNxt
#undef ZLvPt1
#undef ZLvPt2
#undef ZVtPt1
#undef ZVtPt2
#undef ZPtXxx
#undef ZPtYyy
#undef ZVtRev
#undef ZLvVtp
#undef ZLvLvp
#undef ZVtLvp
#undef ZVtRst
#undef ZVtTst
#endif
typedef void mergelvchain(long, long);
mergelvchain*MERGELVCHAIN;
void MERGELVCHAIN_M(long VT, long PV)
{
long LQ, LS, LH, LM;
	LH = ZVTCO5[VT];
	LM = ZVTCO5[PV];
	for(LS = LQ = LH; LQ; LQ = ZLVCO3[LQ])
	{
	LS = LQ, ZLVCO2[LQ] = PV;
	}
	if(LS) ZLVCO3[LS] = LM, ZVTCO5[PV] = LH;
}
void SUBS_PV_BEAD(long O, long n, char z)
{
long PT;
	PT = ZVTCO1[O];
	if(ZPTCO3[PT]==O)
	{
	ZPTCO3[PT] = n;
	if(z) ZPVCO5[n] = ZPVCO5[O];
	return;
	}
	PT = ZPTCO3[PT];
	while(PT)
	{
	if(ZPVCO5[PT]==O)
	{
	if(z) ZPVCO5[n] = ZPVCO5[O];
	ZPVCO5[PT] = n;
	return;
	}
	PT = ZPVCO5[PT];
	}
}
void ERASEVTBEAD
(long VT, long PV)
{
long Vt;
long Pv;
	Vt = ZVTCO6[VT];
	Pv = ZVTCO6[PV];
	if(Vt<VT)
	{
	ZVTCO6[PV] = Vt;
	ZVTCO6[Vt] = PV;

	if(Pv<VT)
	SUBS_PV_BEAD(Vt, ZPVCO5[Vt], 0);
	SUBS_PV_BEAD(Pv, Vt, 1);

	Zswab(&Vt, &Pv);
	}
	MERGELVCHAIN(VT, PV);
	MERGELVCHAIN(Vt, Pv);
	ZVTCO6[VT] = ZVTCO6[Vt] = 0;
#ifndef F_EVAL_MODULE
	if(S_Free)
	{
	ZVTCO4[PV] |= ZVTCO4[VT];
	ZVTCO4[Pv] |= ZVTCO4[Vt];
	}
	else ZVTCO4[PV] = ZVTCO4[Pv] = 0;
#endif
}
void
split_vt_pv
(long vt, long pv, long y2, long y4, long p2, long p4)
{
	if(y2<y4) SPLITVT(pv, p2);
	else SPLITVT(vt, p4);
}
long Pass_Count = 0;
void POINT_VECTOR_ANALYSIS(long first)
{
	char ST, ER, H24, H42;
	long VT, PT, PV, P2, P4, PS, SL, SV;
	long X2, Y2, X4, Y4;
	long double S24, DE4, DE2;

	PV_Error = 0;
	for(VT=first; VT<ZVTLIM; ++VT)
	{
	if(!ZVTCO6[VT]) continue;
	PT = ZVTCO1[VT];
	P2 = ZVTCO2[VT];
	X2 = ZPTCO1[P2]-ZPTCO1[PT];
	Y2 = ZPTCO2[P2]-ZPTCO2[PT];

	if(!X2&&!Y2) continue;

	if(X2>0) SL = Y2<0 ? 4 : 1;
	else
	if(X2<0) SL = Y2>0 ? 2 : 3;
	else	 SL = Y2>0 ? 2 : 4;

	PV = ZPTCO3[PT];
	PS = ER = 0;
	for(ST = 1; PV!=0 && ST; ++Pass_Count)
	{
	P4 = ZVTCO2[PV];
	X4 = ZPTCO1[P4]-ZPTCO1[PT];
	Y4 = ZPTCO2[P4]-ZPTCO2[PT];

	if(X4>0) SV = Y4<0 ? 4 : 1;
	else
	if(X4<0) SV = Y4>0 ? 2 : 3;
	else	 SV = Y4>0 ? 2 : 4;

	if(SL>SV) ST = 1;
	else
	if(SL<SV) ST = 0;
	else
	if(P2==P4) ER = 1, ST = 0;
	else
	{
	S24 = (long double)X2*Y4 - (long double)X4*Y2;

	DE4 = ((X4>0?X4:-X4)+(Y4>0?Y4:-Y4))/(3.0);
	DE2 = ((X2>0?X2:-X2)+(Y2>0?Y2:-Y2))/(3.0);

	H42 = S24 >DE2 ?-1 : S24 <-DE2 ? 1 : 0;
	H24 = S24 >DE4 ? 1 : S24 <-DE4 ?-1 : 0;

	ER = (!H42 && !H24)? 2:0;
	ST = S24 <0 && !ER;
	}
	if(ST) PS = PV, PV = ZPVCO5[PV];
	else if(ER) PV_Error = 1;
	}
	if(ER==0)
	{
	ZPVCO5[VT] = PV;
	if(PS) ZPVCO5[PS] = VT;
	else ZPTCO3[PT] = VT;
	continue;
	}
	else
	if(ER==1)
	{
	if(VT!=PV) ERASEVTBEAD(VT, PV);
	continue;
	}
	if(X2<0) X2 = -X2;
	if(X4<0) X4 = -X4;
	if(Y2<0) Y2 = -Y2;
	if(Y4<0) Y4 = -Y4;

	if(X2>Y2)
	split_vt_pv(VT--, PV, X2, X4, P2, P4);
	else
	split_vt_pv(VT--, PV, Y2, Y4, P2, P4);
	}
	/*END LOOP FOR EACH VECTOR*/
}
long FETCHITSELF(long LB, long VT)
{
long LE;
	LE = ZVTCO5[VT];
	while(LE&&(ZLVCO7[LE]!=LB))
	LE = ZLVCO3[LE];
	return(LE);
}
long FETCHREVERSE(long LB, long LV)
{
long VR, LS, LR, EC;
	LR = EC = 0;
	VR = ZVTCO6[ZLVCO2[LV]];
	LS = ZVTCO5[VR];
	while(LS)
	{
	if(ZLVCO7[LS]==LB) LR = LS, ++EC;
	LS = ZLVCO3[LS];
	}
	if(EC<=1) return(LR);
	/*GET-UP-DIRECT-SUCCESSOR*/
	LS = ZLVCO1[LV];
	while(ZLVCO2[LS]!=VR) LS = ZLVCO1[LS];
	return(LS);
}
void REMOVE_FROM_VT_CHAIN(long LV)
{
long VT;
	VT = ZLVCO2[LV];
	if(ZVTCO5[VT]==LV)
	{
	ZVTCO5[VT] = ZLVCO3[LV];
	return;
	}
	VT = ZVTCO5[VT];
	while(VT)
	{
	if(ZLVCO3[VT]==LV)
	{
	ZLVCO3[VT] = ZLVCO3[LV];
	return;
	}
	VT = ZLVCO3[VT];
	}
}
long CLEAR_LB_PTR(long Lv, long Ln)
/*From Lv To Ln*/
{
long LE, LS;
	DummyClip = 1;
	LS = Lv;
	REMOVE_FROM_VT_CHAIN(Ln);
	for(ZLVCO7[Ln] = 0; LS!=Ln; LS = ZLVCO1[LS])
	{
	ZLVCO7[LS] = 0;
	REMOVE_FROM_VT_CHAIN(LS);
	}
	LE = ZLVCO1[Ln];
	ZLVCO1[Ln] = Lv;
	return(LE);
}
long DUMMYJUNCTION(long LB, long LV)
{
long ER;
long DJ;
long LN; /*LV Coincidencer*/
long LF;
long LS;
	LN = FETCHREVERSE(LB, LV);
	DJ = LN;
	ER = LN ? 1 : 0;
	while(ER==1)
	{
	LF = LV;
	LS = LN;
	LV = ZLVCO1[LV];
	if(LV==LN) ER = 2;
	else
	{
	LN = FETCHREVERSE(LB, LV);
	ER = LN ? 1 : 0;
	while(ER&&(ZLVCO1[LN]!=LS))
	{
	ER = DUMMYJUNCTION(LB, LV);
	if(ER)
	{
		ZLVCO1[LF] = CLEAR_LB_PTR(LV, LN);
		LV = ZLVCO1[LF];
		LN = FETCHREVERSE(LB, LV);
		ER = LN ? 1 : 0;
	}
	}
	if(!ER)
		DJ_FIRSTFR_LV = ZLVCO1[LV];
	}
	}
	DJ_REVERSE_LV = DJ;
	return(ER);
}
long SET_INITIAL_VECTOR(long LB)
{
long LE, LV, LN, LZ, LM;
char badpoly = 0;

	LE = LV = ZLBCO3[LB];

	LN = FETCHREVERSE(LB, LV);
	LM = FETCHREVERSE(LB, LZ = ZLVCO1[LV]);

	if(LN||LM)
	do
	{
	LN = FETCHREVERSE(LB, LV = ZLVCO1[LZ]);
	LM = FETCHREVERSE(LB, LZ = ZLVCO1[LV]);
	}
	while((LN||LM)&&!(badpoly = (LV==LE||LZ==LE)));
	else badpoly = 0;

	if(badpoly)
	{
	CLEAR_LB_PTR(ZLVCO1[LE], LE);
	ZLBCO3[LB] = 0;
	PV_Error = 2;
	return(0);
	}
	return(ZLBCO3[LB]=LZ);
}
void DUMMYJUNCTIONANALYSIS(long LB)
{
long LV;
long LN;
long LE;
	LV = SET_INITIAL_VECTOR(LB);

	if(!LV)return;

	LE = LV;
	LN = ZLVCO1[LV];
	do /*For Each Non Coincidenced LV*/
	{
	DJ_FIRSTFR_LV = ZLVCO1[LN];

	if(FetchReverseLv(LN))
	{
	while(DUMMYJUNCTION(LB, LN))
	{
	LN = CLEAR_LB_PTR(LN, DJ_REVERSE_LV);
	ZLVCO1[LV] = LN;
	DJ_FIRSTFR_LV = ZLVCO1[LN];
	}
	do
	{
	LV = LN;
	LN = ZLVCO1[LV];
	}
	while(LV!=LE&&LN!=DJ_FIRSTFR_LV);
	}
	else
	LV = LN,
	LN = ZLVCO1[LV];
	}
	while(LV!=LE);
}
void Dix_write(void*Buf, size_t Count)
{
	if((Dix_Out_Pos+Count)>=Dix_Out_Len)
	{
	fwrite(Dix_Out_Buf, 1, Dix_Out_Pos, Dix_File);
	Dix_Out_Pos = 0;
	}
	memcpy(&Dix_Out_Buf[Dix_Out_Pos], Buf, Count);
	Dix_Out_Pos += Count;
}
void
Dix_close(void)
{
	if(Dix_Out_Buf!=0)
	{
	if(Dix_Out_Pos>0)
	fwrite(Dix_Out_Buf, 1, Dix_Out_Pos, Dix_File);
	fflush(Dix_File);
	Dix_Out_Pos = 0;
	}
#ifdef F_SIZE_MODULE
	if(Dix_Rewrite_FP_S)
	{
	long P = 0;
	fsetpos(Dix_File, &P);
	fwrite(&FP_S, sizeof(FP_S), 1, Dix_File);
	fflush(Dix_File);
	}
#endif
	fclose(Dix_File);
	if(ferror(Dix_File))zabort();
}
void Dix_output(void)
{
	long LD;
	long OX;
	long OY;
	long dx;
	long dy;
	long DX;
	long DY;
	long XL;
	long YL;
	long ZX;
	long ZY;
	long JJ;
	long fi;
	long ic;
	long cn;
	char DS;
	if(DIX_OpenS!=Z_Open_Shape)
	{
	char D = 0;
	Dix_write(&D, 1);
	D = (Z_Open_Shape+1)<<4;
	Dix_write(&D, 1);
	DIX_OpenS = Z_Open_Shape;
	}
	if(DIX_Layer!=Dix_Layer)
	{
	char D = 0;
	Dix_write(&D, 1);
	D = 64;
	Dix_write(&D, 1);
	Dix_write(&Dix_Layer, 2);
	DIX_Layer = Dix_Layer;
	}
	if(DIX_Fragment!=Z_Fragment)
	{
	char D = 0;
	Dix_write(&D, 1);

	if(Z_Fragment<(0XFFL+1))		D = 1;
	else if(Z_Fragment<(0XFFFFL+1)) D = 2;
	else if(Z_Fragment<(0XFFFFFFL+1)) D = 3;
	else							D = 4;

	Dix_write(&D, 1);

	Dix_write(&Z_Fragment, (size_t)D);
	DIX_Fragment = Z_Fragment;
	}
	dx = floor(ZZ_DELTA_X+0.5);
	dy = floor(ZZ_DELTA_Y+0.5);
	OX = 0;
	OY = 0;
	cn = 0;
	if(STATUS>0)
	fi = 0, ic = +1;
	else
	fi = ZLDLIM-1, ic = -1;
	for(LD = fi; cn<ZLDLIM-1; LD += ic, ++cn)
	{
	ZX = ZLDCO0[LD];
	ZY = ZLDCO1[LD];

#ifdef F_SIZE_MODULE
	if(Dix_Rewrite_FP_S)
	{
	if(ZX<CXMIN)CXMIN = ZX;
	if(ZX>CXMAX)CXMAX = ZX;
	if(ZY<CYMIN)CYMIN = ZY;
	if(ZY>CYMAX)CYMAX = ZY;
	}
#endif

	ZX += dx;
	ZY += dy;

#ifdef M_CONV_MODULE
	if(ZX<CXMIN)CXMIN = ZX;
	if(ZX>CXMAX)CXMAX = ZX;
	if(ZY<CYMIN)CYMIN = ZY;
	if(ZY>CYMAX)CYMAX = ZY;
#endif

	DX = ZX - OX;
	DY = ZY - OY;

	OX = ZX;
	OY = ZY;

	ZX = ZY = 0;

	if(DX<=0) DX = -DX, ZX |= 0200;
	if(DY<=0) DY = -DY, ZY |= 0200;

	JJ = 0;

	if(DX)while(!(DX%10)&&JJ<7) ++JJ, DX/= 10;
	ZX |= JJ<<3;

	JJ = 0;

	if(DY)while(!(DY%10)&&JJ<7) ++JJ, DY/= 10;
	ZY |= JJ<<3;

	if(DX==0) XL = 0;
	else if(DX < (0XFFL+1)) XL = 1;
	else if(DX < (0XFFFFL+1)) XL = 2;
	else if(DX < (0XFFFFFFL+1)) XL = 3;
	else XL = 4;

	if(DY==0) YL = 0;
	else if(DY < (0XFFL+1)) YL = 1;
	else if(DY < (0XFFFFL+1)) YL = 2;
	else if(DY < (0XFFFFFFL+1)) YL = 3;
	else YL = 4;

	ZX |= XL;
	ZY |= YL;

	Dix_write(&ZX, 1L);
	if(XL)
	Dix_write(&DX, XL);

	Dix_write(&ZY, 1L);
	if(YL)
	Dix_write(&DY, YL);
	}
	if(ZLDLIM-1)
	{
	DS = 0, Dix_write(&DS, 1);
	}
	if(ferror(Dix_File))
	{
	Print_Note_Message();
	fprintf(stderr, "\nFile %s Write ERROR", Dix_Name);
	Print_Note_Message();
	zabort();
	}
#ifdef F_INIT_MODULE
	dontworry();
#endif
}
char
ZDNEXT(void)
{
	if(Zd_File_Ptr>=Zd_File_Lim)
	{
	Zd_File_Buf[0] = 0;
	Zd_File_Lim = fread(Zd_File_Buf, 1, Zd_File_Lim, Zd_File);
	ZZ_RP_KB += Zd_File_Lim;
	Zd_File_Eof = (Zd_File_Lim == 0);
	Zd_File_Ptr = 0;
	}
	return Zd_File_Buf[Zd_File_Ptr++];
}
void ZDREAD(char*B, char L)
{
register char i;
	if(Zd_File_Ptr+L>=Zd_File_Lim)
	{
	for(i = 0; i<L; ++i)
	B[i] = ZDNEXT();
	return;
	}
	memcpy(B, &Zd_File_Buf[Zd_File_Ptr], L);
	Zd_File_Ptr += L;
}
#define Z_LEN(Z) ((Z)&7)
#define Z_REV(Z) ((Z)&128)
#define Z_TEN(Z) (((Z)&56)>>3)
void ZDREAD_X(long*D, char Z)
{
D[0] = 0;
ZDREAD((char *)D, Z_LEN(Z));

if(Z_REV(Z))D[0] *=-1L;
for(Z = Z_TEN(Z) ; Z; --Z)D[0] *= 10;
}
void
ZDREAD_P(void)
	{
	char Z;
	long L;
	long X;
	long Y;

	long D[2];

	ZLDLIM = 0;

	ZDREAD(&Z, 1);
	if(!Z)
	{
	ZDREAD(&Z, 1), L = (Z&(7<<4))>>4;
	if(L)
		{
#ifdef F_EVAL_MODULE
		fprintf(stderr, "\nABNORMAL INPUT FILE FORMAT");
		zabort();
#endif
		if(L==4)
			{
			Zd_Layer[0] = 0;
			ZDREAD((char*)Zd_Layer, 2);
			return;
			}
		Z_Open_Shape = L-1;
		return;
		}
	Zd_Fragment[0] = 0;
	ZDREAD((char*)Zd_Fragment, Z);

	if(Zd_File_Eof)
	Zd_Fragment[0] = LONG_MAX;
	return;
	}
	X = Y = 0;
	do
	{
	ZDREAD_X(&D[0], Z);

	ZDREAD(&Z, 1);
	ZDREAD_X(&D[1], Z);

	X += D[0];
	Y += D[1];

	L = ZLDNEXT();

	ZLDCO0[L] = X-Zd_Delta_X;
	ZLDCO1[L] = Y-Zd_Delta_Y;

	ZDREAD(&Z, 1);
	}
	while(Z);
	}
void Z_Read_Polygon(void) {ZDREAD_P();}

void
Ferror(FILE*File, char*Name)
{
	if(File==NULL)return;

	if(ferror(File))
	{
	fprintf(stderr, "\n\nERROR ON FILE %s\n", Name);
	zabort();
	}
}
FILE*Fopen(char*Name, char*Mode)
{
	FILE*File;
	File = fopen(Name, Mode);
	if(File==NULL)
	{
		fprintf(stderr, "\n\nFile %s Open ERROR\n", Name);
		zabort();
	}
	return(File);
}
bool Fcheck(char*Name, char*Mode)
{
	bool rc = false; 
	FILE*File;
	File = fopen(Name, Mode);
	if(File!=NULL)
	{
		fclose(File); 
		rc = true;
	}
	return rc;
}
char*SS_Fetch(char*Name)
{
	long i;
	char*n;
	i = strlen(Name)+1;
	if((S_BLEN-S_BPOS)<=i)zabort();

	memcpy(&S_BTOP[S_BPOS], Name, i);
	n = &S_BTOP[S_BPOS];
	S_BPOS+= i;

	return(n);
}
char ZZ_Hash(char*s)
{
	char i = 0;
	while(*s) i = (i + (*s++))&63;
	return(i);
}
/**/
typedef struct
{
char*N;
char X;
} MM_layer;
MM_layer*MM_Layer;
/**/
long MM_Layer_Old;
long MM_Layer_Lim;
long MM_Layer_Max;
/**/
CHAR MM_Hash[64];
long MM_Fetch(char*Name)
{
	long i;
	long I;
	i = ZZ_Hash(Name);

	I = MM_Hash[i];
	while(I)
	{
	if(stricmp(Name, MM_Layer[I].N)==0)break;
	I = MM_Layer[I].X;
	}
	return(I);
}
long MM_Insert(char*N)
{
	long i;
	long I;
	I = MM_Layer_Lim++;
	if(MM_Layer_Lim>=MM_Layer_Max)zabort();

	MM_Layer[I].N = N;

	i = ZZ_Hash(N);

	MM_Layer[I].X = MM_Hash[i];
	MM_Hash[i] = I;

	return(I);
}
void Make_File_Name_Main(char*Sd, long Id)
{
long Ij;
static char*Sp = "abcxzjps";

sprintf(Sd, "%08ld", Id);

for(Ij = 0; Sp[Ij]!=0; ++Ij)
{
if(Sd[Ij]== 0 )continue;
if(Sd[Ij]=='0')Sd[Ij] = Sp[Ij];
if(Sd[Ij]=='1')Sd[Ij] = 'e';
}
memcpy(Sd, LZ_S.FileP, 3);
}
void Make_BDB_File_Name(char*Sd, long Id)
{
Make_File_Name_Main(Sd, Id);

strncat(Sd, ".xzb", 5);
}
void Make_File_Name(char*Sd, long Id)
{
Make_File_Name_Main(Sd, Id);

strncat(Sd, ".xzq", 5);
}
void Find_ILayer(void)
{
long I;
fpos_t n;
long i;
	I = Z_Layers_Cnt;
	for(i = n = 0; i<I; ++i)
	{
	Z_Layer[i].Exist = MM_Fetch(Z_Layer[i].Name);
	if(Z_Layer[i].Exist)++n;
	}
	if(n<I)
	{
	Print_Note_Message();
	for(i = 0; i<I; ++i)
	{
	if(!Z_Layer[i].Exist)
	fprintf(stderr, "\nLAYER %s is NOT VALID", Z_Layer[i].Name);
	}
	Print_Note_Message();
	zabort();
	}
	for(i = 0; i<I; ++i)
	{
	n = FPOS_2 + (Z_Layer[i].Exist-1)*sizeof(LX_S);

	fsetpos(LX_S_FILE, &n);
	n = fread(&Z_Layer[i].LX_S, sizeof(LX_S), 1, LX_S_FILE);

	if(n!=1)zabort();
	}
}
void
Init_ILayer_File(FX_Struct*L)
{
	L->File.Fil = Fopen(L->PdbN, "rb");

	fread(&FP_S, sizeof(FP_S), 1, L->File.Fil);

	L->Delta_X = FP_S.Delta_X;
	L->Delta_Y = FP_S.Delta_Y;

	L->File.Buf = (char*)malloc(1024);

	L->File.Buf_Len = 1024;
	L->File.Buf_Pos = 1024;

	L->File.Buf_Eof = 0;
}
void INIT_OLayer(FX_Struct*L)
{
	tmpnam(L->PdbN);

	L->File.Fil = Fopen(L->PdbN, "w+b");

	FP_S.Delta_X = 0;
	FP_S.Delta_Y = 0;

	fwrite(&FP_S, sizeof(FP_S), 1, L->File.Fil);

	L->LX_S.CXMIN = LZ_S.CXMIN;
	L->LX_S.CXMAX = LZ_S.CXMAX;
	L->LX_S.CYMIN = LZ_S.CYMIN;
	L->LX_S.CYMAX = LZ_S.CYMAX;

	L->LX_S.XBCNT = LZ_S.XBCNT;
	L->LX_S.YBCNT = LZ_S.YBCNT;
	L->LX_S.XBSIZ = LZ_S.XBSIZ;
	L->LX_S.YBSIZ = LZ_S.YBSIZ;

	L->File.Buf = (char*)malloc(1024);

	L->File.Buf_Len = 1024;
	L->File.Buf_Pos = 0;

	Dix_Out_Buf = L->File.Buf;

	Dix_Out_Pos = L->File.Buf_Pos;
	Dix_Out_Len = L->File.Buf_Len;

	Dix_File = L->File.Fil;
	Dix_Name = L->PdbN;
}
void INIT_ILayer(FX_Struct*L, long n)
{
	Make_File_Name(L->PdbN, L->LX_S.PDBI);

	Init_ILayer_File(L);

	L->Lcode = 1<<(2*n);
	L->Lmask = (L->Lcode<<1)|L->Lcode;

	LYCODE[n] = L->Lcode;
	LYMASK[n] = L->Lmask;

	L->Fragment = 0;
}
void Delete_File(char*Name)
{
	unlink(Name);
}
void renameoutput(void)
{
	long i;
	fpos_t I;
	char Name[20];
	O_Layer.LX_S.ER1I = 0;
	O_Layer.LX_S.BDBI = 0;
	O_Layer.LX_S.PLTI = 0;

	i = MM_Fetch(O_Layer.Name);
	if(i!=0)
	{
	I = FPOS_2 + sizeof(LX_S)*(i-1);
	fsetpos(LX_S_FILE, &I);

	I = fread(&LX_S, sizeof(LX_S), 1, LX_S_FILE);
	if(I!=1)zabort();

	if(LX_S.PDBI)
	Make_File_Name(Name, LX_S.PDBI);
	else
	{
	LZ_S_Changed = 1;
	LX_S.PDBI = LZ_S.FiLID++;
	Make_File_Name(Name, LX_S.PDBI);
	}
	O_Layer.LX_S.PDBI = LX_S.PDBI;
	}
	else
	{
	O_Layer.LX_S.PDBI = LZ_S.FiLID;

	LZ_S_Changed = 1;
	Make_File_Name(Name, LZ_S.FiLID++);

	i = MM_Insert(SS_Fetch(O_Layer.Name));
	}
	Delete_File(Name);
	rename(O_Layer.PdbN, Name);

	I = FPOS_2 + sizeof(LX_S)*(i-1);
	fsetpos(LX_S_FILE, &I);

	I = fwrite(&O_Layer.LX_S, sizeof(LX_S), 1, LX_S_FILE);
	if(I!=1)zabort();

	fflush(LX_S_FILE);
}
#include <IO.H>
void Lz_Lx_Close(void)
{
	long i;
	long I;
	long L;
	if(LZ_S_Changed)
	{
	fsetpos(LZ_S_FILE, &FPOS_2);
	fwrite(&LZ_S, sizeof(LZ_S), 1, LZ_S_FILE);
	fflush(LZ_S_FILE);
	}
	L = FPOS_3;

	fsetpos(LZ_S_FILE, &FPOS_3);
	i = MM_Layer_Old;
	while(i<MM_Layer_Lim)
	{
	I = strlen(MM_Layer[i].N)+1;
	fwrite(MM_Layer[i].N, 1, I, LZ_S_FILE);
	L += I;
	++i;
	}
	fflush(LZ_S_FILE);
	chsize(fileno(LZ_S_FILE), L);
	fclose(LZ_S_FILE);
	fclose(LX_S_FILE);
}
void Z_Check_Closure(void)
{
char ER = 0;
	ER |= ZLDCO0[0]!=ZLDCO0[ZLDLIM-1];
	ER |= ZLDCO1[0]!=ZLDCO1[ZLDLIM-1];
	if(ER)
	{
	long L = ZLDNEXT();
	ZLDCO0[L] = ZLDCO0[0];
	ZLDCO1[L] = ZLDCO1[0];
	}
}
typedef long compare(long);
compare XOR_1;
long XOR_1(long S)
{
	if((S&3)==1)return(1);
	return(0);
}
compare AND_1;
long AND_1(long S)
{
	if((S&3)==3)return(1);
	return(0);
}
compare AN_D;
long AN_D(long S)
{
register short I;
	for(I=0; I<Z_Layers_Cnt; ++I)
	if(!(S&LYMASK[I])) return(0);
	return(1);
}
compare O_R;
long O_R(long S)
{
	if(S) return(1);
	return(0);
}
long AND_NOT_MASK=0XFFFFFFFCl;
compare AND_NOT;
long AND_NOT(long S)
{
	if(!S) return(0);
	if(S&AND_NOT_MASK) return(0);
	return(1);
}
long OVR_ELIM_MASK;
compare OVR_ELIM;
long OVR_ELIM(long S)
{
	if(S&OVR_ELIM_MASK) return(1);
	return(0);
}
compare X_OR;
long X_OR(long S)
{
register short I;
	if(!S) return(0);
	for(I=0; I<Z_Layers_Cnt; ++I)
	if((S|LYMASK[I])==LYMASK[I]) return(1);
	return(0);
}
compare NO_T;
long NO_T(long S)
{
	if(!S) return(0);
	if((S|12)==12) return(1);
	return(0);
}
compare *True_Stat;
compare *Cmp_Modules[6] ={OVR_ELIM, AND_NOT, AN_D, X_OR, O_R, NO_T};
#ifdef F_EVAL_MODULE
void
Dxb_Lb_Image_Maker(long, char*);
#endif
void TRAVERSE(long VT)
{
long LB, LE, LV, V3 = VT, V4;
char DJ = 0;
	LB = ZLBNEXT(), LE = ZLVLIM;
	ZLBCO3[LB] = LE;
	ZLDLIM = 0;
	do
	{
	ZVTCO8[V3] = 1;
	LV = ZLVNEXT();
	ZLVCO3[LV] = 0;

	ZLVCO2[LV] = V3;
	ZLVCO7[LV] = LB;

	ZLVCO5[LV] = ZVTCO1[V3];
	ZLVCO6[LV] = ZVTCO2[V3];
	ZLVCO1[LE] = ZVTCO5[V3] = LV;
	LE = LV;
	V4 = V3;
	V3 = FETCHSUCCESSOR(ZVTCO6[V3]);
	if(ZVTCO6[V4]==V3) DJ = 1;
	}
	while(V3!=VT);
	LE = ZLVCO1[LV] = ZLBCO3[LB];
	if(DJ)
	{
	DUMMYJUNCTIONANALYSIS(LB);
	}
	DETERMINE_STAT_2(LB);
	if(STATUS==+1)return;
#ifdef F_EVAL_MODULE
Print_Note_Message();
fprintf(stderr, "\nROLL BACK BAD...SUMANGLES = %g", sumangles);
fprintf(stderr, "\nATTEMPTING TO GENERATE NON RIGHT POLYGON.");
Print_Note_Message();
fprintf(stderr, "\nNote!Try To RE-Run With SCALE-PARAMETER.");
Print_Note_Message();

Dxb_Lb_Image_Maker(LB, "ERROR");
zabort();
#else
	ZLBCO3[LB] = 0;
	LV = LE;
	do
	{
	REMOVE_FROM_VT_CHAIN(LV);
	LV = ZLVCO1[LV];
	}
	while(LV!=LE);
	ZLBLIM = LB;
	ZLVLIM = LE;
#endif
}
void
FORCE_C_POLYGONS(void)
	{
	long V;

	ZLBLIM = 1;
	ZLVLIM = 1;

	V = ZVTLIM;
	memset(ZVTCO8, 0, V*sizeof(ZVTCO8[0]));
	memset(ZVTCO5, 0, V*sizeof(ZVTCO5[0]));

	for(V = 2; V<ZVTLIM; ++V)
	{
	if(!ZVTCO6[V]||ZVTCO8[V])
		continue;
	if(!True_Stat(ZVTCO4[V]))
		continue;
	TRAVERSE(V);
	}
	COMBINE_C_POLYGONS();
	}
long
Unload_Lv_Polygon(long LB)
{
long LE;
long LV;
long LN;
long LD;

double A1;
double A2;

	LV = ZLBCO3[LB];
	LN = ZLVCO2[LV];
	LN = FETCHITSELF(LB, ZVTCO6[LN]);

	LV = ZLVCO1[LV];
	LD = ZLVCO2[LV];
	LD = FETCHITSELF(LB, ZVTCO6[LD]);

	if(LN||LD)
	{
	LV = SET_INITIAL_VECTOR(LB);
	if(!LV)return(0);
	}
	LE = LV;
	LN = ZLVCO1[LV];
	A1 = 361.0;
	ZLDLIM = 0;
	do
	{
	DJ_FIRSTFR_LV = ZLVCO1[LN];

	if(FetchReverseLv(LN))
	while(DUMMYJUNCTION(LB, LN))
	{
	LN = CLEAR_LB_PTR(LN, DJ_REVERSE_LV);

	ZLVCO1[LV] = LN;
	DJ_FIRSTFR_LV = ZLVCO1[LN];
	}
	LD = ZLDNEXT();

	ZLDCO0[LD] = ZPTCO1[ZLVCO6[LV]];
	ZLDCO1[LD] = ZPTCO2[ZLVCO6[LV]];

	A2 = COMPLVANGLE(LV);
	if(ENVEQ(A1, A2, 1E-3))
	{
	ZLDLIM -= 1;
	ZLDCO0[LD-1] = ZLDCO0[LD];
	ZLDCO1[LD-1] = ZLDCO1[LD];

	if(LD-1)
	A2 = COMPLDANGLE(LD-1);
	}
	A1 = A2;
	LV = LN;
	LN = ZLVCO1[LV];

	while(LN!=DJ_FIRSTFR_LV&&LV!=LE)
	{
	LD = ZLDNEXT();

	ZLDCO0[LD] = ZPTCO1[ZLVCO6[LV]];
	ZLDCO1[LD] = ZPTCO2[ZLVCO6[LV]];

	A1 = COMPLVANGLE(LV);
	LV = LN;
	LN = ZLVCO1[LV];
	}
	/*End Of Broken Junction*/
	}
	while(LV!=LE);

	LD = ZLDNEXT();

	ZLDCO0[LD] = ZLDCO0[0];
	ZLDCO1[LD] = ZLDCO1[0];

	return(ZLDLIM>3);
}
void Initial_Setup_1(void)
{
	S_BPOS = 1;
	S_BLEN = 4096;
	S_BTOP = (char*)calloc(4096, 1);

	S_BLEN-= 20;
	LZ_S_NAME= &S_BTOP[S_BLEN];
	S_BLEN-= 20;
	LX_S_NAME= &S_BTOP[S_BLEN];

	memcpy(LZ_S.FileP, "M2$\n", 4);
	memcpy(LZ_S.Err1P, "E1$\n", 4);
	memcpy(LZ_S.PlotP, "PT$\n", 4);
	memcpy(LZ_S.TempP, "TM$\n", 4);

	Make_File_Name(LZ_S_NAME, 0);
	Make_File_Name(LX_S_NAME, 1);

	LZ_S.FiLID = 2;

	True_Stat = O_R;
	IPOINT = IPOINT3;
	SPLITLV = SPLITLV_M;
	COMPSPLITLV = COMPSPLITLV_M;
	MERGELVCHAIN = MERGELVCHAIN_M;


}
void Initial_Setup_2(void)
{
	char*b;
	long i;
	long m;
	long n;

	if(!Fcheck(LZ_S_NAME, "r+b")) { 
		LZ_S.LVCNT = 2000;
		LZ_S.EMSIZ = 2048;

		LZ_S.UN001 = 1;
		LZ_S.FiLID = 2;

		LZ_S_NAME = (char*)malloc(20);
		LX_S_NAME = (char*)malloc(20);

		memcpy(LZ_S.FileP, "M2$\n", 4);
		memcpy(LZ_S.Err1P, "E1$\n", 4);
		memcpy(LZ_S.PlotP, "PT$\n", 4);
		memcpy(LZ_S.TempP, "TM$\n", 4);

		Make_File_Name(LZ_S_NAME, 0);
		Make_File_Name(LX_S_NAME, 1);

		LZ_S_FILE = Fopen(LZ_S_NAME, "wb");
		LX_S_FILE = Fopen(LX_S_NAME, "wb");

		LZ_S.CXMIN*= LZ_S.UN001;
		LZ_S.CXMAX*= LZ_S.UN001;
		LZ_S.CYMIN*= LZ_S.UN001;
		LZ_S.CYMAX*= LZ_S.UN001;
		LZ_S.UN003*= LZ_S.UN001;

		fwrite(&FP_S, sizeof(FP_S), 1, LZ_S_FILE);
		fwrite(&FP_S, sizeof(FP_S), 1, LX_S_FILE);
		fwrite(&LZ_S, sizeof(LZ_S), 1, LZ_S_FILE);

		fclose(LZ_S_FILE);
		fclose(LX_S_FILE);
	} 

	LZ_S_FILE = Fopen(LZ_S_NAME, "r+b");
	LX_S_FILE = Fopen(LX_S_NAME, "r+b");

	i = fread(&FP_S, sizeof(FP_S), 1, LZ_S_FILE);
	m = fread(&FP_S, sizeof(FP_S), 1, LX_S_FILE);
	n = fread(&LZ_S, sizeof(LZ_S), 1, LZ_S_FILE);
	if(i!=1||m!=1||n!=1)zabort();

	UN001 = LZ_S.UN001;
	UN002*= UN001;
	STSIZ = 1.0/(double)UN001;

	CXMIN = LZ_S.CXMIN;
	CXMAX = LZ_S.CXMAX;
	CYMIN = LZ_S.CYMIN;
	CYMAX = LZ_S.CYMAX;

	XBSIZ = LZ_S.XBSIZ;
	YBSIZ = LZ_S.YBSIZ;
	XBCNT = LZ_S.XBCNT;
	YBCNT = LZ_S.YBCNT;
	LVCNT = LZ_S.LVCNT;

#ifdef __BORLANDC__
	EMSIZ = 0;
#else
	EMSIZ = LZ_S.EMSIZ;
	LVCNT = ((EMSIZ+200.)/350.)*LVCNT;
#endif

	BLCNT = XBCNT*YBCNT;

	MM_Layer = (MM_layer*)calloc(256, 5);
	MM_Layer_Lim = 1;
	MM_Layer_Max = 256;

	b = &S_BTOP[S_BLEN-200];

	fsetpos(LZ_S_FILE, &FPOS_1);
	b[0] = 1;
	while(!feof(LZ_S_FILE))
	{
	i = 0;
	do
	if(!fread(&b[i], 1, 1, LZ_S_FILE))
	b[i] = 0;
	while(b[i++]);

	if(b[0]==0)break;

	i = MM_Insert(SS_Fetch(b));
	}
	MM_Layer_Old = MM_Layer_Lim;

	fgetpos(LZ_S_FILE, &FPOS_3);

	if(ferror(LZ_S_FILE))
	zabort();
}
#ifdef F_SIZE_MODULE
#define F_EVAL_MODULE
#endif
#ifndef F_EVAL_MODULE
void SET_INSIDE_ALL(void)
{
long PV, VR;
	SI_Error = 1;
	for(PV = FETCHSUCCESSOR(SI_V1); PV!=SI_V2; PV = FETCHSUCCESSOR(PV))
	{
	VR = ZVTCO6[PV];

	if(FETCHITSELF(SI_LB, PV)) return;
	if(FETCHITSELF(SI_LB, VR)) return;
	}
	SI_Error = 0;
}
void POINT_VECTOR_CLASS
(long LB)
{
long V4, V5, V6;
long LE, LV, LN;

/*After DUMMYJUNCTIONANALYSIS Only*/

	LV = ZLBCO3[LB];
	LE = LV;

	LN = ZLVCO1[LV];
	V4 = ZLVCO2[LV];
	V4 = ZVTCO6[V4];

	SI_LB = LB;
	/*For Each LV*/
	do
	{
	V5 = ZLVCO2[LN];
	V6 = ZVTCO6[V5];

	if(STATUS==+1)
	SI_V1 = V4,
	SI_V2 = V5;
	else
	SI_V1 = V5,
	SI_V2 = V4;
	SET_INSIDE_ALL();

	if(SI_Error)
	{
	return;
	}
	else V4 = V6, LV = LN, LN = ZLVCO1[LV];
	}
	while(LV!=LE);
}
#else
void
SET_BIT_1(long v)
{
	while(v)
	{
	ZLBCO7[ZLVCO7[v]] |= 1;
	v = ZLVCO3[v];
	}
}
void
SET_BIT_2(long v)
{
static long b;
	while(v)
	{
	b = ZLVCO7[v];
	if(b!=SI_LB) ZLBCO7[b] |= 2;
	v = ZLVCO3[v];
	}
}
#define ZJsNext(JS) if(++(JS)==ZJSMAX)ZOUTABORT()
void
SET_INSIDE_ONE(void)
{
long Lv_1;
long Lv_2;
long Vt_1;
long Vt_2;

long Lv;
long Pv;
long JS;
	Lv = SI_LE;

	Vt_1 = ZLVCO2[Lv];
	Vt_2 = ZVTCO6[Vt_1];

	while(ZLVCO8[Lv]!=SI_LB)
	{
	ZVTCO4[Vt_1] |= SI_ST;
	ZVTCO4[Vt_2] |= SI_ST;

	SET_BIT_2(ZVTCO5[Vt_1]);
	SET_BIT_2(ZVTCO5[Vt_2]);

	ZLVCO8[Lv] = SI_LB;
	Lv = ZLVCO1[Lv];
	JS = ZJSLIM;

	Vt_1 = ZLVCO2[Lv];
	for(Pv = ZPTCO3[ZVTCO1[Vt_1]]; Pv; Pv = ZPVCO5[Pv])
	{
	Lv_1 = ZVTCO5[Pv];
	if(!Lv_1)continue;

	Lv_2 = Lv_1;
	do
	/*Border?*/
		if(SI_LB==ZLVCO7[Lv_2])
		return;
	while((Lv_2 = ZLVCO3[Lv_2])!=0);

	if(Pv==Vt_2)continue;
	if(Pv==Vt_1)continue;

	if(ZLVCO8[Lv_1]!=SI_LB)
		{
		ZJsNext(JS);
		ZJSCO0[JS] = Lv_1;
		}
	}
	ZJSLIM = JS;
	Vt_2 = ZVTCO6[Vt_1];
	}
}
#undef JsNext
void
SET_INSIDE_ALL(void)
{
long PV, LS, VR;
	SI_Error = 1;
	for(PV = FETCHSUCCESSOR(SI_V1); PV!=SI_V2; PV = FETCHSUCCESSOR(PV))
	{
	VR = ZVTCO6[PV];
	LS = ZVTCO5[PV];

	if(FETCHITSELF(SI_LB, PV)) return;
	if(FETCHITSELF(SI_LB, VR)) return;

	if(LS==0) continue;

	ZJSCO0[ZJSLIM = 1] = LS;
	while(ZJSLIM>0)
	{
	SI_LE = ZJSCO0[ZJSLIM--];
	SI_FL = 1, SET_INSIDE_ONE(), SI_FL = 0;
	}
	ZLBCO7[SI_LB] |= 2;
	}
	SI_FL = SI_Error = 0;
}
void
POINT_VECTOR_CLASS
(long LB, long SX, long SY)
{
long V3, V4, V5, V6;
long LE, LV, LN, LT;

/*After DUMMYJUNCTIONANALYSIS Only*/

	LV = ZLBCO3[LB];
	LE = LV;

	LN = ZLVCO1[LV];
	V3 = ZLVCO2[LV];
	V4 = ZVTCO6[V3];

	SI_LB = LB;
	SI_ST = SY;

	PV_Error = 0;
	SI_PA_FL = 1;

	/*For Each LV*/
	do
	{
	SET_BIT_1(ZVTCO5[V3]);
	SET_BIT_2(ZVTCO5[V4]);

	V5 = ZLVCO2[LN];
	V6 = ZVTCO6[V5];

	SI_V1 = V4;
	SI_V2 = V5;

	SET_INSIDE_ALL();

	if(Target_Function<0&&(ZVTCO4[V3]&SX))
	ZVTCO4[V3] |= SY;

	ZVTCO4[V3] |= SX;

	if(SI_Error)
	{
	PV_Error = 1;
	do
	{
	LV = LN;
	LN = ZLVCO1[LV];
	V3 = ZLVCO2[LV];
	V4 = ZVTCO6[V3];
	LT = FETCHITSELF(LB, V4);
	}
	while(LT);
	}
	else V3 = V5, V4 = V6, LV = LN, LN = ZLVCO1[LV];
	}
	while(LV!=LE);

	ZLBCO7[SI_LB] |= 1;
	SI_PA_FL = 0;
}
#endif
void
ZZ_SET_RSTAT(long I)
{
	long S;
	long K;
	long T;
	long J;

	S = ZLBCO3[I];
	if(S==0)
	{
	return;
	}
	K = S;
	T = ZLBCO1[I];
	do
	{
	J = ZLVCO2[K];
	if(T==-1)J = ZVTCO6[J];

#ifdef F_EVAL_MODULE
	ZVTCO4[J] |= 1;
#else
	ZVTCO4[J] = 1;
#endif
	K = ZLVCO1[K];
	}
	while(K!=S);
}
#ifdef F_SIZE_MODULE
#undef F_EVAL_MODULE
#endif
