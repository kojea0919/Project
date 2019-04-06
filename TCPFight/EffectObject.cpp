#include "stdafx.h"
#include "SpriteDib.h"
#include "define.h"
#include "BaseObject.h"
#include "EffectObject.h"

 
extern CSpriteDib g_cSpriteDib;
extern vector<CBaseObject*> g_ObjVec;

CEffectObject::CEffectObject(int iCurX,int iCurY)
	: CBaseObject(iCurX,iCurY, eTYPE_EFFECT, dfDELAY_EFFECT,xSpark_MAX, xSpark_1)
{
}


CEffectObject::~CEffectObject()
{
}

bool CEffectObject::run()
{
	NextFrame();

	//effect�� sprite�� ������ �Ҹ��Ѵ�.
	if (_bEndFrame)
	{
		//vector���� ����� �۾�
		//erase
		//g_ObjVec.pop_back();
		return false;
	}
}

void CEffectObject::render(BYTE * bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	g_cSpriteDib.DrawSprite(_iSpriteNow, _iCurX, _iCurY, bypDest,
		iDestWidth, iDestHeight, iDestPitch);
}
