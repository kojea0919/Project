#include "stdafx.h"
#include "RingBuffer.h"

int CRingBuffer::NonSplitRecvSize()
{
	//��� ������ ������ ������ �Ѿ�� ���
	if (RemainSize() + (_iRear + 1)%_iCapacity > _iCapacity)
	{
		return _iCapacity - _iRear - 1;
	}
	return RemainSize();
}

int CRingBuffer::NonSplitSendSize()
{
	//��� ���� ������ ������ ���ļ� �ִ� ���
	if (inUseSize() + (_iFront + 1)%_iCapacity > _iCapacity)
	{
		return _iCapacity - _iFront - 1;
	}
	return inUseSize();
}

int CRingBuffer::Enqueue(char * chpData, int iSize)
{
	int iRemainSize = RemainSize();
	//���� ���� �����ִ� ������ ������� �ľ��Ѵ�.
	if (iRemainSize < iSize)
		//���� ������ iSize���� ������ iSize�� ���� �������� ����
	{
		iSize = iRemainSize;
	}

	int iInsertPos = (_iRear + 1) % _iCapacity;//�����Ͱ� ���� �����ϴ� ��ġ
	//��踦 �Ѿ 2���� ���ļ� ���縦 �ؾ��ϴ��� Ȯ��
	if (iInsertPos + iSize > _iCapacity)
	{
		int iFirstCopyCnt = _iCapacity - iInsertPos;
		//ù ��° ���翡���� ����
		memcpy(_pBuffer + iInsertPos, chpData, iFirstCopyCnt);
		memcpy(_pBuffer, chpData + iFirstCopyCnt, iSize - iFirstCopyCnt);
	}

	//�ƴϸ� �׳� ����
	else
	{
		memcpy(_pBuffer + iInsertPos, chpData, iSize);
	}

	//Rear����
	MoveRear(iSize);

	return iSize;
}

int CRingBuffer::Dequeue(char * chpData, int iSize)
{
	int iUseSIze = inUseSize();
	//���� ������� �������� �䱸�ϴ� ������ ū�� �ľ��Ѵ�.
	if (iUseSIze < iSize)
		//ū��� iSize�� �����Ѵ�.
	{
		iSize = iUseSIze;
	}


	int iOutPos = (_iFront + 1) % _iCapacity;
	//�ι��� ������ �����ؾ��ϴ��� Ȯ���Ѵ�.(������ �Ѿ�� ���)
	if (iOutPos + iSize > _iCapacity)
	{
		int iFirstCopyCnt = _iCapacity - iOutPos;
		memcpy(chpData, _pBuffer + iOutPos, iFirstCopyCnt);
		memcpy(chpData + iFirstCopyCnt, _pBuffer, iSize - iFirstCopyCnt);
	}

	//�ƴϸ� �׳� ����
	else
	{
		memcpy(chpData, _pBuffer + iOutPos, iSize); 
	}
	//Front����
	MoveFront(iSize);

	return iSize;
}

int CRingBuffer::Peek(char * chpData, int iSize)
{
	int iUseSIze = inUseSize();
	//���� ������� �������� �䱸�ϴ� ������ ū�� �ľ��Ѵ�.
	if (iUseSIze < iSize)
		//ū��� iSize�� �����Ѵ�.
	{
		iSize = iUseSIze;
	}


	int iOutPos = (_iFront + 1) % _iCapacity;
	//�ι��� ������ �����ؾ��ϴ��� Ȯ���Ѵ�.(������ �Ѿ�� ���)
	if (iOutPos + iSize > _iCapacity)
	{
		int iFirstCopyCnt = _iCapacity - iOutPos;
		memcpy(chpData, _pBuffer + iOutPos, iFirstCopyCnt);
		memcpy(chpData + iFirstCopyCnt, _pBuffer, iSize - iFirstCopyCnt);
	}

	//�ƴϸ� �׳� ����
	else
	{
		memcpy(chpData, _pBuffer + iOutPos, iSize);
	}

	return iSize;
}
