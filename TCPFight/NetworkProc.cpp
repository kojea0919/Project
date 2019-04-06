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
	//FALSE�� Return�� ���α׷� ���� ��������
	

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
	//�������� ���� ��Ŷ�� ����
	
	int iRetVal;
	//recv�Լ��� RingBuffer�� �ޱ�
	iRetVal = recv(g_Sock, RecvQ.GetRecvPosOfBuf(), RecvQ.NonSplitRecvSize(), 0);

	//--------------------------------
	//����ó��
	//WOULDBLOCK�̸� ���������� �ذ�
	//�ƴϸ� ����
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

	//recv�� ������ŭ Rear�� �̵���Ű��
	RecvQ.MoveRear(iRetVal);

	//���ũ�⺸�� ������ ������ ũ�Ⱑ ū���� �ݺ�
	while (RecvQ.inUseSize() > dfHEADER_SIZE)
	{
		CPacket Header;
		//��� Peek
		iRetVal = RecvQ.Peek(Header.GetBufferPtr(), dfHEADER_SIZE);

		//����ó��
		if (iRetVal != dfHEADER_SIZE)
		{
			SendMessage(g_hWnd, WM_SOCKETERROR, 0, 0);
			return FALSE;
		}

		Header.MoveWritePos(iRetVal);

		BYTE byCode, bySize, byType, byTemp;
		Header >> byCode >> bySize >> byType >> byTemp;
		//Code Check
		//�̻��� ��Ŷ�̸�? �׳� Dequeue�� ������ ���� ��Ŷ�� ó������?
		BYTE byPayLoadSize = bySize;//END����
		if (byCode != dfNETWORK_PAKCET_CODE)
		{
			RecvQ.MoveFront(dfHEADER_SIZE + byPayLoadSize + 1);
			continue;
		}


		//PayLoad Lenüũ
		//RingBuffer�� ������ �ִ��� Ȯ��
		//�Ȱ����� ������ �������� ó��
		if (byPayLoadSize + dfHEADER_SIZE + 1 > RecvQ.inUseSize())//HEADER + PAYLOAD + END
		{
			return TRUE;
		}

		//������ ������

		//HEADER�� ��ŭ ������
		RecvQ.MoveFront(dfHEADER_SIZE);
		
		//����ȭ ���� �غ�
		CPacket PayLoad;
		
		//PayLoad�̱�
		iRetVal = RecvQ.Dequeue(PayLoad.GetBufferPtr(), byPayLoadSize);
		if (iRetVal != byPayLoadSize)
		{
			return FALSE;
		}
		
		PayLoad.MoveWritePos(iRetVal);

		//EndCode�̱�
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

		//Procȣ��
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
	//������
	int iRetVal;

	//����ȭ ���۸� ����� ���̱� ������ ����ü�� ���� ���ش�.

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
	//������
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

	//SendQ�� �־��ֱ�
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

	//SendQ�� �־��ֱ�
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

	//SendQ�� �־��ֱ�
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