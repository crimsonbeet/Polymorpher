/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#ifndef _QUICKSORT_H_
#define _QUICKSORT_H_

#include "FQEVAL.hpp"

union AUXObj
{
	long L;
	void*P;
};

typedef AUXObj GT_GET(long);
typedef char GT(AUXObj, AUXObj, char);
typedef void GT_SWAP(long, long);
extern GT * Gt;
extern GT_SWAP * GtSwap;
extern GT_GET * GtGetVal;

long GetMinXLv(LvBead*Lv); 
AUXObj GtGetXLv(long k);
void GtSwapLv(long k, long m); 
char GtXLv(AUXObj k, AUXObj m, char f);
AUXObj GtGetVt(long k);
void GtSwapVt(long k, long m);
char GtYVt(AUXObj k, AUXObj m, char f);
char GtyVt(AUXObj k, AUXObj m, char f);
void GtSwapLb(long k, long m);
char GtYLb(AUXObj k, AUXObj m, char f);
char GtyLb(AUXObj k, AUXObj m, char f);
char GtxLb(AUXObj k, AUXObj m, char f);
AUXObj GtGetLb(long k);
void Quick_Sort(long l, long r);

void Lv_Sort(int firts_lb = 1); // returns sorted LvBeads in PTRAux_1


#endif