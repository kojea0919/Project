#pragma once

#define dfHEADER_SIZE 4
//패킷 헤더 크기

#define dfNETWORK_PAKCET_CODE ((BYTE)0x89)
//byCode에 들어갈 코드

#define dfNETWORK_PAKCET_END ((BYTE)0x79)
//EndCode에 들어갈 코드

#define dfPACKET_SC_CREATE_MY_CHARACTER		0
//Server가 Client가 접속했을 때 처음으로 보내는 패킷으로
//ID, Direction, X, Y, HP 정보를 Setting한다.

#define dfPACKET_SC_CREATE_OTHER_CHARACTER	1
//Server에 다른 Client가 접속했을 때 보내주는 패킷으로
//위와 마찬가지 정보를 Setting한다.

#define dfPACKET_SC_DELETE_CHARACTER		2
//Server에서 어떤 Client가 죽었을 때 보내주는 패킷으로
//ID값만 넣어준다.

#define dfPACKET_CS_MOVE_START				10
//Client에서 방향키가 눌려서 움직이기 시작할 때 Server에게
//보내주는 패킷으로 Direction, X, Y 정보를 Setting한다.

#define dfPACKET_SC_MOVE_START				11
//Server가 어떤 Client에게 MOVE_START패킷을 받으면 다른
//Client들에게도 이 패킷을 보내준다.

#define dfPACKET_CS_MOVE_STOP				12
//Client에서 방향키를 때서 움직임을 멈출 때 Server에게
//보내주는 패킷으로 Direction, X ,Y 정보를 Setting한다.

#define dfPACKET_SC_MOVE_STOP				13
//Server가 Client에게 MOVE_STOP패킷을 받으면 다른
//Client들에게도 이 패킷을 보내준다.

#define dfPACKET_CS_ATTACK1					20
//Client가 공격키를 입력하면 보내주는 패킷으로
//Direction, X, Y 정보를 Setting한다.

#define dfPACKET_SC_ATTACK1					21
//Server가 보내주는 패킷으로 이 패킷을 받고 공격 동작을 취한다.
//Direction, X, Y 정보를 Setting한다.

#define dfPACKET_CS_ATTACK2					22

#define dfPACKET_SC_ATTACK2					23

#define dfPACKET_CS_ATTACK3					24

#define dfPACKET_SC_ATTACK3					25

#define dfPACKET_SC_DAMAGE					30
//Server가 보내주는 패킷으로 이 패킷을 받으면 내 공격이
//누군가에게 맞았다는 의미고 공격자ID,피해자ID,피해자HP를
//Setting한다.