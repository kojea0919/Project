#include "stdafx.h"
#include "SpriteDib.h"
#include "define.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"

extern CPlayerObject* g_cPlayer;
extern CSpriteDib g_cSpriteDib;
extern vector<CBaseObject*> g_ObjVec;
extern int iPlayerCnt;

//int IsIn(int iX, int iY)
//{
//	for (int iCnt = 0; iCnt < iPlayerCnt; ++iCnt)
//	{
//		int * _pRect = g_ObjVec[iCnt]->GetRect();
//		//left,up,right,down
//		if (_pRect[0] < iX && iX < _pRect[2] && _pRect[1] < iY && iY < _pRect[3])
//			return iCnt;
//	}
//	return -1;
//}




CPlayerObject::CPlayerObject(int iCurX, int iCurY, BYTE byHP, DWORD ID, BYTE byDir, BOOL bMe)
	: CBaseObject(iCurX, iCurY, eTYPE_PLAYER, dfDELAY_STAND, Stand_R_MAX, Stand_R_01),
	_bMe(bMe), _dwActionCur(eStandState), _byMoveDir(byDir), _byHP(byHP), _dwObjectID(ID)
	, _byCurDir(byDir), _bCollisionCheck(FALSE)
{
	if (byDir == 0)
	{
		SetSprite(Stand_L_MAX, Stand_L_01, dfDELAY_STAND);
	}
	else
	{
		SetSprite(Stand_R_MAX, Stand_R_01, dfDELAY_STAND);
	}

}


CPlayerObject::~CPlayerObject()
{
}

bool CPlayerObject::run()
{
	CheckCollision();
	NextFrame();//Sprite처리
	ActionProc();//키보드입력 처리


	return true;
}

void CPlayerObject::render(BYTE *bypDest,int iDestWidth,int iDestHeight,int iDestPitch)
{
	if (_bMe)
	{
		g_cSpriteDib.DrawRedSprite(_iSpriteNow, _iCurX, _iCurY, bypDest,
			iDestWidth, iDestHeight, iDestPitch);
	}
	else
	{
		g_cSpriteDib.DrawSprite(_iSpriteNow, _iCurX, _iCurY, bypDest,
			iDestWidth, iDestHeight, iDestPitch);
	}

	g_cSpriteDib.DrawMixSprite(Shadow, _iCurX, _iCurY, bypDest,
		iDestWidth, iDestHeight, iDestPitch);

	g_cSpriteDib.DrawSprite(HPGuage, _iCurX - 35, _iCurY + 9, bypDest,
		iDestWidth, iDestHeight, iDestPitch, _byHP);
}

void CPlayerObject::ActionProc()
{
	if (!_bMe)
	{
		//내가 아닌 다른 유저는 공격 중에 이동 패킷을 받아서
		//이동 처리를 해야한다면 공격 모션이 끝나지 않았더라도
		//움직임 처리를 해줘야 한다.
		if (IsEndFrame() && _dwActionCur == eAttackState)
		{
			_dwActionInput = -1;
		}

		InputProc();
		return;
	}

	switch (_dwActionCur)
	{
	case eAttackState:

		if (IsEndFrame())
		{
			SetActionStateStand();
			//----------------------------------
			//_dwActionCur를 e_Stand로 바꿔준다.
			//----------------------------------

			if (_byCurDir == 0)
				SetSprite(Stand_L_MAX, Stand_L_01, dfDELAY_STAND);
			else if (_byCurDir != 0)
				SetSprite(Stand_R_MAX, Stand_R_01, dfDELAY_STAND);
			return;
		}
		break;
	default:
		InputProc();
		break;
	}
}

void CPlayerObject::InputProc()
{
	switch (_dwActionInput)
	{
	case eATTACK1:
		if (_dwActionCur != eAttackState)
		{
			_dwActionCur = eAttackState;
			if (_byCurDir == 0)
				SetSprite(Attack1_L_MAX, Attack1_L_01, dfDELAY_ATTACK1);
			else if (_byCurDir != 0)
				SetSprite(Attack1_R_MAX, Attack1_R_01, dfDELAY_ATTACK1);
		}
		return;
	case eATTACK2:
		if (_dwActionCur != eAttackState)
		{
			_dwActionCur = eAttackState;
			if (_byCurDir == 0)
				SetSprite(Attack2_L_MAX, Attack2_L_01, dfDELAY_ATTACK2);
			else if (_byCurDir != 0)
				SetSprite(Attack2_R_MAX, Attack2_R_01, dfDELAY_ATTACK2);
		}
		return;
	case eATTACK3:
		if (_dwActionCur != eAttackState)
		{
			_dwActionCur = eAttackState;
			if (_byCurDir == 0)
				SetSprite(Attack3_L_MAX, Attack3_L_01, dfDELAY_ATTACK3);
			else if (_byCurDir != 0)
				SetSprite(Attack3_R_MAX, Attack3_R_01, dfDELAY_ATTACK3);
		}
		return;
	case eACTION_MOVE_LL:
		if (_iCurX - 3 <= dfRANGE_MOVE_LEFT)
			break;
		_iCurX -= 3;
		_byCurDir = 0;
		break;
	case eACTION_MOVE_LU:
		if (_iCurX - 3 <= dfRANGE_MOVE_LEFT || _iCurY - 2 <= dfRANGE_MOVE_TOP)
			break;
		_iCurX -= 3;
		_iCurY -= 2;
		_byCurDir = 0;
		break;
	case eACTION_MOVE_UU:
		if (_iCurY - 2 <= dfRANGE_MOVE_TOP)
			break;
		_iCurY -= 2;
		break;
	case eACTION_MOVE_RU:
		if (_iCurX + 3 >= dfRANGE_MOVE_RIGHT || _iCurY - 2 <= dfRANGE_MOVE_TOP)
			break;
		_iCurX += 3;
		_iCurY -= 2;
		_byCurDir = 4;
		break;
	case eACTION_MOVE_RR:
		if (_iCurX + 3 >= dfRANGE_MOVE_RIGHT)
			break;
		_iCurX += 3;
		_byCurDir = 4;
		break;
	case eACTION_MOVE_RD:
		if (_iCurX + 3 >= dfRANGE_MOVE_RIGHT || _iCurY + 2 >= dfRANGE_MOVE_BOTTOM)
			break;
		_iCurX += 3;
		_iCurY += 2;
		_byCurDir = 4;
		break;
	case eACTION_MOVE_DD:
		if (_iCurY + 2 >= dfRANGE_MOVE_BOTTOM)
			break;
		_iCurY += 2;
		break;
	case eACTION_MOVE_LD:
		if (_iCurX - 3 <= dfRANGE_MOVE_LEFT || _iCurY + 2 >= dfRANGE_MOVE_BOTTOM)
			break;
		_iCurX -= 3;
		_iCurY += 2;
		_byCurDir = 0;
		break;
	default:
		//-------------------------------------
		//가만히 서있는게 되는 상태에 대한 처리
		//-------------------------------------
		if (_dwActionCur != eStandState)
		{
			_dwActionCur = eStandState;
			if (_byCurDir == 0)
				SetSprite(Stand_L_MAX, Stand_L_01, dfDELAY_STAND);
			else if (_byCurDir !=0)
				SetSprite(Stand_R_MAX, Stand_R_01, dfDELAY_STAND);
		}
		return;
	}

	//--------------------------------------------
	//처음 움직임을 시작할 때와 방향이 바뀌었을 때
	//--------------------------------------------
	if (_dwActionCur != eMoveState || _byCurDir != _byOldDir)
	{
		if (_byCurDir == 0)//왼쪽
			SetSprite(MOVE_L_MAX, MOVE_L_01, dfDELAY_MOVE);
		else if (_byCurDir != 0)//오른쪽
			SetSprite(MOVE_R_MAX, MOVE_R_01, dfDELAY_MOVE);
	}


	//-------------------------------------------
	//움죽이는 Action일 경우만 여기까지 내려온다.
	//-------------------------------------------
	_dwActionCur = eMoveState;
	_byOldDir = _byCurDir;


	///////////////////////////////////////////////////////////////////
}

void CPlayerObject::CheckCollision()
{
		if ((_iSpriteNow == Attack3_L_02 || _iSpriteNow == Attack3_L_03 ||
			_iSpriteNow == Attack3_R_02 || _iSpriteNow == Attack3_R_03 ||
			_iSpriteNow == Attack2_R_01 || _iSpriteNow == Attack1_R_01 ||
			_iSpriteNow == Attack2_L_01 || _iSpriteNow == Attack1_L_01) && _bCollisionCheck)
		{
			int iDrawY = _iCurY - 90;
			CEffectObject * cEffect;
			if (_byCurDir == 0)
				cEffect = new CEffectObject(_iCurX - 60, iDrawY + 25);
			else
				cEffect = new CEffectObject(_iCurX + 60, iDrawY + 25);
			g_ObjVec.push_back(cEffect);
			_bCollisionCheck = FALSE;
		}
}
