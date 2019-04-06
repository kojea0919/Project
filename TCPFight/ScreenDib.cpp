#include "stdafx.h"
#include "ScreenDib.h"


CScreenDib::CScreenDib(DWORD iWidth, DWORD iHeight, DWORD iColorBit)
	: _iWidth(iWidth), _iHeight(iHeight), _iColorBit(iColorBit)
{
	_iPitch = (_iWidth * _iColorBit) / 8 + 3 & ~3;
	_iBufferSize = _iPitch * _iHeight;

	CreateDibBuffer(_iWidth, _iHeight, iColorBit);
}


CScreenDib::~CScreenDib()
{
	ReleaseDibBuffer();
}

void CScreenDib::CreateDibBuffer(DWORD iWidth, DWORD iHeight, DWORD iColorBit)
{
	if (iColorBit > 8)
	{
		BITMAPINFOHEADER bmih;
		bmih.biSize = sizeof(BITMAPINFOHEADER);
		bmih.biWidth = iWidth;
		bmih.biHeight = -1*iHeight;
		bmih.biPlanes = 1;
		bmih.biBitCount = iColorBit;
		bmih.biCompression = 0;
		bmih.biSizeImage = _iBufferSize;
		bmih.biXPelsPerMeter = 0;
		bmih.biYPelsPerMeter = 0;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;

		_stDibInfo.bmiHeader = bmih;

		_bypBuffer = new BYTE[_iBufferSize];
	}
}


void CScreenDib::ReleaseDibBuffer()
{
	delete[] _bypBuffer;
}


void CScreenDib::DrawBuffer(HWND hWnd, DWORD iX, DWORD iY)
{
	HDC hdc = GetDC(hWnd);
	StretchDIBits(hdc, iX, iY, _iWidth, _iHeight, 0, 0, _iWidth, _iHeight,
		_bypBuffer, &_stDibInfo, DIB_RGB_COLORS, SRCCOPY);

	ReleaseDC(hWnd, hdc);
}