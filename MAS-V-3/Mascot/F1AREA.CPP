#include "stdafx.h" 

#define F_AREA_MODULE
#include<DOS.H>
#include<MATH.H>
#include<STDIO.H>
#ifdef __BORLANDC__
#include<DIR.H>
#include<ALLOC.H>
#endif
#include<STDLIB.H>
#include<CTYPE.H>
#include<STRING.H>
#ifdef __BORLANDC__
#include<VALUES.H>
#endif
#include<LIMITS.H>
#include<PROCESS.H>
#include "fqeval.hpp"
#include "fxeval.hpp"
#include "m_alcr.hpp"
char*f1area="f1area";
extern char*OvrlpMstPrf;
extern void zabort(void);
extern FILE*Rniirm;
extern char*Nniirm;
double L_Area;
void
F_AREA_MAIN(char Name[20])
{
	memcpy(I_Layer[0].Name, Name, 20);
	ILayer_Cnt= 1;
	MaSCOT_IO.Find_ILayer();
	MaSCOT_IO.INIT_ILayer(0);
fprintf(stderr, "\n!!! <Area Counting> Started on %s !!!\n",Name);
fprintf(Rniirm, "\n!!! <Area Counting> Started on %s !!!\n",Name);
	if(!(I_Layer[0].LX_S.STATUS&1))
	{
		fputs(OvrlpMstPrf, stderr);
		fputs(OvrlpMstPrf, Rniirm);
		zabort();
	}
	IEvaluater.__SetVT0LB0__();
	CFragment = 0;
	C_Layer = 0;
	L_Area=0;
	MaSCOT_IO.D_Link(0);
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
			CFragment=I_Layer[0].Fragment;
			continue;
		}
		Loadmaster(1);

		if(ZLBLIM==1)continue;

		LvBead*Le=GetLbBead(1)->ZLBFirstLvBead;
		LvBead*Lv=Le;
		
		do
		{
			PtBead*P1=Lv->ZLVPoint1;
			PtBead*P2=Lv->ZLVPoint2;
			
			double X1=(double)P1->ZPTCO1;
			double X2=(double)P2->ZPTCO1;
			double Y1=(double)P1->ZPTCO2;
			double Y2=(double)P2->ZPTCO2;

			L_Area+=X2*Y1 - X1*Y2;
			Lv=Lv->ZLVNextLvBead;
		}
		while(Lv!=Le);
	}
	L_Area*=STSIZ/2;
	L_Area =floor(L_Area+0.5);
	L_Area/=1000.00; // in mm
	
	MaSCOT_IO.D_BACKLink(0);
	MaSCOT_IO.Close_ILayer(0);
fprintf(stderr, "!!! Common Area on layer (%s) = %lf in mm\n", Name, L_Area);
fprintf(Rniirm, "!!! Common Area on layer (%s) = %lf in mm\n", Name, L_Area);
	__close__(Rniirm,Nniirm);
	exit(0);
}

