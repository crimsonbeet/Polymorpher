/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#include "stdafx.h" 

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
#include<STRING.H>
/**/
#ifdef __BORLANDC__
#include<VALUES.H>
#endif
/**/
#include<LIMITS.H>
#include<PROCESS.H>
/**/
#include "fqeval.hpp"
#include "fxeval.hpp"
/*-----------------------------------------*/
#define zSnum 7
/*-----------------------------------------*/
/*
	You need to define.
char (*__mGt__)(__MGObj__*a, __MGObj__*b)=__mGt__1;
char (*__mGe__)(__MGObj__*a, __MGObj__*b)=__mGe__1;
char (*__mgt__)(long a, long b, char f) = __mgt__1;
void (*zsmove)(__mgseq__*d, __mgseq__*s) = zsmove_1;
void (*pospoly)(__mgseq__*s) = pospoly_1;
void (*READRUN)(void)=READRUN_1;
long (*readrunitem)(long)=readrunitem_1;
void (*ZSMOVE)(__mgseq__*s, long i) = ZSMOVE_1;
*/
long __coreleft__(void)
{
	return 1024 << 9;
}
template<typename T> 
void __ferror__(T * F_I, char * F_N)
{
	if(!F_I||ferror(F_I))MASLog("\nF_I/O_ERR(%s)", F_N), zabort();
}
/*
appending of new structure do not affect on performance of other modules.
*/
union __MGObj__
{
	long Gbl;
	Obj_Line Obj;
	Flashp Flash;
}	;
inline long sizeof__MGObj__(void)
{
	return sizeof(__MGObj__);
}
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
__MGSEQ__*SQ;
__mgseq__*fq;
/*--------------------------------------*/
long*chnl;
long cmax;
long BMAX;
char*mgsort="mgsort";
void __mgAdel__(void)
{
	long i;
	if(SQ->fil)
	{
		fclose(SQ->fil);
		//unlink(SQ->nam);
		MaSCOT_IO.funlink(SQ->nam);
	}
	for(i=0; i<=zSnum-1; ++i)
	{
		if(fq[i].fil)fclose(fq[i].fil);
		//unlink(fq[i].nam);
		MaSCOT_IO.funlink(fq[i].nam);
	}
}
void
z_close(__mgseq__*s)
{
	s->eof = 1;
	if(s->fil)
	{
		fclose(s->fil);
	}
}
void
put_next(__mgseq__*s, char*b, long l)
{
	if(s->pos+l>=s->bfs)
	{
		fwrite(s->buf, 1, s->pos, s->fil);
		__ferror__(s->fil, s->nam);
		s->pos = 0;
	}
	char*s_b = (char*)incr_far(s->buf, s->pos);
	memcpy(s_b, b, l);
	s->pos += l;
}
void
Set_Next(__mgseq__*s)
{
	if(++s->pos>=s->bfs)
	{
		s->buf[0] = 0;
		s->bfs = (long)fread(s->buf, 1, s->bfs, s->fil);
		s->eof = s->bfs<=0;
		s->pos = 0;
		__ferror__(s->fil, s->nam);
	}
}
void
set_next(__mgseq__*s, char*b, long l)
{
	if(s->pos+l>=s->bfs)
	{
		register long i;
		for(i = 0; i<l; ++i)
		{
			char*s_b = (char*)incr_far(s->buf, s->pos);
			*b++ = *s_b;
			Set_Next(s);
		}
	}
	else
	if(l)
	{
		memcpy(b, incr_far(s->buf, s->pos), l);
		s->pos += l;
	}
}
void
pospoly_1(__mgseq__*s)
{
	if(s->eof)
	{
		return;
	}
	char*s_b = (char*)incr_far(s->buf, s->pos);
	while(*s_b==0)
	{
		long l;
		Set_Next(s);
		s_b = (char*)incr_far(s->buf, s->pos);
		l = *s_b;
		Set_Next(s);
		if(s->eof)
		{
			break;
		}
		s->gbl = 0;
		set_next(s, (char*)&s->gbl, l);
		s->Obj->Gbl = s->gbl;
		s_b = (char*)incr_far(s->buf, s->pos);
	}
	s->eof = 0;
}
void (*pospoly)(__mgseq__*s) = pospoly_1;
void
unload_buf(__mgseq__*s)
{
	if(s->pos>0)
	{
		fwrite(s->buf, 1, s->pos, s->fil);
		__ferror__(s->fil, s->nam);
		s->pos = 0;
	}
}
void
startread(__mgseq__*s)
{
	z_close(s);
	//s->fil = fopen(s->nam, "rb");
	s->fil = MaSCOT_IO.fopen(s->nam);
	fread(&MaSCOT_IO._fileIdentity, sizeof(MaSCOT_IO._fileIdentity), 1, s->fil);
	__ferror__(s->fil, s->nam);
	s->ele = 0;
	s->gbl = 0;
	s->Obj->Gbl = 0;
	s->pos = s->bfs;
	Set_Next(s);
	pospoly(s);
}
void
startwrite(__mgseq__*s)
{
	MaSCOT_IO._fileIdentity.Delta_X = 0;
	MaSCOT_IO._fileIdentity.Delta_Y = 0;
	if(s->fil)
	{
		fclose(s->fil);
	}
	//s->fil = fopen(s->nam, "wb");
	s->fil = MaSCOT_IO.fopen(s->nam);
	fwrite(&MaSCOT_IO._fileIdentity, sizeof(MaSCOT_IO._fileIdentity), 1, s->fil);
	s->eof = 1;
	s->pos = 0;
	s->run = 0;
	s->ele = 0;
	s->gbl =-1;
	s->Obj->Gbl = s->gbl;
	s->bfs = BMAX;
}
void
zsmove_1(__mgseq__*d, __mgseq__*s)
{
	long D;
	long Z;
	if(d->gbl!=s->gbl)
	{
		D = 0;
		put_next(d, (char*)&D, 1);
		Z = d->gbl = s->gbl;
		if(d->gbl<(0XFFL+1))	  D = 1;
		else if(d->gbl<(0XFFFFL+1))   D = 2;
		else if(d->gbl<(0XFFFFFFL+1)) D = 3;
		else			  D = 4;
		put_next(d, (char*)&D, 1);
		put_next(d, (char*)&Z, D);
		d->Obj->Gbl = d->gbl;

	}
	char*s_b = (char*)incr_far(s->buf, s->pos);
	D = *s_b;
	Set_Next(s);
	if(s->eof) return;
	while(D!=0)
	{
		put_next(d, (char*)&D, 1);
		D&=7;
		set_next(s, (char*)&Z, D);
		put_next(d, (char*)&Z, D);
		s_b = (char*)incr_far(s->buf, s->pos);
		D = *s_b;
		Set_Next(s);
	}
	put_next(d, (char*)&D, 1);
	d->ele+= 1;
	pospoly(s);
}
void (*zsmove)(__mgseq__*d, __mgseq__*s) = zsmove_1;
void
__mgswap__(long n, long m)
{
	long*c_n = (long*)incr_far(chnl, n);
	long*c_m = (long*)incr_far(chnl, m);
	long t=*c_n;*c_n=*c_m;*c_m=t;
}
char
__gte__4(long b_a, long b_b, char f)
{
	long x_a = b_a/YBCNT;
	long x_b = b_b/YBCNT;
	long y_a = b_a - x_a*YBCNT;
	long y_b = b_b - x_b*YBCNT;
	if(y_a>y_b)return 1;
	if(y_a<y_b)return 0;
	if(f)goto ge;
	if(y_a==((y_a>>1)<<1))return x_a>x_b;
	return x_a<x_b;
	ge:
	if(y_a==((y_a>>1)<<1))return x_a>=x_b;
	return x_a<=x_b;
}
char
__mgt__4(long a, long b, char f)
{
	long*c_a = f==1? &a: (long*)incr_far(chnl, a);
	long*c_b = f==2? &b: (long*)incr_far(chnl, b);
	long*b_a = (long*)incr_far(SQ->buf, (*c_a));
	long*b_b = (long*)incr_far(SQ->buf, (*c_b));
	return __gte__4(*b_a, *b_b, 0);
}
char
__mgt__1(long a, long b, char f)
{
	long*c_a = f==1? &a: (long*)incr_far(chnl, a);
	long*c_b = f==2? &b: (long*)incr_far(chnl, b);
	long*b_a = (long*)incr_far(SQ->buf, (*c_a));
	long*b_b = (long*)incr_far(SQ->buf, (*c_b));
	return *b_a>*b_b;
}
char (*__mgt__)(long a, long b, char f) = __mgt__1;
void
fastquicksort(long sl, long sr)
{
	long k, j;
	register long x;
	if(sr-sl<10)
	while(sl<sr)
	{
		for(j=sr, k=sr; sl<j; j--)
		if(__mgt__(j-1, j, 0))
		{
			long*c_j = (long*)incr_far(chnl, j);
			long*c_j_1;
			x = *c_j;
			do
			{

				c_j_1 = (long*)incr_far(chnl, (--j));
				*c_j = *c_j_1;
			}
			while(sl<j&&__mgt__(j-1, x, 2));
			*c_j_1 = x;
			k = j;
		}
		sl = k+1;
		for(j =sl; j<sr; j++)
		if(__mgt__(j, j+1, 0))
		{
			long*c_j = (long*)incr_far(chnl, j);
			long*c_j_1;
			x = *c_j;
			do
			{
				c_j_1 = (long*)incr_far(chnl, (++j));
				*c_j = *c_j_1;
			}
			while(j<sr&&__mgt__(x, j+1, 1));
			*c_j_1 = x;
			k = j;
		}
		sr = k-1;
	}
	else
	{
		x = (sl + sr)>>1;
		if(__mgt__ (sl, x, 0))__mgswap__(sl, x);
		if(__mgt__ (x, sr, 0))__mgswap__(sr, x);
		if(__mgt__ (sl, x, 0))__mgswap__(sl, x);
		x = *((long*)incr_far(chnl, x));
		for(k =sl+1, j =sr-1; k < j;)
		{
			while(__mgt__(x, k, 1))++k;
			while(__mgt__(j, x, 2))--j;
			if(k<j)
			{
				__mgswap__(k++, j--);
			}
		}
		if(sl<j)fastquicksort(sl, j);
		if(k<sr)fastquicksort(k, sr);
	}
}
void
MG_READPoly(void)
{
	long P;
	long L;
	long Z = 0;
	fread(&Z, 1, 1, SQ->fil);
	if(feof(SQ->fil))
	{
		return;
	}
	while(!Z)
	{
		fread(&Z, 1, 1, SQ->fil);
		SQ->gbl = 0;
		fread(&SQ->gbl, 1, Z, SQ->fil);
		SQ->Obj->Gbl = SQ->gbl;

		Z = 0;
		fread(&Z, 1, 1, SQ->fil);
		if(feof(SQ->fil))
		{
			return;
		}
	}
	*((long*)incr_far(chnl, SQ->cnt++)) = SQ->lpp = SQ->pos;
	*((long*)incr_far(SQ->buf, SQ->pos++)) = SQ->gbl;
	L = ((SQ->bfs-(SQ->pos+1))<<2) - 1;
	char*S_B = (char*)incr_far(SQ->buf, (SQ->pos+1));
	P = 0;
	while(Z)
	{
		*(S_B+P++)= (char)Z;
		Z &= 7;
		if(P+Z>=L)
		{
			SQ->pos = SQ->bfs;
			SQ->lns = P;
			return;
		}
		fread(S_B+P, 1, Z, SQ->fil);
		P+= Z;
		Z = 0;
		fread(&Z, 1, 1, SQ->fil);
	}
	*(S_B+P++)= 0;
	*((long*)incr_far(SQ->buf, SQ->pos++)) = P;
	SQ->pos += P>>2;
	if(P-((P>>2)<<2))SQ->pos++;
	__ferror__(SQ->fil, SQ->nam);
}
void
QSORT_STARTED(char f)
{
	if(SQ->cnt>1)
	{
//if(f)MASLog("!!! QUICKSORT STARTED ON %6ld ELEMENTS\n", SQ->cnt);
		fastquicksort(0, SQ->cnt-1);
	}
}
void
READRUN_1(void)
{
	char end = 0;
	SQ->eof = 1;
	SQ->pos = 0;
	SQ->cnt = 0;
	SQ->lns = 0;
	do
	{
		MG_READPoly();
		end = (SQ->pos+3>=SQ->bfs);
	}
	while(!feof(SQ->fil)&&!end&&SQ->cnt<cmax);
	QSORT_STARTED(1);
	SQ->eof = SQ->pos<1;
	SQ->Gbl = *((long*)incr_far(SQ->buf, chnl[0]));
	SQ->Obj->Gbl = SQ->Gbl;
	if(!SQ->eof)
	{
		SQ->ele += SQ->cnt;
	}
	__ferror__(SQ->fil, SQ->nam);
}
long
readrunitem_1(long cnt)
{
	fread(incr_far(SQ->Buf, SQ->pos), 1, cnt, SQ->fil);
	return cnt;
}
long (*readrunitem)(long)=readrunitem_1;
void
READRUN_X(long cnt)
{
	long end = SQ->BfS-cnt;
	SQ->pos = 0;
	SQ->cnt = 0;
	SQ->lns = 0;
	do
	{
		long cnt_1=readrunitem(cnt);
		if(feof(SQ->fil))
		{
			break;
		}
		*((long*)incr_far(chnl, SQ->cnt++)) = SQ->pos;
		SQ->pos += cnt_1;
		__ferror__(SQ->fil, SQ->nam);
	}
	while(SQ->pos<end&&SQ->cnt<cmax);
	QSORT_STARTED(1);
	SQ->eof = SQ->pos<1;
	if(!SQ->eof)
	{
		SQ->ele += SQ->cnt;
	}
	__ferror__(SQ->fil, SQ->nam);
}
void (*READRUN)(void)=READRUN_1;
void
STARTREAD(void)
{
	SQ->gbl = 0;
	SQ->ele = 0;
	SQ->run = 0;
	SQ->Obj->Gbl = 0;
	BMAX = 0;
	//SQ->fil = fopen(SQ->nam, "rb");
	SQ->fil = MaSCOT_IO.fopen(SQ->nam);
	fread(&MaSCOT_IO._fileIdentity, sizeof(MaSCOT_IO._fileIdentity), 1, SQ->fil);
	__ferror__(SQ->fil, SQ->nam);
	long bmax =(__coreleft__()-0x17ffel)>>2;
	bmax -= (cmax = bmax>>3);
	chnl = (long*)malloc(cmax<<2);
	chnl[0]=0;
	SQ->buf = (long*)malloc(bmax<<2);
	SQ->bfs =--bmax;
	SQ->Buf = (char*)SQ->buf;
	SQ->BfS = SQ->bfs<<2;
	READRUN();
}
void
ZSMOVE_1(__mgseq__*s, long i)
{
	long p;
	long d;
	char z;
	char*c;
	p = *((long*)incr_far(chnl, i));
	long*S_B = (long*)incr_far(SQ->buf, p);
	if(s->gbl!=*S_B)
	{
		d = 0;
		fwrite(&d, 1, 1, s->fil);
		s->gbl = *S_B;
		if(s->gbl<(0XFFL+1))	  d = 1;
		else if(s->gbl<(0XFFFFL+1))   d = 2;
		else if(s->gbl<(0XFFFFFFL+1)) d = 3;
		else			  d = 4;
		fwrite(&d, 1, 1, s->fil);
		fwrite(&s->gbl, 1, d, s->fil);
		s->Obj->Gbl = s->gbl;
	}
	c = (char*)incr_far(SQ->buf, (p+2));
	++s->ele;
	if(p==SQ->lpp&&SQ->lns>0)
	{
		fwrite(c, 1, SQ->lns, s->fil);
		d = 0;
		d = c[SQ->lns-1];
		while(d)
		{
			z = d&7;
			fread(&d, 1, z, SQ->fil);
			fwrite(&d, 1, z, s->fil);
			d = 0;
			fread(&d, 1, 1, SQ->fil);
			fwrite(&d, 1, 1, s->fil);
		}
		return;
	}
	else
	{
		d = *((long*)incr_far(SQ->buf, (p+1)));
	}
	fwrite(c, 1, d, s->fil);
}
void (*ZSMOVE)(__mgseq__*s, long i) = ZSMOVE_1;
void copyrun(__mgseq__*s)
{
	long i;
	for(i=0; i<SQ->cnt; ++i)
	{
		ZSMOVE(s, i);
	}
	READRUN();
}
char __mGt__1(__MGObj__*a, __MGObj__*b)
{
	return a->Gbl>b->Gbl;
}
char __mGe__1(__MGObj__*a, __MGObj__*b)
{
	return a->Gbl>=b->Gbl;
}
char __mGt__4(__MGObj__*a, __MGObj__*b)
{
	return __gte__4(a->Gbl, b->Gbl, 0);
}
char __mGe__4(__MGObj__*a, __MGObj__*b)
{
	return __gte__4(a->Gbl, b->Gbl, 1);
}
char
GT_VECMul(Obj_Line*a,Obj_Line*b)
{
	long Y2 = a->y2-a->y1;
	long Y4 = b->y2-b->y1;
	long X2 = a->x2-a->x1;
	long X4 = b->x2-b->x1;
	char SL;
	char SV;

	if(X2>0) SL = Y2<0 ? 4 : 1;
	else
	if(X2<0) SL = Y2>0 ? 2 : 3;
	else     SL = Y2>0 ? 2 : 4;
	if(X4>0) SV = Y4<0 ? 4 : 1;
	else
	if(X4<0) SV = Y4>0 ? 2 : 3;
	else     SV = Y4>0 ? 2 : 4;

	if(SL>SV)return 0;
	else
	if(SL<SV)return 2;

	double S24;
	S24 = (double)X2*Y4-(double)X4*Y2;

	return S24 <0? 0: S24>0? 2: a->status>b->status? 0: a->status<b->status? 2: 1;
}
char
GTObj_Stat(Obj_Line*b_a,Obj_Line*b_b)
{
	return b_a->y1>b_b->y1? 2: b_a->y1<b_b->y1? 0:
		 b_a->x1>b_b->x1? 2: b_a->x1<b_b->x1? 0: GT_VECMul(b_a,b_b);
}
char
__mgt__2(long a, long b, char f)
{
	long*c_a = f==1? &a: (long*)incr_far(chnl, a);
	long*c_b = f==2? &b: (long*)incr_far(chnl, b);
	Obj_Line*b_a = (Obj_Line*)incr_far(SQ->Buf, (*c_a));
	Obj_Line*b_b = (Obj_Line*)incr_far(SQ->Buf, (*c_b));
	return GTObj_Stat(b_a,b_b)>1;
}
char __mGt__2(__MGObj__*a, __MGObj__*b)
{
	Obj_Line*b_a = &a->Obj;
	Obj_Line*b_b = &b->Obj;
	return GTObj_Stat(b_a,b_b)>1;
}
char __mGe__2(__MGObj__*a, __MGObj__*b)
{
	Obj_Line*b_a = &a->Obj;
	Obj_Line*b_b = &b->Obj;
	return GTObj_Stat(b_a,b_b)>0;
}
char (*__mGt__)(__MGObj__*a, __MGObj__*b)=__mGt__1;
char (*__mGe__)(__MGObj__*a, __MGObj__*b)=__mGe__1;
char
GTFlashp_z(Flashp*b_a,Flashp*b_b)
{
	return  b_a->An>b_b->An? 2: b_a->An<b_b->An? 0:
		b_a->Zn>b_b->Zn? 2: b_a->Zn<b_b->Zn? 0:
		((b_a->Zn>>1)<<1)==b_a->Zn? /*go back*/
		(
		b_a->Cx<b_b->Cx? 2: b_a->Cx>b_b->Cx? 0:
		b_a->Cy>b_b->Cy? 2: b_a->Cy<b_b->Cy? 0: 1
		):(
		b_a->Cx>b_b->Cx? 2: b_a->Cx<b_b->Cx? 0:
		b_a->Cy>b_b->Cy? 2: b_a->Cy<b_b->Cy? 0: 1
		);
}
char __mgt__3(long a, long b, char f)
{
	long*c_a = f==1? &a: (long*)incr_far(chnl, a);
	long*c_b = f==2? &b: (long*)incr_far(chnl, b);
	Flashp*b_a = (Flashp*)incr_far(SQ->Buf, (*c_a));
	Flashp*b_b = (Flashp*)incr_far(SQ->Buf, (*c_b));
	return GTFlashp_z(b_a,b_b)>1;
}
char __mGt__3(__MGObj__*a, __MGObj__*b)
{
	Flashp*b_a = &a->Flash;
	Flashp*b_b = &b->Flash;
	return GTFlashp_z(b_a,b_b)>1;
}
char __mGe__3(__MGObj__*a, __MGObj__*b)
{
	Flashp*b_a = &a->Flash;
	Flashp*b_b = &b->Flash;
	return GTFlashp_z(b_a,b_b)>0;
}
void
pospoly_2(__mgseq__*s)
{
	if(s->bfs<=0||s->eof)
	{
		s->eof = 1;
		Obj_Line z={-LONG_MAX,-LONG_MAX,LONG_MAX,LONG_MAX,0};
		s->Obj->Obj = z;
		return;
	}
	set_next(s, (char*)s->Obj, sizeof(Obj_Line));
	s->eof = 0;
}
void
pospoly_3(__mgseq__*s)
{
	if(s->bfs<=0||s->eof)
	{
		s->eof = 1;
		Flashp z={-LONG_MAX,-LONG_MAX};
		s->Obj->Flash = z;
		return;
	}
	set_next(s, (char*)s->Obj, sizeof(Flashp));
	s->eof = 0;
}
void
zsmove_2(__mgseq__*d, __mgseq__*s)
{
	(*d->Obj) = (*s->Obj);
	put_next(d, (char*)d->Obj, sizeof(Obj_Line));
	d->ele+= 1;
	pospoly_2(s);
}
void
zsmove_3(__mgseq__*d, __mgseq__*s)
{
	(*d->Obj) = (*s->Obj);
	put_next(d, (char*)d->Obj, sizeof(Flashp));
	d->ele+= 1;
	pospoly_3(s);
}
void
READRUN_2(void)
{
	READRUN_X(sizeof(Obj_Line));
	SQ->Obj->Obj = *((Obj_Line*)incr_far(SQ->Buf, chnl[0]));
}
void
READRUN_3(void)
{
	READRUN_X(sizeof(Flashp));
	SQ->Obj->Flash = *((Flashp*)incr_far(SQ->Buf, chnl[0]));
}
void
ZSMOVE_2(__mgseq__*s, long i)
{
	long p = *((long*)incr_far(chnl, i));
	Obj_Line*Obj = (Obj_Line*)incr_far(SQ->Buf, p);
	fwrite(Obj, 1, sizeof(Obj_Line), s->fil);
	if(i==SQ->cnt-1)
	{
		s->Obj->Obj = (*Obj);
	}
	++s->ele;
}
void
ZSMOVE_3(__mgseq__*s, long i)
{
	long p = *((long*)incr_far(chnl, i));
	Flashp*Obj = (Flashp*)incr_far(SQ->Buf, p);
	fwrite(Obj, 1, sizeof(Flashp), s->fil);
	if(i==SQ->cnt-1)
	{
		s->Obj->Flash = (*Obj);
	}
	++s->ele;
}
unsigned
polysort(long N)
{
	long level =1;
	long i, j;
	for(i = 0; i <N; ++i)
	{
		fq[i].zfa = fq[i].zfd = 1;
	}
	fq[N].zfa = fq[N].zfd = 0;
	for(i=0; i<N; ++i)
	{
		startwrite(fq+i);
	}
	STARTREAD();
	for(i = 0; i <N && !SQ->eof; ++i)
	{
		copyrun(fq+i);
		while(!SQ->eof&&__mGe__(SQ->Obj, fq[i].Obj))
		{
			copyrun(fq+i);
		}
		++SQ->run;
		--fq[i].zfd;
		++fq[i].run;
	}
	if(!SQ->eof)
	{
		++level;
		for(i=0; i<N; ++i)
		{
			(fq[i].zfd = 1), fq[i].zfa = 2;
		}
		--fq[N-1].zfd;
		--fq[N-1].zfa;
	}
	j=0;
	while(!SQ->eof)
	{
		if(fq[j].zfd<fq[j+1].zfd)++j;
		else
		{
			if(!fq[j].zfd)
			{
				long z = fq[0].zfa;
				++level;
				for(i=0; i<N; ++i)
				{
					fq[i].zfd = z + fq[i+1].zfa - fq[i].zfa;
					fq[i].zfa = z + fq[i+1].zfa;
				}
			}
			j=0;
		}
		if(__mGe__(SQ->Obj, fq[j].Obj))
		{
			copyrun(fq+j);
			while(!SQ->eof&&__mGe__(SQ->Obj, fq[j].Obj))
			{
				copyrun(fq+j);
			}
		}
		if(!SQ->eof)
		{
			copyrun(fq+j);
			while(!SQ->eof&&__mGe__(SQ->Obj, fq[j].Obj))
			{
				copyrun(fq+j);
			}
			++SQ->run;
			--fq[j].zfd;
			++fq[j].run;
		}
	}
MASLog("!!! RUNS %6ld ELEMENTS %6ld\r\n", SQ->run, SQ->ele);
	fclose(SQ->fil);
	//unlink(SQ->nam);
	MaSCOT_IO.funlink(SQ->nam);
	SQ->fil = 0;
	free(SQ->buf);
	free(chnl);
	for(i=0; i<=N; ++i)
	{
		z_close(fq+i);
	}
/* MERGE RUNS FROM SEQUENCES */
	long t[zSnum+1];
	long a[zSnum+1];
	long bmax =__coreleft__()-0xffffl-((N+1)*1000L);
	bmax = bmax/(N+1);
	for(i=0; i<=N; ++i)
	{
		fq[i].bfs = bmax-1;
		fq[i].buf = (char*)malloc(bmax);
	}
	BMAX = bmax-1;
	for(i=0; i<=N; ++i)
	{
		t[i] = i;
	}
	if(SQ->run>1)
	for(i=0; i<N; ++i)
	{
		startread(fq+i);
	}
	if(SQ->run>1)
	while(level)
	{
		long k = -1;
		long z = fq[t[N-1]].zfa;
		fq[t[N]].zfd = 0;
		startwrite(fq+t[N]);
		while(z)
		{
			for(i=0; i <N; ++i)
			{
				if(fq[t[i]].zfd > 0) --fq[t[i]].zfd;
				else a[++k] = t[i];
			}
			if(k==-1) ++fq[t[N]].zfd;
			else
			{
				long m;
				while(k!=-1)
				{
					for(i=1, m=0; i<=k; ++i)
					if(__mGt__(fq[a[m]].Obj,fq[a[i]].Obj))
					{
						m =i;
					}
					zsmove(fq+t[N], fq+a[m]);
					if(fq[a[m]].eof||__mGt__(fq[t[N]].Obj,fq[a[m]].Obj))
					{
						--fq[a[m]].run;
						a[m] = a[k];
						--k;
					}
				}
				++fq[t[N]].run;
			}
			--z;
		}
		unload_buf(fq+t[N]);
		if(level!=1)
		{
			startread(fq+t[N]);
		}
		{
			long tn = t[N], dn = fq[t[N]].zfd;
			for(i=N, z=fq[t[N-1]].zfa; i>0; --i)
			{
				t[i] = t[i-1];
				fq[t[i]].zfa -= z;
			}
			t[0] = tn;
			fq[tn].zfd = dn;
			fq[tn].zfa = z;
		}
MASLog("!!! LEVEL %ld COMPLETED\n", level);
		--level;
	}
MASLog("!!! TOTAL %ld ELEMENTS SORTED !!!\n", fq[t[0]].ele);
	unload_buf(fq+t[0]);
	for(i=0; i<=N; ++i)
	{
		z_close(fq+i);
	}
	for(i=0; i<=N; ++i)
	{
		fq[i].bfs = 0;
		free(fq[i].buf);
	}
	j = SQ->run>1? N: N-1;
	for(i=0; i<=j; ++i)
	{
		//if(i!=t[0])unlink(fq[i].nam), fq[i].fil = 0;
		if(i!=t[0])MaSCOT_IO.funlink(fq[i].nam), fq[i].fil = 0;
	}
	return(t[0]);
}
char*
mg_sort(char*fnam)
{
	__MGSEQ__ S;
	__mgseq__ F[zSnum];
	__MGObj__ Obj[zSnum+1];
	SQ =&S;
	fq = F;

	SQ->fil = 0;
	SQ->buf = chnl =0;
	SQ->Obj = &Obj[0];
	strncpy(SQ->nam, fnam, 15);
	unsigned i;
	unsigned k;
	for(i=0; i<zSnum; ++i)
	{
		sprintf(fq[i].nam, "mg$sort%d", (long)i); 
		//tmpnam(fq[i].nam);fq[i].nam[0]='s';

		fq[i].fil = NULL;
		fq[i].Obj = &Obj[i+1];
	}
	zAbort_proc = __mgAdel__;
	k = polysort(zSnum-1);
MASLog("\n!!! SORT FINISHED NORMALLY !!!\n");
	zAbort_proc = 0;
	return(fq[k].nam);
}
