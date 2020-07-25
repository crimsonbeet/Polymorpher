#ifdef __ZTC__
#include <new.hpp>
#else
#include <new.h>
#endif
#include <stdio.h>
#include <string.h>
#include "tom_aray.hpp"
#ifndef LONG_MAX
#define LONG_MAX ((1l<<30)+((1l<<30)-1))
#endif
void zabort(void);
void Out_Of_Storage(void)
{
	fprintf(stderr, "\n!!! STORAGE RESOURCE EXHAUSTED !!!");
	zabort();
}
_sCash::_sCash(unsigned short R_Size)
{
	set_new_handler(Out_Of_Storage);
	rSize = R_Size;
	iSize = R_Size;
	bRoot = new char*[rSize];
	memset(bRoot, 0, rSize*sizeof(char*));
	hi =-1;
	lo = 1;
}
void _sCash::_Release(void)
{
	unsigned short i;
	for(i = 0; i<rSize; ++i)
	{
		if(bRoot[i])delete bRoot[i];
		bRoot[i] = 0;
	}
	hi =-1;
	lo = 1;
	if(iSize!=rSize&&rSize)
	{
		rSize = 0;
		delete bRoot;
		bRoot = 0;
	}
}
_sCash::~_sCash(void)
{
	iSize=0;
	_Release();
}
void _sCash::tmemcpy(_sCash*Dst, long Cnt/*in bytes*/)
{
	long i;
	long k;
	char*aHot_Old=Dst->aHot;
	long hi_Old=Dst->hi;
	long lo_Old=Dst->lo;
	if(Cnt<=0)zabort();
	for(i=k=0; Cnt>=0; ++i, k+=4096, Cnt-=4096)
	{
		long z = Cnt<4096? Cnt: 4096;
		if((rSize<=i)||(!bRoot[i]))
		{
			continue;
		}
		Dst->_sGet(k, 12);
		memcpy(Dst->bRoot[i], bRoot[i], z);
	}
	Dst->hi = hi_Old;
	Dst->lo = lo_Old;
	Dst->aHot = aHot_Old;
}
void _sCash::tmemcpy(char*Dst, long Cnt/*in bytes*/)
{
	long i;
	if(Cnt<=0)zabort();

	for(i=0; Cnt>=0; ++i, Cnt-=4096, Dst+=4096)
	{
		long z = Cnt<4096? Cnt: 4096;
		if((rSize<=i)||(!bRoot[i]))
		memset(Dst, 0, z);
		else
		memcpy(Dst, bRoot[i], z);
	}
}
void _sCash::fmemcpy(char*Src, long Cnt/*in bytes*/)
{
	long i=Cnt>>12;
	if(Cnt<=0)zabort();

	if(rSize<=i)
	{
		_reAllocBroot(i);
	}
	for(i=0; Cnt>=0; ++i, Cnt-=4096, Src+=4096)
	{
		if(!bRoot[i])bRoot[i] = new char[4096];
		long z = Cnt<4096? Cnt: 4096;
		memcpy(bRoot[i], Src, z);
	}
}
void _sCash::_Set0(long Lim)
{
	unsigned short i;
	unsigned short z = 12;
	unsigned short t = in+1;
	while(t>1)
	{
		--z;
		t>>=1;
	}
	long l = Lim << ++z;
	for(i = 0; i<rSize; ++i)
	{
		if(bRoot[i])memset(bRoot[i], 0, 4096);
		l -= 4096;
		if(l<=0)break;
	}
}
void _rCash::_Set0(long Lim)
{
	unsigned short i;
	for(i = 0; i<rSize; ++i)
	{
		if(bRoot[i])memset(bRoot[i], 0, 4096);
		Lim -= in+1;
		if(Lim<=0)break;
	}
}
void _sCash::_Set0(void)
{
	unsigned short i;
	for(i = 0; i<rSize; ++i)
	{
		if(bRoot[i])memset(bRoot[i], 0, 4096);
	}
}
void _sCash::_reAllocBroot(unsigned short a)
{
	char**temp_1;
	long temp_2;
	temp_2 = rSize;
	if(!rSize)rSize = iSize? iSize: 16;
	while(rSize<=a)rSize<<=1;
	temp_1 = new char*[rSize];
	memset(temp_1, 0, rSize*sizeof(char*));
	if(bRoot)
	{
		memcpy(temp_1, bRoot, temp_2*sizeof(char*));
		delete bRoot;
	}
	bRoot = temp_1;
}
void _sCash::_sGet(long i, unsigned short z)
{
	unsigned short a = i>>z;
	if(a>=rSize)
	{
		_reAllocBroot(a);
	}
	aHot = bRoot[a];
	if(!aHot)
	{
		aHot = new char[4096];
		bRoot[a] = aHot;
	}
	hi = (lo = ((long)a)<<z)+in;
}
void _rCash::_sGet(long i, unsigned short z)
{
	unsigned short a = i/z;
	if(a>=rSize)
	{
		_reAllocBroot(a);
	}
	aHot = bRoot[a];
	if(!aHot)
	{
		aHot = new char[4096];
		bRoot[a] = aHot;
	}
	hi = (lo = (long)a*(long)z)+in;
}
void _sAlong::incr(long f, long l, long v)
{
	long i;
	for(i=f; i<=l; ++i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 12-2);
		}
		*((long*)aHot+(i-lo)) += v;
	}
}
void _sAlong::incr(long f, long l, double v)
{
	long V = v>0? v+0.5: v-0.5;
	incr(f, l, V);
}
long _sAlong::fetch_max_min(long G_lim, long&XM, long&xm, char return_min)
{
	long i;
	long j;
	long k;
	XM =-LONG_MAX;
	xm = LONG_MAX;
	for(k=j=i=0; i<G_lim; ++i)
	{
		long x;
		if(i<lo||hi<i)
		{
			_sGet(i, 12-2);
		}
		x=*((long*)aHot+(i-lo));
		if(x>XM)XM=x, j=i;
		if(x<xm)xm=x, k=i;
	}
	return return_min? k:j;
}
