/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#ifndef GridModule_H
#define GridModule_H

void FragmPolygons(long Layer);
void Reverse_Lv_Chain(LbBead*Lb); 
void INSERT_VECTORS(LbBead*Lb); 
void SPLIT_POLYGON(char);
char SET_Xg_Yg(long Lay);
void Destroy_GridStorage(void);
typedef void GvS(LvBead*Lv);
extern GvS * GvSplit;
void GvSplit_1(LvBead*LV);
void GvSplit_2(LvBead*LV);
struct GvBead
{
	GvBead*CO1;
	PtBead*CO2;
	PtBead*CO3;
}	;
extern GvBead**ZGaTBl;
extern long XgDIV; /*GRID-BLOCK-SIZE-UNITS-WIDE*/
extern long YgDIV; /*GRID-BLOCK-SIZE*/
extern long XgCNT; /*TOTAL-BLOCKS-IN-X-DIRECTION*/
extern long YgCNT; /*TOTAL-BLOCKS-IN-Y-DIRECTION*/
extern long XgCNT_2; /*TOTAL-BLOCKS-IN-X-DIRECTION*/
extern long YgCNT_2; /*TOTAL-BLOCKS-IN-Y-DIRECTION*/
extern long*ZXgCO2;
extern long*ZXgCO1;
extern long*ZXgCO0;
extern long*ZYgCO2;
extern long*ZYgCO1;
extern long*ZYgCO0;

#endif