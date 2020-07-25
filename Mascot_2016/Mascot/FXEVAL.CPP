/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#include "stdafx.h" 

void zabort(void);

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>
#include<IO.H>
#include<dos.h>
#include<math.h>

#include "fqeval.hpp"
#include "FXEVAL.HPP" 
#include "GridModule.hpp"

#define l_abs(a) ((a)<0? -(a): (a))

const char*fxeval="fxeval";
const char*OvrlpMstPrf="\n!!! NOTE! OVERLAP ELIMINATION MUST BE PERFORMED.";


size_t fread(void*desc, size_t elemsize, size_t count, XMemoryFile*xfile) { 
	return xfile->read((char*)desc, elemsize, count); 
} 
size_t fwrite(const void*src, size_t elemsize, size_t count, XMemoryFile*xfile) { 
	return xfile->write((const char*)src, elemsize, count); 
} 
int fchsize(XMemoryFile*xfile, const size_t newsize) { 
	xfile->chsize(newsize); 
	return 0; 
} 
fpos_t fgetpos(XMemoryFile*xfile) { 
	return xfile->getpos(); 
} 
int fsetpos(XMemoryFile*xfile, const fpos_t *pos) { 
	return xfile->setpos(pos); 
} 
int fclose(XMemoryFile*xfile) { 
	return 0; 
} 
int fflush(XMemoryFile*xfile) { 
	return 0; 
} 
int ferror(XMemoryFile*xfile) { 
	return 0; 
} 
int feof(XMemoryFile*xfile) { 
	return xfile->eof(); 
} 



XMemoryFile* MASCOT_IO::ffind(const char*name) { 
	XMemoryFile *xfile = 0; 
	for(int j = 0; j < xfiles_max && !xfile; ++j) { 
		if(xfiles[j]) { 
			if(*xfiles[j] == name) { 
				xfile = xfiles[j]; 
			} 
		} 
	} 
	return xfile; 
} 
XMemoryFile* MASCOT_IO::fopen(const char*name) { 
	XMemoryFile *xfile = ffind(name); 
	for(int j = 0; j < xfiles_max && !xfile; ++j) { 
		if(xfiles[j]) { 
			if(!xfiles[j]->isinitialized()) { 
				xfile = xfiles[j]; 
			} 
		} 
		else { 
			xfile = xfiles[j] = new XMemoryFile(); 
		} 
	} 
	if(xfile) { 
		xfile->open(name); 
	} 
	return xfile; 
} 
int MASCOT_IO::funlink(const char*name) { 
	int rc = 0; 
	for(int j = 0; !rc && j < xfiles_max; ++j) { 
		if(xfiles[j] && *xfiles[j] == name) { 
			xfiles[j]->unlink(); 
			rc = 1; 
		} 
	} 
	return rc; 
} 
int MASCOT_IO::funlink_all() { 
	int rc = 0; 
	for(int j = 0; j < xfiles_max; ++j) { 
		if(xfiles[j]) { 
			xfiles[j]->unlink(); 
			++rc; 
		} 
	} 
	return rc; 
} 
int MASCOT_IO::frename(const char*oldname, const char*newname) { 
	int rc = 0; 
	for(int j = 0; !rc && j < xfiles_max && xfiles[j]; ++j) { 
		if(*xfiles[j] == oldname) { 
			xfiles[j]->rename(newname); 
			rc = 1; 
		} 
	} 
	return rc; 
} 




void Ferror(XMemoryFile*File, const char*Name)
{
}
void Ferror(FILE*File, const char*Name)
{
	if(!File)return;

	if(ferror(File))
	{
		MASLog("\n\n!!! ERROR ON FILE '%s' !!!\n", Name);
		zabort();
	}
}
FILE*Fopen(char*Name, char*Mode)
{
	FILE*File;
	#pragma warning(suppress : 4996)
	File = fopen(Name, Mode);
	if(File==NULL)
	{
		MASLog("\n\n!!! FILE '%s' OPEN ERROR !!!\n", Name);
		zabort();
	}
	return(File);
}


char*MASLogbuffer; 
long MASLogbuffer_Lim; 
long MASLogbuffer_Max; 

void MASLog(const char * format, ...) { 
	char buffer[1024];

    va_list ap;

	va_start(ap, format);
	long ncount = vsnprintf(buffer, 1024, format, ap); 
	if(ncount < 0) { 
		ncount = 1024; 
	} 

	if((MASLogbuffer_Lim + ncount + 1) >= MASLogbuffer_Max) { 
		char*newMASLogbuffer = new char[(MASLogbuffer_Max + ncount) * 2]; 
		if(MASLogbuffer) { 
			memcpy(newMASLogbuffer, MASLogbuffer, MASLogbuffer_Lim + 1); 
			delete MASLogbuffer; 
		} 
		MASLogbuffer = newMASLogbuffer; 
	} 

	memcpy(MASLogbuffer + MASLogbuffer_Lim, buffer, ncount); 
	MASLogbuffer_Lim += ncount; 
	MASLogbuffer[MASLogbuffer_Lim] = 0; 

	va_end(ap);
} 


char __aLlAng__;
/**/
long XBSIZ;
long YBSIZ;
long XBCNT; /*TOTAL-BLOCKS-IN-X-DIRECTION*/
long YBCNT; /*TOTAL-BLOCKS-IN-Y-DIRECTION*/
long CXMIN; /*MIN-X-COORD-ON-CHIP*/
long CXMAX; /*MAX-X-COORD*/
long CYMIN; /*MIN-Y-COORD-ON-CHIP*/
long CYMAX; /*MAX-Y-COORD*/
/**/
double OutDeltaX;
double OutDeltaY;
/**/
long C_Fragment;
/**/

MASCOT_IO *ptrMaSCOT_IO = new MASCOT_IO();
MASCOT_IO& MaSCOT_IO = *ptrMaSCOT_IO;

void MASCOT_IO::Dix_write(void*Buf, long Count)
{
	fwrite(Buf, 1, Count, O_File);
}
void MASCOT_IO::Dix_close(char do_save_identity)
{
	if(do_save_identity)
	{
		fpos_t P = 0;
		fsetpos(O_File, &P);
		fwrite(&_fileIdentity, sizeof(_fileIdentity), 1, O_File);
	}
	fflush(O_File);
	fclose(O_File);
	Ferror(O_File, O_FileName);
	O_File = 0;
}
void MASCOT_IO::Dix_output(char do_change_chip_dimensions, LayerIdentity_Struct*lDim, char open_shape)
{
	long LD;
	long dx;
	long dy;
	long DX;
	long DY;
	long XL;
	long YL;
	long ZX;
	long ZY;
	long JJ;
	long fi;
	long la;
	long ic;
	long cn;
	char DS;
	char add_dl;
	static long OX;
	static long OY;
	if(DIX_Written_Open_Shape!=open_shape)
	{
		long D = 0;
		Dix_write(&D, 1);
		D = (open_shape!=0? 2: 1)<<4;
		Dix_write(&D, 1);
		DIX_Written_Open_Shape = open_shape;
	}
	if(DIX_Written_Layer!=DIX_Layer)
	{
		long D = 0;
		Dix_write(&D, 1);
		D = 128;
		Dix_write(&D, 1);
		Dix_write(&DIX_Layer, 2);
		DIX_Written_Layer = DIX_Layer;
	}
	if(O_Fragment!=C_Fragment)
	{
		long D = 0;
		Dix_write(&D, 1);

		if(C_Fragment<(0XFFL+1))	  D = 1;
		else if(C_Fragment<(0XFFFFL+1))   D = 2;
		else if(C_Fragment<(0XFFFFFFL+1)) D = 3;
		else			     D = 4;

		Dix_write(&D, 1);

		Dix_write(&C_Fragment, D);
		O_Fragment = C_Fragment;
	}
	dx = (long)floor(OutDeltaX+0.5);
	dy = (long)floor(OutDeltaY+0.5);
	if(dx||dy)
	{
		add_dl = 1;
	}
	else	add_dl = 0;
	OX = 0;
	OY = 0;
	cn = 0;
	la = ZLDLIM-1;
	if(STATUS>0)
	{
		fi = 0l;
		ic = +1;
	}
	else
	{
		fi = ZLDLIM-1;
		ic = -1;
	}
	for(LD = fi; cn<la; LD += ic, ++cn)
	{
		ZX = ZLDCO0[LD];
		ZY = ZLDCO1[LD];

		if(do_change_chip_dimensions)
		{
			if(ZX<CXMIN)CXMIN = ZX;
			if(ZX>CXMAX)CXMAX = ZX;
			if(ZY<CYMIN)CYMIN = ZY;
			if(ZY>CYMAX)CYMAX = ZY;
		}
		if(add_dl)
		{
			ZX += dx;
			ZY += dy;
		}
		if(lDim)
		{
			if(ZX<lDim->CXMIN)lDim->CXMIN = ZX;
			if(ZX>lDim->CXMAX)lDim->CXMAX = ZX;
			if(ZY<lDim->CYMIN)lDim->CYMIN = ZY;
			if(ZY>lDim->CYMAX)lDim->CYMAX = ZY;
		}
		DX = ZX - OX;
		DY = ZY - OY;

		OX = ZX;
		OY = ZY;

		ZX = ZY = 0;

		if(DX<=0) DX = -DX, ZX |= 0200;
		if(DY<=0) DY = -DY, ZY |= 0200;

		JJ = 0;

		if(DX)while(!(DX%10)&&JJ<7) ++JJ, DX/= 10;
		ZX |= JJ<<3;

		JJ = 0;

		if(DY)while(!(DY%10)&&JJ<7) ++JJ, DY/= 10;
		ZY |= JJ<<3;

		if(DX==0) XL = 0;
		else if(DX < (0XFFL+1)) XL = 1;
		else if(DX < (0XFFFFL+1)) XL = 2;
		else if(DX < (0XFFFFFFL+1)) XL = 3;
		else XL = 4;

		if(DY==0) YL = 0;
		else if(DY < (0XFFL+1)) YL = 1;
		else if(DY < (0XFFFFL+1)) YL = 2;
		else if(DY < (0XFFFFFFL+1)) YL = 3;
		else YL = 4;

		ZX |= XL;
		ZY |= YL;

		Dix_write(&ZX, 1L);
		if(XL)Dix_write(&DX, XL);

		Dix_write(&ZY, 1L);
		if(YL)Dix_write(&DY, YL);
	}
	if(la)
	{
		DS = 0, Dix_write(&DS, 1);
	}
	Ferror(O_File, O_FileName);
}
void MASCOT_IO::I_READ(char*B, char L)
{ 
	if(L <= 0) { 
		return; 
	} 
	size_t n = fread(B, 1, L, I_File(_iLayer)); 
	if(n < (size_t)L) { 
		memset(B + n, 0, (size_t)L - n); 
	} 
}
#define Z_LEN(Z) ((Z)&7)
#define Z_REV(Z) ((Z)&128)
#define Z_TEN(Z) (((Z)&56)>>3)
void MASCOT_IO::I_READ_X(long*D, char Z)
{
	(*D) = 0;
	I_READ((char*)D, Z_LEN(Z));

	if(Z_REV(Z)) (*D)*=-1L;

	for(Z = Z_TEN(Z); Z; --Z)(*D)*= 10;
}
char MASCOT_IO::I_READ_L(Obj_Line&Obj)
{
	I_READ((char*)&Obj, sizeof(Obj_Line));
	if(I_File_Eof(_iLayer))
	{
		Obj_Line Obj_LInfinite = {LONG_MAX-1,LONG_MAX-1,LONG_MAX,LONG_MAX};
		Obj = Obj_LInfinite;
		return 0;
	}
	return 1;
}
char MASCOT_IO::I_READ_P(char UseSecondaryLDSet, char SetUp_Min_Max) 
// Reads a polygon from storage. 
// Puts the points to ZLDCO0 and ZLDCO1, or into ZLDCO4 and ZLDCO5 based on parameter UseSecondaryLDSet (0 by default). 
{
	char Z;
	long L;
	long X;
	long Y;
	long Px;
	long Py;
	long D[2];
	char adddl;

	char open_shape = 0; 

	ZLDLIM = 0;
	I_READ(&Z, 1);
	if(!Z)
	{
		I_READ(&Z, 1), L = (Z&(7<<4))>>4;
		if(L)
		{
			if(L==8)
			{
				DIX_Layer = 0;
				I_READ((char*)&DIX_Layer, 2);
			}
			else
			if(L<=2)
			{
				open_shape = (char)(L-1);
			}
			else
			{
				zAbort(fxeval, __LINE__);
			}
			return open_shape;
		}
		I_Fragment(_iLayer) = 0;
		I_READ((char*)&I_Fragment(_iLayer), Z);
		if(I_File_Eof(_iLayer))I_Fragment(_iLayer) = LONG_MAX;
		return open_shape;
	}
	X = Y = 0;
	if(SetUp_Min_Max)
	{
		PXMIN = PYMIN = LONG_MAX;
		PXMAX = PYMAX =-LONG_MAX;
	}
	adddl = DDelta_X||DDelta_Y? 1: 0;
	do
	{
		I_READ_X(&D[0], Z);
		I_READ(&Z, 1);
		I_READ_X(&D[1], Z);
		X += D[0];
		Y += D[1];
		if(adddl)
		{
			Px = X-DDelta_X;
			Py = Y-DDelta_Y;
		}
		else	Px = X, Py = Y;
		if(!__aLlAng__)
		if(D[0]&&D[1])
		if(l_abs(D[0])!=l_abs(D[1]))
		{
			__aLlAng__ = ZLDLIM!=0;
		}
		L = (ZLDLIM++);
		if(UseSecondaryLDSet)
		{
			++L;
			ZLDCO4[L] = Px;
			ZLDCO5[L] = Py;
		}
		else
		{
			ZLDCO0[L] = Px;
			ZLDCO1[L] = Py;
		}
		if(SetUp_Min_Max)
		{
			if(PXMIN>Px)PXMIN = Px;
			if(PXMAX<Px)PXMAX = Px;
			if(PYMIN>Py)PYMIN = Py;
			if(PYMAX<Py)PYMAX = Py;
		}
		I_READ(&Z, 1);
	}
	while(Z);

	return open_shape; 
}
char MASCOT_IO::I_SKIP_P(void)
{
	char Z[4];
	I_READ(Z, 1);
	if(!Z[0])
	{
		return 0;
	}
	I_READ(Z, Z[0]&7);
	return 1;
}
MASCOTStringBuffer::MASCOTStringBuffer() { 
	S_BPOS = 1;
	S_BLEN = 8192;
	S_BTOP = new char[S_BLEN];
	memset(S_BTOP, 0, S_BLEN);

	MASLogbuffer_Lim = 0; 
	MASLogbuffer_Max = 2048; 
	MASLogbuffer = new char[MASLogbuffer_Max]; 
} 
MASCOTStringBuffer::~MASCOTStringBuffer() { 
	delete S_BTOP;
	delete MASLogbuffer; 
	MASLogbuffer = 0; 
} 
char*MASCOTStringBuffer::Fetch(char*str) {
	long i;
	char*n;
	i = strlen(str)+1;
	if((S_BLEN-S_BPOS)<=i)zAbort(fxeval,__LINE__);

	memcpy(&S_BTOP[S_BPOS], str, i);
	n = &S_BTOP[S_BPOS];
	S_BPOS+= i;

	return(n);
}
char*MASCOTStringBuffer::Get(size_t len) {
	if((size_t)(S_BLEN-S_BPOS)<=len)zAbort(fxeval,__LINE__);

	char*n = &S_BTOP[S_BPOS];
	S_BPOS+= (long)len;

	return(n);
}
char MASCOTStringHash::NHash(const char*s) {
	char i = 0;
	while(*s) i = (i + (*s++))&63;
	return(i);
}
unsigned char MASCOTStringHash::Exist(const char*str) {
	unsigned char I = Hash[NHash(str)];
	while(I)
	{
		#pragma warning(suppress : 4996)
		if(memicmp(str, Strings[I].N, strlen(Strings[I].N))==0)
		{
			break;
		}
		I = Strings[I].X;
	}
	return(I);
}
unsigned char MASCOTStringHash::Insert(const char*str) {
	unsigned char i = NHash(str);
	unsigned char I = Strings_Lim++;
	if(Strings_Lim>=Strings_Max)zAbort(fxeval,__LINE__);

	Strings[I].N = str;
	Strings[I].X = Hash[i];
	Hash[i] = I;

	return(I);
}
MASCOTStringHash::MASCOTStringHash(MASCOTStringBuffer&strbuf) { 
	Strings_Lim = 1;
	Strings_Max = 192;
	Strings = (MHString*)strbuf.Get(sizeof(MHString)*Strings_Max);

	memset(Strings, 0, sizeof(MHString)*Strings_Max);

	Hash = (unsigned char*)strbuf.Get(64);
} 

void MASCOT_IO::Make_File_Name_Main(char*Sd, long Id)
{
	long Ij;
	static const char*Sp = "abcxzjps";

	#pragma warning(suppress : 4996)
	sprintf(Sd, "%08ld", Id);

	for(Ij = 0; Sp[Ij]!=0; ++Ij)
	{
		if(Sd[Ij]== 0 )continue;
		if(Sd[Ij]=='0')Sd[Ij] = Sp[Ij];
		if(Sd[Ij]=='1')Sd[Ij] = 'e';
	}
	memcpy(Sd, _header.FileP, 3);
}
void MASCOT_IO::Make_PLT_File_Name(char*Sd, long Id)
{
	Make_File_Name_Main(Sd, Id);
	#pragma warning(suppress : 4996)
	strncat(Sd, ".xzp", 5);
}
void MASCOT_IO::Make_OUT_File_Name(char*Sd, long Id)
{
	Make_File_Name_Main(Sd, Id);
	#pragma warning(suppress : 4996)
	strncat(Sd, ".xzo", 5);
}
void MASCOT_IO::Make_BDB_File_Name(char*Sd, long Id)
{
	Make_File_Name_Main(Sd, Id);
	#pragma warning(suppress : 4996)
	strncat(Sd, ".xzb", 5);
}
void MASCOT_IO::Make_File_Name(char*Sd, long Id)
{
	Make_File_Name_Main(Sd, Id);
	#pragma warning(suppress : 4996)
	strncat(Sd, ".xzq", 5);
}
long MASCOT_IO::Find_ILayer(const char*name) { 
	for(size_t i = 0; i < ILayer_Cnt; ++i) {
		if(strcmp(I_Layer[i].Name, name) == 0) { 
			return i + 1; 
		} 
	} 
	return -1; 
} 
void MASCOT_IO::Load_ILayer_Identity(int i) { 
	I_Layer[i].Exist = iLnames_hash.Exist(I_Layer[i].Name);
	if(I_Layer[i].Exist) { 
		fpos_t n = FPOS_2 + (I_Layer[i].Exist-1)*sizeof(LayerIdentity_Struct);
		fsetpos(_dbLayers_FILE, &n);
		n = fread(&I_Layer[i]._lIdentity, sizeof(LayerIdentity_Struct), 1, _dbLayers_FILE);
		if(n!=1) { 
			zAbort(fxeval,__LINE__);
		} 
	} 
	else { 
		MASLog("\n!!! LAYER '%s' IS NOT VALID !!!", I_Layer[i].Name);
		zAbort(fxeval,__LINE__);
	} 
} 
void MASCOT_IO::Activate_ILayers(void) {
	for(int i = 0; i < ILayer_Cnt; ++i) {
		Load_ILayer_Identity(i); 
	}
}
void MASCOT_IO::Init_ILayer_File(LayerDBHeader_Struct*L)
{
	L->File = fopen(L->PdbN);
	if(L->File) { 
		fread(&_fileIdentity, sizeof(_fileIdentity), 1, L->File);
	} 
	else { 
		zAbort(fxeval,__LINE__);
	} 
	if(_fileIdentity.Delta_X == LONG_MAX || _fileIdentity.Delta_Y == LONG_MAX) { 
		_fileIdentity.Delta_X = _header.CXMIN; 
		_fileIdentity.Delta_Y = _header.CYMIN; 
		L->File->setpos(0); 
		fwrite(&_fileIdentity, sizeof(_fileIdentity), 1, L->File);
	} 

	L->Delta_X = _fileIdentity.Delta_X;
	L->Delta_Y = _fileIdentity.Delta_Y;
}
void MASCOT_IO::INIT_OLayer(void)
{
	O_Layer.Exist = 0; 
	O_Layer.Lcode = 0; 
	O_Layer.Lmask = 0; 

	O_Layer._lIdentity << _header;

	//tmpnam(O_Layer.PdbN);O_Layer.PdbN[0] = 'o'; 
	Make_OUT_File_Name(O_Layer.PdbN, 0); 

	//O_Layer.File = Fopen(O_Layer.PdbN, "w+b");
	O_Layer.File = fopen(O_Layer.PdbN);

	_fileIdentity.Delta_X = 0;
	_fileIdentity.Delta_Y = 0;

	fwrite(&_fileIdentity, sizeof(_fileIdentity), 1, O_Layer.File);

	DIX_Written_Open_Shape = 0;
	DIX_Written_Layer = -1;
	DIX_Layer = -1;

	C_Fragment = 0;

	OutDeltaX = 0;
	OutDeltaY = 0;
}
void MASCOT_IO::Close_ILayer(long n)
{
	LayerDBHeader_Struct*L = &I_Layer[n];
	fclose(L->File);
	Ferror(L->File, L->PdbN);
	L->File = 0;
}
void MASCOT_IO::INIT_ILayer(long n)
{
	LayerDBHeader_Struct*L = &I_Layer[n];
	Make_File_Name(L->PdbN, L->_lIdentity.PDBI);

	Init_ILayer_File(L);

	L->Lcode = 1<<(2*n);
	L->Lmask = (L->Lcode<<1)|L->Lcode;

	LYCODE[n] = L->Lcode;
	LYMASK[n] = L->Lmask;

	L->Fragment = 0;
}
void MASCOT_IO::renameoutput(void)
{
	fpos_t I;
	char Name[20];
	char BNam[20];

	O_Layer.Exist = iLnames_hash.Exist(O_Layer.Name);
	if(O_Layer.Exist)
	{
		I = FPOS_2 + sizeof(LayerIdentity_Struct)*(O_Layer.Exist-1);
		fsetpos(_dbLayers_FILE, &I);

		LayerIdentity_Struct lx_s; 
		I = fread(&lx_s, sizeof(LayerIdentity_Struct), 1, _dbLayers_FILE);
		if(I!=1)zAbort(fxeval,__LINE__);

		if(lx_s.PDBI)Make_File_Name(Name, lx_s.PDBI);
		else
		{
			_dbHeader_Changed = 1;
			Make_File_Name(Name, lx_s.PDBI = _header.FiLID++);
		}

		Make_BDB_File_Name(BNam, O_Layer._lIdentity.PDBI = lx_s.PDBI);
		//unlink(BNam);
		funlink(BNam);
	}
	else
	{
		O_Layer.Exist = iLnames_hash.Insert(strbuf.Fetch(O_Layer.Name));

		_dbHeader_Changed = 1;
		Make_File_Name(Name, O_Layer._lIdentity.PDBI = _header.FiLID++);
	}

	//unlink(Name);
	//rename(O_Layer.PdbN, Name);
	funlink(Name);
	frename(O_Layer.PdbN, Name);

	I = FPOS_2 + sizeof(LayerIdentity_Struct)*(O_Layer.Exist-1);
	fsetpos(_dbLayers_FILE, &I);

	I = fwrite(&O_Layer._lIdentity, sizeof(LayerIdentity_Struct), 1, _dbLayers_FILE);
	if(I!=1)zAbort(fxeval,__LINE__);

	fflush(_dbLayers_FILE);
}
void*MASCOT_IO::operator new(size_t Size) { 
	void* chunk = (void*)malloc(Size); 
	if(chunk) { 
		memset(chunk, 0, Size); 
	} 
	return chunk; 
} 

void MASCOT_IO::operator delete(void *p) { 
	free(p); 
} 

MASCOT_IO::MASCOT_IO(): strbuf(), iLnames_hash(strbuf) 
{
	_dbHeader_Changed = 0; 

	_dbHeader_NAME = strbuf.Get(20);
	_dbLayers_NAME = strbuf.Get(20);

	memset(&I_Layer[0], 0, sizeof(I_Layer)); 
	memset(&O_Layer, 0, sizeof(O_Layer)); 
	memset(&P_Layer, 0, sizeof(P_Layer)); 

	memset(xfiles, 0, sizeof(xfiles)); 

	MASInitialize(1, 1); 
}
void MASCOT_IO::SetMinMax(void)
{
	CXMIN = _header.CXMIN;
	CXMAX = _header.CXMAX;
	CYMIN = _header.CYMIN;
	CYMAX = _header.CYMAX;

	XBSIZ = _header.XBSIZ;
	YBSIZ = _header.YBSIZ;
	XBCNT = _header.XBCNT;
	YBCNT = _header.YBCNT;

	//XBCNT = (CXMAX - CXMIN) / 512; 
	//YBCNT = (CYMAX - CYMIN) / 512; 

	if(XBCNT<=0)XBCNT=1;
	if(YBCNT<=0)YBCNT=1;

	if(XBSIZ <= 0 || YBSIZ <= 0) { 
		XBSIZ = (CXMAX - CXMIN) / XBCNT; 
		YBSIZ = (CYMAX - CYMIN) / YBCNT; 
		if((XBSIZ * XBCNT) < (CXMAX - CXMIN)) ++XBSIZ;
		if((YBSIZ * YBCNT) < (CYMAX - CYMIN)) ++YBSIZ;

		_header.XBSIZ = XBSIZ; 
		_header.YBSIZ = YBSIZ;

		_dbHeader_Changed = 1;
	} 

	if((XBSIZ * XBCNT) < (CXMAX - CXMIN)) ++XBCNT;
	if((YBSIZ * YBCNT) < (CYMAX - CYMIN)) ++YBCNT;
}
void
MASCOT_IO::D_Link(long n, long dx, long dy)
{
	_iLayer = n; 

	DDelta_X = I_Layer[n].Delta_X;
	DDelta_Y = I_Layer[n].Delta_Y;

	DDelta_X = -dx;
	DDelta_Y = -dy;
}
void
MASCOT_IO::SetAbsDelta(long n)
{
	long XB;
	long YB;
	C_Fragment = I_Fragment(n);
	if(C_Fragment!=LONG_MAX)
	{
		DDelta_X = I_Layer[n].Delta_X-CXMIN;
		DDelta_Y = I_Layer[n].Delta_Y-CYMIN;

		XB = C_Fragment/I_YBCNT(n);
		YB = C_Fragment-(XB*I_YBCNT(n));

		DDelta_X -= XB*I_XBSIZ(n);
		DDelta_Y -= YB*I_YBSIZ(n);
	}
}
void MASCOT_IO::Setup(void)
{
	char b[200];
	long i;
	long m;
	long n;

	//_dbHeader_FILE = Fopen(_dbHeader_NAME, "r+b");
	//_dbLayers_FILE = Fopen(_dbLayers_NAME, "r+b");
	_dbHeader_FILE = fopen(_dbHeader_NAME);
	_dbLayers_FILE = fopen(_dbLayers_NAME);

	i = fread(&_fileIdentity, sizeof(_fileIdentity), 1, _dbHeader_FILE);
	m = fread(&_fileIdentity, sizeof(_fileIdentity), 1, _dbLayers_FILE);
	n = fread(&_header, sizeof(_header), 1, _dbHeader_FILE);

	if(i!=1||m!=1||n!=1)zAbort(fxeval,__LINE__);

	SetMinMax();

	fsetpos(_dbHeader_FILE, &FPOS_1);

	b[0] = 1;
	while(!feof(_dbHeader_FILE))
	{
		i = 0;
		do
		{
			if(!fread(&b[i], 1, 1, _dbHeader_FILE))b[i] = 0;
		}
		while(b[i++]);
		if(!b[0])break;
		if(!iLnames_hash.Exist(b)) { 
			i = iLnames_hash.Insert(strbuf.Fetch(b));
		} 
	}
	ILayers_LimOld = iLnames_hash.Strings_Lim;

	FPOS_3 = fgetpos(_dbHeader_FILE);
	Ferror(_dbHeader_FILE, _dbHeader_NAME);
}
void
MASCOT_IO::FCloseAll(void)
{
	long i;
	long I;

	if(P_Layer.File) { 
		fclose(P_Layer.File); 
		P_Layer.File = 0; 
	} 

	if(!_dbHeader_FILE) { 
		return; 
	} 

	for(I = 0; I<ILayer_Cnt; ++I)
	{
		if(I_File(I)) { 
			fclose(I_File(I));
		} 
		I_File(I) = 0; 
	}
	Dix_close();

	if(strlen(O_Layer.Name)) { 
		renameoutput();
	} 

	if(_dbHeader_Changed)
	{
		fsetpos(_dbHeader_FILE, &FPOS_2);
		fwrite(&_header, sizeof(_header), 1, _dbHeader_FILE);
		fflush(_dbHeader_FILE);
	}
	fsetpos(_dbHeader_FILE, &FPOS_3);
	for(i = ILayers_LimOld; i < iLnames_hash.Strings_Lim; ++i)
	{
		fwrite(iLnames_hash.Strings[i].N, 1, strlen(iLnames_hash.Strings[i].N) + 1, _dbHeader_FILE);
	}
	fflush(_dbHeader_FILE);
	//chsize(fileno(_dbHeader_FILE), (size_t)_dbHeader_FILE->getpos());
	fchsize(_dbHeader_FILE, (size_t)_dbHeader_FILE->getpos());
	fclose(_dbHeader_FILE);
	fclose(_dbLayers_FILE);
} 



bool MASInitialize(long blocks_count_x, long blocks_count_y) { 
	return MaSCOT_IO.MASInitialize(blocks_count_x, blocks_count_y); 
} 
bool MASGrid(long blocks_count_x, long blocks_count_y) { 
	return MaSCOT_IO.MASGrid(blocks_count_x, blocks_count_y); 
}

void MASReleaseRTStorage() { 
	MaSCOT_IO.MASReleaseRTStorage();
} 
bool MASLayerCreateDBStorage(const char* name) { 
	return MaSCOT_IO.MASLayerCreateDBStorage(name);
}
bool MASLayerAddPolygon2DBStorage(const char* inpLayer_name, long* points_x, long* points_y, long points_count) { 
	return MaSCOT_IO.MASLayerAddPolygon2DBStorage(inpLayer_name, points_x, points_y, points_count);
} 
bool MASLayerAddPolygon2DBStorage(const char* inpLayer_name, _sAlong& points_x, _sAlong& points_y) { 
	return MaSCOT_IO.MASLayerAddPolygon2DBStorage(inpLayer_name, points_x, points_y);
} 
bool MASEvaluate1(MAS_Function action, const char* inpLayer_name, const char* outLayer_name) { 
	return MaSCOT_IO.MASEvaluate1(action, inpLayer_name, outLayer_name);
} 
bool MASEvaluate2(MAS_Function action, const char** inpLayers, long inpLayers_count, const char* outLayer_name) { 
	return MaSCOT_IO.MASEvaluate2(action, inpLayers, inpLayers_count, outLayer_name);
} 
long MASLayerCountRTPolygons(const char* outLayer_name, size_t* first_polygon_number) { 
	return MaSCOT_IO.MASLayerCountRTPolygons(outLayer_name, first_polygon_number);
} 
long MASLayerGetRTPolygon(const char* outLayer_name, long polygon_number, long* points_x, long* points_y, long points_count) { 
	return MaSCOT_IO.MASLayerGetRTPolygon(outLayer_name, polygon_number, points_x, points_y, points_count);
} 
long MASLayerGetRTPolygon(const char* outLayer_name, long polygon_number, _sAlong& points_x, _sAlong& points_y) { 
	return MaSCOT_IO.MASLayerGetRTPolygon(outLayer_name, polygon_number, points_x, points_y);
} 




bool MASCOT_IO::MASInitialize(long blocks_count_x, long blocks_count_y) { 
	bool ok = false; 
	try { 
		O_Layer.Name[0] = 0; 
		FCloseAll(); 
		ok = true; 
	} 
	catch(char*excpt_msg) { 
		MASLog(excpt_msg); 
	} 
	if(!ok) { 
		return false; 
	} 

	ILayer_Cnt = 0; 
	memset(I_Layer, 0, sizeof(I_Layer)); 

	iLnames_hash.Strings_Lim = 1;
	strbuf.S_BPOS = 1;
	memset(strbuf.S_BTOP, 0, strbuf.S_BLEN);

	funlink_all(); 

	IEvaluater._ReleaseStorage();




	memset(&_header, 0, sizeof(_header)); 

	if(blocks_count_x <= 0) { 
		blocks_count_x = 1; 
	} 
	if(blocks_count_x <= 0) { 
		blocks_count_y = 1; 
	} 

	_header.XBCNT = blocks_count_x; 
	_header.YBCNT = blocks_count_y; 

	_header.CXMIN = CXMIN = LONG_MAX;
	_header.CYMIN = CYMIN = LONG_MAX;
	_header.CXMAX = CXMAX =-LONG_MAX;
	_header.CYMAX = CYMAX =-LONG_MAX;

	_header.UN001 = 1; 

	memcpy(_header.FileP, "M2$\n", 4);
	memcpy(_header.Err1P, "E1$\n", 4);
	memcpy(_header.PlotP, "PT$\n", 4);
	memcpy(_header.TempP, "TM$\n", 4);

	Make_File_Name(_dbHeader_NAME, 0);
	Make_File_Name(_dbLayers_NAME, 1);

	_header.FiLID = 2;

	_fileIdentity.Initialize(); 

	FPOS_1 = sizeof(_header)+sizeof(_fileIdentity);
	FPOS_2 = sizeof(_fileIdentity);

	_dbLayers_FILE = fopen(_dbLayers_NAME);
	_dbHeader_FILE = fopen(_dbHeader_NAME);

	fwrite(&_fileIdentity, sizeof(_fileIdentity), 1, _dbHeader_FILE);
	fwrite(&_fileIdentity, sizeof(_fileIdentity), 1, _dbLayers_FILE);

	fwrite(&_header, sizeof(_header), 1, _dbHeader_FILE);

	fclose(_dbHeader_FILE);
	fclose(_dbLayers_FILE);

	ILayers_LimOld = 1; 

	return true; 
} 

bool MASCOT_IO::MASGrid(long blocks_count_x, long blocks_count_y) { 
	if(_header.FiLID != 2) { 
		return false; 
	} 

	if(blocks_count_x <= 0) { 
		blocks_count_x = 1; 
	} 
	if(blocks_count_x <= 0) { 
		blocks_count_y = 1; 
	} 

	_header.XBCNT = blocks_count_x; 
	_header.YBCNT = blocks_count_y; 
	_header.XBSIZ = 0; 
	_header.YBSIZ = 0; 

	_dbHeader_FILE = fopen(_dbHeader_NAME);

	fsetpos(_dbHeader_FILE, &FPOS_2);
	fwrite(&_header, sizeof(_header), 1, _dbHeader_FILE);

	fclose(_dbHeader_FILE);

	return true; 
} 

void MASCOT_IO::MASReleaseRTStorage() { 
	IEvaluater._ReleaseStorage();
} 

bool MASCOT_IO::MASLayerCreateDBStorage(const char* name) { 
	if(iLnames_hash.Exist(name)) { 
		return false; 
	} 

	if(strlen(name) >= ARRAY_NUM_ELEMENTS(I_Layer[0].Name)) { 
		return false; 
	} 

	LayerDBHeader_Struct L;


	L._lIdentity.Initialize(); 

	Make_File_Name(L.PdbN, L._lIdentity.PDBI = _header.FiLID);
	if(ffind(L.PdbN)) { 
		return false; 
	} 
	_header.FiLID++; 

	_fileIdentity.Initialize(); 

	_fileIdentity.Delta_X = LONG_MAX; 
	_fileIdentity.Delta_Y = LONG_MAX; 

	L.File = fopen(L.PdbN);
	fwrite(&_fileIdentity, sizeof(_fileIdentity), 1, L.File);
	fclose(L.File); 
	L.File = 0; 


	_dbHeader_FILE = fopen(_dbHeader_NAME);
	_dbLayers_FILE = fopen(_dbLayers_NAME);

	#pragma warning(suppress : 4996)
	strncpy(L.Name, name, ARRAY_NUM_ELEMENTS(L.Name));

	fpos_t I = FPOS_2 + sizeof(LayerIdentity_Struct)*((L.Exist = iLnames_hash.Insert(strbuf.Fetch(L.Name))) - 1);
	fsetpos(_dbLayers_FILE, &I);
	fwrite(&L._lIdentity, sizeof(LayerIdentity_Struct), 1, _dbLayers_FILE);

	fsetpos(_dbHeader_FILE, &FPOS_3);
	for(long i = ILayers_LimOld; i < iLnames_hash.Strings_Lim; ++i)
	{
		fwrite(iLnames_hash.Strings[i].N, 1, strlen(iLnames_hash.Strings[i].N) + 1, _dbHeader_FILE);
	}

	fclose(_dbHeader_FILE); 
	fclose(_dbLayers_FILE); 

	return true;
} 


bool MASCOT_IO::MASLayerAddPolygon2DBStorage(const char* inpLayer_name) { 
	unsigned char n = MaSCOT_IO.iLnames_hash.Exist(inpLayer_name);
	if(n == 0) { 
		return false; 
	} 

	_dbLayers_FILE = fopen(_dbLayers_NAME);

	if(P_Layer.Exist != n) { 
		fpos_t I = FPOS_2 + sizeof(LayerIdentity_Struct)*(n - 1);
		fsetpos(_dbLayers_FILE, &I);
		fread(&P_Layer._lIdentity, 1, sizeof(LayerIdentity_Struct), _dbLayers_FILE);

		P_Layer.Exist = 0; 

		if(P_Layer._lIdentity.PDBI < 2 || P_Layer._lIdentity.PDBI >= MaSCOT_IO._header.FiLID) { 
			fclose(_dbLayers_FILE); 
			return false; 
		} 

		if(P_Layer._lIdentity.XBCNT > 1 || P_Layer._lIdentity.YBCNT > 1) { 
			fclose(_dbLayers_FILE); 
			return false; 
		} 

		P_Layer.Exist = n; 

		Make_File_Name(P_Layer.PdbN, P_Layer._lIdentity.PDBI);

		P_Layer.File = fopen(P_Layer.PdbN); 
	} 

	DIX_Written_Open_Shape = 0;
	DIX_Written_Layer = -1;
	DIX_Layer = -1;

	C_Fragment = 0;

	OutDeltaX = 0;
	OutDeltaY = 0;

	O_Fragment = P_Layer._lIdentity.lastWrittenFragment; 

	O_File = P_Layer.File; 
	O_File->setpos_end(); 
	Dix_output(1, &P_Layer._lIdentity); 
	O_File = 0; 

	P_Layer._lIdentity.layer_STATUS = 0; 
	P_Layer._lIdentity.lastWrittenFragment = O_Fragment; 

	fpos_t I = FPOS_2 + sizeof(LayerIdentity_Struct)*(n - 1);
	fsetpos(_dbLayers_FILE, &I);
	fwrite(&P_Layer._lIdentity, sizeof(LayerIdentity_Struct), 1, _dbLayers_FILE);

	fclose(_dbLayers_FILE); 

	_header.CXMIN = CXMIN;
	_header.CYMIN = CYMIN;
	_header.CXMAX = CXMAX;
	_header.CYMAX = CYMAX;

	_dbHeader_Changed = 1; 

	return true; 
} 


bool MASCOT_IO::MASLayerAddPolygon2DBStorage(const char* name, long* points_x, long* points_y, long points_count) { 
	if(points_x == 0 || points_y == 0 || points_count <= 0) { 
		return false; 
	} 

	ZLDCO0.fmemcpy((char*)points_x, points_count * sizeof(long)); 
	ZLDCO1.fmemcpy((char*)points_y, points_count * sizeof(long)); 

	ZLDLIM = points_count; 

	return MASLayerAddPolygon2DBStorage(name); 
} 

bool MASCOT_IO::MASLayerAddPolygon2DBStorage(const char* inpLayer_name, _sAlong& points_x, _sAlong& points_y) { 
	long points_count = (long)points_x.Size(); 

	if(points_count == 0 || points_count != (long)points_y.Size()) { 
		return false; 
	} 

	ZLDCO0 = points_x; 
	ZLDCO1 = points_y; 

	ZLDLIM = points_count; 

	return MASLayerAddPolygon2DBStorage(inpLayer_name); 
} 


bool MASCOT_IO::MASLayerAddRTPolygon(const char* name) { // This can work only with one grid-block. 
	int n = (int)iLnames_hash.Exist(name); 
	if(n == 0) { 
		if(MASLayerCreateDBStorage(name)) { 
			n = iLnames_hash.Exist(name); 
		} 
	} 
	if(n == 0) { 
		return false; 
	} 

	n = Find_ILayer(name); 
	if(n == -1 && ILayer_Cnt < ARRAY_NUM_ELEMENTS(I_Layer)) { 
		n = ILayer_Cnt++; 
		#pragma warning(suppress : 4996)
		strcpy(I_Layer[n].Name, "2");

		Load_ILayer_Identity(n); 

		INIT_ILayer(n);
	} 
	else { 
		--n; 
	} 

	long firstVt = ZVTLIM; 
	long firstPt = ZPTLIM; 

	for(long p = 0; p < ZLDLIM; ++p) {
		ZLDCO0[p] <<= S_FACTOR;
		ZLDCO1[p] <<= S_FACTOR;
	}

	C_Layer = I_Layer[n].Lcode;

	LbBead*Lb = Loadmaster(1); 
	if(!Lb) { 
		return false; 
	} 

	DETERMINE_CLOCKSTATUS(Lb); 

	if(STATUS == -1) {
		STATUS = 1; 
		Reverse_Lv_Chain(Lb); 
	} 

	return true; 
} 

bool MASCOT_IO::MASLayerAddRTPolygon(const char* name, long* points_x, long* points_y, long points_count) { 
	if(points_x == 0 || points_y == 0 || points_count <= 0) { 
		return false; 
	} 

	ZLDCO0.fmemcpy((char*)points_x, points_count * sizeof(long)); 
	ZLDCO1.fmemcpy((char*)points_y, points_count * sizeof(long)); 

	ZLDLIM = points_count; 

	return MASLayerAddRTPolygon(name); 
} 

bool MASCOT_IO::MASLayerAddRTPolygon(const char* inpLayer_name, _sAlong& points_x, _sAlong& points_y) { 
	long points_count = (long)points_x.Size(); 

	if(points_count == 0 || points_count != (long)points_y.Size()) { 
		return false; 
	} 

	ZLDCO0 = points_x; 
	ZLDCO1 = points_y; 

	ZLDLIM = points_count; 

	return MASLayerAddRTPolygon(inpLayer_name); 
} 



bool MASCOT_IO::MASEvaluateMain(long Function, const char* outLayer_name) { 
	bool rc  = true; 
	try {
		if(_dbHeader_Changed) {
			if(_dbHeader_FILE == 0) { 
				_dbHeader_FILE = fopen(_dbHeader_NAME);
			} 
			_dbHeader_Changed = 0;
		}
		if(_dbHeader_FILE) { 
			fsetpos(_dbHeader_FILE, &FPOS_2);
			fwrite(&_header, sizeof(_header), 1, _dbHeader_FILE);
			fflush(_dbHeader_FILE);
			fclose(_dbHeader_FILE);

			_dbHeader_FILE = 0; 
		} 
		Setup();
		Activate_ILayers();

		LFuInitialize(outLayer_name);
		switch(Function) { 
		case MFunction_Nothing: 
			break; 
		case MFunction_Area: 
			F_AREA_MAIN(I_Layer[0].Name);
			break; 
		default: 
			LFuEvaluaterLoop(Function); 
			break; 
		} 
	} 
	catch(char*excpt_msg) { 
		MASLog(excpt_msg); 
		rc = false; 
	} 

	try { 
		FCloseAll(); 
	} 
	catch(char*excpt_msg) { 
		MASLog(excpt_msg); 
		rc = false; 
	} 

	return rc; 
} 


bool MASCOT_IO::MASEvaluate1(MAS_Function action, const char* inpLayer_name, const char* outLayer_name) { 
	const char* in_layers[2] = {inpLayer_name, 0}; 
	return MASEvaluate2(action, in_layers, 1, outLayer_name); 
} 

bool MASCOT_IO::MASEvaluate2(MAS_Function action, const char** inpLayers, long inpLayers_count, const char* outLayer_name) { 
	int Function = action; 

	if(inpLayers == 0 || inpLayers_count <= 0) {
		MASLog("\nEVAL:No input Layers specified\n");
		return false;
	} 
	if(inpLayers_count > 4) {
		MASLog("\nEVAL:Too many input Layers\n");
		return false;
	} 
	if(Function==MFunction_Size)
	{
		MASLog("\nEVAL:Size is not implemented\n"); 
		return false;
	}
	if(Function==MFunction_5009)
	{
		MASLog("\nEVAL:5009 is not implemented\n");
		return false;
	}

	if(outLayer_name == 0) { 
		outLayer_name = inpLayers[0]; 
	} 
	if(inpLayers_count == 1 && Function == MFunction_And) {
		Function = MFunction_AndOne;
	}
	if(inpLayers_count == 1 && Function==MFunction_Xor)
	{
		Function = MFunction_XorOne;
	}
	if(inpLayers_count == 1 && Function==MFunction_AndNot)
	{
		Function = MFunction_Not;
	}

	for(int j = 0; j < inpLayers_count; ++j) { 
		if(inpLayers[j] == 0) {
			MASLog("\nEVAL:Bad layer name\n");
			return false;
		} 
		#pragma warning(suppress : 4996)
		strncpy(I_Layer[j].Name, inpLayers[j], sizeof(I_Layer[j].Name));
	} 
	ILayer_Cnt = (unsigned short)inpLayers_count;

	return MASEvaluateMain(Function, outLayer_name); 
} 


long Layer_Name2Pos(const char* layer_name) { 
	long n = -1; 
	if((char*)MaSCOT_IO.O_Layer.Name == (char*)layer_name) { 
		n = 0; 
	} 
	else { 
		n = MaSCOT_IO.Find_ILayer(layer_name); 
	} 
	if(n < 0 && strcmp(MaSCOT_IO.O_Layer.Name, layer_name) == 0) { 
		n = 0; 
	} 
	return n; 
} 

long MASCOT_IO::MASLayerCountRTPolygons(const char* layer_name, size_t* first_polygon_number) { 
	long n = Layer_Name2Pos(layer_name); 
	if(n < 0) { 
		return -1; 
	} 

	int cnt = 0; 
	if(ZLBLIM > 1) { 
		LayerDBHeader_Struct& L = n == 0? O_Layer: I_Layer[n - 1]; 

		if(n == 0 && GetLbBead(1)->ZLBLayer == L.Lcode) { 
			cnt = ZLBLIM - 1; 
			*first_polygon_number = 1; 
		} 
		else 
		for(int j = 1; j < ZLBLIM; ++j) { 
			if(GetLbBead(j)->ZLBLayer == L.Lcode) { 
				if(++cnt == 1) { 
					if(first_polygon_number) { 
						*first_polygon_number = j; 
					} 
				} 
			} 
		} 
	} 

	return cnt; 
} 

template<typename T>
long copy_inputXY(T& points_x, T& points_y, long points_count, long dx, long dy) { 
	for(int j = 0; j < ZLDLIM && j < points_count; ++j) { 
		points_x[j] = ZLDCO0[j] - dx; 
		points_y[j] = ZLDCO1[j] - dy; 
	} 
	return ZLDLIM; 
} 

/*
	Polygon number makes sense only when processing the output layer, i.e. the function has been performed.
	Otherwise the polygon number has to be known exactly. 
*/
long MASCOT_IO::MASLayerGetRTPolygon(const char* layer_name, long polygon_number, long* points_x, long* points_y, long points_count) { 
	long n = Layer_Name2Pos(layer_name); 
	if(n < 0) { 
		return -1; 
	} 

	if(points_x == 0 || points_y == 0 || points_count <= 0) { 
		return -1; 
	} 

	if(polygon_number < 1 || ZLBLIM <= polygon_number) { 
		return -1; 
	} 

	LayerDBHeader_Struct& L = n == 0? O_Layer: I_Layer[n - 1]; 

	long dx = L.Delta_X - _header.CXMIN; 
	long dy = L.Delta_Y - _header.CYMIN; 

	LbBead*Lb = GetLbBead(polygon_number); 
	if(Lb->ZLBLayer == L.Lcode) { 
		if(Lb->ZLBFirstLvBead) { 
			if(LbBead_Unload2InputXY(Lb)) { 
				return copy_inputXY(points_x, points_y, points_count, dx, dy); 
			} 
		} 
	} 

	return -1; 
} 

long MASCOT_IO::MASLayerGetRTPolygon(const char* layer_name, long polygon_number, _sAlong& points_x, _sAlong& points_y) { 
	long n = Layer_Name2Pos(layer_name); 
	if(n < 0) { 
		return -1; 
	} 

	if(polygon_number < 1 || ZLBLIM <= polygon_number) { 
		return -1; 
	} 

	LayerDBHeader_Struct& L = n == 0? O_Layer: I_Layer[n - 1]; 

	long dx = L.Delta_X - _header.CXMIN; 
	long dy = L.Delta_Y - _header.CYMIN; 

	LbBead*Lb = GetLbBead(polygon_number); 
	if(Lb->ZLBLayer == L.Lcode) { 
		if(Lb->ZLBFirstLvBead && Lb->ZLBFirstLvBead->ZLVMyVector) { 
			if(LbBead_Unload2InputXY(Lb)) { 
				return copy_inputXY(points_x, points_y, ZLDLIM, dx, dy); 
			} 
		} 
		else 
		if(ZLDLIM > 3) { 
			return copy_inputXY(points_x, points_y, ZLDLIM, dx, dy); 
		} 
	} 

	return -1; 
} 



DWORD WINAPI MASEvaluate_Test(LPVOID) { 
	const char* outLayer_Name = "X"; 

	long lb_count = 0; 
	lb_count = MASLayerCountRTPolygons(outLayer_Name); 

	if(lb_count <= 0) { 
		return 1; 
	} 

	_sAlong *lb_x = new _sAlong[lb_count]; 
	_sAlong *lb_y = new _sAlong[lb_count]; 

	for(int j = 1; j < lb_count; ++j) { 
		MASLayerGetRTPolygon(outLayer_Name, j, lb_x[j], lb_y[j]); 
	} 

	MASInitialize(10, 10); 

	MASLayerCreateDBStorage(outLayer_Name); 

	for(int j = 0; j < lb_count; ++j) { 
		MASLayerAddPolygon2DBStorage(outLayer_Name, lb_x[j], lb_y[j]); 
	} 

	const char* in_layers[2] = {outLayer_Name, "XNot"}; 

	MASEvaluate1(MAS_Not, in_layers[0], in_layers[1]); 

	MASEvaluate2(MAS_And, &in_layers[0], 2, "XAnd"); 

	lb_count = MASLayerCountRTPolygons(outLayer_Name); 

	delete[] lb_x;
	delete[] lb_y;

	lb_x = new _sAlong[lb_count]; 
	lb_y = new _sAlong[lb_count]; 

	for(int j = 1; j < lb_count; ++j) { 
		MASLayerGetRTPolygon(outLayer_Name, j, lb_x[j], lb_y[j]); 
	} 

	return 0; 
} 



