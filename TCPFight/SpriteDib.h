#pragma once
class CSpriteDib
{
public:
	//-------------------------
	//DIB ��������Ʈ ����ü
	//-------------------------
	typedef struct st_SPRITE
	{
		BYTE*	_bypImage;
		DWORD	_iWidth;
		DWORD	_iHeight;
		DWORD	_iPitch;

		DWORD	_iCenterPointX;
		DWORD	_iCenterPointY;
		//Image���� ���� x,y��ǥ
	};

public:
	CSpriteDib(int iMaxSprite,DWORD dwColorKey);
	//---------------------------------------------------
	//iMaxSprite : �ҷ��� ���� ����(�̸� �����Ҵ��� ����)
	//dwColorKey : ����ó���� ��(0xffffffff)
	//---------------------------------------------------
	virtual ~CSpriteDib();


	//----------------------------------------
	//LoadDibSprite�Լ�
	//bmp������ �о �ϳ��� ���������� ����
	//----------------------------------------
	BOOL LoadDibSprite(int iSpriteIndex, wchar_t *szFileName, int iCenterPointX, int iCenterPointY);

	//----------------------------------------
	//ReleaseSprite�Լ�
	//�ش� ��������Ʈ ����
	//----------------------------------------
	void ReleaseSprite(int iSpriteIndex);

	//------------------------------------------------------------------
	//DrawSprite
	//�ش� �޸� ��ġ�� �����ó��, Ŭ����ó�� �� ��������Ʈ�� ����Ѵ�. 
	//------------------------------------------------------------------
	void DrawSprite(int iSpriteIndex,int iDrawX, int iDrawY, BYTE *bypDest,int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);
	//iSpriteIndex : ��� ° Sprite�� �������
	//iDrawX	   : �׸����� �ϴ� �޸� x��ġ
	//iDrawY	   : �׸����� �ϴ� �޸� y��ġ
	//bypDest	   : ������� ���� ��ġ
	//iDestWidth   : ������� Width
	//iDestHeight  : ������� height
	//iDestPitch   : ������� Pitch
	//iDrawlen	   : �׸� �κ��� Width�� �����ϱ� ���� ��


	//------------------------------------------------------------------
	//DrawMixSprite
	//���� �ش� �κп� ���� ĥ���������� ���������� ���̵��� ����Ѵ�.
	//------------------------------------------------------------------
	void DrawMixSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch);

	//------------------------------------------------------------------
	//DrawImage
	//�ٴ��� ���簡 �̷�� ���� Map Sprite�� ó�����ֱ� ���� �Լ�
	//------------------------------------------------------------------
	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE * bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);

	//------------------------------------------------------------------
	//DrawRedSprite
	//Sprtie�� ������ �κ��� �����ؼ� ������ֱ� ���� �Լ�
	//------------------------------------------------------------------
	void DrawRedSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch);


protected:
	int			_iMaxSprite;
	st_SPRITE * _stpSprite;

	DWORD		_dwColorKey;
};

