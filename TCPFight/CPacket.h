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
	//������ & ���� ������
	//---------------------
	CPacket();
	CPacket(int iBufferSize);
	CPacket(const CPacket & cSrcPacket);

	//----------------------------
	//�Ҹ���(������� Ȯ�嵵 ���)
	//----------------------------
	virtual ~CPacket();

protected:
	//������ ȣ��� ���� setting
	//void Initial(int iBufferSize = eBUFFER_DEFAULT);
	//�Ҹ��� ȣ��� ó������ �͵�
	//void Release();

public:
	//���� ����
	void Clear();

	//���� ũ��
	int GetBufferSize() { return _iCapacity; }

	//��Ŷ ũ��
	int GetPacketSize() { return _iPacketSize; }

	//���� ������
	char* GetBufferPtr()  {return _pBuffer; }

	//���ۿ��� Write ��ġ �̵�
	int MoveWritePos(int iSize);

	//���ۿ��� Read ��ġ �̵�
	int MoveReadPos(int iSize);

	//������ ����
	int Enqueue(char * pSrc, int iSize);

	//������ ���
	int Dequeue(char * pDest, int iSize);

	//������ �����ε�
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