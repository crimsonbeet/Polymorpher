#define F_SIZE_MODULE

#include "stdafx.h"

#include<STDLIB.H>
#include<CTYPE.H>
#include<STRING.H>

#include<LIMITS.H>
#include<PROCESS.H>

#include "MASInterface.h"
#include "fqeval.hpp"
#include "fxeval.hpp"
#include "scale.hpp"

long Total_Shapes;
const char*f3size="f3size";
extern const char*OvrlpMstPrf;
extern void zabort(void);
extern void INSERT_VECTORS(LbBead*Lb);
/*CORES FOR LEAD-IN AND TAIL VECTORS*/
/*FOR EACH ROW */
_sAlong*LLDPTR[3];
long LLDLIM[3];
long LLDASS[3];
long CnFROM[3];
long CentTO[3];
long RgFROM[3];
long RighTO[3];
long LfFROM[3];
long LeftTO[3];
/*FOR EACH COLUMN*/
#define Pdb_File_X (MaSCOT_IO.I_Layer[3].File)
#define Pdb_File_1 (MaSCOT_IO.I_Layer[0].File)
#define Pdb_File_2 (MaSCOT_IO.I_Layer[1].File)
#define Pdb_File_3 (MaSCOT_IO.I_Layer[2].File)
#define Bdb_File_X (MaSCOT_IO.O_Layer.File)

char Pdb_Load_Too;

#define Pdb_Block_X (MaSCOT_IO.I_Layer[3].Fragment)
#define Bdb_Block_1 (MaSCOT_IO.I_Layer[0].Fragment)
#define Bdb_Block_2 (MaSCOT_IO.I_Layer[1].Fragment)
#define Bdb_Block_3 (MaSCOT_IO.I_Layer[2].Fragment)
#define Block_For_Get (MaSCOT_IO.DFragment)
/*COMMON GLOBALS*/
long Pdb_Xg;
long Pdb_Yg;
long Bdb_Xg;
long Bdb_Yg;
char Bdb_Regen;
char Left_Enabled; /*FOR Xg == 0 LEFT COLUMN DISABLED*/
char Right_Enabled; /*FOR Xg == XgCnt-1 RIGHT COLUMN DISABLED*/
#define Left_Disabled (!Left_Enabled)
#define Right_Disabled (!Right_Enabled)
/**/
char BS1_coord;
char BS2_coord;
/**/
#define XgCnt XBCNT
#define YgCnt YBCNT
#define XgDiv XBSIZ
#define YgDiv YBSIZ
#define CxMin CXMIN
#define CyMin CYMIN
#define CxMax CXMAX
#define CyMax CYMAX
/**/
long Lb_CnFrom[3];
long Lb_LfFrom[3];
long Lb_RgFrom[3];
long Lb_CentTo[3];
long Lb_LeftTo[3];
long Lb_RighTo[3];

long Lb_PbasTO;

long Delta_X;
long Delta_Y;
long Delta_X_2;
long Delta_Y_1;
long Delta_Y_2;


char __inTOPlt__;




/**/
void __FINISHEDNORMALLY__(void);
void
__POSERR__(void)
{
	fprintf(stderr, "\n!!! PDB-FILE POSITION ERROR.");
}
long
LldNext(long L)
{
	return(LLDLIM[L]++);
}


void
LinkToMasInp(long F)
{
	MaSCOT_IO.D_Link(F);
	MaSCOT_IO.DDelta_X = -Delta_X;
	MaSCOT_IO.DDelta_Y = -Delta_Y;
}
void
Load_Insert_Master(char open_shape = 0)
{
	INSERT_VECTORS(Loadmaster(0, open_shape));
}


void
Mirror_File_Buffer(long S, long D)
{
	XMemoryFile* file_s = MaSCOT_IO.I_Layer[S].File; 
	XMemoryFile* file_d = MaSCOT_IO.I_Layer[D].File;

	fpos_t I = fgetpos(file_s);
	fsetpos(file_d, &I);

	char aByte; 
	while(file_s->read(&aByte, 1, 1)) {
		file_d->write(&aByte, 1, 1);
	}
}
char ZDSKIP_P(void)
{
	char Z[4];
	MaSCOT_IO.I_READ(Z, 1);
	if(!Z[0])
	{
		return 0;
	}
	#define Z_LEN(Z) ((Z)&7)
	MaSCOT_IO.I_READ(Z, Z_LEN(Z[0]));
	#undef Z_LEN
	return(1);
}

void
__54217__(void)
{
	long i=0;
start__54217__:
	#define BL MaSCOT_IO.I_Layer[MaSCOT_IO._iLayer].Fragment
	++i;
	if(!ZDSKIP_P())
	{
		if(i>1)
		{
			return;
		}
		long L;
		char Z;
		MaSCOT_IO.I_READ(&Z, 1), L = (Z&(7<<4))>>4;
		if(L)
		{
			zabort(f3size,__LINE__);
		}
		BL = 0;
		MaSCOT_IO.I_READ((char*)&BL, Z);
		if(feof(MaSCOT_IO.I_Layer[MaSCOT_IO._iLayer].File))
		{
			BL = LONG_MAX;
		}
		return;
	}
	#undef BL
	else
	if(!ZDSKIP_P())
	{
	      zabort(f3size,__LINE__);
	}
	goto start__54217__;
}
void
Seek_Block(long F)
{
	long B;
	B = Bdb_Xg * YgCnt + Bdb_Yg;

	LinkToMasInp(F);
	while (MaSCOT_IO.I_Layer[MaSCOT_IO._iLayer].Fragment < B)__54217__();
}


void
Generate_CBDB(_sAlong*Z, long L)
{
	long i, Y;
	char o, O;
	Y = ZLDCO1[0];

	if(Y==Delta_Y_1) o = 1;
	else
	if(Y==Delta_Y_2) o = 1;
	else o = 0;

	long y=Y;
	for(i = 1; i<ZLDLIM; ++i)
	{
		Y = ZLDCO1[i];

		O = Y==Delta_Y_1? 1: Y==Delta_Y_2? 1: 0;
		char f = __inTOPlt__? O&&o: O||o;
		if(f)
		{
			(*Z)[LldNext(L)] = ZLDCO0[i-1];
			(*Z)[LldNext(L)] = y;

			(*Z)[LldNext(L)] = ZLDCO0[i];
			(*Z)[LldNext(L)] = Y;
		}
		o = O;
		y = Y;
	}
}
void
Generate_BBDB(_sAlong*Z, long L)
{
	long i, X;
	char o, O;
	X = ZLDCO0[0];

	if(X==Delta_X_2) o = 1;
	else o = 0;

	long x=X;
	for(i = 1; i<ZLDLIM; ++i)
	{
		X = ZLDCO0[i];

		O = X==Delta_X_2? O = 1: 0;
		char f = __inTOPlt__? O&&o: O||o;
		if(f)
		{
			(*Z)[LldNext(L)] = x;
			(*Z)[LldNext(L)] = ZLDCO1[i-1];

			(*Z)[LldNext(L)] = X;
			(*Z)[LldNext(L)] = ZLDCO1[i];
		}
		o = O;
		x = X;
	}
}
void
Read_Bdb_Center(long L)
{
	long B;
	_sAlong*Z;
	L = LLDASS[L];
	Z = LLDPTR[L];

	CnFROM[L] = LLDLIM[L];
	CentTO[L] = LLDLIM[L];

	if(Bdb_Yg==YgCnt)
	{
/*SCAN REACHED UPPER BOUNDARY.*/
		Bdb_Regen = 1;
		return;
	}
	Delta_X = CxMin + Bdb_Xg*XgDiv;
	Delta_Y = CyMin + Bdb_Yg*YgDiv;

	Delta_Y_1 = Delta_Y;
	Delta_Y_2 = Delta_Y + YgDiv;
	if(Delta_Y_2==CyMax)
	{
/*UPPER BOUNDARY VECTORS MUST BE SCIPPED.*/
		Delta_Y_2 -= YgDiv;
	}
	if(Delta_Y_1==CyMin)
	{
/*LOWER BOUNDARY VECTORS MUST BE SCIPPED.*/
		Delta_Y_1 += YgDiv;
	}
	B = Bdb_Xg*YgCnt + Bdb_Yg;

	if(Bdb_Block_2>B)
	{
/*THIS FRAGMENT IS EMPTY.*/
		return;
	}
	else
	if(Bdb_Block_2<B)
	{
		__POSERR__();
		zabort(f3size,__LINE__);
	}
	LinkToMasInp(1);

	for(MaSCOT_IO.I_READ_P(); Bdb_Block_2==B; MaSCOT_IO.I_READ_P())
	{
		if(ZLDLIM==0)
		{
			continue;
		}
		LDMake_ClosedPolygon();
		if(Pdb_Load_Too)
		{
			Load_Insert_Master();
		}
		if(YgCnt==1)
		{
			continue;
		}
		Generate_CBDB(Z, L);
	}
	CentTO[L] = LLDLIM[L];
}
void
Read_Bdb_Right(long L)
{
	long B;
	_sAlong*Z;
	L = LLDASS[L];
	Z = LLDPTR[L];

	RgFROM[L] = LLDLIM[L];
	RighTO[L] = LLDLIM[L];

	if(Right_Disabled)
	{
/*BORDER CASE*/
		return;
	}
	if(Bdb_Yg==YgCnt)
	{
/*SCAN REACHED UPPER BOUNDARY.*/
		Bdb_Regen = 1;
		return;
	}
	Delta_X = CxMin + Bdb_Xg*XgDiv;
	Delta_Y = CyMin + Bdb_Yg*YgDiv;
	Delta_X_2 = Delta_X;

	B = Bdb_Xg*YgCnt + Bdb_Yg;
	if(Bdb_Block_3>B)
	{
/*THIS FRAGMENT IS EMPTY.*/
		return;
	}
	else
	if(Bdb_Block_3<B)
	{
		__POSERR__();
		zabort(f3size,__LINE__);
	}
	LinkToMasInp(2);

	for(MaSCOT_IO.I_READ_P(); Bdb_Block_3==B; MaSCOT_IO.I_READ_P())
	{
		if(ZLDLIM==0)
		{
			continue;
		}
		LDMake_ClosedPolygon();
		Generate_BBDB(Z, L);
	}
	RighTO[L] = LLDLIM[L];
}
void
Read_Bdb_Left(long L)
{
	long B;
	_sAlong*Z;
	L = LLDASS[L];
	Z = LLDPTR[L];

	LfFROM[L] = LLDLIM[L];
	LeftTO[L] = LLDLIM[L];

	if(Left_Disabled)
	{
/*BORDER CASE*/
		return;
	}
	if(Bdb_Yg==YgCnt)
	{
/*SCAN REACHED UPPER BOUNDARY.*/
		Bdb_Regen = 1;
		return;
	}
	Delta_X = CxMin + Bdb_Xg*XgDiv;
	Delta_Y = CyMin + Bdb_Yg*YgDiv;
	Delta_X_2 = Delta_X + XgDiv;

	B = Bdb_Xg*YgCnt + Bdb_Yg;
	if(Bdb_Block_1>B)
	{
/*THIS FRAGMENT IS EMPTY.*/
		return;
	}
	else
	if(Bdb_Block_1<B)
	{
		__POSERR__();
		zabort(f3size,__LINE__);
	}
	LinkToMasInp(0);

	for(MaSCOT_IO.I_READ_P(); Bdb_Block_1==B; MaSCOT_IO.I_READ_P())
	{
		if(ZLDLIM==0)
		{
			continue;
		}
		LDMake_ClosedPolygon();
		Generate_BBDB(Z, L);
	}
	LeftTO[L] = LLDLIM[L];
}
void
Read_Pdb_Next_Block(void)
{
	long P;
	if(Pdb_Yg==YgCnt)
	{
		Bdb_Regen = 1;
		return;
	}
	Delta_X = CxMin + Pdb_Xg*XgDiv;
	Delta_Y = CyMin + Pdb_Yg*YgDiv;

	P = Pdb_Xg*YgCnt + Pdb_Yg;
	if(Pdb_Block_X>P)
	{
/*THIS FRAGMENT IS EMPTY.*/
		return;
	}
	else
	if(Pdb_Block_X<P)
	{
		__POSERR__();
		zabort(f3size,__LINE__);
	}
	LinkToMasInp(3);

	for(MaSCOT_IO.I_READ_P(); Pdb_Block_X==P; MaSCOT_IO.I_READ_P())
	{
		if(ZLDLIM==0)
		{
			continue;
		}
		LDMake_ClosedPolygon();
		Load_Insert_Master();
	}
}
void
Load_Next_Row_Common_Case(void)
{
	long I;
	I = LLDASS[0];
	LLDASS[0] = LLDASS[1];
	LLDASS[1] = LLDASS[2];
	LLDASS[2] = I;
	LLDLIM[I] = 0;
	LeftTO[I] = 0;
	CentTO[I] = 0;
	RighTO[I] = 0;
	LfFROM[I] = 0;
	CnFROM[I] = 0;
	RgFROM[I] = 0;

	++Pdb_Yg;

	PXMIN = CxMin + Pdb_Xg*XgDiv - (XgDiv>>1);
	PYMIN = CyMin + Pdb_Yg*YgDiv - (YgDiv>>1);

	PXMAX = CxMin + Pdb_Xg*XgDiv + (XgDiv<<1);
	PYMAX = CyMin + Pdb_Yg*YgDiv + (YgDiv<<1);

	CleanZPTHASH();

	Bdb_Xg = Pdb_Xg;
	Bdb_Yg = Pdb_Yg + 1;

	Pdb_Load_Too = 0;

	Read_Bdb_Center(2);
	Bdb_Xg = Pdb_Xg - 1;
	Read_Bdb_Left(2);
	Bdb_Xg = Pdb_Xg + 1;
	Read_Bdb_Right(2);

	Read_Pdb_Next_Block();
}
void
Regen_Row_Streams(void)
{
	long i;
	LLDASS[0] = 0;
	LLDASS[1] = 1;
	LLDASS[2] = 2;
	LLDLIM[0] = 0;
	LLDLIM[1] = 0;
	LLDLIM[2] = 0;

	for(i = 0; i<3; ++i)
	{
		LeftTO[i] = 0;
		CentTO[i] = 0;
		RighTO[i] = 0;
		LfFROM[i] = 0;
		CnFROM[i] = 0;
		RgFROM[i] = 0;
	}
	LLDPTR[0] = &ZLDCO2;
	LLDPTR[1] = &ZLDCO3;
	LLDPTR[2] = &ZLDCO4;

	if(Left_Disabled&&XgCnt!=1)
	{
		Left_Enabled = 1;
	}
	Pdb_Xg+= 1;
	Pdb_Yg = 0;

	PXMIN = CxMin + Pdb_Xg*XgDiv - (XgDiv>>1);
	PYMIN = CyMin + Pdb_Yg*YgDiv - (YgDiv>>1);

	PXMAX = CxMin + Pdb_Xg*XgDiv + (XgDiv<<1);
	PYMAX = CyMin + Pdb_Yg*YgDiv + (YgDiv<<1);

	CleanZPTHASH();

	Bdb_Regen = 0;

	if(Pdb_Xg==XgCnt-1) Right_Enabled=0;
	else if(Pdb_Xg==XgCnt) return;
	else Right_Enabled=1;

	Pdb_Load_Too = 1;

	Bdb_Xg = Pdb_Xg;
	Bdb_Yg = Pdb_Yg;
	Read_Bdb_Center(1);

	Mirror_File_Buffer(1, 3);

	Pdb_Load_Too = 0;
	++Bdb_Yg;
	Read_Bdb_Center(2);
	Bdb_Xg = Pdb_Xg-1;
	Bdb_Yg = Pdb_Yg;
	Read_Bdb_Left(1);
	++Bdb_Yg;
	Read_Bdb_Left(2);
	Bdb_Xg = Pdb_Xg+1;
	Bdb_Yg = Pdb_Yg;
	Read_Bdb_Right(1);
	++Bdb_Yg;
	Read_Bdb_Right(2);
}
void
Initial_Load_Common_Case(void)
{
	long i;
	ZLBLIM = 1;
	ZLVLIM = 1;
	ZPTLIM = 1;
	ZVTLIM = 2;

	Pdb_Xg = 0;
	Pdb_Yg = 0;

	PXMIN = CxMin + Pdb_Xg*XgDiv - (XgDiv>>1);
	PYMIN = CyMin + Pdb_Yg*YgDiv - (YgDiv>>1);

	PXMAX = CxMin + Pdb_Xg*XgDiv + 2.0*XgDiv;
	PYMAX = CyMin + Pdb_Yg*YgDiv + 2.0*YgDiv;

	CleanZPTHASH();

	LLDASS[0] = 0;
	LLDASS[1] = 1;
	LLDASS[2] = 2;
	LLDLIM[0] = 0;
	LLDLIM[1] = 0;
	LLDLIM[2] = 0;
	for(i = 0; i<3; ++i)
	{
		LeftTO[i] = 0;
		CentTO[i] = 0;
		RighTO[i] = 0;
		LfFROM[i] = 0;
		CnFROM[i] = 0;
		RgFROM[i] = 0;
	}
	LLDPTR[0] = &ZLDCO2;
	LLDPTR[1] = &ZLDCO3;
	LLDPTR[2] = &ZLDCO4;

	Left_Enabled=0;
	if(XgCnt>1) Right_Enabled=1;
	else Right_Enabled=0;

	Bdb_Regen = 0;

	Pdb_Load_Too = 1;
	Bdb_Xg = 0;
	Bdb_Yg = 0;
	Read_Bdb_Center(1);
	Mirror_File_Buffer(1, 3);
	Pdb_Load_Too = 0;
	Bdb_Yg = 1;
	Read_Bdb_Center(2);
	Mirror_File_Buffer(1, 2);
	Bdb_Xg = 1;
	Bdb_Yg = 0;
	if(Right_Enabled)Seek_Block(2);
	Read_Bdb_Right(1);
	Bdb_Yg = 1;
	Read_Bdb_Right(2);
}
void
LLD_Insert(_sAlong*L,long D,char c,long I,long J)
{
	char on;
	char zz;
	long LD;
	long zx;
	char open_shape = 1;
	zz = __inTOPlt__? 1: 0;
	for(on = zz; I<J; on = zz)
	{
		ZLDLIM = 0;

		LD = ZLDLIM++;
		ZLDCO0[LD] = (*L)[I++];
		zx = ZLDCO1[LD] = (*L)[I++];
		if(c)
		{
			if(__inTOPlt__)
			{
				on &= zx==D? 1: 0;
			}
			else	on |= zx==D? 1: 0;
		}
		else on = 1;

		LD = ZLDLIM++;
		ZLDCO0[LD] = (*L)[I++];
		zx = ZLDCO1[LD] = (*L)[I++];

		if(c)
		{
			if(__inTOPlt__)
			{
				on &= zx==D? 1: 0;
			}
			else	on |= zx==D? 1: 0;
		}
		if(on) Load_Insert_Master(open_shape);
	}
	open_shape = 0;
}
void Load_Right(long L)
{
	long K, I, J, D;

	K = L;
	L = LLDASS[L];
	I = RgFROM[L];
	J = RighTO[L];

	D = CyMin + Pdb_Yg*YgDiv;
	if(K==2) D += YgDiv;

	Lb_RgFrom[K] = ZLBLIM;
	LLD_Insert(LLDPTR[L], D, (K!=1), I, J);
	Lb_RighTo[K] = ZLBLIM;
}
void Load_Left(long L)
{
	long K, I, J, D;

	K = L;
	L = LLDASS[L];
	I = LfFROM[L];
	J = LeftTO[L];

	D = CyMin + Pdb_Yg*YgDiv;
	if(K==2) D += YgDiv;

	Lb_LfFrom[K] = ZLBLIM;
	LLD_Insert(LLDPTR[L], D, (K!=1), I, J);
	Lb_LeftTo[K] = ZLBLIM;
}
void Load_Center(long L)
{
	long K, I, J, D;

	K = L;
	L = LLDASS[L];
	Lb_CnFrom[K] = ZLBLIM;
	Lb_CentTo[K] = ZLBLIM;

	if(K==1)
	{
		return;
	}
	I = CnFROM[L];
	J = CentTO[L];
	D = CyMin + Pdb_Yg*YgDiv;
	if(K==2) D += YgDiv;

	LLD_Insert(LLDPTR[L], D, 1, I, J);
	Lb_CentTo[K] = ZLBLIM;
}
char
BScan(long I, long J, VtBead*D, long y, long Y)
{
	long K;
	long z, x, Z, X;
	LbBead*b;
	LvBead*v;
	LvBead*e;
	VtBead*V;
	PtBead*p;
	PtBead*P;
	while(I++<J)
	{
		b = GetLbBead(I-1);
		v = b->ZLBFirstLvBead;
		if(v==0)
		{
			continue;
		}
		e = v;
		K = 0;
		do
		{
			V = v->ZLVMyVector;
			v = v->ZLVNextLvBead;
			++K;
			if (!VtIsEven(V))V = V->ZVTMeInreverse;
			p = V->ZVTCO1;
			z = BS1_coord ? p->ZPTCO1 : p->ZPTCO2;
			x = BS2_coord ? p->ZPTCO2 : p->ZPTCO1;
			P = V->ZVTCO2;
			Z = BS1_coord ? P->ZPTCO1 : P->ZPTCO2;
			X = BS2_coord ? P->ZPTCO2 : P->ZPTCO1;
			if (z != Z)
			{
				b->ZLBFirstLvBead = 0;
				continue;
			}
			if (X <= y)
			{
				continue;
			}
			if (x >= Y)
			{
				continue;
			}
			if (x == y)
			{
				if (K == 1 && v == e && X == Y)
				{
					b->ZLBFirstLvBead = 0;
					return 0;
				}
				continue;
			}
			if (X == Y)
			{
				continue;
			}
			if (x < y)
			{
				SPLITVT(V, D->ZVTCO1);
				return 1;
			}
			if (x > y)
			{
				SPLITVT(D, p);
				return 1;
			}
		} while (v && e != v);
	}
	return 0;
}
char
On_Left_Bound(VtBead*D, long y, long Y)
{
	long I;
	long J;
	char R;
	if(!VtIsEven(D))
	{
		D = D->ZVTMeInreverse;
		I = Y;
		Y = y;
		y = I;
	}
	I = Lb_LfFrom[1];
	J = Lb_LeftTo[1];

	BS1_coord = 1;
	BS2_coord = 1;

	R = BScan(I, J, D, y, Y);
	return R;
}
char
On_Right_Bound(VtBead*D, long y, long Y)
{
	long I;
	long J;
	char R;
	if(!VtIsEven(D))
	{
		D = D->ZVTMeInreverse;
		I = Y;
		Y = y;
		y = I;
	}
	I = Lb_RgFrom[1];
	J = Lb_RighTo[1];

	BS1_coord = 1;
	BS2_coord = 1;

	R = BScan(I, J, D, y, Y);
	return R;
}
char
On_Lower_Bound(VtBead*D, long x, long X)
{
	long I;
	long J;
	char R;
	if(!VtIsEven(D))
	{
		D = D->ZVTMeInreverse;
		I = X;
		X = x;
		x = I;
	}
	I = Lb_CnFrom[0];
	J = Lb_CentTo[0];

	BS1_coord = 0;
	BS2_coord = 0;

	R = BScan(I, J, D, x, X);
	return R;
}
char
On_Upper_Bound(VtBead*D, long x, long X)
{
	long I;
	long J;
	char R;
	if(!VtIsEven(D))
	{
		D = D->ZVTMeInreverse;
		I = X;
		X = x;
		x = I;
	}
	I = Lb_CnFrom[2];
	J = Lb_CentTo[2];

	BS1_coord = 0;
	BS2_coord = 0;

	R = BScan(I, J, D, x, X);
	return R;
}
void On_Boundary_Extraction(void)
{
	long b;
	char o, O, z, Z, A, B, R;
	long x, y, X, Y;
	Delta_Y = CyMin + Pdb_Yg*YgDiv;
	Delta_X = CxMin + Pdb_Xg*XgDiv;

	Delta_Y_2 = Delta_Y + YgDiv;
	Delta_X_2 = Delta_X + XgDiv;

	for(b = 1; b<Lb_PbasTO; ++b)
	{
		LbBead*Lb=GetLbBead(b);
		LvBead*v;
		LvBead*e;
		PtBead*p;
		VtBead*V;
		v = e = Lb->ZLBFirstLvBead;

		p = e->ZLVMyVector->ZVTCO1;
		x = p->ZPTCO1;
		y = p->ZPTCO2;

		o = 0;
		O = 0;

		if(x==Delta_X)   o = 1;
		else
		if(x==Delta_X_2) o = 2;

		if(y==Delta_Y)   O = 4;
		else
		if(y==Delta_Y_2) O = 8;

		A = o|O;
		do
		{
			V = v->ZLVMyVector;
			p = V->ZVTCO2;

			X = p->ZPTCO1;
			Y = p->ZPTCO2;

			z = 0;
			Z = 0;
			R = 0;
			if(X==Delta_X)   z = 1;
			else
			if(X==Delta_X_2) z = 2;
			if(Y==Delta_Y)   Z = 4;
			else
			if(Y==Delta_Y_2) Z = 8;
			B = z|Z;
			if(!A||!B||!(A&B))
			{
		/*INSIDE! MUST BE SKIPPED.*/
				x = X;
				y = Y;
				A = B;
				o = z;
				O = Z;
				v = v->ZLVNextLvBead;
				continue;
			}
		/*NOTE! ALL POLYGONS ARE*/
		/*CLOCKWISE.*/
		/*BECAUSE OF IT BOUNDARY*/
		/*SIDE EVALUATING IS MORE*/
		/*SIMPLE.*/
			if(o)
			switch(o)
			{
			case 1: /*LEFT OR UPPER*/
				if(z==1) R = On_Left_Bound (V, y, Y);
				else     R = On_Upper_Bound(V, x, X);
				break;
			case 2: /*RIGHT OR LOWER*/
				if(z==2) R = On_Right_Bound(V, y, Y);
				else     R = On_Lower_Bound(V, x, X);
				break;
			}
			else
			switch(O)
			{
			case 8: /*UPPER*/
				R = On_Upper_Bound(V, x, X);
				break;
			case 4: /*LOWER*/
				R = On_Lower_Bound(V, x, X);
				break;
			}
			if(!R)
			{
				x = X;
				y = Y;
				A = B;
				o = z;
				O = Z;
				v = v->ZLVNextLvBead;
			}
		}
		while(!R&&v!=e);
	}
}
#define NtHrVt(V) ((V)->ZVTCO1->ZPTCO2!=(V)->ZVTCO2->ZPTCO2)
long __SI1__(char Hr)
{
	register long v;
	long c=0;
	for(v = 2; v<ZVTLIM; ++v)
	{
		VtBead*V=GetVtBead(v);
		VtBead*R=V->ZVTMeInreverse;
		if(!R)
		{
			continue;
		}
		if(!V->ZVTFirstCoincident)
		{
			if(!Hr||NtHrVt(V))
			{
				++c;
				continue;
			}
		}
		else
		if(!R->ZVTFirstCoincident)
		{
			if(!Hr||NtHrVt(R))
			{
				++c;
				continue;
			}
		}
		V->ZVTMeInreverse = 0;
		R->ZVTMeInreverse = 0;
	}
	if(c!=((c>>1)<<1))
	{
		zabort(f3size,__LINE__);
	}
	return c;
}
void __SI2__(void)
{
	long b;
	for(b = 1; b<Lb_PbasTO; ++b)
	{
		LbBead*Lb=GetLbBead(b);
		LvBead*e;
		LvBead*v;
		VtBead*z;
		e = Lb->ZLBFirstLvBead;
		if(!e)
		{
			continue;
		}
		v = e;
		do
		{
			z = v->ZLVMyVector;
			z->ZVTStatus = 1;
			v = v->ZLVNextLvBead;
		}
		while(v!=e);
	}
}


char __Tail_follow__;
char __Head_follow__;

long __ZLDFI__;
long __ZLDLA__;


void
Write_Chain_Pieces(void)
{
	long S;
	long E;
	long I;
	char open_shape = 1;
	if(INDAux_3Lim%2)
	{
		zabort(f3size,__LINE__);
	}
	I=0;
	while(I<INDAux_3Lim)
	{
		S = INDAux_3[I++];
		E = INDAux_3[I++];
		if(S>E)
		{
			__Head_follow__= 1;
			__ZLDFI__= S;
			__ZLDLA__= ZLDLIM;

			MaSCOT_IO.Dix_output(0, 0, open_shape);

			__Head_follow__= 0;
			__Tail_follow__= 1;
			__ZLDFI__= 0;
			__ZLDLA__= E+1;

			MaSCOT_IO.Dix_output(0, 0, open_shape);
			__Tail_follow__= 0;
		}
		else
		{
			__ZLDFI__= S;
			__ZLDLA__= E+1;
			MaSCOT_IO.Dix_output(0, 0, open_shape);
		}
		__ZLDFI__= 0;
		__ZLDLA__= 0;
	}
	open_shape = 0;
}
void
Put_Dj_Local(long LD)
{
	INDAux_3[INDAux_3Lim++] = LD;
}

void(*SxTraverse)(VtBead*VT);

void
SxTraverse_0(VtBead*VT)
{
	long LD;
	long XM;
	long xm;
	long MX;
	long mx;
	long xx;
	PtBead*P2;
	VtBead*V3;
	VtBead*V4;
	XM = CxMin;
	xm = CxMax;
	mx = CxMin + Pdb_Xg*XgDiv;
	MX = mx+XgDiv;

	ZLDLIM = 0;
	INDAux_3Lim = 1;
	STATUS = 1;

	V3 = VT;
	do
	{
		V3->ZVTAux = 1;
		LD = ZLDLIM++;
		P2 = V3->ZVTCO2;
		xx = ZLDCO0[LD] = P2->ZPTCO1;
		ZLDCO1[LD] = P2->ZPTCO2;
		if(xx > XM) XM = xx;
		if(xx<xm)xm = xx;
		V4 = V3;
		V3 = V3->ZVTMeInreverse;
		V3 = FETCHSUCCESSOR(V3);
		while(!V3->ZVTMeInreverse)
		{
			V3 = FETCHSUCCESSOR(V3);
		}
		if(V4->ZVTMeInreverse==V3)Put_Dj_Local(LD);
	}
	while(V3!=VT);

	if(xm >= MX||XM <= mx)
	{
		if(!__inTOPlt__)
		{
			return;
		}
	}
	if(--INDAux_3Lim)
	{
		INDAux_3[0] = INDAux_3[INDAux_3Lim];
		Write_Chain_Pieces();
		return;
	}
	LDMake_ClosedPolygon();

	MaSCOT_IO.Dix_output();
}
void
SXTraverse(void)
{
	long v;
	ZLBLIM = 1;
	ZLVLIM = 1;

	for(v = 2; v<ZVTLIM; ++v)
	{
		VtBead*V=GetVtBead(v);
		if(V->ZVTAux)
		{
			continue;
		}
		if(!V->ZVTMeInreverse)
		{
			continue;
		}
		if(!V->ZVTStatus)
		{
			continue;
		}
		SxTraverse(V);
	}
}
void
ZZxLoadxAbout(void)
{
	long i;
	Lb_PbasTO = ZLBLIM;
	for(i = 0; i<3; ++i)
	{
		Load_Center(i);
		if(__inTOPlt__&&i!=1)
		{
			continue;
		}
		Load_Left(i);
		Load_Right(i);
	}
}
void Shapes_Generation(void)
{
	On_Boundary_Extraction();
	POINT_VECTOR_ANALYSIS(2);

	__SI1__(0);/*Horizontal Vt must be available.*/
	__SI2__();
	SXTraverse();
}
void
F_SIZE_SHAPES(F_SIZE_JOB&JOB)
{
fprintf(stderr, "\n!!! <Boundary data Extraction> Started on %s !!!\n",JOB.Name);
fprintf(stderr, "\nTotal Fragments = %ld (X=%ld, Y=%ld)\n", XgCnt*YgCnt, XgCnt, YgCnt);
	unsigned I;
	for(I = 0; I<4; ++I)
	{
		memcpy(MaSCOT_IO.I_Layer[I].Name, JOB.Name, 20);
	}
	MaSCOT_IO.ILayer_Cnt = 4;
	MaSCOT_IO.Activate_ILayers();
	for(I = 0; I<4; ++I)
	{
		MaSCOT_IO.INIT_ILayer(I);
	}
	if(!(MaSCOT_IO.I_Layer[0]._lIdentity.layer_STATUS&1))
	{
		fputs(OvrlpMstPrf, stderr);
		zabort();
	}
	OutDeltaX = 0;
	OutDeltaY = 0;

	SxTraverse = SxTraverse_0;

	MaSCOT_IO.O_Layer._lIdentity.PDBI = MaSCOT_IO.I_Layer[0]._lIdentity.PDBI;

	__inTOPlt__= JOB.Size==0;

	MaSCOT_IO.INIT_OLayer();
	IEvaluater.__SetVT0LB0__();
	Initial_Load_Common_Case();
	ZZxLoadxAbout();

	Shapes_Generation();
	while(Pdb_Xg+1<XgCnt||Pdb_Yg+1<YgCnt)
	{
		ZLBLIM = 1;
		ZLVLIM = 1;
		ZPTLIM = 1;
		ZVTLIM = 2;
		if(Bdb_Regen)
			Regen_Row_Streams();
		else
			Load_Next_Row_Common_Case();
		ZZxLoadxAbout();

		Shapes_Generation();
	}
	MaSCOT_IO.Dix_close();

end_shapes_job:
	MaSCOT_IO.SetMinMax();
	__inTOPlt__= 0;
	MaSCOT_IO.FCloseAll(); 
	IEvaluater._ReleaseStorage();
	memcpy(JOB.Name, MaSCOT_IO.O_Layer.Name, 20); 
__FINISHEDNORMALLY__();
}
