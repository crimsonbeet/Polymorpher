#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef __ZTC__
#include <alloc.h>
#endif
/*
REDEFINITION OF MALLOC, CORELEFT.
*/
#ifndef DOS386
	#define malloc farmalloc
	#define calloc farcalloc
	#define free farfree
	#ifdef __ZTC__
		#define coreleft farcoreleft
	#endif
#else
	#define coreleft _x386_coreleft
#endif
void zabort(void);
void
__FINISHEDNORMALLY__(void)
{
	fprintf(stderr, "\n!!! FINISHED NORMALLY. !!!\n");
}
void
FinishedNormally(void){__FINISHEDNORMALLY__();}
void
__free__(void * p)
{
	if(!p)zabort();
	free(p);
}
void *
__malloc__(long Size)
{
	void * Ptr;
	Ptr = malloc(Size);
	if(!Ptr)
		fprintf(stderr, "\nM_ALLOC_ERR(%ld)", Size), zabort();
	return Ptr;
}
void *
__calloc__(long Cnt, long Size)
{
	void * Ptr;
	Ptr = calloc(Cnt, Size);
	if(!Ptr)
		fprintf(stderr, "\nM_ALLOC_ERR(%ld)", Size), zabort();
	return Ptr;
}
long
__coreleft__(void)
{
	return coreleft();
}
FILE *
__open__(char * F_N, char * M_D)
{
	FILE * F_I;
	F_I = fopen(F_N, M_D);
	if(F_I==NULL)
		fprintf(stderr, "\nF_OPEN_ERR(%s)", F_N), zabort();
	return F_I;
}
void
__error__(FILE * F_I, char * F_N)
{
	if(!F_I||ferror(F_I))
	fprintf(stderr, "\nF_I/O_ERR(%s)", F_N), zabort();
}
void
__close__(FILE * F_I, char * F_N)
{
	fclose(F_I);
	__error__(F_I, F_N);
}
#ifndef DOS386
#ifndef __ZTC__
#undef MK_FP
#undef FP_SEG
#define MK_FP(seg,offset) ((void far*)(((unsigned long)(seg)<<16)|(unsigned)(offset)))
#define FP_SEG(fp) ((unsigned)((unsigned long)(fp)>>16))
#endif
void *
__normalizefar__(void *fp, long st)
{
	long off;
	long seg;
	off = (long)(FP_OFF(fp))+st;
	seg = (long)(FP_SEG(fp))+(off>>4);
	return MK_FP(seg,(off&0xf));
}
#define incr_far(z,s) (__normalizefar__((z), ((long)(s))*sizeof(*(z))))
#else
/*
#define incr_far(z,s) ((void*)(&(((char*)(z))[(s)*sizeof(*(z))])))
*/
#define incr_far(z,s) ((z)+(s))
#endif

void
__write__(void*b, long s, long l, FILE* f)
{
	if(l*s>0xfff0l)
	{
		char*z = (char*)b;
		l *= s;
		do
		{
			if(l>0xfff0l)s=0xfff0l;
			else s = l;
			fwrite(z, 1, s, f);
			l -= s;
			z = (char*)incr_far(z,s);
		}
		while(l);
		return;
	}
	fwrite(b, s, l, f);
}
long
__read__(void*b, long t, long l, FILE* f)
{
	long c;
	long s;
	if(l*t>0xfff0l)
	{
		char*z = (char*)b;
		l *= t;
		c=0;
		do
		{
			if(l>0xfff0l)s=0xfff0l;
			else s = l;
			if(feof(f))return(c/t);
			else
			c += fread(z, 1, s, f);
			l -= s;
			z = (char*)incr_far(z,s);
		}
		while(l);
		return(c/t);
	}
	c = fread(b, t, l, f);
	return c;
}
void
__rename__(char * old_n, char * new_n)
{
	if(rename(old_n, new_n)!=0)
	fprintf(stderr, "REN_ERR(%s to %s)\n", old_n, new_n), zabort();
}
void
__unlink__(char * n)
{
	if(unlink(n)!=0)
	fprintf(stderr, "UNLINK_ERR(%s)\n", n), zabort();
}
