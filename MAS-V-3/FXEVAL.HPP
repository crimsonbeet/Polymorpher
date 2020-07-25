extern char __Tail_follow__;
extern char __Head_follow__;

extern long __ZLDFI__;
extern long __ZLDLA__;

extern char __aLlAng__;

extern char __INTOPLD__;
extern char SetUp_Min_Max;
extern char __inTOPlt__;
/**/
extern long UN001;
extern long UN002;
extern long EMSIZ;
extern long LVCNT;
extern long BLCNT;
extern long UBSIZ;
extern long XBSIZ;
extern long YBSIZ;
extern long XBCNT; /*TOTAL-BLOCKS-IN-X-DIRECTION*/
extern long YBCNT; /*TOTAL-BLOCKS-IN-Y-DIRECTION*/
extern long CXMIN; /*MIN-X-COORD-ON-CHIP*/
extern long CXMAX; /*MAX-X-COORD*/
extern long CYMIN; /*MIN-Y-COORD-ON-CHIP*/
extern long CYMAX; /*MAX-Y-COORD*/
extern double STSIZ;
extern double OutDeltaX;
extern double OutDeltaY;
/**/
extern long CFragment;
extern long XFragment;
extern unsigned short ILayer_Cnt;
/**/
struct Flashp
{
	long Cx;
	long Cy;
	unsigned short Wd;
	unsigned short Hg;
	unsigned short An;
	unsigned short Zn;
}	;
struct Obj_Line
{
	long x1;
	long y1;
	long x2;
	long y2;
	char status;
}	;
struct Coord5089
{
	short x;
	unsigned char y;
}	;
struct W_ITEM
{
	char npoints;
	short nblock;
	short rang_x;
	short step_x;
	long x0;
	unsigned char y0;
	Coord5089 coord[13];
}	;
extern Obj_Line Obj_LInfinite;
struct LZ_Struct
{
	long EMSIZ; /*In PAGES BY 1K*/
	long UN001; /*Units_Per_Micron*/
	long UN003; /*Delta Window In Units*/
	long CXMIN; /*In Units*/
	long CXMAX; /*In Units*/
	long CYMIN; /*In Units*/
	long CYMAX; /*In Units*/
	long XBSIZ; /*In Units*/
        long YBSIZ; /*In Units*/
        long XBCNT;
        long YBCNT;
        long LVCNT; /*Vectors Count on Block*/
        long FiLID;
        char FileP[4];
        char Err1P[4];
        char PlotP[4];
        char TempP[4];
}       ;
/**/
struct LX_Struct
{
        long STATUS;

        long CXMIN;
        long CXMAX;
        long CYMIN;
        long CYMAX;

        long XBCNT;
        long YBCNT;
        long XBSIZ;
        long YBSIZ;

        long ER1I;
        long PLTI;
        long BDBI;
        long PDBI;
}       ;
/**/
struct LF_Struct
{
        FILE*Fil;
        char*Buf;
        long Buf_Len;
        long Buf_Pos;
        char Buf_Eof;
}       ;
struct FX_Struct
{
        long Fragment;

        LX_Struct LX_S;
        LF_Struct File;

        char Name[20];
        char PdbN[20];
        char BdbN[20];
        char Er1N[20];
        char PltN[20];

        long Delta_X;
        long Delta_Y;

        long Lcode;
        long Lmask;

        char Exist;

        FILE*ER1;
        FILE*ER2;
}       ;
class MASCOT_IO
{
	public:
	long ZZ_RP_KB;
	/**/
	long DIX_OpenS;
	char DIX_LineS;
	long DIX_Layer;
	long Dix_Layer;

	char*Dix_Out_Buf;
	long Dix_Out_Pos;
	long Dix_Out_Len;
	long Dix_Out_Cnt;

	FILE*Dix_File;
	char*Dix_Name;
	/**/
	char Rewrite_FP_S; /*for F_SIZE_MODULE only.*/
	/**/
	char*DFile_Buf;
	long DFile_Ptr;
	long DFile_Eof;
	long DFile_Lim;

	FILE*DFile;
	long*DFragment;
	long*DLayer;

	long DDelta_X;
	long DDelta_Y;
	/**/
        struct FP_Struct
        {
		char Id_String[32];

		long Delta_X; /*In Units*/
		long Delta_Y; /*In Units*/
        }	;
	LZ_Struct LZ_S;
	LX_Struct LX_S;

	FP_Struct FP_S;

	long FPOS_1;
	long FPOS_2;
	long FPOS_3;

	char LZ_S_Changed;

	FILE*LZ_S_FILE;
	FILE*LX_S_FILE;
	char*LZ_S_NAME;
	char*LX_S_NAME;
	/**/
	long S_BPOS;
	long S_BLEN;
	char*S_BTOP;
	/**/
	struct MM_layer
	{
		char*N;
		char X;
	}	;
	MM_layer*MM_Layer;
	/**/
	long MM_Layer_Old;
	long MM_Layer_Lim;
	long MM_Layer_Max;
	unsigned char*MM_Hash;
	/**/
	~MASCOT_IO(){};
	MASCOT_IO();
	void Dix_write(void*Buf, long Count);
	void Dix_close(void);
	void(*Dix_special)(void);
	void Dix_output_line(void);
	void Dix_output(void);
	char ZDNEXT(void);
	void ZDREAD(char*B, char L);
	void ZDREAD_X(long*D, char Z);
	char ZDREAD_L(Obj_Line&Obj);
	void ZDREAD_P(void);
	char*SS_Fetch(char*Name); /*puts to buffer and returns pointer.*/
	char ZZ_Hash(char*s); /*evaluates hash number.*/
	long MM_Fetch(char*Name);
	long MM_Insert(char*N);
	void SetMinMax(void);
	void Setup(void);
	void Make_File_Name(char*Sd, long Id);
	void Make_File_Name_Main(char*Sd, long Id);
	void Make_PLT_File_Name(char*Sd, long Id);
	void Make_BDB_File_Name(char*Sd, long Id);
	void SetAbsDelta(long);
	void Find_ILayer(void);
	void Init_ILayer_File(FX_Struct*L);
	void INIT_OLayer(void);
	void Close_ILayer(long n);
	void INIT_ILayer(long n);
	void renameoutput(void);
	void D_Link(long n);
	void D_BACKLink(long n);
	void CloseLzLx(void);
	void FCloseAll(void);
}	;
extern MASCOT_IO MaSCOT_IO;
extern FX_Struct I_Layer[4];
extern FX_Struct O_Layer;

char FindxFile(char*Name);
void Delete_File(char*Name);
void Ferror(FILE*File, char*Name);
FILE*Fopen(char*Name, char*Mode);

#define I_XBCNT(n) (I_Layer[(n)].LX_S.XBCNT)
#define I_YBCNT(n) (I_Layer[(n)].LX_S.YBCNT)
#define I_XBSIZ(n) (I_Layer[(n)].LX_S.XBSIZ)
#define I_YBSIZ(n) (I_Layer[(n)].LX_S.YBSIZ)
#define I_CXMIN(n) (I_Layer[(n)].LX_S.CXMIN)
#define I_CXMAX(n) (I_Layer[(n)].LX_S.CXMAX)
#define I_CYMIN(n) (I_Layer[(n)].LX_S.CYMIN)
#define I_CYMAX(n) (I_Layer[(n)].LX_S.CYMAX)

#define __ILay_File__(s) (I_Layer[(s)].File.Fil)
#define __ILay_FBuf__(s) (I_Layer[(s)].File.Buf)
#define __ILay_BPos__(s) (I_Layer[(s)].File.Buf_Pos)
#define __ILay_BLen__(s) (I_Layer[(s)].File.Buf_Len)
#define __ILay_BEof__(s) (I_Layer[(s)].File.Buf_Eof)
#define __ILay_Frag__(s) (I_Layer[(s)].Fragment)
