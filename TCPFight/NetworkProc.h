#pragma once

BOOL NetworkProc(LPARAM lParam);
//------------------------------------------------
//WM_SOCKET �޽��� �߻��� NetworkProc�Լ����� ó��
//------------------------------------------------

BOOL RecvProc();
//FD_READ�߻� ó��

BOOL SendProc();
//FD_WRITE�߻� ó��

//Key�Է¿� ���� ���� ��Ŷ
void SendMoveStartPacket();
void SendMoveStopPacket();
void SendAttack1Packet();
void SendAttack2Packet();
void SendAttack3Packet();
///////////////////////////////////////////////////////////////////////////////////
//HEADER : byCode(0x89)|bySize(���̷ε� ������)|byType(��Ŷ Ÿ��)|byTemp(padding)//
///////////////////////////////////////////////////////////////////////////////////


struct st_HEADER
{
	BYTE	_byCode;
	BYTE	_bySize;
	BYTE	_byType;
	BYTE	_byTemp;
};


#pragma pack(1)
struct stPACKET_SC_CREATE_MY_CHARACTER
{
	DWORD	_dwID;
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
	BYTE	_byHP;
};

struct stPACKET_SC_CREATE_OTHER_CHARACTER
{
	DWORD	_dwID;
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
	BYTE	_byHP;
};

struct stPACKET_SC_DELETE_CHARACTER
{
	DWORD _dwID;
};

struct stPACKET_CS_MOVE_START
{
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_SC_MOVE_START
{
	DWORD	_dwID;
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_CS_MOVE_STOP
{
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_SC_MOVE_STOP
{
	DWORD	_dwID;
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_CS_ATTACK1
{
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_SC_ATTACK1
{
	DWORD	_dwID;
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_CS_ATTACK2
{
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_SC_ATTACK2
{
	DWORD	_dwID;
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_CS_ATTACK3
{
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_SC_ATTACK3
{
	DWORD	_dwID;
	BYTE	_byDirection;
	WORD	_wX;
	WORD	_wY;
};

struct stPACKET_SC_DAMAGE
{
	DWORD	_dwAttackID;//������
	DWORD	_dwDamageID;//������
	BYTE	_byDamageHP;
};

#pragma pack()