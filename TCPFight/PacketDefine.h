#pragma once

#define dfHEADER_SIZE 4
//��Ŷ ��� ũ��

#define dfNETWORK_PAKCET_CODE ((BYTE)0x89)
//byCode�� �� �ڵ�

#define dfNETWORK_PAKCET_END ((BYTE)0x79)
//EndCode�� �� �ڵ�

#define dfPACKET_SC_CREATE_MY_CHARACTER		0
//Server�� Client�� �������� �� ó������ ������ ��Ŷ����
//ID, Direction, X, Y, HP ������ Setting�Ѵ�.

#define dfPACKET_SC_CREATE_OTHER_CHARACTER	1
//Server�� �ٸ� Client�� �������� �� �����ִ� ��Ŷ����
//���� �������� ������ Setting�Ѵ�.

#define dfPACKET_SC_DELETE_CHARACTER		2
//Server���� � Client�� �׾��� �� �����ִ� ��Ŷ����
//ID���� �־��ش�.

#define dfPACKET_CS_MOVE_START				10
//Client���� ����Ű�� ������ �����̱� ������ �� Server����
//�����ִ� ��Ŷ���� Direction, X, Y ������ Setting�Ѵ�.

#define dfPACKET_SC_MOVE_START				11
//Server�� � Client���� MOVE_START��Ŷ�� ������ �ٸ�
//Client�鿡�Ե� �� ��Ŷ�� �����ش�.

#define dfPACKET_CS_MOVE_STOP				12
//Client���� ����Ű�� ���� �������� ���� �� Server����
//�����ִ� ��Ŷ���� Direction, X ,Y ������ Setting�Ѵ�.

#define dfPACKET_SC_MOVE_STOP				13
//Server�� Client���� MOVE_STOP��Ŷ�� ������ �ٸ�
//Client�鿡�Ե� �� ��Ŷ�� �����ش�.

#define dfPACKET_CS_ATTACK1					20
//Client�� ����Ű�� �Է��ϸ� �����ִ� ��Ŷ����
//Direction, X, Y ������ Setting�Ѵ�.

#define dfPACKET_SC_ATTACK1					21
//Server�� �����ִ� ��Ŷ���� �� ��Ŷ�� �ް� ���� ������ ���Ѵ�.
//Direction, X, Y ������ Setting�Ѵ�.

#define dfPACKET_CS_ATTACK2					22

#define dfPACKET_SC_ATTACK2					23

#define dfPACKET_CS_ATTACK3					24

#define dfPACKET_SC_ATTACK3					25

#define dfPACKET_SC_DAMAGE					30
//Server�� �����ִ� ��Ŷ���� �� ��Ŷ�� ������ �� ������
//���������� �¾Ҵٴ� �ǹ̰� ������ID,������ID,������HP��
//Setting�Ѵ�.