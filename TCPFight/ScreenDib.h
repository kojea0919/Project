#pragma once


class CScreenDib
{
public:
	CScreenDib(DWORD iWidth, DWORD iHeight, DWORD iColorBit);
	virtual ~CScreenDib();

protected:
	void CreateDibBuffer(DWORD iWidth, DWORD iHeight, DWORD iColorBit);
	//-----------------------------------------
	//해당 인자값을 가지는 DibBuffer를 생성한다
	//-----------------------------------------

	void ReleaseDibBuffer(void);
	//-----------------
	//메모리 공간 해제
	//-----------------

public:
	void DrawBuffer(HWND hWnd, DWORD iX = 0, DWORD iY = 0);

	BYTE* GetDibBuffer(void) { return _bypBuffer; }
	DWORD GetWidth() { return _iWidth; }
	DWORD GetHeight() { return _iHeight; }
	DWORD GetPitch() { return _iPitch; }


protected:
	BITMAPINFO		_stDibInfo;
	BYTE*			_bypBuffer;

	DWORD			_iWidth;
	DWORD			_iHeight;
	DWORD			_iPitch;
	DWORD			_iColorBit;
	DWORD			_iBufferSize;
};

