#include "stdafx.h"

#include "RingBuffer.h"
#include "define.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"
#include "PacketDefine.h"
#include "NetworkProc.h"

#include "WinScreenDib.h"
#include "CPacket.h"
#include "TypeProc.h"

extern CRingBuffer RecvQ;
extern vector<CBaseObject*> g_ObjVec;
extern int iPlayerCnt;
extern BOOL g_bGenSelf;
extern HWND g_hWnd;
extern CPlayerObject * g_cPlayer;
extern int iPlayerCnt;

int GetIdxOfPlayer(DWORD dwID)
{
	for (int iCnt = 0; iCnt < g_ObjVec.size(); ++iCnt)
	{
		if (((CPlayerObject*)g_ObjVec[iCnt])->GetObjectID() == dwID)
			return iCnt;
	}
	return -1;
}

BOOL CreateSelf(CPacket & Packet)
{
	DWORD	dwID;
	BYTE	byDirection;
	WORD	wX;
	WORD	wY;
	BYTE	byHP;

	Packet >> dwID >> byDirection >> wX >> wY >> byHP;

	g_cPlayer = new CPlayerObject(wX, wY, byHP
		, dwID,byDirection, TRUE);
	g_ObjVec.push_back(g_cPlayer);

	++iPlayerCnt;
	g_bGenSelf = TRUE;


	return TRUE;
}

BOOL CreateOther(CPacket & Packet)
{
	CPlayerObject * cOtherPlayer;

	DWORD	dwID;
	BYTE	byDirection;
	WORD	wX;
	WORD	wY;
	BYTE	byHP;

	Packet >> dwID >> byDirection >> wX >> wY >> byHP;

	cOtherPlayer = new CPlayerObject(wX, wY,
		byHP, dwID, byDirection);
	g_ObjVec.push_back(cOtherPlayer);
	++iPlayerCnt;

	return TRUE;
}

BOOL MoveOther(CPacket & Packet)
{
	DWORD	dwID;
	BYTE	byDirection;
	WORD	wX;
	WORD	wY;

	Packet >> dwID >> byDirection >> wX >> wY;

	int iIdxOfFind = GetIdxOfPlayer(dwID);
	if (iIdxOfFind == -1)
	{//끊는게 맞을까? 이상한 순간은 맞는거 같은데
		return FALSE;
	}

	//해당 Player의 Direction, X, Y 설정해주자
	CPlayerObject * TargetPlayer = (CPlayerObject*)g_ObjVec[iIdxOfFind];

	TargetPlayer->ActionInput(byDirection);
	//zTargetPlayer->SetActionStateMove();
	TargetPlayer->SetCurX(wX);
	TargetPlayer->SetCurY(wY);


	return TRUE;
}


BOOL StopOther(CPacket & Packet)
{
	DWORD	dwID;
	BYTE	byDirection;
	WORD	wX;
	WORD	wY;

	Packet >> dwID >> byDirection >> wX >> wY;

	int iIdxOfFind = GetIdxOfPlayer(dwID);
	if (iIdxOfFind == -1)
	{//끊는게 맞을까?
		return FALSE;
	}

	//해당 Player의 Direction, X, Y 설정해주자
	CPlayerObject * TargetPlayer = (CPlayerObject*)g_ObjVec[iIdxOfFind];
	
	TargetPlayer->ActionInput(-1);
	TargetPlayer->SetCurDir(byDirection);
	TargetPlayer->SetCurX(wX);
	TargetPlayer->SetCurY(wY);


	return TRUE;
}

BOOL Attack1Other(CPacket & Packet)
{
	DWORD	dwID;
	BYTE	byDirection;
	WORD	wX;
	WORD	wY;

	Packet >> dwID >> byDirection >> wX >> wY;

	int iIdxOfFind = GetIdxOfPlayer(dwID);
	if (iIdxOfFind == -1)
	{//끊는게 맞을까?
		return FALSE;
	}

	CPlayerObject * TargetObj = (CPlayerObject*)g_ObjVec[iIdxOfFind];
	TargetObj->ActionInput(eATTACK1);
	TargetObj->SetCurX(wX);
	TargetObj->SetCurY(wY);

	return TRUE;
}


BOOL Attack2Other(CPacket & Packet)
{
	DWORD	dwID;
	BYTE	byDirection;
	WORD	wX;
	WORD	wY;

	Packet >> dwID >> byDirection >> wX >> wY;

	int iIdxOfFind = GetIdxOfPlayer(dwID);
	if (iIdxOfFind == -1)
	{//끊는게 맞을까?
		return FALSE;
	}

	CPlayerObject * TargetObj = (CPlayerObject*)g_ObjVec[iIdxOfFind];
	TargetObj->ActionInput(eATTACK2);
	TargetObj->SetCurX(wX);
	TargetObj->SetCurY(wY);

	return TRUE;
}


BOOL Attack3Other(CPacket & Packet)
{
	DWORD	dwID;
	BYTE	byDirection;
	WORD	wX;
	WORD	wY;

	Packet >> dwID >> byDirection >> wX >> wY;

	int iIdxOfFind = GetIdxOfPlayer(dwID);
	if (iIdxOfFind == -1)
	{//끊는게 맞을까?
		return FALSE;
	}

	CPlayerObject * TargetObj = (CPlayerObject*)g_ObjVec[iIdxOfFind];
	TargetObj->ActionInput(eATTACK3);
	TargetObj->SetCurX(wX);
	TargetObj->SetCurY(wY);

	return TRUE;
}

BOOL GiveDamage(CPacket & Packet)
{
	DWORD	dwAttackID;
	DWORD	dwDamageID;
	BYTE	byDamageHP;

	Packet >> dwAttackID >> dwDamageID >> byDamageHP;

	int iIdxOfAttack = GetIdxOfPlayer(dwAttackID);//적당한 시점에 이펙트 만드는 작업
	if (iIdxOfAttack == -1)
	{//끊는게 맞을까?
		return FALSE;
	}

	int iIdxOfDamage = GetIdxOfPlayer(dwDamageID);//HP 줄이는 작업
	if (iIdxOfDamage == -1)
	{//끊는게 맞을까?
		return FALSE;
	}

	CPlayerObject * DamageObject = (CPlayerObject*)g_ObjVec[iIdxOfDamage];
	DamageObject->SetHP(byDamageHP);

	CPlayerObject * AttackObject = (CPlayerObject*)g_ObjVec[iIdxOfAttack];

	AttackObject->SetCollisionCheck(TRUE);
	return TRUE;
}

BOOL DeleteCharacter(CPacket & Packet)
{
	DWORD dwID;

	Packet >> dwID;

	int iIdxOfDelete = GetIdxOfPlayer(dwID);//적당한 시점에 이펙트 만드는 작업
	if (iIdxOfDelete == -1)
	{//끊는게 맞을까?
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return FALSE;
	}

	g_ObjVec.erase(g_ObjVec.begin() + iIdxOfDelete);
	--iPlayerCnt;
}


void TypeProc(BYTE byType, CPacket & Packet)
{
	switch (byType)
	{
		//-----------------
		//자기자신 생성하기
		//-----------------
	case dfPACKET_SC_CREATE_MY_CHARACTER:
		CreateSelf(Packet);
		break;
	
		//-----------------
		//다른사람 생성하기
		//-----------------
	case dfPACKET_SC_CREATE_OTHER_CHARACTER:
		CreateOther(Packet);
		break;

		//---------------
		//죽은 Client처리
		//---------------
	case dfPACKET_SC_DELETE_CHARACTER:
		DeleteCharacter(Packet);
		break;

		//-----------------
		//다른사람 움직이기
		//-----------------
	case dfPACKET_SC_MOVE_START:
		MoveOther(Packet);
		break;

		//-----------------
		//다른사람 멈추기
		//-----------------
	case dfPACKET_SC_MOVE_STOP:
		StopOther(Packet);
		break;

		//----------------
		//다른사람 Attack1
		//----------------
	case dfPACKET_SC_ATTACK1:
		Attack1Other(Packet);
		break;

		//----------------
		//다른사람 Attack2
		//----------------
	case dfPACKET_SC_ATTACK2:
		Attack2Other(Packet);
		break;

		//----------------
		//다른사람 Attack3
		//----------------
	case dfPACKET_SC_ATTACK3:
		Attack3Other(Packet);
		break;

		//----------------
		//Damage패킷 처리
		//----------------
	case dfPACKET_SC_DAMAGE:
		GiveDamage(Packet);
		break;


		//default 처리 꼭 넣어야 한다.
	default:
		//처리할수 없는 패킷이다. 서버쪽에는 100% 
		//들어가야한다. 로그를 찍던 조치를 취해야한다.
		break;
	}
}