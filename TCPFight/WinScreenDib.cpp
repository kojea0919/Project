// WinForm.cpp : ���� ���α׷��� ���� �������� �����մϴ�.


#include "stdafx.h"
#include "WinScreenDib.h"

#include "ScreenDib.h"
#include "SpriteDib.h"
#include "define.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "NetworkProc.h"
#include "RingBuffer.h"
#include "NetworkProc.h"
#include "PacketDefine.h"

#pragma comment(lib,"ws2_32")
#pragma comment(lib,"Winmm.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>


vector<CBaseObject*> g_ObjVec;
int iPlayerCnt = 0;



//////////////////////////////////////////////////////
CScreenDib g_cScreenDib(640, 480, 32);				//Dib    Setting
CSpriteDib g_cSpriteDib(HPGuage + 1, 0xffffffff);	//Sprite Setting
//////////////////////////////////////////////////////
CPlayerObject * g_cPlayer;							//User
//////////////////////////////////////////////////////
DWORD g_dwFrameCount = 0;							//Frame�� 50���� ���߱� ���ؼ�
//////////////////////////////////////////////////////


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


SOCKET g_Sock;
HWND g_hWnd;
BOOL g_bActiveApp;//������ â�� Ȱ��ȭ �ƴ���
BOOL g_bConnect = FALSE;
BOOL g_bGenSelf = FALSE;
BOOL g_bKeyPress = FALSE;
BOOL g_bSendFlag = TRUE;

extern CRingBuffer SendQ;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MSG msg;
	HWND hWnd;

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);//�� ����ü�� ������
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;//�߰� ����� ����?
	wcex.hInstance = hInstance;//WinMain�� ���� ���� Instance�ڵ� ��
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINSCREENDIB));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);//�⺻ Ŀ�� �ڵ�
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_WINSCREENDIB);
	wcex.lpszClassName = L"ClassName_990";
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);

	GameInit();

	hWnd = CreateWindowW(L"ClassName_990", L"Caption", WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, nullptr, nullptr, hInstance, nullptr);

	g_hWnd = hWnd;

	if (!hWnd)//������ ������ false
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);//�����츦 ȭ�鿡 �����ش�.

	UpdateWindow(hWnd);
	SetFocus(hWnd);

	RECT WindowRect;
	WindowRect.top = 0;
	WindowRect.left = 0;
	WindowRect.right = 640;
	WindowRect.bottom = 480;
  


	//////////////////////////////////////////
	////////////////////����//////////////////
	int iRetVal;

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	g_Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//TCP���
	if (g_Sock == INVALID_SOCKET)
	{
		wprintf(L"socket create error");
		return 1;
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	InetPtonW(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	iRetVal = WSAAsyncSelect(g_Sock, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	if (iRetVal == SOCKET_ERROR)
	{
		wprintf(L"WSAAsyncSelect error");
		return 1;
	}
	
	iRetVal = connect(g_Sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (iRetVal == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			wprintf(L"connect error");
			return 1;
		}
	}
	else
		g_bConnect = TRUE;
	//////////////////////////////////////////


	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				//���� ����
				closesocket(g_Sock);
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if(g_bConnect)
				Update();		
		}
	}

	for (int iCnt = 0; iCnt < g_ObjVec.size(); ++iCnt)
	{
		delete g_ObjVec[iCnt];
	}


    return (int) msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
 
            switch (wmId)
            {
            case IDM_ABOUT:
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    
	case WM_DESTROY:
        PostQuitMessage(0);
        break;
	
	case WM_ACTIVATEAPP:
		g_bActiveApp = (BOOL)wParam;
		break;

	case WM_SOCKET:
		if (!NetworkProc(lParam))
		{
			PostQuitMessage(0);
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Update()
{
	BYTE *bypDest = g_cScreenDib.GetDibBuffer();
	DWORD iDestWidth = g_cScreenDib.GetWidth();
	DWORD iDestHeight = g_cScreenDib.GetHeight();
	DWORD iDestPitch = g_cScreenDib.GetPitch();

	

	if(g_bActiveApp && g_bGenSelf)
		KeyProcess();

	//map render �κ�
	g_cSpriteDib.DrawImage(_MAP, 0, 0, bypDest, iDestWidth, iDestHeight, iDestPitch);
	//Buffer Clear�� ���� �ʿ� ����. Map���� ��� ���� �����.


	ObjSort();
	//run()

	for (int iCnt = 0; iCnt < g_ObjVec.size(); ++iCnt)
	{
		 CBaseObject * pCurObj = g_ObjVec[iCnt];
		if (!pCurObj->run())
		{
			//����� �۾� ����� �ؾ���
			g_ObjVec.erase(g_ObjVec.begin() + iCnt);

			delete pCurObj;
		}
		//g_ObjVec[iCnt].run();
	}

	//render()
	for (int iCnt = 0; iCnt < g_ObjVec.size(); ++iCnt)
	{
		g_ObjVec[iCnt]->render(bypDest, iDestWidth, iDestHeight, iDestPitch);
		//g_ObjVec[iCnt].render(bypDest, iDestWidth, iDestHeight, iDestPitch);
	}

	//-------------------------------//
	//////////frame ��������///////////

	//if(FrameSkip() && g_bActiveApp)
	if (FrameSkip())
		g_cScreenDib.DrawBuffer(g_hWnd);
	//�ð��� ����� ��쿡�� render 


	//-------------------------------//



}

BOOL FrameSkip()
{
	timeBeginPeriod(1);
	static DWORD PrevTime = NULL;
	static DWORD CurTime = NULL;

	static DWORD dwFrameStart = NULL;
	static DWORD dwErrorTime = 0;

	BOOL bFrameSkip = TRUE;

	//ó�� ���Խô� pass
	if (NULL == PrevTime)
	{
		PrevTime = timeGetTime();
		dwFrameStart = PrevTime;
		++g_dwFrameCount;

		return bFrameSkip;
	}

	//1 frame ó���ϴµ� �ɸ� �ð� ���
	CurTime = timeGetTime();
	DWORD DeltaTime = CurTime - PrevTime;

	//���� �������� ������ŭ �����ش�.
	if (DeltaTime <= dfTimeOfFrame)
	{
		Sleep(dfTimeOfFrame - DeltaTime);
		PrevTime = timeGetTime();
	}

	//���� ���� ���� ���
	else
	{
		dwErrorTime += DeltaTime - dfTimeOfFrame;
		if (dwErrorTime >= dfTimeOfFrame)
		{
			dwErrorTime = dwErrorTime - dfTimeOfFrame;
			PrevTime = timeGetTime();
			
			bFrameSkip = FALSE;
		}
		PrevTime = timeGetTime();
	}

	if (CurTime - dwFrameStart >= 1000)
	{
		WCHAR buf[10];
		_itow_s(g_dwFrameCount, buf, 10, 10);
		SetWindowText(g_hWnd, buf);
		dwFrameStart = timeGetTime();
		g_dwFrameCount = 0;
	}


	if(bFrameSkip)
		++g_dwFrameCount;

	return bFrameSkip;
}

BOOL GameInit(void)
{
	if (!g_cSpriteDib.LoadDibSprite(_MAP, L"Sprite\\_Map.bmp", 0, 0))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Stand_L_01, L"Sprite\\Stand_L_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Stand_L_02, L"Sprite\\Stand_L_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Stand_L_03, L"Sprite\\Stand_L_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Stand_R_01, L"Sprite\\Stand_R_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Stand_R_02, L"Sprite\\Stand_R_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Stand_R_03, L"Sprite\\Stand_R_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_01, L"Sprite\\MOVE_L_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_02, L"Sprite\\MOVE_L_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_03, L"Sprite\\MOVE_L_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_04, L"Sprite\\MOVE_L_04.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_05, L"Sprite\\MOVE_L_05.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_06, L"Sprite\\MOVE_L_06.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_07, L"Sprite\\MOVE_L_07.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_08, L"Sprite\\MOVE_L_08.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_09, L"Sprite\\MOVE_L_09.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_10, L"Sprite\\MOVE_L_10.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_11, L"Sprite\\MOVE_L_11.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_L_12, L"Sprite\\MOVE_L_12.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_01, L"Sprite\\MOVE_R_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_02, L"Sprite\\MOVE_R_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_03, L"Sprite\\MOVE_R_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_04, L"Sprite\\MOVE_R_04.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_05, L"Sprite\\MOVE_R_05.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_06, L"Sprite\\MOVE_R_06.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_07, L"Sprite\\MOVE_R_07.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_08, L"Sprite\\MOVE_R_08.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_09, L"Sprite\\MOVE_R_09.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_10, L"Sprite\\MOVE_R_10.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_11, L"Sprite\\MOVE_R_11.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(MOVE_R_12, L"Sprite\\MOVE_R_12.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack1_L_01, L"Sprite\\Attack1_L_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack1_L_02, L"Sprite\\Attack1_L_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack1_L_03, L"Sprite\\Attack1_L_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack1_L_04, L"Sprite\\Attack1_L_04.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack1_R_01, L"Sprite\\Attack1_R_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack1_R_02, L"Sprite\\Attack1_R_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack1_R_03, L"Sprite\\Attack1_R_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack1_R_04, L"Sprite\\Attack1_R_04.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack2_L_01, L"Sprite\\Attack2_L_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack2_L_02, L"Sprite\\Attack2_L_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack2_L_03, L"Sprite\\Attack2_L_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack2_L_04, L"Sprite\\Attack2_L_04.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack2_R_01, L"Sprite\\Attack2_R_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack2_R_02, L"Sprite\\Attack2_R_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack2_R_03, L"Sprite\\Attack2_R_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack2_R_04, L"Sprite\\Attack2_R_04.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_L_01, L"Sprite\\Attack3_L_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_L_02, L"Sprite\\Attack3_L_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_L_03, L"Sprite\\Attack3_L_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_L_04, L"Sprite\\Attack3_L_04.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_L_05, L"Sprite\\Attack3_L_05.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_L_06, L"Sprite\\Attack3_L_06.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_R_01, L"Sprite\\Attack3_R_01.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_R_02, L"Sprite\\Attack3_R_02.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_R_03, L"Sprite\\Attack3_R_03.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_R_04, L"Sprite\\Attack3_R_04.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_R_05, L"Sprite\\Attack3_R_05.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Attack3_R_06, L"Sprite\\Attack3_R_06.bmp", 71, 90))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(Shadow, L"Sprite\\Shadow.bmp", 32, 4))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(xSpark_1, L"Sprite\\xSpark_1.bmp", 70, 70))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(xSpark_2, L"Sprite\\xSpark_2.bmp", 70, 70))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(xSpark_3, L"Sprite\\xSpark_3.bmp", 70, 70))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(xSpark_4, L"Sprite\\xSpark_4.bmp", 70, 70))
		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(HPGuage, L"Sprite\\HPGuage.bmp", 0, 0))
		return FALSE;


	//�������� �� �κ�
	//g_cPlayer = new CPlayerObject(100, 100, TRUE);
	//g_ObjVec.push_back(g_cPlayer);

	//iPlayerCnt = 1;

	return TRUE;
}

//************************************************//
//*****************�ڵ� ��������******************//
//************************************************//
void KeyProcess()
{
	/////////////////////////
	//---------------------//
	//����Ű �Է� ó�� �κ�//
	//---------------------//
	/////////////////////////
	DWORD dwPrevAction = g_cPlayer->GetActionState();
	BYTE byPrevMoveDir = g_cPlayer->GetMoveDir();

	BYTE byNewMoveDir;
	

	if (GetAsyncKeyState(VK_LEFT) & 0x8001)
	{
		byNewMoveDir = eACTION_MOVE_LL;
		//LL
		/*g_cPlayer->ActionInput(eACTION_MOVE_LL);
		g_cPlayer->SetMoveDir(eACTION_MOVE_LL);*/
		if (GetAsyncKeyState(VK_UP) & 0x8001)
		{
			//������ ���� Packet�� �������ϴ� ������?
			// ó�� �������� ������ ��(ó����)
			// �������� ������ �ٲ���� ��
			byNewMoveDir = eACTION_MOVE_LU;
			//LU
			/*g_cPlayer->ActionInput(eACTION_MOVE_LU);
			g_cPlayer->SetMoveDir(eACTION_MOVE_LU);*/
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8001)
		{
			byNewMoveDir = eACTION_MOVE_LD;
			//LD
			/*g_cPlayer->ActionInput(eACTION_MOVE_LD);
			g_cPlayer->SetMoveDir(eACTION_MOVE_LD);*/
			//return;
		}
	}

	else if (GetAsyncKeyState(VK_RIGHT) & 0x8001)
	{
		byNewMoveDir = eACTION_MOVE_RR;
		/*g_cPlayer->ActionInput(eACTION_MOVE_RR);
		g_cPlayer->SetMoveDir(eACTION_MOVE_RR);*/


		if (GetAsyncKeyState(VK_UP) & 0x8001)
		{
			byNewMoveDir = eACTION_MOVE_RU;

			//RU
		/*	g_cPlayer->ActionInput(eACTION_MOVE_RU);
			g_cPlayer->SetMoveDir(eACTION_MOVE_RU);*/
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8001)
		{
			byNewMoveDir = eACTION_MOVE_RD;

			//RD
		/*	g_cPlayer->ActionInput(eACTION_MOVE_RD);
			g_cPlayer->SetMoveDir(eACTION_MOVE_RD);*/
		}

	}
	else if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		byNewMoveDir = eACTION_MOVE_UU;

		/*g_cPlayer->ActionInput(eACTION_MOVE_UU);
		g_cPlayer->SetMoveDir(eACTION_MOVE_UU);*/
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		byNewMoveDir = eACTION_MOVE_DD;

	/*	g_cPlayer->ActionInput(eACTION_MOVE_DD);
		g_cPlayer->SetMoveDir(eACTION_MOVE_DD);*/
	}
	
	/////////////////////////
	//---------------------//
	//����Ű �Է� ó�� �κ�//
	//---------------------//
	/////////////////////////
	else if (GetAsyncKeyState(0x41) & 0x8001)
	{
		if(g_cPlayer->GetActionState() != eAttackState)
			SendAttack1Packet();
		g_cPlayer->ActionInput(eATTACK1);
		return;
	}
	else if (GetAsyncKeyState(0x53) & 0x8001)
	{
		if (g_cPlayer->GetActionState() != eAttackState)
			SendAttack2Packet();
		g_cPlayer->ActionInput(eATTACK2);
		return;
	}
	else if (GetAsyncKeyState(0x44) & 0x8001)
	{
		if (g_cPlayer->GetActionState() != eAttackState)
			SendAttack3Packet();
		g_cPlayer->ActionInput(eATTACK3);
		return;
	}
	else
	{
		if (g_cPlayer->GetActionState() != eStandState && g_cPlayer->GetActionState() != eAttackState)
			SendMoveStopPacket();
		g_cPlayer->ActionInput(-1);
		return;
	}


	g_cPlayer->ActionInput(byNewMoveDir);
	g_cPlayer->SetMoveDir(byNewMoveDir);
	if (dwPrevAction != eAttackState &&
		(dwPrevAction == eStandState ||
			byPrevMoveDir != byNewMoveDir))
	{
		SendMoveStartPacket();
	}
}

void ObjSort()
{
	for (int iCnt1 = 1; iCnt1 < iPlayerCnt; ++iCnt1)
	{
		for (int iCnt2 = 0; iCnt2 < iPlayerCnt - iCnt1 ; ++iCnt2)
		{
			CBaseObject * cTarget1 = g_ObjVec[iCnt2];
			CBaseObject * cTarget2 = g_ObjVec[iCnt2 + 1];
			if (cTarget1->GetCurY() > cTarget2->GetCurY())
			{
				g_ObjVec[iCnt2] = cTarget2;
				g_ObjVec[iCnt2 + 1] = cTarget1;
			}
		}
	}
}
