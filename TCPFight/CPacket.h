#pragma once

class CPacket
{
public:
	enum en_PACKET
	{
		eBUFFER_DEFAULT				= 1024,
		ePACKET_MAX_SIZE			= 1024
	};

	//---------------------
	//생성자 & 복사 생성자
	//---------------------
	CPacket();
	CPacket(int iBufferSize);
	CPacket(const CPacket & cSrcPacket);

	//----------------------------
	//소멸자(상속으로 확장도 고려)
	//----------------------------
	virtual ~CPacket();

protected:
	//생성자 호출시 변수 setting
	//void Initial(int iBufferSize = eBUFFER_DEFAULT);
	//소멸자 호출시 처리해줄 것들
	//void Release();

public:
	//버퍼 비우기
	void Clear();

	//버퍼 크기
	int GetBufferSize() { return _iCapacity; }

	//패킷 크기
	int GetPacketSize() { return _iPacketSize; }

	//버퍼 포인터
	char* GetBufferPtr()  {return _pBuffer; }

	//버퍼에서 Write 위치 이동
	int MoveWritePos(int iSize);

	//버퍼에서 Read 위치 이동
	int MoveReadPos(int iSize);

	//데이터 삽입
	int Enqueue(char * pSrc, int iSize);

	//데이터 얻기
	int Dequeue(char * pDest, int iSize);

	//연산자 오버로딩
	CPacket & operator << (int iValue);
	CPacket & operator >> (int & iValue);

	CPacket & operator << (long lValue);
	CPacket & operator >> (long & lValue);

	CPacket & operator << (short sValue);
	CPacket & operator >> (short & sValue);

	CPacket & operator << (char cValue);
	CPacket & operator >> (char & cValue);

	CPacket & operator << (float fValue);
	CPacket & operator >> (float & fValue);

	CPacket & operator << (double dValue);
	CPacket & operator >> (double & dValue);

	CPacket & operator << (BYTE bValue);
	CPacket & operator >> (BYTE & bValue);

	CPacket & operator << (WORD wValue);
	CPacket & operator >> (WORD & wValue);

	CPacket & operator << (DWORD dwValue);
	CPacket & operator >> (DWORD & dwValue);
private:
	char *		_pBuffer;
	int			_iCapacity;
	int			_iPacketSize;
	int			_iWritePos;
	int			_iReadPos;
};