/* Created by Alex Nikol Bourakov in 1989 */

#include "stdafx.h" 

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>

#include "fqeval.hpp"
#include "fxeval.hpp"
#include "scale.hpp"
#include "GridModule.hpp"
#include "mgsort.hpp"

//#include "OleDate.h"
/**/
#define Isalpha(S) (isalpha((S))||(S)=='$')
/**/
const char*FUNAME[14]={"OVR", "ANDNOT", "AND", "XOR", "OR", "NOT", "AREA", "SIZE", "5009", "NOTHING"};
/**/
#include<string.h>
#include<ctype.h>

#pragma warning(disable : 4996)

std::string itostdstring(int x, int width) {
	std::ostringstream ostr; 
	if(width > 0) { 
		ostr << std::setfill('0') << std::setw(width) << x; 
	}
	else { 
		ostr << x;
	} 
	return ostr.str();
}
void FileLog(const std::string& msg, bool do_close) {
}


void graph_wait_for_user_action() { 
} 

bool graph_log_layer(const char* layer_Name, int lb/*optional 0*/, int layer_index/*optional -1*/) { 
	size_t first_polygon_number = 0; 
	long lb_count = 0; 
	if(lb) { 
		lb_count = lb; 
	} 
	else { 
		lb = 1; 
		if(strlen(layer_Name)) { 
			lb_count = MASLayerCountRTPolygons(layer_Name, &first_polygon_number); 
		} 
	} 

	std::ostringstream ostr;

	if(lb_count > 0) { 
		if(layer_index < 0) { 
			layer_index = MaSCOT_IO.iLnames_hash.Exist(layer_Name); //Layer_Name2Pos(layer_Name); 
		} 

		_sAlong *lb_x = new _sAlong[lb_count]; 
		_sAlong *lb_y = new _sAlong[lb_count]; 

		for(int j = lb - 1; j < lb_count; ++j) { 
			_sAlong& x = lb_x[j];
			_sAlong& y = lb_y[j];

			long nPoints = MASLayerGetRTPolygon(layer_Name, j + (int)first_polygon_number, x, y);

			long n = 0;
			while (n < nPoints) {
				if (n > 0) {
					ostr << ','; 
				}
				ostr << (x[n] >> S_FACTOR) << ',' << (y[n] >> S_FACTOR);
				++n;
			}
			ostr << std::endl; 
		}
		WriteStream2file(&ostr, layer_Name);

		delete[] lb_x; 
		delete[] lb_y; 
	} 

	return lb_count > 0;
} 

void graph_log_polygons(const char* file_name, long offsX, long offsY) {
	std::ostringstream ostr;
	for (long i = 1; i < ZLBLIM; ++i) {
		LbBead* Lb = GetLbBead(i);
		if (Lb->ZLBFirstLvBead != 0 && LbBead_Unload2InputXY(Lb, 0)) {
			for (long j = 0; j < ZLDLIM; ++j) {
				if (j > 0) {
					ostr << ',';
				}
				ostr << (offsX + (long)ZLDCO0[j]) << ',' << (offsY + (long)ZLDCO1[j]);
			}
			ostr << std::endl;
		}
	}
	WriteStream2file(&ostr, file_name);
}

void graph_log_ZLDLIM(const char* file_name, long offsX, long offsY) {
	std::ostringstream ostr;
	for (long j = 0; j < ZLDLIM; ++j) {
		if (j > 0) {
			ostr << ',';
		}
		ostr << (offsX + (long)ZLDCO0[j]) << ',' << (offsY + (long)ZLDCO1[j]);
	}
	ostr << std::endl;
	WriteStream2file(&ostr, file_name);
}


void zabort(void)
{
	if(MaSCOT_IO.O_Layer.File)
	{
		fclose(MaSCOT_IO.O_Layer.File);
		//unlink(O_Layer.PdbN);
		MaSCOT_IO.funlink(MaSCOT_IO.O_Layer.PdbN);
	}
	//exit(1);
	throw "\nFATAL ERROR."; 
}



void __sizemain__(char Name[20], long Size, char Sign)
{
	F_SIZE_JOB JOB;

	memcpy(JOB.Name, Name, 20);
	JOB.Size = Size;
	JOB.Sign = Sign;
	JOB.Do_log_graph = 0;

	std::string O_name(MaSCOT_IO.O_Layer.Name, 20);
	std::string I_name(Name, 20);

	F_SIZE_SHAPES(JOB);
	F_SIZE_COLLARS(JOB); // S_FACTOR is set here

	MaSCOT_IO.ILayer_Cnt = 1;
	memcpy(MaSCOT_IO.I_Layer[0].Name, MaSCOT_IO.O_Layer.Name, 20);
	MaSCOT_IO.Activate_ILayers();
	if (MaSCOT_IO.I_Layer[0]._lIdentity.layer_STATUS == 0) {
		LFuInitialize(MaSCOT_IO.O_Layer.Name);
		LFuEvaluaterLoop(MAS_OverlapElim, JOB.Do_log_graph);
	}

	MaSCOT_IO.Setup();

	MaSCOT_IO.ILayer_Cnt = 2;
	memcpy(MaSCOT_IO.I_Layer[0].Name, I_name.c_str(), 20);
	memcpy(MaSCOT_IO.I_Layer[1].Name, MaSCOT_IO.O_Layer.Name, 20);
	MaSCOT_IO.Activate_ILayers();
	LFuInitialize(O_name.c_str());
	LFuEvaluaterLoop(Sign > 0 ? (long)MAS_Or : (long)MAS_AndNot, JOB.Do_log_graph);
}


void LFuInitialize_S_Factor() {
	PYMIN = 0;
	PXMIN = 0;
	PYMAX = XBSIZ;
	PXMAX = YBSIZ;
	S_FACTOR = 0;
	long n = XBSIZ>YBSIZ? XBSIZ: YBSIZ;
	if(!n)zabort();
	do
	{
		++S_FACTOR;
	}
	while(((n<<S_FACTOR)>>S_FACTOR)==n);
	S_FACTOR--;
	S_FACTOR--;
	if(S_FACTOR<0)S_FACTOR = 0;
	if(S_FACTOR>9)S_FACTOR = 9;
}
void LFuInitialize_OutLayer(const char*outLayer_name = 0) {
	if(outLayer_name) { 
		if (MaSCOT_IO.O_Layer.Name != outLayer_name) {
			memset(MaSCOT_IO.O_Layer.Name, 0, 20);
#pragma warning(suppress : 4996)
			strncpy(MaSCOT_IO.O_Layer.Name, outLayer_name, 20);
		}
		MaSCOT_IO.INIT_OLayer();
		MaSCOT_IO.O_Layer._lIdentity.layer_STATUS = 1;

		IEvaluater.__SetVT0LB0__();
	} 
}
void LFuInitialize_ILayers() {
	for(long n = 0; n < MaSCOT_IO.ILayer_Cnt; ++n) {
		MaSCOT_IO.INIT_ILayer(n);
		if(!SET_Xg_Yg(n))
		{
			continue;
		}
		memcpy(MaSCOT_IO.O_Layer.Name, MaSCOT_IO.I_Layer[n].Name, 20);
		MaSCOT_IO.INIT_OLayer();
		FragmPolygons(n);
		MaSCOT_IO.Dix_close();
		MaSCOT_IO.Close_ILayer(n);
		Destroy_GridStorage();
		//IEvaluater._ReleaseStorage();
		memcpy(MaSCOT_IO.O_Layer.PdbN,mg_sort(MaSCOT_IO.O_Layer.PdbN),15);
		MaSCOT_IO.O_Layer._lIdentity.layer_STATUS = 0;
		MaSCOT_IO.renameoutput();
		MaSCOT_IO.INIT_ILayer(n);
	}
	C_Fragment = 0; 
}
void LFuInitialize(const char*outLayer_name = 0) {
	LFuInitialize_ILayers(); 

	if (S_FACTOR == 0) {
		LFuInitialize_S_Factor();
	}

	LFuInitialize_OutLayer(outLayer_name); 
}
bool LFuEvaluaterLoop(long Function, int log_graph, char classifyType/*0 - main, 1 - light*/)
{
	int produced_polygons = 0;

	while(C_Fragment!=LONG_MAX)
	{
		CleanZPTHASH();
		produced_polygons += LFuEvaluater(Function, log_graph, classifyType);
		SetNxtFrag(Function);
	}
	S_FACTOR = 0;

	MaSCOT_IO.FCloseAll();
	//IEvaluater._ReleaseStorage();
	FinishedNormally();

	return produced_polygons > 0;
}
void Load_Boundary(void)
{
	long DX;
	long DY;
	long XB;
	long YB;
	STATUS =+1;
	ZLDLIM = 5;

	C_Layer = 4;
	Target_Function = MFunction_Not;

	XB = C_Fragment/YBCNT;
	if((XB+1)==XBCNT)
	{
		DX = XBSIZ*XBCNT-(CXMAX - CXMIN);
		if(DX<0)zabort();
		DX = XBSIZ-DX;
	}
	else DX = XBSIZ;
	YB = C_Fragment - XB*YBCNT;
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

// use Test contours: ANB-07-APR-2022
int s_testPointsContour[] = { 177, 208, 180, 209, 184, 209, 188, 207, 191, 205, 193, 204, 196, 203, 202, 197, 204, 196, 208, 192, 203, 191, 198, 185, 200, 184, 200, 178, 199, 171, 201, 164, 197, 157, 203, 150, 206, 146, 209, 145, 211, 143, 215, 137, 216, 134, 219, 133, 222, 131, 224, 129, 226, 125, 221, 124, 218, 123, 245, 122, 211, 121, 210, 115, 213, 110, 216, 109, 218, 107, 221, 103, 224, 102, 214, 101, 210, 100, 210, 94, 208, 80, 210, 74, 216, 73, 212, 72, 215, 69, 221, 66, 223, 64, 229, 63, 228, 57, 226, 55, 222, 53, 218, 52, 191, 49, 188, 49, 185, 55, 185, 61, 187, 62, 187, 69, 181, 76, 179, 83, 183, 97, 181, 111, 178, 118, 177, 125, 183, 139, 183, 146, 181, 153, 180, 160, 180, 167, 177, 174, 172, 181, 170, 188, 166, 194, 166, 196, 162, 197, 160, 203, 162, 204, 164, 207, 177, 208, std::numeric_limits<int>::max() };
int s_testPointsCollar[] = { 161, 205, 163, 208, 177, 209, 180, 210, 184, 210, 189, 208, 192, 206, 193, 205, 197, 204, 203, 198, 205, 197, 209, 193, 209, 192, 208, 191, 204, 190, 200, 185, 201, 185, 201, 178, 200, 171, 202, 164, 198, 157, 204, 151, 207, 147, 210, 146, 212, 144, 216, 137, 217, 135, 219, 134, 223, 132, 225, 130, 227, 125, 227, 124, 226, 124, 225, 124, 245, 123, 245, 122, 245, 121, 212, 120, 211, 115, 214, 111, 217, 110, 219, 108, 222, 104, 220, 103, 217, 106, 215, 108, 212, 109, 209, 115, 210, 122, 218, 122, 218, 123, 218, 124, 221, 125, 225, 126, 223, 128, 221, 130, 219, 132, 215, 133, 214, 137, 210, 142, 208, 144, 205, 145, 202, 149, 196, 157, 200, 164, 198, 171, 199, 178, 199, 183, 198, 184, 197, 185, 197, 186, 202, 192, 206, 193, 203, 195, 201, 196, 195, 202, 193, 203, 190, 204, 187, 206, 184, 208, 180, 208, 177, 207, 165, 206, 163, 203, 161, 202, 163, 198, 167, 197, 167, 194, 171, 188, 173, 181, 178, 174, 181, 167, 181, 160, 182, 153, 184, 146, 184, 139, 178, 125, 179, 118, 182, 111, 184, 97, 180, 83, 182, 76, 188, 69, 188, 61, 186, 60, 186, 55, 189, 50, 191, 50, 218, 53, 222, 54, 225, 56, 227, 57, 228, 62, 223, 63, 220, 65, 214, 68, 211, 71, 211, 72, 209, 73, 207, 80, 209, 94, 209, 101, 214, 102, 220, 103, 222, 104, 224, 103, 224, 102, 224, 101, 214, 100, 211, 99, 211, 94, 209, 80, 211, 75, 216, 74, 216, 73, 216, 72, 214, 71, 216, 70, 222, 67, 223, 65, 230, 64, 229, 57, 227, 54, 222, 52, 218, 51, 191, 48, 187, 48, 184, 55, 184, 62, 186, 63, 186, 69, 180, 76, 178, 83, 182, 97, 180, 111, 177, 118, 176, 125, 182, 139, 182, 146, 180, 153, 179, 160, 179, 167, 176, 174, 171, 181, 169, 188, 165, 194, 165, 195, 161, 196, 159, 204, 161, 205, std::numeric_limits<int>::max() };

// use Test contours: ANB-07-APR-2022
void LoadTestFragment() {
	short n;
	__aLlAng__ = 0;
	for (n = 0; n < MaSCOT_IO.ILayer_Cnt; ++n) {

		C_Layer = MaSCOT_IO.I_Layer[n].Lcode;

		long layer_STATUS = MaSCOT_IO.I_Layer[n]._lIdentity.layer_STATUS;

		MaSCOT_IO.D_Link(n);
		if (MaSCOT_IO.I_Fragment(n) <= C_Fragment) {
			while (MaSCOT_IO.I_Fragment(n) == C_Fragment) {
				char open_shape = n == 0 ? MaSCOT_IO.I_READ_P(0, 0, s_testPointsContour) : MaSCOT_IO.I_READ_P(0, 0, s_testPointsCollar);
				open_shape = n == 0 ? MaSCOT_IO.I_READ_P(0, 0, s_testPointsContour) : MaSCOT_IO.I_READ_P(0, 0, s_testPointsCollar);
				MaSCOT_IO.I_Fragment(n) = LONG_MAX;
				layer_STATUS = 0;
				if (ZLDLIM > 0) {
					if (layer_STATUS == 0) {
						LDMake_ClosedPolygon();
						DETERMINE_CLOCKSTATUS(ZLDCO0, ZLDCO1, ZLDLIM);
					}
					else {
						STATUS = 1;
					}
					Loadmaster(1, open_shape);
					break;
				}
			}
		}
	}
	C_Fragment = LONG_MAX;
}


void LoadFragment(int log_graph) {
	short n;
	__aLlAng__ = 0;
	for(n = 0; n < MaSCOT_IO.ILayer_Cnt; ++n) {

		C_Layer = MaSCOT_IO.I_Layer[n].Lcode;

		long layer_STATUS = MaSCOT_IO.I_Layer[n]._lIdentity.layer_STATUS; 

		MaSCOT_IO.D_Link(n);
		if(MaSCOT_IO.I_Fragment(n)<=C_Fragment) {
			while(MaSCOT_IO.I_Fragment(n)==C_Fragment) {
				char open_shape = MaSCOT_IO.I_READ_P();
				layer_STATUS = 0;
				if(ZLDLIM>0) {
					if(layer_STATUS == 0) { 
						LDMake_ClosedPolygon();
						DETERMINE_CLOCKSTATUS(ZLDCO0, ZLDCO1, ZLDLIM); 
					} 
					else { 
						STATUS = 1; 
					} 
					if (log_graph) {
						Sleep(1); 
						graph_log_ZLDLIM("LoadFragment", 0, 0);
					}
					Loadmaster(1, open_shape);
				}
			}
		} 
	}
}

void PrintOk(void) {
	MASLog(".");
}

void ClrStr(void){
}

int WRITE_C_POLYGONS() {
	long cnt = 0; 
	for(long lb = 1; lb<ZLBLIM; ++lb) {
		LbBead*Lb = GetLbBead(lb);
		if(!Lb->ZLBFirstLvBead) {
			continue;
		}
		if(!LbBead_Unload2InputXY(Lb)) {
			continue;
		}
		if(MaSCOT_IO.O_Layer.File) { 
			MaSCOT_IO.Dix_output(); 
			++cnt; 
		} 
	}
	return cnt; 
}
int LFuEvaluater(long Function, int log_graph, char classifyType/*0 - main, 1 - light*/) {
	Target_Function = Function;
	ZLBLIM = 1;
	ZLVLIM = 1;
	ZPTLIM = 1;
	ZVTLIM = 2;
	STATUS =+1;

	//// use Test contours: ANB-07-APR-2022
	//if (Function == MAS_AndNot) {
	//	LoadTestFragment();
	//}
	//else {
	//	LoadFragment(log_graph);
	//}

	LoadFragment(log_graph);

	if (Function == MFunction_ActivateLayer) {
		INTERSECTION_ANALYSIS();
		POINT_VECTOR_ANALYSIS(2);
		return true;
	}

	if(Function==MFunction_Not) {
		Load_Boundary();
	}

	ScalePoints();

	ZVTLIM = 2;
	STATUS =+1;
	INTERSECTION_ANALYSIS();

	POINT_VECTOR_ANALYSIS(2);

	Classify_Polygons(classifyType); 

	if(Function != MFunction_Intersect) { 
		if(Function<0) True_Stat = Function==-1? AND_1:XOR_1;
		else True_Stat = Cmp_Modules[Function];

		FORCE_C_POLYGONS();

		for(long I = 1; I<ZLBLIM; ++I) {
			LbBead*Lb = GetLbBead(I);
			if(Lb->ZLBFirstLvBead)DUMMYJUNCTIONANALYSIS(Lb);
		}

		if(log_graph) {
			graph_wait_for_user_action();
			graph_log_layer(MaSCOT_IO.O_Layer.Name);
		} 
	} 
	else { 
		ZLBLIM = 1;
		ZLVLIM = 1;
	} 

	Back_ScalePoints();

	S_FACTOR = 0; 

	int cnt = WRITE_C_POLYGONS();
	return cnt; 
}
void SetNxtFrag(long Function)
{
	long F;
	long I;
	F = LONG_MAX;
	for(I = 0; I<MaSCOT_IO.ILayer_Cnt; ++I)
	{
		if(MaSCOT_IO.I_Fragment(I)<F)F = MaSCOT_IO.I_Fragment(I);
	}
	if(Function==MFunction_Not)
	if(++C_Fragment<(XBCNT*YBCNT))
	{
		return;
	}
	C_Fragment = F;
}

int F_EVAL_JOB::ProcessArguments(short N, const char*A[]) { 
	size_t I, M, ERR, F, O, D, Q, NF=14;
	size_t J, K, L;
	long w=1, W, s=0, i=0, r=0;
	char S[100];

	classifyType = 0; 
	Function = 0;

	MaSCOT_IO.I_Layer[0].PdbN[0] = 0;

	ERR = F = O = L = D = Q = W = 0;
	if(N==1)
	{
		MASLog("\nPG-mashine-(5009,5089) postprocessor\n");
		zabort();
	}
	for(I = 1; I<N; ++I)
	{
		for(K = 0; A[I][K]; ++K)
		if(islower(A[I][K])) const_cast<char*>(A[I])[K] = toupper(A[I][K]);

		M = A[I][1];
		if(A[I][0]=='-') /*OPTIONAL-PARAMETER*/
		{
			switch(M)
			{
				case 'I': /*input sou file.*/
				strncpy(MaSCOT_IO.I_Layer[0].PdbN, A[I]+2, 20);
				ERR=0;
				if(i)ERR=2;
				i=1;
				break;

				case 'S': /*sizing width.*/
				strncpy(S, A[I]+2, 99);
				A[99] = 0;
				ERR = sscanf(S, "%ld", &w);
				if(ERR!=1){ERR = 1;break;}
				ERR = 0;
				if(W) ERR = 2;
				W = 1;
				break;

				case 'Q': /*QUICK ANALIZE FOR OVERLp.*/
				K = strlen(A[I]+2)+2;
				if(A[I][2]){ ERR = 1; break; }
				if(Q) ERR = 2;
				Q = 1;
				classifyType = 1;
				break;

				case 'F': /*FUNCTION*/
				strncpy(S, A[I]+2, 100);
				for(K = 0; K<NF; ++K)
				if(stricmp(FUNAME[K], S)==0)break;

				if(K==NF){ ERR = 1; break; }

				Function = (long)K;
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

			MaSCOT_IO.ILayer_Cnt = (unsigned short)L;
			strncpy(MaSCOT_IO.I_Layer[L-1].Name, A[I], 20);
			strncpy(I_name[L-1], A[I], 20);
		}
	}
	/*End Parse*/
	if(ERR==2)MASLog("\n!!! Ambiguous Parameter '%s' !!!", A[I]);
	if(ERR==1)MASLog("\n!!! Strange Parameter '%s' !!!", A[I]);
	if(ERR)
	{
		zabort();
	}
	if(L==0)MASLog("\nNo input Layers specified\n"), zabort();
	if(O==0)strncpy(O_name, &I_name[0][0], sizeof(O_name));
	ERR = 0;
	if(L>=2&&Function==MFunction_Not)ERR=1;
	else
	if(L>=2&&Function==MFunction_Area)ERR=1;
	else
	if(L>=2&&Function==MFunction_Size)ERR=1;
	else
	if(L>=2&&Function==MFunction_5009)ERR=1;
	if(ERR)
	{
		MASLog("\n!!! %s IS UNARY OPERATION !!!", FUNAME[Function]);
		zabort();
	}
	if(L==1&&Function==MFunction_And)
	{
		Function = MFunction_AndOne;
	}
	if(L==1&&Function==MFunction_Xor)
	{
		Function = MFunction_XorOne;
	}
	if(L==1&&Function==MFunction_AndNot)
	{
		Function = MFunction_Not;
	}
	if(Function==MFunction_Size)
	{
		MASLog("\nSize is not implemented\n"), zabort();
	}
	if(Function==MFunction_5009)
	{
		MASLog("\n5009 is not implemented\n"), zabort();
	}

	return (int)ERR; 
} 
void F_EVAL_JOB::PerformFunction(int log_graph) { 
	MaSCOT_IO.Setup();
	MaSCOT_IO.Activate_ILayers(); 

	if(O_name) { 
		LFuInitialize(O_name);

		MASLog("\n||| Function <%s> for ", FUNAME[Function]);
		for(int n = 0; n<MaSCOT_IO.ILayer_Cnt; ++n) {
			MASLog("<%s>", MaSCOT_IO.I_Layer[n].Name);
		}
		MASLog(" started; Output is <%s> !!!\n", MaSCOT_IO.O_Layer.Name);

		if(Function!=MFunction_Nothing) { 
			if(Function==MFunction_Area) {
				F_AREA_MAIN(I_name[0]);
			}
			else { 
				LFuEvaluaterLoop(Function, log_graph, classifyType);
			} 
		} 
	} 

	MaSCOT_IO.FCloseAll();
} 


F_EVAL_JOB::F_EVAL_JOB(short N, const char*A[], int log_graph) { 
	if(ProcessArguments(N, A) == 0) { 
		PerformFunction(log_graph); 
	} 
}

F_EVAL_JOB::F_EVAL_JOB() { 
	Function = 0;

	memset(I_name, 0, sizeof(I_name));
	memset(O_name, 0, sizeof(O_name));

	classifyType = 0; 
} 


void DbWide_EliminateOverlaps(long N = 0, const char *Names[] = 0) { 
	MaSCOT_IO.Setup();

	std::vector<std::string> layers_todo; 

	if(N == 0) { 
		N = MaSCOT_IO.iLnames_hash.Strings_Lim; 
	} 

	int n = 1; 
	while(n < N) {
		const char *layer_name = Names? Names[n++]: MaSCOT_IO.iLnames_hash.Strings[n++].N; 

		int x = layer_name[0]? MaSCOT_IO.iLnames_hash.Exist(layer_name): 0; 
		if(x > 0) { 
			fpos_t fp = MaSCOT_IO.FPOS_2 + (x - 1) * sizeof(LayerIdentity_Struct); 
			fsetpos(MaSCOT_IO._dbLayers_FILE, &fp);

			LayerIdentity_Struct l; 
			fp = fread(&l, sizeof(LayerIdentity_Struct), 1, MaSCOT_IO._dbLayers_FILE); 

			if(fp == 1 && l.layer_STATUS == 0) { 
				layers_todo.push_back(layer_name); 
			} 
		} 
	}; 

	MaSCOT_IO.FCloseAll();


	for(size_t x = 0; x < layers_todo.size(); ++x) {
		const char*a[3] = {"", (char*)layers_todo[x].c_str(), "-FOVR"}; 
//		F_EVAL_JOB JOB(3, a, 1);
//		graph_wait_for_user_action(); 
		F_EVAL_JOB JOB(3, a);
	} 
} 


extern char*MASLogbuffer; 

BOOL Load_Database() { 
	MaSCOT_IO.funlink_all(); 

	WIN32_FIND_DATAA stFindData; 
	const char *strFileName = "M2*.xzq"; 
	HANDLE hFind = FindFirstFileA(strFileName, &stFindData); 
	BOOL fOk = (hFind != INVALID_HANDLE_VALUE); 
	BOOL fRc = fOk; 
	while(fOk) { 
		if((stFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) { 
			FILE *fil = fopen(stFindData.cFileName, "rb"); 
			if(fil) { 
				XMemoryFile *xfil = MaSCOT_IO.fopen(stFindData.cFileName); 
				while(!feof(fil)) { 
					char buf; 
					if(fread(&buf, 1, 1, fil)) { 
						xfil->write(&buf, 1, 1); 
					} 
				} 
				fclose(xfil); 
				fclose(fil); 
			} 
		} 
		fOk = FindNextFileA(hFind, &stFindData); 
	} 
	if(hFind != INVALID_HANDLE_VALUE) { 
		FindClose(hFind); 
	} 

	return fRc; 
} 


void dosmain(short N, const char*A[]) { 
	Load_Database(); 

	DbWide_EliminateOverlaps(N, A); 

	try { 
		F_EVAL_JOB JOB(N, A, 1); // output layer name in JOB.O_name
	} 
	catch(char*excpt_msg) { 
		MASLog(excpt_msg); 
	} 

	if(MASLogbuffer) { 
		printf(MASLogbuffer); 
	} 

	graph_wait_for_user_action(); 
}

DWORD WINAPI dosmain_inbatch(LPVOID pArgs) { 
	SCommandLineArguments& args = *(SCommandLineArguments*)pArgs; 
	dosmain(args.N, (const char**)args.A); 
	return 0; 
} 




extern long int ZPTLim_threshold; 
extern long int ZVTLim_threshold; 


bool MASPreload_StaticLayers(long N, const char *Names[]) { 
	if(Load_Database()) { 
		DbWide_EliminateOverlaps(N, Names); 
	} 
	else { 
		MASInitialize(1, 1); 
	} 

	MaSCOT_IO.I_Layer[0].PdbN[0] = 0;
	MaSCOT_IO.ILayer_Cnt = 0; 

	int n; 
	for(n = 1; n < N && MaSCOT_IO.ILayer_Cnt < 4; ++n) { 
		if(!Names[n]) { 
			continue; 
		} 
		++MaSCOT_IO.ILayer_Cnt;
		strncpy(MaSCOT_IO.I_Layer[n - 1].Name, Names[n], 20);
	} 
	if(n < N) { 
		return false; 
	} 

	MaSCOT_IO.Setup();
	MaSCOT_IO.Activate_ILayers(); 

//	IEvaluater._ReleaseStorage();

	ZPTLim_threshold = 0; 
	ZVTLim_threshold = 0; 

	bool rc = false; 
	try { 
		LFuInitialize(); 
		LFuEvaluaterLoop(/*MFunction_Intersect*/MFunction_Or); 
		rc = true; 

		S_FACTOR = 0;

		ZPTLim_threshold = ZPTLIM; 
		ZVTLim_threshold = ZVTLIM; 

		memcpy(ZPTHASH_Static, ZPTHASH, (ZPBLIM+1)*sizeof(PtBead*)); 

		for(int vt = 0; vt < ZVTLIM; ++vt) { 
			GetVtBead(vt)->ZVTFirstCoincident = 0; 
		} 
	} 
	catch(char*excpt_msg) { 
		MASLog(excpt_msg); 
	} 
	catch(...) { 
	} 

	if(MASLogbuffer) { 
		printf(MASLogbuffer); 
	} 

	ZLBLIM = 1; 
	ZLVLIM = 1; 

	return rc; 
} 


