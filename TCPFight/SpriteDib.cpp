#include "stdafx.h"
#include "define.h"
#include "SpriteDib.h"



CSpriteDib::CSpriteDib(int iMaxSprite, DWORD dwColorKey)
	: _iMaxSprite(iMaxSprite), _dwColorKey(dwColorKey)
{
	_stpSprite = new st_SPRITE[iMaxSprite];
	//스프라이트를 넣을 공간 할당
}


CSpriteDib::~CSpriteDib()
{
	for (int iCnt = 0; iCnt < _iMaxSprite; ++iCnt)
	{
		ReleaseSprite(iCnt);
	}
	delete[] _stpSprite;
}

BOOL CSpriteDib::LoadDibSprite(int iSpriteIndex, wchar_t * szFileName, int iCenterPointX, int iCenterPointY)
{
	st_SPRITE LoadSprite;
	
	FILE * fp;
	_wfopen_s(&fp, szFileName, L"rb");
	if (NULL == fp || iSpriteIndex > _iMaxSprite - 1)
	{
		return FALSE;
	}
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);
	

	//bitmap정보 읽기
	fread(&bmih, sizeof(BITMAPINFOHEADER), 1, fp);
	DWORD dwbitCount = bmih.biBitCount;
	DWORD dwWidth = bmih.biWidth;
	DWORD dwHeight = bmih.biHeight;
	DWORD dwImageSize = (dwWidth * dwHeight) * dwbitCount / 8;


	//Sprite setting
	LoadSprite._bypImage = new BYTE[dwImageSize];
	LoadSprite._iWidth = dwWidth;
	LoadSprite._iHeight = dwHeight;
	LoadSprite._iPitch = dwWidth * 4;//32비트
	LoadSprite._iCenterPointX = iCenterPointX;
	LoadSprite._iCenterPointY = iCenterPointY;

	BYTE* bypTempBuf = new BYTE[dwImageSize];
	//뒤집어져 있는 이미지를 받아올 버퍼

	BYTE* bypDelete = bypTempBuf;

	fread(bypTempBuf, dwImageSize, 1, fp);
	BYTE* bypDestBuf = LoadSprite._bypImage;
	//뒤집어져 있는 이미지를 뒤집어서 넣을 버퍼

	bypTempBuf = bypTempBuf + dwImageSize - dwWidth * 4;
	for (DWORD dwCnt = 0; dwCnt < dwHeight; ++dwCnt)
	{
		memcpy(bypDestBuf, bypTempBuf, dwWidth * 4);
		bypTempBuf -= dwWidth * 4;
		bypDestBuf += dwWidth * 4;
	}

	_stpSprite[iSpriteIndex] = LoadSprite;

	delete[] bypDelete;

	return TRUE;
}

void CSpriteDib::ReleaseSprite(int iSpriteIndex)
{
	if (_iMaxSprite <= iSpriteIndex || iSpriteIndex != Stand_L_MAX || iSpriteIndex != Stand_R_MAX ||
		iSpriteIndex != MOVE_L_MAX || iSpriteIndex != MOVE_R_MAX ||
		iSpriteIndex != Attack1_L_MAX || iSpriteIndex != Attack1_R_MAX ||
		iSpriteIndex != Attack2_L_MAX || iSpriteIndex != Attack2_R_MAX ||
		iSpriteIndex != Attack3_L_MAX || iSpriteIndex != Attack3_R_MAX ||
		iSpriteIndex != xSpark_MAX)
		return;

	if (NULL != _stpSprite[iSpriteIndex]._bypImage)
	{
		delete[] _stpSprite[iSpriteIndex]._bypImage;
		//memset(&_stpSprite[iSpriteIndex], 0, sizeof(st_SPRITE));
	}
}

void CSpriteDib::DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE * bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	//최대 스프라이트 개수를 초과한 경우
	if (iSpriteIndex > _iMaxSprite)
		return;

	st_SPRITE stCurSprite = _stpSprite[iSpriteIndex];

	if (iDrawLen != 100)
		stCurSprite._iWidth = (int)((stCurSprite._iWidth * iDrawLen) / 100.0);

	iDrawX = iDrawX - stCurSprite._iCenterPointX;
	iDrawY = iDrawY - stCurSprite._iCenterPointY;
	//--------------------------------------
	//중점이 iDrawX iDrawY에 오도록 
	//실제로 그리기 시작할 값을 바꿔줘야한다.
	//--------------------------------------


	////////////////////////////////////////////////
	/////////////////////클리핑/////////////////////

	//상단 클리핑
	//위에서 계산한 그리기 시작할 좌표의 Y좌표가 0보다 작은 경우
	//src를 넘어간 만큼 내려줘야 하고 height를 줄여야 한다.
	//그리고 그리기 시작할 Y좌표를 0으로 바꿔준다
	if (iDrawY < 0)
	{
		stCurSprite._bypImage = stCurSprite._bypImage - (iDrawY *stCurSprite._iPitch);
		stCurSprite._iHeight += iDrawY;
		iDrawY = 0;
	}

	//하단 클리핑
	if (iDrawY + stCurSprite._iHeight > iDestHeight)
	{
		stCurSprite._iHeight = iDestHeight - iDrawY;
	}


	if (iDrawX < 0)
	{
		stCurSprite._bypImage -= iDrawX * 4;
		stCurSprite._iWidth += iDrawX;
		iDrawX = 0;
	}

	if (iDrawX + stCurSprite._iWidth > iDestWidth)
	{
		stCurSprite._iWidth = iDestWidth - iDrawX;
	}
	////////////////////클리핑끝////////////////////
	////////////////////////////////////////////////

	DWORD dwSrcWidth = stCurSprite._iWidth;
	DWORD dwSrcHeight = stCurSprite._iHeight;
	DWORD dwSrcPitch = stCurSprite._iPitch;


	//그림을 그릴 부분으로 이동
	bypDest = bypDest + (iDrawX * 4) + (iDrawY * iDestPitch);

	BYTE* bypBaseDestOrg = bypDest;
	BYTE* bypBaseSrcOrg = stCurSprite._bypImage;

	BYTE * BaseDest = bypDest;
	DWORD * ImageSrc = (DWORD*)stCurSprite._bypImage;
	BYTE * BaseSrc = (BYTE*)ImageSrc;
	
	DWORD * pDest = (DWORD*)BaseDest;
	
	//투명색 처리하며 그림 출력
	for (int iCntY = 0; iCntY < dwSrcHeight; ++iCntY)
	{
		for (int iCntX = 0; iCntX < dwSrcWidth; ++iCntX)
		{
			if(*(ImageSrc + iCntX) != 0xffffffff)
				*pDest = *(ImageSrc + iCntX);
			++pDest;
		}
		BaseDest = BaseDest + iDestPitch;
		pDest = (DWORD*)BaseDest;
		BaseSrc = BaseSrc + dwSrcPitch;
		ImageSrc = (DWORD*)BaseSrc;
	}
}

void CSpriteDib::DrawMixSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE * bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	//최대 스프라이트 개수를 초과한 경우
	if (iSpriteIndex > _iMaxSprite)
		return;

	st_SPRITE stCurSprite = _stpSprite[iSpriteIndex];

	iDrawX = iDrawX - stCurSprite._iCenterPointX;
	iDrawY = iDrawY - stCurSprite._iCenterPointY;
	//--------------------------------------
	//중점이 iDrawX iDrawY에 오도록 
	//실제로 그리기 시작할 값을 바꿔줘야한다.
	//--------------------------------------


	////////////////////////////////////////////////
	/////////////////////클리핑/////////////////////

	//상단 클리핑
	//위에서 계산한 그리기 시작할 좌표의 Y좌표가 0보다 작은 경우
	//src를 넘어간 만큼 내려줘야 하고 height를 줄여야 한다.
	//그리고 그리기 시작할 Y좌표를 0으로 바꿔준다
	if (iDrawY < 0)
	{
		stCurSprite._bypImage = stCurSprite._bypImage - (iDrawY *stCurSprite._iPitch);
		stCurSprite._iHeight += iDrawY;
		iDrawY = 0;
	}

	//하단 클리핑
	if (iDrawY + stCurSprite._iHeight > iDestHeight)
	{
		stCurSprite._iHeight = iDestHeight - iDrawY;
	}


	if (iDrawX < 0)
	{
		stCurSprite._bypImage -= iDrawX * 4;
		stCurSprite._iWidth += iDrawX;
		iDrawX = 0;
	}

	if (iDrawX + stCurSprite._iWidth > iDestWidth)
	{
		stCurSprite._iWidth = iDestWidth - iDrawX;
	}
	////////////////////클리핑끝////////////////////
	////////////////////////////////////////////////

	DWORD dwSrcWidth = stCurSprite._iWidth;
	DWORD dwSrcHeight = stCurSprite._iHeight;
	DWORD dwSrcPitch = stCurSprite._iPitch;


	//그림을 그릴 부분으로 이동
	bypDest = bypDest + (iDrawX * 4) + (iDrawY * iDestPitch);

	BYTE* bypBaseDestOrg = bypDest;
	BYTE* bypBaseSrcOrg = stCurSprite._bypImage;

	BYTE * BaseDest = bypDest;
	DWORD * ImageSrc = (DWORD*)stCurSprite._bypImage;
	BYTE * BaseSrc = (BYTE*)ImageSrc;

	DWORD * pDest = (DWORD*)BaseDest;
	DWORD * pSrc = (DWORD*)ImageSrc;

	DWORD dwTempDest;
	DWORD dwTempSrc;

	//투명색 처리하며 그림 출력
	for (int iCntY = 0; iCntY < dwSrcHeight; ++iCntY)
	{
		for (int iCntX = 0; iCntX < dwSrcWidth; ++iCntX)
		{
			if (*(ImageSrc + iCntX) != 0x00ffffff)
			{
				dwTempDest = (*pDest >> 1) & 0x7f7f7f7f;
				dwTempSrc = (*pSrc >> 1) & 0x7f7f7f7f;

				*pDest = dwTempDest + dwTempSrc;


			}
			++pDest;
			++pSrc;
		}
		BaseDest = BaseDest + iDestPitch;
		pDest = (DWORD*)BaseDest;
		BaseSrc = BaseSrc + dwSrcPitch;
		ImageSrc = (DWORD*)BaseSrc;
	}
}

void CSpriteDib::DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE * bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	//최대 스프라이트 개수를 초과한 경우
	if (iSpriteIndex > _iMaxSprite)
		return;

	st_SPRITE stCurSprite = _stpSprite[iSpriteIndex];

	iDrawX = iDrawX - stCurSprite._iCenterPointX;
	iDrawY = iDrawY - stCurSprite._iCenterPointY;
	//--------------------------------------
	//중점이 iDrawX iDrawY에 오도록 
	//실제로 그리기 시작할 값을 바꿔줘야한다.
	//--------------------------------------


	////////////////////////////////////////////////
	/////////////////////클리핑/////////////////////

	//상단 클리핑
	//위에서 계산한 그리기 시작할 좌표의 Y좌표가 0보다 작은 경우
	//src를 넘어간 만큼 내려줘야 하고 height를 줄여야 한다.
	//그리고 그리기 시작할 Y좌표를 0으로 바꿔준다
	if (iDrawY < 0)
	{
		stCurSprite._bypImage = stCurSprite._bypImage - (iDrawY *stCurSprite._iPitch);
		stCurSprite._iHeight += iDrawY;
		iDrawY = 0;
	}

	//하단 클리핑
	if (iDrawY + stCurSprite._iHeight > iDestHeight)
	{
		stCurSprite._iHeight = iDestHeight - iDrawY;
	}


	if (iDrawX < 0)
	{
		stCurSprite._bypImage -= iDrawX * 4;
		stCurSprite._iWidth += iDrawX;
		iDrawX = 0;
	}

	if (iDrawX + stCurSprite._iWidth > iDestWidth)
	{
		stCurSprite._iWidth = iDestWidth - iDrawX;
	}
	////////////////////클리핑끝////////////////////
	////////////////////////////////////////////////

	DWORD dwSrcWidth = stCurSprite._iWidth;
	DWORD dwSrcHeight = stCurSprite._iHeight;
	DWORD dwSrcPitch = stCurSprite._iPitch;


	//그림을 그릴 부분으로 이동
	bypDest = bypDest + (iDrawX * 4) + (iDrawY * iDestPitch);

	BYTE* bypBaseDestOrg = bypDest;
	BYTE* bypBaseSrcOrg = stCurSprite._bypImage;

	//투명색 처리하며 그림 출력
	for (int iCntY = 0; iCntY < dwSrcHeight; ++iCntY)
	{
		memcpy(bypBaseDestOrg, bypBaseSrcOrg, dwSrcPitch);
		bypBaseDestOrg = bypBaseDestOrg + iDestPitch;
		bypBaseSrcOrg = bypBaseSrcOrg + dwSrcPitch;
	}
}

void CSpriteDib::DrawRedSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE * bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	//최대 스프라이트 개수를 초과한 경우
	if (iSpriteIndex > _iMaxSprite)
		return;

	st_SPRITE stCurSprite = _stpSprite[iSpriteIndex];

	iDrawX = iDrawX - stCurSprite._iCenterPointX;
	iDrawY = iDrawY - stCurSprite._iCenterPointY;
	//--------------------------------------
	//중점이 iDrawX iDrawY에 오도록 
	//실제로 그리기 시작할 값을 바꿔줘야한다.
	//--------------------------------------


	////////////////////////////////////////////////
	/////////////////////클리핑/////////////////////

	//상단 클리핑
	//위에서 계산한 그리기 시작할 좌표의 Y좌표가 0보다 작은 경우
	//src를 넘어간 만큼 내려줘야 하고 height를 줄여야 한다.
	//그리고 그리기 시작할 Y좌표를 0으로 바꿔준다
	if (iDrawY < 0)
	{
		stCurSprite._bypImage = stCurSprite._bypImage - (iDrawY *stCurSprite._iPitch);
		stCurSprite._iHeight += iDrawY;
		iDrawY = 0;
	}

	//하단 클리핑
	if (iDrawY + stCurSprite._iHeight > iDestHeight)
	{
		stCurSprite._iHeight = iDestHeight - iDrawY;
	}


	if (iDrawX < 0)
	{
		stCurSprite._bypImage -= iDrawX * 4;
		stCurSprite._iWidth += iDrawX;
		iDrawX = 0;
	}

	if (iDrawX + stCurSprite._iWidth > iDestWidth)
	{
		stCurSprite._iWidth = iDestWidth - iDrawX;
	}
	////////////////////클리핑끝////////////////////
	////////////////////////////////////////////////

	DWORD dwSrcWidth = stCurSprite._iWidth;
	DWORD dwSrcHeight = stCurSprite._iHeight;
	DWORD dwSrcPitch = stCurSprite._iPitch;


	//그림을 그릴 부분으로 이동
	bypDest = bypDest + (iDrawX * 4) + (iDrawY * iDestPitch);

	BYTE* bypBaseDestOrg = bypDest;
	BYTE* bypBaseSrcOrg = stCurSprite._bypImage;

	BYTE * BaseDest = bypDest;
	DWORD * ImageSrc = (DWORD*)stCurSprite._bypImage;
	BYTE * BaseSrc = (BYTE*)ImageSrc;

	DWORD * pDest = (DWORD*)BaseDest;
	DWORD * pSrc = (DWORD*)ImageSrc;

	//투명색 처리하며 그림 출력
	for (int iCntY = 0; iCntY < dwSrcHeight; ++iCntY)
	{
		for (int iCntX = 0; iCntX < dwSrcWidth; ++iCntX)
		{
			if (*(ImageSrc + iCntX) != 0xffffffff)//argb
			{				
				*pDest = (*pSrc) & 0xffff0f0f;
			}
			++pDest;
			++pSrc;
		}
		BaseDest = BaseDest + iDestPitch;
		pDest = (DWORD*)BaseDest;
		BaseSrc = BaseSrc + dwSrcPitch;
		ImageSrc = (DWORD*)BaseSrc;
		pSrc = ImageSrc;
	}
}
