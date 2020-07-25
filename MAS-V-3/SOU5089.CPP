#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#define min(a,b) ((a)>(b)?(b):(a))
#define max(a,b) ((a)>(b)?(a):(b))
#include "fxeval.hpp"
#include "fqeval.hpp"
#include "_scale.hpp"
#include "dxbout.hpp"
#include "fzinit.hpp"
#include "m_alcr.hpp"
#include "mgsort.hpp"
long*G_buf0_long;
char*G_buf0_char;
long*G_buf1_long;
long*G_buf2_long;
long*G_buf3_long;
long G_buf_lim_long;
long G_buf_max_long;
double ZZ_DELTA_X;
double ZZ_DELTA_Y;
#define G_buf0 G_buf0_long
#define G_buf1 G_buf1_long
#define G_buf2 G_buf2_long
#define G_buf3 G_buf3_long
#define G_blim G_buf_lim_long
#define G_bmax G_buf_max_long
#define MAX_POLY_LENGTH 32767    // 2**15-1
#define EDGE_LIM 7
#define STRIP_SIZE 128
char*sp5089="sp5089";
void Out_Of_Storage(void);
void Terminate_Poly_3(void);
long W_CXMAX;
long W_CYMAX;
long W_CXMIN;
long W_CYMIN;
short Inp_Sou_Layer;
short Current_Rangx;
short Current_Stepx;
long G_blimN(void)
{
	if(G_blim>=G_bmax)Out_Of_Storage();
	return G_blim++;
}
/**/
struct FF_Entr;
struct FF_Cell
{
	char*N; /*Name Of Cell*/
	char*F; /*Name Of File*/
	FF_Cell*X; /*Next Cell In Cell's Hash*/
	FF_Cell*x; /*Next Cell In Cell's Heap*/
	FF_Entr*R; /*A Entr's Heap Header*/
};
#define FF_Cell_Len sizeof(FF_Cell)
struct FF_AMatrix;
struct FF_Entr
{
	FF_Cell*I; /*Called Cell Index*/
	FF_AMatrix*M; /*Start Of Arrays For This Call*/
	FF_Entr*X; /*Next Call*/
};
#define FF_Entr_Len sizeof(FF_Entr)
struct FF_RMatrix /*Rotate Matrix*/
{
	double A; /*+COS*/
	double B; /*-SIN*/
	double D; /*+SIN*/
	double E; /*+COS*/
	FF_RMatrix*X; /*Next In RMHash*/
};
#define FF_RMatrix_Len sizeof(FF_RMatrix)
struct FF_AMatrix /*Array Of Insertion*/
{
	double DX;
	double DY;
	double SX;
	double SY;
	long KX;
	long KY;
	FF_RMatrix*RM; /*Orient Inside Array. (RMatrix Index)*/
	FF_AMatrix*NX; /*Next AMatrix*/
};
#define FF_AMatrix_Len sizeof(FF_AMatrix)
struct FF_RArray
{
	double SX; /*Already Rotated*/
	double sy;
	double SY; /*Already Rotated*/
	double sx;
	double DX; /*Already Rotated*/
	double DY;
	long KX;
	long KY;
	FF_RArray*NX; /*Array On Level-1*/
};
#define FF_RArray_Len sizeof(FF_RArray)
struct FF_RChain
{
	FF_RArray*RA; /*Start Of Arrays-Chain*/
	FF_RChain*NX; /*Next Chain*/
};
#define FF_RChain_Len sizeof(FF_RChain)
struct FF_Orient
{
	FF_RMatrix*RM; /*Rotation-Matrix*/
	FF_RChain*ST; /*Start Of Chains*/
	FF_Orient*NX; /*Next Orient*/
};
#define FF_ORient_Len sizeof(FF_Orient)
/**/
extern long YY_Xrelocate;
extern long YY_Yrelocate;
/**/
void**FF_Stack;
long FF_Stack_Lim;
long FF_Stack_Max;

FF_RMatrix*FF_RM0000;
FF_RMatrix*FF_RM0001;
FF_RMatrix*FF_RM0002;
FF_RMatrix*FF_RM0003;
FF_RMatrix*FF_RM0010;
FF_RMatrix*FF_RM0011;
FF_RMatrix*FF_RM0012;
FF_RMatrix*FF_RM0013;
FF_RMatrix RM0000 ={+1,  0,  0, +1};
FF_RMatrix RM0001 ={ 0, -1, +1,  0};
FF_RMatrix RM0002 ={-1,  0,  0, -1};
FF_RMatrix RM0003 ={ 0, +1, -1,  0};
FF_RMatrix RM0010 ={+1,  0,  0, -1};
FF_RMatrix RM0011 ={ 0, -1, -1,  0};
FF_RMatrix RM0012 ={-1,  0,  0, +1};
FF_RMatrix RM0013 ={ 0, +1, +1,  0};

FF_Cell**FF_CE_Hash; /*at least 32 entry must be*/
FF_RMatrix**FF_RM_Hash; /*at least 32 entry must be*/

FF_Cell*FF_Cell_Top;
FF_Cell*FF_Cell_Bot;
FF_Cell*FF_Cell_Crn;

FF_Orient*FF_Orient_Top;

long FF_Cell_Cnt;
long FF_Inst_Cnt;
long FF_Ornt_Cnt;
/**/
_sAchar*FF_Heap_1;
long FF_Heap_1_Lim;
long FF_Heap_1_Max;

_sAchar*FF_Heap_2;
long FF_Heap_2_Lim;
long FF_Heap_2_Max;
/**/
long Zrej_N;
_sAlong*Zrej_X;
_sAlong*Zrej_Y;
_sAlong*Zrej_P;
long*zzedge;
/**/
long read_witem(W_ITEM*, long, FILE*);
long write_witem(W_ITEM*, long);
long MultFlashes;
long SizeX5089;
extern long WMinX, WMinY, WMaxX, WMaxY;  // window for layout
long Block_89, NStrip;
void UPSet_Xg_Yg(void);
void SPLIT_POLYGON_S89(void);
void Dix_5089(void);
void Determine_split_method(void);
/**/
void
SPLIT_POLYGON_S89(void)
{
	Check_Closure();
	SPLIT_POLYGON(1);
}
long
write_witem(W_ITEM*z, long n)
{
	long i;
	for(i=0; i<n; ++i)
	{
		if(z[i].npoints<0)zAbort(sp5089,__LINE__);
		MaSCOT_IO.Dix_write(&z[i], (size_t)12+(z[i].npoints-2)*3);
	}
	return i;
}
long
read_witem(W_ITEM*z, long n, FILE*f)
{
	long i, j;
	unsigned char l;
	if(!f)return 0;
	for(i=j=0; i<n; i++)
	{
		fread(&l, 1, 1 ,f);
		z[i].npoints = l;
		l = 11+(l-2)*3;
		fread(&z[i].nblock, (size_t)l, 1, f);
		__error__(f, 0);
		j+=l+1;
	}
	return j;
}
#define gt(x,y) ((x).x0> (y).x0?1:0)
#define ge(x,y) ((x).x0>=(y).x0?1:0)
char
__mgt__5(long a, long b, char f)
{
	long*c_a = f==1? &a: (long*)incr_far(chnl, a);
	long*c_b = f==2? &b: (long*)incr_far(chnl, b);
	W_ITEM*b_a = (W_ITEM*)incr_far(SQ->Buf, (*c_a));
	W_ITEM*b_b = (W_ITEM*)incr_far(SQ->Buf, (*c_b));
	if(b_a->nblock>b_b->nblock)return 1;
	if(b_a->nblock<b_b->nblock)return 0;
	char z_a = b_a->rang_x>1;
	char z_b = b_b->rang_x>1;
	if(z_a>z_b)return 0;
	if(z_b>z_a)return 1;
	return gt((*b_a),(*b_b));
}
char __mGt__5(__MGObj__*a, __MGObj__*b)
{
	if(a->W_item.nblock>b->W_item.nblock)return 1;
	if(a->W_item.nblock<b->W_item.nblock)return 0;
	char z_a = a->W_item.rang_x>1;
	char z_b = b->W_item.rang_x>1;
	if(z_a>z_b)return 0;
	if(z_b>z_a)return 1;
	return gt(a->W_item,b->W_item);
}
char __mGe__5(__MGObj__*a, __MGObj__*b)
{
	if(a->W_item.nblock>b->W_item.nblock)return 1;
	if(a->W_item.nblock<b->W_item.nblock)return 0;
	char z_a = a->W_item.rang_x>1;
	char z_b = b->W_item.rang_x>1;
	if(z_a>z_b)return 0;
	if(z_b>z_a)return 1;
	return ge(a->W_item,b->W_item);
}
void
pospoly_5(__mgseq__*s)
{
	if(s->bfs<=0||s->eof)
	{
		s->eof = 1;
		memset(s->Obj, 0, sizeof(W_ITEM));
		return;
	}
	W_ITEM*Obj=(W_ITEM*)incr_far(s->buf, s->pos);
	long cnt=(long)12+(Obj->npoints-2)*3l;
	if(cnt>sizeof(W_ITEM)||cnt<0)
	{
		zAbort(sp5089,__LINE__);
	}
	set_next(s, (char*)s->Obj, cnt);
	s->eof = 0;
}
void
ZSMOVE_5(__mgseq__*s, long i)
{
	long p = *((long*)incr_far(chnl, i));
	W_ITEM*Obj = (W_ITEM*)incr_far(SQ->Buf, p);
	long cnt=(long)12+(Obj->npoints-2)*3l;
	if(cnt>sizeof(W_ITEM)||cnt<0)
	{
		zAbort(sp5089,__LINE__);
	}
	fwrite(Obj, 1, cnt, s->fil);
	if(i==SQ->cnt-1)
	{
		s->Obj->W_item = (*Obj);
	}
	++s->ele;
}
void
zsmove_5(__mgseq__*d, __mgseq__*s)
{
	(*d->Obj) = (*s->Obj);
	long cnt = 12+(long)(d->Obj->W_item.npoints-2)*3l;
	if(cnt>sizeof(W_ITEM)||cnt<0)
	{
		zAbort(sp5089,__LINE__);
	}
	put_next(d, (char*)d->Obj, cnt);
	d->ele+= 1;
	pospoly_5(s);
}
void
READRUN_5(void)
{
	READRUN_X(sizeof(W_ITEM));
	SQ->Obj->W_item = *((W_ITEM*)incr_far(SQ->Buf, chnl[0]));
}
#undef gt
#undef ge
long
readrunitem_5(long cnt)
{
	cnt = read_witem((W_ITEM*)incr_far(SQ->Buf, SQ->pos), 1, SQ->fil);
	return cnt;
}
/**/
void l_swap(double&A, double&B)
{
double T=A;A=B;B=T;
}
void*
FF_Halloc_0(long L,_sAchar*Heap,long&Lim)
{
	long z = 4096-(Lim%4096);
	if(z<L)
	{
		Lim += z;
	}
	long I = Lim;
	Lim += L;
	return (void*)(&((*Heap)[I]));
}
#define	FF_Halloc_1(L) (FF_Halloc_0((L),FF_Heap_1,FF_Heap_1_Lim))
#define	FF_Halloc_2(L) (FF_Halloc_0((L),FF_Heap_2,FF_Heap_2_Lim))
long
FI_CE_Hash(char*s)
{
	long i = 0;
	while(*s) i = (i + (*s++))&31;
	return(i);
}
FF_Cell*
FF_Find_CE(char*N)
{
	long i;
	FF_Cell*I;

	i = FI_CE_Hash(N);
	I = FF_CE_Hash[i];
	while(I)
	{
		if(stricmp(I->N, N)==0)return(I);
		I = I->X;
	}
	I = (FF_Cell*)FF_Halloc_1(FF_Cell_Len);
	FF_Cell_Cnt++;

	if(FF_Cell_Top==0)
	{
		FF_Cell_Top = I;
		FF_Cell_Bot = I;
	}
	else
	{
		FF_Cell_Bot->x = I;
		FF_Cell_Bot = I;
	}
	I->N = MaSCOT_IO.SS_Fetch(N);
	I->F = 0;
	I->R = 0;
	I->x = 0;
	I->X = FF_CE_Hash[i];
	FF_CE_Hash[i] = I;

	return I;
}
void
FF_Insert_Entr
(
	double DX, double DY, double SX, double SY,
	long KX, long KY, FF_RMatrix*RM, FF_Cell*C1, FF_Cell*C2
)
{
	FF_Entr*I;
	FF_AMatrix*J;

	for(I = C1->R; I; I = I->X)
	{
		if(I->I==C2)break;
	}
	if(I==0)
	{
		I = (FF_Entr*)FF_Halloc_1(FF_Entr_Len);

		I->M = 0L;
		I->I = C2;
		I->X = C1->R;
		C1->R = I;
	}
	J = (FF_AMatrix*)FF_Halloc_1(FF_AMatrix_Len);
	J->NX = I->M; I->M = J;
	J->DX = DX;
	J->DY = DY;
	J->SX = SX;
	J->SY = SY;
	J->KX = KX;
	J->KY = KY;
	J->RM = RM;
}
long
FI_RM_Hash(struct FF_RMatrix*RM)
{
	long I;
	double S;
	S = RM->A+RM->B+RM->D+RM->E;
	S*= 100;
	I = ((long)S)&31;
	return(I);
}
FF_RMatrix*
FF_Find_RM(FF_RMatrix*M)
{
	FF_RMatrix*I;
	long i;
	i = FI_RM_Hash(M);
	I = FF_RM_Hash[i];
	while(I)
	{
		if(!(M->A!=I->A))
		if(!(M->B!=I->B))
		if(!(M->D!=I->D))
		if(!(M->E!=I->E))
		return I;
		I = I->X;
	}
	I = (FF_RMatrix*)FF_Halloc_1(FF_RMatrix_Len);

	I[0] = M[0];

	I->X = FF_RM_Hash[i];
	FF_RM_Hash[i] = I;

	return(I);
}
void
FL_Rotate(long*x, long*y, FF_RMatrix*m)
{
	long t = *x;
	*x = floor(t * m->A + *y * m->B +0.5);
	*y = floor(t * m->D + *y * m->E +0.5);
}
void
FF_Rotate(double*x, double*y, FF_RMatrix*m)
{
	double t = *x;
	*x = t * m->A + *y * m->B;
	*y = t * m->D + *y * m->E;
}
FF_RMatrix
FF_Zmult(FF_RMatrix*m0, FF_RMatrix*m1)
{
	FF_RMatrix goal;
	goal.A = m1->A * m0->A + m1->B * m0->D;
	goal.B = m1->A * m0->B + m1->B * m0->E;
	goal.D = m1->D * m0->A + m1->E * m0->D;
	goal.E = m1->D * m0->B + m1->E * m0->E;
	return goal;
}
void
FF_Matrix_Transform(long Level, long Inst, FF_RArray*A, FF_RMatrix R)
{
	static FF_Entr*S;
	FF_AMatrix*M;
	static long T;
	static FF_Orient*I;
	static FF_RMatrix*J;
	static FF_RChain*K;
	static FF_RArray*Z;

	static FF_RMatrix X;
	S = (FF_Entr*)FF_Stack[Level];

	for(M = S->M; M; M = M->NX)
	{
		Z = (FF_RArray*)FF_Halloc_2(FF_RArray_Len);
		Z->NX = A;
		Z->sx = 0;
		Z->sy = 0;
		Z->DX = M->DX;
		Z->DY = M->DY;
		Z->SX = M->SX;
		Z->SY = M->SY;
		Z->KX = M->KX;
		Z->KY = M->KY;
		T = Inst*Z->KX*Z->KY;
		if(A!=0)
		{
			FF_Rotate(&Z->SX, &Z->sy, &R);
			FF_Rotate(&Z->sx, &Z->SY, &R);
			FF_Rotate(&Z->DX, &Z->DY, &R);
			X = FF_Zmult(M->RM, &R);
		}
		else
			X=*(M->RM);
		if(Level)
		{
			FF_Matrix_Transform(Level-1, T, Z, X);
			continue;
		}
		FF_Inst_Cnt+= T;
		J = FF_Find_RM(&X);

		I = FF_Orient_Top;
		while(I)
		{
			if(I->RM==J)break;
			I = I->NX;
		}
		if(I==0)
		{
			I = (FF_Orient*)FF_Halloc_2(FF_ORient_Len);
			I->NX = FF_Orient_Top;
			FF_Orient_Top = I;
			I->RM = J;
			I->ST = 0;
			++FF_Ornt_Cnt;
		}
		K = (FF_RChain*)FF_Halloc_2(FF_RChain_Len);
		K->RA = Z;
		K->NX = 0;
		K->NX = I->ST;
		I->ST = K;
		/*End Of Current Matrix*/
	}
}
void
FF_Explode_Cell(long L, FF_Cell*C)
{
	FF_Entr*R = C->R;
	while(R)
	{
		if(L>=FF_Stack_Max)
		{
			zAbort(sp5089,__LINE__);
		}
		FF_Stack[L] = (void*)R;

		if(R->I)
		{
			FF_Explode_Cell(L+1, R->I);
		}
		else
		{
			FF_Matrix_Transform(L, 1, 0, RM0000);
		}
		R = R->X;
	}
}
void FF_SOU_CONVERT(FF_Cell*C);
void
FF_Convert_Cell(FF_Cell*Cell)
{
	FF_SOU_CONVERT(Cell);
}
void
FF_EXPLODE_CELLS(void)
{
	FF_Cell*I = FF_Cell_Top;
	for(;I;I = I->x)
	{
		FF_Heap_2_Lim = 1;
		FF_Orient_Top = 0;
		FF_Ornt_Cnt = 0;
		FF_Inst_Cnt = 0;
fprintf(stderr, "\nCELL-EXPLODE ON %s STARTED\n", I->N);
		FF_Explode_Cell(0, I);
fprintf(stderr, "TOTAL ORIENTATIONS = %ld, INSTANCES = %ld\n", FF_Ornt_Cnt, FF_Inst_Cnt);
		FF_Convert_Cell(I);
	}
}
long
Sou_Swap(short b[], long*i)
{
	union
	{
		long L;
		short I[2];
	}	U;
	U.I[0] = b[*i+1], U.I[1] = b[*i];
	return(*i += 2, U.L);
}
long
pp_next(short b[], long*t, long*m)
{
	*t = b[*m + 1] & 03;
	return(Sou_Swap(b, m)>>3);
}
FF_RMatrix*
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
#define YY_VALID_SOU_LAYER(L) (Inp_Sou_Layer==(L))
void
Sou_PickIter(short B[])
{
	FF_Cell*I;
	long k;
	FF_RMatrix*M;

	long DX;
	long DY;
	long SX;
	long SY;
	long KX;
	long KY;

	char C[7];
	char*c = (char*)B + 2;

	memcpy(C, c, 6);
	C[6] = 0;

	k = 5;
	while(C[k]<33)C[k--] = 0;

	I = FF_Find_CE(C);

	M = FF_RM_Sou(B[4]&017);

	k = 5;
	DY = Sou_Swap(B, &k);
	DX = Sou_Swap(B, &k);

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
	FF_Cell*I;
	long k;
	FF_RMatrix*M;

	long X;
	long Y;

	char C[7];
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
		Y = Sou_Swap(B, &k);
		X = Sou_Swap(B, &k);

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
	if(XMIN>XMAX)l_swap(XMIN, XMAX);
	if(YMIN>YMAX)l_swap(YMIN, YMAX);

	G_buf0[0] = G_buf0[1] = XMIN;
	G_buf0[2] = G_buf0[3] = XMAX;
	G_buf1[0] = G_buf1[3] = YMIN;
	G_buf1[1] = G_buf1[2] = YMAX;

	G_buf0[4] = G_buf0[0];
	G_buf1[4] = G_buf1[0];

	G_blim = 5;
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

	G_blim = 0;

	while(k<c)
	{
		y = Sou_Swap(B, &k);
		x = Sou_Swap(B, &k);
		p = G_blimN();
		G_buf0[p] = x;
		G_buf1[p] = y;
	}
	Terminate_Poly_3();
}
#define rx0 G_buf0[0]
#define ry0 G_buf1[0]
#define rx1 G_buf0[1]
#define ry1 G_buf1[1]
#define rx2 G_buf0[2]
#define ry2 G_buf1[2]
#define rx3 G_buf0[3]
#define ry3 G_buf1[3]
#define rx4 G_buf0[4]
#define ry4 G_buf1[4]

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
		y = Sou_Swap(b, &k)>>3;
		x = Sou_Swap(b, &k)>>3;
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
		G_blim = 5;
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
			y = Sou_Swap(b, &k) >>3;
			x = Sou_Swap(b, &k) >>3;
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
		y = Sou_Swap(B, &k);
		x = Sou_Swap(B, &k);
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

	G_blim = 0;

	while(i++<4)
	{
		y = Sou_Swap(B, &k);
		x = Sou_Swap(B, &k);

		p = G_blimN();
		G_buf0[p] = x;
		G_buf1[p] = y;
	}
	p = G_blimN();
	G_buf0[p] = G_buf0[0];
	G_buf1[p] = G_buf1[0];
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

	G_blim = 0;

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
		if(err)continue;
		p = G_blimN();
		G_buf0[p] = x;
		G_buf1[p] = y;
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
			switch(S_P[0]&017400)
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
			S_P[0] = 0377;
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

	if(File==NULL)zAbort(sp5089,__LINE__);

	while(fread(S_B, 1, 01000, File)==01000)
	{
		S_P = S_B;
		while((S_P-S_B)<0377)
		{
			switch(S_P[0]&017400)
			{
			case 001000:
			Sou_PickCell(S_P);
			break;
			case 005000:
			Sou_PickIter(S_P);
			break;
			case 002000:
			S_P[0] = 0377;
			}
			S_P += *(S_P++)&0377;
		}
	}
}
void
yycscan_sou(char*I_Name)
{
	char Name[33];
	FILE*File;
	char*Cell;
	char*sou_suf=".sou";
	FF_Cell*Cnum = FF_Find_CE(I_Name);
	FF_Insert_Entr(0, 0, 0, 0, 1, 1, FF_RM0000, Cnum, 0);
	strncpy(Name, I_Name, 33);
	strncat(Name, sou_suf, 5);
	Cnum->F = MaSCOT_IO.SS_Fetch(Name);
	File = __open__(Cnum->F,"rb");
	FF_Cell_Crn = 0;
	do
	{
		fprintf(stderr, "\nCELL-SCAN STARTED ON FILE %s", Name);
		FF_SOU_CELL_SCAN(File);
		__error__(File, Name);
		__close__(File, Name);
		File = 0;
		Cnum = Cnum->x;
		while(Cnum)
		{
			Cell = Cnum->N;
			FF_Cell_Crn = Cnum;
			strncpy(Name, Cell, 9);
			strncat(Name, sou_suf, 5);
			File = fopen(Name, "rb");
			if(File)
			{
				Cnum->F = MaSCOT_IO.SS_Fetch(Name);
				break;
			}
			Cnum->F = 0;
			fprintf(stderr, "\nFILE %s OPEN ERROR", Name);
			Cnum = Cnum->x;
		}
	}
	while(File);
}
char
check5089_poly(void)
{
	long ymin, ymax, xmin, xmax, xn, xc;
	long i, j, k;
	long nv, ext, rc, rn;
	k=ZLDCO1.fetch_max_min(ZLDLIM, ymax, ymin, 1/*return min-value index*/);
	k=ZLDCO0.fetch_max_min(ZLDLIM, xmax, xmin, 1/*return min-value index*/);
	char z=0;
	z|=xmin<W_CXMIN;
	z|=xmax>W_CXMAX;
	z|=ymin<W_CYMIN;
	z|=ymax>W_CYMAX;
	if(z)
	{
		return -1;
	}
	Block_89 = (ymax-1-W_CYMIN)/STRIP_SIZE;
	if(ZLDLIM>15)
	{
		return 2;
	}
	if(xmax-xmin>MAX_POLY_LENGTH)
	{
		return 3;   // bad length
	}
	rc=1; xc=xmin; j=k; ext=0; nv=0;
	for(i=0; i<ZLDLIM; ++i)
	{
		if(++j==ZLDLIM)j=0;
		xn=ZLDCO0[j];
		if(xc>xn)rn=+1;
		else
		if(xc<xn)rn=-1;
		else rn=rc;
		if(rn!=rc)ext++;
		if(xc!=xn)nv++;
		rc=rn;
		xc=xn;
	}
	if(nv>EDGE_LIM)return 2;// bad edges quantity
	if(ext>2)return 1;// non convex poly
	return 0;// good poly
}
long
next_point(long nv, char strip)
{
	char plus=(strip? (STATUS<0? 1:0) : (STATUS>0? 1:0));
	if(plus)
	{
		if(++nv>=ZLDLIM)nv=1;
	}
	else
	{
		if(--nv<0)nv=ZLDLIM-2;
	}
	return nv;
}
void
v_a_move(_sAlong*Src, _sAlong*Dst, long s, long d, long Lim)
{
	long i=0;
	while(i++<Lim)
	{
		(*Dst)[d++]=(*Src)[s++];
	}
}
void
Dix_5089(void)
{
	long lowY_strip;
	long i, j, dir, nv;
	char not_even;
	long cx, bx, by, cy, margin;
	W_ITEM wfile_item;

	if(ZLDLIM<4)
	{
		return;
	}
	if((i=check5089_poly())<0)
	{
		return;   // out of window
	}
	if(i)             // non 5089 poly
	{
		long k=(*Zrej_P)[Zrej_N];
		(*Zrej_P)[++Zrej_N]=k+ZLDLIM;
		v_a_move(&ZLDCO0, Zrej_X, 0, k, ZLDLIM);
		v_a_move(&ZLDCO1, Zrej_Y, 0, k, ZLDLIM);
		return;
	}
	wfile_item.npoints = (char)ZLDLIM;
	wfile_item.nblock = (short)Block_89;
	lowY_strip = W_CYMIN + (long)Block_89*STRIP_SIZE;
	MultFlashes++;
	not_even = wfile_item.nblock%2;
	if(not_even)
	{
		dir=-1;
		margin=W_CXMAX;  // ... 1,3,5
		if(Current_Rangx>1)
		ZLDCO0.incr(0, ZLDLIM-1, (long)Current_Stepx*(Current_Rangx-1));
	}
	else
	{
		dir=1;
		margin=W_CXMIN;   // ... 0,2,4
	}
	nv=0;
	bx=ZLDCO0[0];
	by=ZLDCO1[0];
	for(i=1; i<ZLDLIM; ++i)    // find min x and min y
	{
		cx=ZLDCO0[i];
		cy=ZLDCO1[i];
		if(not_even)
		{
			if(bx<cx) bx=cx, by=cy, nv=i; // find max
			else
			if((bx==cx)&&(by>cy)) by=cy, nv=i; // ... 1,3,5
		}
		else
		{
			if(bx>cx) bx=cx, by=cy, nv=i; // find min
			else
			if((bx==cx)&&(by>cy)) by=cy, nv=i; // ... 0,2,4
		}
	}
	cx = ZLDCO0[nv]; // origin point for poly
	wfile_item.x0 = dir*(cx-margin);
	wfile_item.y0 = (unsigned char)(ZLDCO1[nv]-lowY_strip);
	nv = next_point(nv, not_even);
	for(j=0,i=1; i<ZLDLIM-1; ++i,++j)
	{
		wfile_item.coord[j].x = (short)(dir*(ZLDCO0[nv]-cx));
		wfile_item.coord[j].y = (unsigned char)(ZLDCO1[nv]-lowY_strip);
		nv = next_point(nv, not_even);
	}
	wfile_item.rang_x=Current_Rangx;
	wfile_item.step_x=Current_Stepx;
	write_witem(&wfile_item, 1);
}
#define zswap(A,B,C) ((C)=zzedge[(A)],zzedge[(A)]=zzedge[(B)],zzedge[(B)]=(C))
void
sortzzedge(long*zzedge, long sl, long sr)
{
	long k, j;
	long w, x;
	if((sr-sl)<17) /*SHACKERSORT*/
	{
		while(sl<sr)
		{
			for(j=sr, k=sr; sl<j; --j)
			if(zzedge[j-1] > zzedge[j])
			{
				x = zzedge[j];
				do zzedge[j] = zzedge[j-1], --j;
				while((sl<j) && (zzedge[j-1] > x));
				zzedge[j] = x, k = j;
			}
			sl = k+1;
			for(j=sl; j<sr; ++j)
			if(zzedge[j] > zzedge[j+1])
			{
				x = zzedge[j] ;
				do zzedge[j] = zzedge[j+1], ++j;
				while((j<sr) && (x > zzedge[j+1]));
				zzedge[j] = x, k = j;
			}
			sr = k-1;
		}
	}
	else
	{
		x = (sl + sr)>>1;
		if(zzedge[sl] > zzedge[x]) zswap(sl, x, w);
		if(zzedge[x] > zzedge[sr]) zswap(sr, x, w);
		if(zzedge[sl] > zzedge[x]) zswap(sl, x, w);
		x = zzedge[x];
		for(k=sl+1, j=sr-1; k<j;)
		{
			while(x > zzedge[k]) ++k;
			while(zzedge[j] > x) --j;
			if(k<j) zswap(k, j, w ), ++k, --j;
		}
		if(sl<j)sortzzedge(zzedge, sl, j);
		if(k<sr)sortzzedge(zzedge, k, sr);
	}
}
#undef zswap
void
Determine_split_method(void)
{
	long xmin, xmax, xn, xc, yc, yl;
	long ymin, ymax, yyy=LONG_MAX;
	long i, j, k, nv, ext, rc, rn;
	k=ZLDCO1.fetch_max_min(ZLDLIM, ymax, ymin, 1);
	k=ZLDCO0.fetch_max_min(ZLDLIM, xmax, xmin, 1);
	CXMIN = xmin;
	CYMIN = ymin;
	CXMAX = xmax;
	CYMAX = ymax;
	GvSplit = GvSplit_2;
	ZXgCO0[0] = xmin;
	ZYgCO0[0] = ymin;
	if(xmax-xmin>MAX_POLY_LENGTH)
	{
	//bad length
		XgCNT = 2;
		YgCNT = 1;
		ZXgCO0[1] = xmin+MAX_POLY_LENGTH;
		ZXgCO0[2] = xmax;
		ZYgCO0[1] = ymax;
		return;
	}
	rc=+1; xc=xmin; j=k; ext=0; nv=0;
//	xc=xmin; rc=-1; j=k; ext=0; nv=0;
	yl=ymin+((ymax-ymin)>>1);
	for(i=0; i<ZLDLIM; ++i)
	{
		if(++j==ZLDLIM)j = 0;
		xn = ZLDCO0[j];
		if(xc>xn)rn=+1;
		else
		if(xc<xn)rn=-1;
		else rn=rc;
		if(rn!=rc)
		{
			ext++;
			yc = ZLDCO1[j];
			char z=(yc!=ymin)&&(yc!=ymax);
			if(z)yl=yc;
			if((xn!=xmin)&&(xn!=xmax)&&z)yyy=yc;
		}
		if(xc!=xn)
		{
			if(nv>=512)break;
			zzedge[nv++] = xn;
		}
		rc = rn; xc = xn;
	}
	if(ext>2)
	{
	// non convex poly
		XgCNT = 1;
		YgCNT = 2;
		ZXgCO0[1] = xmax;
		ZYgCO0[1] = yyy!=LONG_MAX? yyy: yl;
		ZYgCO0[2] = ymax;
		return;
	}
	if(nv>EDGE_LIM||ZLDLIM>15)
	{
	// bad edges quantity
		if(nv<3)zAbort(sp5089,__LINE__);
		sortzzedge(zzedge, 0, nv-1);
		XgCNT = 2;
		YgCNT = 1;
		if(nv>EDGE_LIM)
			ZXgCO0[1] = zzedge[EDGE_LIM-1];
		else
			ZXgCO0[1] = zzedge[(nv>>1)];
		ZXgCO0[2] = xmax;
		ZYgCO0[1] = ymax;
		return;
	}
	zAbort(sp5089,__LINE__);
}
void
fetch_max_min(long*G_buf, long G_lim, long&XM, long&xm)
{
	long i=0;

	XM=xm=G_buf[0];
	for(i=1; i<G_lim; ++i)
	{
		long x=G_buf[i];
		if(x>XM)XM=x;
		else
		if(x<xm)xm=x;
	}
}
#define xmin PXMIN
#define ymin PYMIN
#define xmax PXMAX
#define ymax PYMAX
void
SplitPolyFromSrc(void) //Input from ZLD...
{
	//It assumes PXMIN, PXMAX, PYMIN, PYMAX, STATUS to be setted up.
	long sy, zrej_n;
	long reg_x_min, reg_y_min, reg_x_max, reg_y_max;
	long ngx, ngy;
	char regulargrid;
	long i, j;
	long status=STATUS;
	i=0;
	i|=xmax<=W_CXMIN;
	i|=xmin>=W_CXMAX;
	i|=ymax<=W_CYMIN;
	i|=ymin>=W_CYMAX;
	if(i)
	{
		return;
	}
	reg_y_min = max(ymin, W_CYMIN);
	reg_y_max = min(ymax, W_CYMAX);
	reg_x_min = max(xmin, W_CXMIN);
	reg_x_max = min(xmax, W_CXMAX);
	ngx=ngy=0;
	regulargrid=1;
	if(xmin<reg_x_min)
	{
		ZXgCO0[ngx++]=xmin;
		regulargrid=0;
	}
	ZXgCO0[ngx++]=reg_x_min;
	ZXgCO0[ngx++]=reg_x_max;
	if(xmax>reg_x_max)
	{
		ZXgCO0[ngx++]=xmax;
		regulargrid=0;
	}
	if(ymin<reg_y_min)
	{
		ZYgCO0[ngy++]=ymin; sy = W_CYMIN;
		regulargrid=0;
	}
	else	sy = ((ymin-W_CYMIN)/STRIP_SIZE)*STRIP_SIZE + W_CYMIN;
	ZYgCO0[ngy++]=sy;
	while(ZYgCO0[ngy-1]<reg_y_max)
	{
		ZYgCO0[ngy] = ZYgCO0[ngy-1]+STRIP_SIZE;
		ngy++;
	}
	if(ymax>reg_y_max)
	{
		ZYgCO0[ngy++]=ymax;
		regulargrid=0;
	}
	GvSplit = regulargrid? GvSplit_1: GvSplit_2;
	CXMIN = ZXgCO0[0];
	CXMAX = ZXgCO0[ngx-1];
	CYMIN = ZYgCO0[0];
	CYMAX = ZYgCO0[ngy-1];
	XgCNT = ngx-1;
	YgCNT = ngy-1;
	YgDIV = STRIP_SIZE;
	XgDIV = CXMAX-CXMIN;
	/*init loop for rejected*/
	zrej_n=Zrej_N=0;
	(*Zrej_P)[0]=0;
	SPLIT_POLYGON_S89();
	while(zrej_n<Zrej_N)
	{
		i=(*Zrej_P)[zrej_n];
		j=(*Zrej_P)[++zrej_n];
		if(i+2>=j)
		{
			zAbort(sp5089,__LINE__);
		}
		ZLDLIM=j-i;
		v_a_move(Zrej_X, &ZLDCO0, i, 0, ZLDLIM);
		v_a_move(Zrej_Y, &ZLDCO1, i, 0, ZLDLIM);
		Determine_split_method();
		STATUS=1;
		SPLIT_POLYGON_S89();
	}
	STATUS=status;
}
#undef xmin
#undef xmax
#undef ymin
#undef ymax
#define DX K->DX
#define DY K->DY
#define SX K->SX
#define SY K->SY
#define sx K->sx
#define sy K->sy
#define KX K->KX
#define KY K->KY
void
FF_Explode_Item(double X, double Y, long C)
{
	long I;
	long J;
	FF_RArray*K;
	K = (FF_RArray*)FF_Stack[C];
	X += DX;
	Y += DY;
	if(C==0)
	{
		double sX=SX;
		double sY=SY;
		double dX=sx;
		double dY=sy;
		long kX=KX;
		long kY=KY;
		long XM;
		long xm;
		long YM;
		long ym;
		long tm;
		long xI;
		long xJ;
		double local_dx;
		double local_dy;
		fetch_max_min(G_buf0, G_blim, XM, xm);
		fetch_max_min(G_buf1, G_blim, YM, ym);
		ZZ_DELTA_Y = Y;
		ZZ_DELTA_X = X;
		if(kX<=1)
		{
			kX=1; sX=dY=0;
		}
		if(kY<=1)
		{
			kY=1; sY=dX=0;
		}
		xI=kX-1; xJ=kX-1;
		if(kX==1&&kY==1)
		{
			goto processing_non_regular_part;
		}
		else
		if(_EQ_(sX, 0, 1e-7)&&_EQ_(sY, 0, 1e-7))
		{
			l_swap(kX, kY);
			l_swap(sX, dX);
			l_swap(sY, dY);
		}
		else
		if(!_EQ_(dX, 0, 1e-7)||!_EQ_(dY, 0, 1e-7))
		{
			goto processing_non_regular_part;
		}
		if(kX==1)
		{
			goto processing_non_regular_part;
		}
		if(sX<0)
		{
			ZZ_DELTA_X += sX*(kX-1);
			sX *= -1;
		}
		PXMIN=xm+ZZ_DELTA_X;
		if(PXMIN>=W_CXMAX)
		{
			return;
		}
		tm=W_CXMIN-PXMIN;
		if(tm>0)
		{
			xI=((tm+1)/sX)+1;
			if(xI>kX)xI=kX;
		}
		else	xI=0;
		if(xI)PXMIN+=sX*xI;
		PXMAX=PXMIN+(XM-xm);
		if(PXMAX<=W_CXMIN)
		{
			return;
		}
		tm=W_CXMAX-PXMAX;
		xJ=xI+1;
		if(tm>0)
		{
			xJ+=tm/sX;
			if(xJ>kX)xJ=kX;
		}
		local_dx=ZZ_DELTA_X+sX*xI;
		local_dy=ZZ_DELTA_Y;
		PYMIN=ym+ZZ_DELTA_Y;
		PYMAX=YM+ZZ_DELTA_Y;
		Current_Stepx=sX;
		Current_Rangx=xJ-xI;
		if(Current_Rangx<2)
		{
			goto processing_non_regular_part;
		}
		for(J=0; J<kY; ++J)
		{
			ZLDCO0.fmemcpy((char*)G_buf0, G_blim<<2);
			ZLDCO1.fmemcpy((char*)G_buf1, G_blim<<2);

			ZLDLIM = G_blim;
			ZLDCO0.incr(0, G_blim-1, local_dx);
			ZLDCO1.incr(0, G_blim-1, local_dy);
			SplitPolyFromSrc();
			PYMIN+=sY;
			PYMAX+=sY;
			local_dy+=sY;
		}
		processing_non_regular_part:
		Current_Stepx=0;
		Current_Rangx=1;
		for(I=0; I<kX; ++I)
		{
			if(I>=xI&&I<xJ)
			{
				I=xJ;
				if(I>=kX)break;
			}
			local_dx=ZZ_DELTA_X+sX*I;
			local_dy=ZZ_DELTA_Y+dY*I;
			PXMIN=xm+local_dx;
			PXMAX=XM+local_dx;
			PYMIN=ym+local_dy;
			PYMAX=YM+local_dy;
			for(J=0; J<kY; ++J)
			{
				if(PXMIN>=W_CXMAX||PXMAX<=W_CXMIN||
				   PYMIN>=W_CYMAX||PYMAX<=W_CYMIN)
				{
					goto loop_next_step;
				}
				ZLDCO0.fmemcpy((char*)G_buf0, G_blim<<2);
				ZLDCO1.fmemcpy((char*)G_buf1, G_blim<<2);

				ZLDLIM = G_blim;
				ZLDCO0.incr(0, G_blim-1, local_dx);
				ZLDCO1.incr(0, G_blim-1, local_dy);
				SplitPolyFromSrc();
				loop_next_step:
				PYMIN+=sY;
				PYMAX+=sY;
				PXMIN+=dX;
				PXMAX+=dX;
				local_dy+=sY;
				local_dx+=dX;
			}
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
	long*Zldco0 = G_buf0;
	long*Zldco1 = G_buf1;
	FF_Orient*I;
	FF_RChain*J;
	FF_RArray*K;
	long S;
	long status=STATUS;

	I = FF_Orient_Top;
	if(I==0)return;

	while(I)
	{
		if(FF_Ornt_Cnt>1)
		{
			memcpy(G_buf2, G_buf0, G_blim*4);
			memcpy(G_buf3, G_buf1, G_blim*4);
			G_buf0 = G_buf2;
			G_buf1 = G_buf3;
		}
		if(I->RM!=FF_RM0000)
		{
			long j;
			for(j = 0; j<G_blim; ++j)
			FL_Rotate
			(
				&G_buf0[j],
				&G_buf1[j],
				I->RM
			);
			DETERMINE_STAT_3(G_buf0, G_buf1, G_blim);
		}
		else
		{
			if(!status)
			DETERMINE_STAT_3(G_buf0, G_buf1, G_blim);
			status=STATUS;
		}
		if(!STATUS)
		{
fprintf(stderr, "\nERROR!!!POLYGON DISCARDED>IMPOSSIBLE TO TRANSFORM");
			G_buf0 = Zldco0;
			G_buf1 = Zldco1;
			return;
		}
		for(J = I->ST; J; J = J->NX)
		{
			K = J->RA;
			if(!K)
			{
				continue;
			}
			S = -1;
			while(K)
			{
				FF_Stack[++S] = (void*)K;
				K = K->NX;
			}
			FF_Explode_Item(YY_Xrelocate, YY_Yrelocate, S);
		}
		G_buf0 = Zldco0;
		G_buf1 = Zldco1;
		I = I->NX;
	}
}
void
Terminate_Poly_3(void)
{
	STATUS=0; //unknown.
	FF_Explode_Chain();
}
void
Init_5089_values(void)
{
	long m1, m2, m3;
	IEvaluater.__SetVT0LB0__();
	MaSCOT_IO.Dix_special = Dix_5089;
	W_CXMAX = (WMaxX+YY_Xrelocate)*UN001;
	W_CXMIN = (WMinX+YY_Xrelocate)*UN001;
	SizeX5089 = (W_CXMAX-W_CXMIN)>>9;
	if((W_CXMAX-W_CXMIN)%512)
	{
		SizeX5089++;
	}
	W_CXMAX = W_CXMIN+(SizeX5089<<9);
	W_CYMIN = (WMinY+YY_Yrelocate)*UN001;
	W_CYMAX = (WMaxY+YY_Yrelocate)*UN001;
	if((W_CYMAX<=W_CYMIN)||(W_CXMAX<=W_CXMIN))
	{
		zAbort(sp5089,__LINE__);
	}
	NStrip = (W_CYMAX-W_CYMIN)/STRIP_SIZE;
	if(NStrip*STRIP_SIZE < W_CYMAX-W_CYMIN)
	{
		NStrip++;
	}
	W_CYMAX = W_CYMIN + NStrip*STRIP_SIZE;
	MultFlashes = 0;
	XgCNT = 3;
	YgCNT = NStrip+2;
	ZXgCO0 = new long[XgCNT+9];
	ZYgCO0 = new long[YgCNT+1];
	ZXgCO1 = ZXgCO0;
	ZYgCO1 = ZYgCO0;
	m1 = XgCNT;
	m2 = YgCNT;
	m3 = m1*m2;
	ZGaTBl = new GvBead*[m3];
	memset(ZXgCO0, 0, m1*sizeof(ZXgCO0[0]));
	memset(ZYgCO0, 0, m2*sizeof(ZYgCO0[0]));
	memset(ZGaTBl, 0, m3*sizeof(ZGaTBl[0]));
	memcpy(O_Layer.Name, "DATA5089", 20);
	MaSCOT_IO.INIT_OLayer();
}
extern char*OvrlpMstPrf;
void
M_5089_MAIN_X(char I_name[20], char Win_Set)
{
	_sAlong Heap_3;
	_sAlong Heap_4;
	_sAlong Heap_5;
	Zrej_X = &Heap_3;
	Zrej_Y = &Heap_4;
	Zrej_P = &Heap_5;
	MaSCOT_IO.Setup();
	memcpy(I_Layer[0].Name, I_name, 20);
	MaSCOT_IO.Find_ILayer();
	MaSCOT_IO.INIT_ILayer(0);
	if(!(I_Layer[0].LX_S.STATUS&1))
	{
		fputs(OvrlpMstPrf, stderr);
		zAbort(sp5089,__LINE__);
	}
	if(!Win_Set)
	{
		WMinX = CXMIN/UN001;
		WMaxX = CXMAX/UN001;
		WMinY = CYMIN/UN001;
		WMaxY = CYMAX/UN001;
	}
	STATUS=1;//After OverlapElimination all polygons are clock wise.
	CFragment = 0;
	XFragment =-1;
	MaSCOT_IO.D_Link(0);
	MaSCOT_IO.SetAbsDelta(0);
	Init_5089_values();
	long cxmin=CXMIN;
	long cxmax=CXMAX;
	long cymin=CYMIN;
	long cymax=CYMAX;
	zzedge = new long[512];
	SetUp_Min_Max=1;
	Current_Stepx=0;
	Current_Rangx=1;
	while(CFragment!=LONG_MAX)
	{
		MaSCOT_IO.ZDREAD_P();
		if(ZLDLIM==0)
		{
			MaSCOT_IO.SetAbsDelta(0);
			continue;
		}
		Check_Closure();
		SplitPolyFromSrc();
		CXMIN=cxmin;
		CXMAX=cxmax;
		CYMIN=cymin;
		CYMAX=cymax;
	}
	delete zzedge;
	UPSet_Xg_Yg();
	MaSCOT_IO.Dix_special = 0;
	MaSCOT_IO.Dix_close();
	MaSCOT_IO.Close_ILayer(0);
	IEvaluater._ReleaseStorage();
}
void
M_5089_MAIN_1(char I_name[20], char Win_Set, char*Tmp_Str)
{
	_sAchar Heap_1;
	_sAchar Heap_2;
	_sAlong Heap_3;
	_sAlong Heap_4;
	_sAlong Heap_5;
	Zrej_X = &Heap_3;
	Zrej_Y = &Heap_4;
	Zrej_P = &Heap_5;
	FF_Heap_1 = &Heap_1;
	FF_Heap_2 = &Heap_2;
	FF_Stack = new void*[100];
	memset(FF_Stack, 0, sizeof(FF_Stack[0])*100);
	FF_Stack_Lim = 0;
	FF_Stack_Max = 100;
	FF_CE_Hash = new FF_Cell*[32];
	memset(FF_CE_Hash, 0, sizeof(FF_CE_Hash[0])*32);
	FF_RM_Hash = new FF_RMatrix*[32];
	memset(FF_RM_Hash, 0, sizeof(FF_RM_Hash[0])*32);
	FF_Cell_Top = 0;
	FF_Cell_Bot = 0;
	FF_Cell_Crn = 0;
	FF_Orient_Top = 0;
	FF_Cell_Cnt = 0;
	FF_Inst_Cnt = 0;
	FF_Ornt_Cnt = 0;
	G_blim = 0l;
	G_bmax = 64;
	zzedge = new long[512];
	G_buf0_long = new long[64];
	G_buf1_long = new long[64];
	G_buf2_long = new long[64];
	G_buf3_long = new long[64];
	FF_RM0000 = FF_Find_RM(&RM0000);
	FF_RM0001 = FF_Find_RM(&RM0001);
	FF_RM0002 = FF_Find_RM(&RM0002);
	FF_RM0003 = FF_Find_RM(&RM0003);
	FF_RM0010 = FF_Find_RM(&RM0010);
	FF_RM0011 = FF_Find_RM(&RM0011);
	FF_RM0012 = FF_Find_RM(&RM0012);
	FF_RM0013 = FF_Find_RM(&RM0013);
	memcpy(I_Layer[0].Name, I_name, 20);
	if(sscanf(I_name, "%d%s", &Inp_Sou_Layer, Tmp_Str)!=1)
	{
		fprintf(stderr, "\n!!! Bad layer name <%s> !!!\n", I_name);
		zAbort(sp5089,__LINE__);
	}
	yycscan_sou(I_Layer[0].PdbN);
	if(!Win_Set)
	{
		WMinX = 0;
		WMaxX = 100000l;
		WMinY = 0;
		WMaxY = 100000l;
	}
	Init_5089_values();
	FF_EXPLODE_CELLS();
//freing heap for sorting.
	UPSet_Xg_Yg();
	MaSCOT_IO.Dix_special = 0;
	MaSCOT_IO.Dix_close();
	IEvaluater._ReleaseStorage();
	delete zzedge;
	delete G_buf0_long;
	delete G_buf1_long;
	delete G_buf2_long;
	delete G_buf3_long;
	delete FF_Stack;
	delete FF_CE_Hash;
	delete FF_RM_Hash;
}
void
M_5089_MAIN_2(void)
{
	__mGt__	= __mGt__5;
	__mGe__	= __mGe__5;
	__mgt__	= __mgt__5;
	zsmove	= zsmove_5;
	pospoly	= pospoly_5;
	READRUN	= READRUN_5;
	ZSMOVE	= ZSMOVE_5;
	readrunitem=readrunitem_5;
	memcpy(O_Layer.PdbN,mg_sort(O_Layer.PdbN), 15);
	memcpy(I_Layer[0].PdbN, O_Layer.PdbN, 20);
}
void M_5089_MAIN_3(void);
void
M_5089_MAIN(char I_name[20], char Win_Set, char*Tmp_Str)
{
	UN001=1;
	if(!I_Layer[0].PdbN[0])
	{
	//Mascot input
		M_5089_MAIN_X(I_name, Win_Set);
	}
	else
	{
	//Sou-file input
		M_5089_MAIN_1(I_name, Win_Set, Tmp_Str);
	}
	M_5089_MAIN_2();
	M_5089_MAIN_3();
	exit(0);
}
#undef G_blim
#undef G_bmax
#undef G_buf0
#undef G_buf1
#undef G_buf2
#undef G_buf3
#define MAX_BL_LENGTH 32700   // 2**15  = 32768
#define MAX_DISTANCE  16383   // 2**14-1
#define buf_put_char(c) (buffer[buf_length++]=(c))
#define buf_put_int(i) (put_int(&buffer[buf_length++],(i)),++buf_length)
unsigned char ks_c;
unsigned char*Buffer5089;
unsigned long Buffer5089L;
long lastX;
#define __dat__ (I_Layer[0].File.Fil)
#define __lgl__ (O_Layer.File.Fil)
#define __nam__ (O_Layer.PdbN)
#define coordX G_buf0_long
#define coordY G_buf0_char
#define LengthPoly G_buf_lim_long
#define buffer Buffer5089
#define buf_length Buffer5089L
void
check_sum(unsigned char*c, long l)
{
	unsigned long k, i;
	for(i=0; i<l; ++i)
	{
		k = ks_c; k += c[i];
		ks_c = k%256;
		if(k>>8)ks_c++;
	}
}
void
put_int(unsigned char*p, short i)
{
	union { short i; unsigned char c[2]; } iii;
	iii.i = i;
	*p=iii.c[1]; p++;
	*p=iii.c[0];
}
#define put_int_2(p, i) put_int((p),(i))
void
put_int_3(unsigned char *p, long l)
{
	union { long l; unsigned char c[4]; } iii;
	iii.l = l;
	*p=iii.c[2]; p++;
	*p=iii.c[1]; p++;
	*p=iii.c[0];
}
void
put_long(unsigned char *p, long l)
{
	union { long l; unsigned char c[4]; } iii;
	iii.l = l;
	*p=iii.c[3]; p++;
	*p=iii.c[2]; p++;
	*p=iii.c[1]; p++;
	*p=iii.c[0];
}
void
put_area_descr(void)
{
	long i;
	char b[7];
	ks_c = 0;
	buffer[0]=0x80;
	buffer[1]=29;  // block length
	memset(&buffer[2], ' ', 6);
	memcpy(b, I_Layer[0].Name, 6);
	for(i=0; i<6&&b[i]; i++)buffer[i+2]=b[i];
	put_int(&buffer[8], Inp_Sou_Layer);
	buffer[10] = 7;
	buffer[11] = 7;
	buffer[12] = 7;
	buffer[13] = 7;
	put_int(&buffer[14], NStrip);
	put_int(&buffer[16], SizeX5089);
	put_int(&buffer[18], STRIP_SIZE);
	put_long(&buffer[20], W_CXMIN);
	put_long(&buffer[24], W_CYMIN);
	check_sum(buffer, 28);
	buffer[28] = ks_c;
	fwrite(buffer, 1, 29,__lgl__);
}
long
get_poly_5089(long ns)
{
	long i;
	long off0;
	W_ITEM it;
	LengthPoly = 0;
	off0 = ftell(__dat__);
	read_witem(&it, 1,__dat__);
	if(feof(__dat__))return 0;
	if(it.nblock==ns)
	{
		coordX[0] = it.x0;
		coordY[0] = it.y0;
		LengthPoly = it.npoints;
		if(LengthPoly<4)
		{
			zAbort(sp5089,__LINE__);
		}
		for(i=1; i<LengthPoly-1; ++i)
		{
			coordX[i] = (long)it.coord[i-1].x + coordX[0];
			coordY[i] = (char)it.coord[i-1].y;
		}
		coordX[LengthPoly-1] = coordX[0];
		coordY[LengthPoly-1] = coordY[0];
		Current_Rangx=it.rang_x;
		Current_Stepx=it.step_x;
		return 1;
	}
	else
	if(it.nblock>ns)
	{
		fseek(__dat__, off0, SEEK_SET);
		return 0;
	}
	zAbort(sp5089,__LINE__);
	return 0;
}
long
rectangle(void)
{
	if(LengthPoly!=5)return 0;
	if((coordX[0]==coordX[1])
	&& (coordX[2]==coordX[3])
	&& (coordY[1]==coordY[2])
	&& (coordY[3]==coordY[0]) )return 1;
	return 0;
}
void
put_poly_to_buf(void)
{
	unsigned short dx, i;
	while((coordX[0]-lastX)>MAX_DISTANCE)
	{
		dx = MAX_DISTANCE|0x8000;   // dummy rectangle
		buf_put_int(dx);
		buf_put_char(0);
		buf_put_char(0);
		buf_put_char(1);
		lastX += MAX_DISTANCE;
	}
	dx = (short)(coordX[0]-lastX);     // origin point
	if(rectangle())
	{
		if(dx<64)
		buf_put_char((unsigned char)(dx));
		else
		buf_put_int(dx|0x8000);
		buf_put_char(coordY[0]);
		dx = (short)(coordX[2]-coordX[0]);
		if(dx<128)buf_put_char((unsigned char)(dx));
		else buf_put_int(dx|0x8000);
		buf_put_char(coordY[2]);
	}
	else  // poly
	{
		if(dx<64)buf_put_char((unsigned char)(dx|0x40));
		else buf_put_int(dx|0xC000);
		buf_put_char(coordY[0]);
		buf_put_char((unsigned char)LengthPoly-1);
		for(i=1; i<LengthPoly-1; ++i)
		{
			dx = (short)(coordX[i]-coordX[0]);
			if(dx<128)buf_put_char((unsigned char)(dx));
			else buf_put_int(dx|0x8000);
			buf_put_char(coordY[i]);
		}
	}
}
void M_5089_MAIN_3(void)
{
	unsigned char fragm[5], fragm_0[5], head[6], chs_buf[3];
	long i, j, nblock, origin, nfragm, nsubblk;
	long strip_off, fragm_off;
	long total_mult=0, total_poly=0;
	long Buf_1[32];
	char Buf_2[32];
	MaSCOT_IO.Init_ILayer_File(&I_Layer[0]);
	__nam__[0]=0;
	strncat(__nam__, I_Layer[0].Name, 8);
	strncat(__nam__, ".lgl", 5);
	__lgl__=__open__(__nam__, "wb");
	buffer = new unsigned char[MAX_BL_LENGTH+100];
	coordX = Buf_1;
	coordY = Buf_2;
	put_area_descr();
	for(i=0; i<NStrip; ++i)
	{
		buf_length=lastX=nblock=nfragm=nsubblk=0;
		ks_c=0;
		while(get_poly_5089(i))
		{
			if(Current_Rangx>1)
			{
				if(!nblock)
				{
					strip_off = ftell(__lgl__);
					fwrite(head,1,6,__lgl__); // reserved space for strip header
				}
				buf_length=lastX=0;
				origin=coordX[0];
				coordX[0]=0;
				for(j=1; j<LengthPoly-1; ++j)coordX[j]-=origin;
				put_poly_to_buf();
				put_int(&fragm[0], (buf_length+5)|0x8000); // last subarray
				put_int_3(&fragm[2],lastX);
				check_sum(fragm,5);
				fwrite(fragm,1,5,__lgl__);
				check_sum(buffer,buf_length);
				fwrite(buffer,1,buf_length,__lgl__);

				put_int(&buffer[0], 10|0x8000);
				buffer[2]=0;   // mult
				put_int_3(&buffer[3],origin);
				put_int_2(&buffer[6],Current_Stepx);
				put_int_2(&buffer[8],Current_Rangx);
				check_sum(buffer,10);
				fwrite(buffer,1,10,__lgl__);
				++nfragm;
				++nblock;
				total_mult+=Current_Rangx;
				buf_length=lastX=0;
				continue;
			}
			put_poly_to_buf();
			++total_poly;
			lastX = coordX[0];
			if(buf_length>MAX_BL_LENGTH)
			{
				if(nblock)
				{
					if (!nsubblk)
					{
						fragm_off = ftell(__lgl__);
						put_int(&fragm_0[0], buf_length+5);
						fwrite(fragm_0,1,5,__lgl__); 	// reserved space for fragment header
						__error__(__lgl__,__nam__);
					}
					else
					{
						put_int(&fragm[0], buf_length+2);
						check_sum(fragm,2);
						fwrite(fragm,1,2,__lgl__);
						__error__(__lgl__,__nam__);
					}
				}
				else
				{
					strip_off = ftell(__lgl__);
					fwrite(head, 1, 6,__lgl__);    // reserved space for strip header
					fragm_off = ftell(__lgl__);
					fwrite(fragm_0, 1, 5,__lgl__); // reserved space for fragment header
					__error__(__lgl__,__nam__);
					put_int(&fragm_0[0], buf_length+5);
				}
				check_sum(buffer, buf_length);
				fwrite(buffer, 1, buf_length,__lgl__);
				__error__(__lgl__,__nam__);
				++nblock;
				nsubblk++;
				buf_length = 0;
			}
		}
		if(nblock)fflush(__lgl__),fseek(__lgl__,strip_off,SEEK_SET);	// write strip description
		head[0] = 0x80;
		head[1] = 6;
		put_int(&head[2], i);
		head[4] =(unsigned char)++nfragm;  // numbers of fragmrnts of the strip
		if(nfragm>200)
		{
			fprintf(stderr, "\n!!! TOO MANY MULTIPLICATIONS ON STRIP.");
			zAbort(sp5089,__LINE__);
		}
		head[5] = (i%2)*2;		   // direction: 2 - right to left
		if(i==NStrip-1)head[5]|=0x4;       // last strip
		if(buf_length|nblock)head[5]|=0x8; // filled strip
		check_sum(head,6);
		fwrite(head,1,6,__lgl__);
		__error__(__lgl__,__nam__);
		if(nblock)fflush(__lgl__);
		if(nsubblk)fseek(__lgl__,fragm_off,SEEK_SET);   // write first block header
		else
		{
			fseek(__lgl__,0L,SEEK_END);
			put_int(&fragm_0[0],(buf_length+5)|0x8000);
		}
		put_int_3(&fragm_0[2],lastX);
		check_sum(fragm_0,5);
		fwrite(fragm_0,1,5,__lgl__);
		__error__(__lgl__,__nam__);
		if(nsubblk)
		{
			fflush(__lgl__);
			fseek(__lgl__,0L,SEEK_END);
			put_int(&fragm[0],(buf_length+2)|0x8000);
			check_sum(fragm,2);
			fwrite (fragm,1,2,__lgl__);
		}
		fseek(__lgl__, 0L, SEEK_END);
		if(buf_length)
		{
			check_sum(buffer,buf_length);
			fwrite(buffer,1,buf_length,__lgl__);
		}
		chs_buf[0] = 0;
		chs_buf[1] = 0;
		chs_buf[2] = ks_c;
		fwrite(chs_buf, 1, 3,__lgl__);
		__error__(__lgl__,__nam__);
	}
	fprintf(stderr, "\n!!! TOTAL:\tFLASHES=%ld", total_poly+total_mult);
	put_int(&fragm[0], 0x8000);
	fwrite (fragm, 1, 2,__lgl__);
	__close__(__lgl__,__nam__);
	delete buffer;
	MaSCOT_IO.Close_ILayer(0);
	__unlink__(I_Layer[0].PdbN);
}
