/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#include <stdafx.h>

#define F_AREA_MODULE
#include<DOS.H>
#include<MATH.H>
#include<STDIO.H>
#include<STDLIB.H>
#include<CTYPE.H>
#include<STRING.H>
#include<LIMITS.H>
#include<PROCESS.H>
#include "fqeval.hpp"
#include "fxeval.hpp"
const char*f1area="f1area";
extern const char*OvrlpMstPrf;
double L_Area;
void
F_AREA_MAIN(const char* Name)
{
	#pragma warning(suppress : 4996)
	strcpy(MaSCOT_IO.I_Layer[0].Name, Name);
	MaSCOT_IO.ILayer_Cnt = 1;
	MaSCOT_IO.Activate_ILayers();
	MaSCOT_IO.INIT_ILayer(0);
MASLog("\n!!! <Area Counting> Started on %s !!!\n",Name);
	if(!(MaSCOT_IO.I_Layer[0]._lIdentity.layer_STATUS&1))
	{
		fputs(OvrlpMstPrf, stderr);
		zabort();
	}
	IEvaluater.__SetVT0LB0__();
	C_Fragment = 0;
	C_Layer = 0;
	L_Area=0;
	MaSCOT_IO.D_Link(0);
	while(C_Fragment!=LONG_MAX)
	{
		ZLBLIM = 1;
		ZLVLIM = 1;
		ZPTLIM = 1;
		ZVTLIM = 2;
		STATUS =+1;
		CleanZPTHASH();

		char open_shape = MaSCOT_IO.I_READ_P();
		if(ZLDLIM==0)
		{
			C_Fragment=MaSCOT_IO.I_Fragment(0);
			continue;
		}
		Loadmaster(1, open_shape);

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
	L_Area*=1.0/2.0;
	L_Area =floor(L_Area+0.5);
	L_Area/=1000.00; // in mm
	
	MaSCOT_IO.Close_ILayer(0);
MASLog("!!! Common Area on layer (%s) = %lf in mm\n", Name, L_Area);
	exit(0);
}

