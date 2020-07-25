/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

union __MGObj__
{
	long Gbl;
	Obj_Line Obj;
	Flashp Flash;
}	;
struct __mgseq__
{
	char nam[15];
	long eof;
	long zfa;
	long zfd;
	long bfs;
	long pos;
	long run;
	long ele;
	long gbl;
	char*buf;
	//FILE*fil;
	XMemoryFile*fil;
	__MGObj__*Obj;
}	;
struct __MGSEQ__
{
	char nam[15];
	long eof;
	long bfs;
	long BfS;
	long run;
	long pos;
	long cnt;
	long ele;
	long gbl;
	long lpp;
	long lns;
	long Gbl;
	//FILE*fil;
	XMemoryFile*fil;
	long*buf;
	char*Buf;
	__MGObj__*Obj;
}	;
extern __MGSEQ__*SQ;
extern __mgseq__*fq;
extern char (*__mGt__)(__MGObj__*a, __MGObj__*b);
extern char (*__mGe__)(__MGObj__*a, __MGObj__*b);
extern char (*__mgt__)(long a, long b, char f);
extern void (*zsmove)(__mgseq__*d, __mgseq__*s);
extern void (*pospoly)(__mgseq__*s);
extern void (*READRUN)(void);
extern void (*ZSMOVE)(__mgseq__*s, long i);
extern long*chnl;
extern long cmax;
extern long BMAX;
char*mg_sort(char*fnam);
char GTObj_Stat(Obj_Line*b_a,Obj_Line*b_b);
void fastquicksort(long sl, long sr);
char __mGt__1(__MGObj__*a, __MGObj__*b);
char __mGe__1(__MGObj__*a, __MGObj__*b);
char __mGt__2(__MGObj__*a, __MGObj__*b);
char __mGe__2(__MGObj__*a, __MGObj__*b);
char __mGt__3(__MGObj__*a, __MGObj__*b);
char __mGe__3(__MGObj__*a, __MGObj__*b);
char __mGt__4(__MGObj__*a, __MGObj__*b);
char __mGe__4(__MGObj__*a, __MGObj__*b);
void ZSMOVE_3(__mgseq__*s, long i);
void ZSMOVE_2(__mgseq__*s, long i);
void ZSMOVE_1(__mgseq__*s, long i);
void READRUN_1(void);
void READRUN_2(void);
void READRUN_3(void);
void READRUN_X(long);
extern long (*readrunitem)(long);
long readrunitem_1(long);
void zsmove_1(__mgseq__*d, __mgseq__*s);
void zsmove_2(__mgseq__*d, __mgseq__*s);
void zsmove_3(__mgseq__*d, __mgseq__*s);
char __mgt__1(long a, long b, char f);
char __mgt__2(long a, long b, char f);
char __mgt__3(long a, long b, char f);
char __mgt__4(long a, long b, char f);
void pospoly_1(__mgseq__*s);
void pospoly_2(__mgseq__*s);
void pospoly_3(__mgseq__*s);
void set_next(__mgseq__*s, char*b, long l);
void put_next(__mgseq__*s, char*b, long l);
