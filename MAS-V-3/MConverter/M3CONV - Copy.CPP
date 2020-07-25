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

/*A Bison parser, made from m-conv.y*/

#define	EOS	258


typedef union { double d; long l; char c; } YYSTYPE;

#ifndef YYLTYPE
typedef
struct yyltype
	{
	short timestamp;
	short first_line;
	short first_column;
	short last_line;
	short last_column;
	char *text;
 }
yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __STDC__
#define const
#endif



#define	YYFINAL		589
#define	YYFLAG		-32768
#define	YYNTBASE	45

#define YYTRANSLATE(x) ((unsigned)(x) <= 258 ? yytranslate[x] : 164)

static const char yytranslate[] = {	 0,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	40,	 2,	 2,	 2,	 2,
	 2,	41,	42,	 2,	43,	 2,	 2,	 4,	28,	19,
	29,	30,	31,	32,	33,	34,	35,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	11,	12,	 9,	 7,	 5,	17,
	36,	20,	18,	37,	14,	13,	38,	 6,	16,	23,
	15,	21,	 8,	10,	26,	22,	25,	27,	24,	39,
	 2,	 2,	 2,	 2,	44,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2,
	 2,	 2,	 2,	 2,	 2,	 1,	 2,	 3
};

#ifndef YYDEBUG
#define YYDEBUG 0
#endif

#if YYDEBUG != 0
static const short yyrline[] = {	 0,
	13,	16,	18,	20,	22,	24,	24,	26,	26,	28,
	28,	30,	30,	33,	37,	38,	41,	44,	45,	46,
	47,	50,	53,	54,	57,	60,	62,	63,	66,	67,
	70,	71,	72,	73,	74,	75,	78,	79,	82,	85,
	87,	88,	91,	92,	95,	96,	99, 101, 103, 104,
 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
 118, 124, 130, 132, 134, 136, 138, 140, 146, 148,
 154, 160, 166, 172, 174, 176, 178, 180, 186, 192,
 198, 204, 206, 208, 210, 212, 218, 224, 226, 228,
 230, 232, 234, 236, 242, 248, 254, 260, 266, 272,
 274, 276, 278, 284, 290, 296, 302, 312, 322, 328,
 330, 332, 334, 336, 338, 340, 346, 352, 359, 360,
 361, 362, 363, 364, 365, 366, 367, 368, 369, 370,
 371, 372, 373, 374, 375, 376, 377, 378, 379, 380,
 381, 382, 383, 384, 385, 386, 387, 388, 389, 390,
 391, 392, 393, 394, 395, 396, 397, 398, 399, 400,
 401, 402, 403, 404, 405, 406, 407, 408, 409, 410,
 411, 412, 413, 414, 415, 416, 419, 420, 422, 424,
 426, 428, 430, 432, 434, 436, 439, 441, 444, 445,
 448, 449, 452, 455, 457, 461, 462, 465, 474, 478,
 478, 481, 481, 484, 485, 486, 487, 488, 489, 490,
 491, 492, 493, 496, 497, 498, 499, 500, 501, 502,
 503, 504, 505, 506, 507, 508, 509, 510, 511, 512,
 513, 514, 515, 516, 517, 518, 519, 520, 521, 522,
 523, 524, 525, 526, 529, 530, 531, 531, 534, 534,
 537, 538
};

static const char * const yytname[] = {	 0,
"error","$illegal.","EOS","'0'","'E'","'N'","'D'","'S'","'C'","'T'",
"'A'","'B'","'L'","'K'","'Q'","'O'","'F'","'I'","'2'","'H'",
"'R'","'V'","'P'","'Y'","'W'","'U'","'X'","'1'","'3'","'4'",
"'5'","'6'","'7'","'8'","'9'","'G'","'J'","'M'","'Z'","'$'",
"'*'","'+'","'-'","'_'","YY_0EOS"
};
#endif

static const short yyr1[] = {	 0,
	45,	46,	47,	48,	49,	50,	50,	51,	51,	52,
	52,	53,	53,	54,	55,	55,	56,	57,	57,	57,
	57,	58,	59,	59,	61,	60,	62,	62,	63,	63,
	64,	64,	64,	64,	64,	64,	65,	65,	67,	66,
	68,	68,	69,	69,	70,	70,	72,	71,	73,	73,
	74,	74,	74,	74,	74,	74,	74,	74,	74,	74,
	75,	76,	77,	78,	79,	80,	81,	82,	83,	84,
	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,
	95,	96,	97,	98,	99, 100, 101, 102, 103, 104,
 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
 115, 116, 117, 118, 119, 120, 121, 122, 123, 124,
 125, 126, 127, 128, 129, 130, 131, 132, 133, 133,
 133, 133, 133, 133, 133, 133, 133, 133, 133, 133,
 133, 133, 133, 133, 133, 133, 133, 133, 133, 133,
 133, 133, 133, 133, 133, 133, 133, 133, 133, 133,
 133, 133, 133, 133, 133, 133, 133, 133, 133, 133,
 133, 133, 133, 133, 133, 133, 133, 133, 133, 133,
 133, 133, 133, 133, 133, 133, 134, 134, 135, 136,
 137, 138, 139, 140, 141, 142, 144, 143, 145, 145,
 146, 146, 148, 147, 149, 150, 150, 152, 151, 154,
 153, 156, 155, 157, 157, 157, 157, 157, 157, 157,
 157, 157, 157, 158, 158, 158, 158, 158, 158, 158,
 158, 158, 158, 158, 158, 158, 158, 158, 158, 158,
 158, 158, 158, 158, 158, 158, 158, 158, 158, 158,
 158, 158, 158, 158, 160, 159, 161, 159, 162, 162,
 163, 163
};

static const short yyr2[] = {	 0,
	 2,	 8,	 8,	 8,	 8,	 1,	 2,	 1,	 2,	 1,
	 2,	 1,	 2,	 5,	 1,	 2,	12,	 9,	 9,	 9,
	11,	 1,	 1,	 2,	 0,	 9,	 2,	 3,	 2,	 3,
	 7,	 7,	 7,	 7,	 6,	 5,	 1,	 2,	 0,	 9,
	 2,	 3,	 2,	 3,	 1,	 8,	 0,	 2,	 2,	 3,
	10,	 6,	 7,	 8,	 6,	 7,	 7,	 5,	 8,	 7,
	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 4,
	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,
	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,
	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,
	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,	 4,
	 4,	 4,	 4,	 4,	 4,	 5,	 5,	 5,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 2,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 0,	 2,	 2,	 3,
	 1,	 2,	 0,	10,	 9,	 1,	 2,	 0,	 4,	 0,
	 2,	 0,	 2,	 1,	 1,	 1,	 1,	 1,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,
	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,
	 1,	 1,	 1,	 1,	 0,	 2,	 0,	 3,	 1,	 2,
	 1,	 1
};

static const short yydefact[] = {	 0,
	 0,	 0,	 0,	15,	 1,	 0,	 0,	16,	 0,	 0,
	 0,	 0,	 0,	14,	 0,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	 0,	 0,	17,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	23,	 0,	 0,	37,	 0,	 0,	 0,	 0,
	 0,	 0,	 0,	 0,	 0,	 0, 119, 120, 121, 122,
 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
 173, 174, 175, 176, 177,	22,	 0,	 0,	 0,	 6,
	18,	24,	 0,	 0,	20,	38, 247,	 0,	 0,	 0,
	 0,	 0,	 0,	 0,	 0, 247,	 0,	 0,	 0,	 0,
	 0,	 0,	 0,	 0, 247,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	 0,	 0,	 0, 247,	 0,	 0,	 0,	 0,
	 0,	 0,	 0, 247,	 0,	 0, 247,	 0,	 0, 247,
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, 247,
 247,	 0,	19, 178,	 0,	 0,	 0,	 0,	 7,	 0,
	62,	 0,	 0, 200, 200,	 0, 200,	 0, 200,	 0,
 200, 200, 200, 200,	61, 200, 200, 200, 200, 200,
 200, 200, 200,	63, 200, 200, 200, 200, 200, 200,
 200, 200, 200, 200,	64, 200, 200, 200, 200, 200,
 200, 200,	65, 200, 200,	66, 202, 202,	67, 202,
 202, 202, 202, 202, 202, 202, 202, 202,	68,	69,
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	49,	 0,	21,
	 0,	 0,	 0, 246, 204, 218, 227, 217, 232, 216,
 233, 214, 215, 225, 224, 230, 228, 219, 222, 206,
 221, 231, 235, 229, 238, 236, 234, 237, 205, 207,
 208, 209, 210, 211, 212, 213, 220, 223, 226, 239,
 240, 241, 242, 243, 244, 252, 251,	 0, 249,	78,
	 0,	80, 200,	79, 200,	81, 200,	82,	83,	84,
	85,	70,	72,	71,	73,	74,	75,	76,	77,	86,
	88,	87,	89,	90,	91,	92,	93,	94,	95,	96,
	98,	97,	99, 100, 101, 102, 103, 104, 105,	 0,
 106, 107, 109, 108, 110, 111, 112, 113, 114, 115,
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	50,
	 0,	 0,	 0, 248, 250, 201, 117, 116, 118, 203,
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	 0,	25,	 0,	39,	 0,	 0,	 0,	 0,
	 0, 182,	58,	 0,	 0,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	 0, 185,	55,	 0, 179,	52,	 0,	 0,
	 0,	26,	 0,	 2,	40,	 0, 183,	57, 184,	60,
	 0, 186,	56, 187,	 0, 180,	53,	 0,	 8,	27,
	 0,	 0,	10,	41,	 0, 181,	54,	59,	 0,	 0,
	 0,	 0,	 0,	 0,	 0,	29,	 9,	 0,	28,	 0,
	 0,	43,	45,	11,	 0,	42,	 0, 188,	 0,	 0,
	 0,	 0,	 0,	 0,	 0,	 0,	30,	 0,	 0,	44,
	 0,	12, 189,	 0, 191, 198,	51,	 0,	 0,	 0,
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	13, 190,
 192,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	 0,	 0, 196,	 0,	 0,	 0,	 0,	 0,
	 0,	 0,	36,	 0,	 0,	 0,	 0,	 0,	 0, 199,
 197,	 0,	 0,	 0,	 0,	35,	 0,	 0,	 0,	 0,
	 0,	 0,	 3,	34,	32,	33,	31,	 4,	47,	 0,
	 0,	 0,	46,	 0,	 0,	 0,	 0,	48,	 5, 193,
	 0,	 0,	 0, 194,	 0, 195,	 0,	 0,	 0
};

static const short yydefgoto[] = { 501,
 130, 459, 463, 502, 131, 460, 464, 503, 587,	 3,
	 4,	25,	66,	52,	53, 428, 442, 461, 476,	55,
	56, 430, 445, 465, 482, 573, 574, 196, 483,	67,
	68,	69,	70,	71,	72,	73,	74,	75,	76,	77,
	78,	79,	80,	81,	82,	83,	84,	85,	86,	87,
	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,
	98,	99, 100, 101, 102, 103, 104, 105, 106, 107,
 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
 118, 119, 120, 121, 122, 123, 124, 125, 126, 438,
 457, 467, 423, 448, 450, 435, 453, 468, 469, 488,
 504, 505, 582, 535, 536, 507, 522, 320, 321, 359,
 360, 316, 317, 201, 202, 203, 318, 319
};

static const short yypact[] = {	 5,
	32,	33,	 5,-32768,-32768,	55,	51,-32768,	59,	58,
	66,	73, 114,-32768, 103, 128, 133, 118, 135, 108,
 134, 131, 126, 139,-32768, 138, 146, 144, 148, 145,
 149, 152, 157, 162, 170, 153, 171, 161, 172, 173,
 163, 177, 182, 183, 184, 180,	 5,	 5, 333, 187,
 185,	 5,-32768, 268,	 5,-32768, 350, 356, 122,	35,
	 4,	26, 279, 189, 191,	 5,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768, 333,	 5, 277,	39, 262,
-32768,-32768, 284,	41,-32768,-32768, 298, 299,	15, 112,
 120, 300, 301, 311, 312, 298, 316, 317, 319, 320,
 321, 322, 323, 325, 298, 326, 332, 334, 336, 346,
 348, 352, 353, 354, 355, 298, 367, 369, 370, 371,
 373, 389, 399, 298, 400, 401, 298, 402, 403, 298,
 404, 405, 406, 407, 408, 409, 410, 411, 412, 298,
 298, 413,-32768,-32768, 282,	 5, 304, 414,-32768, 415,
-32768, 416, 235,-32768,-32768, 418,-32768, 419,-32768, 420,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
	 1, 398,	 9, 396, 417, 422, 421,-32768, 160,-32768,
 423, 425, 424,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768, 194,-32768,-32768,
 426,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768, 427,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
 428, 431, 432, 397, 433, 429, 439, 444,	44,-32768,
 434, 446, 441,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
 438, 435, 448, 437, 450, 457, 458, 459, 442, 461,
 466, 467, 468, 469, 470, 460, 471, 472, 333, 475,
 462, 473, 474,-32768, 476,-32768, 477, 478, 482, 333,
 479, 333,-32768, 333, 480, 481, 485, 333, 486, 333,
 333, 333, 488, 333,-32768, 333, 333,-32768, 489, 487,
 333,-32768, 313,-32768,-32768, 313, 333,-32768, 333,-32768,
 333, 333,-32768,-32768, 490, 333,-32768, 109, 262,-32768,
	 5, 276, 262,-32768,	 5, 333,-32768,-32768, 333, 491,
 492, 493, 119,	49, 495,-32768,-32768, 109,-32768, 494,
	21,-32768,-32768,-32768, 276,-32768, 313,-32768, 333, 498,
 455, 483, 484, 496, 497, 501,-32768, 499, 500,-32768,
 101, 262,-32768,	 5,-32768, 333,-32768, 502, 503, 504,
 506, 505, 507, 511, 508, 510, 513, 509,-32768,-32768,
-32768,	 5, 512, 516, 519, 514, 522, 521, 333, 520,
 524, 517, 527, 518,-32768,	 5, 526, 523, 531, 536,
 333, 538, 333, 528, 529, 539, 530, 540,	 3,-32768,
-32768, 544, 333, 333, 333, 333, 333, 545, 546, 537,
 532, 533,-32768, 333, 333, 333, 333,-32768,-32768, 548,
 541, 542,-32768, 333, 553, 554, 555, 333,-32768,-32768,
 534, 333, 556, 333, 333, 333, 515, 558,-32768
};

static const short yypgoto[] = {	 0,
-32768,-32768,-32768,-32768, -53, -36, -39,-500,-32768,-32768,
 559,-32768,-32768,-32768, 382,-32768,-32768,-32768,	18,-32768,
 447,-32768,-32768,-32768,	12,-32768,-32768,-32768,-168,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-125,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-116,-408,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,	60,-32768,	27,-32768,-32768,-32768,-134,-32768, 143,
-32768,-32768,-32768,-140,-32768,-32768,-32768, 247
};


#define	YYLAST		565


static const short yytable[] = {	 2,
 422, 135,	 7, 520, 199, 215, 174, 175,	 1, 194,
 517, 434, 193, 374, 224, 437, 371, 205, 206, 443,
 372, 446, 447, 449, 548, 235, 268, 452, 177, 375,
 499, 176, 456, 243,	 5, 550, 246, 166, 167, 249,
	 6, 376, 466, 198, 178, 198,	51,	54, 128, 259,
 260, 129, 133, 168, 134,	10, 399, 179,	 6,	 9,
 487, 400, 169, 170, 171, 192, 494,	11, 172, 173,
 322, 495, 324,	12, 326,	13, 328, 329, 330, 331,
 506, 332, 333, 334, 335, 336, 337, 338, 339,	14,
 340, 341, 342, 343, 344, 345, 346, 347, 348, 349,
 380, 350, 351, 352, 353, 354, 355, 356, 517, 357,
 358, 518,	21, 471, 207, 208, 472,	22,	16,	23,
 543, 473, 209, 210, 155, 156, 195,	24, 492, 493,
 474,	15, 556,	17, 475,	18,	19,	20,	28,	26,
 157,	27, 270,	29, 564, 565, 566,	30, 567, 158,
 159, 160, 161, 162, 163, 164, 165,	31,	33,	32,
	36,	35,	34,	37, 198, 578,	40, 261, 262, 263,
 264,	38, 265, 584,	39,	41, 586, 266,	42,	43,
	44,	46, 267,	45,	47,	48,	49,	50, 387, 127,
 388, 190, 389, 191, 128, 269, 384, 275, 276, 277,
 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
 288, 289, 290, 291, 292, 293, 294, 295, 296, 297,
 298, 299, 300, 301, 302, 303, 304, 305, 306, 307,
 308, 309, 310, 311, 312, 313, 314, 315, 275, 276,
 277, 278, 279, 280, 281, 282, 283, 284, 285, 286,
 287, 288, 289, 290, 291, 292, 293, 294, 295, 296,
 297, 298, 299, 300, 301, 302, 303, 304, 305, 306,
 307, 308, 309, 310, 311, 312, 313, 314, 315, 133,
 480, 180, 181, 261, 262, 263, 481, 197, 265, 261,
 262, 263, 264, 266, 265,	64, 200, 182, 267, 266,
-245, 204, 211, 212, 267, 194, 183, 184, 185, 186,
 187, 188, 189, 213, 214, 271,	 1, 194, 216, 217,
 194, 218, 219, 220, 221, 222, 194, 223, 225, 194,
 194,	57, 194, 477, 226, 194, 227, 484, 228, 194,
	58,	59,	60,	61,	62,	63,	64,	65, 229, 194,
 230,	57, 137, 138, 231, 232, 233, 234, 146, 147,
	58,	59,	60,	61,	62,	63,	64,	65, 139, 236,
 194, 237, 238, 239, 148, 240, 519, 140, 141, 142,
 143, 144, 145, 149, 150, 151, 152, 153, 154, 194,
 361, 241, 362, 363, 364, 365, 366, 367, 368, 369,
 370, 242, 244, 245, 247, 248, 250, 251, 252, 253,
 254, 255, 256, 257, 258, 373, 376, 198, 274, 272,
 323, 325, 327, 394, 479, 486, 194, 378, 386, 390,
 273, 382, 383, 132, 377, 381, 379, 396, 401, 194,
 391, 392, 458, 395, 397, 462, 407, 194, 194, 194,
 194, 398, 393, 402, 403, 404, 406, 405, 408, 409,
 478, 194, 410, 411, 485, 412, 413, 194, 414, 194,
 416, 415, 419, 420, 418, 417, 421, 424, 509, 431,
 432, 436, 425, 427, 429, 426, 433, 441, 444, 439,
 451, 454, 455, 489, 470, 497, 500, 490, 440, 498,
 512, 136, 491, 496, 508, 515, 510, 511, 514, 516,
 526, 523, 513, 529, 588, 524, 531, 532, 533, 530,
 538, 534, 537, 539, 541, 553, 525, 528, 545, 527,
 542, 546, 544, 554, 540, 549, 547, 552, 555, 548,
 557, 558, 570, 560, 562, 559, 563, 568, 569, 571,
 561, 577, 576, 572, 575, 579, 580, 589, 585, 581,
 583,	 8, 551, 521, 385
};

static const short yycheck[] = {	 0,
 409,	55,	 3, 504, 130, 146,	 3,	 4,	 4, 126,
	 8, 420,	66,	 5, 155, 424,	16,	 3,	 4, 428,
	20, 430, 431, 432,	22, 166, 195, 436,	 3,	21,
	10,	28, 441, 174,	 3, 536, 177,	 3,	 4, 180,
	 8,	21, 451,	 5,	19,	 5,	47,	48,	10, 190,
 191,	52,	12,	19,	55,	 5,	13,	32,	 8,	 5,
 469,	18,	28,	29,	30,	66,	18,	 9,	34,	35,
 205,	23, 207,	16, 209,	10, 211, 212, 213, 214,
 489, 216, 217, 218, 219, 220, 221, 222, 223,	17,
 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
 269, 236, 237, 238, 239, 240, 241, 242,	 8, 244,
 245,	11,	 5,	 5,	 3,	 4,	 8,	10,	16,	12,
 529,	13,	 3,	 4,	 3,	 4, 127,	20,	10,	11,
	22,	18, 541,	 6,	26,	 3,	19,	 3,	13,	 6,
	19,	11, 196,	 5, 553, 554, 555,	10, 557,	28,
	29,	30,	31,	32,	33,	34,	35,	12,	11,	16,
	 9,	13,	18,	 7,	 5, 574,	14,	 8,	 9,	10,
	11,	10,	13, 582,	 5,	 5, 585,	18,	18,	 8,
	 8,	 5,	23,	21,	 3,	 3,	 3,	 8, 323,	 3,
 325,	 3, 327,	 3,	10, 196,	 3,	 4,	 5,	 6,
	 7,	 8,	 9,	10,	11,	12,	13,	14,	15,	16,
	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,
	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,
	37,	38,	39,	40,	41,	42,	43,	44,	 4,	 5,
	 6,	 7,	 8,	 9,	10,	11,	12,	13,	14,	15,
	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,
	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,
	36,	37,	38,	39,	40,	41,	42,	43,	44,	12,
	 5,	 3,	 4,	 8,	 9,	10,	11,	11,	13,	 8,
	 9,	10,	11,	18,	13,	34,	13,	19,	23,	18,
	 3,	 3,	 3,	 3,	23, 422,	28,	29,	30,	31,
	32,	33,	34,	 3,	 3,	12,	 4, 434,	 3,	 3,
 437,	 3,	 3,	 3,	 3,	 3, 443,	 3,	 3, 446,
 447,	19, 449, 459,	 3, 452,	 3, 463,	 3, 456,
	28,	29,	30,	31,	32,	33,	34,	35,	 3, 466,
	 3,	19,	 3,	 4,	 3,	 3,	 3,	 3,	 3,	 4,
	28,	29,	30,	31,	32,	33,	34,	35,	19,	 3,
 487,	 3,	 3,	 3,	19,	 3, 502,	28,	29,	30,
	31,	32,	33,	28,	29,	30,	31,	32,	33, 506,
 248,	 3, 250, 251, 252, 253, 254, 255, 256, 257,
 258,	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 3,
	 3,	 3,	 3,	 3,	 3,	18,	21,	 5,	 3,	 6,
	 3,	 3,	 3,	27, 461, 465, 543,	 6,	 3,	 3,
	16,	 7,	 9,	52,	18,	13,	16,	 9,	 5, 556,
	13,	11, 443,	11,	 6, 446,	10, 564, 565, 566,
 567,	 8,	21,	 8,	14,	18,	 9,	23,	 9,	 3,
 461, 578,	 5,	 5, 465,	24,	 6, 584,	 3, 586,
	 3,	 5,	13,	 3,	 5,	 7,	 5,	 3,	24,	 3,
	 3,	 3,	21,	10,	 9,	13,	 5,	 3,	 3,	10,
	 3,	 3,	 6,	 3,	 5, 478, 485,	 6,	18,	 6,
	 5,	55,	10,	 9,	 7,	 7,	24,	24,	 8,	10,
	 5,	10,	16,	 3,	 0,	13,	 7,	 5,	10,	12,
	 5, 522,	11,	 5,	 3,	 3,	23,	21,	 5,	25,
	10,	15,	13,	 3,	21, 536,	10,	12,	 3,	22,
	 3,	14,	 6,	 5,	 5,	17,	 3,	 3,	 3,	18,
	21,	10,	12,	21,	 7,	 3,	 3,	 0,	 3,	 5,
	27,	 3, 536, 504, 318
};
#define YYPURE 1

/* -*-C-*-Note some compilers choke on comments on `#line' lines.*/


/* Skeleton output parser for bison,
 Copyright (C) 1984 Bob Corbett and Richard Stallman

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 1, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.*/


#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__)
#include <alloca.h>
#endif

/* This is the parser code that is written into each bison parser
when the %semantic_parser declaration is not specified in the grammar.
It was written by Richard Stallman by simplifying the hairy parser
used when %semantic_parser is specified.*/

/* Note: there must be only one dollar sign in this file.
 It is replaced by the list of actions, each action
 as one case of the switch.*/

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYFAIL		goto yyerrlab;
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYIMPURE
#define YYLEX		yylex()
#endif

#ifndef YYPURE
#define YYLEX		yylex(&yylval, &yylloc)
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYIMPURE

int	yychar;			/*the lookahead symbol		*/
YYSTYPE	yylval;			/*the semantic value of the		*/
				/*lookahead symbol			*/

YYLTYPE yylloc;			/*location data for the lookahead	*/
				/*symbol				*/

short yynerrs;			/*number of parse errors so far	 */
#endif/* YYIMPURE */

#ifndef YYDEBUG
#define YYDEBUG 0
#endif

#if YYDEBUG != 0
short yydebug;			/*nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
 from coexisting.*/
#endif

/*YYMAXDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYMAXDEPTH
#define YYMAXDEPTH 200
#endif

/*YYMAXLIMIT is the maximum size the stacks can grow to
	(effective only if the built-in stack extension method is used).*/

#ifndef YYMAXLIMIT
#define YYMAXLIMIT 10000
#endif

#ifndef YYLSP_NEEDED
#define YYLSP_NEEDED 0
#endif


int
yyparse()
{
register short yystate;
register short yyn;
register short *yyssp;
register YYSTYPE *yyvsp;
YYLTYPE *yylsp;
short yyerrstatus;	/*number of tokens to shift before error messages enabled */
short yychar1;		/*lookahead token as an internal (translated) token number */

short *yyss = NULL;		/*refer to the stacks thru separate pointers */
YYSTYPE *yyvs = NULL;	/*to allow yyoverflow to reallocate them elsewhere */
YYLTYPE *yyls = NULL;

short yymaxdepth = YYMAXDEPTH;

#ifndef YYPURE
short yychar;
YYSTYPE yylval;
YYLTYPE yylloc;
short yynerrs;
#endif

YYSTYPE yyval;		/*the variable used to return		*/
				/*semantic values from the action	*/
				/*routines				*/

short yylen;

#if YYDEBUG != 0
if (yydebug)
	fprintf(stderr, "Starting parse\n");
#endif

	yyss = (short *) malloc (yymaxdepth * sizeof (*yyssp));
	yyls = (YYLTYPE *) malloc (yymaxdepth * sizeof (*yylsp));
	yyvs = (YYSTYPE *) malloc (yymaxdepth * sizeof (*yyvsp));
	if(!yyss||!yyls||!yyvs)zabort();

yystate = 0;
yyerrstatus = 0;
yynerrs = 0;
yychar = YYEMPTY;		/* Cause a token to be read.*/

/* Initialize stack pointers.
	 Waste one element of value and location stack
	 so that they stay on the same level as the state stack.*/

yyssp = yyss - 1;
yyvsp = yyvs;
yylsp = yyls;

/* Push a new state, which is found inyystate.*/
/* In all cases, when you get here, the value and location stacks
 have just been pushed. so pushing a state here evens the stacks.*/
yynewstate:

*++yyssp = yystate;

if (yyssp >= yyss + yymaxdepth - 1)
	{
	/* Give user a chance to reallocate the stack */
	/* Use copies of these so that the &'s don't force the real ones into memory. */
	YYSTYPE *yyvs1 = yyvs;
	YYLTYPE *yyls1 = yyls;
	short *yyss1 = yyss;

	/* Get the current used size of the three stacks, in elements.*/
	short size = yyssp - yyss + 1;

#ifdef yyoverflow
	/* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.*/
	yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yymaxdepth);

	yyss = yyss1; yyvs = yyvs1; yyls = yyls1;
#else /* no yyoverflow */
	/* Extend the stack our own way.*/
	if (yymaxdepth >= YYMAXLIMIT)
	yyerror("parser stack overflow");
	yymaxdepth *= 2;
	if (yymaxdepth > YYMAXLIMIT)
	yymaxdepth = YYMAXLIMIT;
	yyss = (short *) malloc (yymaxdepth * sizeof (*yyssp));
	memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
	free (yyss1);
	yyls = (YYLTYPE *) malloc (yymaxdepth * sizeof (*yylsp));
	memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
	free (yyls1);
#ifdef YYLSP_NEEDED
	yyvs = (YYSTYPE *) malloc (yymaxdepth * sizeof (*yyvsp));
	memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
	free (yyvs1);
#endif
#endif /* no yyoverflow */

	yyssp = yyss + size - 1;
	yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
	yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
	if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yymaxdepth);
#endif

	if (yyssp >= yyss + yymaxdepth - 1)
	YYABORT;
	}

#if YYDEBUG != 0
if (yydebug)
	fprintf(stderr, "Entering state %d\n", yystate);
#endif

/* Do appropriate processing given the current state.*/
/* Read a lookahead token if we need one and don't already have one.*/
yyresume:

/* First try to decide what to do without reference to lookahead token.*/

yyn = yypact[yystate];
if (yyn == YYFLAG)
	goto yydefault;

/* Not known => get a lookahead token if don't already have one.*/

/* yychar is either YYEMPTY or YYEOF
	 or a valid token in external form.*/

if (yychar == YYEMPTY)
	{
#if YYDEBUG != 0
	if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
	yychar = YYLEX;
	}

/* Convert token to internal form (in yychar1) for indexing tables with */

if (yychar <= 0)		/* This means end of input. */
	{
	yychar1 = 0;
	yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
	if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
	}
else
	{
	yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
	if (yydebug)
	fprintf(stderr, "Next token is %d (%s)\n", yychar, yytname[yychar1]);
#endif
	}

yyn += yychar1;
if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
	goto yydefault;

yyn = yytable[yyn];

/* yyn is what to do for this token type in this state.
	 Negative => reduce, -yyn is rule number.
	 Positive => shift, yyn is new state.
	 New state is final state => don't bother to shift,
	 just return success.
	 0, or most negative number => error.*/

if (yyn < 0)
	{
	if (yyn == YYFLAG)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
	}
else if (yyn == 0)
	goto yyerrlab;

if (yyn == YYFINAL)
	YYACCEPT;

/* Shift the lookahead token.*/

#if YYDEBUG != 0
if (yydebug)
	fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

/* Discard the token being shifted unless it is eof.*/
if (yychar != YYEOF)
	yychar = YYEMPTY;

*++yyvsp = yylval;
#ifdef YYLSP_NEEDED
*++yylsp = yylloc;
#endif

/* count tokens shifted since error; after three, turn off error status.*/
if (yyerrstatus) yyerrstatus--;

yystate = yyn;
goto yynewstate;

/* Do the default action for the current state.*/
yydefault:

yyn = yydefact[yystate];
if (yyn == 0)
	goto yyerrlab;

/* Do a reduction.yyn is the number of a rule to reduce with.*/
yyreduce:
yylen = yyr2[yyn];
yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
if (yydebug)
	{
	if (yylen == 1)
	fprintf (stderr, "Reducing 1 value via line %d, ",
		 yyrline[yyn]);
	else
	fprintf (stderr, "Reducing %d values via line %d, ",
		 yylen, yyrline[yyn]);
	}
#endif


switch (yyn) {

case 1:
{ YY_CLEAR_FLAGS(); ;
	break;}
case 14:
{ return(0) ;
	break;}
case 25:
{ YY_2String = YY_3String; ;
	break;}
case 39:
{ YY_2String = YY_4String; ;
	break;}
case 47:
{ YY_2String = YY_7String; ;
	break;}
case 61:
{
YY_01_FLAG = 1;
strncpy(YY_6String, YY_1String, 30);
;
	break;}
case 62:
{
YY_02_FLAG = 1;
strncpy(YY_2String, YY_1String, 30);
;
	break;}
case 68:
{
YY_08_FLAG = 1;
strncpy(YY_5String, YY_1String, 30);
;
	break;}
case 70:
{
YY_10_FLAG = 1;
Item_10_Val = yyvsp[0].d;
;
	break;}
case 71:
{
YY_11_FLAG = 1;
Item_11_Val = yyvsp[0].d;
;
	break;}
case 72:
{
YY_12_FLAG = 1;
Item_12_Val = yyvsp[0].d;
;
	break;}
case 73:
{
YY_13_FLAG = 1;
Item_13_Val = yyvsp[0].d;
;
	break;}
case 78:
{
YY_20_FLAG = 1;
Item_20_Val = yyvsp[0].d;
;
	break;}
case 79:
{
YY_21_FLAG = 1;
Item_21_Val = yyvsp[0].d;
;
	break;}
case 80:
{
YY_22_FLAG = 1;
Item_22_Val = yyvsp[0].d;
;
	break;}
case 81:
{
YY_23_FLAG = 1;
Item_23_Val = yyvsp[0].d;
;
	break;}
case 86:
{
YY_30_FLAG = 1;
Item_30_Val = yyvsp[0].d;
;
	break;}
case 87:
{
YY_31_FLAG = 1;
Item_31_Val = yyvsp[0].d;
;
	break;}
case 94:
{
YY_38_FLAG = 1;
Item_38_Val = yyvsp[0].d;
;
	break;}
case 95:
{
YY_39_FLAG = 1;
Item_39_Val = yyvsp[0].d;
;
	break;}
case 96:
{
YY_40_FLAG = 1;
Item_40_Val = yyvsp[0].d;
;
	break;}
case 97:
{
YY_41_FLAG = 1;
Item_41_Val = yyvsp[0].d;
;
	break;}
case 98:
{
YY_42_FLAG = 1;
Item_42_Val = yyvsp[0].d;
;
	break;}
case 99:
{
YY_43_FLAG = 1;
Item_43_Val = yyvsp[0].d;
;
	break;}
case 103:
{
YY_50_FLAG = 1;
Item_50_Val = yyvsp[0].d;
;
	break;}
case 104:
{
YY_51_FLAG = 1;
Item_51_Val = yyvsp[0].d;
;
	break;}
case 105:
{
YY_62_FLAG = 1;
Item_62_Val = yyvsp[0].l;
;
	break;}
case 106:
{
YY_66_FLAG = 1;
Item_66_Val = yyvsp[0].l;
;
	break;}
case 107:
{
YY_70_FLAG = 1;
Bit_70_01 = yyvsp[0].l & 01;
Bit_70_02 = yyvsp[0].l & 02;
Bit_70_04 = yyvsp[0].l & 04;
Bit_70_08 = yyvsp[0].l & 8L;
Bit_70_16 = yyvsp[0].l & 16;
;
	break;}
case 108:
{
YY_71_FLAG = 1;
Bit_71_01 = yyvsp[0].l & 01;
Bit_71_02 = yyvsp[0].l & 02;
Bit_71_04 = yyvsp[0].l & 04;
Bit_71_08 = yyvsp[0].l & 8L;
Bit_71_16 = yyvsp[0].l & 16;
;
	break;}
case 109:
{
YY_72_FLAG = 1;
Item_72_Val = yyvsp[0].l;
;
	break;}
case 116:
{
YY210_FLAG = 1;
Item210_Val = yyvsp[0].d;
;
	break;}
case 117:
{
YY220_FLAG = 1;
Item220_Val = yyvsp[0].d;
;
	break;}
case 118:
{
YY230_FLAG = 1;
Item230_Val = yyvsp[0].d;
;
	break;}
case 187:
{ YY_2String = YY_4String; ;
	break;}
case 188:
{ Terminate_Insert(); ;
	break;}
case 193:
{ YY_2String = YY_7String; ;
	break;}
case 195:
{ Terminate_Vertex(); ;
	break;}
case 198:
{
VERLIM = 1;

Closed_Polygon = YY_70_FLAG ? Bit_70_01 :0;

ZZ_START_WIDTH = YY_40_FLAG ? Item_40_Val :0;
ZZ_ENDIN_WIDTH = YY_41_FLAG ? Item_41_Val :0;

YY_Interpolate = 0;

MAX_WIDTH = ZZ_START_WIDTH;
if(MAX_WIDTH<ZZ_ENDIN_WIDTH)
MAX_WIDTH = ZZ_ENDIN_WIDTH;
;
	break;}
case 199:
{ Terminate_Poly_1(); ;
	break;}
case 200:
{ yylex = YY_FLOAT_LEX; ;
	break;}
case 201:
{ yylex = DXFyylex; yyval.d = YY_F_VAL; ;
	break;}
case 202:
{ yylex = YY_INT_LEX; ;
	break;}
case 203:
{ yylex = DXFyylex; yyval.l = YY_I_VAL; ;
	break;}
case 204:
{ yyval.c='0' ;
	break;}
case 205:
{ yyval.c='1' ;
	break;}
case 206:
{ yyval.c='2' ;
	break;}
case 207:
{ yyval.c='3' ;
	break;}
case 208:
{ yyval.c='4' ;
	break;}
case 209:
{ yyval.c='5' ;
	break;}
case 210:
{ yyval.c='6' ;
	break;}
case 211:
{ yyval.c='7' ;
	break;}
case 212:
{ yyval.c='8' ;
	break;}
case 213:
{ yyval.c='9' ;
	break;}
case 214:
{ yyval.c='A' ;
	break;}
case 215:
{ yyval.c='B' ;
	break;}
case 216:
{ yyval.c='C' ;
	break;}
case 217:
{ yyval.c='D' ;
	break;}
case 218:
{ yyval.c='E' ;
	break;}
case 219:
{ yyval.c='F' ;
	break;}
case 220:
{ yyval.c='G' ;
	break;}
case 221:
{ yyval.c='H' ;
	break;}
case 222:
{ yyval.c='I' ;
	break;}
case 223:
{ yyval.c='J' ;
	break;}
case 224:
{ yyval.c='K' ;
	break;}
case 225:
{ yyval.c='L' ;
	break;}
case 226:
{ yyval.c='M' ;
	break;}
case 227:
{ yyval.c='N' ;
	break;}
case 228:
{ yyval.c='O' ;
	break;}
case 229:
{ yyval.c='P' ;
	break;}
case 230:
{ yyval.c='Q' ;
	break;}
case 231:
{ yyval.c='R' ;
	break;}
case 232:
{ yyval.c='S' ;
	break;}
case 233:
{ yyval.c='T' ;
	break;}
case 234:
{ yyval.c='U' ;
	break;}
case 235:
{ yyval.c='V' ;
	break;}
case 236:
{ yyval.c='W' ;
	break;}
case 237:
{ yyval.c='X' ;
	break;}
case 238:
{ yyval.c='Y' ;
	break;}
case 239:
{ yyval.c='Z' ;
	break;}
case 240:
{ yyval.c='$' ;
	break;}
case 241:
{ yyval.c='*' ;
	break;}
case 242:
{ yyval.c='+' ;
	break;}
case 243:
{ yyval.c='-' ;
	break;}
case 244:
{ yyval.c='_' ;
	break;}
case 245:
{ I_Str = 0; ;
	break;}
case 246:
{ YY_1String[I_Str++] = 0; ;
	break;}
case 247:
{ I_Str = 0; ;
	break;}
case 248:
{ YY_1String[I_Str++] = 0; ;
	break;}
case 251:
{ YY_1String[I_Str++] = yyvsp[0].c; ;
	break;}
case 252:
{ YY_1String[I_Str++] = yyvsp[0].c; ;
	break;}
}
 /* the action file gets copied in in place of this dollarsign */


yyvsp -= yylen;
yyssp -= yylen;
#ifdef YYLSP_NEEDED
yylsp -= yylen;
#endif

#if YYDEBUG != 0
if (yydebug)
	{
	short *ssp1 = yyss - 1;
	fprintf (stderr, "state stack now");
	while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
	fprintf (stderr, "\n");
	}
#endif

*++yyvsp = yyval;

#ifdef YYLSP_NEEDED
yylsp++;
if (yylen == 0)
	{
	yylsp->first_line = yylloc.first_line;
	yylsp->first_column = yylloc.first_column;
	yylsp->last_line = (yylsp-1)->last_line;
	yylsp->last_column = (yylsp-1)->last_column;
	yylsp->text = 0;
	}
else
	{
	yylsp->last_line = (yylsp+yylen-1)->last_line;
	yylsp->last_column = (yylsp+yylen-1)->last_column;
	}
#endif

/* Now "shift" the result of the reduction.
	 Determine what state that goes to,
	 based on the state we popped back to
	 and the rule number reduced by.*/

yyn = yyr1[yyn];

yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
	yystate = yytable[yystate];
else
	yystate = yydefgoto[yyn - YYNTBASE];

goto yynewstate;

yyerrlab: /* here on detecting error */

if (! yyerrstatus)
	/* If not already recovering from an error, report this error.*/
	{
	++yynerrs;
	yyerror("parse error");
	}

if (yyerrstatus == 3)
	{
	/* if just tried and failed to reuse lookahead token after an error, discard it.*/

	/* return failure if at end of input */
	if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
	if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

	yychar = YYEMPTY;
	}

/* Else will try to reuse lookahead token
	 after shifting the error token.*/

yyerrstatus = 3;		/* Each real token shifted decrements this */

goto yyerrhandle;

yyerrdefault:/* current state does not do anything special for the error token. */

#if 0
/* This is wrong; only states that explicitly want error tokens
	 should shift them.*/
yyn = yydefact[yystate];/* If its default is to accept any token, ok.Otherwise pop it.*/
if (yyn) goto yydefault;
#endif

yyerrpop: /* pop the current state because it cannot handle the error token */

if (yyssp == yyss) YYABORT;
yyvsp--;
yystate = *--yyssp;
#ifdef YYLSP_NEEDED
yylsp--;
#endif

#if YYDEBUG != 0
if (yydebug)
	{
	short *ssp1 = yyss - 1;
	fprintf (stderr, "Error: state stack now");
	while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
	fprintf (stderr, "\n");
	}
#endif

yyerrhandle:

yyn = yypact[yystate];
if (yyn == YYFLAG)
	goto yyerrdefault;

yyn += YYTERROR;
if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
	goto yyerrdefault;

yyn = yytable[yyn];
if (yyn < 0)
	{
	if (yyn == YYFLAG)
	goto yyerrpop;
	yyn = -yyn;
	goto yyreduce;
	}
else if (yyn == 0)
	goto yyerrpop;

if (yyn == YYFINAL)
	YYACCEPT;

#if YYDEBUG != 0
if (yydebug)
	fprintf(stderr, "Shifting error token, ");
#endif

*++yyvsp = yylval;
#ifdef YYLSP_NEEDED
*++yylsp = yylloc;
#endif

yystate = yyn;
goto yynewstate;
}

