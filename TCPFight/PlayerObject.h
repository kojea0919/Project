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
	//Damage��Ŷ ���� �� �ʿ�
	//------------------------
	void SetHP(BYTE byHP) { _byHP = byHP; }

	//------------------------
	//�׾����� �Ǵ��� �� �ʿ�
	//------------------------
	DWORD GetHP() { return _byHP; }

	//-----------------------------
	//�ڱ� �ڽ����� �Ǵ����ִ� �Լ�
	//���� �ٸ� ������ ó���� �ٸ��� ���ֱ� ����
	//-----------------------------
	BOOL IsPlayer() { return _bMe; }
	

	//-------------------------
	//dwActionCur �ٲ��ִ� �Լ�
	//-------------------------
	void SetActionStateAttack() { _dwActionCur = eAttackState; }
	void SetActionStateMove() { _dwActionCur = eMoveState; }
	void SetActionStateStand() { _dwActionCur = eStandState; }

	//�����̴� ���� �ٲ��ִ� �Լ�
	void SetMoveDir(BYTE byNewDir) { _byMoveDir = byNewDir; }
	//�ٶ󺸴� ���� �ٲ��ִ� �Լ�
	void SetCurDir(BYTE byNewDir) { _byCurDir = byNewDir; }
	//Damage��Ŷ�� ���� TRUE��
	void SetCollisionCheck(BOOL bCheck) { _bCollisionCheck = bCheck; }
	//Key�Է��̳� Packet�� �޾��� ��
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
	//���� �ൿ�� ���� ����1 or ����2 or LU������ or ....
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	BYTE		_byCurDir;
	BYTE		_byOldDir;
	//���� : 0 || ������ : 4
	//�׸� �׸��� ���� �������� �ٲ������ Ȯ���� ����
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	BYTE		_byMoveDir;
	//������ �ٲ� �Ϳ� ���� ��Ŷ�� ������ �ϴϱ� �ʿ��� ����
	//0���� ������� left,up,right,bottom
	/////////////////////////////////////////////////////////////


	DWORD		_dwObjectID;
	BOOL		_bCollisionCheck;
	DWORD		_dwActionInput;
	//_dwActionInput : Ű���� Msg ��ü�� �����ϴ� ����
};