#pragma once

//Packet만들어 주는 함수들 종속성을 최대한 줄여야한다.

//--------------------------------------
//stPACKET_CS_MOVE_START Packet생성 함수
//--------------------------------------
void mpMoveStart(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

//--------------------------------------
//stPACKET_CS_MOVE_STOP Packet생성 함수
//--------------------------------------
void mpMoveStop(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

//--------------------------------------
//stPACKET_SC_ATTACK1 Packet생성 함수
//--------------------------------------,
void mpAttack1(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

//--------------------------------------
//stPACKET_SC_ATTACK2 Packet생성 함수
//--------------------------------------,
void mpAttack2(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

//--------------------------------------
//stPACKET_SC_ATTACK3 Packet생성 함수
//--------------------------------------,
void mpAttack3(CPacket * pHeader, CPacket * pPacket, BYTE byDir,
	WORD wX, WORD wY);

