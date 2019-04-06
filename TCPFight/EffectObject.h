#pragma once
class CEffectObject : public CBaseObject
{
public:
	CEffectObject(int iCurX, int iCurY);
	~CEffectObject();

public:
	virtual bool run();
	virtual void render(BYTE * bypDest, int iDestWidth, int iDestHeight,  int iDestPitch);

private:
	/*BOOL	_bEffectStart;
	DWORD	_dwAttackID;*/
};

