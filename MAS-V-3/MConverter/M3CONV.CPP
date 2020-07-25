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
#define stricmp strcmpl
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

extern char YY_ITEM_FLAGS[64];

void YY_CLEAR_FLAGS(void);
void
yyoverflow
(char*, void*, long, void*, long, void*, long, long*);
void zabort(void);
void yyerror(char*);
/**/
void Terminate_Vertex(void);
void Terminate_Insert(void);
void Terminate_Poly_1(void);
/**/
extern long I_Str;

extern char*YY_1String; /*COMMON STRING*/
extern char*YY_2String; /*COMMON NAME*/
extern char*YY_3String; /*TABLE NAME*/
extern char*YY_4String; /*BLOCK-CELL NAME*/
extern char*YY_5String; /*LAYER NAME*/
extern char*YY_6String; /*TEXTUAL STRING*/
extern char*YY_7String; /*ATTRIBUTE TAG*/
/**/
extern long Bit_70_01;
extern long Bit_70_02;
extern long Bit_70_04;
extern long Bit_70_08;
extern long Bit_70_16;
extern long Bit_71_01;
extern long Bit_71_02;
extern long Bit_71_04;
extern long Bit_71_08;
extern long Bit_71_16;
/**/
extern double Item_10_Val;
extern double Item_11_Val;
extern double Item_12_Val;
extern double Item_13_Val;
extern double Item_20_Val;
extern double Item_21_Val;
extern double Item_22_Val;
extern double Item_23_Val;
extern double Item_30_Val;
extern double Item_31_Val;
extern double Item_38_Val;
extern double Item_39_Val;
extern double Item_40_Val;
extern double Item_41_Val;
extern double Item_42_Val;
extern double Item_43_Val;
extern double Item_50_Val;
extern double Item_51_Val;
extern double Item210_Val;
extern double Item220_Val;
extern double Item230_Val;
/**/
extern long Item_62_Val;
extern long Item_66_Val;
extern long Item_70_Val;
extern long Item_71_Val;
extern long Item_72_Val;
/**/
extern double ZZ_START_WIDTH;
extern double ZZ_ENDIN_WIDTH;
/**/
extern long VERLIM;
extern char Closed_Polygon;
extern char YY_Interpolate;
extern double MAX_WIDTH;
extern long YY_I_VAL;
extern double YY_F_VAL;
/**/
typedef short Z_YY_LEX(void);
extern Z_YY_LEX * yylex;
/**/
short DXFyylex(void);
short YY_INT_LEX(void);
short YY_FLOAT_LEX(void);
void Terminate_Circle(void); 

/*A Bison parser, made from m-conv.y*/

#include "m-conv.tab.c"

