/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#include "stdafx.h" 


#include "KVectors.h"
#include "FrameDrawing.h"


#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>

#include "fqeval.hpp"
#include "fxeval.hpp"
#include "scale.hpp"
#include "GridModule.hpp"
#include "mgsort.hpp"
/**/
#define Isalpha(S) (isalpha((S))||(S)=='$')
/**/
char*FUNAME[14]={"OVR", "ANDNOT", "AND", "XOR", "OR", "NOT", "AREA", "SIZE", "5009", "NOTHING"};
/**/
#include<string.h>
#include<ctype.h>

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
std::string stringify_currentTime(bool file_mode) {
	struct _timeb t;
	_ftime(&t);
	std::istringstream istrTime(ctime(&t.time));
	std::string sWeekDay, sMonth;
	int iDay, iHour, iMinutes, iSeconds, iYear;
	char cSColmn;
	istrTime >> sWeekDay >> sMonth >> iDay >> iHour >> cSColmn >> iMinutes >> cSColmn >> iSeconds >> iYear;
	std::ostringstream ostr;
	ostr.fill('0');
	if(file_mode) { 
		ostr << std::setw(2) << iYear << sMonth << iDay << '_' << iHour << std::setw(2) << iMinutes << std::setw(2) << iSeconds << '_' << std::setw(3) << (int)t.millitm;
	} 
	else { 
		ostr << std::setw(2) << iHour << ':' << std::setw(2) << iMinutes << ':' << std::setw(2) << iSeconds << '_' << std::setw(3) << (int)t.millitm;
	} 

	return ostr.str();
}
void FileLog(const std::string& msg, bool do_close) {
	static wsi_gate gate;
	static HANDLE handle_file = (HANDLE)-1;
	gate.lock();
	if(!do_close && handle_file == (HANDLE)-1) {
		handle_file = OSWRONLYOpenFile("MASCOT" + stringify_currentTime(true) + ".csv");
	}
	if(handle_file != (HANDLE)-1) {
		OSAppendStringToFile(handle_file, msg);
	}
	if(do_close && handle_file != (HANDLE)-1) {
		OSCloseFile(handle_file);
		handle_file = (HANDLE)-1;
	}
	gate.unlock();
}


void graph_wait_for_user_action() { 
	std::string old_name = _g_pDrawing->_ctl.name; 

	PostMessage(_g_drawing_frame->_hwnd, WM_USER, 1, 0); 
	while(_g_drawing_frame && (_g_pDrawing == 0 || old_name == _g_pDrawing->_ctl.name)) { 
		OSWait(); 
	} 
	OSSleep(500); 
} 

void graph_log_layer(char* layer_Name, int lb/*optional 0*/, int layer_index/*optional -1*/) { 
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

	if(lb_count) { 
		if(layer_index < 0) { 
			layer_index = MaSCOT_IO.iLnames_hash.Exist(layer_Name); //Layer_Name2Pos(layer_Name); 
		} 

		_sAlong *lb_x = new _sAlong[lb_count]; 
		_sAlong *lb_y = new _sAlong[lb_count]; 

		for(int j = lb - 1; j < lb_count; ++j) { 
			MASLayerGetRTPolygon(layer_Name, j + (int)first_polygon_number, lb_x[j], lb_y[j]); 
		} 

		_g_pDrawing->_meta.lock(); 

		for(int j = lb - 1; j < lb_count; ++j) { 
			if(lb_x[j].Size()) { 
				KWSIPointArray *ar = new KWSIPointArray(); 
				ar->SetBrush(layer_index); 

				ar->_points.resize(lb_x[j].Size()); 
				for(size_t n = 0; n < lb_x[j].Size(); ++n) { 
					ar->_points[n] = KWSIPoint(lb_x[j][n], lb_y[j][n]); 
				} 
				_g_pDrawing->_meta.AddPtr(ar); 
			} 
		} 

		_g_pDrawing->_meta.unlock(); 

		PostMessage(_g_drawing_frame->_hwnd, WM_USER, 0, 0); 

		delete[] lb_x; 
		delete[] lb_y; 
	} 
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
		memcpy(MaSCOT_IO.O_Layer.Name, outLayer_name, 20);
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

	LFuInitialize_S_Factor(); 

	LFuInitialize_OutLayer(outLayer_name); 
}
void LFuEvaluaterLoop(long Function, char classifyType/*0 - main, 1 - quick*/, int log_graph)
{
	while(C_Fragment!=LONG_MAX)
	{
		CleanZPTHASH();
		LFuEvaluater(Function, classifyType, log_graph);
		SetNxtFrag(Function);
	}
	//MaSCOT_IO.FCloseAll();
	//IEvaluater._ReleaseStorage();
	FinishedNormally();
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
		if(DX<0)
			zabort();
		DX = XBSIZ-DX;
	}
	else DX = XBSIZ;
	YB = C_Fragment - XB*YBCNT;
	if((YB+1)==YBCNT)
	{
		DY = YBSIZ*YBCNT-(CYMAX - CYMIN);
		if(DY<0)
			zabort();
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
void LoadFragment() {
	unsigned short n;
	__aLlAng__ = 0;
	for(n = 0; n<MaSCOT_IO.ILayer_Cnt; ++n) {
		C_Layer = MaSCOT_IO.I_Layer[n].Lcode;

		long layer_STATUS = MaSCOT_IO.I_Layer[n]._lIdentity.layer_STATUS; 

		MaSCOT_IO.D_Link(n);
		if(MaSCOT_IO.I_Fragment(n)<=C_Fragment) {
			while(MaSCOT_IO.I_Fragment(n)==C_Fragment) {
				char open_shape = MaSCOT_IO.I_READ_P();
				if(ZLDLIM>0) { 
					if(layer_STATUS == 0) { 
						LDMake_ClosedPolygon();
						DETERMINE_CLOCKSTATUS(ZLDCO0, ZLDCO1, ZLDLIM); 
					} 
					else { 
						STATUS = 1; 
					} 
					Loadmaster(1, open_shape);
				}
			}
		} 
	}
}
void PrintOk(void)
{
	MASLog(".");
}
#ifdef __BORLANDC__
void ClrStr(void)
{
	MASLog("\r");
	clreol();
	MASLog("\r");
}
#else
void ClrStr(void){}
#endif
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
void DspNmbrFrgmnt(long Fr)
{
ClrStr();MASLog("\rFRAGMENT%ld!", Fr);
}
void LFuEvaluater(long Function, char classifyType/*0 - main, 1 - quick*/, int log_graph) {
DspNmbrFrgmnt(C_Fragment);
	Target_Function = Function;
	ZLBLIM = 1;
	ZLVLIM = 1;
	ZPTLIM = 1;
	ZVTLIM = 2;
	STATUS =+1;

	LoadFragment();

	if(Function==MFunction_Not) {
		Load_Boundary();
	}
	__pAm__ = ZPTLIM;

	ScalePoints();

PrintOk();

	ZVTLIM = 2;
	STATUS =+1;
	INTERSECTION_ANALYSIS();

	POINT_VECTOR_ANALYSIS(2);

	if(log_graph) { 
		for(int n = 0; n < MaSCOT_IO.ILayer_Cnt; ++n) {
			graph_log_layer(MaSCOT_IO.I_Layer[n].Name); 
		} 
	} 
PrintOk();

	Classify_Polygons(classifyType); 

PrintOk();

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

PrintOk();
	if(!__aLlAng__||!__xSx__)goto w_c_p_only;

	ZVTLIM = 2;
	STATUS =+1;
	INTERSECTION_ANALYSIS();
	POINT_VECTOR_ANALYSIS(2);
PrintOk();
	Classify_Polygons(1/*quickclassify*/); 
	if(Function != MFunction_Intersect) { 
		True_Stat = O_R;
		FORCE_C_POLYGONS();
	} 
PrintOk();

w_c_p_only:
	WRITE_C_POLYGONS();
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

int F_EVAL_JOB::ProcessArguments(short N, char*A[]) { 
	long I, M, ERR, F, O, D, Q, NF=14;
	long J, K, L;
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
		if(islower(A[I][K])) A[I][K] = toupper(A[I][K]);

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
				strncpy(S, A[I]+2, 100);
				ERR = sscanf(S, "%ld", &w);
				if(ERR!=1){ERR = 1;break;}
				ERR = 0;
				if(W) ERR = 2;
				W = 1;
				break;

				case 'Q': /*QUICK ANALIZE FOR OVERLp.*/
				K = (long)strlen(A[I]+2)+2;
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

				Function = K;
				if(F) ERR = 2;
				F = K;
				break;

				case 'O': /*OUTPUT*/
				K = (long)strlen(A[I]+2);
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
			K = (long)strlen(A[I]);
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

	return ERR; 
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
				LFuEvaluaterLoop(Function, classifyType, log_graph); 
			} 
		} 
	} 

	MaSCOT_IO.FCloseAll();
} 


F_EVAL_JOB::F_EVAL_JOB(short N, char*A[], int log_graph) { 
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


void DbWide_EliminateOverlaps(long N = 0, char *Names[] = 0) { 
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
		char*a[3] = {"", (char*)layers_todo[x].c_str(), "-FOVR"}; 
//		F_EVAL_JOB JOB(3, a, 1);
//		graph_wait_for_user_action(); 
		F_EVAL_JOB JOB(3, a);
	} 
} 


extern char*MASLogbuffer; 

BOOL Load_Database() { 
	MaSCOT_IO.funlink_all(); 

	WIN32_FIND_DATAA stFindData; 
	char *strFileName = "M2*.xzq"; 
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


void dosmain(short N, char*A[]) { 
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
	dosmain(args.N, args.A); 
	return 0; 
} 




extern long int ZPTLim_threshold; 
extern long int ZVTLim_threshold; 


bool MASPreload_StaticLayers(long N, char *Names[]) { 
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


DWORD WINAPI test_preload(LPVOID pArgs) { 
	char*a[2] = {"", "3"}; 
	MASPreload_StaticLayers(2, a); 

	unsigned short n = MaSCOT_IO.iLnames_hash.Exist("2"); 
	if(n == 0) { 
		return 1; 
	} 

	n = MaSCOT_IO.ILayer_Cnt++; 

	strcpy(MaSCOT_IO.I_Layer[n].Name, "2");

	MaSCOT_IO.Load_ILayer_Identity(n); 

	MaSCOT_IO.INIT_ILayer(n);

	C_Layer = MaSCOT_IO.I_Layer[n].Lcode;
	C_Fragment = 0; 
	MaSCOT_IO.D_Link(n);
	while(MaSCOT_IO.I_Fragment(n)==C_Fragment) {
		MaSCOT_IO.I_READ_P();
		MaSCOT_IO.MASLayerAddRTPolygon("2"); 
	}
	MaSCOT_IO.I_Layer[n].Fragment = C_Fragment = 0; 

	LFuInitialize_S_Factor(); 

	LFuInitialize_OutLayer("X"); 


	Target_Function = MFunction_Or;

	STATUS =+1;
	if(Target_Function==MFunction_Not) {
		Load_Boundary();
	}
	__pAm__ = ZPTLIM;

	ScalePoints();

	STATUS =+1;
	INTERSECTION_ANALYSIS(true); 

	POINT_VECTOR_ANALYSIS(ZVTLim_threshold);

	Classify_Polygons(0); 

	if(Target_Function<0) True_Stat = Target_Function==-1? AND_1:XOR_1;
	else True_Stat = Cmp_Modules[Target_Function];

	FORCE_C_POLYGONS();

	for(long I = 1; I<ZLBLIM; ++I) {
		LbBead*Lb = GetLbBead(I);
		if(Lb->ZLBFirstLvBead)DUMMYJUNCTIONANALYSIS(Lb);
	}

	graph_log_layer(MaSCOT_IO.O_Layer.Name); 

	Back_ScalePoints();

	WRITE_C_POLYGONS();

	graph_wait_for_user_action(); 

	return 0; 
} 


