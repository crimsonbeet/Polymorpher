#include "stdafx.h" 

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>

#include "fxeval.hpp"
#include "fqeval.hpp"
#include "_scale.hpp"
#include "dxbout.hpp"
#include "fzinit.hpp"
#include "m_alcr.hpp"
#include "mgsort.hpp"
long YY_Xrelocate;
long YY_Yrelocate;
long WMinX;
long WMaxX;
long WMinY;
long WMaxY;
char dbgoutdxb;
char QuickClass;
void __FINISHEDNORMALLY__(void);
FILE*Rniirm;
char*Nniirm="mniirm.rpt";
class F_EVAL_JOB
{
public:
	char*O_name;
	char*O_nameTemp;
	char*I_name[4];
	long Function;
	char D_Output;
	void ComEvalInit(void);
	void LFuEvaluater(void);
	void LFuEvaluaterLoop(void);
	void SetNxtFrag(void);
	F_EVAL_JOB(short N, char*A[]);
	~F_EVAL_JOB(){};
}       ;
#define Isalpha(S) (isalpha((S))||(S)=='$')
/**/
char*FUNAME[14]={"OVR", "ANDNOT", "AND", "XOR", "OR", "NOT", "SIZE", "5009", "559", "OTELLO", "PDIF", "5089", "AREA", "DUMMY"};
FILE*Pdif;
char*Pdif_Name="$cmd.pdf";
/**/
#include<string.h>
#include<ctype.h>
#ifdef __ZTC__
#define stricmp strcmpl
#endif
struct F_SIZE_JOB
{
	char Name[20];
	long Size;
	char Sign;
}       ;
void	zabort(void);
void	FinishedNormally(void);
void    F_SIZE_SHAPES(F_SIZE_JOB&JOB);
void    F_SIZE_COLLARS(F_SIZE_JOB&JOB);
void
__sizemain__(char Name[20], long Size, char Sign)
{
	F_SIZE_JOB JOB;
	memcpy(JOB.Name, Name, 20);
	JOB.Size = Size;
	JOB.Sign = Sign;
	F_SIZE_SHAPES(JOB);
	F_SIZE_COLLARS(JOB);
}
void F_EVAL_JOB::ComEvalInit(void)
{
	long n;
	long F=Function;
	for(n = 0; n<ILayer_Cnt; ++n)
	{
		MaSCOT_IO.INIT_ILayer(n);
		if(!SET_Xg_Yg(n))
		{
			continue;
		}
		memcpy(O_Layer.Name, I_Layer[n].Name, 20);
		MaSCOT_IO.INIT_OLayer();
		FragmPolygons(n);
		MaSCOT_IO.Dix_close();
		MaSCOT_IO.Close_ILayer(n);
		UPSet_Xg_Yg();
		IEvaluater._ReleaseStorage();
		memcpy(O_Layer.PdbN,mg_sort(O_Layer.PdbN),15);
		O_Layer.LX_S.STATUS = 0;
		MaSCOT_IO.renameoutput();
		MaSCOT_IO.INIT_ILayer(n);
	}
	PYMIN = 0;
	PXMIN = 0;
	PYMAX = XBSIZ;
	PXMAX = YBSIZ;
	n = XBSIZ>YBSIZ? XBSIZ: YBSIZ;
	S_Factor = 0;
	S_FACTOR = 0;
	if(!n)zabort();
	do
	{
		++S_Factor;
	}
	while(((n<<S_Factor)>>S_Factor)==n);
	S_Factor--;
	S_Factor--;
	if(S_Factor<0)S_Factor = 0;
	if(S_Factor>9)S_Factor = 9;

	S_FACTOR = S_Factor;
	S_Factor = 0;

	memcpy(O_Layer.Name, O_name, 20);
	MaSCOT_IO.INIT_OLayer();
	O_Layer.LX_S.STATUS = 1;
	OutDeltaX = 0;
	OutDeltaY = 0;
	CFragment = 0;
	XFragment =-1;
	fprintf(stderr, "\n||| Function <%s> for ", FUNAME[F]);
	for(n=0; n<ILayer_Cnt; ++n)
	{
		fprintf(stderr, "<%s>", I_name[n]);
	}
	fprintf(stderr, " started; Output is <%s> !!!\n", O_name);
	IEvaluater.__SetVT0LB0__();
F_EVAL_MODULE = 1;
}
void F_EVAL_JOB::LFuEvaluaterLoop(void)
{
	while(CFragment!=LONG_MAX)
	{
		CleanZPBCOR((unsigned short)ZPBLIM+1);
		LFuEvaluater();
		SetNxtFrag();
	}
	MaSCOT_IO.FCloseAll();
	IEvaluater._ReleaseStorage();
	FinishedNormally();
}
F_EVAL_JOB::F_EVAL_JOB(short N, char*A[])
{
	long I, M, ERR, F, O, D, Q, NF=14;
	long J, K, L;
	long w=1, W, s=0, i=0, r=0;
	char S[100];
	I_Layer[0].PdbN[0]=0;
	L = MaSCOT_IO.S_BLEN;
	L -= 20;
	O_name = &MaSCOT_IO.S_BTOP[L];
	L -= 20;
	O_nameTemp = &MaSCOT_IO.S_BTOP[L];
	for(I=0; I<4; ++I)
	{
		L-= 20;
		I_name[I] = &MaSCOT_IO.S_BTOP[L];
	}
	MaSCOT_IO.S_BLEN = L;
	Function = 0;
	D_Output = 0;

	ERR = F = O = L = D = Q = W = 0;
	if(N==1)
	{
		fprintf(stderr, "\nPG-mashine-(5009,5089) postprocessor\n");
		exit(0);
	}
	for(I = 1; I<N; ++I)
	{
		for(K = 0; A[I][K]; ++K)
		if(islower(A[I][K])) A[I][K] = toupper(A[I][K]);

		M = A[I][1];
		if(A[I][0]=='-') /*OPTIONAL-PARAMETER*/
		{
			switch(M)
			{
				case 'W': /*window for PG mashine.*/
				/*xmin,ymin,xmax,ymax*/
				strncpy(S, A[I]+2, 100);
				ERR = sscanf(S, "%ld,%ld,%ld,%ld,%s", &WMinX,&WMinY,&WMaxX,&WMaxY,S+50);
				if(ERR!=4){ERR = 1;break;}
				ERR = 0;
				if(s) ERR = 2;
				s = 1;
				break;

				case 'I': /*input sou file.*/
				strncpy(I_Layer[0].PdbN, A[I]+2, 20);
				ERR=0;
				if(i)ERR=2;
				i=1;
				break;

				case 'R': /*rellocate for PG*/
				strncpy(S, A[I]+2, 100);
				ERR = sscanf(S, "%ld,%ld,%s", &YY_Xrelocate,&YY_Yrelocate,S+50);
				if(ERR!=2){ERR = 1;break;}
				ERR = 0;
				if(r)ERR = 2;
				r = 1;
				break;

				case 'S': /*sizing width.*/
				strncpy(S, A[I]+2, 100);
				ERR = sscanf(S, "%ld", &w);
				if(ERR!=1){ERR = 1;break;}
				ERR = 0;
				if(W) ERR = 2;
				W = 1;
				break;

				case 'D': /*OUTPUT TO DXB*/
				K = strlen(A[I]+2)+2;
				if(A[I][2]){ ERR = 1; break; }
				if(D) ERR = 2;
				D = 1;
				D_Output = 1;
				dbgoutdxb= 1;
				break;

				case 'Q': /*QUICK ANALIZE FOR OVERLp.*/
				K = strlen(A[I]+2)+2;
				if(A[I][2]){ ERR = 1; break; }
				if(Q) ERR = 2;
				Q = 1;
				QuickClass = 1;
				break;

				case 'F': /*FUNCTION*/
				strncpy(S, A[I]+2, 100);
				for(K = 0; K<NF; ++K)
				if(stricmp(FUNAME[K], S)==0)break;

				if(K==NF){ ERR = 1; break; }

				Function = K;
				if(F) ERR = 2;
				F = K;
				break;

				case 'O': /*OUTPUT*/
				K = strlen(A[I]+2);
				if(!K||K>=20){ ERR = 1; break; }

				for(J = 2; A[I][J]; ++J)
				if(!isdigit(A[I][J])&&!isalpha(A[I][J]))break;

				if(A[I][J]){ ERR = 1; break; }

				strncpy(O_name, A[I]+2, 20);
				if(O) ERR = 2;
				O = 1;
				break;

				default: ERR = 1;
			}
			if(ERR) break;
		}
		else
		/*ILayer Name*/
		{
			K = strlen(A[I]);
			if(!K||K>=20){ ERR = 1; break; }

			for(J = 0; J<K; ++J)
			if(!isdigit(A[I][J])&&!Isalpha(A[I][J]))break;
			if(++L>4||J<K){ ERR = 1; break; }

			ILayer_Cnt = (unsigned short)L;
			strncpy(I_Layer[L-1].Name, A[I], 20);
			strncpy(I_name[L-1], A[I], 20);
		}
	}
	/*End Parse*/
	if(ERR==2)
	fprintf(stderr, "\n!!! Ambiguous Parameter '%s' !!!", A[I]);
	if(ERR==1)
	fprintf(stderr, "\n!!! Strange Parameter '%s' !!!", A[I]);
	if(ERR)
	{
		zabort();
	}
	if(L==0)exit(1);
	if(O==0)
	strncpy(O_name, I_name[0], 20);
	ERR = 0;
	if(L>=2&&Function==ZZZNOTJ)ERR=1;
	else
	if(L>=2&&Function==ZZZAREA)ERR=1;
	else
	if(L>=2&&Function==ZZZSIZE)ERR=1;
	else
	if(L>=2&&Function==ZZZ5089)ERR=1;
	else
	if(L>=2&&Function==ZZZ5009)ERR=1;
	else
	if(L>=2&&Function==ZZZZ559)ERR=1;
	else
	if(L>=2&&Function==ZOTELLO)ERR=1;
	if(ERR)
	{
		fprintf(stderr, "\n!!! %s IS UNARY OPERATION !!!", FUNAME[Function]);
		zabort();
	}
	if(L==1&&Function==ZZZANDJ)
	{
		Function = ZZZAND1;
	}
	if(L==1&&Function==ZZZXORJ)
	{
		Function = ZZZXOR1;
	}
	if(L==1&&Function==ZANDNOT)
	{
		Function = ZZZNOTJ;
	}
	if(i&&!(Function==ZZZ5089))ERR=1;
	if(ERR)
	{
		fprintf(stderr, "\n!!! Input file is legal for PG-mashine-5089 only !!!");
		zabort();
	}
	if(Function==ZZZ5089)
	{
		exit(0);
	}
	MaSCOT_IO.Setup();
	if(Function==ZZZSIZE)
	{
		long z = w<0?-w: w;
		char s = w<0?-1: 1;
		__sizemain__(I_name[0], z, s);
		memset(I_name[1], 0, 20);
		memcpy(I_name[1], I_name[0], 20);
		memset(I_name[0], 0, 20);
		memcpy(I_name[0], MaSCOT_IO.LZ_S.TempP, 3);
		memcpy(I_Layer[0].Name, I_name[0], 20);
		memcpy(O_nameTemp, O_name, 20);
		memcpy(O_name, I_name[0], 20);
		ILayer_Cnt=1;
		Function=ZZZZORJ;
		ComEvalInit();
		LFuEvaluaterLoop();
		MaSCOT_IO.Setup();
		
		memset(I_name[0], 0, 20);
		memcpy(I_name[0], I_name[1], 20);
		memcpy(I_Layer[0].Name, I_name[0], 20);
		memset(I_name[1], 0, 20);
		memcpy(I_name[1], MaSCOT_IO.LZ_S.TempP, 3);
		memcpy(I_Layer[1].Name, I_name[1], 20);
		memcpy(O_name, O_nameTemp, 20);
		if(s>0)Function=ZZZZORJ;
		else   Function=ZANDNOT;
		ILayer_Cnt=2;
	}
	void
	M_5009_MAIN(char Name[20], long FMin, long FMax, long HMax);
	if(Function==ZZZZ559)
	{
		Rniirm=__open__(Nniirm,"a+t");
		M_5009_MAIN(I_name[0], 4, 3000, 6000);
	}
	if(Function==ZZZ5009)
	{
		Rniirm=__open__(Nniirm,"a+t");
		M_5009_MAIN(I_name[0], 4, 200, 3000);
	}
	void
	F_AREA_MAIN(char Name[20]);
	if(Function==ZZZAREA)
	{
		Rniirm=__open__(Nniirm,"a+t");
		F_AREA_MAIN(I_name[0]);
	}
	void
	OTELLO_MAIN(char Name[20], long BSiz, long FSiz, long STep);
	if(Function==ZOTELLO)
	{
		if(w>31)
		{
		fprintf(stderr, "\nwarning:w-param setted to 31 units\n");
		}
		if(w<=0)
		{
		fprintf(stderr, "\nerror:bad w-param\n");
		zabort();
		}
		if(!W)w=31;
		OTELLO_MAIN(I_name[0], 15000, 62, w);
	}
	long n;
	MaSCOT_IO.Find_ILayer();
	//void    PDIF_OUT_MAIN(long L_index, long Width);
	if(Function==ZZZPDIF)
	{
		Pdif =__open__(Pdif_Name, "wb");
		fprintf(Pdif, "{COMPONENT $CMD.PCB\r\n{ENVIRONMENT\r\n{PDIFvrev 4.00}\r\n");
		fprintf(Pdif, "{DBtype \"PC-Board\"}\r\n{DBvrev 1.04}\r\n{DBunit \"CMM\"}\r\n{DBgrid 1}\r\n");
		fprintf(Pdif, "{Lyrstr");
		for(n = 0; n<ILayer_Cnt; ++n)
		{
			fprintf(Pdif, " \"%s\" 6",  I_Layer[n].Name);
		}
		fprintf(Pdif, "}\r\n}\r\n{USER{VIEW{Mode DETL}}}\r\n{DISPLAY}\r\n");
		fprintf(Pdif, "{SYMBOL{PIN_DEF}{PIC}{ATR{IN{Org -32767 -32767}{Ty 255}}}}\r\n");

		fprintf(Pdif, "{DETAIL\r\n{ANNOTATE\r\n");
		if(!W||!w)w=1;
		if(w<0)w=-w;
		for(n = 0; n<ILayer_Cnt; ++n)
		{
			//PDIF_OUT_MAIN(n, w);
		}
		fprintf(Pdif, "}\r\n{NET_DEF}\r\n{PAD_STACK}\r\n{SUBCOMP}\r\n}\r\n}\r\n");
		__close__(Pdif, Pdif_Name);
		exit(0);
	}
	ComEvalInit();
	if(Function==ZZDUMMY)
	{
		MaSCOT_IO.FCloseAll();
	}
	else 
	{ 
		LFuEvaluaterLoop(); 
	} 
}
DxbOutPut DxbOut;
void
__DXBLD__(char closure, char*s)
{
	long L;
	long X;
	long Y;
	long x;
	long y;
	if(ZLDLIM==0)
	{
		DxbOut.write_eof();
		return;
	}
	if(ZLDLIM<2)return;
	if(!DxbOut.DXB)
	{
		DxbOut.nam = "$cmd.dxb";
		DxbOut.DXB = Fopen(DxbOut.nam, "wb");
		DxbOut.write_header();
		DxbOut.set_double();
	}
	if(s)
	{
		DxbOut.write_layer(s);
	}
	X = CFragment/YBCNT;
	Y = CFragment - X*YBCNT;

	x = CXMIN + X*XBSIZ;
	y = CYMIN + Y*YBSIZ;

	if(ZLDLIM==2)
	{
		DxbOut.write_line(x+ZLDCO0[0], y+ZLDCO1[0], x+ZLDCO0[1], y+ZLDCO1[1]);
		return;
	}
	DxbOut.write_pline((short)closure);
	for(L = 0; L<ZLDLIM; ++L)
	{
		DxbOut.write_vertex(x+ZLDCO0[L], y+ZLDCO1[L]);
	}
	DxbOut.write_seqend();
}
void Load_Boundary(void)
{
	long DX;
	long DY;
	long XB;
	long YB;
	STATUS = 1;
	ZLDLIM = 5;

	C_Layer = 4;
	Target_Function = ZZZNOTJ;

	XB = CFragment/YBCNT;
	if((XB+1)==XBCNT)
	{
		DX = XBSIZ*XBCNT-(CXMAX - CXMIN);
		if(DX<0)zabort();
		DX = XBSIZ-DX;
	}
	else DX = XBSIZ;
	YB = CFragment - XB*YBCNT;
	if((YB+1)==YBCNT)
	{
		DY = YBSIZ*YBCNT-(CYMAX - CYMIN);
		if(DY<0)zabort();
		DY = YBSIZ-DY;
	}
	else DY = YBSIZ;

	ZLDCO0[0] = ZLDCO0[1] = 0L;
	ZLDCO0[2] = ZLDCO0[3] = DX;
	ZLDCO1[0] = ZLDCO1[3] = 0L;
	ZLDCO1[1] = ZLDCO1[2] = DY;
	ZLDCO0[4] = ZLDCO0[0];
	ZLDCO1[4] = ZLDCO1[0];

	Loadmaster();
}
void
LoadFragment(void)
{
	unsigned short n;
	__aLlAng__ = 0;
	for(n = 0; n<ILayer_Cnt; ++n)
	{
		C_Layer = I_Layer[n].Lcode;

		MaSCOT_IO.D_Link(n);
		if(I_Layer[n].Fragment<=CFragment)
		{
			while(I_Layer[n].Fragment==CFragment)
			{
				MaSCOT_IO.ZDREAD_P();
				if(ZLDLIM>0) 
				{ 
					Loadmaster(1);
				}
			}
			MaSCOT_IO.D_BACKLink(n);
		}
	}
}
void PrintOk(void)
{
	fprintf(stderr, ".");
}
#ifdef __BORLANDC__
void ClrStr(void)
{
	fprintf(stderr, "\r");
	clreol();
	fprintf(stderr, "\r");
}
#else
void ClrStr(void){}
#endif
void
WRITE_C_POLYGONS(F_EVAL_JOB*JOB)
{
	long lb;
	for(lb = 1; lb<ZLBLIM; ++lb)
	{
		LbBead*Lb = GetLbBead(lb);
		if(!Lb->ZLBFirstLvBead)
		{
			continue;
		}
		if(!__UNL__(Lb))
		{
			continue;
		}
		MaSCOT_IO.Dix_output();
		if(JOB->D_Output)
		{
			__DXBLD__(1, O_Layer.Name);
		}
	}
}
void DspNmbrFrgmnt(long Fr)
{
ClrStr();fprintf(stderr, "\rFRAGMENT%ld!", Fr);
}
void
F_EVAL_JOB::LFuEvaluater(void)
{
	Target_Function = Function;
	ZLBLIM = 1;
	ZLVLIM = 1;
	ZPTLIM = 1;
	ZVTLIM = 2;
	STATUS =+1;
DspNmbrFrgmnt(CFragment);
	LoadFragment();
	if(Function==ZZZNOTJ)
	{
		Load_Boundary();
	}
	__pAm__ = ZPTLIM;
	__SCL__();

start_CmplxIntrsctn:
PrintOk();
	do { 
		RepeatComplexIntersection=0;
		ComplexIntersectionAllowed=1;
		ZVTLIM = 2;
		STATUS =+1;
		INTERSECTION_ANALYSIS();

	} while(RepeatComplexIntersection); 

	POINT_VECTOR_ANALYSIS(2);

PrintOk();

	RepeatComplexIntersection=0;
	ComplexIntersectionAllowed=0;
	BuildSLine = 1;
	if(Function==ZZZANDJ) BuildSLine = 0;
	else
	if(Function==ZZZAND1) BuildSLine = 0;

	Classify_Polygons(QuickClass); 

PrintOk();

	long I = Function;
	if(I<0)	True_Stat = I==-1? AND_1:XOR_1;
	else	True_Stat = Cmp_Modules[I];
	FORCE_C_POLYGONS();

PrintOk();

	for(I = 1; I<ZLBLIM; ++I)
	{
		LbBead*Lb = GetLbBead(I);
		if(Lb->ZLBFirstLvBead)
		DUMMYJUNCTIONANALYSIS(Lb);
	}
	__BSC__();
PrintOk();
	if(!__aLlAng__||!__xSx__)goto w_c_p_only;
	ZVTLIM = 2;
	STATUS =+1;
	INTERSECTION_ANALYSIS();
	POINT_VECTOR_ANALYSIS(2);
PrintOk();
	Classify_Polygons(1/*quickclassify*/); 
	True_Stat = O_R;
	FORCE_C_POLYGONS();
PrintOk();
w_c_p_only:
	WRITE_C_POLYGONS(this);
}
void
F_EVAL_JOB::SetNxtFrag(void)
{
	long F;
	long I;
	F = LONG_MAX;
	for(I = 0; I<ILayer_Cnt; ++I)
	{
		if(I_Layer[I].Fragment<F)F = I_Layer[I].Fragment;
	}
	if(Function==ZZZNOTJ)
	if(++CFragment<(XBCNT*YBCNT))
	{
		return;
	}
	CFragment = F;
}
void zabort(void)
{
fprintf(stderr, "\nEXITING DUE TO FATAL ERROR.");
	if(O_Layer.File.Fil)
	{
		fclose(O_Layer.File.Fil);
		Delete_File(O_Layer.PdbN);
	}
	DxbOut.write_eof();
	if(DxbOut.nam)Delete_File(DxbOut.nam);
	exit(1);
}
void
dosmain(short N, char*A[])
{
	F_EVAL_JOB JOB(N, A);
	exit(0);
}
