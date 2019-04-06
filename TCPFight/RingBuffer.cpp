#include "stdafx.h"
#include "RingBuffer.h"

int CRingBuffer::NonSplitRecvSize()
{
	//사용 가능한 공간이 경계면을 넘어가는 경우
	if (RemainSize() + (_iRear + 1)%_iCapacity > _iCapacity)
	{
		return _iCapacity - _iRear - 1;
	}
	return RemainSize();
}

int CRingBuffer::NonSplitSendSize()
{
	//사용 중인 공간이 경계면을 걸쳐서 있는 경우
	if (inUseSize() + (_iFront + 1)%_iCapacity > _iCapacity)
	{
		return _iCapacity - _iFront - 1;
	}
	return inUseSize();
}

int CRingBuffer::Enqueue(char * chpData, int iSize)
{
	int iRemainSize = RemainSize();
	//먼저 현재 남아있는 공간이 충분한지 파악한다.
	if (iRemainSize < iSize)
		//남은 공간이 iSize보다 작으면 iSize를 남은 공간으로 갱신
	{
		iSize = iRemainSize;
	}

	int iInsertPos = (_iRear + 1) % _iCapacity;//데이터가 들어가기 시작하는 위치
	//경계를 넘어서 2번에 걸쳐서 복사를 해야하는지 확인
	if (iInsertPos + iSize > _iCapacity)
	{
		int iFirstCopyCnt = _iCapacity - iInsertPos;
		//첫 번째 복사에서의 개수
		memcpy(_pBuffer + iInsertPos, chpData, iFirstCopyCnt);
		memcpy(_pBuffer, chpData + iFirstCopyCnt, iSize - iFirstCopyCnt);
	}

	//아니면 그냥 복사
	else
	{
		memcpy(_pBuffer + iInsertPos, chpData, iSize);
	}

	//Rear갱신
	MoveRear(iSize);

	return iSize;
}

int CRingBuffer::Dequeue(char * chpData, int iSize)
{
	int iUseSIze = inUseSize();
	//현재 사용중인 공간보다 요구하는 공간이 큰지 파악한다.
	if (iUseSIze < iSize)
		//큰경우 iSize를 갱신한다.
	{
		iSize = iUseSIze;
	}


	int iOutPos = (_iFront + 1) % _iCapacity;
	//두번에 나눠서 복사해야하는지 확인한다.(경계면을 넘어가는 경우)
	if (iOutPos + iSize > _iCapacity)
	{
		int iFirstCopyCnt = _iCapacity - iOutPos;
		memcpy(chpData, _pBuffer + iOutPos, iFirstCopyCnt);
		memcpy(chpData + iFirstCopyCnt, _pBuffer, iSize - iFirstCopyCnt);
	}

	//아니면 그냥 복사
	else
	{
		memcpy(chpData, _pBuffer + iOutPos, iSize); 
	}
	//Front갱신
	MoveFront(iSize);

	return iSize;
}

int CRingBuffer::Peek(char * chpData, int iSize)
{
	int iUseSIze = inUseSize();
	//현재 사용중인 공간보다 요구하는 공간이 큰지 파악한다.
	if (iUseSIze < iSize)
		//큰경우 iSize를 갱신한다.
	{
		iSize = iUseSIze;
	}


	int iOutPos = (_iFront + 1) % _iCapacity;
	//두번에 나눠서 복사해야하는지 확인한다.(경계면을 넘어가는 경우)
	if (iOutPos + iSize > _iCapacity)
	{
		int iFirstCopyCnt = _iCapacity - iOutPos;
		memcpy(chpData, _pBuffer + iOutPos, iFirstCopyCnt);
		memcpy(chpData + iFirstCopyCnt, _pBuffer, iSize - iFirstCopyCnt);
	}

	//아니면 그냥 복사
	else
	{
		memcpy(chpData, _pBuffer + iOutPos, iSize);
	}

	return iSize;
}
