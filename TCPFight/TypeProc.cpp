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
	{//���°� ������? �̻��� ������ �´°� ������
		return FALSE;
	}

	//�ش� Player�� Direction, X, Y ����������
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
	{//���°� ������?
		return FALSE;
	}

	//�ش� Player�� Direction, X, Y ����������
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
	{//���°� ������?
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
	{//���°� ������?
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
	{//���°� ������?
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

	int iIdxOfAttack = GetIdxOfPlayer(dwAttackID);//������ ������ ����Ʈ ����� �۾�
	if (iIdxOfAttack == -1)
	{//���°� ������?
		return FALSE;
	}

	int iIdxOfDamage = GetIdxOfPlayer(dwDamageID);//HP ���̴� �۾�
	if (iIdxOfDamage == -1)
	{//���°� ������?
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

	int iIdxOfDelete = GetIdxOfPlayer(dwID);//������ ������ ����Ʈ ����� �۾�
	if (iIdxOfDelete == -1)
	{//���°� ������?
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
		//�ڱ��ڽ� �����ϱ�
		//-----------------
	case dfPACKET_SC_CREATE_MY_CHARACTER:
		CreateSelf(Packet);
		break;
	
		//-----------------
		//�ٸ���� �����ϱ�
		//-----------------
	case dfPACKET_SC_CREATE_OTHER_CHARACTER:
		CreateOther(Packet);
		break;

		//---------------
		//���� Clientó��
		//---------------
	case dfPACKET_SC_DELETE_CHARACTER:
		DeleteCharacter(Packet);
		break;

		//-----------------
		//�ٸ���� �����̱�
		//-----------------
	case dfPACKET_SC_MOVE_START:
		MoveOther(Packet);
		break;

		//-----------------
		//�ٸ���� ���߱�
		//-----------------
	case dfPACKET_SC_MOVE_STOP:
		StopOther(Packet);
		break;

		//----------------
		//�ٸ���� Attack1
		//----------------
	case dfPACKET_SC_ATTACK1:
		Attack1Other(Packet);
		break;

		//----------------
		//�ٸ���� Attack2
		//----------------
	case dfPACKET_SC_ATTACK2:
		Attack2Other(Packet);
		break;

		//----------------
		//�ٸ���� Attack3
		//----------------
	case dfPACKET_SC_ATTACK3:
		Attack3Other(Packet);
		break;

		//----------------
		//Damage��Ŷ ó��
		//----------------
	case dfPACKET_SC_DAMAGE:
		GiveDamage(Packet);
		break;


		//default ó�� �� �־�� �Ѵ�.
	default:
		//ó���Ҽ� ���� ��Ŷ�̴�. �����ʿ��� 100% 
		//�����Ѵ�. �α׸� ��� ��ġ�� ���ؾ��Ѵ�.
		break;
	}
}