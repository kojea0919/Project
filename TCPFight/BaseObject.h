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
	//_bEndFrame ; �ش� �ൿ�� Sprite�� ���� �����ϸ� TRUE


	int			_iCurX;
	int			_iCurY;
	//Object�� ���� ��ǥ

	int			_iDelayCnt;
	//�ش� Sprite�� �ִϸ��̼� ������ ������ ��ŭ 
	//���߱� ���� ī���� ����
	int			_iFrameDelay;
	//�ش� Sprite�� �ִϸ��̼� ������ ������


	int			_iObjectType;

	int			_iSpriteEnd;
	int			_iSpriteNow;
	int			_iSpriteStart;
	//Sprite�� ������ Index
	//Sprite�� ����   Index
	//Sprite�� ����	  Index

	friend void ObjSort();
};