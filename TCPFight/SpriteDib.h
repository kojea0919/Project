#pragma once
class CSpriteDib
{
public:
	//-------------------------
	//DIB 스프라이트 구조체
	//-------------------------
	typedef struct st_SPRITE
	{
		BYTE*	_bypImage;
		DWORD	_iWidth;
		DWORD	_iHeight;
		DWORD	_iPitch;

		DWORD	_iCenterPointX;
		DWORD	_iCenterPointY;
		//Image에서 중점 x,y좌표
	};

public:
	CSpriteDib(int iMaxSprite,DWORD dwColorKey);
	//---------------------------------------------------
	//iMaxSprite : 불러올 파일 개수(미리 공간할당을 위해)
	//dwColorKey : 투명처리할 색(0xffffffff)
	//---------------------------------------------------
	virtual ~CSpriteDib();


	//----------------------------------------
	//LoadDibSprite함수
	//bmp파일을 읽어서 하나의 프레임으로 저장
	//----------------------------------------
	BOOL LoadDibSprite(int iSpriteIndex, wchar_t *szFileName, int iCenterPointX, int iCenterPointY);

	//----------------------------------------
	//ReleaseSprite함수
	//해당 스프라이트 해제
	//----------------------------------------
	void ReleaseSprite(int iSpriteIndex);

	//------------------------------------------------------------------
	//DrawSprite
	//해당 메모리 위치에 투명색처리, 클리핑처리 후 스프라이트를 출력한다. 
	//------------------------------------------------------------------
	void DrawSprite(int iSpriteIndex,int iDrawX, int iDrawY, BYTE *bypDest,int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);
	//iSpriteIndex : 몇번 째 Sprite를 출력할지
	//iDrawX	   : 그리고자 하는 메모리 x위치
	//iDrawY	   : 그리고자 하는 메모리 y위치
	//bypDest	   : 백버퍼의 시작 위치
	//iDestWidth   : 백버퍼의 Width
	//iDestHeight  : 백버퍼의 height
	//iDestPitch   : 백버퍼의 Pitch
	//iDrawlen	   : 그릴 부분의 Width를 설정하기 위한 값


	//------------------------------------------------------------------
	//DrawMixSprite
	//만약 해당 부분에 색이 칠해져있으면 반투명으로 보이도록 출력한다.
	//------------------------------------------------------------------
	void DrawMixSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch);

	//------------------------------------------------------------------
	//DrawImage
	//줄단위 복사가 이루어 지는 Map Sprite를 처리해주기 위한 함수
	//------------------------------------------------------------------
	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE * bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);

	//------------------------------------------------------------------
	//DrawRedSprite
	//Sprtie를 빨간색 부분을 강조해서 출력해주기 위한 함수
	//------------------------------------------------------------------
	void DrawRedSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch);


protected:
	int			_iMaxSprite;
	st_SPRITE * _stpSprite;

	DWORD		_dwColorKey;
};

