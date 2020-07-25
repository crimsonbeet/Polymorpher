#pragma once
// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

const COLORREF black 	   = RGB(   0,    0,    0);
const COLORREF darkred     = RGB(0x80,    0,    0);
const COLORREF darkgreen   = RGB(   0, 0x80,    0);
const COLORREF darkyellow  = RGB(0x80, 0x80,    0);
const COLORREF darkblue    = RGB(   0,    0, 0x80);
const COLORREF darkmagenta = RGB(0x80,    0, 0x80);
const COLORREF darkcyan    = RGB(   0, 0x80, 0x80);
const COLORREF darygray    = RGB(0x80, 0x80, 0x80);

const COLORREF moneygreen  = RGB(0xC0, 0xDC, 0xC0);
const COLORREF skyblue     = RGB(0xA6, 0xCA, 0xF0);
const COLORREF cream       = RGB(0xFF, 0xFB, 0xF0);
const COLORREF mediumgray  = RGB(0xA0, 0xA0, 0xA4);
const COLORREF lightgray   = RGB(0xC0, 0xC0, 0xC0);

const COLORREF red         = RGB(0xFF,    0,    0);
const COLORREF green       = RGB(   0, 0xFF,    0);
const COLORREF yellow      = RGB(   0, 0xFF,    0);
const COLORREF blue        = RGB(   0,    0, 0xFF);
const COLORREF magenta	   = RGB(0xFF,    0, 0xFF);
const COLORREF cyan	       = RGB(   0, 0xFF, 0xFF);
const COLORREF white	   = RGB(0xFF, 0xFF, 0xFF);


void minmax(int x0, int x1, int x2, int x3, int & minx, int & maxx);

int GetDIBPixelSize(const BITMAPINFOHEADER & bmih);
int GetDIBColorCount(const BITMAPINFOHEADER & bmih);


PBITMAPINFO BitmapToDIB(HPALETTE hPal/*palette for color conversion*/, HBITMAP hBmp, int nBitCount = 0, int nCompression = BI_RGB);	


int PixelFormat(HDC hdc);
const char * PixelFormatName(int id);


void EXP_KWIN SaveWindowToDIB(const char * pFileName, HWND hWnd, int nBitCount, int nCompression);
BOOL EXP_KWIN SaveDIBToFile(const char * pFileName, const BITMAPINFO * pBMI, const void * pBits);


BOOL DIB2DIBAlphaBlend3232(
	BITMAPINFO * pBMIDst, BYTE * pBitsDst, int dx, int dy, int w, int h,
	BITMAPINFO * pBMISrc, BYTE * pBitsSrc, int sx, int sy,
	BLENDFUNCTION blend);


// AlphaBlending a 32-bpp DIB to hdc
BOOL DIB2HDCAlphaBlend(HDC hdc, int x0, int y0, const BITMAPINFO *pbmpInfo, const void * pAlphaImageData);




class KColor
{
	typedef enum
	{
		Red,
		Green,
		Blue
	};

public:

	unsigned char red;
	unsigned char green;
	unsigned char blue;

	double lightness, saturation, hue;

	KColor()
	{
	}

	KColor(double h, double l, double s)
	{
		hue		   = h;
		lightness  = l;
		saturation = s;
	}

	KColor (BYTE r, BYTE g, BYTE b)
	{
		red   = r;
		green = g;
		blue  = b;
	}

	KColor(COLORREF color)
	{
		red   = GetRValue(color);
		green = GetGValue(color);
		blue  = GetBValue(color);
	}
	
	void ToHLS(void);
	void ToRGB(void);

	COLORREF GetColorRef(void) const
	{
		return RGB(red, green, blue);
	}
};



// eM11	eM12
// eM21	eM22
// eDx	eDy
// x' = m11 * x + m21 * y + dx
// y' = m12 * x + m22 * y + dy

class KAffine
{
public:
	XFORM m_xm;

	KAffine()
	{
		Reset();
	}

	void Map(int x, int y, float & rx, float & ry)
	{
		rx = m_xm.eM11 * x + m_xm.eM21 * y + m_xm.eDx;
		ry = m_xm.eM12 * x + m_xm.eM22 * y + m_xm.eDy;
	}
	
	void Reset();
	BOOL SetTransform(const XFORM & xm);
	BOOL Combine(const XFORM & b);
	BOOL Invert(void);

	BOOL Translate(FLOAT dx, FLOAT dy);
	BOOL Scale(FLOAT sx, FLOAT dy);
	BOOL Rotate(FLOAT angle, FLOAT x0=0, FLOAT y0=0);

	BOOL MapTri(FLOAT px0, FLOAT py0, FLOAT qx0, FLOAT qy0, FLOAT rx0, FLOAT ry0);

	BOOL MapTri(FLOAT px0, FLOAT py0, FLOAT qx0, FLOAT qy0, FLOAT rx0, FLOAT ry0,
				FLOAT px1, FLOAT py1, FLOAT qx1, FLOAT qy1, FLOAT rx1, FLOAT ry1);

	BOOL MapTriInt(int px0, int py0, int qx0, int qy0, int rx0, int ry0,
				int px1, int py1, int qx1, int qy1, int rx1, int ry1)
	{
		return MapTri((float) px0, (float) py0, (float) qx0, (float) qy0, (float) rx0, (float) ry0,
					  (float) px1, (float) py1, (float) qx1, (float) qy1, (float) rx1, (float) ry1);
	}

	BOOL GetDPtoLP(HDC hDC);
};


class KReverseAffine : public KAffine
{
	int x0, y0, x1, y1, x2, y2;

public:
	int minx, maxx, miny, maxy;

	KReverseAffine(const POINT * pPoint)
	{
		x0 = pPoint[0].x;	//   P0       P1
		y0 = pPoint[0].y;	//
		x1 = pPoint[1].x;	//
		y1 = pPoint[1].y;	//   P2       P3
		x2 = pPoint[2].x;
		y2 = pPoint[2].y;
	}

	bool Simple(void) const
	{
		return (y0==y1) && (x0==x2);
	}

	void Setup(int nXSrc, int nYSrc, int nWidth, int nHeight)
	{
		MapTriInt(x0, y0, x1, y1, x2, y2, nXSrc, nYSrc, nXSrc+nWidth, nYSrc, nXSrc, nYSrc+nHeight);
		
		minmax(x0, x1, x2, x2 + x1 - x0, minx, maxx);
		minmax(y0, y1, y2, y2 + y1 - y0, miny, maxy);
	}
};




typedef enum 
{
	DIB_1BPP,			//   2 color image, palette-based
	DIB_2BPP,			//   4 color image, palttte-based
	DIB_4BPP,			//  16 color image, palette-based
	DIB_4BPPRLE,		//  16 color image, palette-based, RLE compressed
	DIB_8BPP,			// 256 color image, palette-based 
	DIB_8BPPRLE,		// 256 color image, palette-based, RLE compressed

	DIB_16RGB555,		// 15 bit RGB color image, 5-5-5
	DIB_16RGB565,		// 16 bit RGB color image, 5-6-5, 1 bit unused
	DIB_24RGB888,		// 24 bit RGB color image, 8-8-8
	DIB_32RGB888,		// 32 bit RGB color image, 8-8-8, 8 bit unused

	DIB_32RGBA8888,		// 32 bit RGBA color image, 8-8-8-8

	DIB_16RGBbitfields,	// 16 bit RGB color image, non-standard bit masks, NT-only
	DIB_32RGBbitfields,	// 32 bit RGB color image, non-standard bit masks, NT-only

	DIB_JPEG,			// embedded JPEG image
	DIB_PNG				// embedded PNG image
}  DIBFormat;



class KBitmapInfo { 
public:
	typedef struct { 
		BITMAPINFOHEADER bmiHeader; 
		RGBQUAD	bmiColors[256 + 3]; 
	} DIBINFO; 

	DIBINFO _dibinfo;

	void SetFormat(int width, int height, int bitcount, int compression = BI_RGB);

	void SetGrayScale(DWORD mask);

	PBITMAPINFO Get(void) const { 
		return (PBITMAPINFO)&_dibinfo; 
	}

	PBITMAPINFO Copy(void) const;
};





typedef enum 
{
	DIB_BMI_NEEDFREE   = 1,
	DIB_BMI_READONLY   = 2,
	DIB_BITS_NEEDFREE  = 4,
	DIB_BITS_READONLY  = 8	
};



class EXP_KWIN KDDB; // Forward declaration. 


class EXP_KWIN KDIB { 
public:
	DIBFormat	 _imageFormat;	// pixel array format
	int			 _ctlFlags;		// DIB_BMI_NEEDFREE, ....
	BITMAPINFO * _pbmInfo;		// Bitmap Information Header + mask + color table
	BYTE       * _pbmBits;		// pixel array

	RGBTRIPLE  * _prgbTriple;	// OS/2 DIB color table within _pbmInfo
	RGBQUAD    * _prgbQuad;		// V3,4,5 DIB color table within _pbmInfo
	int			 _clrUsed;		// No of color in color table
	int			 _clrImprtnt;	// Real color used
	DWORD	   * _pbitMasks;	// 16, 32-bpp masks within _pbmInfo
	
	int			 _width;		// image pixel width
	int			 _height;		// image pixel height, positive
	int			 _planes;		// plane count
	int			 _bitCount;		// bit per plane
	int			 _colorDepth;	// bit count * plane count
	int			 _imageSize;	// pixel array size

	int			 _compression; 

	// Precalculated values

	int			 _bytsPscline;	// byte per scan line, per plane
	BYTE	   * _porigin;		// point to logical origin
	int			 _ndelta;		// delta to next scan line

	KDIB(); // default constructor, empty image
	virtual ~KDIB(); 
	void Init(); 

	BOOL Create(int width, int height, int bitcount);

	BOOL Attach(PBITMAPINFO pbminfo, PBYTE pBits = 0, int flags = 0); 
	BOOL Attach(KDDB& kddb); 
	void Release(); 

	BOOL LoadBitmap(HMODULE hModlue, LPCTSTR pBitmapName);

	bool IsCompressed(void) const {
		return (_imageFormat == DIB_4BPPRLE) || (_imageFormat == DIB_8BPPRLE) || (_imageFormat == DIB_JPEG) || (_imageFormat == DIB_PNG);
	}

	int GetColorCount(void) const;

	int Draw(HDC hdc, int xdst, int ydst, int wdst, int hdst, int x, int y, int width, int height, DWORD rop = SRCCOPY) {
		if(_pbmInfo) { 
			return StretchDIBits(hdc, xdst, ydst, wdst, hdst, x, y, width, height, _pbmBits, _pbmInfo, DIB_RGB_COLORS, rop); 
		} 
		return GDI_ERROR; 
	}

	int Set(HDC hdc, int x, int y) {
		if(_pbmInfo) { 
			return SetDIBitsToDevice(hdc, x, y, _width, _height, 0, 0, 0, _height, _pbmBits, _pbmInfo, DIB_RGB_COLORS); 
		} 
		return GDI_ERROR;
	}

	HBITMAP ConvertToDDB(HDC hdc) { 
		return CreateDIBitmap(hdc, &_pbmInfo->bmiHeader, CBM_INIT, _pbmBits, _pbmInfo, DIB_RGB_COLORS); 
	} 

	DWORD GetPixel(int x, int y) const;
	DWORD GetPixelRGB(int x, int y) const; 

	BOOL SetPixel(int x, int y, DWORD index);
};



class KDIBGammaCorrect { 
	BYTE _redGammaRamp[256]; 
	BYTE _greenGammaRamp[256]; 
	BYTE _blueGammaRamp[256]; 

	inline BYTE gamma(double g, int index) { 
		return std::min(255, (int)((255.0 * pow(index/255.0, 1.0/g)) + 0.5 )); 
	} 

public: 
	KDIBGammaCorrect(double redgamma, double greengamma, double bluegamma) { // gamma values between 0.2 and 5.0.
		for(int i = 0; i < 256; ++i) { 
			_redGammaRamp[i] = gamma(redgamma, i);
			_greenGammaRamp[i] = gamma(greengamma, i);
			_blueGammaRamp[i] = gamma(bluegamma, i);
		}
	} 

	inline void operator()(BYTE& red, BYTE& green, BYTE& blue) { 
		red = _redGammaRamp[red];
		green = _greenGammaRamp[green];
		blue = _blueGammaRamp[blue];
	} 
}; 


bool EXP_KWIN KDIBGrayColorTransform(KDIB& kdib); 

bool EXP_KWIN KDIBGammaCorrectTransform(KDIB& kdib, KDIBGammaCorrect& gammaCorrect); 


bool EXP_KWIN KDIBSplitChannel(const KDIB& kdib, BYTE (*map)(BYTE, BYTE, BYTE), BITMAPINFO **p); 


BYTE EXP_KWIN KDIBTakeRed(BYTE red, BYTE green, BYTE blue); 
BYTE EXP_KWIN KDIBTakeGreen(BYTE red, BYTE green, BYTE blue); 
BYTE EXP_KWIN KDIBTakeBlue(BYTE red, BYTE green, BYTE blue); 
// KCMY channel splitting operators 
BYTE EXP_KWIN KDIBTakeK(BYTE red, BYTE green, BYTE blue); 
BYTE EXP_KWIN KDIBTakeC(BYTE red, BYTE green, BYTE blue); 
BYTE EXP_KWIN KDIBTakeM(BYTE red, BYTE green, BYTE blue); 
BYTE EXP_KWIN KDIBTakeY(BYTE red, BYTE green, BYTE blue); 
// HLS channel splitting operators ;
BYTE EXP_KWIN KDIBTakeH(BYTE red, BYTE green, BYTE blue); 
BYTE EXP_KWIN KDIBTakeL(BYTE red, BYTE green, BYTE blue); 
BYTE EXP_KWIN KDIBTakeS(BYTE red, BYTE green, BYTE blue); 




class EXP_KWIN KDDB {
protected:
	HBITMAP	_hbitmap; 
	HBITMAP _holdbitmap; 

	void Release(void); 

public:
	HDC	_hmemdc; 
	bool GetDimensions(int& width, int& height); 

	UINT _transparentclr; 

	HBITMAP GetBitmap(void) const { 
		return _hbitmap; 
	} 

	KDDB(); 
	virtual ~KDDB(); 

	BOOL Attach(HBITMAP hbitmap); 
	BOOL Attach(KDIB& kdib); 

	typedef enum { 
		KDDBdraw_normal, 
		KDDBdraw_center, 
		KDDBdraw_tile, 
		KDDBdraw_stretch, 
		KDDBdraw_stretchprop 
	}; 

	int Draw(HDC hdc, int x0, int y0, int w = 0, int h = 0, int opt = KDDBdraw_stretch, DWORD rop = SRCCOPY); 
	int TransparentDraw(HDC hdc, int xdst, int ydst, int wdst, int hdst, int x = 0, int y = 0, int width = 0, int height = 0); 

	HBITMAP CreateMask(COLORREF crBackGround, HDC hMaskdc); 
}; 


class EXP_KWIN KDIBSection : public KDIB, public KDDB { 
protected: 
	void Release(); 

	UINT GetColorTable();
	UINT SetColorTable(); 

public:
	KDIBSection(); 
	virtual ~KDIBSection(); 

	BOOL CreateDIBSection(const BITMAPINFO * pbmi, UINT iUsage = DIB_RGB_COLORS, HANDLE hSection = NULL, DWORD dwOffset = 0); 
	BOOL CreateDIBSection32(int width, int height); 

	BOOL Attach(HBITMAP hbitmap); 
	BOOL Attach(PBITMAPINFO pbminfo, PBYTE pBits = 0, int flags = 0); 

	BOOL Attach(KDIB& kdib); 
	BOOL Attach(KDDB& kddb); 

	int Draw(HDC hdc, int x0, int y0, int w = 0, int h = 0, int opt = KDDBdraw_stretch, DWORD rop = SRCCOPY); 
	int Draw(HDC hdc, int x0, int y0, int w, int h, int x, int y, int width, int height, DWORD rop = SRCCOPY); 

	int AlphaBlend(HDC hdc, RECT& rdst, RECT *prSrc = 0, BYTE alpha = 255, BYTE alphaformat = AC_SRC_ALPHA/*set to 0 for constant alpha*/); 

	void RemoveAlphaChannel(); 

	template <class ALPHAFunction> BOOL SetAlphaChannel(ALPHAFunction& computeAlpha) { 
		if(!_pbmBits) { 
			return FALSE; 
		} 
		if(_bitCount != 32) { 
			return FALSE; 
		} 
		if(_compression != BI_RGB) { 
			return FALSE; 
		} 

		RECT rect = { 0, 0, _width, _height }; 
		PBYTE pPixel = _pbmBits; 
		for(int y = 0; y < _height; ++y) { 
			for(int x = 0; x < _width; ++x, pPixel += 4) { 
				POINT point = { x, y }; 
				BYTE alpha = computeAlpha(point, rect); 
				pPixel[0] = pPixel[0] * alpha / 255; 
				pPixel[1] = pPixel[1] * alpha / 255; 
				pPixel[2] = pPixel[2] * alpha / 255; 
				pPixel[3] = alpha; 

			} 
		} 
		return TRUE; 
	} 
};

BYTE EXP_KWIN ComputeAlphaRadial(POINT& point, RECT& rect); 
BYTE EXP_KWIN ComputeAlphaCentral(POINT& point, RECT& rect); 
