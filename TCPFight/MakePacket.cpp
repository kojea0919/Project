#include "stdafx.h"
#include "NetworkProc.h"
#include "PacketDefine.h"
#include "CPacket.h"
#include "MakePacket.h"



void mpMoveStart(CPacket * pHeader, CPacket * pPacket, BYTE byDir, WORD wX, WORD wY)
{
	//PayLoad
	*pPacket << byDir << wX << wY;
	/*pPacket->_byDirection = byDir;
	pPacket->_wX = wX;
	pPacket->_wY = wY;*/


	//Header
	*pHeader << (BYTE)dfNETWORK_PAKCET_CODE << (BYTE)pPacket->GetPacketSize() << (BYTE)dfPACKET_CS_MOVE_START << (BYTE)0;
	/*pHeader->_byCode = dfNETWORK_PAKCET_CODE;
	pHeader->_bySize = sizeof(stPACKET_CS_MOVE_START);
	pHeader->_byType = dfPACKET_CS_MOVE_START;*/
}

void mpMoveStop(CPacket * pHeader, CPacket * pPacket, BYTE byDir, WORD wX, WORD wY)
{
	*pPacket << byDir << wX << wY;
	//pPacket->_byDirection = byDir;
	//pPacket->_wX = wX;
	//pPacket->_wY = wY;

	*pHeader << (BYTE)dfNETWORK_PAKCET_CODE << (BYTE)pPacket->GetPacketSize() << (BYTE)dfPACKET_CS_MOVE_STOP << (BYTE)0;
	/*pHeader->_byCode = dfNETWORK_PAKCET_CODE;
	pHeader->_bySize = sizeof(stPACKET_CS_MOVE_STOP);
	pHeader->_byType = dfPACKET_CS_MOVE_STOP;*/
}


void mpAttack1(CPacket * pHeader, CPacket * pPacket, BYTE byDir, WORD wX, WORD wY)
{
	*pPacket << byDir << wX << wY;
	/*pPacket->_byDirection = byDir;
	pPacket->_wX = wX;
	pPacket->_wY = wY;*/

	*pHeader << (BYTE)dfNETWORK_PAKCET_CODE << (BYTE)pPacket->GetPacketSize() << (BYTE)dfPACKET_CS_ATTACK1 << (BYTE)0;

	/*pHeader->_byCode = dfNETWORK_PAKCET_CODE;
	pHeader->_bySize = sizeof(stPACKET_CS_ATTACK1);
	pHeader->_byType = dfPACKET_CS_ATTACK1;*/
}

void mpAttack2(CPacket * pHeader, CPacket * pPacket, BYTE byDir, WORD wX, WORD wY)
{
	*pPacket << byDir << wX << wY;
	/*pPacket->_byDirection = byDir;
	pPacket->_wX = wX;
	pPacket->_wY = wY;*/

	*pHeader << (BYTE)dfNETWORK_PAKCET_CODE << (BYTE)pPacket->GetPacketSize() << (BYTE)dfPACKET_CS_ATTACK2 << (BYTE)0;

	/*pHeader->_byCode = dfNETWORK_PAKCET_CODE;
	pHeader->_bySize = sizeof(stPACKET_CS_ATTACK1);
	pHeader->_byType = dfPACKET_CS_ATTACK1;*/
}

void mpAttack3(CPacket * pHeader, CPacket * pPacket, BYTE byDir, WORD wX, WORD wY)
{
	*pPacket << byDir << wX << wY;
	/*pPacket->_byDirection = byDir;
	pPacket->_wX = wX;
	pPacket->_wY = wY;*/

	*pHeader << (BYTE)dfNETWORK_PAKCET_CODE << (BYTE)pPacket->GetPacketSize() << (BYTE)dfPACKET_CS_ATTACK3 << (BYTE)0;

	/*pHeader->_byCode = dfNETWORK_PAKCET_CODE;
	pHeader->_bySize = sizeof(stPACKET_CS_ATTACK1);
	pHeader->_byType = dfPACKET_CS_ATTACK1;*/
}
