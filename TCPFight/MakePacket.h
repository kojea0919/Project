#pragma once

//Packet����� �ִ� �Լ��� ���Ӽ��� �ִ��� �ٿ����Ѵ�.

//--------------------------------------
//stPACKET_CS_MOVE_START Packet���� �Լ�
//--------------------------------------
void mpMoveStart(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

//--------------------------------------
//stPACKET_CS_MOVE_STOP Packet���� �Լ�
//--------------------------------------
void mpMoveStop(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

//--------------------------------------
//stPACKET_SC_ATTACK1 Packet���� �Լ�
//--------------------------------------,
void mpAttack1(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

//--------------------------------------
//stPACKET_SC_ATTACK2 Packet���� �Լ�
//--------------------------------------,
void mpAttack2(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

//--------------------------------------
//stPACKET_SC_ATTACK3 Packet���� �Լ�
//--------------------------------------,
void mpAttack3(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

