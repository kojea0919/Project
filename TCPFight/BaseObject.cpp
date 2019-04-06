#include "stdafx.h"
#include "define.h"
#include "BaseObject.h"


CBaseObject::CBaseObject(int iCurX, int iCurY, int iObjectType, int iFrameDelay, int iSpriteEnd, int iSpriteStart)
	: _bEndFrame(FALSE), _iCurX(iCurX), _iCurY(iCurY),_iDelayCnt(0),
	_iFrameDelay(iFrameDelay), _iObjectType(iObjectType),
	_iSpriteEnd(iSpriteEnd),_iSpriteNow(iSpriteStart),_iSpriteStart(iSpriteStart)
{
}


CBaseObject::~CBaseObject()
{
}

void CBaseObject::SetSprite(int SpriteEnd, int SpriteStart, int iFrameDelay)
{
	_iSpriteEnd = SpriteEnd;
	_iSpriteNow = SpriteStart;
	_iSpriteStart = SpriteStart;

	_iFrameDelay = iFrameDelay;
	_iDelayCnt = 0;
	_bEndFrame = FALSE;
}

void CBaseObject::NextFrame()
{
	++_iDelayCnt;

	
	//Sprite가 바뀌어야 할 순간
	if (_iDelayCnt >= _iFrameDelay)
	{
		++_iSpriteNow;
		_iDelayCnt = 0;

		if (_iSpriteEnd == _iSpriteNow)
		{
			_iSpriteNow = _iSpriteStart;
			_bEndFrame = TRUE;//하나의 동작 처리 끝남
		}
	}


}
