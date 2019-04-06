#pragma once

class CRingBuffer
{
public:
	//--------------------------
	//         ������
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
	//        ��� �Լ�
	int inUseSize()
	{
		return (_iRear - _iFront + _iCapacity) % _iCapacity;
	}
	//���� ��� ���� �뷮�� ��ȯ�Ѵ�.(byte����)
	
	int RemainSize()
	{
		return _iCapacity - inUseSize() - 1;
	}
	//���� ���� �뷮�� ��ȯ�Ѵ�.

	void MoveRear(int iSize)
	{
		_iRear = (_iRear + iSize) % _iCapacity;
	}
	void MoveFront(int iSize)
	{
		_iFront = (_iFront + iSize) % _iCapacity;
	}
	//���� ũ�� ��ŭ Rear �Ǵ� Front�� �̵���Ų��.

	int NonSplitRecvSize();
	//Direct�� Recv�� �� ����Ѵ�.

	int NonSplitSendSize();
	//Direct�� Send�� �� ����Ѵ�.

	int Enqueue(char *chpData, int iSize);
	//chpData���� iSize��ŭ Buffer�� �����Ѵ�.

	int Dequeue(char *chpData, int iSize);
	//Front���� iSize��ŭ chpData�� �����Ѵ�.

	int Peek(char * chpData, int iSize);
	//������ �о����

	void ClearBuffer() { _iFront = _iRear; }
	//���� ����

	char* GetRecvPosOfBuf() { return _pBuffer + (_iRear + 1)%_iCapacity; }
	char* GetSendPosOfBuf() { return _pBuffer + (_iFront + 1)%_iCapacity; }

private:
	//-------------------------
	//       ��� ����
	char*		_pBuffer;
	int			_iFront;
	int			_iRear;
	int			_iCapacity;
	//-------------------------
};

