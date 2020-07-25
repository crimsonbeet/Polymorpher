// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include "stdafx.h"

#include "Dib.h"




KDIB::KDIB() { 
	Init(); 
} 

void KDIB::Init() { 
	_pbmInfo         = NULL;
	_pbmBits        = NULL;
	_ctlFlags		   = 0;

	_compression = 0; 

	_width       = 0;
	_height      = 0;
	_planes      = 1;
	_bitCount    = 1;
	_colorDepth  = 1;
	_imageSize   = 0;
	_imageFormat = DIB_1BPP;

	_prgbTriple   = NULL;
	_prgbQuad	   = NULL;
} 


void KDIB::Release(void) { 
	if(_ctlFlags & DIB_BMI_NEEDFREE) { 
		delete (PBYTE)_pbmInfo; 

		_pbmInfo = 0; 
		_ctlFlags &= ~ DIB_BMI_NEEDFREE;
	}

	if(_ctlFlags & DIB_BITS_NEEDFREE) { 
		delete [] _pbmBits;

		_pbmBits = 0; 
		_ctlFlags &= ~ DIB_BITS_NEEDFREE;
	}
}

KDIB::~KDIB() { 
	Release();
}

BOOL KDIB::Attach(PBITMAPINFO pbminfo, PBYTE pBits, int flags) { 
	Release(); 
	Init(); 

	_pbmInfo = pbminfo;
	_ctlFlags = flags;

	DWORD size = *(PDWORD)pbminfo; // always DWORD size, key to information header

	BOOL ok = FALSE; 

//	gather information from bitmap information header structures 
	switch(size) {
		case sizeof(BITMAPCOREHEADER): {
			BITMAPCOREHEADER *pHeader = (BITMAPCOREHEADER*)pbminfo;

			_compression = BI_RGB; 

			_width = pHeader->bcWidth;
			_height = pHeader->bcHeight;
			_planes = pHeader->bcPlanes;
			_bitCount = pHeader->bcBitCount;
			_imageSize = 0;

			_pbitMasks = 0;
			_prgbQuad = 0; 

			if(_bitCount <= 8) {
				_clrUsed = 1 << _bitCount;
				_clrImprtnt = _clrUsed;

				_prgbTriple = (RGBTRIPLE*)((PBYTE)_pbmInfo + size); 
				_pbmBits = (PBYTE)&_prgbTriple[_clrUsed]; 
			}
			else { 
				_clrUsed = 0;
				_clrImprtnt = 0; 

				_prgbTriple = 0; // No color table
				_pbmBits = (PBYTE)_pbmInfo + size; 
			}  

			ok = TRUE; 
			break;
		}

		case sizeof(BITMAPINFOHEADER):
		case sizeof(BITMAPV4HEADER):
		case sizeof(BITMAPV5HEADER): { 
			BITMAPINFOHEADER * pHeader = & _pbmInfo->bmiHeader; 

			_compression = pHeader->biCompression;

			_width = pHeader->biWidth;
			_height = pHeader->biHeight;
			_planes = pHeader->biPlanes;
			_bitCount = pHeader->biBitCount;
			_imageSize= pHeader->biSizeImage;

			_clrUsed = pHeader->biClrUsed;
			_clrImprtnt = pHeader->biClrImportant;

			_prgbTriple = 0; 

			if(_bitCount <= 8) { 
				if(_clrUsed == 0) { // 0 means full color table
					_clrUsed = 1 << _bitCount; 
				} 
			} 

			if(_clrUsed) { // has a color table
				if(_clrImprtnt == 0)	{ // 0 means all important
					_clrImprtnt = _clrUsed; 
				} 
			
				if(_compression == BI_BITFIELDS) { // 3 bit-masks provided to specify how to extract RGB component from a pixel. 
					_pbitMasks = (PDWORD)((PBYTE)_pbmInfo + size);
					_prgbQuad = (RGBQUAD*)((PBYTE)_pbmInfo + size + 3 * sizeof(DWORD)); 
				}
				else { 
					_pbitMasks = 0;
					_prgbQuad = (RGBQUAD*)((PBYTE)pbminfo + size); 
				} 

				_pbmBits = (PBYTE)&_prgbQuad[_clrUsed]; 
			}
			else {
				if(_compression == BI_BITFIELDS) {
					_pbitMasks = (PDWORD)((PBYTE)_pbmInfo + size);
					_pbmBits = ((PBYTE)_pbmInfo + size + 3 * sizeof(DWORD)); 
				}
				else
					_pbmBits = (PBYTE)_pbmInfo + size; 
			}

			ok = TRUE; 
			break;
		}
	}

	if(!ok) { 
		return FALSE; 
	} 

	if(pBits) { 
		_pbmBits = pBits; 
	} 

//	precalculate information DIB parameters 

	_colorDepth = _planes * _bitCount; // DIB format only supports single plain images. 

	_bytsPscline = (_width * _bitCount + 31) / 32 * 4;
	
	if(_height < 0) { // top-down bitmap
		_height = -_height; // change to positive 

		_ndelta = _bytsPscline; // forward
		_porigin = _pbmBits; // scan0 .. scanN-1 
	}
	else {
		_ndelta = -_bytsPscline; // backward
		_porigin = _pbmBits + (_height - 1) * _bytsPscline * _planes;	// scanN-1..scan0 
	}

	if(_imageSize == 0) { 
		_imageSize = _bytsPscline * _planes * _height; 
	} 

	ok = FALSE; 

//	convert compression mode to image format.

	switch(_bitCount) { 
		case 1:
			_imageFormat = DIB_1BPP; 
			ok = TRUE; 
		break;

		case 2:
			_imageFormat = DIB_2BPP;
			ok = TRUE; 
			break;

		case 4:
			if(_compression == BI_RLE4) { 
				_imageFormat = DIB_4BPPRLE; 
			} 
			else { 
				_imageFormat = DIB_4BPP; 
			} 
			ok = TRUE; 
			break;

		case 8:
			if(_compression == BI_RLE8) { 
				_imageFormat = DIB_8BPPRLE; 
			} 
			else { 
				_imageFormat = DIB_8BPP; 
			} 
			ok = TRUE; 
			break;
		
		case 16:
			if(_compression == BI_BITFIELDS ) { 
				_imageFormat = DIB_16RGBbitfields; 
			} 
			else { 
				_imageFormat = DIB_16RGB555; // see below 
			} 
			ok = TRUE; 
			break;
		
		case 24:
			_imageFormat = DIB_24RGB888;
			ok = TRUE; 
			break;

		case 32:
			if(_compression == BI_BITFIELDS) { 
				_imageFormat = DIB_32RGBbitfields; 
			} 
			else { 
				_imageFormat = DIB_32RGB888; // see below 
			} 
			ok = TRUE; 
			break;
	}

//	try to understand bit-masks 

	if(_compression == BI_BITFIELDS) { 

		DWORD red = _pbitMasks[0];
		DWORD green = _pbitMasks[1];
		DWORD blue = _pbitMasks[2];

		if((blue == 0x001F) && (green == 0x03E0) && (red == 0x7C00)) { 
			_imageFormat = DIB_16RGB555; 
		} 
		else 
		if((blue == 0x001F) && (green == 0x07E0) && (red==0xF800)) { 
			_imageFormat = DIB_16RGB565; 
		} 
		else 
		if((blue == 0x00FF) && (green == 0xFF00) && (red == 0xFF0000)) { 
			_imageFormat = DIB_32RGB888; 
		} 
	}

	return ok;
}


BOOL KDIB::Attach(KDDB& kddb) { 
	BOOL ok = FALSE; 

	PBITMAPINFO pbminfo = BitmapToDIB(NULL, kddb.GetBitmap()); 
	if(pbminfo) { 
		ok = Attach(pbminfo, 0, DIB_BMI_NEEDFREE); 
	} 

	return ok; 
} 


BOOL KDIB::LoadBitmap(HMODULE hModule, LPCTSTR pBitmapName) { 
	HRSRC hRes = FindResource(hModule, pBitmapName, RT_BITMAP);
	if(hRes == NULL ) { 
		return FALSE; 
	} 

	HGLOBAL hGlb = LoadResource(hModule, hRes);

	if(hGlb == NULL) { 
		return FALSE; 
	} 

	PBITMAPINFO pbminfo = (BITMAPINFO *) LockResource(hGlb); 
	if(pbminfo == NULL) { 
		return FALSE; 
	} 

	return Attach(pbminfo, NULL, DIB_BMI_READONLY | DIB_BITS_READONLY); 
}



const BYTE Shift1bpp[] = { 7,     6,     5,     4,     3,     2,     1,     0     };
const BYTE Mask1bpp [] = { ~0x80, ~0x40, ~0x20, ~0x10, ~0x08, ~0x04, ~0x02, ~0x01 };

const BYTE Shift2bpp[] = { 6,     4,     2,     0     };
const BYTE Mask2bpp [] = { ~0xC0, ~0x30, ~0x0C, ~0x03 };

const BYTE Shift4bpp[] = { 4, 0 };
const BYTE Mask4bpp [] = { ~0xF0, ~0x0F };


DWORD KDIB::GetPixel(int x, int y) const { 
	if((x < 0) || (x >= _width)) { 
		return -1; 
	} 
	if((y < 0) || (y >= _height)) { 
		return -1; 
	} 

	BYTE * pPixel = _porigin + y * _ndelta;

	switch(_imageFormat) { 
		case DIB_1BPP:
			return ( pPixel[x/8] >> Shift1bpp[x%8] ) & 0x01;

		case DIB_2BPP:
			return ( pPixel[x/4] >> Shift2bpp[x%4] ) & 0x03;

		case DIB_4BPP:
			return ( pPixel[x/2] >> Shift4bpp[x%2] ) & 0x0F;

		case DIB_8BPP:
			return pPixel[x];

		case DIB_16RGB555:
		case DIB_16RGB565:
			return ((WORD *)pPixel)[x];

		case DIB_24RGB888:
			pPixel += x * 3;
			return (pPixel[0]) | (pPixel[1] << 8) | (pPixel[2] << 16);
		
		case DIB_32RGB888:
		case DIB_32RGBA8888:
			return ((DWORD *)pPixel)[x];
	}

	return -1;
}


DWORD KDIB::GetPixelRGB(int x, int y) const { 
	if((x < 0) || (x >= _width)) { 
		return -1; 
	} 
	if((y < 0) || (y >= _height)) { 
		return -1; 
	} 

	BYTE * pPixel = _porigin + y * _ndelta; 

	DWORD indx = -1; 

	switch(_imageFormat) { 
		case DIB_1BPP:
			indx = ( pPixel[x/8] >> Shift1bpp[x%8] ) & 0x01; 
			break; 

		case DIB_2BPP:
			indx = ( pPixel[x/4] >> Shift2bpp[x%4] ) & 0x03;
			break; 

		case DIB_4BPP:
			indx = ( pPixel[x/2] >> Shift4bpp[x%2] ) & 0x0F;
			break; 

		case DIB_8BPP:
			indx = pPixel[x];
			break; 

		case DIB_16RGB555:
		case DIB_16RGB565:
			return ((WORD *)pPixel)[x];

		case DIB_24RGB888:
			pPixel += x * 3;
			return (pPixel[0]) | (pPixel[1] << 8) | (pPixel[2] << 16);
		
		case DIB_32RGB888:
		case DIB_32RGBA8888:
			return ((DWORD *)pPixel)[x];
	} 

	PBYTE rgb = (PBYTE)&indx; 

	if(indx < (DWORD)_clrUsed) { 
		if(_prgbQuad) { 
			RGBQUAD quad = _prgbQuad[indx]; 
			rgb[0] = quad.rgbRed; 
			rgb[1] = quad.rgbGreen; 
			rgb[2] = quad.rgbBlue; 
			rgb[3] = 0; 
		} 
		else 
		if(_prgbTriple) { 
			RGBTRIPLE triple = _prgbTriple[indx]; 
			rgb[0] = triple.rgbtRed; 
			rgb[1] = triple.rgbtGreen; 
			rgb[2] = triple.rgbtBlue; 
			rgb[3] = 0; 
		} 
		else { 
			indx = -1; 
		} 

	} 

	return indx;
} 


BOOL KDIB::SetPixel(int x, int y, DWORD index) { 
	if((x < 0) || (x >= _width)) { 
		return FALSE; 
	} 

	if((y < 0) || (y >= _height)) { 
		return FALSE; 
	} 

	BYTE * pPixel = _porigin + y * _ndelta;
	
	switch(_imageFormat) {
		case DIB_1BPP:
			pPixel[x/8] = (BYTE) ( ( pPixel[x/8] & Mask1bpp[x%8] ) | ( (index & 1) << Shift1bpp[x%8] ) );
			break;

		case DIB_2BPP:
			pPixel[x/4] = (BYTE) ( ( pPixel[x/4] & Mask2bpp[x%4] ) | ( (index & 3) << Shift2bpp[x%4] ) );
			break;

		case DIB_4BPP:
			pPixel[x/2] = (BYTE) ( ( pPixel[x/2] & Mask4bpp[x%2] ) | ( (index & 15) << Shift4bpp[x%2] ) );
			break;

		case DIB_8BPP:
			pPixel[x] = (BYTE) index;
			break;

		case DIB_16RGB555:
		case DIB_16RGB565:
			((WORD *)pPixel)[x] = (WORD) index;
			break;

		case DIB_24RGB888:
			((RGBTRIPLE *)pPixel)[x] = * ((RGBTRIPLE *) & index);
			break;

		case DIB_32RGB888:
		case DIB_32RGBA8888:
			((DWORD *)pPixel)[x] = index;
			break;

		default:
			return FALSE;
	}

	return TRUE;
}


BOOL KDIB::Create(int width, int height, int bitcount) { 
	KBitmapInfo dibinfo; 

	dibinfo.SetFormat(width, height, bitcount, BI_RGB); 

	int nBitsSize = (width * bitcount + 31) /32 * 4 * height; 
	PBYTE pBits = new BYTE[nBitsSize]; 

	if(pBits) { 
		return Attach(dibinfo.Copy(), pBits, 0); 
	} 

	return FALSE;
}




KDDB::KDDB() { 
	_hbitmap = NULL; 
	_hmemdc  = NULL; 
	_holdbitmap = NULL; 

	_transparentclr = 0; 
} 

KDDB::~KDDB() { 
	Release(); 
} 

// quey size 
bool KDDB::GetDimensions(int& width, int& height) { 
	width = 0; 
	height = 0; 
	if(_hbitmap == NULL) { 
		return false; 
	} 

	BITMAP bmp; 
	if(GetObject(_hbitmap, sizeof(bmp), &bmp)) { 
		width = bmp.bmWidth; 
		height = bmp.bmHeight; 
	} 
	return _holdbitmap != NULL;
}

// Convert color DDB into monochrome mask based on a background color
HBITMAP KDDB::CreateMask(COLORREF crBackGround, HDC hMaskdc) { 
	int width;
	int height;
	if(!GetDimensions(width, height)) { 
		return 0; 
	} 

	SetBkColor(_hmemdc, crBackGround); 

	HBITMAP hMask = CreateBitmap(width, height, 1/*nPlanes*/, 1/*nBitcount*/, NULL); 

	HBITMAP hOld = (HBITMAP)SelectObject(hMaskdc, hMask);

	BitBlt(hMaskdc, 0, 0, width, height, _hmemdc, 0, 0, SRCCOPY);

	return hOld;
}

// Relase resource
void KDDB::Release(void) { 
	if(_hmemdc) {
		if(_holdbitmap) { 
			SelectObject(_hmemdc, _holdbitmap); 
		} 
		DeleteObject(_hmemdc);
		_hmemdc = NULL;
	}

	if(_hbitmap) { 
		DeleteObject(_hbitmap);
		_hbitmap = NULL;
	}

	_holdbitmap = NULL;
}


BOOL KDDB::Attach(HBITMAP hbmp) { 
	if(hbmp == NULL) { 
		return FALSE; 
	} 

	if(_holdbitmap)	{ 
		SelectObject(_hmemdc, _holdbitmap);
		_holdbitmap = NULL;
	}

	if(_hbitmap) { 
		DeleteObject(_hbitmap); 
	} 

	_hbitmap = hbmp; // replace with new one. 

	if(_hmemdc)	{ 
		_holdbitmap = (HBITMAP)SelectObject(_hmemdc, _hbitmap); 
	} 
	else { 
		BITMAP bmp; 
		if(GetObject(_hbitmap, sizeof(bmp), &bmp)) { 
			HDC hdc = GetWindowDC(NULL); 
			_hmemdc = CreateCompatibleDC(hdc); 
			_holdbitmap = (HBITMAP)SelectObject(_hmemdc, _hbitmap); 
			ReleaseDC(NULL, hdc); 
		} 
	} 

	return _holdbitmap != NULL; 
} 

BOOL KDDB::Attach(KDIB& kdib) { 
	BOOL ok = FALSE; 
	HDC hdc = GetDC(NULL); 

	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, kdib._width, kdib._height); 
	if(Attach(hbitmap)) { 
		ok = kdib.Set(_hmemdc, 0, 0) != GDI_ERROR; 
	} 

	_transparentclr = kdib.GetPixelRGB(0, kdib._height - 1); 

	ReleaseDC(NULL, hdc); 
	return ok; 
} 


int KDDB::Draw(HDC hdc, int x0, int y0, int w, int h, int opt, DWORD rop) { 
	int bmpwidth = 0; 
	int bmpheight = 0; 

	if(!GetDimensions(bmpwidth, bmpheight)) { 
		return GDI_ERROR; 
	} 

	if(w == 0) { 
		w = bmpwidth; 
	} 
	if(h == 0) { 
		h = bmpheight; 
	} 

	BOOL ok = FALSE; 

	switch(opt)	{ 
		case KDDBdraw_normal:
			ok = BitBlt(hdc, x0, y0, bmpwidth, bmpheight, _hmemdc, 0, 0, rop); 
			break; 

		case KDDBdraw_center: 
			ok = BitBlt(hdc, x0 + (w - bmpwidth) / 2, y0 + (h - bmpheight) / 2, bmpwidth, bmpheight, _hmemdc, 0, 0, rop); 
			break; 

		case KDDBdraw_tile: 
			ok = TRUE; 
			for(int j = 0; ok && j < h; j += bmpheight) 
			for(int i = 0; ok && i < w; i += bmpwidth)
				if(!BitBlt(hdc, x0 + i, y0 + j, bmpwidth, bmpheight, _hmemdc, 0, 0, rop))
					ok = FALSE;

			break; 

		case KDDBdraw_stretch:
			ok = StretchBlt(hdc, x0, y0, w, h, _hmemdc, 0, 0, bmpwidth, bmpheight, rop);
			break; 

		case KDDBdraw_stretchprop: { 
			int ww; 
			int hh; 

			if((w * bmpheight) < (h * bmpwidth)) { // w/bmWidth is the mimimum scale. 
				hh = bmpheight * w / bmpwidth; 
				ww = w; 
			} 
			else { 
				hh = h; 
				ww = bmpwidth  * h / bmpheight; 
			} 

//			proportional scaling and centering. 
			ok = StretchBlt(hdc, x0 + (w - ww) / 2, y0 + (h - hh) / 2, ww, hh, _hmemdc, 0, 0, bmpwidth, bmpheight, rop); 
			break; 
		} 
	} 

	return ok? 0: GDI_ERROR; 
} 

int KDDB::TransparentDraw(HDC hdc, int xdst, int ydst, int wdst, int hdst, int x, int y, int w, int h) { 
	int err = GDI_ERROR; 

	int bmpwidth = 0; 
	int bmpheight = 0; 

	if(!GetDimensions(bmpwidth, bmpheight)) { 
		return err; 
	} 

	if(_hbitmap) { 
		err = TransparentBlt(hdc, xdst, ydst, wdst, hdst, _hmemdc, x, y, w? w: bmpwidth, h? h: bmpheight, _transparentclr); 
	} 
	return err; 
} 


KDIBSection::KDIBSection() { 
} 

KDIBSection::~KDIBSection() { 
	Release(); 
} 

void KDIBSection::Release() { 
	KDDB::Release(); 
	KDIB::Release(); 
} 

// Copy from DIB section color table to DIB color table
UINT KDIBSection::GetColorTable() { 
	if((_colorDepth > 8) || !_hmemdc) { 
		return 0; 
	} 

	return GetDIBColorTable(_hmemdc, 0, _clrUsed, _prgbQuad);
}


// Copy from DIB's color table to DIB section's color table
UINT KDIBSection::SetColorTable() { 
	if((_colorDepth > 8) || !_hmemdc) { 
		return 0; 
	} 

	return SetDIBColorTable(_hmemdc, 0, _clrUsed, _prgbQuad); 
} 


BOOL KDIBSection::CreateDIBSection(const BITMAPINFO *pbmi/*in*/, UINT iUsage/*DIB_RGB_COLORS*/, HANDLE hSection/*NULL*/, DWORD dwOffset/*0*/) {  
	BOOL ok = FALSE; 
	HDC hdc = GetDC(NULL); 

	PVOID pBits = NULL; 
	HBITMAP hbmp = ::CreateDIBSection(hdc, pbmi/*in*/, iUsage, &pBits/*out*/, hSection, dwOffset); 
	if(hbmp) { 
		Release(); // free the previous object
		
		if(KDDB::Attach(hbmp)) { 
			int nColor = GetDIBColorCount(pbmi->bmiHeader);
			int nSize  = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColor;
		
			PBITMAPINFO pdibsec = (PBITMAPINFO)new BYTE[nSize]; 
			if(pdibsec) { 
				memcpy(pdibsec, pbmi, nSize); // copy header and color table. 
				KDIB::Attach(pdibsec, (PBYTE)pBits, DIB_BMI_NEEDFREE); 
				KDIBSection::GetColorTable(); 

				ok = TRUE; 
			} 
		} 

		if(!ok) { 
			Release(); 
		} 
	} 

	ReleaseDC(NULL, hdc); 
	return ok; 
} 


BOOL KDIBSection::CreateDIBSection32(int width, int height) { 
	BITMAPINFO bmi = { { sizeof(BITMAPINFOHEADER), width, height, 1, 32, BI_RGB } }; 

	return CreateDIBSection(&bmi, DIB_RGB_COLORS); 
} 


BOOL KDIBSection::Attach(HBITMAP hbitmap) { 
	if(hbitmap == NULL) { 
		return FALSE; 
	} 

	BOOL ok = FALSE; 

	BITMAP bmp; 
	if(GetObject(hbitmap, sizeof(bmp), &bmp)) { 
		if(CreateDIBSection32(bmp.bmWidth, bmp.bmHeight)) { 
			KDDB kddb; 
			if(kddb.Attach(hbitmap)) { 
				ok = kddb.Draw(_hmemdc, 0, 0) != GDI_ERROR; 
			} 
		} 
	} 

	return ok; 
} 

BOOL KDIBSection::Attach(PBITMAPINFO pbminfo, PBYTE pBits, int flags) { 
	if(pbminfo == NULL) { 
		return FALSE; 
	} 

	BOOL ok = FALSE; 

	if(CreateDIBSection32(pbminfo->bmiHeader.biWidth, pbminfo->bmiHeader.biHeight)) { 
		KDIB kdib; 
		if(kdib.Attach(pbminfo, pBits, flags)) { 
			ok = kdib.Set(_hmemdc, 0, 0) != GDI_ERROR; 
		} 
	} 

	return ok; 
} 

BOOL KDIBSection::Attach(KDIB& kdib) { 
	return Attach(kdib._pbmInfo, kdib._pbmBits); 
} 

BOOL KDIBSection::Attach(KDDB& kddb) { 
	BOOL ok = FALSE; 

	BITMAP bmp; 
	if(GetObject(kddb.GetBitmap(), sizeof(bmp), &bmp)) { 
		if(CreateDIBSection32(bmp.bmWidth, bmp.bmHeight)) { 
			ok = kddb.Draw(_hmemdc, 0, 0) != GDI_ERROR; 
		} 
	} 

	return ok; 
} 

int KDIBSection::Draw(HDC hdc, int x0, int y0, int w, int h, int opt, DWORD rop) { 
	return KDDB::Draw(hdc, x0, y0, w, h, opt, rop); 
} 

int KDIBSection::Draw(HDC hdc, int x0, int y0, int w, int h, int x, int y, int width, int height, DWORD rop) { 
	return KDIB::Draw(hdc, x0, y0, w, h, x, y, width, height, rop); 
} 

void KDIBSection::RemoveAlphaChannel() { 
	if(!_pbmBits) { 
		return; 
	} 
	if(_bitCount != 32) { 
		return; 
	} 
	if(_compression != BI_RGB) { 
		return; 
	} 

	PBYTE pPixel = _pbmBits; 
	for(int y = 0; y < _height; ++y) { 
		for(int x = 0; x < _width; ++x, pPixel += 4) { 
			BYTE alpha = pPixel[3]; 
			if(alpha) { 
				pPixel[0] = ((int)pPixel[0] * 255) / alpha; 
				pPixel[1] = ((int)pPixel[1] * 255) / alpha; 
				pPixel[2] = ((int)pPixel[2] * 255) / alpha; 
				pPixel[3] = 255; 
			} 
		} 
	} 
} 

int KDIBSection::AlphaBlend(HDC hdc, RECT& rdst, RECT *prSrc, BYTE alpha, BYTE alphaformat) { 
	if(_bitCount != 32) { 
		return GDI_ERROR; 
	} 
	if(_compression != BI_RGB) { 
		return GDI_ERROR; 
	} 

	BLENDFUNCTION blend = { AC_SRC_OVER, 0, alpha, alphaformat }; 
	RECT rsrc = { 0, 0, _width, _height }; 
	if(prSrc == NULL) { 
		prSrc = &rsrc; 
	} 

	int x = rdst.left; 
	int y = rdst.top; 
	int w = rdst.right - rdst.left; 
	int h = rdst.bottom - rdst.top; 

	int X = prSrc->left; 
	int Y = prSrc->top; 
	int W = prSrc->right - prSrc->left; 
	int H = prSrc->bottom - prSrc->top; 

	BOOL ok = ::AlphaBlend(hdc, x, y, w, h, _hmemdc, X, Y, W, H, blend); 

	return ok? 0: GDI_ERROR; 
} 


BYTE EXP_KWIN ComputeAlphaRadial(POINT& point, RECT& rect) { 
	int x2 = point.x - rect.right/2; 
	x2 *= x2; 
	int y2 = point.y - rect.bottom/2; 
	y2 *= y2; 
	int dist = (int)(sqrt((double)x2 + y2) * 255 / (std::max(rect.right, rect.bottom) / 2)); 
	return (BYTE)std::max(std::min(255 - dist, 255), 0); 
} 

BYTE EXP_KWIN ComputeAlphaCentral(POINT& point, RECT& rect) { 
	int xc = rect.right/2; 
	int x = point.x > xc? (rect.right - point.x): point.x; 
	int xd = x * (255 + 127) / xc; 

	int yc = rect.bottom/2; 
	int y = point.y > yc? (rect.bottom - point.y): point.y; 
	int yd = y * (255 + 127) / yc; 

	return (BYTE)(std::min(std::min(xd, yd), 255)); 
} 






// lightness	[0..1]
// saturation	[0..1]
// hue			[0..359)

void KColor::ToHLS(void) { 
	double mn; 
	double mx; 
	int major; 
	
	if(red < green) { 
		mn = red; mx = green; major = Green;
	}
	else { 
		mn = green; mx = red; major = Red;
	}

	if(blue < mn) { 
		mn = blue; 
	} 
	else 
	if(blue > mx) { 
		mx = blue; major = Blue;
	}
	
	if(mn == mx) { 
		lightness = mn/255; 
		saturation = 0; 
		hue = 0; 
	}   
	else { 
		lightness = (mn + mx) / 510; 

		if(lightness <= 0.5) { 
			saturation = (mx - mn) / (mn + mx); 
		} 
		else { 
			saturation = (mx - mn) / (510 - mn - mx); 
		} 

		switch(major) { 
			case Red: hue = (green - blue) * 60 / (mx - mn) + 360; break;
			case Green: hue = (blue - red) * 60  / (mx - mn) + 120;  break;
			case Blue: hue = (red - green) * 60 / (mx - mn) + 240; 
		}

		if(hue >= 360) { 
			hue = hue - 360; 
		} 
    }
}

unsigned char Value(double m1, double m2, double h)
{
	while (h >= 360) h -= 360;
	while (h <    0) h += 360;
 
	if (h < 60) 
		m1 = m1 + (m2 - m1) * h / 60;   
	else if (h < 180) 
		m1 = m2;
	else if (h < 240) 
		m1 = m1 + (m2 - m1) * (240 - h) / 60;      
                   
	return (unsigned char)(m1 * 255);
}


void KColor::ToRGB(void)
{
    if (saturation == 0)
    {
	    red = green = blue = (unsigned char) (lightness*255);
    }
    else
    {
		double m1, m2;
         
		if ( lightness <= 0.5 )
			m2 = lightness + lightness * saturation;  
		else                     
			m2 = lightness + saturation - lightness * saturation;
      
		m1 = 2 * lightness - m2;   
      
		red   = Value(m1, m2, hue + 120);   
		green = Value(m1, m2, hue);
		blue  = Value(m1, m2, hue - 120);
    }
}



const RGBQUAD StandardColorTable16[] = {
	{ 0x00, 0x00, 0x00, 0x00 }, 
	{ 0x00, 0x00, 0x80, 0x00 },
	{ 0x00, 0x80, 0x00, 0x00 },
	{ 0x00, 0x80, 0x80, 0x00 },
	{ 0x80, 0x00, 0x00, 0x00 },
	{ 0x80, 0x00, 0x80, 0x00 },
	{ 0x80, 0x80, 0x00, 0x00 },
	{ 0xC0, 0xC0, 0xC0, 0x00 },
	{ 0x80, 0x80, 0x80, 0x00 },
	{ 0x00, 0x00, 0xFF, 0x00 },
	{ 0x00, 0xFF, 0x00, 0x00 },
	{ 0x00, 0xFF, 0xFF, 0x00 },
	{ 0xFF, 0x00, 0x00, 0x00 },
	{ 0xFF, 0x00, 0xFF, 0x00 },
	{ 0xFF, 0xFF, 0x00, 0x00 },
	{ 0xFF, 0xFF, 0xFF, 0x00 }
};

// R G B
const DWORD StandardMask555[] =  { 0x007C00, 0x0003E0, 0x00001F };
const DWORD StandardMask565[] =  { 0x00F800, 0x0007E0, 0x00001F };
const DWORD StandardMask888[] =  { 0xFF0000, 0x00FF00, 0x0000FF };


void KBitmapInfo::SetFormat(int width, int height, int bitcount, int compression) { 
//	fill out BITMAPINFOHEADER based on size and required format 
	memset(&_dibinfo, 0, sizeof(_dibinfo));

	_dibinfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	_dibinfo.bmiHeader.biWidth       = width;
	_dibinfo.bmiHeader.biHeight      = height;
	_dibinfo.bmiHeader.biPlanes      = 1;
	_dibinfo.bmiHeader.biBitCount    = bitcount;
	_dibinfo.bmiHeader.biCompression = compression;

//	set up standard palette 
	switch(bitcount) { 
		case 2:
			_dibinfo.bmiColors[0].rgbRed   = 0;	// black	
			_dibinfo.bmiColors[0].rgbGreen = 0;
			_dibinfo.bmiColors[0].rgbBlue  = 0;
			_dibinfo.bmiColors[1].rgbRed   = 0xFF; // white
			_dibinfo.bmiColors[1].rgbGreen = 0xFF;
			_dibinfo.bmiColors[1].rgbBlue  = 0xFF;

		case 4:
			memcpy(_dibinfo.bmiColors, StandardColorTable16, 16 * sizeof(RGBQUAD));
			break;

		case 8: { // halftone palette with 6*6*6=216 colors
			for(int blue =0; blue <6; blue ++)
			for(int green=0; green<6; green++)
			for(int red  =0; red  <6; red  ++) { 
				int i = (blue * 6 + green) * 6 + red;

				_dibinfo.bmiColors[i].rgbRed = red   * 55; 
				_dibinfo.bmiColors[i].rgbGreen = green * 55; 
				_dibinfo.bmiColors[i].rgbBlue = blue  * 55; 
			}
			break;
		}

		case 16:
			if(compression == BI_BITFIELDS) { 
				memcpy(_dibinfo.bmiColors, StandardMask555, sizeof(StandardMask555)); 
			} 
			break;

		case 32:
			if(compression == BI_BITFIELDS) { 
				memcpy(_dibinfo.bmiColors, StandardMask888, sizeof(StandardMask888)); 
			} 
			break;
	}
}


void KBitmapInfo::SetGrayScale(DWORD mask) { 
	int nColor = 1 << _dibinfo.bmiHeader.biBitCount;

	for (int i=0; i<nColor; i++) { 
		_dibinfo.bmiColors[i].rgbRed = i & GetRValue(mask); 
		_dibinfo.bmiColors[i].rgbGreen = i & GetGValue(mask); 
		_dibinfo.bmiColors[i].rgbBlue = i & GetBValue(mask); 
		_dibinfo.bmiColors[i].rgbReserved = 0; 
	}		
}


PBITMAPINFO KBitmapInfo::Copy(void) const { 
	int nColor;

	if(_dibinfo.bmiHeader.biCompression == BI_BITFIELDS) { 
		nColor = 3; 
	} 
	else 
	if(_dibinfo.bmiHeader.biBitCount <= 8) { 
		nColor = 1 << _dibinfo.bmiHeader.biBitCount; 
	} 
	else
		nColor = 0;

	int nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColor;

	PBITMAPINFO pbmi = (PBITMAPINFO) new BYTE[nSize]; 
	if(pbmi) { 
		memcpy(pbmi, &_dibinfo, nSize); 
	} 
	
	return pbmi;
}




void Map(XFORM * xm, int x, int y, int & rx, int & ry)
{
	rx = (int) ( xm->eM11 * x + xm->eM21 * y + xm->eDx );
	ry = (int) ( xm->eM12 * x + xm->eM22 * y + xm->eDy );
}

BOOL Invert(XFORM & xm)
{
	FLOAT det = xm.eM11 * xm.eM22 - xm.eM21 * xm.eM12;

	if ( det==0 )
		return FALSE;

	XFORM old = xm;

	xm.eM11 =   old.eM22 / det;
	xm.eM12 = - old.eM12 / det;
	xm.eM21 = - old.eM21 / det;
	xm.eM22 =   old.eM11 / det;
	
	xm.eDx  = - ( xm.eM11 * old.eDx + xm.eM21 * old.eDy );
	xm.eDy  = - ( xm.eM12 * old.eDx + xm.eM22 * old.eDy );

	return TRUE;
}


// Reset to identity transform
void KAffine::Reset()
{
	m_xm.eM11 = 1;
	m_xm.eM12 = 0;
	m_xm.eM21 = 0;
	m_xm.eM22 = 1;
	m_xm.eDx  = 0;
	m_xm.eDy  = 0;
}


// Copy transform if valid
BOOL KAffine::SetTransform(const XFORM & xm)
{
	if ( xm.eM11 * xm.eM22 == xm.eM12 * xm.eM21 )
		return FALSE;

	m_xm = xm;	
	return TRUE;
}


// transform = transform * b
BOOL KAffine::Combine(const XFORM & b)
{
	if ( b.eM11 * b.eM22 == b.eM12 * b.eM21 )
		return FALSE;

	XFORM a = m_xm;

	// 11 12   11 12
	// 21 22   21 22
	m_xm.eM11 = a.eM11 * b.eM11 + a.eM12 * b.eM21;
	m_xm.eM12 = a.eM11 * b.eM12 + a.eM12 * b.eM22;
	m_xm.eM21 = a.eM21 * b.eM11 + a.eM22 * b.eM21;
	m_xm.eM22 = a.eM21 * b.eM12 + a.eM22 * b.eM22;
	m_xm.eDx  = a.eDx  * b.eM11 + a.eDy  * b.eM21 + b.eDx;
	m_xm.eDy  = a.eDx  * b.eM12 + a.eDy  * b.eM22 + b.eDy;

	return TRUE;
}


// transform = 1 / transform
//  M = A * x + B 
//  Inv(M) = Inv(A) * x - Inv(A) * B
BOOL KAffine::Invert(void)
{
	FLOAT det = m_xm.eM11 * m_xm.eM22 - m_xm.eM21 * m_xm.eM12;

	if ( det==0 )
		return FALSE;

	XFORM old = m_xm;

	m_xm.eM11 =   old.eM22 / det;
	m_xm.eM12 = - old.eM12 / det;
	m_xm.eM21 = - old.eM21 / det;
	m_xm.eM22 =   old.eM11 / det;
	
	m_xm.eDx  = - ( m_xm.eM11 * old.eDx + m_xm.eM21 * old.eDy );
	m_xm.eDy  = - ( m_xm.eM12 * old.eDx + m_xm.eM22 * old.eDy );

	return TRUE;
}


BOOL KAffine::Translate(FLOAT dx, FLOAT dy)
{
	m_xm.eDx += dx;
	m_xm.eDy += dy;

	return TRUE;
}


BOOL KAffine::Scale(FLOAT sx, FLOAT sy)
{
	if ( (sx==0) || (sy==0) )
		return FALSE;

	m_xm.eM11 *= sx;
	m_xm.eM12 *= sx;
	m_xm.eM21 *= sy;
	m_xm.eM22 *= sy;
	m_xm.eDx  *= sx;
	m_xm.eDy  *= sy;

	return TRUE;
}


BOOL KAffine::Rotate(FLOAT angle, FLOAT x0, FLOAT y0)
{
	XFORM xm;

	Translate(-x0, -y0);	// make (x0,y0) the origin
	
    double rad = angle * (3.14159265359 / 180); 

	xm.eM11 =   (FLOAT) cos(rad);
	xm.eM12 =   (FLOAT) sin(rad);
	xm.eM21 = -         xm.eM12;
	xm.eM22 =           xm.eM11;
	xm.eDx  = 0;
	xm.eDy  = 0;

	Combine(xm);			// rotate
	Translate(x0, y0);		// move origin back

	return TRUE;
}


// Find a transform which maps (0,0) (1,0) (0,1) to p, q, and r respectively
BOOL KAffine::MapTri(FLOAT px0, FLOAT py0, FLOAT qx0, FLOAT qy0, FLOAT rx0, FLOAT ry0)
{
	// px0 = dx, qx0 = m11 + dx, rx0 = m21 + dx
	// py0 = dy, qy0 = m12 + dy, ry0 = m22 + dy
	m_xm.eM11 = qx0 - px0;
	m_xm.eM12 = qy0 - py0;
	m_xm.eM21 = rx0 - px0;
	m_xm.eM22 = ry0 - py0;
	m_xm.eDx  = px0;
	m_xm.eDy  = py0;

	return m_xm.eM11 * m_xm.eM22 != m_xm.eM12 * m_xm.eM21;
}


// Find a transform which maps p0, q0, and r0 to p1, p1 and r1 respectively
BOOL KAffine::MapTri(FLOAT px0, FLOAT py0, FLOAT qx0, FLOAT qy0, FLOAT rx0, FLOAT ry0,
			  	     FLOAT px1, FLOAT py1, FLOAT qx1, FLOAT qy1, FLOAT rx1, FLOAT ry1)
{
	if ( ! MapTri(px0, py0, qx0, qy0, rx0, ry0) )
		return FALSE;

	Invert();		// transform p0, q0, and r0 back to (0,0),(1,0),(0,1)

	KAffine map1;

	if (! map1.MapTri(px1, py1, qx1, qy1, rx1, ry1) )
		return FALSE;

	return Combine(map1.m_xm);	// then to p1,r1,q1
}


// get the combined world to device coordinate space mapping
BOOL KAffine::GetDPtoLP(HDC hDC)
{
	if ( ! GetWorldTransform(hDC, & m_xm) )
		return FALSE;

	POINT origin;
	GetWindowOrgEx(hDC, & origin);
	Translate( - (FLOAT) origin.x, - (FLOAT) origin.y);

	SIZE  sizew, sizev;
	GetWindowExtEx  (hDC, & sizew);
	GetViewportExtEx(hDC, & sizev);
	
	Scale( (FLOAT) sizew.cx/sizev.cx, (FLOAT) sizew.cy/sizev.cy);

	GetViewportOrgEx(hDC, & origin);
	Translate( (FLOAT) origin.x, (FLOAT) origin.y);

	return TRUE;
}


