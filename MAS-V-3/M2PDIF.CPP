#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>

#include "m_alcr.hpp"
#include "fqeval.hpp"
#include "fxeval.hpp"
char*m2pdif="m2pdif";
extern long __SI1__(char Hr);
extern void __SI2__(void);
extern void(*SxTraverse)(VtBead*VT);
extern void INSERT_VECTORS(LbBead*Lb);
extern void Load_Insert_Master(void);
extern void SXTraverse(void);
extern long Lb_PbasTO;
extern FILE*Pdif;
extern char*Pdif_Name;
long PdifPoly_Level;
void
SxTraverse_1(VtBead*VT)
{
	long LD;
	PtBead*P2;
	VtBead*V3;
	VtBead*V4;
	ZLDLIM = 0;
	STATUS = 1;
	V3 = VT;
	do
	{
		V3->ZVTAux = 1;
		LD = ZLDLIM++;
		P2 = V3->ZVTCO2;
		ZLDCO0[LD] = P2->ZPTCO1;
		ZLDCO1[LD] = P2->ZPTCO2;
		V4 = V3;
		V3 = V3->ZVTCO6;
		V3 = FETCHSUCCESSOR(V3);
		while(!V3->ZVTCO6)
		{
			V3 = FETCHSUCCESSOR(V3);
		}
		if(V4->ZVTCO6==V3)zAbort(m2pdif,__LINE__);
	}
	while(V3!=VT);
	if(PdifPoly_Level++==0)
	{
		fprintf(Pdif, "{Ol 1");
	}
	else
	{
		fprintf(Pdif, "{Pv");
	}
	double Dcr = 10*UN001;

	long X = CFragment/YBCNT;
	long Y = CFragment - X*YBCNT;

	long x = CXMIN + X*XBSIZ;
	long y = CYMIN + Y*YBSIZ;

	for(LD=0; LD<ZLDLIM; ++LD)
	{
		long p_x = floor(((double)x+ZLDCO0[LD])/Dcr+0.5);
		long p_y = floor(((double)y+ZLDCO1[LD])/Dcr+0.5);
		fprintf(Pdif, " %ld %ld", p_x, p_y);
	}
	fprintf(Pdif, "}\r\n");
	__error__(Pdif, Pdif_Name);
}
void
PDIF_OUT_MAIN(long L_index, long Width)
{
	IEvaluater.__SetVT0LB0__();
	MaSCOT_IO.INIT_ILayer(L_index);
	CFragment = 0;
	Lb_PbasTO = 2;
	C_Layer = L_index;
	MaSCOT_IO.D_Link(L_index);
	SxTraverse = SxTraverse_1;
	fprintf(Pdif, "[Ly \"%s\"]\r\n", I_Layer[L_index].Name);
	while(CFragment!=LONG_MAX)
	{
		ZLBLIM = 1;
		ZLVLIM = 1;
		ZPTLIM = 1;
		ZVTLIM = 2;
		STATUS =+1;
		CleanZPBCOR(ZPBLIM+1);

		MaSCOT_IO.ZDREAD_P();
		if(ZLDLIM==0)
		{
			CFragment=I_Layer[L_index].Fragment;
			continue;
		}
		Check_Closure();
		Load_Insert_Master();
		POINT_VECTOR_ANALYSIS(2);
		__SI1__(0);/*Horizontal Vt must be available.*/
		__SI2__();
		PdifPoly_Level = 0;
		fprintf(Pdif, "{Poly\r\n{Polyap %ld}\r\n", Width);
		SXTraverse();
		fprintf(Pdif, "}\r\n");
	}
	MaSCOT_IO.D_BACKLink(L_index);
	MaSCOT_IO.Close_ILayer(L_index);
}
