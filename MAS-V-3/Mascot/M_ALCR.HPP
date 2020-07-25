void
__free__(void * Ptr);
void *
__malloc__(long Size);
void *
__calloc__(long Cnt, long Size);
long
__coreleft__(void);
FILE *
__open__(char * F_N, char * M_D);
void
__error__(FILE * F_I, char * F_N);
void
__close__(FILE * F_I, char * F_N);
void
__write__(void*b, long s, long l, FILE* f);
long
__read__(void*b, long s, long l, FILE* f);
#ifndef DOS386
void *
__normalizefar__(void *fp, long st);
#define incr_far(z,s) (__normalizefar__((z), ((long)(s))*sizeof(*(z))))
#else
/*
#define incr_far(z,s) ((void*)(&(((char*)(z))[(s)*sizeof(*(z))])))
*/
#define incr_far(z,s) (&(z)[(s)])
#endif
void
__rename__(char * old_n, char * new_n);
void
__unlink__(char * n);
