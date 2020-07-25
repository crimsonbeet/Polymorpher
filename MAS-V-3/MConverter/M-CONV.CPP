/**/
#include "stdafx.h" 

#ifndef M_CONV_MODULE
#define M_CONV_MODULE
#endif

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
#ifndef __BORLANDC__
//#define stricmp strcmpl
#endif
/**/
#ifdef __BORLANDC__
#include<VALUES.H>
#else
#include<FLOAT.H>
#define MAXFLOAT FLT_MAX
#endif
/**/
#include<LIMITS.H>
#include<PROCESS.H>
/**/

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510
#endif

#define	EOS	258
/**/
#define YY_01_FLAG YY_ITEM_FLAGS[0]
#define YY_02_FLAG YY_ITEM_FLAGS[1]
#define YY_03_FLAG YY_ITEM_FLAGS[2]
#define YY_06_FLAG YY_ITEM_FLAGS[3]
#define YY_07_FLAG YY_ITEM_FLAGS[4]
#define YY_08_FLAG YY_ITEM_FLAGS[5]

#define YY_10_FLAG YY_ITEM_FLAGS[6]
#define YY_11_FLAG YY_ITEM_FLAGS[7]
#define YY_12_FLAG YY_ITEM_FLAGS[8]
#define YY_13_FLAG YY_ITEM_FLAGS[9]

#define YY_20_FLAG YY_ITEM_FLAGS[10]
#define YY_21_FLAG YY_ITEM_FLAGS[11]
#define YY_22_FLAG YY_ITEM_FLAGS[12]
#define YY_23_FLAG YY_ITEM_FLAGS[13]

#define YY_30_FLAG YY_ITEM_FLAGS[14]
#define YY_31_FLAG YY_ITEM_FLAGS[15]
#define YY_38_FLAG YY_ITEM_FLAGS[16]
#define YY_39_FLAG YY_ITEM_FLAGS[17]

#define YY_40_FLAG YY_ITEM_FLAGS[18]
#define YY_41_FLAG YY_ITEM_FLAGS[19]
#define YY_42_FLAG YY_ITEM_FLAGS[20]
#define YY_43_FLAG YY_ITEM_FLAGS[21]

#define YY_50_FLAG YY_ITEM_FLAGS[22]
#define YY_51_FLAG YY_ITEM_FLAGS[23]

#define YY_62_FLAG YY_ITEM_FLAGS[24]
#define YY_66_FLAG YY_ITEM_FLAGS[25]

#define YY_70_FLAG YY_ITEM_FLAGS[26]
#define YY_71_FLAG YY_ITEM_FLAGS[27]
#define YY_72_FLAG YY_ITEM_FLAGS[28]

#define YY210_FLAG YY_ITEM_FLAGS[29]
#define YY220_FLAG YY_ITEM_FLAGS[30]
#define YY230_FLAG YY_ITEM_FLAGS[31]

char YY_ITEM_FLAGS[64];

void YY_CLEAR_FLAGS(void)
{
	static double Zero = 0;
	register char I;
	for(I = 0; I<8; ++I) memcpy(&YY_ITEM_FLAGS[I*8], &Zero, 8);
}
void zabort(void);
int yyparse(void);
long YY_C_STR_N;
void yyerror(char*message)
{
	fprintf(stderr, "\nAT %ld ERRORS WERE DETECTED", YY_C_STR_N);
	zabort();
}
/**/
typedef void ZZ_fun(void);
ZZ_fun *Zz_Oopen;
ZZ_fun *Zz_Write;
ZZ_fun *Zz_Close;
ZZ_fun *Zi_Close;
ZZ_fun *ZZ_Parse;
ZZ_fun *ZZ_Cscan;
/**/
long ZZ_Funum = 1;
long Is_Funum = 2;
/**/
#define Is_Mas_Input (Is_Funum==0)
#define Is_Sou_Input (Is_Funum==1)
#define Is_Dxf_Input (Is_Funum==2)
/**/
void Terminate_Vertex(void);
void Terminate_Insert(void);
void Terminate_Poly_1(void);

void Terminate_Trace(void);
/**/
long I_Str;

char*YY_1String; /*COMMON STRING*/
char*YY_2String; /*COMMON NAME*/
char*YY_3String; /*TABLE NAME*/
char*YY_4String; /*BLOCK-CELL NAME*/
char*YY_5String; /*LAYER NAME. DURATION VERY SMALL*/
char*YY_6String; /*TEXTUAL STRING*/
char*YY_7String; /*ATTRIBUTE TAG*/
/**/
long Bit_70_01;
long Bit_70_02;
long Bit_70_04;
long Bit_70_08;
long Bit_70_16;
long Bit_71_01;
long Bit_71_02;
long Bit_71_04;
long Bit_71_08;
long Bit_71_16;
/**/
double Item_10_Val;
double Item_11_Val;
double Item_12_Val;
double Item_13_Val;
double Item_20_Val;
double Item_21_Val;
double Item_22_Val;
double Item_23_Val;
double Item_30_Val;
double Item_31_Val;
double Item_38_Val;
double Item_39_Val;
double Item_40_Val;
double Item_41_Val;
double Item_42_Val;
double Item_43_Val;
double Item_50_Val;
double Item_51_Val;
double Item210_Val;
double Item220_Val;
double Item230_Val;
/**/
long Item_62_Val;
long Item_66_Val;
long Item_70_Val;
long Item_71_Val;
long Item_72_Val;
/**/
double ZZ_START_WIDTH;
double ZZ_ENDIN_WIDTH;
/**/
typedef short Z_YY_LEX(void);
Z_YY_LEX * yylex;
/**/
short DXFyylex(void);
short YY_INT_LEX(void);
short YY_FLOAT_LEX(void);
/**/
double*VERCOX;
double*VERCOY;
double*VERSTW;
double*VERENW;

double MAX_WIDTH;
/**/
double**VERCOR[5] = {&VERCOX, &VERCOY, &VERSTW, &VERENW, NULL};
/**/
double XCOVER[9];
double YCOVER[9];
double YY_Buldge;
/**/
char Sized_Shape;
char without_pcheck = 0;
long error_level = 0;

long VV_Cover;
char YY_Interpolate;
/**/
double NXFROM, NYFROM; /*(FROM-TO-NEXT)-NORMAL-VECTOR-COORD*/
double XELBOW, YELBOW; /*ELBOW-COORDINATES*/
double Epsilon = M_PI; /*IN MICRONS*/
double Epsilon_2 = M_PI/3;
/**/
long VERLIM;
long VERMAX;
/**/
long Last_Segment;
/**/
char I_LS_DF;
char I_FR_DF;
char I_DEF_P_V;
char I_DEF_N_V;
double I_VAL_PVX;
double I_VAL_PVY;
double I_VAL_NVX;
double I_VAL_NVY;
/**/
struct YY_layer
{
FILE*z;
FILE*x;
char*Z;
char*X;
char*I;
char*O;
char*B;
char o;
long L;
long P;
long n;
long p;
long v;
};
/*(z) TEMP FILE.(Z) NAME OF FILE*/
/*(x) TEMP FILE FOR LINE ITEMS.(X) NAME OF FILE*/
/*(I) INPUT LAYER NAME*/
/*(O) OUTPUT NAME (NUMBER FOR SOU); (o) BINARY VALUE OF SOU NAME*/
/*(P) POS IN BUFFER*/
/*(L) LENGTH OF BUFFER*/
/*(B) BUFFER*/
/*(p) TOTAL POLYGONS*/
/*(v) TOTAL VECTORS*/
/*(n) NEXT LAYER IN HASH*/
struct YY_layer*YY_Layer;

char YY_Layer_All;
long YY_Layer_Lim;
long YY_Layer_Max;
long YI_Layer;
long LI_Layer;
/**/
char YY_BOUND;
/**/
long YY_I_VAL;
double YY_F_VAL;
/**/
char Is_Error_Output;
void Dxb_Ld_Image_Maker(char closure, char*s);
void
yyoverflow
(char *S, void *ss, long SS, void *vs, long VS, void *ls, long LS, long *MD)
{
	fprintf(stderr, "\n%s", S);
	fprintf(stderr, "\nyyss = %ld yyvs = %ld yyls = %ld maxdepth = %ld",
	SS, VS, LS, MD[0]);
	zabort();
	free(ss), free(vs), free(ls);
}


/**/
#include "F0EVAL.H"

double UN004;
double UN006;
/**/


void
prefcat(char * n, long z, long t)
{
	char * P = NULL;
	switch(z)
	{
	case 1: P = ".SOU";
	break;
	case 2:
	if(t)
	P = ".DXB";
	else
	P = ".DXF";
	break;
	}
	if(P)
	strncat(n, P, 5);
}
/**/
long L_Hash[64];
/**/
long LL_Fetch(char*Name)
{
	long i;
	long I;
	i = ZZ_Hash(Name);

	I = L_Hash[i];
	while(I)
	{
	if(stricmp(Name, YY_Layer[I].I)==0)break;
	I = YY_Layer[I].n;
	}
	return(I);
}
long LL_Insert(char*Name)
{
	long i;
	long I;
	I = YY_Layer_Lim++;
	if(YY_Layer_Lim>=YY_Layer_Max)return(0);

	YY_Layer[I].I = Name;
	YY_Layer[I].O = Name;

	YY_Layer[I].o = I;

	i = ZZ_Hash(Name);

	YY_Layer[I].n = L_Hash[i];
	L_Hash[i] = I;

	return(I);
}
/**/
char*I_BTOP;
char*I_BEOB;
char*I_BBUF;
long I_BLEN;
/**/
FILE*Input_File;
char*Input_Cell;
char*Input_Name;

FILE*Output_File;
char*Output_Name;
/**/
void I_Read(void)
{
	I_BEOB = (I_BBUF = I_BTOP) +fread(I_BTOP, 1, I_BLEN, Input_File);
	if(feof(Input_File)) *I_BEOB++ = 0;
}
void NEXTBYTE(void)
{
	if(++I_BBUF == I_BEOB) I_Read();
}
short DXFyylex(void)
{
	static char C;

	while(I_BBUF[0]==040) NEXTBYTE();

	C = I_BBUF[0];
	NEXTBYTE();

	if(isprint(C))
	{
		if(islower(C)) C = toupper(C);
		return(C);
	}
	if(C=='\r')
	{
		YY_C_STR_N++;
		while(I_BBUF[0]=='\r')
		NEXTBYTE();
		NEXTBYTE();
	}
	return(EOS);
}
short YY_INT_LEX(void)
{
	char S[80]; long P; long Sign;
	P = 0;
	while(I_BBUF[0]==040) NEXTBYTE();

	if(I_BBUF[0]=='-')
	{
		Sign = -1;
		NEXTBYTE();
	}
	else
	{
		Sign = +1;
		if(I_BBUF[0]=='+') NEXTBYTE();
	}
	while(isdigit(I_BBUF[0]))
	{
		S[P++] = I_BBUF[0];
		NEXTBYTE();
	}
	S[P] = 0;
	YY_I_VAL = atol(S)*Sign;

	if(I_BBUF[0]=='\r')
	{
		while(I_BBUF[0]=='\r') NEXTBYTE();
		NEXTBYTE();
	}
	return(EOS);
}
#define isexpchar(c) ((c)=='e'||(c)=='E'||(c)=='+'||(c)=='-'||(c)=='.')
short YY_FLOAT_LEX(void)
{
	char S[80]; char*Z; long P;

	P = 0;
	while(I_BBUF[0]==040) NEXTBYTE();

	while(isdigit(I_BBUF[0])||isexpchar(I_BBUF[0]))
	{
		S[P++] = I_BBUF[0];
		NEXTBYTE();
	}
	S[P] = 0;
	YY_F_VAL = strtod(S, &Z)*UN002;

	if(Z!=(S+P))return(0);

	if(I_BBUF[0]=='\r')
	{
		while(I_BBUF[0]=='\r') NEXTBYTE();
		NEXTBYTE();
	}
	return(EOS);
}
void zabort(void)
{
	fprintf(stderr, "\nFATAL! EXITING DUE TO ERRORS");
	exit(1);
}

struct FF_Cell
{
	char*N; /*Name Of Cell*/
	char*F; /*Name Of File*/
	long X; /*Next Cell In Cell's Hash*/
	long x; /*Next Cell In Cell's Heap*/
	long R; /*A Entr's Heap Header*/
};
long FF_Cell_Len = sizeof(struct FF_Cell);
struct FF_Entr
{
	long I; /*Called Cell Index*/
	long M; /*Start Of Arrays For This Call*/
	long X; /*Next Call*/
};
long FF_Entr_Len = sizeof(struct FF_Entr);
struct FF_RMatrix /*Rotate Matrix*/
{
	double A; /*+COS*/
	double B; /*-SIN*/
	double D; /*+SIN*/
	double E; /*+COS*/

	long X; /*Next In RMHash*/
};
long FF_RMatrix_Len = sizeof(struct FF_RMatrix);
struct FF_AMatrix /*Array Of Insertion*/
{
	double DX;
	double DY;

	double SX; /*Already Rotated*/
	double SY; /*Already Rotated*/

	long KX;
	long KY;

	long RM; /*Orient Inside Array. (RMatrix Index)*/

	long NX; /*Next AMatrix*/
};
long FF_AMatrix_Len = sizeof(struct FF_AMatrix);
struct FF_RArray
{
	double SX;
	double sy;
	double SY;
	double sx;
	double DX;
	double DY;

	long KX;
	long KY;

	long NX; /*Array On Level-1*/
};
long FF_RArray_Len = sizeof(struct FF_RArray);
struct FF_Orient
{
	long RM; /*Rotation-Matrix*/
	long ST; /*Start Of Chains*/
	long NX; /*Next Orient*/
};
long FF_ORient_Len = sizeof(struct FF_Orient);
struct FF_RChain
{
	long RA; /*Start Of Arrays-Chain*/
	long NX; /*Next Chain*/
};
long FF_RChain_Len = sizeof(struct FF_RChain);
/**/
long YY_Xrelocate;
long YY_Yrelocate;
/**/
long*FF_Stack;
long FF_Stack_Lim;
long FF_Stack_Max;

long FF_RM0000;
long FF_RM0001;
long FF_RM0002;
long FF_RM0003;
long FF_RM0010;
long FF_RM0011;
long FF_RM0012;
long FF_RM0013;
struct FF_RMatrix RM0000 ={+1,0,0, +1};
struct FF_RMatrix RM0001 ={ 0, -1, +1,0};
struct FF_RMatrix RM0002 ={-1,0,0, -1};
struct FF_RMatrix RM0003 ={ 0, +1, -1,0};
struct FF_RMatrix RM0010 ={+1,0,0, -1};
struct FF_RMatrix RM0011 ={ 0, -1, -1,0};
struct FF_RMatrix RM0012 ={-1,0,0, +1};
struct FF_RMatrix RM0013 ={ 0, +1, +1,0};

long FF_CE_Hash[32];
long FF_RM_Hash[32];

long FF_Cell_Top;
long FF_Cell_Bot;
long FF_Cell_Lim;
long FF_Cell_Crn;

long FF_Orient_Top;
long FF_Instan_Cnt;
long FF_Orient_Cnt;
/**/
#define FF_CELL(i) (*(struct FF_Cell*)&FF_Heap_1[(i)])
#define FF_ENTR(i) (*(struct FF_Entr*)&FF_Heap_1[(i)])

#define FF_RMATRIX(i) (*(struct FF_RMatrix*)&FF_Heap_1[(i)])
#define FF_AMATRIX(i) (*(struct FF_AMatrix*)&FF_Heap_1[(i)])

#define FF_ORIENT(i) (*(struct FF_Orient*)&FF_Heap_2[(i)])
#define FF_RCHAIN(i) (*(struct FF_RChain*)&FF_Heap_2[(i)])
#define FF_RARRAY(i) (*(struct FF_RArray*)&FF_Heap_2[(i)])
/**/
char*FF_Heap_1;
long FF_Heap_1_Lim;
long FF_Heap_1_Max;

char*FF_Heap_2;
long FF_Heap_2_Lim;
long FF_Heap_2_Max;
/**/
long
FF_Halloc_1(long L)
{
	long I = FF_Heap_1_Lim;
	FF_Heap_1_Lim += L;

	if(FF_Heap_1_Lim>=FF_Heap_1_Max)zabort();
	return(I);
}
long
FF_Halloc_2(long L)
{
	long I = FF_Heap_2_Lim;
	FF_Heap_2_Lim += L;

	if(FF_Heap_2_Lim>=FF_Heap_2_Max)zabort();
	return(I);
}
long FI_CE_Hash
(char*s)
{
	long i = 0;
	while(*s) i = (i + (*s++))&31;
	return(i);
}
long FF_Find_CE
(char*N)
{
	long i;
	long I;

	i = FI_CE_Hash(N);
	I = FF_CE_Hash[i];
	while(I)
	{
	if(stricmp(FF_CELL(I).N, N)==0)return(I);
	I = FF_CELL(I).X;
	}
	I = FF_Halloc_1(FF_Cell_Len);
	FF_Cell_Lim++;

	if(FF_Cell_Top==0)
	{
	FF_Cell_Top = I;
	FF_Cell_Bot = I;
	}
	else
	{
	FF_CELL(FF_Cell_Bot).x = I;
	FF_Cell_Bot = I;
	}
	FF_CELL(I).N = SS_Fetch(N);
	FF_CELL(I).R = 0;
	FF_CELL(I).x = 0;
	FF_CELL(I).X = FF_CE_Hash[i];
	FF_CE_Hash[i] = I;

	return(I);
}
void
FF_Insert_Entr
(
double DX, double DY, double SX, double SY,
long KX, long KY, long RM, long C1, long C2
)
{
	long I;
	long J;

	for(I = FF_CELL(C1).R; I; I = FF_ENTR(I).X)
	{
	if(FF_ENTR(I).I==C2)break;
	}
	if(I==0)
	{
	I = FF_Halloc_1(FF_Entr_Len);

	FF_ENTR(I).M = 0L;
	FF_ENTR(I).I = C2;

	FF_ENTR(I).X = FF_CELL(C1).R;
	FF_CELL(C1).R = I;
	}
	J = FF_Halloc_1(FF_AMatrix_Len);

	FF_AMATRIX(J).NX = FF_ENTR(I).M;
	FF_ENTR(I).M = J;

	FF_AMATRIX(J).DX = DX;
	FF_AMATRIX(J).DY = DY;
	FF_AMATRIX(J).SX = SX;
	FF_AMATRIX(J).SY = SY;
	FF_AMATRIX(J).KX = KX;
	FF_AMATRIX(J).KY = KY;
	FF_AMATRIX(J).RM = RM;
}
long FI_RM_Hash
(struct FF_RMatrix*RM)
{
	long I;
	double S;
	S = RM->A+RM->B+RM->D+RM->E;
	S*= 100;
	I = ((long)S)&31;
	return(I);
}
long
FF_Find_RM(struct FF_RMatrix*M)
{
	long I;
	long i;
	i = FI_RM_Hash(M);
	I = FF_RM_Hash[i];
	while(I)
	{
	if(!(M->A!=FF_RMATRIX(I).A))
	if(!(M->B!=FF_RMATRIX(I).B))
	if(!(M->D!=FF_RMATRIX(I).D))
	if(!(M->E!=FF_RMATRIX(I).E))
	return(I);
	I = FF_RMATRIX(I).X;
	}
	I = FF_Halloc_1(FF_RMatrix_Len);

	FF_RMATRIX(I) = M[0];

	FF_RMATRIX(I).X = FF_RM_Hash[i];
	FF_RM_Hash[i] = I;

	return(I);
}
void
FL_Rotate
(long*x, long*y, struct FF_RMatrix*m)
{
	long t = *x;
	*x = floor(t * m->A + *y * m->B +0.5);
	*y = floor(t * m->D + *y * m->E +0.5);
}
void
FF_Rotate
(double*x, double*y, struct FF_RMatrix*m)
{
	double t = *x;
	*x = t * m->A + *y * m->B;
	*y = t * m->D + *y * m->E;
}
struct FF_RMatrix
FF_Zmult
(struct FF_RMatrix*m0, struct FF_RMatrix*m1)
{
	struct FF_RMatrix goal;
	goal.A = m1->A * m0->A + m1->B * m0->D;
	goal.B = m1->A * m0->B + m1->B * m0->E;
	goal.D = m1->D * m0->A + m1->E * m0->D;
	goal.E = m1->D * m0->B + m1->E * m0->E;
	return(goal);
}
void
FF_Matrix_Transform
(long Level, long Inst, long A, struct FF_RMatrix R)
{
	static long S;
	long M;
	static long T;
	static long I;
	static long J;
	static long K;
	static long Z;

	static struct FF_RMatrix X;
	S = FF_Stack[Level];

	for(M = FF_ENTR(S).M; M; M = FF_AMATRIX(M).NX)
	{
	Z = FF_Halloc_2(FF_RArray_Len);

	FF_RARRAY(Z).NX = A;
	FF_RARRAY(Z).sx = 0;
	FF_RARRAY(Z).sy = 0;

	FF_RARRAY(Z).DX = FF_AMATRIX(M).DX;
	FF_RARRAY(Z).DY = FF_AMATRIX(M).DY;
	FF_RARRAY(Z).SX = FF_AMATRIX(M).SX;
	FF_RARRAY(Z).SY = FF_AMATRIX(M).SY;
	FF_RARRAY(Z).KX = FF_AMATRIX(M).KX;
	FF_RARRAY(Z).KY = FF_AMATRIX(M).KY;

	T = Inst*FF_RARRAY(Z).KX*FF_RARRAY(Z).KY;

	if(A!=0)
	{
	FF_Rotate(&FF_RARRAY(Z).SX, &FF_RARRAY(Z).sy, &R);
	FF_Rotate(&FF_RARRAY(Z).sx, &FF_RARRAY(Z).SY, &R);
	FF_Rotate(&FF_RARRAY(Z).DX, &FF_RARRAY(Z).DY, &R);
	X = FF_Zmult(&FF_RMATRIX(FF_AMATRIX(M).RM), &R);
	}
	else
	X = FF_RMATRIX(FF_AMATRIX(M).RM);

	if(Level)
	{
	FF_Matrix_Transform(Level-1, T, Z, X);
	continue;
	}
	FF_Instan_Cnt+= T;

	J = FF_Find_RM(&X);
	I = FF_Orient_Top;

	while(I)
	{
	if(FF_ORIENT(I).RM==J)break;
	I = FF_ORIENT(I).NX;
	}
	if(I==0)
	{
	I = FF_Halloc_2(FF_ORient_Len);

	FF_ORIENT(I).NX = FF_Orient_Top;
	FF_Orient_Top = I;

	FF_ORIENT(I).RM = J;
	FF_ORIENT(I).ST = 0;

	++FF_Orient_Cnt;
	}
	K = FF_Halloc_2(FF_RChain_Len);

	FF_RCHAIN(K).RA = Z;
	FF_RCHAIN(K).NX = 0;
	FF_RCHAIN(K).NX = FF_ORIENT(I).ST;
	FF_ORIENT(I).ST = K;
	/*End Of Current Matrix*/
	}
}
void
FF_Explode_Cell(long L, struct FF_Cell*C)
{
	long R = C->R;

	if(R==0)return;
	while(R)
	{
	if(L>=FF_Stack_Max)
	{
	zabort();
	}
	FF_Stack[L] = R;

	if(FF_ENTR(R).I)
	{
	FF_Explode_Cell(L+1, &FF_CELL(FF_ENTR(R).I));
	}
	else
	{
	FF_Matrix_Transform(L, 1, 0, RM0000);
	}
	R = FF_ENTR(R).X;
	}
}
struct FF_Cell*ZZ_YY_CELL;
void
FF_SOU_CONVERT(struct FF_Cell*C);
void
yyparse_sou(void)
{
FF_SOU_CONVERT(ZZ_YY_CELL);
}
void
yyparse_mas(void)
{
}
void
yycscan_mas(void)
{
}
void
yycscan_dxf(void)
{
}
void
yyparse_dxf(void)
{
int yyparse(); 
yyparse();
}
void
FF_Convert_Cell(struct FF_Cell*C);
void
FF_EXPLODE_CELLS(void)
{
	long I;
	for(I = FF_Cell_Top; I; I = FF_CELL(I).x)
	{
	FF_Heap_2_Lim = 1;
	FF_Orient_Top = 0;
	FF_Orient_Cnt = 0;
	FF_Instan_Cnt = 0;

	fprintf(stderr, "\n\nCELL-EXPLODE ON %s STARTED", FF_CELL(I).N);
	FF_Explode_Cell(0, &FF_CELL(I));
	fprintf(stderr, "\nTOTAL ORIENTATIONS = %ld, INSTANCES = %ld",
	FF_Orient_Cnt, FF_Instan_Cnt);
	FF_Convert_Cell(&FF_CELL(I));
	}
}
long
Sou_Swab(short b[], long*i)
{
	union
	{
	long L; short I[2];
	} U;
	U.I[0] = b[*i+1], U.I[1] = b[*i];
	return(*i += 2, U.L);
}
long pp_next(short b[], long*t, long*m)
{
	*t = b[*m + 1] & 03;
	return(Sou_Swab(b, m)>>3);
}
long
FF_RM_Sou(long I)
{
	switch(I&013)
	{
	case 001: return(FF_RM0001);
	case 002: return(FF_RM0002);
	case 003: return(FF_RM0003);
	case 010: return(FF_RM0010);
	case 011: return(FF_RM0011);
	case 012: return(FF_RM0012);
	case 013: return(FF_RM0013);
	}
	return(FF_RM0000);
}
void
Terminate_Poly_2(void);
void
yy_init_phash(void)
{
long I;

PXMIN = PYMIN = LONG_MAX;
PXMAX = PYMAX =-LONG_MAX;

for(I = 0; I<ZLDLIM; ++I)
{
if(PXMIN>ZLDCO0[I]) PXMIN = ZLDCO0[I];
if(PXMAX<ZLDCO0[I]) PXMAX = ZLDCO0[I];
if(PYMIN>ZLDCO1[I]) PYMIN = ZLDCO1[I];
if(PYMAX<ZLDCO1[I]) PYMAX = ZLDCO1[I];
}
Determine_Point_Base();
}
void
Terminate_Poly_3(void)
{
if(!without_pcheck)yy_init_phash();

Terminate_Poly_2();
}
char
YY_VALID_LAYER(void)
{
	YI_Layer =
	LL_Fetch(YY_5String);

	if(YI_Layer==0&&YY_Layer_All)
	{
	YI_Layer =
	LL_Insert(SS_Fetch(YY_5String));
	}
	/*
	if(YI_Layer>3)YI_Layer = 0;
	*/
	return(YI_Layer!=0);
}
char
YY_VALID_SOU_LAYER(short L)
{
	sprintf(YY_5String, "%d", L);
	return
	YY_VALID_LAYER();
}
void
Sou_PickIter(short B[])
{
	long I;
	long k;
	long M;

	long DX;
	long DY;
	long SX;
	long SY;
	long KX;
	long KY;

	char*C = YY_2String;
	char*c = (char*)B + 2;

	memcpy(C, c, 6);
	C[6] = 0;

	k = 5;
	while(C[k]<33)C[k--] = 0;

	I = FF_Find_CE(C);

	M = FF_RM_Sou(B[4]&017);

	k = 5;
	DY = Sou_Swab(B, &k);
	DX = Sou_Swab(B, &k);

	KY = B[9], KX = B[11];

	SY = B[10];
	SX = B[12];

	if(KY<2)KY = 1, SY = 0;
	if(KX<2)KX = 1, SX = 0;

	FF_Insert_Entr(DX, DY, SX, SY, KX, KY, M, I, FF_Cell_Crn);

	return;
}
void
Sou_PickCell(short B[])
{
	long I;
	long k;
	long M;

	long X;
	long Y;

	char*C = YY_2String;
	char*c = (char*)B + 2;

	memcpy(C, c, 6);
	C[6] = 0;

	k = 5;
	while(C[k]<33)C[k--] = 0;

	I = FF_Find_CE(C);

	M = FF_RM_Sou(B[4]&017);

	k = 5;
	while(k<(B[0]&0377))
	{
	Y = Sou_Swab(B, &k);
	X = Sou_Swab(B, &k);

	FF_Insert_Entr(X, Y, 0, 0, 1, 1, M, I, FF_Cell_Crn);
	}
	return;
}
void
FF_Explode_Chain(void);
void
Sou_WriteBox(long ZZ[])
{
	long XMIN;
	long XMAX;
	long YMIN;
	long YMAX;

	XMIN = ZZ[0];
	XMAX = ZZ[2];
	YMIN = ZZ[1];
	YMAX = ZZ[3];
	if(XMIN>XMAX)Zswab(&XMIN, &XMAX);
	if(YMIN>YMAX)Zswab(&YMIN, &YMAX);

	ZLDCO0[0] = ZLDCO0[1] = XMIN;
	ZLDCO0[2] = ZLDCO0[3] = XMAX;
	ZLDCO1[0] = ZLDCO1[3] = YMIN;
	ZLDCO1[1] = ZLDCO1[2] = YMAX;

	ZLDCO0[4] = ZLDCO0[0];
	ZLDCO1[4] = ZLDCO1[0];

	ZLDLIM = 5;
	STATUS = 1;
	FF_Explode_Chain();
}
void
Sou_PickWire(short B[])
{
	long k;
	long c;
	long p;
	long x;
	long y;

	k = 2;
	p = 0;
	c = B[0] & 0377;

	if(!YY_VALID_SOU_LAYER(B[1]&0177))
	return;

	ZLDLIM = 0;

	while(k<c)
	{
	y = Sou_Swab(B, &k);
	x = Sou_Swab(B, &k);
	p = ZLDNEXT();
	if(p>64)return;
	ZLDCO0[p] = x;
	ZLDCO1[p] = y;
	}
	Terminate_Poly_3();
}
#define rx0 ZLDCO0[0]
#define ry0 ZLDCO1[0]
#define rx1 ZLDCO0[1]
#define ry1 ZLDCO1[1]
#define rx2 ZLDCO0[2]
#define ry2 ZLDCO1[2]
#define rx3 ZLDCO0[3]
#define ry3 ZLDCO1[3]
#define rx4 ZLDCO0[4]
#define ry4 ZLDCO1[4]

#define HY b[7]
#define WY b[8]
#define HX b[6]
#define WX b[9]
void
Sou_PickExpo(short b[])
{
	if(!YY_VALID_SOU_LAYER(b[1]&0177))
	return;

	if(b[10]) /*ANGLE-NON-ZERO*/
	{
	long k;
	long y;
	long x;

	k = 6;
	while(k<10)b[k++]>>=3;
	k = 2;
	y = Sou_Swab(b, &k)>>3;
	x = Sou_Swab(b, &k)>>3;
	ry0 = y;
	rx0 = x + HX;
	ry1 = y + HY;
	rx1 = x;
	ry2 = ry1 + WY;
	rx2 = rx1 + WX;
	ry3 = ry2 - HY;
	rx3 = rx2 + HX;
	ry4 = ry3 - WY;
	rx4 = rx3 - WX;

	STATUS = 1;
	ZLDLIM = 5;
	FF_Explode_Chain();
	}
	else /*ZERO-ANGLE*/
	{
	long k;
	long p;
	long x;
	long y;

	long zz[5];

	k = 2;
	p = 0;
	while(k<=8)
	{
	y = Sou_Swab(b, &k) >>3;
	x = Sou_Swab(b, &k) >>3;
	zz[p++] = x;
	zz[p++] = y;
	}
	Sou_WriteBox(zz);
	}
}
#undef rx0
#undef ry0
#undef rx1
#undef ry1
#undef rx2
#undef ry2
#undef rx3
#undef ry3
#undef rx4
#undef ry4

#undef HY
#undef WY
#undef HX
#undef WX
void
Sou_PickBox(short B[])
{
	long k;
	long p;
	long x;
	long y;

	long zz[5];

	k = 2;
	p = 0;

	if(!YY_VALID_SOU_LAYER(B[1]&0177))
	return;

	while(k<=8)
	{
	y = Sou_Swab(B, &k);
	x = Sou_Swab(B, &k);
	zz[p++] = x;
	zz[p++] = y;
	}
	Sou_WriteBox(zz);
}
void
Sou_PickRect(short B[])
{
	long k;
	long i;
	long p;
	long y;
	long x;

	k = 2;
	i = p = 0;

	if(!YY_VALID_SOU_LAYER(B[1]&0177))
	return;

	ZLDLIM = 0;

	while(i++<4)
	{
	y = Sou_Swab(B, &k);
	x = Sou_Swab(B, &k);

	p = ZLDNEXT();
	ZLDCO0[p] = x;
	ZLDCO1[p] = y;
	}
	Terminate_Poly_3();
}
void
Sou_PickPoly(short B[])
{
	long k;
	long t;
	long c;
	long p;
	long y;
	long x;
	long z;
	long err;

	y = x = z = t = p = 0;
	k = 7;
	c = B[0]&0377;

	if(!YY_VALID_SOU_LAYER(B[1]&0177))
	return;

	ZLDLIM = 0;

	while(k<c)
	{
	err = 0;
	z = pp_next(B, &t, &k);
	switch(t)
	{
	case 0: x = pp_next(B, &t, &k); y = z;
	break;
	case 1: y = z;
	break;
	case 2: x = z;
	break;
	default: err = 1;
	}
	if(err)return;
	p = ZLDNEXT();
	if(p>64)return;
	ZLDCO0[p] = x;
	ZLDCO1[p] = y;
	}
	Terminate_Poly_3();
}
void
FF_SOU_CONVERT(struct FF_Cell*C)
{
	short S_B[01000];
	short*S_P;

	FILE*File;
	if(strlen(C->F)==0)return;

	File = Fopen(C->F, "rb");

	while(fread(S_B, 1, 01000, File)==01000)
	{
	S_P = S_B;
	while((S_P-S_B)<0377)
	{
	switch
	(S_P[0]&017400)
	{
	case 002400:
	case 003400:
	Sou_PickWire(S_P);
	break;
	case 013400:
	Sou_PickPoly(S_P);
	break;
	case 004400:
	Sou_PickRect(S_P);
	break;
	case 001400:
	Sou_PickBox(S_P);
	break;
	case 007400:
	Sou_PickExpo(S_P);
	break;
	case 002000:
	default:
	S_P[0] = 0377;
	break;
	}
	S_P += *(S_P++)&0377;
	}
	}
	Ferror(File, C->F);
	fclose(File);
}
void
FF_SOU_CELL_SCAN(FILE*File)
{
	short S_B[01000];
	short*S_P;

	if(File==NULL)zabort();

	while(fread(S_B, 1, 01000, File)==01000)
	{
	S_P = S_B;
	while((S_P-S_B)<0377)
	{
	switch
	(S_P[0]&017400)
	{
	case 000000:
	case 002400:
	case 003400:
	case 013400:
	case 004400:
	case 001400:
	case 007400:
	break;
	case 001000:
	Sou_PickCell(S_P);
	break;
	case 005000:
	Sou_PickIter(S_P);
	break;
	default:
fprintf(stderr, "\nBad sector.");zabort();
	break;
	case 002000:
	S_P[0] = 0377;
	}
	S_P += *(S_P++)&0377;
	}
	}
}
void
yycscan_sou(void)
{
	char*Name = YY_1String;
	FILE*File;
	long Cnum = FF_Cell_Top;
	char*Cell;

	File = Input_File;
	strncpy(Name, Input_Name, 100);

	FF_Cell_Crn = 0;
	do
	{
	fprintf(stderr, "\nCELL-SCAN STARTED ON FILE %s", Name);
	FF_SOU_CELL_SCAN(File);
	Ferror(File, Name);
	fclose(File);

	File = NULL;
	Cnum = FF_CELL(Cnum).x;
	while(Cnum)
	{
	Cell = FF_CELL(Cnum).N;
	FF_Cell_Crn = Cnum;

	strncpy(Name, Cell, 9);
	prefcat(Name, 1, 0);

	File = fopen(Name, "rb");
	if(File!=NULL)
	{
	FF_CELL(Cnum).F = SS_Fetch(Name);
	break;
	}
	FF_CELL(Cnum).F = NULL;
	fprintf(stderr, "\nFILE %s OPEN ERROR", Name);
	Cnum = FF_CELL(Cnum).x;
	}
	/*End While Cnum*/
	}
	while(File!=NULL);
}
#define DX FF_RARRAY(K).DX
#define DY FF_RARRAY(K).DY
#define SX FF_RARRAY(K).SX
#define SY FF_RARRAY(K).SY
#define sx FF_RARRAY(K).sx
#define sy FF_RARRAY(K).sy
#define KX FF_RARRAY(K).KX
#define KY FF_RARRAY(K).KY
void
FF_Explode_Item
(double X, double Y, long C)
{
	long I;
	long J;
	long K;

	K = FF_Stack[C];

	X += DX;
	Y += DY;
	if(C==0)
	{
		double y;
		double x;
		ZZ_DELTA_X = X;
		ZZ_DELTA_Y = Y;
		for(J = 0; J<KX; ++J)
		{
			x = ZZ_DELTA_X;
			y = ZZ_DELTA_Y;
			for(I = 0; I<KY; ++I)
			{
				if(Is_Error_Output==2)
				{
					Dxb_Ld_Image_Maker(1, YY_5String);
					error_level = 2;
					Is_Error_Output = 1;
					return;
				}
				Zz_Write();
				ZZ_DELTA_Y+=SY;
				ZZ_DELTA_X+=sx;
			}
			ZZ_DELTA_Y = y;
			ZZ_DELTA_X = x;
			ZZ_DELTA_X+=SX;
			ZZ_DELTA_Y+=sy;
		}
	}
	else
	{
		double y;
		double x;
		for(J = 0; J<KX; ++J)
		{
			x = X;
			y = Y;
			for(I = 0; I<KY; ++I)
			{
				FF_Explode_Item(X, Y, C-1);
				Y += SY;
				X += sx;
			}
			X = x;
			Y = y;
			X += SX;
			Y += sy;
		}
	}
}
#undef KX
#undef KY
#undef SX
#undef SY
#undef DX
#undef DY
void
FF_Explode_Chain(void)
	{
	long*Zldco0 = ZLDCO0;
	long*Zldco1 = ZLDCO1;

	long I;
	long J;
	long K;
	long S;
	long status=STATUS;

	I = FF_Orient_Top;
	if(I==0)return;

	while(I)
	{
	if(FF_Orient_Cnt>1)
		{
		memcpy(ZLDCO2, ZLDCO0, ZLDLIM*4);
		memcpy(ZLDCO3, ZLDCO1, ZLDLIM*4);

		ZLDCO0 = ZLDCO2;
		ZLDCO1 = ZLDCO3;
		}
	if(FF_ORIENT(I).RM!=FF_RM0000)
		{
		for(J = 0; J<ZLDLIM; ++J)
			FL_Rotate
		(&ZLDCO0[J], &ZLDCO1[J], &FF_RMATRIX(FF_ORIENT(I).RM));

		if(Is_Error_Output==0)
			DETERMINE_STAT_1();
		else	STATUS = 1;

		if(!STATUS)
			{
fprintf(stderr, "\nERROR!!!POLYGON DISCARDED>IMPOSSIBLE TO TRANSFORM");
error_level = 2;
			ZLDCO0 = Zldco0;
			ZLDCO1 = Zldco1;

			return;
			}
		}
	else	STATUS=status;
	for(J = FF_ORIENT(I).ST; J; J = FF_RCHAIN(J).NX)
		{
		K = FF_RCHAIN(J).RA;
		if(!K)continue;

		S = -1;

		while(K)
			{
			FF_Stack[++S] = K;
			K = FF_RARRAY(K).NX;
			}
		if(Is_Error_Output!=0)
			Is_Error_Output = 2;
		FF_Explode_Item(YY_Xrelocate, YY_Yrelocate, S);
		if(Is_Error_Output!=0)
			{
			Is_Error_Output = 2;
			return;
			}
		}
	ZLDCO0 = Zldco0;
	ZLDCO1 = Zldco1;

	I = FF_ORIENT(I).NX;
	}
	/*End For Orients*/
	}
void write_header(FILE*DXB)
{
	char header[40];
	strncpy(header, "AutoCAD DXB 1.0\r\n\x1a", 40);
	fwrite(header, strlen(header)+1, 1, DXB);
	return;
}
void set_double(FILE*DXB)
{
	unsigned char id;
	short mode;

	id = 135; /*ITEM-IS-NUMBER-MODE*/
	mode = 1; /*DOUBLE-MODE-FOLLOWS*/
	fwrite((char*)&id, 1, 1, DXB);
	fwrite((char*)&mode, 2, 1, DXB);
}
void write_pline
(FILE*DXB, short closure)
{
struct
{
	unsigned char id;
	short closure;
} pline;
	pline.id = 19;
	pline.closure = closure;
	fwrite((char*)&pline, 3, 1, DXB);
}
void write_layer
(FILE*DXB, char*layer)
{
	unsigned char id;

	id = 129;
	fwrite((char*)&id, 1, 1, DXB);
	fwrite(layer, strlen(layer)+1, 1, DXB);
}
void write_vertex
(FILE*DXB, long x, long y)
{
struct
{
	unsigned char id;
	double dx, dy;
} vertex;
	vertex.id = 20;
	vertex.dx = (double)x + ZZ_DELTA_X;
	vertex.dy = (double)y + ZZ_DELTA_Y;
	vertex.dx /= UN006;
	vertex.dy /= UN006;
	fwrite((char*)&vertex, 17, 1, DXB);
}
void write_seqend
(FILE*DXB)
{
unsigned char id;
	id = 17;
	fwrite((char*)&id, 1, 1, DXB);
}
void
Dxb_Ld_Image_Maker(char closure, char*s)
{
	long L;
	unsigned char id;
	if(ZLDLIM==0)
	{
	id = 0;
	if(ERRFIL)
	{
	fwrite((char*)&id, 1, 1, ERRFIL);
	fclose(ERRFIL);
	}
	else
	{
	if(!ERRNAM)
	ERRNAM = "m-cerr.dxb";
	Delete_File(ERRNAM);
	}
	return;
	}
	if(ZLDLIM<=1) return;
	if(!ERRFIL)
	{
	ERRNAM = "m-cerr.dxb";
	if((ERRFIL=fopen(ERRNAM, "wb"))==NULL)zabort();
	write_header(ERRFIL);
	set_double(ERRFIL);
	}
	if(s) write_layer(ERRFIL, s);
	write_pline(ERRFIL, (short)closure);
	for(L = 0; L<ZLDLIM; ++L)
	{
	write_vertex(ERRFIL, ZLDCO0[L], ZLDCO1[L]);
	}
	write_seqend(ERRFIL);
}
void
FF_Explode_Chain(void);
void
ZZ_EXPLODE_CHAIN(void)
{
	long I;

	for(I = 1; I<ZLBLIM; ++I)
	{
	if(ZLBCO3[I]==0)continue;

	Unload_Lv_Polygon(I);
	if(STATUS)
	FF_Explode_Chain();
	}
}
void ZVXALLOC(long W)
{
#ifdef __BORLANDC__
if(W>8000)W = 8000;
#else
if(W>64000)W = 64000;
#endif
	VERMAX = W;
	ZXXALLOC((void***)VERCOR, W*8);
	VERLIM = 1;
}
long VERNEXT(void)
{
if((VERLIM+1)>=VERMAX)ZOUTABORT();
return(VERLIM++);
}
double DEFINE_CSLOPE(long K, long I, long J)
{
double X, Y, A;
	X = VERCOX[J]-VERCOX[I];
	Y = VERCOY[J]-VERCOY[I];
	A = Eval_Angle(X, Y);
	X = VERCOX[K]-VERCOX[I];
	Y = VERCOY[K]-VERCOY[I];
	A = A + 360-Eval_Angle(X, Y);
	if(A>360) A-=360;
	return(A);
}
long SPLINE_TYPE(double A)
{
long T;
	if(ENVEQ(A, 360, 5.0)) T =-1;
	else if(ENVEQ(A, 180, 0.1)) T = 0;
	else if(A<=270) T =+1;
	else if(ENVEQ(A, 360, 15.0)) T = 2;
	else T = 3;
	return(T);
}
void DEFINE_DISPLACE(long I, long K, double*X, double*Y)
{
	X[0] = X[2]*VERSTW[I]+VERCOX[I];
	Y[0] = Y[2]*VERSTW[I]+VERCOY[I];
	X[1] = X[2]*VERENW[K]+VERCOX[K];
	Y[1] = Y[2]*VERENW[K]+VERCOY[K];
}
long DEFINE_WISE_SIGN
(double IX, double IY, double X, double Y)
{
double M;
	M = (double)IX*Y-(double)X*IY;
	return(M>1E-7? 1: M<-1E-7? -1: 0);
}
void DEFINE_NORMALFROM(long I, long K)
{
double SZ, DX, DY, NX, NY;
	DX = VERCOX[K]-VERCOX[I];
	DY = VERCOY[K]-VERCOY[I];
	NX =+DY;
	NY =-DX;
	SZ = sqrt(DX*DX+DY*DY);
	NX = NX/SZ;
	NY = NY/SZ;
	NXFROM = NX;
	NYFROM = NY;
}
long DEFINE_INTERSECTION
(double*X1, double*Y1, double*X2, double*Y2,
 double*X, double*Y)
{
double DL, A1, B1, C1, A2, B2, C2;
	A1 = Y1[0]-Y1[1];
	B1 = X1[1]-X1[0];
	C1 = X1[0]*Y1[1]-X1[1]*Y1[0];

	A2 = Y2[0]-Y2[1];
	B2 = X2[1]-X2[0];
	C2 = X2[0]*Y2[1]-X2[1]*Y2[0];

	DL = A1*B2 - B1*A2;
	if(ENVEQ(DL, 0, 1E-7))return(0);

	X[0] = (B1*C2-B2*C1)/DL;
	Y[0] = (A2*C1-A1*C2)/DL;
	if(X[0]>LONG_MAX||Y[0]>LONG_MAX)return(0);
	return(1);
}
char NO_CONT_1;
char NO_CONT_2;
long SW_NORMAL;
void Unl_ToIntlayStandart(void)
{
	long I;
	long J;

	J = ZLDLIM;
	if(J==0)return;

	while(J--)
	{
	I = ZLDNEXT();
	ZLDCO0[I] = ZLDCO2[J];
	ZLDCO1[I] = ZLDCO3[J];
	}
	STATUS = 1;

	Z_Check_Closure();
	Z_FEval_Storage();

	ZLDLIM = 0;
}
void UNL_TOINTLAYSTANDART(double*X, double*Y)
{
	long J;
	long I;
	long s;

	long*Zldco0 = ZLDCO0;
	long*Zldco1 = ZLDCO1;
	long Zldlim = ZLDLIM;

	long x[5];
	long y[5];

	long *X0 = ZLDCO0;
	long *X1 = ZLDCO2;
	long *Y0 = ZLDCO1;
	long *Y1 = ZLDCO3;

	s = 1L<<S_FACTOR;

	ZLDCO0 = x;
	ZLDCO1 = y;
	ZLDLIM = 0;
	for(J = 4; J<9; ++J)
	{
	I = ZLDNEXT();
	x[I] = floor(X[J]*s+0.5);
	y[I] = floor(Y[J]*s+0.5);
	}
	J = DETERMINE_STAT_1();

	if(J==0)
	{
	DEFINE_INTERSECTION(X+5, Y+5, X+7, Y+7, X, Y);
	/*
	x[2] = floor(X[0]-X[2]*SW_NORMAL*0.5+0.5);
	y[2] = floor(Y[0]-Y[2]*SW_NORMAL*0.5+0.5);
	*/
	x[2] = floor(X[0]*s+0.5);
	y[2] = floor(Y[0]*s+0.5);
	x[3] = x[2];
	y[3] = y[2];

	J = DETERMINE_STAT_1();
	}
	ZLDLIM = Zldlim;
	ZLDCO0 = Zldco0;
	ZLDCO1 = Zldco1;

	if(J!=0)
	{
	if(J!=1)
	{
	long *X2 = X0;
	long *Y2 = Y0;
	X0 = X1, X1 = X2;
	Y0 = Y1, Y1 = Y2;
	}
	if(ZLDLIM!=0)
	{
	I = ZLDLIM-1;
	J = X0[I]!=x[0]||X1[I]!=x[3]||Y0[I]!=y[0]||Y1[I]!=y[3];

	if(J||NO_CONT_2)
	Unl_ToIntlayStandart();
	}
	if(ZLDLIM==0)
	{
	I = ZLDNEXT();

	X0[I] = x[0];
	X1[I] = x[3];
	Y0[I] = y[0];
	Y1[I] = y[3];
	}
	I = ZLDNEXT();

	X0[I] = x[1];
	X1[I] = x[2];
	Y0[I] = y[1];
	Y1[I] = y[2];

	STATUS = 0;
	}
}
void
CORRECT_ELBOW(double W)
{
	double Z;

	double X = XELBOW;
	double Y = YELBOW;

	Z = sqrt(X*X+Y*Y);

	X /= Z;
	Y /= Z;

	XELBOW = X*W;
	YELBOW = Y*W;
}
void DEFINE_ELBOW
(double*X1, double*Y1, long I)
{
	if(!Closed_Polygon)
	if(Last_Segment)
	if(I_LS_DF)
	{
	CORRECT_ELBOW(VERENW[Last_Segment]);
	}
	X1[5] = XELBOW + VERCOX[I];
	Y1[5] = YELBOW + VERCOY[I];
	X1[6] = VERCOX[I] - XELBOW;
	Y1[6] = VERCOY[I] - YELBOW;
}
long POINTONSEGMENT
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
Dswab(double*A, double*B)
{
double T = *A; *A = *B, *B = T;
}
void
DEFINE_SEGMENT(long J, long I, long K)
{
double IX, IY, X1[9], X2[3], Y1[9], Y2[3], CAN, MAN;
/*[0]-FIRST-DISPLACE-POINT! [1]-SECOND-DISP-POINT! [2]-NORMAL*/

long SW0, SW1, SW2, SW3, SPT;/*SPECIAL-FLAGS*/

static long SW4;
	NO_CONT_1 = 1;
	SW_NORMAL = 1;

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
	VV_Cover = 0;

	if(ENVEQ(VERSTW[I], 0, 1E-5)) --SPT;
	if(ENVEQ(VERENW[I], 0, 1E-5)) --SPT;

	if(!SPT) SPT = SPLINE_TYPE(MAN);

	DEFINE_DISPLACE(J, I, X1, Y1);
	DEFINE_DISPLACE(I, K, X2, Y2);

	if(SPT>0)
	{
	SW3 = DEFINE_INTERSECTION(X1, Y1, X2, Y2, &IX, &IY);
	if(!SW3) SPT = 0;
	else
	IX -= VERCOX[I],
	IY -= VERCOY[I];
	}
	X1[4] = XELBOW +VERCOX[J];
	Y1[4] = YELBOW +VERCOY[J];
	X1[7] = VERCOX[J] -XELBOW;
	Y1[7] = VERCOY[J] -YELBOW;
	X1[5] = X1[1];
	Y1[5] = Y1[1];
	X1[6] = 2*VERCOX[I]-X1[1];
	Y1[6] = 2*VERCOY[I]-Y1[1];
	switch(SPT)
	{
	case-2:
	XELBOW = YELBOW = 0;
	break;
	case-1: /*VERY-ACUTE-ANGLE*/
	XELBOW = NXFROM*VERSTW[I], YELBOW = NYFROM*VERSTW[I];
	break;
	case 0: /*GORIZONTAL!NEEDED-BOX-GENERATING*/
	SW1 = SW2 = 1;
	XELBOW = X1[2]*VERSTW[I], YELBOW = Y1[2]*VERSTW[I];

	if(SW0) XELBOW =-XELBOW, YELBOW =-YELBOW;

	if(SW4!=SW0)
	{
	Dswab(X1+4, X1+7);
	Dswab(Y1+4, Y1+7);
	Dswab(X1+5, X1+6);
	Dswab(Y1+5, Y1+6);

	SW_NORMAL = -1;
	}
	break;
	case 1: /*NOT-ACUTE-ANGLE*/
	if(VERSTW[I]!=VERENW[I])
	{
	X1[3] = X1[0]-VERCOX[I];
	Y1[3] = Y1[0]-VERCOY[I];
	X2[3] = X2[1]-VERCOX[I];
	Y2[3] = Y2[1]-VERCOY[I];
	SW1 = DEFINE_WISE_SIGN(IX, IY, X1[3], Y1[3]);
	SW2 = DEFINE_WISE_SIGN(IX, IY, X2[3], Y2[3]);
	}
	else SW1 =-1, SW2 =+1;

	NO_CONT_1 = SW1==SW2;

	XELBOW = SW1==SW2? X1[2]*VERSTW[I] : IX;
	YELBOW = SW1==SW2? Y1[2]*VERSTW[I] : IY;
	if(SW1!=SW2) DEFINE_ELBOW(X1, Y1, I);

	if(SW0) XELBOW =-XELBOW, YELBOW =-YELBOW;

	break;
	case 2: /*ACUTE-ANGLE*/
	case 3:
	/*MIRROR-NORMALS*/
	X1[2] =-X1[2];
	Y1[2] =-Y1[2];
	X2[2] =-X2[2];
	Y2[2] =-Y2[2];

	SW_NORMAL = -1;
	/*DEFINE-INSIDE-EDGES*/
	DEFINE_DISPLACE(J, I, X1, Y1);
	DEFINE_DISPLACE(I, K, X2, Y2);
	/*CHECK-FOR-CASE-TYPE*/
	SW3 = DEFINE_INTERSECTION(X1, Y1, X2, Y2, X1+3, Y1+3);
	if(!SW3)zabort();
	SW1 = POINTONSEGMENT(X1, Y1, X1[3], Y1[3]);
	SW2 = POINTONSEGMENT(X2, Y2, X1[3], Y1[3]);
	if(SW1&&SW2&&SPT!=2)
	{
	XELBOW = IX;
	YELBOW = IY;
	DEFINE_ELBOW(X1, Y1, I);

	if(SW0) XELBOW =-XELBOW, YELBOW =-YELBOW;

	break;
	}
	X2[2] =-X2[2];
	Y2[2] =-Y2[2];
	XELBOW = X2[2]*VERSTW[I];
	YELBOW = Y2[2]*VERSTW[I];
	XCOVER[5] = X1[5]+XELBOW;
	YCOVER[5] = Y1[5]+YELBOW;
	XCOVER[6] = VERCOX[I]+XELBOW;
	YCOVER[6] = VERCOY[I]+YELBOW;
	XCOVER[7] = VERCOX[I];
	YCOVER[7] = VERCOY[I];
	XCOVER[8] = XCOVER[4] = X1[5];
	YCOVER[8] = YCOVER[4] = Y1[5];

	if(SW0) XELBOW =-XELBOW, YELBOW =-YELBOW;

	VV_Cover = 1;
	break;
	default: zabort();
	}
	X1[8] = X1[4];
	Y1[8] = Y1[4];
	UNL_TOINTLAYSTANDART(X1, Y1);

	NO_CONT_2 = NO_CONT_1;
	SW4 = SW0;
}
void
D_E_2_EVAL(double X, double Y, double*x, double*y)
{
	double h = sqrt(X*X+Y*Y);

	x[0] = (X/h)*Epsilon_2;
	y[0] = (Y/h)*Epsilon_2;
}
void
YY_Interpolate_Arc(void)
{
	double X;
	double Y;
	double x;
	double y;

	double Angle;

	double Buldge;
	double Ver1_X;
	double Ver1_Y;
	double Ver2_X;
	double Ver2_Y;

	double Radius;
	double Delta_W;

	double Arc_Len;
	double Hord_Len;

	double High_T;
	double High_T_X;
	double High_T_Y;

	double Norm_X;
	double Norm_Y;
	double Norm_T_X;
	double Norm_T_Y;

	double Center_X;
	double Center_Y;

	double Beta;
	double Zeta;

	double End_W_1;
	double End_W_2;

	double Start_W_1;
	double Start_W_2;

	long Kapa;

	struct FF_RMatrix Rotate;

	long J;
	long I;

	I = VERLIM - 1;
	if(I<2)zabort();

	Ver1_X = VERCOX[I-1];
	Ver1_Y = VERCOY[I-1];

	Ver2_X = VERCOX[I];
	Ver2_Y = VERCOY[I];

	Start_W_1 = VERSTW[I-1];
	End_W_1 = VERENW[I-1];

	Start_W_2 = VERSTW[I];
	End_W_2 = VERENW[I];

	Buldge = YY_Buldge;

	if(ENVEQ(Buldge, 0, 1E-4))zabort();
	if(Buldge>0)
	{
	/*Counter Clockwise.*/
	/*Center on left Side.*/
	/*Rotate to left.*/ STATUS = 1;
	}
	else
	{
	STATUS =-1;
	Buldge =-Buldge;
	}
	Angle = atan(Buldge)*4;

	X = Ver2_X - Ver1_X;
	Y = Ver2_Y - Ver1_Y;
	Hord_Len = sqrt(X*X+Y*Y);

	if(Hord_Len<Epsilon&&Angle<M_PI)return;

	Radius = (Hord_Len/2)/(sin(Angle/2));
	if(Radius<=0)zabort();

	if(Radius<(2*Epsilon))return;

	High_T = Radius*(cos(Angle/2));
	Arc_Len = Radius*Angle;

	Norm_X = X/Hord_Len;
	Norm_Y = Y/Hord_Len;

	if(STATUS>0)
	{
	Norm_T_X =-Norm_Y;
	Norm_T_Y = Norm_X;
	}
	else
	{
	Norm_T_X = Norm_Y;
	Norm_T_Y =-Norm_X;
	}
	High_T_X = Norm_T_X*High_T;
	High_T_Y = Norm_T_Y*High_T;

	Center_X = (X/2) + High_T_X + Ver1_X;
	Center_Y = (Y/2) + High_T_Y + Ver1_Y;

	Beta = acos((Radius-Epsilon)/Radius)*2;
	Zeta = Radius*Beta;

	Kapa = ceil(Arc_Len/Zeta);
	Zeta = Arc_Len/Kapa;
	Beta = Zeta/Radius;

	if(Kapa<2)return;

	Delta_W = (Start_W_1 - End_W_1)/Kapa;
	I = VERLIM - 2;
	VERENW[I] = (Start_W_1 -= Delta_W);

	Beta = Beta*STATUS;

	X = Ver1_X - Center_X;
	Y = Ver1_Y - Center_Y;

	Rotate.A = cos(Beta);
	Rotate.D = sin(Beta);

	Rotate.E = Rotate.A;
	Rotate.B =-Rotate.D;

	if(I_DEF_P_V)
	{
	struct FF_RMatrix ROtate;
	ROtate = Rotate;
	ROtate.D *= -1;
	ROtate.B *= -1;

	I_VAL_PVX = X;
	I_VAL_PVY = Y;
	FF_Rotate(&I_VAL_PVX, &I_VAL_PVY, &ROtate);

	D_E_2_EVAL(I_VAL_PVX, I_VAL_PVY, &x, &y);

	I_VAL_PVX += Center_X + x;
	I_VAL_PVY += Center_Y + y;
	}
	VERLIM -= 1;
	for(I = 1; I<Kapa; ++I)
	{
	FF_Rotate(&X, &Y, &Rotate);
	J = VERNEXT();

	D_E_2_EVAL(X, Y, &x, &y);

	VERCOX[J] = X + Center_X + x;
	VERCOY[J] = Y + Center_Y + y;

	VERSTW[J] = Start_W_1;
	Start_W_1 -= Delta_W;
	VERENW[J] = Start_W_1;
	}
	if(I_DEF_N_V)
	{
	I_VAL_NVX = Ver2_X - Center_X;
	I_VAL_NVY = Ver2_Y - Center_Y;
	FF_Rotate(&I_VAL_NVX, &I_VAL_NVY, &Rotate);

	D_E_2_EVAL(I_VAL_NVX, I_VAL_NVY, &x, &y);

	I_VAL_NVX += Center_X + x;
	I_VAL_NVY += Center_Y + y;
	}
	J = VERNEXT();

	VERCOX[J] = Ver2_X;
	VERCOY[J] = Ver2_Y;

	VERSTW[J] = Start_W_2;
	VERENW[J] = End_W_2;
}
void
Terminate_Circle(void)
{
	long I;
	VERLIM = 3;
	YY_Buldge = 1.0;

	I_DEF_N_V = 0;
	I_DEF_P_V = 0;
	if(Item_40_Val<(3*Epsilon))
	{
	long ZZ[5];
	ZZ[0] = floor(Item_10_Val - Item_40_Val +0.5);
	ZZ[1] = floor(Item_20_Val - Item_40_Val +0.5);
	ZZ[2] = floor(Item_10_Val + Item_40_Val +0.5);
	ZZ[3] = floor(Item_20_Val + Item_40_Val +0.5);

	Sou_WriteBox(ZZ);
	return;
	}
	VERCOX[1] = Item_10_Val - Item_40_Val;
	VERCOY[1] = Item_20_Val;
	VERCOX[2] = Item_10_Val + Item_40_Val;
	VERCOY[2] = Item_20_Val;

	VERSTW[1] = VERENW[1] = 0;
	VERSTW[2] = VERENW[2] = 0;

	YY_Interpolate_Arc();

	I = VERNEXT();
	VERSTW[I] = VERENW[I] = 0;
	VERCOX[I] = Item_10_Val + Item_40_Val;
	VERCOY[I] = Item_20_Val;

	I = VERNEXT();
	VERSTW[I] = VERENW[I] = 0;
	VERCOX[I] = Item_10_Val - Item_40_Val;
	VERCOY[I] = Item_20_Val;

	YY_Interpolate_Arc();

	Terminate_Poly_2();
}
#define X1 Item_10_Val
#define X2 Item_11_Val
#define X3 Item_12_Val
#define X4 Item_13_Val
#define Y1 Item_20_Val
#define Y2 Item_21_Val
#define Y3 Item_22_Val
#define Y4 Item_23_Val
void
Terminate_Trace(void)
{
double DX;
double DY;

long Wd;

static FILE*P;
static char*Ly_Old;
static long Wd_Old=-1;

if(!YY_VALID_LAYER())
return;

DX = (X2-X1);
DY = (Y2-Y1);

Wd = floor(sqrt(DX*DX+DY*DY)/10+0.5);

X1 += DX/2;
Y1 += DY/2;

DX = (X4-X3)/2;
DY = (Y4-Y3)/2;

X3 += DX;
Y3 += DY;

if(P==NULL)P=Fopen("MTRACE.PDF", "wb");

#define Ly YY_Layer[YI_Layer].O
if(Ly_Old!=Ly)
{
Ly_Old = Ly;
fprintf (P, "\r\n[Ly \x22%s\x22]", Ly_Old);
}
#undef Ly

if(Wd!=Wd_Old)
{
Wd_Old = Wd;
fprintf(P, "\r\n[Wd %ld]", Wd);
}
fprintf (P, "\r\n{L %ld %ld %ld %ld}",
(long)floor(X1/10+0.5), (long)floor(Y1/10+0.5),
(long)floor(X3/10+0.5), (long)floor(Y3/10+0.5));
}
#undef X4
#undef X3
#undef X2
#undef X1
#undef Y4
#undef Y3
#undef Y2
#undef Y1
#define M_W MAX_WIDTH
#define S_W ZZ_START_WIDTH
#define E_W ZZ_ENDIN_WIDTH
void
Terminate_Vertex(void)
{
	long I;
	I = VERNEXT();
	if(I==1)
	{
		I_LS_DF = 0;
		I_FR_DF = 0;
	}
	VERCOX[I] = Item_10_Val;
	VERCOY[I] = Item_20_Val;

	if(YY_40_FLAG)
	{
		VERSTW[I] = Item_40_Val;
		if(VERSTW[I]>M_W)
			M_W = VERSTW[I];
	}
	else
		VERSTW[I] = S_W;

	if(YY_41_FLAG)
	{
		VERENW[I] = Item_41_Val;
		if(VERENW[I]>M_W)
			M_W = VERENW[I];
	}
	else
		VERENW[I] = E_W;

	if(YY_Interpolate)
	{
	I_DEF_N_V = 1;
		I_DEF_P_V = I == 2;

		YY_Interpolate_Arc();
		YY_Interpolate = 0;

		I_LS_DF = 1;
		if(I_DEF_P_V)
			I_FR_DF = 1;
	}
	else
		I_LS_DF = 0;

	if(YY_42_FLAG)
	{
		YY_Buldge = Item_42_Val/UN002;
		YY_Interpolate = !(ENVEQ(YY_Buldge, 0, 1E-4));
	}
}
#undef M_W
#undef S_W
#undef E_W
void Terminate_Insert(void)
{
}
/**/
#define Z_Stat(VT,Vt) (ZVTCO8[(Vt)]>0||ZVTCO8[(VT)]>0||(ZVTCO8[(Vt)]&&ZVTCO8[VT]))
/**/
void ZZ_REPAIR_PLINE(long Lb)
{
	long Lb_Lim;
	long Lv_Lim;

	char St;
	long LB;
	long Lv;
	long LV;
	long Le;
	long LE;
	long VT;
	long Vt;
	long V3;
	long Ls;

	Roll_Back_Intsect = 0;

	Is_Error_Output = 2;
	strncpy(YY_5String, YY_Layer[YI_Layer].I, 30);
	strncat(YY_5String, "$ERR", 5);

fprintf(stderr, "\n! Layer %s? Pline At Point (%lf, %lf) Repaired",
YY_Layer[YI_Layer].I, ZLDCO0[0]/UN006, ZLDCO1[0]/UN006);
error_level = 2;

	STATUS = 1;
	ZZ_EXPLODE_CHAIN();

	Lb_Lim = ZLBLIM;

	Le = ZLBCO3[Lb];
	Lv = Le;
	if(!Le)return;

	Le = 1L;
	Le = ZLVCO8[Le];
	ZLBCO3[Lb] = Le;
	Lv = Le;

	/*After DummyJunctionAnalysis Le is*/
	/*Never Coincidenced.*/
	do
	{
	Ls = FETCHREVERSE(Lb, Lv);
	if(Ls)
	{
	ZLBCO3[Lb] = Lv;
	Le = Lv;
	break;
	}
	Lv = ZLVCO1[Lv];
	}
	while(Lv!=Le);
	do
	{
	Vt = ZLVCO2[Lv];
	VT = ZVTCO6[Vt];

	if(Z_Stat(VT,Vt))
	{
	do
	{
	Lv = ZLVCO1[Lv];
	Vt = ZLVCO2[Lv];
	VT = ZVTCO6[Vt];
	}
	while(Z_Stat(VT,Vt)&&Lv!=Le);

	if(Lv==Le)break;
	}
	if(ZVTCO8[Vt]==-1)V3 = VT, Vt = VT;
	else V3 = Vt;

	LB = ZLBNEXT();

	Lv_Lim = LE = ZLVLIM;
	ZLBCO3[LB] = LE;
	do
	{
	LV = ZLVNEXT();
	ZLVCO8[LV] = 0;
	ZVTCO8[V3] = 1;
	ZLVCO3[LV] = ZVTCO5[V3];
	ZLVCO2[LV] = V3;
	ZLVCO7[LV] = LB;
	ZLVCO1[LE] = LV;
	ZVTCO5[V3] = LV;
	ZLVCO5[LV] = ZVTCO1[V3];
	ZLVCO6[LV] = ZVTCO2[V3];
	LE = LV;
	V3 = FETCHSUCCESSOR(ZVTCO6[V3]);
	}
	while(Vt!=V3);
	LE = ZLVCO1[LV] = ZLBCO3[LB];

	DUMMYJUNCTIONANALYSIS(LB);
	DETERMINE_STAT_2(LB);

	if(STATUS!=1)
	{
	ZLBLIM -= 1;
	LV = LE;
	ZLVLIM = Lv_Lim;
	do
	{
	REMOVE_FROM_VT_CHAIN(LV);
	VT = ZLVCO2[LV];
	ZVTCO8[VT] = -1;

	LV = ZLVCO1[LV];
	}
	while(LV!=LE);
	St = 1;
	}
	else
	St = 0,
	Lv = ZLVCO1[Lv];
	}
	while(St||Lv!=Le);

	if(Lb_Lim==ZLBLIM)
	{
	Roll_Back_Intsect = 1;
	return;
	}
	ZLBCO3[Lb] = 0;
	Lv = Le;
	do
	{
	REMOVE_FROM_VT_CHAIN(Lv);
	Vt = ZLVCO2[Lv];
	VT = ZVTCO6[Vt];

	ZVTCO8[Vt] = 0;
	ZVTCO8[VT] = 0;
	ZLVCO7[Lv] = 0;

	Lv = ZLVCO1[Lv];
	}
	while(Lv!=Le);
	if(ZLBLIM>3)COMBINE_C_POLYGONS();

	strncpy(YY_5String, YY_Layer[YI_Layer].I, 30);
	strncat(YY_5String, "$NEW", 5);
	ZZ_EXPLODE_CHAIN();

	Is_Error_Output = 0;
	ZZ_EXPLODE_CHAIN();
}
#undef Z_Stat
void
Terminate_Poly_2(void)
{
	long J;

	ZLBLIM = 1;
	ZLVLIM = 1;
	ZPTLIM = 1;
	ZYPLIM = 1;
	ZVTLIM = 2;

	J = ZLDLIM;
	--J;
	if(ZLDCO0[0]!=ZLDCO0[J]||ZLDCO1[0]!=ZLDCO1[J])
	{
	if(!Closed_Polygon)
	{
	fprintf(stderr, "\n! Layer %s? Not Closed Pline At Point (%lf, %lf) Discarded",
	YY_Layer[YI_Layer].I, ZLDCO0[J]/UN006, ZLDCO1[J]/UN006);
	return;
	}
	J = ZLDNEXT();
	ZLDCO0[J] = ZLDCO0[0];
	ZLDCO1[J] = ZLDCO1[0];
	}
	if(without_pcheck)
	{
	DETERMINE_STAT_1();
	if(STATUS){
	FF_Explode_Chain(); return;}
	fprintf(stderr, "\nInput data need check\n");
	zabort();
	}
	Z_FEval_Storage();
	if(ZLBLIM<2)
	{
	fprintf(stderr, "\n! Layer %s? Strange Pline At Point (%lf, %lf) Discarded",
	YY_Layer[YI_Layer].I, ZLDCO0[J]/UN006, ZLDCO1[J]/UN006);
	return;
	}
	INTERSECTION_ANALYSIS();
	POINT_VECTOR_ANALYSIS(2);
	STATUS = 0;
	DUMMYJUNCTIONANALYSIS(1);
	DETERMINE_STAT_2(1);
	if(!STATUS)
	{
	ZZ_REPAIR_PLINE(1);
	return;
	}
	/*NON Zero Status*/
	else
	{
		void POINT_VECTOR_CLASS(long); 
		POINT_VECTOR_CLASS(1);
		if(SI_Error)
		{
			ZZ_REPAIR_PLINE(1);
			return;
		}
	}
	ZZ_EXPLODE_CHAIN();
}
void
OV_EL_SEGMENTS(void)
{
	long I;
	/*Overlap Elimination*/
	ZVTLIM = 2;

	INTERSECTION_ANALYSIS();
	POINT_VECTOR_ANALYSIS(2);

	for(I = 1; I<ZLBLIM; ++I)
	{
	if(ZLBCO3[I]==0)
	{
	continue;
	}
	DUMMYJUNCTIONANALYSIS(I);
	ZZ_SET_RSTAT(I);
	}
	FORCE_C_POLYGONS();
}
long Back_Scale_0(void);
void
Terminate_Poly_1(void)
{
	long S;
	long K;
	long T;
	long I;
	long J;
	long Q;
	long Z;

	if(!YY_VALID_LAYER())
		return;

	if(VERLIM<3)
	{
		return;
	}
	if(YY_Interpolate&&Closed_Polygon)
	if(!ENVEQ(VERCOX[VERLIM-1],VERCOX[1], Epsilon) || !ENVEQ(VERCOY[VERLIM-1],VERCOY[1], Epsilon))
	{
		I = VERNEXT();
		VERCOX[I] = VERCOX[1];
		VERCOY[I] = VERCOY[1];

		YY_Interpolate_Arc();
		VERLIM--;
	}
	K = 0;
	{
		/*Erases Duplicate Vertexes*/
		long ZZ[4];
		PXMIN = PYMIN = LONG_MAX;
		PXMAX = PYMAX =-LONG_MAX;

		MAX_WIDTH *= 1.5;

		for(I = 1; I<VERLIM-1; ++I)
		{
			if(VERCOX[I]==VERCOX[I+1]&&VERCOY[I]==VERCOY[I+1])
			{
				VERCOX[I] = VERCOY[I] = MAXFLOAT;
				continue;
			}
			else ++K;
			ZZ[0] = VERCOX[I]-MAX_WIDTH;
			ZZ[1] = VERCOY[I]-MAX_WIDTH;
			ZZ[2] = VERCOX[I]+MAX_WIDTH;
			ZZ[3] = VERCOY[I]+MAX_WIDTH;

			if(PXMIN>ZZ[0]) PXMIN = ZZ[0];
			if(PXMAX<ZZ[2]) PXMAX = ZZ[2];
			if(PYMIN>ZZ[1]) PYMIN = ZZ[1];
			if(PYMAX<ZZ[3]) PYMAX = ZZ[3];
		}
	}
	/*END-OF-CHECK-DUPLICATES-POINTS*/
	if(K==0)
	{
		return;
	}
	Determine_Point_Base();
	/*Cheks Segments With Zero Width*/
	J = K = 0;
	if(MAX_WIDTH>=0.5)
		for(I = 1; I<VERLIM; ++I)
		{
			if(VERCOX[I]>LONG_MAX)
			{
				VERSTW[I] /= 2.0;
				VERENW[I] /= 2.0;
				continue;
			}
			if(VERSTW[I]<0.5&&VERENW[I]<0.5) ++J;
			else
			{
				++K;
				VERSTW[I] /= 2.0;
				VERENW[I] /= 2.0;
			}
		}
	if(K) 
		Sized_Shape = 1; 
	else 
		Sized_Shape = 0;
	if(!Sized_Shape)
	{
		ZLDLIM = 0;
		for(I = 1; I<VERLIM; ++I)
		{
			if(VERCOX[I]>LONG_MAX)continue;
			J = ZLDNEXT();
			ZLDCO0[J] = floor(VERCOX[I]+0.5);
			ZLDCO1[J] = floor(VERCOY[I]+0.5);
		}
		Terminate_Poly_2();
		return;
	}
	/*BUILDS-FIRST-AND-LAST-ITEMS*/
	S = 1;
	if(Closed_Polygon)
	{
		/*CONTINUES LASTPOINT BY FIRSTPOINT*/
		J = 1, K = VERLIM-1;
		I = VERNEXT();
		while(VERCOX[J]>LONG_MAX)++J;
		while(VERCOX[J]==VERCOX[K]&&VERCOY[J]==VERCOY[K])
		{
			VERCOX[J] = MAXFLOAT;
			++J;
			while(VERCOX[J]>LONG_MAX) ++J;
		}
		VERCOX[I] = VERCOX[J];
		VERCOY[I] = VERCOY[J];

		VERSTW[I] = VERSTW[1];
		VERENW[I] = VERENW[1];

		/*CONTINUES FIRSTPOINT BY LASTPOINT*/
		VERCOX[J-1] = VERCOX[K];
		VERCOY[J-1] = VERCOY[K];

		VERSTW[J-1] = VERSTW[K];
		VERENW[J-1] = VERENW[K];
		S = J, T = ++J;
		while(VERCOX[T]>LONG_MAX) ++T;
		I = VERNEXT();
		VERCOX[I] = VERCOX[T];
		VERCOY[I] = VERCOY[T];

		VERSTW[I] = VERSTW[J];
		VERENW[I] = VERENW[J];
		Q = S-1, Z = S;
	}
	else
	/*SPREADS END POINT TOWARD*/
	{
		J = VERLIM -1;
		I = VERNEXT();
		while(VERCOX[J]>LONG_MAX) --J;

		if(I_LS_DF)
		{
			VERCOX[I] = I_VAL_NVX;
			VERCOY[I] = I_VAL_NVY;
		}
		else
		{
			K = J-1;
			while(VERCOX[K]>LONG_MAX) --K;
			VERCOX[I] = 2*VERCOX[J]-VERCOX[K];
			VERCOY[I] = 2*VERCOY[J]-VERCOY[K];
		}
		VERSTW[I] = VERENW[J];
		VERENW[I] = VERENW[J];

		VERSTW[0] = VERSTW[1];
		VERENW[0] = VERENW[1];
		S = 1;
		while(VERCOX[S]>LONG_MAX) ++S;
		T = S+1;
		while(VERCOX[T]>LONG_MAX) ++T;
		if(I_FR_DF)
		{
			VERCOX[S-1] = I_VAL_PVX;
			VERCOY[S-1] = I_VAL_PVY;
			Q = S-1, Z = S;
		}
		else
			Q = S, Z = T;
	}
	/*MOVES ENDWIDTH TOWARD*/
	for(J = VERLIM-1; J; --J) VERENW[J] = VERENW[J-1];
	/*SCALE POINTS*/
	I = PXMAX>0? PXMAX: -PXMAX;
	J = PYMAX>0? PYMAX: -PYMAX;
	if(I>J)J = I;
	I = PXMIN>0? PXMIN: -PXMIN;
	if(I>J)J = I;
	I = PYMIN>0? PYMIN: -PYMIN;
	if(I>J)J = I;

	S_FACTOR = 0;
	do
	{
		++S_FACTOR;
	}
	while(((J<<S_FACTOR)>>S_FACTOR)==J);
	S_FACTOR--;
	if(S_FACTOR<0)S_FACTOR = 0;
	if(S_FACTOR>9)S_FACTOR = 9;

	Without_Scale = S_FACTOR==0;

	XPDIV <<= S_FACTOR;
	YPDIV <<= S_FACTOR;
	PXMAX <<= S_FACTOR;
	PYMAX <<= S_FACTOR;
	PXMIN <<= S_FACTOR;
	PYMIN <<= S_FACTOR;
	/*SETS START VERTEX*/
	DEFINE_NORMALFROM(Q, Z);
	XELBOW = NXFROM*VERSTW[Q];
	YELBOW = NYFROM*VERSTW[Q];
	ZLBLIM = 1;
	ZLVLIM = 1;
	ZPTLIM = 1;
	ZYPLIM = 1;
	ZVTLIM = 2;
	ZJSLIM = 0;
	ZLDLIM = 0;
	if(Closed_Polygon||I_FR_DF)DEFINE_SEGMENT(S-1, S, T);

	Last_Segment = 0;
	if(!Closed_Polygon&&I_FR_DF)
	{
		CORRECT_ELBOW(VERSTW[S]);
	}
	ZLDLIM = 0;

	NO_CONT_2 = 1;

	K = S, S = T, ++T;
	while(T<VERLIM)
	{
		while(VERCOX[T]>LONG_MAX) ++T;

		if(T==VERLIM-1)
			Last_Segment = T;

		DEFINE_SEGMENT(K, S, T);

		if(VV_Cover)
		{
			NO_CONT_2 = 1;
			UNL_TOINTLAYSTANDART(XCOVER, YCOVER);
			NO_CONT_1 = 1;
		}
		K = S, S = T, ++T;
	}
	if(ZLDLIM)
		Unl_ToIntlayStandart();
	Zptlim_After_Loadmaster = ZPTLIM;

	OV_EL_SEGMENTS();

	if(!Without_Scale)
		for(I = 1; I<ZLBLIM; ++I)
		{
			if(!ZLBCO3[I])continue;

			DUMMYJUNCTIONANALYSIS(I);
		}
	if(Back_Scale_0()!=0)
	{
		OV_EL_SEGMENTS();
	}
	ZZ_EXPLODE_CHAIN();

	S_FACTOR = 8;
	MAX_WIDTH = 0;
	Without_Scale = 0;
	return;
}
#define Y_L YY_Layer[YI_Layer]
#define L_I YY_Layer[LI_Layer]
/*Mascot Output*/
void
Set_Mascot_OStream(void)
{
	Dix_Out_Buf = Y_L.B;
	Dix_Out_Pos = Y_L.P;
	Dix_Out_Len = Y_L.L;

	Dix_File = Y_L.z;
	Dix_Name = Y_L.Z;
}
void
ITEMGEN1(void)
{
	if(Y_L.z==NULL)
	/*Create File Struct*/
	{
	if(S_BLEN-S_BPOS<15)zabort();

	if(Y_L.Z==NULL)
	/*Get New Temp Name*/
	{
	Y_L.Z = &S_BTOP[S_BPOS];

	tmpnam(Y_L.Z);
	Y_L.Z[0] = 'x'; 
	S_BPOS+= strlen(Y_L.Z)+1;
	}
	/*End Temp Name*/
	Y_L.z = Fopen(Y_L.Z, "w+b");

	FP_S.Delta_X = 0;
	FP_S.Delta_Y = 0;

	fwrite(&FP_S, sizeof(FP_S), 1, Y_L.z);

	if(Y_L.B==NULL)
	/*Alloc I/O Buffer*/
	{
	Y_L.B = (char*)malloc((Y_L.L = 512));
	if(!Y_L.B)zabort();
	}
	/*End Alloc*/
	Y_L.P = 0;
	}
	/*End Create File*/
	if(ZLDLIM==0)return;

	Set_Mascot_OStream();
	Dix_output();

	Y_L.p++, Y_L.v+= ZLDLIM;

	Y_L.P = Dix_Out_Pos;
}
#define Y_I YI_Layer
#define O_L O_Layer
void OUTCLOS1(void)
{
	long v = 0;
	char z = 0;
	char Z = 1;
	if(YY_Layer_Lim==1)return;

	if(CXMIN==+LONG_MAX)CXMIN = 0;
	if(CYMIN==+LONG_MAX)CYMIN = 0;
	if(CXMAX==-LONG_MAX)CXMAX = 0;
	if(CYMAX==-LONG_MAX)CYMAX = 0;

	if(YY_BOUND)
	{
	LZ_S_Changed = 1;
	LZ_S.CXMIN = (CXMIN-=LZ_S.UN003);
	LZ_S.CXMAX = (CXMAX+=LZ_S.UN003);
	LZ_S.CYMIN = (CYMIN-=LZ_S.UN003);
	LZ_S.CYMAX = (CYMAX+=LZ_S.UN003);
	}
	for(Y_I = 1; Y_I<YY_Layer_Lim; ++Y_I)
	{
	if(Y_L.v>v) v = Y_L.v;
	}
fprintf(stderr, "\ntotal points on max layer = %ld", (long)v);
	v = (v>>7)<<7;
	BLCNT = (LVCNT+v)/LVCNT;
fprintf(stderr, "\nevaluated blocks count = %ld", (long)BLCNT);

	z = BLCNT>(XBCNT*YBCNT);

	Z&= CXMIN>=LZ_S.CXMIN;
	Z&= CXMAX<=LZ_S.CXMAX;
	Z&= CYMIN>=LZ_S.CYMIN;
	Z&= CYMAX<=LZ_S.CYMAX;

	if(Z==0)
	{
	Print_Note_Message();
	fprintf(stderr, "\nWARNING!GRAPHICS DATA OUT OF BOUNDARY");
	Print_Note_Message();
	}
	if(z&&Z)
	{
	long DX = CXMAX-CXMIN;
	long DY = CYMAX-CYMIN;
	long dx = LZ_S.CXMAX-LZ_S.CXMIN;
	long dy = LZ_S.CYMAX-LZ_S.CYMIN;

	long blcnt;

	double z; z = (double)DX/(double)DY;
	if(z<1.0)z = 1.0/z;

	blcnt = floor(z*sqrt((double)BLCNT));
	if(blcnt<2)blcnt = 2;

	UBSIZ = DX>DY? DX:DY;

	if(BLCNT<=7||blcnt>=BLCNT-1)
	{
	blcnt = ++BLCNT;
	UBSIZ = (((UBSIZ+blcnt)/blcnt)<<1) + 1;
	}
	else
	UBSIZ = ((UBSIZ+blcnt)/blcnt) + 1;

	XBCNT = (DX + UBSIZ)/UBSIZ;
	YBCNT = (DY + UBSIZ)/UBSIZ;
	XBSIZ = (dx + XBCNT)/XBCNT;
	YBSIZ = (dy + YBCNT)/YBCNT;

	if(XBSIZ%UN001)
	XBSIZ = UN001+((XBSIZ/UN001)*UN001);
	if(YBSIZ%UN001)
	YBSIZ = UN001+((YBSIZ/UN001)*UN001);

	if((XBCNT*YBCNT)==1)
	{
	XBSIZ = dx;
	YBSIZ = dy;
	}
	LZ_S.XBSIZ = XBSIZ;
	LZ_S.YBSIZ = YBSIZ;

	LZ_S.XBCNT = XBCNT;
	LZ_S.YBCNT = YBCNT;

	LZ_S_Changed = 1;
	}
fprintf(stderr, "\nx-blocks count = %ld", (long)XBCNT);
fprintf(stderr, "\ny-blocks count = %ld", (long)YBCNT);
	if(YY_BOUND)
	{
	CXMIN+=LZ_S.UN003;
	CXMAX-=LZ_S.UN003;
	CYMIN+=LZ_S.UN003;
	CYMAX-=LZ_S.UN003;
	}
	O_L.LX_S.CXMIN = CXMIN;
	O_L.LX_S.CXMAX = CXMAX;
	O_L.LX_S.CYMIN = CYMIN;
	O_L.LX_S.CYMAX = CYMAX;

	O_L.LX_S.XBCNT = 1;
	O_L.LX_S.YBCNT = 1;
	O_L.LX_S.XBSIZ = CXMAX-CXMIN;
	O_L.LX_S.YBSIZ = CYMAX-CYMIN;

	for(Y_I = 1; Y_I<YY_Layer_Lim; ++Y_I)
	{
		fpos_t P = 0;
		if(Y_L.z==NULL)
		{
			fprintf(stderr, "\nWARNING!LAYER %s DISCARDED", Y_L.I);
			continue;
		}
		if(Y_L.P) fwrite(Y_L.B, 1, Y_L.P, Y_L.z);
		Y_L.P = 0;
		fflush(Y_L.z);

		fsetpos(Y_L.z, &P);
		FP_S.Delta_X = LZ_S.CXMIN;
		FP_S.Delta_Y = LZ_S.CYMIN;
		fwrite(&FP_S, sizeof(FP_S), 1, Y_L.z);
		fflush(Y_L.z);
		fclose(Y_L.z);

		memcpy(O_L.PdbN, Y_L.Z, strlen(Y_L.Z)+1);
		memcpy(O_L.Name, Y_L.O, strlen(Y_L.O)+1);

		renameoutput();
	}
	LZ_S.UN003 = 0;

	Lz_Lx_Close();
}
void OUTOPEN1(void)
{
	if(!Is_Mas_Input)
	Initial_Setup_2();

	if(CXMIN==CXMAX)
	{
	YY_BOUND = 1;
	}
	CXMIN = LONG_MAX;
	CYMIN = LONG_MAX;
	CXMAX =-LONG_MAX;
	CYMAX =-LONG_MAX;
}
void INPCLOS1(void)
{
}
void
Long_Item_Store(void)
{
	Dix_Layer = Y_I;

	Y_I = 0;
	ITEMGEN1();

	Y_I = Dix_Layer;
}
void
Relink_Long_Storage(void)
{
	struct YY_layer T_L = L_I;
	Y_I = 0;
	L_I = Y_L;

	if(T_L.z)
	{
	fclose(T_L.z);
	unlink(T_L.Z);

	T_L.z = NULL;
	}
	Y_L = T_L;
}
void
SPLIT_POLYGON(void);
void
Processe_Long_Storage(void)
{
	Relink_Long_Storage();
	while(L_I.z)
	{
	fprintf(stderr, "\nPROCESSE %ld REJECTED POLYGONS", L_I.p);
	DIX_Layer = 0;

	if(L_I.P)
	fwrite(L_I.B, 1, L_I.P, L_I.z);
	fflush(L_I.z);

	L_I.P = 0;
	rewind(L_I.z);

	Zd_File = L_I.z;

	Zd_File_Buf = L_I.B;
	Zd_File_Ptr = L_I.L;
	Zd_File_Eof = 0;
	Zd_File_Lim = L_I.L;

	Zd_Delta_X = 0;
	Zd_Delta_Y = 0;

	Zd_Fragment = &Z_Fragment;
	Z_Fragment = 0;
	fread(&FP_S, sizeof(FP_S), 1, L_I.z);
	while(Z_Fragment!=LONG_MAX)
	{
	Z_Read_Polygon();
	if(ZLDLIM==0)
	{
	Y_I = Dix_Layer;
	continue;
	}
	SPLIT_POLYGON();
	}
	Relink_Long_Storage();
	}
}
/*Source Output*/
#define OUT Output_File
#define NAM Output_Name
void
ITEMGEN2(void)
{
	long L;
	char x;
	char y;

	long OLDX;
	long OLDY;

	static short NULITEM, SOU_PTR = 0, P;
	short POLY[256];
	union { long L; short I[2]; } U;

	if(ZLDLIM>60)
	{
	Long_Item_Store();
	return;
	}
	for(L = 0; L<7; ++L)POLY[L] = 0;

	OLDX = OLDY = LONG_MAX;
	for(L = 0, P = 7; L<ZLDLIM; ++L)
	{
	x = (OLDX==ZLDCO0[L]);
	y = (OLDY==ZLDCO1[L]);
	OLDX = ZLDCO0[L];
	OLDY = ZLDCO1[L];
	if(!y)
	{
	U.L = (ZLDCO1[L]+(long)ZZ_DELTA_Y)<<3;
	if(x) U.L |= 1;
	POLY[P++] = U.I[1];
	POLY[P++] = U.I[0];
	}
	if(!x)
	{
	U.L = (ZLDCO0[L]+(long)ZZ_DELTA_X)<<3;
	if(y) U.L |= 2;
	POLY[P++] = U.I[1];
	POLY[P++] = U.I[0];
	}
	} /*END-FOR-EACH-POINT*/
	POLY[0] = (P-1) | 013400;
	POLY[1] = Y_L.o;
	if((SOU_PTR+P) > 256)
	{
	NULITEM = 256 - ++SOU_PTR;
	fwrite(&NULITEM, 2, NULITEM+1, OUT);
	SOU_PTR = 0 ;
	}
	if(P>7)
	{
	fwrite(POLY, 2, P, OUT);
	SOU_PTR += P;
	}
	Ferror(OUT, NAM);
}
void OUTCLOS2(void)
{
	fpos_t P;
	long I;
	long B = 0;
	if(!OUT)zabort();
	Processe_Long_Storage();

	fgetpos(OUT, &P);
	I = P - ((P>>8)<<8);
	if(I) while(I++<512) fwrite(&B, 1, 1, OUT);
	fwrite(&B, 1, 1, OUT);
	B = 4;
	fwrite(&B, 1, 1, OUT);
	I = B = 0;
	while(I++<510) fwrite(&B, 1, 1, OUT);
	fclose(OUT);
}
void OUTOPEN2(void)
{
Output_File = Fopen(Output_Name, "wb");
}
#undef OUT
#undef NAM
void INPCLOS2(void)
{
}
void ITEMGEN3(void)
{
	static char*N;
	long L;
	if(ZLDLIM<=1)return;
	if(N!=Y_L.O)
	{
	write_layer(Output_File, Y_L.O);
	N = Y_L.O;
	}
	write_pline(Output_File, (short)1);

	for(L = 0; L<ZLDLIM-1; ++L)
	{
	write_vertex(Output_File, ZLDCO0[L], ZLDCO1[L]);
	}
	write_seqend(Output_File);
}
#undef Y_L
#undef Y_I
#undef L_I
#undef O_L
void OUTCLOS3(void)
{
char id = 0;
fwrite(&id, 1, 1, Output_File);
fclose(Output_File);
Ferror(Output_File, Output_Name);
}
void OUTOPEN3(void)
{
Output_File = Fopen(Output_Name, "wb");
write_header(Output_File);
set_double(Output_File);
}
void INPCLOS3(void)
{
}
void ITEMGEN4(void)
{
}
void OUTCLOS4(void)
{
}
void OUTOPEN4(void)
{
}
void INPCLOS4(void)
{
}
void ITEMGEN5(void)
{
}
void OUTCLOS5(void)
{
}
void OUTOPEN5(void)
{
}
void INPCLOS5(void)
{
}
void ITEMGEN6(void)
{
}
void OUTCLOS6(void)
{
}
void OUTOPEN6(void)
{
}
void INPCLOS6(void)
{
}
ZZ_fun *ZZ_Write[6] = {ITEMGEN1, ITEMGEN2, ITEMGEN3, ITEMGEN4, ITEMGEN5, ITEMGEN6};
ZZ_fun *ZZ_Close[6] = {OUTCLOS1, OUTCLOS2, OUTCLOS3, OUTCLOS4, OUTCLOS5, OUTCLOS6};
ZZ_fun *ZZ_Oopen[6] = {OUTOPEN1, OUTOPEN2, OUTOPEN3, OUTOPEN4, OUTOPEN5, OUTOPEN6};
ZZ_fun *ZI_Close[6] = {INPCLOS1, INPCLOS2, INPCLOS3, INPCLOS4, INPCLOS5, INPCLOS6};

ZZ_fun *ZZ_PARSE[6] = {yyparse_mas, yyparse_sou, yyparse_dxf, NULL, NULL, NULL};
ZZ_fun *ZZ_CSCAN[6] = {yycscan_mas, yycscan_sou, yycscan_dxf, NULL, NULL, NULL};

void
FF_Convert_Cell(struct FF_Cell*Cell)
{
	ZZ_YY_CELL = Cell;

	ZZ_Parse();
}
/**/
void
Initial_Setup_4(void)
{
	YY_Layer = (YY_layer*)calloc(51, sizeof(struct YY_layer));
	if(YY_Layer==NULL)zabort();

	LI_Layer = 50;

	YY_Layer_Lim = 1l;
	YY_Layer_Max = 50;
}
void
Initial_Setup_3(void)
{
	long S;
	Closed_Polygon = 1;

	PXCNT = 32;
	PYCNT = 32;

	PBLIM = PXCNT*PYCNT;
	ZPBCOR = (unsigned short*)malloc(PBLIM*2);

	IPOINT = IPOINT3;

	S_BLEN-= 256;
	YY_1String = &S_BTOP[S_BLEN];
	YY_2String = YY_1String;
	S_BLEN-= 256;
	YY_6String = &S_BTOP[S_BLEN];
	S_BLEN-= 30;
	YY_3String = &S_BTOP[S_BLEN];
	S_BLEN-= 30;
	YY_4String = &S_BTOP[S_BLEN];
	S_BLEN-= 30;
	YY_5String = &S_BTOP[S_BLEN];
	S_BLEN-= 30;
	YY_7String = &S_BTOP[S_BLEN];

	if(Is_Dxf_Input)
	{
	/*Bison-Parser Should Be Implemented*/
	I_BTOP = (char*)malloc(20500);
	I_BLEN = 20480;
	I_Read();
	yylex = DXFyylex;
	}
	if(YY_Layer==NULL)Initial_Setup_4();

	LF_LIM = 2560;

	Lf_Sdown = (char*)malloc(LF_LIM*1);
	Lf_Rolld = (unsigned short*)malloc(LF_LIM*2);
	Lf_ROLLD = (unsigned short*)malloc(LF_LIM*2);
	Lf_First = (unsigned short*)malloc(LF_LIM*2);
	Lf_Upper = (unsigned short*)malloc(LF_LIM*2);
	Lf_Lower = (unsigned short*)malloc(LF_LIM*2);
	Lf_Ymidl = (long*)malloc(LF_LIM*4);
	Lf_Stack = (unsigned short*)malloc(LF_LIM*2);

	Lf_Factor = 1;
	while(LF_LIM>>Lf_Factor)
	++Lf_Factor;
	--Lf_Factor;

	FF_Stack = (long*)malloc(sizeof(long)*100);
	FF_Stack_Lim = 0;
	FF_Stack_Max = 100;

#ifdef __BORLANDC__
	if(Is_Funum==1)
	S = ((coreleft()/2) - FF_Stack_Max*sizeof(long))>>1;
	else
	S = ((coreleft()/3) - FF_Stack_Max*sizeof(long))>>1;
#else
	S = (EMSIZ/3)*1024; /*EMS-SIZE=1000 Pages*/
#endif

	if(Is_Funum==2)S = 500;
	else S>>=1;

	FF_Heap_1 = (char*)malloc(S);
	FF_Heap_1_Lim = 1;
	FF_Heap_1_Max = S;

	FF_Cell_Top = 0;
	FF_Cell_Lim = 0;

	FF_Heap_2 = (char*)malloc(S);
	FF_Heap_2_Lim = 0;
	FF_Heap_2_Max = S;

	FF_RM0000 =
	FF_Find_RM(&RM0000);
	FF_RM0001 =
	FF_Find_RM(&RM0001);
	FF_RM0002 =
	FF_Find_RM(&RM0002);
	FF_RM0003 =
	FF_Find_RM(&RM0003);
	FF_RM0010 =
	FF_Find_RM(&RM0010);
	FF_RM0011 =
	FF_Find_RM(&RM0011);
	FF_RM0012 =
	FF_Find_RM(&RM0012);
	FF_RM0013 =
	FF_Find_RM(&RM0013);

#ifdef __BORLANDC__
	S = coreleft() -25000;
#else
	if(Is_Funum==2)
	S = EMSIZ*1024 + 200000L;
	else
	S = (EMSIZ/3)*2048 + 250000L;
#endif

	S /= 2*(Z_LD_Sz) + Z_LB_Sz + Z_LV_Sz + Z_VT_Sz + Z_PT_Sz + Z_VX_Sz;

	ZLDALLOC((long)(S*Z_LD_Wt));
	ZldALLOC((long)(S*Z_LD_Wt));
	ZLBALLOC((long)(S*Z_LB_Wt));
	ZLVALLOC((long)(S*Z_LV_Wt));
	ZVTALLOC((long)(S*Z_VT_Wt));
	ZPTALLOC((long)(S*Z_PT_Wt));
	ZVXALLOC((long)(S*Z_VX_Wt));

	UN006 = UN002/UN004;

	Epsilon *= UN004*UN001;
	Epsilon_2 *= UN004*UN001;
}
void
Com_Line_Scan(short N, char*A[])
{
	long I;
	long K;
	char M;
	char e;
	char o;
	char i;
	char m;
	char s;

	char * P;

	char ERR;
	if(N<2)zabort();

	e = s = o = i = m = ERR = 0;

	for(I = 1; I<N; ++I)
	{
		for(K = 0; A[I][K]; ++K) if(islower(A[I][K]))A[I][K] = toupper(A[I][K]);

		if(A[I][0]=='-') /*OPTIONAL PARAMETER*/
		switch(A[I][1])
		{
		case 'C': /*WITHOUT PCHECK*/
			if(A[I][2]) ERR = 1;
			without_pcheck = 1;
			break;
		case 'I': /*INPUT FORMAT*/
			M = A[I][2];
			if(!M) ERR = 1;
			else
			if(A[I][3])
			{
				Input_Name = SS_Fetch(&A[I][3]);
				S_BPOS += 5;
				for(K = 3; A[I][K]; ++K)
				{
					if(A[I][K]=='.')ERR = 1;
				}
				Input_Cell = SS_Fetch(Input_Name);
			}
			if(ERR)break;
			switch(M)
			{
			case 'M': /*MAS*/
				ERR = Input_Name!=NULL || ZZ_Funum==0;

				Is_Funum = 0;

				break;
			case 'S': /*SOU*/

				Is_Funum = 1;

				break;
			case 'G': /*GDS*/
				ERR = 3;
				break;
			case 'P': /*PDF*/
				ERR = 3;
				break;
			case 'C': /*CIF*/
				ERR = 3;
			break;
			case 'D': /*DXF*/
				UN002 = 1000;

				Is_Funum = 2;

				break;
			default: ERR = 4;
				break;
			}
			if(ERR)break;
			if(i)ERR = 2;
			i = 1;
			break;

		case 'O': /*OUTPUT FORMAT*/
			M = A[I][2];
			if(!M) ERR = 1;
			else
			if(A[I][3])
			{
				Output_Name = SS_Fetch(&A[I][3]);
				S_BPOS += 5;
				for(K = 3; A[I][K]; ++K)
				{
					if(A[I][K]=='.')ERR = 1;
				}
			}
			if(ERR)break;
			switch(M)
			{
			case 'M': /*MAS*/
				ERR = Output_Name!=NULL || Is_Funum==0;
				ZZ_Funum = 0;
				break;
			case 'S': /*SOU*/
				ZZ_Funum = 1;
				break;
			case 'G': /*GDS*/
				ERR = 3;
				break;
			case 'P': /*PDF*/
				ERR = 3;
				break;
			case 'C': /*CIF*/
				ERR = 3;
				break;
			case 'D': /*DXB*/
				UN002 = 1000;

				ZZ_Funum = 2;
				break;
			default: ERR = 4;
				break;
			}
			if(ERR)break;
			if(o)ERR = 2;
			o = 1;
			break;

		case 'E': /* EMS - SIZE IN PAGES */
			EMSIZ = strtod(&A[I][2], &P);
			if(EMSIZ<=0)ERR = 1;
			else
			if(A[I][2]==0)ERR = 1;
			else
			if(P[0]!=0)ERR = 1;
			else
			if(e)ERR = 2;
			e = 1;
			break;

		case 'M': /* MAGNIFICATION */
			UN004 = strtod(&A[I][2], &P);
			if(A[I][2]==0)ERR = 1;
			else
			if(P[0]!=0)ERR = 1;
			else
			if(m)ERR = 2;
			m = 1;
			break;

		case 'S': /* STEP SIZE (FOR DXF-DXB ONLY)*/
			UN001 = strtod(&A[I][2], &P);
			if(A[I][2]==0)ERR = 1;
			else
			if(P[0]!=0)ERR = 1;
			else
			if(s)ERR = 2;
			s = 1;
			break;

		default: ERR = 4;
			break;
		}
		else /*LAYER: [OUTPUT NAME=]INPUT NAME*/
		{
			char * L_O_N;
			char * L_I_N;
			L_O_N = SS_Fetch(A[I]);

			for(K = 0; L_O_N[K]; ++K)
			{
				if(L_O_N[K]=='=')break;
			}
			if(L_O_N[K])L_I_N = L_O_N+K+1, L_O_N[K] = 0;
			else L_I_N = L_O_N;

			if(strlen(L_I_N)==0) {ERR = 1; break;}
			if(strlen(L_O_N)==0) {ERR = 1; break;}

			for(K = 0; L_O_N[K]; ++K)
			{
				if(isalpha(L_O_N[K]))continue;
				if(isdigit(L_O_N[K]))continue;

				break;
			}
			if(L_O_N[K]) 
			{
				ERR = 1; 
				break;
			}

			for(K = 0; L_I_N[K]; ++K)
			{
				if(isalpha(L_I_N[K]))continue;
				if(isdigit(L_I_N[K]))continue;

				break;
			}
			if(L_I_N[K]) {
				ERR = 1; 
				break; 
			}

			if(YY_Layer==NULL)Initial_Setup_4();

			YY_Layer[LL_Insert(L_I_N)].O = L_O_N;
			YY_Layer_All = 0;
		}
		if(ERR)break;
	}
	/* End for A[I] */
	if(ERR==3)fprintf(stderr, "\nNOT ALLOWED FORMAT TYPE FOR THIS VERSION");
	if(ERR==2)fprintf(stderr, "\nAmbiguous Parameter %s", A[I]);
	if(ERR==1)fprintf(stderr, "\nStrange Parameter %s", A[I]);
	if(ERR==4)fprintf(stderr, "\nInvalid Parameter %s", A[I]);
	if(ERR)zabort();

	if(ZZ_Funum!=2||Is_Funum!=2)
	{
		if(UN001!=1) fprintf(stderr, "\n-S(tepsize) Parameter will not be in effect");
		UN001 = 1;
	}
	if(Input_Name==NULL)
	{
		if(Output_Name==NULL) Output_Name = SS_Fetch("main"),
		S_BPOS += 5;
		Input_Cell = SS_Fetch(Output_Name);
		Input_Name = SS_Fetch(Output_Name);
		S_BPOS += 5;
	}
	if(Output_Name==NULL)
	{
		Output_Name = SS_Fetch(Input_Cell);
		S_BPOS += 5;
	}
	prefcat(Input_Name, Is_Funum, 0);
	prefcat(Output_Name, ZZ_Funum, 1);

	if(Is_Funum==ZZ_Funum)
	{
		ERR |= !strncmp(Input_Name, Output_Name, 100);
	}
	if(ERR!=0)fprintf(stderr, "\nInvalid Input/Output File Names");
	if(ERR)zabort();

	if(ZZ_Funum==1)
	for(K = 1; K<YY_Layer_Lim; ++K)
	{
		YY_Layer[K].o = atol(YY_Layer[K].O);
		if(YY_Layer[K].o<=0)ERR |= 1;
		if(YY_Layer[K].o>99)ERR |= 1;
	}
	if(ERR!=0)fprintf(stderr, "\nInvalid Sou-Names");
	if(ERR)zabort();
}
void
dosconvertermain(short N, char*A[])
{
	UN001 = 1;
	UN002 = 1;
	UN004 = 1;
	UN006 = 1;

	EMSIZ = 900;
	YY_Layer_All = 1;

	Initial_Setup_1();

	Com_Line_Scan(N, A);

	UN002 = UN002*UN004*UN001;

	if(!Is_Mas_Input)
		Input_File = Fopen(Input_Name, "rb");
	else
		Initial_Setup_2();

	Initial_Setup_3();

	if(FF_Cell_Top!=FF_Find_CE(Input_Cell))zabort();

	FF_Insert_Entr(0, 0, 0, 0, 1, 1, FF_RM0000, FF_Cell_Top, 0);
	FF_CELL(FF_Cell_Top).F = Input_Name;

	ZZ_Cscan = ZZ_CSCAN[Is_Funum];
	ZZ_Parse = ZZ_PARSE[Is_Funum];
	Zz_Oopen = ZZ_Oopen[ZZ_Funum];
	Zz_Write = ZZ_Write[ZZ_Funum];
	Zz_Close = ZZ_Close[ZZ_Funum];
	Zi_Close = ZI_Close[ZZ_Funum];

	ZZ_Cscan();
	Zz_Oopen();

	FF_EXPLODE_CELLS();

	Zz_Close();

	ZLDLIM = 0;
	Dxb_Ld_Image_Maker(0, NULL);
	exit(error_level);
}
