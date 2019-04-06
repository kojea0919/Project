#pragma once
class CPlayerObject : public CBaseObject
{
public:
	CPlayerObject(int iCurX, int iCurY, BYTE byHP, DWORD ID,BYTE byDir, BOOL bMe = FALSE);
	~CPlayerObject();

public:
	virtual bool run();
	virtual void render(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);
	void ActionProc();
	void InputProc();
	void CheckCollision();

	//------------------------
	//Damage패킷 왔을 때 필요
	//------------------------
	void SetHP(BYTE byHP) { _byHP = byHP; }

	//------------------------
	//죽었는지 판단할 때 필요
	//------------------------
	DWORD GetHP() { return _byHP; }

	//-----------------------------
	//자기 자신인지 판단해주는 함수
	//나와 다른 유저의 처리를 다르게 해주기 위함
	//-----------------------------
	BOOL IsPlayer() { return _bMe; }
	

	//-------------------------
	//dwActionCur 바꿔주는 함수
	//-------------------------
	void SetActionStateAttack() { _dwActionCur = eAttackState; }
	void SetActionStateMove() { _dwActionCur = eMoveState; }
	void SetActionStateStand() { _dwActionCur = eStandState; }

	//움직이는 방향 바꿔주는 함수
	void SetMoveDir(BYTE byNewDir) { _byMoveDir = byNewDir; }
	//바라보는 방향 바꿔주는 함수
	void SetCurDir(BYTE byNewDir) { _byCurDir = byNewDir; }
	//Damage패킷이 오면 TRUE로
	void SetCollisionCheck(BOOL bCheck) { _bCollisionCheck = bCheck; }
	//Key입력이나 Packet을 받았을 떄
	void ActionInput(DWORD Action) { _dwActionInput = Action; }


	DWORD GetActionState() { return _dwActionCur; }
	BYTE GetMoveDir() { return _byMoveDir; }
	BYTE GetCurDir() { return _byCurDir; }
	int GetObjectID() { return _dwObjectID; }


private:
	BOOL		_bMe;
	BYTE		_byHP;

	/////////////////////////////////////////////////////////////
	DWORD		_dwActionCur;
	//현재 행동의 종류 공격1 or 공격2 or LU움직임 or ....
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	BYTE		_byCurDir;
	BYTE		_byOldDir;
	//왼쪽 : 0 || 오른쪽 : 4
	//그림 그릴때 왼족 오른쪽이 바뀌었는지 확인할 변수
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	BYTE		_byMoveDir;
	//방향이 바뀐 것에 따라 패킷을 보내야 하니까 필요한 변수
	//0부터 순서대로 left,up,right,bottom
	/////////////////////////////////////////////////////////////


	DWORD		_dwObjectID;
	BOOL		_bCollisionCheck;
	DWORD		_dwActionInput;
	//_dwActionInput : 키보드 Msg 객체에 전달하는 변수
};