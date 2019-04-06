#pragma once

class CRingBuffer
{
public:
	//--------------------------
	//         생성자
	CRingBuffer()
		: _iFront(0), _iRear(0), _iCapacity(10000)
	{
		_pBuffer = new char[10000];
	}
	CRingBuffer(int iBufferSize)
		:_iFront(0), _iRear(0), _iCapacity(iBufferSize)
	{
		_pBuffer = new char[iBufferSize];
	}
	~CRingBuffer()
	{
		delete[] _pBuffer;
	}
	//--------------------------

public:
	//--------------------------
	//        멤버 함수
	int inUseSize()
	{
		return (_iRear - _iFront + _iCapacity) % _iCapacity;
	}
	//현재 사용 중인 용량을 반환한다.(byte단위)
	
	int RemainSize()
	{
		return _iCapacity - inUseSize() - 1;
	}
	//현재 남은 용량을 반환한다.

	void MoveRear(int iSize)
	{
		_iRear = (_iRear + iSize) % _iCapacity;
	}
	void MoveFront(int iSize)
	{
		_iFront = (_iFront + iSize) % _iCapacity;
	}
	//인자 크기 만큼 Rear 또는 Front를 이동시킨다.

	int NonSplitRecvSize();
	//Direct로 Recv할 때 사용한다.

	int NonSplitSendSize();
	//Direct로 Send할 때 사용한다.

	int Enqueue(char *chpData, int iSize);
	//chpData부터 iSize만큼 Buffer에 복사한다.

	int Dequeue(char *chpData, int iSize);
	//Front부터 iSize만큼 chpData에 복사한다.

	int Peek(char * chpData, int iSize);
	//데이터 읽어오기

	void ClearBuffer() { _iFront = _iRear; }
	//버퍼 비우기

	char* GetRecvPosOfBuf() { return _pBuffer + (_iRear + 1)%_iCapacity; }
	char* GetSendPosOfBuf() { return _pBuffer + (_iFront + 1)%_iCapacity; }

private:
	//-------------------------
	//       멤버 변수
	char*		_pBuffer;
	int			_iFront;
	int			_iRear;
	int			_iCapacity;
	//-------------------------
};

