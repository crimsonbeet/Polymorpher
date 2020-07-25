  /**/
  #ifdef CHAR
  #undef CHAR
  #endif
  #ifdef SHORT
  #undef SHORT
  #endif
  #define CHAR unsigned char
  #define SHORT unsigned short
  /**/
  extern long*ZLDCO0; /*X-COORDINATE*/
  extern long*ZLDCO1; /*Y-COORDINATE*/
  extern long*ZLDCO2; /*X-COORDINATE*/
  extern long*ZLDCO3; /*Y-COORDINATE*/
  extern long*ZPTCO1; /*X-COORDINATE*/
  extern long*ZPTCO2; /*Y-COORDINATE*/
  extern char*ZLBCO7; /*ALREADY-CLASSIFIED-FLAG*/
  extern CHAR*ZPVCO3; /*PSEUDO-SLOPE*/
  extern char*ZVTCO8; /*TRAVERSAL-STATUS*/
  extern char*ZLBCO1; /*POLYGON-RIGHT/LEFT-STATUS*/
  extern CHAR*ZVTCO4; /*RIGHT-STATUS*/
  extern CHAR*ZLBCO4; /*LAYER-CODE*/
  extern SHORT*ZLbXx1; /*MIN-POINT-ON-POLYGON(AFTER-LOAD-MASTER)*/
  extern SHORT*ZLbXx2; /*MAX-X-POINT-ON-POLYGON*/
  extern SHORT*ZLbYy1; /*MIN-Y-POINT-ON-POLYGON*/
  extern SHORT*ZLbYy2; /*MAX-Y-POINT-ON-POLYGON*/
  extern SHORT*ZLBCO3; /*POINTER-TO-THE-FIRST-VECTOR-BEAD*/
  extern SHORT*ZLBCO6; /*POLYGON-NUMBER*/
  extern SHORT*ZLbNxt; /*NEXT-HASH-POLYGON*/
  extern SHORT*ZLVCO1; /*NEXT-POLYGON-VECTOR(LV)-BEAD(CHAIN HEAD IS ZLBCO3!CIRCULAR)*/
  extern SHORT*ZLVCO2; /*VECTOR-NUMBER*/
  extern SHORT*ZLVCO3; /*NEXT-COINCIDENT-VECTOR-PTR(CHAIN HEAD IS ZVTCO5)*/
  extern SHORT*ZLVCO5; /*FIRST-POINT*/
  extern SHORT*ZLVCO6; /*SECOND-POINT*/
  extern SHORT*ZLVCO7; /*PTR-TO-POLYGON-BEAD*/
  extern SHORT*ZLVCO8; /*PTR-TO-CLASS-POLYGON*/
  extern SHORT*ZPVCO5; /*POINTER-TO-NEXT-PV-BEAD-FOR-THIS-POINT(COUNTERCLOCKWISE)*/
  extern SHORT*ZPTCO3; /*PTR-TO-PV-BEAD(CHAIN HEAD)*/
  extern SHORT*ZPTCO5; /*NEXT-X-POINTER-IN-Y-CHAIN*/
  extern SHORT*ZVTCO1; /*FIRST-POINT-NUMBER*/
  extern SHORT*ZVTCO2; /*SECOND-POINT-NUMBER*/
  extern SHORT*ZVTCO5; /*PTR-TO-FIRST-POLYGON-VECTOR-BEAD-CHAIN-OF-COINCIDENT-VECTORS(IS NEVER ZERO)*/
  extern SHORT*ZVTCO6; /*ASSOCIATED-VECTOR(REVERSE DIRECTION)*/
  extern SHORT*ZJSCO0; /*LV-WILL-BE-PROCESSED-BY-SET-INSIDE*/
  extern SHORT*ZJSCO1; /*ENCLOSED-LB*/

  extern long**LLDCOR[3];
  extern long**Lldcor[3];
  extern long**LPTCOR[3];

  extern char**CVTCOR[3];
  extern char**CLBCOR[4];

  extern SHORT**ILBCOR[9];
  extern SHORT**ILVCOR[8];
  extern SHORT**IVTCOR[7];
  extern SHORT**IPTCOR[3];
  extern SHORT**IJSCOR[3];
  /**/
  extern long ZLSLIM;
  extern long ZLDLIM;
  extern long ZLDMAX;
  extern long ZLBLIM;
  extern long ZLBMAX;
  extern long ZLVLIM;
  extern long ZLVMAX;
  extern long ZPTLIM;
  extern long ZPTMAX;
  extern long ZYPLIM;
  extern long ZYPMAX;
  extern long ZPVLIM;
  extern long ZPVMAX;
  extern long ZVTLIM;
  extern long ZVTMAX;
  extern long ZJSLIM;
  extern long ZJSMAX;
  /**/
#ifdef M_CONV_MODULE
  #define Z_LD_Wt 0.8
  #define Z_LB_Wt 0.5
  #define Z_LV_Wt 1.0
  #define Z_VT_Wt 2.0
  #define Z_PT_Wt 1.0
  #define Z_JS_Wt 0.1
  #define Z_gV_Wt 1.0
  #define Z_VX_Wt 0.8
#else
  #define Z_LD_Wt 0.5
  #define Z_LB_Wt 0.5
  #define Z_LV_Wt 2.3
  #define Z_VT_Wt 4.0
  #define Z_PT_Wt 1.7
  #define Z_JS_Wt 0.1
  #define Z_gV_Wt 2.3
  #define Z_VX_Wt 0.5
#endif

  #define Z_gV_Sz 6 *Z_gV_Wt
  #define Z_LD_Sz 8 *Z_LD_Wt
  #define Z_VX_Sz 32*Z_VX_Wt
  #define Z_LB_Sz 3 *Z_LB_Wt + 14*Z_LB_Wt
  #define Z_LV_Sz 14*Z_LV_Wt
  #define Z_VT_Sz 2 *Z_VT_Wt + 10*Z_VT_Wt
  #define Z_PT_Sz 8 *Z_PT_Wt + 4 *Z_PT_Wt
  #define Z_JS_Sz 2 *Z_JS_Wt
  /**/
  extern long LYCODE[10]; /*LAYER-CODE*/
  extern long LYMASK[10]; /*LAYER-MASKA*/
  /**/
  extern long Target_Function; /*FUNCTION-CODE*/
  #define OVRELIM 0
  #define ZANDNOT 1
  #define ZZZANDJ 2
  #define ZZZXORJ 3
  #define ZZZZORJ 4
  #define ZZZNOTJ 5

  #define ZZZAND1 -1
  #define ZZZXOR1 -2
  /**/
  extern char SI_Error;
  extern char PV_Error;
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
  /**/
  extern double ZZ_DELTA_X;
  extern double ZZ_DELTA_Y;
  /**/
  extern double STSIZ;
  /**/
  extern char S_Free;
  extern long STATUS;
  extern long C_Layer;
  extern long Z_JFlag;
  extern long S_Factor;
  extern long S_FACTOR; /*M2CONV.C*/
  extern long ZZ_RP_KB;
  extern char DummyClip;
  extern long Z_Fragment;
  extern char Ia_Complex;
  extern long Z_Full_Scan;
  extern long Z_User_Block;
  extern long Z_Layers_Cnt;
  extern char Z_Open_Shape;
  extern char Without_Scale; /*M2CONV.C*/
  extern char Roll_Back_Intsect;
  extern long Zptlim_After_Loadmaster;
  /**/
  extern SHORT*ZPBCOR;
  extern long PBLIM;
  extern long PXCNT;
  extern long PYCNT;
  extern long XPDIV;
  extern long YPDIV;
  extern long PXMAX;
  extern long PYMAX;
  extern long PXMIN;
  extern long PYMIN;
  /**/
  extern long SI_LB, SI_V1, SI_V2, SI_ST;
  extern long SI_LE;
  extern char SI_FL, SI_IX_FL, SI_PA_FL;
  /**/
#ifdef F_EVAL_MODULE
  extern long Lb_Ymin;
  extern long Lb_Ymax;

  extern long Lv_Ymin;
  extern long Lv_Ymax;
  extern long Lv_Xmin;
  extern long Lv_Xmax;
  extern long Lv_This;

  extern long Vt_Ymax;
  extern long Vt_Ymin;
  extern long Vt_Last;
  extern long Vt_Auxx;

  extern long Lv_Pt1;
  extern long Lv_Pt2;
  extern long Lv_Xx1;
  extern long Lv_Yy1;
  extern long Lv_Xx2;
  extern long Lv_Yy2;

  extern char Lv_Swi;
  extern char Vt_Swi;
#endif
  /**/
  extern char Closed_Polygon;
  /**/
  extern long Delta_X;
  extern long Delta_Y;
  /**/
  extern long Sized_X;
  extern long Sized_Y;
  /**/
  extern SHORT*Lf_Rolld;
  extern SHORT*Lf_ROLLD;
  extern SHORT*Lf_First;
  extern SHORT*Lf_Stack;
  extern SHORT*Lf_Upper;
  extern SHORT*Lf_Lower;

  extern long*Lf_Ymidl;
  extern char*Lf_Sdown;
  extern char*Lf_Flags;

  extern char Lf_Factor;

  extern SHORT Lf_Lim;
  extern SHORT LF_LIM;
  /**/
  extern long DIX_Fragment;
  extern long DIX_Layer;
  extern long Dix_Layer;

  extern char*Dix_Out_Buf;
  extern long Dix_Out_Pos;
  extern long Dix_Out_Len;

  extern FILE*Dix_File;
  extern char*Dix_Name;
  /**/
  extern char*Zd_File_Buf;
  extern long Zd_File_Ptr;
  extern long Zd_File_Eof;
  extern long Zd_File_Lim;

  extern FILE*Zd_File;
  extern long*Zd_Fragment;
  extern long*Zd_Layer;

  extern long Zd_Delta_X;
  extern long Zd_Delta_Y;
  /**/
  typedef struct
  {
  char Id_String[32];

  long Delta_X; /*In Units*/
  long Delta_Y; /*In Units*/
  } FP_Struct;
  typedef struct
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
  long LVCNT;
  long FiLID;
  char FileP[4];
  char Err1P[4];
  char PlotP[4];
  char TempP[4];
  } LZ_Struct;
  /**/
  typedef struct
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
  } LX_Struct;
  typedef struct
  {
  FILE*Fil;
  char*Buf;
  long Buf_Len;
  long Buf_Pos;
  char Buf_Eof;
  } LF_Struct;
  typedef struct
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
  } FX_Struct;

  extern LZ_Struct LZ_S;
  extern LX_Struct LX_S;
  extern FX_Struct Z_Layer[4], O_Layer;

  extern FP_Struct FP_S;

  extern char LZ_S_Changed;

  extern FILE*LZ_S_FILE;
  extern FILE*LX_S_FILE;
  extern char*LZ_S_NAME;
  extern char*LX_S_NAME;
  /**/
  extern long S_BPOS;
  extern long S_BLEN;
  extern char*S_BTOP;
  /**/
  #define FETCHSUCCESSOR(VT) (ZPVCO5[(VT)]?ZPVCO5[(VT)]:ZPTCO3[ZVTCO1[(VT)]])
  #define ENVEQ(VA,VL,EN) (((VA)>(VL)-(EN))&&((VA)<(VL)+(EN)))
  #define zznoeven(V) ((V)-(((V)>>1)<<1))
  #define FetchReverseLv(LV) (ZVTCO5[ZVTCO6[ZLVCO2[(LV)]]])
  /**/
  void Zswab(long*, long*);
  #define ZSWAB Zswab
  extern FILE*ERRFIL;
  extern char*ERRNAM;
  extern void Print_Note_Message(void);
  void
  zabort(void);
  void
  ZOUTABORT(void);
  void
  ZALLOCATE(void**, long);
  void
  ZXXALLOC(void**X[], long L);
  void
  ZXXFREE(void**X[]);
  void
  ZLDALLOC(long);
  void
  ZldALLOC(long);
  void
  ZLBALLOC(long);
  void
  ZLVALLOC(long);
  void
  ZVTALLOC(long);
  void
  ZPTALLOC(long);
  void
  ZJSALLOC(long);
  long
  ZVTNEXT(void);
  long
  ZLVNEXT(void);
  long
  ZLBNEXT(void);
  long
  ZYPNEXT(void);
  long
  ZPTNEXT(void);
  long
  ZLDNEXT(void);
  void
  COMBINE_C_POLYGONS(void);
  typedef long ipoint(long, long);
  extern ipoint*IPOINT;
  /**/
  void
  Determine_Point_Base(void);
  long
  IPOINT0(long, long);
  long
  IPOINT3(long, long);
  long
  IPOINT1(long, long);
  double
  Eval_Angle(double, double);
  extern double sumangles;
  double
  SUB_ANGLES(double, double);
  double
  COMPLVANGLE(long);
  double
  COMPLDANGLE(long);
  long
  DETERMINE_STAT_1(void);
  long
  DETERMINE_STAT_2(long);
  void
  Z_FEval_Storage(void);
#ifdef F_EVAL_MODULE
  void
  QLb_Sort(long, long);
#endif
  long
  SPLITLV(long, long);
  long
  SPLITVT(long, long);
  long
  INSERTV(long, long, long);
  void
  INTERSECTION_ANALYSIS(void);
  void
  POINT_VECTOR_ANALYSIS(long);
  long
  FETCHITSELF(long, long);
  long
  FETCHREVERSE(long, long);
  void
  REMOVE_FROM_VT_CHAIN(long);
  long
  CLEAR_LB_PTR(long, long);
  long
  DUMMYJUNCTION(long, long);
  long
  SET_INITIAL_VECTOR(long);
  void
  DUMMYJUNCTIONANALYSIS(long);
  void
  Dix_close(void);
  void
  Dix_output(void);
  void
  Z_Read_Polygon(void);
  void
  Ferror(FILE*, char*);
  FILE*
  Fopen(char*, char*);
  char*
  SS_Fetch(char*);
  char
  ZZ_Hash(char*s);
  void
  Make_BDB_File_Name(char*, long);
  void
  Find_ILayer(void);
  void
  INIT_OLayer(FX_Struct*);
  void
  INIT_ILayer(FX_Struct*, long);
  void
  Delete_File(char*);
  void
  renameoutput(void);
  void
  Lz_Lx_Close(void);
  void
  Z_Check_Closure(void);
  typedef long compare(long);
  extern compare XOR_1;
  extern compare AND_1;
  extern compare AN_D;
  extern compare O_R;
  extern compare AND_NOT;
  extern long OVR_ELIM_MASK;
  extern compare OVR_ELIM;
  extern compare X_OR;
  extern compare NO_T;
  extern compare *True_Stat;
  extern compare *Cmp_Modules[6];
  void
  FORCE_C_POLYGONS(void);
  long
  Unload_Lv_Polygon(long);
  void
  Initial_Setup_1(void);
  void
  Initial_Setup_2(void);
  void
  SET_INSIDE_ALL(void);
#ifdef  F_SIZE_MODULE
#define F_EVAL_MODULE
#endif
#ifndef F_EVAL_MODULE
  void
  POINT_VECTOR_CLASS(long);
#else
  void
  Init_Vt_Hash_Tree(void);
  void
  POINT_VECTOR_CLASS(long, long, long);
  void
  SET_BIT_1(long);
  void
  SET_BIT_2(long);
#endif
#ifdef F_SIZE_MODULE
#undef F_EVAL_MODULE
#endif
  void
  ZZ_SET_RSTAT(long);
  long
  Back_Scale_0(void); /*M2CONV.C*/
