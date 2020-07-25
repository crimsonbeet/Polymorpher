// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include "stdafx.h"

#include "Dib.h"



const char * mess_DIBFormat[] =
{
	"DIB_1BPP",			//   2 color image, palette-based
	"DIB_2BPP",			//   4 color image, palttte-based
	"DIB_4BPP",			//  16 color image, palette-based
	"DIB_4BPPRLE",		//  16 color image, palette-based, RLE compressed
	"DIB_8BPP",			// 256 color image, palette-based 
	"DIB_8BPPRLE",		// 256 color image, palette-based, RLE compressed

	"DIB_16RGB555",		// 15 bit RGB color image, 5-5-5
	"DIB_16RGB565",		// 16 bit RGB color image, 5-6-5, 1 bit unused
	"DIB_24RGB888",		// 24 bit RGB color image, 8-8-8
	"DIB_32RGB888",		// 32 bit RGB color image, 8-8-8, 8 bit unused

	"DIB_32RGBA8888",	// 32 bit RGBA color image, 8-8-8-8

	"DIB_16RGBbitmsk",	// 16 bit RGB color image, non-standard bit masks, NT-only
	"DIB_32RGBbitmsk",	// 32 bit RGB color image, non-standard bit masks, NT-only

	"DIB_JPEG",			// embedded JPEG image
	"DIB_PNG"			// embedded PNG image
};

const char * PixelFormatName(int id) { 
	return (id >= DIB_1BPP) && (id <= DIB_PNG)? mess_DIBFormat[id - DIB_1BPP]: "Unknown Pixel Format";
}


int GetDIBPixelSize(const BITMAPINFOHEADER& bmih) { 
	return bmih.biSizeImage? bmih.biSizeImage: ((bmih.biWidth * bmih.biBitCount + 31) / 32 * 4 * bmih.biPlanes * abs(bmih.biHeight));
}

int GetDIBColorCount(const BITMAPINFOHEADER& bmih) {
	if(bmih.biBitCount <= 8) { 
		if(bmih.biClrUsed) { 
			return bmih.biClrUsed; 
		} 
		else { 
			return 1 << bmih.biBitCount;  
		} 
	} 
	else 
	if(bmih.biCompression == BI_BITFIELDS) { 
		return 3 + bmih.biClrUsed; 
	} 

	return bmih.biClrUsed;
}

int PixelFormat(HDC hdc) { 
	typedef struct
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD	 	     bmiColors[256+3];
	}	DIBINFO;

	DIBINFO dibinfo;

	HBITMAP hBmp = CreateCompatibleBitmap(hdc, 1, 1);
	if(!hBmp) { 
        return -1; 
	} 

	memset(&dibinfo, 0, sizeof(dibinfo));

    dibinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

    // 1st call to get hdc biBitCount.
    GetDIBits(hdc, hBmp, 0, 1, NULL, (BITMAPINFO*) & dibinfo, DIB_RGB_COLORS);
       
    // 2nd calls to get color table or bitfields
    GetDIBits(hdc, hBmp, 0, 1, NULL, (BITMAPINFO*) & dibinfo, DIB_RGB_COLORS);
       
    DeleteObject(hBmp);

	// try to understand bit fields
	if(dibinfo.bmiHeader.biBitCount == BI_BITFIELDS) { 
		DWORD * pBitFields = (DWORD *) dibinfo.bmiColors;
		DWORD red   = pBitFields[0];
		DWORD green = pBitFields[1];
		DWORD blue  = pBitFields[2];

		if((blue == 0x001F) && (green == 0x03E0) && (red == 0x7C00))
			return DIB_16RGB555;
		else if((blue == 0x001F) && (green == 0x007E) && (red == 0xF800))
			return DIB_16RGB565;
		else if((blue == 0x00FF) && (green == 0xFF00) && (red == 0xFF0000))
			return DIB_32RGB888;
		else
			return -1;
	}

	switch(dibinfo.bmiHeader.biBitCount) {
		case 1:	return DIB_1BPP;
		case 2: return DIB_2BPP;
		case 4:	return DIB_4BPP;
		case 8: return DIB_8BPP;
		case 24: return DIB_24RGB888;
		case 16: return DIB_16RGB555;
		case 32: return DIB_32RGB888;
	}
	return -1;
}


HBITMAP CaptureWindow(HWND hWnd) {
	RECT wnd;

//	if(!GetWindowRect(hWnd, &wnd)) { 
	if(!GetClientRect(hWnd, &wnd)) { 
		return NULL; 
	} 

	HDC hDC = GetWindowDC(hWnd);

	HBITMAP hBmp = CreateCompatibleBitmap(hDC, wnd.right - wnd.left, wnd.bottom - wnd.top);
	if(hBmp) {
		HDC hMemDC = CreateCompatibleDC(hDC);
		HGDIOBJ hOld = SelectObject(hMemDC, hBmp);

		BitBlt(hMemDC, 0, 0, wnd.right - wnd.left, wnd.bottom - wnd.top, hDC, 0, 0, SRCCOPY); 

		SelectObject(hMemDC, hOld);
		DeleteObject(hMemDC);
	}
	
	ReleaseDC(hWnd, hDC);

	return hBmp;
}


BOOL EXP_KWIN SaveDIBToFile(const char * pFileName, const BITMAPINFO * pBMI, const void * pBits) { 
	HANDLE handle = CreateFile(pFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
	if(handle == INVALID_HANDLE_VALUE) { 
		return FALSE; 
	} 

	BITMAPFILEHEADER bmFH;

	int nHeadSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetDIBColorCount(pBMI->bmiHeader); 

	bmFH.bfType      = 0x4D42;
	bmFH.bfSize      = nHeadSize + GetDIBPixelSize(pBMI->bmiHeader);
	bmFH.bfReserved1 = 0;
	bmFH.bfReserved2 = 0;
	bmFH.bfOffBits   = nHeadSize + sizeof(BITMAPFILEHEADER);

	DWORD dw = 0;
	WriteFile(handle, &bmFH, sizeof(bmFH), &dw, NULL);

	if(pBits == NULL) { // packed DIB
		pBits = (PBYTE)pBMI + nHeadSize; 
	} 
	
	WriteFile(handle, pBMI, nHeadSize, &dw, NULL); 
	WriteFile(handle, pBits, GetDIBPixelSize(pBMI->bmiHeader), &dw, NULL); 

	CloseHandle(handle);
	return TRUE;
}

void EXP_KWIN SaveWindowToDIB(const char * pFileName, HWND hWnd, int nBitCount, int nCompression) {
	HBITMAP hbmp = CaptureWindow(hWnd);
	if(hbmp) {
		PBITMAPINFO pbminfo = BitmapToDIB(NULL, hbmp, nBitCount, nCompression); 
		if(pbminfo) { 
			SaveDIBToFile(pFileName, pbminfo, NULL);
			delete (PBYTE)pbminfo;
		}
		DeleteObject(hbmp);
	}
}


PBITMAPINFO BitmapToDIB(HPALETTE hPal, HBITMAP hBmp, int nBitCount, int nCompression) { 
//	HPALETTE hPal palette - for color conversion
//	HBITMAP hBmp - DDB for convert
//	int nBitCount - format wanted
//	int nCompression - format wanted

	typedef struct
	{
		BITMAPINFOHEADER bmiHeader; 
		RGBQUAD bmiColors[256+3]; 
	} DIBINFO;

	BITMAP ddbinfo;
	DIBINFO dibinfo;

	// retrieve DDB information 
	if(GetObject(hBmp, sizeof(BITMAP), &ddbinfo) == 0) { 
		return NULL; 
	} 

	memset(&dibinfo, 0, sizeof(dibinfo)); 

//	fill out BITMAPINFOHEADER based on size and required format 

	dibinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	dibinfo.bmiHeader.biWidth = ddbinfo.bmWidth;
	dibinfo.bmiHeader.biHeight = ddbinfo.bmHeight;
	dibinfo.bmiHeader.biPlanes = 1;
	dibinfo.bmiHeader.biBitCount = nBitCount > 0? nBitCount: ddbinfo.bmBitsPixel; 
	dibinfo.bmiHeader.biCompression = nCompression;

	HDC hdc = GetDC(NULL); // screen DC 

	HGDIOBJ hpalold = hPal? SelectPalette(hdc, hPal, FALSE): 0;
	
//	query GDI for image size. 
//	GetDIBits function retrieves the bits of the DDB and copies them into a buffer as a DIB using the specified format. 

	GetDIBits(hdc, hBmp, 0/*uStartScan*/, ddbinfo.bmHeight/*cScanLines*/, /*lpvBits*/NULL, (BITMAPINFO *)&dibinfo/*in/out*/, DIB_RGB_COLORS); 

	int ninfosize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetDIBColorCount(dibinfo.bmiHeader);
	int ntotalsize = ninfosize + GetDIBPixelSize(dibinfo.bmiHeader);

	PBYTE pbminfo = new BYTE[ntotalsize];

	if(pbminfo) {
		memcpy(pbminfo, &dibinfo, ninfosize); 
		if(ddbinfo.bmHeight != GetDIBits(hdc, hBmp, 0, ddbinfo.bmHeight, pbminfo + ninfosize, (BITMAPINFO *) pbminfo, DIB_RGB_COLORS)) {
			delete pbminfo;
			pbminfo = 0;
		}
	}

	if(hpalold) { 
		SelectObject(hdc, hpalold); 
	} 

	ReleaseDC(NULL, hdc);

	return (PBITMAPINFO)pbminfo;
}



template <class MAPFunction> bool KDIBColorTransform(KDIB& kdib, MAPFunction& map) { // single pixel transform. 

//	OS/2 DIB color table: 1-bpp, 4-bpp, 8-bpp, include RLE compression. 
	if(kdib._prgbTriple) { 	
		for(int i = 0; i < kdib._clrUsed; ++i) { 
			map(kdib._prgbTriple[i].rgbtRed, kdib._prgbTriple[i].rgbtGreen, kdib._prgbTriple[i].rgbtBlue); 
		} 
		return true;
	} 

//	Windows DIB color table: 1-bpp, 2-bpp, 4-bpp, 8-bpp, include RLE compression. 
	if(kdib._prgbQuad) { 
		for(int i = 0; i < kdib._clrUsed; ++i) { 
			map(kdib._prgbQuad[i].rgbRed, kdib._prgbQuad[i].rgbGreen, kdib._prgbQuad[i].rgbBlue); 
		} 

		return true;
	} 

//	No color table. Pixel represent RGB color image. 
	for(int y = 0; y < kdib._height; ++y) { 
		int width = kdib._width;
		PBYTE pBuffer = (PBYTE)kdib._pbmBits + kdib._bytsPscline * y;
		
		switch(kdib._imageFormat) { 
			case DIB_16RGB555: 	// 15 bit RGB color image, 5-5-5, 1 bit unused
				for(; width > 0; width--) { 
					BYTE red   = ( (* (WORD *) pBuffer) & 0x7C00 ) >> 7;
					BYTE green = ( (* (WORD *) pBuffer) & 0x03E0 ) >> 2;
					BYTE blue  = ( (* (WORD *) pBuffer) & 0x001F ) << 3;
					
					map(red, green, blue);
					
					*(WORD*)pBuffer = ( ( red >> 3 ) << 10 ) | ( ( green >> 3 ) << 5 ) | ( blue >> 3 );
					
					pBuffer += 2;
				}
				break;

			case DIB_16RGB565:	// 16 bit RGB color image, 5-6-5
				for(; width > 0; width--) { 
					BYTE red   = ( (* (WORD *) pBuffer) & 0xF800 ) >> 8;
					BYTE green = ( (* (WORD *) pBuffer) & 0x07E0 ) >> 3;
					BYTE blue  = ( (* (WORD *) pBuffer) & 0x001F ) << 3;
					
					map(red, green, blue);
					
					*(WORD*)pBuffer = ( ( red >> 3 ) << 11 ) | ( ( green >> 2 ) << 5 ) | ( blue >> 3 );
					
					pBuffer += 2;
				}
				break;

			case DIB_24RGB888:	// 24-bpp RGB
				for(; width > 0; width--) { 
					map(pBuffer[2], pBuffer[1], pBuffer[0]); 
					pBuffer += 3;
				}
				break;

			case DIB_32RGBA8888: // 32-bpp RGBA
			case DIB_32RGB888:   // 32-bpp RGB
				for(; width > 0; width--) { 
					map(pBuffer[2], pBuffer[1], pBuffer[0]);
					pBuffer += 4;
				}
				break;

			default:
				return false;
		} 
	} 
	
	return true;
}


// 0.299 * red + 0.587 * green + 0.114 * blue 
inline void Map2Gray(BYTE& red, BYTE& green, BYTE& blue) {
	blue = green = red = (red * 77 + green * 150 + blue * 29 + 128) / 256;
}




bool EXP_KWIN KDIBGrayColorTransform(KDIB& kdib) { 
	return KDIBColorTransform(kdib, Map2Gray); 
} 


bool EXP_KWIN KDIBGammaCorrectTransform(KDIB& kdib, KDIBGammaCorrect& gammaCorrect) { 
	return KDIBColorTransform(kdib, gammaCorrect); 
} 





bool EXP_KWIN KDIBSplitChannel(const KDIB& kdib, BYTE (*map)(BYTE, BYTE, BYTE), BITMAPINFO **p) { 
	if(!p) { 
		return false; 
	} 

	*p = 0; 

//	Target number of bytes per scan line. 

	int bytsPscline = (kdib._width + 3) / 4 * 4; // 8-bpp scan-line size. 

//	create target BITMAPINFO. 

	int headsize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);

	PBITMAPINFO pbmInfo = (PBITMAPINFO)new BYTE[headsize + bytsPscline * abs(kdib._height)]; 
	if(!pbmInfo) { 
		return false; 
	} 

	bool rc = true; 

	memset(pbmInfo, 0, headsize);

	pbmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmInfo->bmiHeader.biWidth = kdib._width; 
    pbmInfo->bmiHeader.biHeight = kdib._height; 
    pbmInfo->bmiHeader.biPlanes	= 1; 
    pbmInfo->bmiHeader.biBitCount = 8; 
    pbmInfo->bmiHeader.biCompression = BI_RGB; 

//	Target gray color table. 

	for(int c = 0; c < 256; ++c) { 
		pbmInfo->bmiColors[c].rgbRed = c;
		pbmInfo->bmiColors[c].rgbGreen = c;
		pbmInfo->bmiColors[c].rgbBlue = c;
	}

//	Target pixels. 

	PBYTE pbmBits  = (PBYTE)&pbmInfo->bmiColors[256]; 

//	Set up the pointer to color table of original bitmap for 1-bpp, 4-bpp, 8-bpp. 
//	Zero if no original color table. 

	const PBYTE pColors = kdib._prgbTriple? (PBYTE)kdib._prgbTriple: kdib._prgbQuad? (PBYTE)kdib._prgbQuad: 0; 
	const int nColorsize = kdib._prgbTriple? 3: kdib._prgbQuad? 4: 0; 

//	For each scan line. 
	for(int y = 0; y < kdib._height; ++y) { 
		int width = kdib._width;

		PBYTE pBuffer = kdib._pbmBits + kdib._bytsPscline * y; 
		PBYTE pPixel = pbmBits + bytsPscline * y; 
		
		switch(kdib._imageFormat) { 
			case DIB_1BPP:
				for(int mask = 0x80, shift = 7; width > 0; width--) { 
					int index = ((pBuffer[0] & mask) >> shift) & 0x1; 
					index *= nColorsize; 

					*pPixel++ = map(pColors[index + 2]/*red*/, pColors[index + 1]/*green*/, pColors[index + 0]/*blue*/); 

					mask >>= 1;
					shift -= 1;

					if(mask == 0) { 
						++pBuffer; mask = 0x80; shift = 7; 
					}
				}
				break;

			case DIB_2BPP:
				for(int mask = 0xC0, shift = 6; width > 0; width--) { 
					int index = ((pBuffer[0] & mask) >> shift) & 0x3; 
					index *= nColorsize; 

					*pPixel++ = map(pColors[index + 2]/*red*/, pColors[index + 1]/*green*/, pColors[index + 0]/*blue*/); 

					mask >>= 2;
					shift -= 2;

					if(mask == 0) { 
						++pBuffer; mask = 0xC0; shift = 6; 
					} 
				} 
				break; 

			case DIB_4BPP: 
				for(int mask = 0xF0, shift = 4; width > 0; width--) { 
					int index = ((pBuffer[0] & mask) >> shift) & 0xF; 
					index *= nColorsize; 

					*pPixel++ = map(pColors[index + 2]/*red*/, pColors[index + 1]/*green*/, pColors[index + 0]/*blue*/); 

					mask = ~mask; 
					shift = 4 - shift; 

					if(mask == 0xF0) { 
						++pBuffer; 
					} 
				}
				break;

			case DIB_8BPP:
				for(; width > 0; width--) { 
					int index = pBuffer[0]; 
					index *= nColorsize; 

					*pPixel++ = map(pColors[index + 2]/*red*/, pColors[index + 1]/*green*/, pColors[index + 0]/*blue*/); 

					++pBuffer; 
				}
				break;

			case DIB_16RGB555: 	// 15 bit RGB color image, 5-5-5, 1 bit unused
				for(; width > 0; width--) { 
					BYTE red   = ( (* (WORD *) pBuffer) & 0x7C00 ) >> 7;
					BYTE green = ( (* (WORD *) pBuffer) & 0x03E0 ) >> 2;
					BYTE blue  = ( (* (WORD *) pBuffer) & 0x001F ) << 3;
					
					*pPixel++ = map(red, green, blue);
					
					pBuffer += 2;
				}
				break;

			case DIB_16RGB565:	// 16 bit RGB color image, 5-6-5
				for(; width > 0; width--) { 
					const DWORD StandardMask565[] =  { 0x00F800, 0x0007E0, 0x00001F };

					BYTE red   = ( (* (WORD *) pBuffer) & 0xF800 ) >> 8;
					BYTE green = ( (* (WORD *) pBuffer) & 0x07E0 ) >> 3;
					BYTE blue  = ( (* (WORD *) pBuffer) & 0x001F ) << 3;
					
					*pPixel++ = map(red, green, blue);
					
					pBuffer += 2;
				}
				break;

			case DIB_24RGB888:	// 24-bpp RGB
				for(; width > 0; width--) { 
					*pPixel++ = map(pBuffer[2], pBuffer[1], pBuffer[0]); 

					pBuffer += 3;
				}
				break;

			case DIB_32RGBA8888: // 32-bpp RGBA
			case DIB_32RGB888:   // 32-bpp RGB
				for(; width > 0; width--) { 
					*pPixel++ = map(pBuffer[2], pBuffer[1], pBuffer[0]); 

					pBuffer += 4;
				}
				break;

			default:
				rc = false;
				break; 
		} 
	} 

	if(rc) { 
		*p = pbmInfo; 
	} 
	else { 
		delete [] pbmInfo; 
	} 
	
	return true;
}


BYTE EXP_KWIN KDIBTakeRed(BYTE red, BYTE green, BYTE blue) { 
	return red;
}

BYTE EXP_KWIN KDIBTakeGreen(BYTE red, BYTE green, BYTE blue) { 
	return green; 
} 

BYTE EXP_KWIN KDIBTakeBlue(BYTE red, BYTE green, BYTE blue) { 
	return blue; 
} 

// KCMY channel splitting operators 
BYTE EXP_KWIN KDIBTakeK(BYTE red, BYTE green, BYTE blue) { 
//	min(255-red, 255-green, 255-blue) 
	if(red < green) { 
		return green < blue? (255 - blue): (255 - green); 
	} 

	return (255 - red); 
} 

BYTE EXP_KWIN KDIBTakeC(BYTE red, BYTE green, BYTE blue) { 
	return 255 - red; 
} 

BYTE EXP_KWIN KDIBTakeM(BYTE red, BYTE green, BYTE blue) { 
	return 255 - green; 
} 

BYTE EXP_KWIN KDIBTakeY(BYTE red, BYTE green, BYTE blue) { 
	return 255 - blue; 
} 

// HLS channel splitting operators 
BYTE EXP_KWIN KDIBTakeH(BYTE red, BYTE green, BYTE blue) { 
	KColor color(red, green, blue); 
	color.ToHLS(); 

	return (BYTE)(color.hue * 255 / 360); 
} 

BYTE EXP_KWIN KDIBTakeL(BYTE red, BYTE green, BYTE blue) { 
	KColor color(red, green, blue); 
	color.ToHLS(); 

	return (BYTE)(color.lightness * 255); 
} 

BYTE EXP_KWIN KDIBTakeS(BYTE red, BYTE green, BYTE blue) { 
	KColor color(red, green, blue); 
	color.ToHLS(); 

	return (BYTE)(color.saturation * 255); 
} 





// Calculate DIB pixel offset
inline int GetOffset(BITMAPINFO * pBMI, int x, int y) {
	if(pBMI->bmiHeader.biHeight > 0) { // for bottom up, reflect y
		y = pBMI->bmiHeader.biHeight - 1 - y; 
	} 

	int rv = (pBMI->bmiHeader.biWidth * pBMI->bmiHeader.biBitCount + 31) / 32 * 4 * y; 
	rv += (pBMI->bmiHeader.biBitCount / 8) * x; 

	return rv; 
}




// Alpha Blending between two 32-bpp DIBs
BOOL DIB2DIBAlphaBlend3232(
BITMAPINFO * pBMIDst, BYTE * pBitsDst, int dx, int dy, int w, int h, 
BITMAPINFO * pBMISrc, BYTE * pBitsSrc, int sx, int sy, BLENDFUNCTION blend) { 

	BOOL ok = TRUE; 

	if(pBMIDst->bmiHeader.biBitCount != 32) ok = FALSE; 
	if(pBMISrc->bmiHeader.biBitCount != 32) ok = FALSE; 

	if(pBMIDst->bmiHeader.biCompression != BI_RGB) ok = FALSE; 
	if(pBMISrc->bmiHeader.biCompression != BI_RGB) ok = FALSE; 

	if(!ok) { 
		return FALSE; 
	} 

	ok = FALSE; 

	int format = blend.AlphaFormat == 0? 0: blend.SourceConstantAlpha == 255? 1: 2; 

	PBYTE pLastDst = pBitsDst + 4 * pBMIDst->bmiHeader.biWidth * pBMIDst->bmiHeader.biHeight; 
	PBYTE pLastSrc = pBitsSrc + 4 * pBMISrc->bmiHeader.biWidth * pBMISrc->bmiHeader.biHeight; 


	int alpha = blend.SourceConstantAlpha; // constant alpha
	int beta = 255 - alpha; // constant beta 


	for(int j = 0; j < h; ++j) { 
		BYTE * D = pBitsDst + GetOffset(pBMIDst, dx, j + dy);
		BYTE * S = pBitsSrc + GetOffset(pBMISrc, sx, j + sy);
		
		int i;

		if(pBitsDst < pLastDst && pBitsSrc < pLastSrc) 
		switch(format) { 
			case 0: // constant alpha only (use blend.SourceConstantAlpha) 
				for(i = 0; i < w; ++i) { 
					D[0] = (S[0] * alpha + beta * D[0] + 127) / 255; 
					D[1] = (S[1] * alpha + beta * D[1] + 127) / 255; 
					D[2] = (S[2] * alpha + beta * D[2] + 127) / 255; 
					D[3] = (S[3] * alpha + beta * D[3] + 127) / 255; 
					D += 4; 
					S += 4;
				} 
				ok = TRUE; 
				break;

			case 1: // alpha channel only (blend.AlphaFormat == AC_SRC_ALPHA and blend.SourceConstantAlpha == 255) 
				for(i = 0; i < w; ++i) { 
					beta = 255 - S[3];
					D[0] = S[0] + (beta * D[0] + 127) / 255;
					D[1] = S[1] + (beta * D[1] + 127) / 255;
					D[2] = S[2] + (beta * D[2] + 127) / 255;
					D[3] = S[3] + (beta * D[3] + 127) / 255;
					D += 4; 
					S += 4;
				}
				ok = TRUE; 
				break;
			
			case 2: // both constant alpha and alpha channel
				for(i = 0; i < w; ++i) { 
					beta = 255 - (S[3] * alpha + 127) / 255; 
					
					D[0] = (S[0] * alpha + beta * D[0] + 127) / 255;
					D[1] = (S[1] * alpha + beta * D[1] + 127) / 255;
					D[2] = (S[2] * alpha + beta * D[2] + 127) / 255;
					D[3] = (S[3] * alpha + beta * D[3] + 127) / 255;
					D += 4; 
					S += 4;
				}
				ok = TRUE; 
				break;
		}
	}

	return ok; 
}


// AlphaBlending a 32-bpp DIB to hdc
BOOL DIB2HDCAlphaBlend(HDC hdc, int x0, int y0, const BITMAPINFO *pbmpInfo, const void * pAlphaImageData) { 
	if(pbmpInfo->bmiHeader.biBitCount != 32) { 
		return FALSE; 
	} 

	if(pbmpInfo->bmiHeader.biCompression != BI_RGB) { 
		return FALSE; 
	} 

	int width = pbmpInfo->bmiHeader.biWidth; 
	int height = pbmpInfo->bmiHeader.biHeight; 

	BITMAPINFOHEADER bmih = { sizeof(BITMAPINFOHEADER), width, height, 1, 24 };
	PVOID pBits; 

	HBITMAP hbmp = CreateDIBSection(NULL, (BITMAPINFO*)&bmih, DIB_RGB_COLORS, &pBits, NULL, NULL); 

	if(hbmp == NULL) { 
		return FALSE; 
	} 

	HDC hmemdc = CreateCompatibleDC(hdc); 
	HGDIOBJ holdbmp = SelectObject(hmemdc, hbmp); 

	BitBlt(hmemdc, 0, 0, width, height, hdc, x0, y0, SRCCOPY); // copy destination, not working on printer 

	const BYTE *psrc = (const BYTE*)pAlphaImageData; 
	PBYTE pdst = (PBYTE)pBits;

	for(int y = 0; y < abs(height); ++y) { 
		for(int x = width; x > 0; --x) { 
			BYTE alpha  = psrc[3]; //Get alpha channel byte.
			BYTE salpha = 255 - alpha;

			pdst[0] = (BYTE)((psrc[0] * alpha + pdst[0] * salpha) / 255); 
			pdst[1] = (BYTE)((psrc[1] * alpha + pdst[1] * salpha) / 255); 
			pdst[2] = (BYTE)((psrc[2] * alpha + pdst[2] * salpha) / 255); 

			pdst += 3;
			psrc += 4;
		}

		pdst += width % 3;
	}

	BOOL rslt = BitBlt(hdc, x0, y0, width, height, hmemdc, 0, 0, SRCCOPY); 

	SelectObject(hmemdc, holdbmp); 
	DeleteObject(hbmp); 
	DeleteObject(hmemdc); 

	return rslt;
}

void minmax(int x0, int x1, int x2, int x3, int &minx, int &maxx) { 
	if(x0 < x1) { 
		minx = x0; maxx = x1;	
	} 
	else { 
		minx = x1; maxx = x0;
	} 

	if(x2 < minx) { 
		minx = x2; 
	} 
	else 
	if(x2 > maxx) { 
		maxx = x2; 
	} 

	if(x3 < minx) { 
		minx = x3; 
	} 
	else 
	if(x3 > maxx) { 
		maxx = x3; 
	} 
}
