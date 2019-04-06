#include "stdafx.h"
#include "CPacket.h"
#include <string.h>

CPacket::CPacket()
	: _iCapacity(eBUFFER_DEFAULT), _iPacketSize(0), _iWritePos(0), _iReadPos(0)
{
	_pBuffer = new char[eBUFFER_DEFAULT];
}

CPacket::CPacket(int iBufferSize)
	: _iCapacity(iBufferSize), _iPacketSize(0), _iWritePos(0), _iReadPos(0)
{
	_pBuffer = new char[iBufferSize];
}

CPacket::CPacket(const CPacket & cSrcPacket)
	: _iCapacity(cSrcPacket._iCapacity), _iPacketSize(cSrcPacket._iPacketSize)
	,_iWritePos(cSrcPacket._iWritePos),_iReadPos(cSrcPacket._iReadPos)
{
	memcpy(_pBuffer, cSrcPacket._pBuffer, cSrcPacket._iPacketSize);
}

CPacket::~CPacket()
{
	delete _pBuffer;
}

void CPacket::Clear()
{
	//Size�� Cursor�� 0���� ������
	_iPacketSize = 0;
	_iWritePos = 0;
	_iReadPos = 0;
}

int CPacket::MoveWritePos(int iSize)
{
	//iSize�� �����ΰ�� -1 return
	if (iSize < 0)
		return -1;

	//�����ִ� ũ�⺸�� ū ���� ������ iSize ����
	if (iSize > _iCapacity - _iPacketSize)
		iSize = _iCapacity - _iPacketSize;

	//Cursor�����̱�
	_iWritePos += iSize;
	_iPacketSize += iSize;
	//������ �� ��ȯ
	return iSize;
}

int CPacket::MoveReadPos(int iSize)
{
	//�ϴ� ����
	return 0;
}

int CPacket::Enqueue(char * pSrc, int iSize)
{
	//pSrc�� NULL���̸� -1�� return
	if (NULL == pSrc || iSize < 0)
		return -1;

	//�����ִ� ũ�⺸�� ū �����Ͱ� ������ iSize����
	if(iSize > _iCapacity - _iPacketSize)
		iSize = _iCapacity - _iPacketSize;

	//memcpy�� �ƴ� ������ ����
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
		_pBuffer[_iWritePos + iCnt] = pSrc[iCnt];

	_iPacketSize += iSize;
	_iWritePos += iSize;

	return iSize;
}

int CPacket::Dequeue(char * pDest, int iSize)
{
	//pDest�� NULL���̸� -1�� return
	if (NULL == pDest || iSize < 0)
		return -1;

	//������ �ִ� ũ�⺸�� ū ���� ��û�ϸ� iSize����
	if (iSize > _iPacketSize)
		iSize = _iPacketSize;

	//������ ����
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
		pDest[iCnt] = _pBuffer[_iReadPos + iCnt];

	_iPacketSize -= iSize;
	_iReadPos += iSize;

	return iSize;
}

CPacket & CPacket::operator<<(int iValue)
{
	Enqueue((char*)&iValue, sizeof(int));
	return *this;
}

CPacket & CPacket::operator >> (int & iValue)
{
	Dequeue((char*)&iValue, sizeof(int));
	return *this;
}

CPacket & CPacket::operator<<(long lValue)
{
	Enqueue((char*)&lValue, sizeof(long));
	return *this;
}

CPacket & CPacket::operator >> (long & lValue)
{
	Dequeue((char*)&lValue, sizeof(long));
	return *this;
}

CPacket & CPacket::operator<<(short sValue)
{
	Enqueue((char*)&sValue, sizeof(short));
	return *this;
}

CPacket & CPacket::operator >> (short & sValue)
{
	Dequeue((char*)&sValue, sizeof(short));
	return *this;
}

CPacket & CPacket::operator<<(char cValue)
{
	Enqueue((char*)&cValue, sizeof(char));
	return *this;
}

CPacket & CPacket::operator >> (char & cValue)
{
	Dequeue((char*)&cValue, sizeof(char));
	return *this;
}

CPacket & CPacket::operator<<(float fValue)
{
	Enqueue((char*)&fValue, sizeof(float));
	return *this;
}

CPacket & CPacket::operator >> (float & fValue)
{
	Dequeue((char*)&fValue, sizeof(float));
	return *this;
}

CPacket & CPacket::operator<<(double dValue)
{
	Enqueue((char*)&dValue, sizeof(double));
	return *this;
}

CPacket & CPacket::operator >> (double & dValue)
{
	Dequeue((char*)&dValue, sizeof(double));
	return *this;
}

CPacket & CPacket::operator<<(BYTE bValue)
{
	Enqueue((char*)&bValue, sizeof(BYTE));
	return *this;
}

CPacket & CPacket::operator >> (BYTE & bValue)
{
	Dequeue((char*)&bValue, sizeof(BYTE));
	return *this;
}

CPacket & CPacket::operator<<(WORD wValue)
{
	Enqueue((char*)&wValue, sizeof(WORD));
	return *this;
}

CPacket & CPacket::operator >> (WORD & wValue)
{
	Dequeue((char*)&wValue, sizeof(WORD));
	return *this;
}

CPacket & CPacket::operator<<(DWORD dwValue)
{
	Enqueue((char*)&dwValue, sizeof(DWORD));
	return *this;
}

CPacket & CPacket::operator >> (DWORD & dwValue)
{
	Dequeue((char*)&dwValue, sizeof(DWORD));
	return *this;
}

