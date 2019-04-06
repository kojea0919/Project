#pragma once

class CBaseObject
{
public:
	CBaseObject(int iCurX,int iCurY, int iObjectType, int iFrameDelay, int iSpriteEnd, int iSpriteStart);
	~CBaseObject();

public:
	virtual bool run() { return true; }
	virtual void render(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch) {}


	void SetSprite(int SpriteEnd, int SpriteStart, int iFrameDelay);

	int GetObjectType() { return _iObjectType; }

	void NextFrame();

	virtual int* GetRect() { return NULL; }
	
	void SetCurX(int iX) { _iCurX = iX; }
	void SetCurY(int iY) { _iCurY = iY; }
	int GetCurX() { return _iCurX; }
	int GetCurY() { return _iCurY; }

protected:
	BOOL IsEndFrame() { return _bEndFrame; }
	int GetNowSprite() { return _iSpriteNow; }
	int GetEndSprite() { return _iSpriteEnd; }

protected:
	BOOL		_bEndFrame;
	//_bEndFrame ; 해당 행동의 Sprite의 끝에 도달하면 TRUE


	int			_iCurX;
	int			_iCurY;
	//Object의 현재 좌표

	int			_iDelayCnt;
	//해당 Sprite의 애니메이션 프레임 딜레이 만큼 
	//멈추기 위한 카운팅 변수
	int			_iFrameDelay;
	//해당 Sprite의 애니메이션 프레임 딜레이


	int			_iObjectType;

	int			_iSpriteEnd;
	int			_iSpriteNow;
	int			_iSpriteStart;
	//Sprite의 마지막 Index
	//Sprite의 현재   Index
	//Sprite의 시작	  Index

	friend void ObjSort();
};