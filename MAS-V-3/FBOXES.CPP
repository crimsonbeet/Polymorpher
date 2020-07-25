char*fboxes="fboxes";
#define bcxm CSrc.x2
#define bcxl CSrc.x1
#define bcyl CSrc.y1
#define bcym CSrc.y2
#define bckd (((double)(bcxl-bcxm))/((double)(bcyl-bcym)))

#define Objl_Rstatus 1 /*IT'S LEFT EDGE AND RSTAT = 1*/
#define Objl_Lstatus 0 /*IT'S RIGHT EDGE AND RSTAT = 0*/

#define NDir (CDir->Nxt)

#define nlyl (NDir->LY1)
#define nlym (NDir->LY2)
#define nlxl (NDir->LX1)
#define nlxm (NDir->LX2)
#define nryl (NDir->RY1)
#define nrym (NDir->RY2)
#define nrxl (NDir->RX1)
#define nrxm (NDir->RX2)

#define olyl (ODir->LY1)
#define olym (ODir->LY2)
#define olxl (ODir->LX1)
#define olxm (ODir->LX2)
#define oryl (ODir->RY1)
#define orym (ODir->RY2)
#define orxl (ODir->RX1)
#define orxm (ODir->RX2)

#define flyl (CDir->LY1)
#define flym (CDir->LY2)
#define flxl (CDir->LX1)
#define flxm (CDir->LX2)
#define fryl (CDir->RY1)
#define frym (CDir->RY2)
#define frxl (CDir->RX1)
#define frxm (CDir->RX2)

#define flkd (((double)(flxl-flxm))/((double)(flyl-flym)))
#define frkd (((double)(frxl-frxm))/((double)(fryl-frym)))

struct Dir_Line
{
	double LX1; /*LOWER X*/
	double RX1; /*LOWER X*/
	long LY1; /*LOWER Y*/
	long LX2; /*UPPER X*/
	long LY2; /*UPPER Y*/
	long RY1; /*LOWER Y*/
	long RX2; /*UPPER X*/
	long RY2; /*UPPER Y*/
	Dir_Line*Nxt;
}	;
class _zbDirl: public _rCash
{
public:
	_zbDirl():_rCash(4096/sizeof(Dir_Line), 128){};
	~_zbDirl(){};
	Dir_Line& operator[](long int i)
	{
		if(i<lo||hi<i)
		{
			_sGet(i, 4096/sizeof(Dir_Line));
		}
		return *((Dir_Line*)aHot+(i-lo));
	}
}	;
class Dir_Buffer
{
	long BLim;
	char BEof;
	long fcyl;

	Obj_Line CSrc;

	Dir_Line*CDir;
	Dir_Line*ODir;

	Dir_Line*FDir;
	Dir_Line*STak;

	_zbDirl __B__;

	Dir_Line*pop(void);
	void push(Dir_Line*Dir);
	void awaycrn(void);
	void getsrc(void);
	Dir_Line*put(void);
	Dir_Line*pop(void);
	void push(Dir_Line*Dir);
	void terminate(long Y);
	unsigned analize(void);
	unsigned merge(void);
public:
	void makeboxes(void);
	~Dir_Buffer();
	Dir_Buffer(M_5009_JOB JOB);
}	;
void Dir_Buffer::getsrc(void)
{
	do
	{
		if(!MaSCOT_IO.ZDREAD_L(CSrc))
		{
			BEof = 1;
			return;
		}
		if(bcxm==bcxl)
		{
			return;
		}
	}
	while(bcym==bcyl);
}
void Dir_Buffer::awaycrn(void)
{
	Dir_Line*Nxt;
	if(ODir==0)
	Nxt = (FDir = CDir->Nxt);
	else
	Nxt = (ODir->Nxt = CDir->Nxt);
	push(CDir);
	CDir = Nxt;
}
Dir_Line*Dir_Buffer::put(void)
{
	Dir_Line*z=pop();
	if(!z)
	{
		z=&B->__B__[B->BLim++];
	}
	if(ODir!=0)
	{
		z->Nxt = ODir->Nxt;
		ODir->Nxt = z;
	}
	else
	{
		z->Nxt = FDir;
		Fdir = z;
	}
	return z;
}
Dir_Line*Dir_Buffer::pop(void)
{
	Dir_Line*z=STak;
	if(z)
	{
		STak = STak->Nxt;
	}
	return z;
}
void Dir_Buffer::push(Dir_Line*Dir)
{
	Dir->Nxt = STak;
	STak = Dir;
}
Dir_Buffer::Dir_Buffer(M_5009_JOB JOB)
{
	memcpy(I_Layer[0].Name, JOB.Name, 20);
	ILayer_Cnt= 1;
	MaSCOT_IO.Find_ILayer();
	OutDeltaX = 0;
	OutDeltaY = 0;
	MaSCOT_IO.Make_PLT_File_Name(I_Layer[0].PdbN, I_Layer[0].LX_S.PDBI);
	if(!FindxFile(I_Layer[0].PdbN))
	{
		zAbort(fractf,__LINE__);
	}
	MaSCOT_IO.Init_ILayer_File(I_Layer);
	CDir=0;
	ODir=0;
	FDir=0;
	STak=0;
	BLim=0;
	BEof=0;
	CFragment = 0;
	XFragment =-1;
	MaSCOT_IO.D_Link(0);
	getsrc();
}
Dir_Buffer::~Dir_Buffer()
{
	MaSCOT_IO.Close_ILayer(0);
	__unlink__(I_Layer[0].PdbN);
ZLDLIM=0;
__DXBLD__(0,0);
}
double
compute_cut_x(double X1, long Y1, long X2, long Y2, long Y)
{
	double X;
	double a;
	double b;
	double c;
	double d;
	a = Y1-Y2;
	b = X2-X1;
	if(b==0)
	{
		return X1;
	}
	c = (double)(X1)*Y2 - (double)(X2)*Y1;
	d = -(Y*b + c);
	X = a!=0? d/a: X1;
	return X;
}
void
Dir_Buffer::terminate(long ym)
{
	char r;
	char l;
	l = flyl!=flym&&flxl!=flxm;
	r = fryl!=frym&&frxl!=frxm;
	if(l&&r)
	{
		long xl=ceil(max(flxl, flxm));
		long xm=floor(min(frxl, frxm));
		if(xl<xm)
		{
			long yl=max(flyl, fryl);
			if(ym>yl)
			{
				unsigned i;
				ZLDLIM = 4;
				for(i = 0; i<4; ++i)
				{
					ZLDCO0[i] = i<2? xl: xm;
					ZLDCO1[i] = i%3? ym: yl;
				}
				__DXBLD__(1, "B");
			}
		}
	}
	if(r)
	{
		if(flym!=ym)
		flxl = compute_cut_x(flxl, flyl, flxm, flym, ym);
		else
		flxl = flxm;
	}
	if(r)
	{
		if(frym!=ym)
		frxl = compute_cut_x(frxl, fryl, frxm, frym, ym);
		else
		frxl = frxm;
	}
	flyl = Y;
	fryl = Y;
}
unsigned
Dir_Buffer::analize(void)
/*NON HORIZONTAL OBJ-LINE MUST BE.*/
/*NO ANY JUNCTION LINES MUST BE.*/
{
	register char R_F;

	R_F = bcyl!=fcyl;
	if(R_F)
	{
		return 0; /*REWIND FRONTS*/
	}
	R_F = CDir==0;
	if(R_F)
	{
		return 1; /*IT IS END OF FRONTS*/
	}
	R_F = flym==flyl&&frym==fryl;
	if(R_F)
	{
		return 2; /*THIS FRONT WAS MERELY TERMINATED*/
	}
	R_F = fcyl!=(flyl==flym? fryl: flyl);
	if(R_F)
	{
		if(flym==flyl)return 3;
		if(frym==fryl)return 7;

		if(bcyl<flym&&bcyl<frym)
		{
			double ffxl=min(flxl, flxm);
			double ffxm=max(frxl, frxm);

			if(bcxl<=ffxl)return 4;
			if(bcxl>=ffxm)return 5;

			ffxl = compute_cut_x(flxl, flyl, flxm, flym, fcyl);
			ffxm = compute_cut_x(frxl, fryl, frxm, frym, fcyl);

			if(bcxl<=ffxl)return 4;
			if(bcxl>=ffxm)return 5;
		}
		terminate(min(fcyl, min(flym, frym)));
		return analize();
	}
	R_F = flym==flyl;
	if(R_F)
	{
		return 3;
	}
	/*THIS IS SPECIAL CASE FOR MERGENING*/
	R_F = frym==fryl;
	if(R_F)
	{
		R_F = bcxl==frxm&&bcym==frym;
		if(R_F)
		{
			return 8;
		}
		R_F = bcxl>=flxl;
		if(R_F)
		{
			return 7;
		}
	}
	/*ELSE IT IS A "LEFT" CASE AND SUBMITTED TO SYMPLE PASS*/
	R_F = bcxl<flxl;
	if(R_F)
	{
		return 4;
	}
	R_F = bcxl==flxl;
	if(R_F)
	return bckd<flkd? 4: (bcxl==frxl? (bckd>frkd? 5: 6): 6);

	R_F = bcxl<frxl;
	if(R_F)
	{
		return 6;
	}
	return 5;
}
unsigned
Dir_Buffer::merge(void)
{
	unsigned error=0;
	Dir_Line*o_l_d = ODir;
	Dir_Line*c_r_n = CDir;
	unsigned endloop=0;
	ODir = CDir;
	CDir = NDir;
	while(!endloop)
	{
		if(error)
		{
			break;
		}
		switch(analize())
		{
			case 6: case 0: error = 1;
			case 2:	awaycrn();
			break;
			case 7:
			error = merge();
			endloop = 1;
			break;
			case 3:
			oryl = fryl;
			orym = frym;
			orxl = frxl;
			orxm = frxm;
			awaycrn();
			endloop = 1;
			break;
			default:
			endloop = 1;
			break;
		}
	}
	ODir = o_l_d;
	CDir = c_r_n;
	return error;
}
void
Dir_Buffer::makeboxes(void)
{
	unsigned nlop = 0;
	unsigned error = 0;

	fcyl=-LONG_MAX;
	while(!BEof)
	{
		if(nlop>512)
		{
			error = 1;
			break;
		}
		switch(analize())
		{
			case 0:
			fcyl = bcyl;
			CDir = FDir;
			ODir = 0;
			nlop = 0;
			break;
			case 1:
			fcyl = bcyl;
			case 4:
			CDir = put();
			flxl = bcxl;
			flyl = bcyl;
			flxm = bcxm;
			flym = bcym;
			frxl = LONG_MAX;
			fryl = bcyl;
			frxm = LONG_MAX;
			frym = bcyl;
			getsrc();
			nlop = 0;
			break;
			case 2:
			if(bcyl!=flyl||bcxl!=flxl)
			{
				awaycrn();
				nlop = 0;
				break;
			}
			case 3:
			flxl = bcxl;
			flyl = bcyl;
			flxm = bcxm;
			flym = bcym;
			getsrc();
			nlop = 0;
			break;
			case 5:
			ODir = CDir;
			CDir = NDir;
			nlop = 0;
			break;
			case 6:
			CDir = put();
			flxl = nlxl;
			flyl = nlyl;
			flxm = nlxm;
			flym = nlym;
			nlym = nlyl;
			frxl = bcxl;
			fryl = bcyl;
			frxm = bcxm;
			frym = bcym;
			getsrc();
			nlop = 0;
			break;
			case 7:
			++nlop;
			error = merge();
			if(error)
			{
				break;
			}
			if(fryl!=frym||fcyl!=flyl)
			{
				break;
			}
			case 8:
			frxl = bcxl;
			fryl = bcyl;
			frxm = bcxm;
			frym = bcym;
			getsrc();
			nlop = 0;
			break;
			default: error = 1;
		}
		if(error)
		{
			break;
		}
	}
	if(error)zAbort(fboxes,__LINE__);
}
