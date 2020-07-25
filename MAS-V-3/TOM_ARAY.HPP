#ifndef TOM_ARAY
#define TOM_ARAY 1
class _sCash
{
public:
	long lo;
	long hi;
	long in;
	char*aHot;
	char**bRoot; /*Addresses of segments.*/
	unsigned short iSize; /*initial Root size.*/
	unsigned short rSize; /*Root size.*/
	_sCash(unsigned short R_Size = 16);
	~_sCash();
	void _Set0(void);
	virtual void _Set0(long);
	virtual void _sGet(long, unsigned short);
	virtual void fmemcpy(char*Src, long Cnt/*in bytes*/);
	virtual void tmemcpy(char*Dst, long Cnt/*in bytes*/);
	virtual void tmemcpy(_sCash*Dst, long Cnt/*in bytes*/);
	void _reAllocBroot(unsigned short);
	void _Release(void);
}	;
class _rCash: public _sCash
{
public:
	_rCash(long In, unsigned short Rs):_sCash(Rs){in=In-1;};
	~_rCash(){};
	void _Set0(long);
	void _sGet(long, unsigned short);
}	;
class _uAshort: public _sCash
{
public:
	_uAshort():_sCash(32){in = 2047;};
	~_uAshort(){};
	unsigned short& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 12-1);
		}
		return *((unsigned short*)aHot+(i-lo));
	}
}	;
class _uAchar: public _sCash
{
public:
	_uAchar():_sCash(16){in = 4095;};
	~_uAchar(){};
	unsigned char& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 12-0);
		}
		return *((unsigned char*)aHot+(i-lo));
	}
}	;
class _sAchar: public _sCash
{
public:
	_sAchar():_sCash(16){in = 4095;};
	~_sAchar(){};
	char& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 12-0);
		}
		return *((char*)aHot+(i-lo));
	}
}	;
class _zAvoid: public _sCash
{
public:
	_zAvoid():_sCash(64){in = 1023;};
	~_zAvoid(){};
	void*& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 12-2);
		}
		return *((void**)aHot+(i-lo));
	}
}	;
class _sAlong: public _sCash
{
public:
	_sAlong():_sCash(64){in = 1023;};
	~_sAlong(){};
	long& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 12-2);
		}
		return *((long*)aHot+(i-lo));
	}
	void incr(long first, long last, double val);
	void incr(long first, long last, long val);
	long fetch_max_min(long G_lim, long&XM, long&xm, char return_min);
}	;
#endif
