#include "stdafx.h"

#include <winsock2.h>
#include <ws2tcpip.h>


#include "WinScreenDib.h"
#include "NetworkProc.h"
#include "PacketDefine.h"
#include "RingBuffer.h"

#include "SpriteDib.h"
#include "define.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"
#include "CPacket.h"
#include "TypeProc.h"
#include "MakePacket.h"

extern BOOL		g_bConnect;
extern BOOL		g_bGenSelf;
extern SOCKET	g_Sock;
extern HWND		g_hWnd;
extern BOOL		g_bSendFlag;
extern CPlayerObject * g_cPlayer;
extern vector<CBaseObject*> g_ObjVec;
extern int iPlayerCnt;

CRingBuffer RecvQ;
CRingBuffer SendQ;

	
	
BOOL NetworkProc(LPARAM lParam)
{
	//FALSE를 Return시 프로그램 종료 방향으로
	

	if (WSAGETSELECTERROR(lParam))
	{
		//WSAGETSELECTERROR
		return FALSE;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		g_bConnect = TRUE;
		return TRUE;

	case FD_READ:
		if(!RecvProc())
			return FALSE;
		return TRUE;

	case FD_WRITE:
		g_bSendFlag = TRUE;
		if (!SendProc())
			return FALSE;

		return TRUE;

	case FD_CLOSE:
		return FALSE;
	}
}

BOOL RecvProc()
{
	if (!g_bConnect)
	{
		return TRUE;
	}
	//서버에서 오는 패킷을 받자
	
	int iRetVal;
	//recv함수로 RingBuffer에 받기
	iRetVal = recv(g_Sock, RecvQ.GetRecvPosOfBuf(), RecvQ.NonSplitRecvSize(), 0);

	//--------------------------------
	//에러처리
	//WOULDBLOCK이면 다음번에서 해결
	//아니면 종료
	//--------------------------------
	if (iRetVal == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
			return FALSE;
		}
		return TRUE;
	}

	//recv로 받은만큼 Rear를 이동시키기
	RecvQ.MoveRear(iRetVal);

	//헤더크기보다 저장한 데이터 크기가 큰동안 반복
	while (RecvQ.inUseSize() > dfHEADER_SIZE)
	{
		CPacket Header;
		//헤더 Peek
		iRetVal = RecvQ.Peek(Header.GetBufferPtr(), dfHEADER_SIZE);

		//에러처리
		if (iRetVal != dfHEADER_SIZE)
		{
			SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
			return FALSE;
		}

		Header.MoveWritePos(iRetVal);

		BYTE byCode, bySize, byType, byTemp;
		Header >> byCode >> bySize >> byType >> byTemp;
		//Code Check
		//이상한 패킷이면? 그냥 Dequeue해 버리고 다음 패킷을 처리하자?
		BYTE byPayLoadSize = bySize;//END까지
		if (byCode != dfNETWORK_PAKCET_CODE)
		{
			RecvQ.MoveFront(dfHEADER_SIZE + byPayLoadSize + 1);
			continue;
		}


		//PayLoad Len체크
		//RingBuffer가 가지고 있는지 확인
		//안가지고 있으면 다음번에 처리
		if (byPayLoadSize + dfHEADER_SIZE + 1 > RecvQ.inUseSize())//HEADER + PAYLOAD + END
		{
			return TRUE;
		}

		//가지고 있으면

		//HEADER는 만큼 제거후
		RecvQ.MoveFront(dfHEADER_SIZE);
		
		//직렬화 버퍼 준비
		CPacket PayLoad;
		
		//PayLoad뽑기
		iRetVal = RecvQ.Dequeue(PayLoad.GetBufferPtr(), byPayLoadSize);
		if (iRetVal != byPayLoadSize)
		{
			return FALSE;
		}
		
		PayLoad.MoveWritePos(iRetVal);

		//EndCode뽑기
		BYTE EndBuf;
		iRetVal = RecvQ.Dequeue((char*)&EndBuf, sizeof(EndBuf));
		if (iRetVal != sizeof(EndBuf))
		{
			return FALSE;
		}

		if (EndBuf != dfNETWORK_PAKCET_END)
		{
			continue;
		}

		//Proc호출
		TypeProc(byType, PayLoad);
	}
	return TRUE;
}

BOOL SendProc()
{
	if (!g_bConnect)
	{
		return TRUE;
	}

	int iRetVal;
	iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(), 0);
	if (iRetVal == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
			return FALSE;
		}
		g_bSendFlag = FALSE;
		return TRUE;
	}

	SendQ.MoveFront(iRetVal);

	if (SendQ.inUseSize() > 0 && g_bSendFlag)
	{

		iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(), 0);
		if (iRetVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
				return FALSE;
			}
			g_bSendFlag = FALSE;
			return TRUE;
		}
		SendQ.MoveFront(iRetVal);
	}
	return TRUE;
}

void SendMoveStartPacket()
{
	//보내기
	int iRetVal;

	//직렬화 버퍼를 사용할 것이기 때문에 구조체는 전부 없앤다.

	CPacket MoveStartPacket;
	CPacket Header;
	mpMoveStart(&Header, &MoveStartPacket, g_cPlayer->GetMoveDir(), g_cPlayer->GetCurX(), g_cPlayer->GetCurY());

	//End
	BYTE End = dfNETWORK_PAKCET_END;
	MoveStartPacket << End;

	iRetVal = SendQ.Enqueue((char*)Header.GetBufferPtr(), Header.GetPacketSize());
	if (iRetVal != Header.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}


	iRetVal = SendQ.Enqueue((char*)MoveStartPacket.GetBufferPtr(), MoveStartPacket.GetPacketSize());
	if (iRetVal != MoveStartPacket.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}

	if (g_bSendFlag == FALSE)
		return;

	SendProc();
}

void SendMoveStopPacket()
{
	//보내기
	int iRetVal;

	
	CPacket Header;
	CPacket MoveStopPacket;

	mpMoveStop(&Header,&MoveStopPacket,g_cPlayer->GetCurDir(),g_cPlayer->GetCurX(),g_cPlayer->GetCurY());

	//End
	BYTE End = dfNETWORK_PAKCET_END;
	MoveStopPacket << End;

	iRetVal = SendQ.Enqueue(Header.GetBufferPtr(), Header.GetPacketSize());
	if (iRetVal != Header.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}

	iRetVal = SendQ.Enqueue(MoveStopPacket.GetBufferPtr(), MoveStopPacket.GetPacketSize());
	if (iRetVal != MoveStopPacket.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}

	if (g_bSendFlag == FALSE)
		return;

	iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(), 0);
	if (iRetVal == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
			return;
		}
		g_bSendFlag = FALSE;
		return;
	}

	SendQ.MoveFront(iRetVal);

	if (SendQ.inUseSize() > 0 && g_bSendFlag)
	{

		iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(), 0);
		if (iRetVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
				return;
			}
			g_bSendFlag = FALSE;
			return;
		}
		SendQ.MoveFront(iRetVal);
	}
}

void SendAttack1Packet()
{
	int iRetVal;
	 
	CPacket Attack1Packet;
	CPacket Header;

	mpAttack1(&Header, &Attack1Packet, g_cPlayer->GetCurDir(), g_cPlayer->GetCurX(), g_cPlayer->GetCurY());

	//End
	BYTE End = dfNETWORK_PAKCET_END;
	Attack1Packet << End;

	//SendQ에 넣어주기
	iRetVal = SendQ.Enqueue(Header.GetBufferPtr(), Header.GetPacketSize());
	if (iRetVal != Header.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}

	iRetVal = SendQ.Enqueue(Attack1Packet.GetBufferPtr(), Attack1Packet.GetPacketSize());
	if (iRetVal != Attack1Packet.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}

	iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(),0);
	if (iRetVal == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
			return;
		}
		g_bSendFlag = FALSE;
		return;
	}

	SendQ.MoveFront(iRetVal);

	if (SendQ.inUseSize() > 0 && g_bSendFlag)
	{
		iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(),0);
		if (iRetVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
				return;
			}
			g_bSendFlag = FALSE;
			return;
		}
		SendQ.MoveFront(iRetVal);
	}
}

void SendAttack2Packet()
{
	int iRetVal;

	CPacket Attack2Packet;
	CPacket Header;

	mpAttack2(&Header, &Attack2Packet, g_cPlayer->GetCurDir(), g_cPlayer->GetCurX(), g_cPlayer->GetCurY());

	//End
	BYTE End = dfNETWORK_PAKCET_END;
	Attack2Packet << End;

	//SendQ에 넣어주기
	iRetVal = SendQ.Enqueue(Header.GetBufferPtr(), Header.GetPacketSize());
	if (iRetVal != Header.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}

	iRetVal = SendQ.Enqueue(Attack2Packet.GetBufferPtr(), Attack2Packet.GetPacketSize());
	if (iRetVal != Attack2Packet.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}


	iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(), 0);
	if (iRetVal == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
			return;
		}
		g_bSendFlag = FALSE;
		return;
	}

	SendQ.MoveFront(iRetVal);

	if (SendQ.inUseSize() > 0 && g_bSendFlag)
	{
		iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(), 0);
		if (iRetVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
				return;
			}
			g_bSendFlag = FALSE;
			return;
		}
		SendQ.MoveFront(iRetVal);
	}
}

void SendAttack3Packet()
{
	int iRetVal;

	CPacket Attack3Packet;
	CPacket Header;

	mpAttack3(&Header, &Attack3Packet, g_cPlayer->GetCurDir(), g_cPlayer->GetCurX(), g_cPlayer->GetCurY());

	//End
	BYTE End = dfNETWORK_PAKCET_END;
	Attack3Packet << End;

	//SendQ에 넣어주기
	iRetVal = SendQ.Enqueue(Header.GetBufferPtr(), Header.GetPacketSize());
	if (iRetVal != Header.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}

	iRetVal = SendQ.Enqueue(Attack3Packet.GetBufferPtr(), Attack3Packet.GetPacketSize());
	if (iRetVal != Attack3Packet.GetPacketSize())
	{
		SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
		return;
	}

	iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(), 0);
	if (iRetVal == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
			return;
		}
		g_bSendFlag = FALSE;
		return;
	}

	SendQ.MoveFront(iRetVal);

	if (SendQ.inUseSize() > 0 && g_bSendFlag)
	{
		iRetVal = send(g_Sock, SendQ.GetSendPosOfBuf(), SendQ.NonSplitSendSize(), 0);
		if (iRetVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
				return;
			}
			g_bSendFlag = FALSE;
			return;
		}
		SendQ.MoveFront(iRetVal);
	}
}