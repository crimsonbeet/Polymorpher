/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#ifndef fxevalH

#include <string>

#ifndef ipcgateH
template <int N> struct array_size_struct {
	unsigned char c[N];
};

template <class T, int N>
	array_size_struct<N> const& static_array_size(T (&)[N]);

#define ARRAY_NUM_ELEMENTS(ar) sizeof(static_array_size(ar).c)
#endif

	
extern char __aLlAng__;
/**/
extern long XBSIZ;
extern long YBSIZ;
extern long XBCNT; /*TOTAL-BLOCKS-IN-X-DIRECTION*/
extern long YBCNT; /*TOTAL-BLOCKS-IN-Y-DIRECTION*/
extern long CXMIN; /*MIN-X-COORD-ON-CHIP*/
extern long CXMAX; /*MAX-X-COORD*/
extern long CYMIN; /*MIN-Y-COORD-ON-CHIP*/
extern long CYMAX; /*MAX-Y-COORD*/
extern double OutDeltaX;
extern double OutDeltaY;
/**/
extern long C_Fragment;
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
/**/
struct MASCOTFileHeader_Struct
{
	long EMSIZ; /*In PAGES BY 1K - unused*/
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
	long LVCNT; /*Vectors Count on Block - input parameter - unused*/
	long FiLID;
	char FileP[4];
	char Err1P[4];
	char PlotP[4];
	char TempP[4];
};
struct MASCOTFileIdentity_Struct
{
	char MAS_File_Identification[32];

	long Delta_X; /*In Units*/
	long Delta_Y; /*In Units*/

	void Initialize() { 
		memset(this, 0, sizeof(MASCOTFileIdentity_Struct)); 
		#pragma warning(suppress : 4996)
		strcpy(MAS_File_Identification, "Mascot V2.1 Internal Storage\n");
	} 
};
struct LayerIdentity_Struct
{
	long layer_STATUS;

	long CXMIN;
	long CXMAX;
	long CYMIN;
	long CYMAX;

	long XBCNT;
	long YBCNT;
	long XBSIZ;
	long YBSIZ;

	long _1;
	long _2;

	long lastWrittenFragment; 

	long PDBI;

	LayerIdentity_Struct operator << (const MASCOTFileHeader_Struct& header) { 
		CXMIN = header.CXMIN;
		CXMAX = header.CXMAX;
		CYMIN = header.CYMIN;
		CYMAX = header.CYMAX;

		XBCNT = header.XBCNT;
		YBCNT = header.YBCNT;
		XBSIZ = header.XBSIZ;
		YBSIZ = header.YBSIZ;

		layer_STATUS = 0; 

		lastWrittenFragment = -1;

		return *this; 
	} 
	void Initialize() { 
		XBCNT = 1; 
		YBCNT = 1; 
		XBSIZ = 0;
		YBSIZ = 0;

		CXMIN = LONG_MAX;
		CYMIN = LONG_MAX;
		CXMAX =-LONG_MAX;
		CYMAX =-LONG_MAX;

		layer_STATUS = 0; 

		lastWrittenFragment = -1;
	} 
};
/**/
struct LayerDBHeader_Struct
{
	long Fragment;

	LayerIdentity_Struct _lIdentity;
	XMemoryFile* File;

	char Name[20];
	char PdbN[20];
	char BdbN[20];
	char Er1N[20];
	char PltN[20];

	long Delta_X;
	long Delta_Y;

	unsigned long Lcode;
	unsigned long Lmask;

	unsigned char Exist;

	//FILE*ER1;
	//FILE*ER2;
	XMemoryFile*ER1;
	XMemoryFile*ER2;
};

struct MASCOTStringBuffer { 
	long S_BPOS;
	long S_BLEN;
	char*S_BTOP;

	long s_bposRest; 

	MASCOTStringBuffer(); 
	~MASCOTStringBuffer(); 

	char*Fetch(char*str);
	char*Get(size_t len); 
}; 
struct MASCOTStringHash { 
	struct MHString
	{
		const char*N;
		unsigned char X;
	};

	MHString*Strings;
	unsigned long Strings_Lim;
	unsigned long Strings_Max;

	unsigned char*Hash;

	char NHash(const char*str); /*evaluates hash number.*/
	unsigned char Exist(const char*str);
	unsigned char Insert(const char*str);

	MASCOTStringHash(MASCOTStringBuffer&strbuf); 
}; 
class MASCOT_IO {
public:
	/**/
	long DIX_Written_Open_Shape;
	long DIX_Written_Layer;
	long DIX_Layer;

	long _iLayer; 

	long DDelta_X;
	long DDelta_Y;
	/**/
	MASCOTFileHeader_Struct _header;
	MASCOTFileIdentity_Struct _fileIdentity;

	fpos_t FPOS_1;
	fpos_t FPOS_2;
	fpos_t FPOS_3;

	char _dbHeader_Changed;

	XMemoryFile* xfiles[256]; 
	#define xfiles_max (ARRAY_NUM_ELEMENTS(xfiles)) 

	//FILE*_dbHeader_FILE;
	//FILE*_dbLayers_FILE;
	XMemoryFile*_dbHeader_FILE;
	XMemoryFile*_dbLayers_FILE;
	char*_dbHeader_NAME;
	char*_dbLayers_NAME;

	MASCOTStringBuffer strbuf; 
	MASCOTStringHash iLnames_hash; 

	long ILayers_LimOld;

	LayerDBHeader_Struct I_Layer[10]; 
	LayerDBHeader_Struct O_Layer; 
	LayerDBHeader_Struct P_Layer;

	unsigned short ILayer_Cnt;


	~MASCOT_IO(){};
	MASCOT_IO();

	void*operator new(size_t Size); 
	void operator delete(void *p); 

	XMemoryFile* ffind(const char*name); 
	XMemoryFile* fopen(const char*name);
	int funlink(const char*name);
	int funlink_all(); 
	int frename(const char*oldname, const char*newname); 

	void Dix_write(void*Buf, long Count);
	void Dix_close(char do_save_identity = 0);
	void Dix_output(char do_change_chip_dimensions = 0, LayerIdentity_Struct*lDim = 0, char open_shape = 0);
	void I_READ(char*B, char L);
	void I_READ_X(long*D, char Z);
	char I_READ_L(Obj_Line&Obj);
	char I_READ_P(char UseSecondaryLDSet = 0, char SetUp_Min_Max = 0);
	char I_SKIP_P(void); 
	void SetMinMax(void);
	void Setup(void);
	void Make_File_Name(char*Sd, long Id);
	void Make_File_Name_Main(char*Sd, long Id);
	void Make_PLT_File_Name(char*Sd, long Id);
	void Make_BDB_File_Name(char*Sd, long Id);
	void Make_OUT_File_Name(char*Sd, long Id);
	void SetAbsDelta(long);
	long Find_ILayer(const char*name); // returns -1 if not found, else pos + 1, thus never zero.
	void Activate_ILayers(void);
	void Load_ILayer_Identity(int n);
	void Init_ILayer_File(LayerDBHeader_Struct*L);
	void INIT_ILayer(long n);
	void INIT_OLayer(void);
	void D_Link(long nLayer, long dx = 0, long dy = 0);
	void renameoutput(void);
	void Close_ILayer(long n);
	void FCloseAll(void);

	bool MASEvaluateMain(long Function, const char* outLayer_name, int size_param = 0);
	bool MASLayerAddPolygon2DBStorage(const char* inpLayer_name); 
	bool MASLayerAddRTPolygon(const char* inpLayer_name); 

	bool MASInitialize(long blocks_count_x, long blocks_count_y); 
	bool MASGrid(long blocks_count_x, long blocks_count_y); 

	bool MASLayerCreateDBStorage(const char* inpLayer_name); 
	bool MASLayerAddPolygon2DBStorage(const char* inpLayer_name, long* points_x, long* points_y, long points_count); 
	bool MASLayerAddPolygon2DBStorage(const char* inpLayer_name, _sAlong& points_x, _sAlong& points_y); 
	bool MASEvaluate1(MAS_Function action, const char* inpLayer_name, const char* outLayer_name); 
	bool MASEvaluate2(MAS_Function action, const char** inpLayers, long inpLayers_count, const char* outLayer_name, int size_param = 0);
	void MASReleaseRTStorage();
	long MASLayerCountRTPolygons(const char* outLayer_name, size_t* first_polygon_number = 0); 
	long MASLayerGetRTPolygon(const char* outLayer_name, long polygon_number, long* points_x, long* points_y, long points_count); 
	long MASLayerGetRTPolygon(const char* outLayer_name, long polygon_number, _sAlong& points_x, _sAlong& points_y); 

	bool MASLayerAddRTPolygon(const char* inpLayer_name, long* points_x, long* points_y, long points_count); 
	bool MASLayerAddRTPolygon(const char* inpLayer_name, _sAlong& points_x, _sAlong& points_y); 
};
extern MASCOT_IO& MaSCOT_IO;

#define I_XBCNT(n) I_Layer[(n)]._lIdentity.XBCNT
#define I_YBCNT(n) I_Layer[(n)]._lIdentity.YBCNT
#define I_XBSIZ(n) I_Layer[(n)]._lIdentity.XBSIZ
#define I_YBSIZ(n) I_Layer[(n)]._lIdentity.YBSIZ
#define I_CXMIN(n) I_Layer[(n)]._lIdentity.CXMIN
#define I_CXMAX(n) I_Layer[(n)]._lIdentity.CXMAX
#define I_CYMIN(n) I_Layer[(n)]._lIdentity.CYMIN
#define I_CYMAX(n) I_Layer[(n)]._lIdentity.CYMAX

#define I_File(s) (I_Layer[(s)].File)
#define I_File_Eof(s) (feof(I_Layer[(s)].File))
#define I_Fragment(s) I_Layer[(s)].Fragment

#define O_File (O_Layer.File)
#define O_FileName (O_Layer.PdbN)
#define O_Fragment (O_Layer._lIdentity.lastWrittenFragment)


size_t fread(void*desc, size_t elemsize, size_t count, XMemoryFile*xfile); 
size_t fwrite(const void*src, size_t elemsize, size_t count, XMemoryFile*xfile); 
int fclose(XMemoryFile*xfile); 
int fflush(XMemoryFile*xfile); 
int ferror(XMemoryFile*xfile); 
int feof(XMemoryFile*xfile); 
fpos_t fgetpos(XMemoryFile*xfile);
int fsetpos(XMemoryFile*xfile, const fpos_t *pos); 
int fchsize(XMemoryFile*xfile, const size_t newsize); 
//XMemoryFile* fopen(const char*name); 
//XMemoryFile* ffind(const char*name); 
//int funlink(const char*name); 
//int funlink_all(); 
//int frename(const char*oldname, const char*newname); 


long Layer_Name2Pos(const char* layer_name); 

void graph_wait_for_user_action(); 
void graph_log_layer(const char* layer_Name, int lb = 0, int layer = -1); 
void graph_log_polygons(const char* file_name, long offsX, long offsY); 
void graph_log_ZLDLIM(const char* file_name, long offsX, long offsY); 

std::string itostdstring(int x, int width = 0); 
void FileLog(const std::string& msg, bool do_close = false); 

void WriteStream2file(std::ostringstream* pOstr, const std::string& file_name = "");

#endif
