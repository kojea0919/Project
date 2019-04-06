#pragma once


#define dfFrameNum	50
#define dfTimeOfFrame 1000 / dfFrameNum



////OBJECT의 화면 이동 가능영역///
#define dfRANGE_MOVE_TOP	50	//
#define dfRANGE_MOVE_LEFT	10	//	
#define dfRANGE_MOVE_RIGHT	630	//
#define dfRANGE_MOVE_BOTTOM	470	//	
//////////////////////////////////


////애니메이션 프레임 DELAY///
#define dfDELAY_STAND	5	//
#define dfDELAY_MOVE	4	//
#define dfDELAY_ATTACK1	3	//
#define dfDELAY_ATTACK2	4	//
#define dfDELAY_ATTACK3	4	//
#define dfDELAY_EFFECT	3	//
//////////////////////////////



enum e_OBJECT_STATE
{
	eAttackState,
	eMoveState,
	eStandState
};


enum e_OBJECT_TYPE
{
	eTYPE_PLAYER,
	eTYPE_EFFECT
};


//-------------------------------------
//-------------------------------------
//Input Msg로 받을 값으로
//PlayerObject객체의 InputProc함수에서
//처리해준다.

enum e_MOVE_DIRECTION
{
	eACTION_MOVE_LL=0,
	eACTION_MOVE_LU,
	eACTION_MOVE_UU,
	eACTION_MOVE_RU,
	eACTION_MOVE_RR,
	eACTION_MOVE_RD,
	eACTION_MOVE_DD,
	eACTION_MOVE_LD
};

enum e_ATTACK
{
	eATTACK1 = 8,
	eATTACK2,
	eATTACK3
};
//-------------------------------------
//-------------------------------------



enum Sprite
{
	_MAP = 0,

	Stand_L_01,
	Stand_L_02,
	Stand_L_03,
	Stand_L_MAX,

	Stand_R_01,
	Stand_R_02,
	Stand_R_03,
	Stand_R_MAX,

	MOVE_L_01,
	MOVE_L_02,
	MOVE_L_03,
	MOVE_L_04,
	MOVE_L_05,
	MOVE_L_06,
	MOVE_L_07,
	MOVE_L_08,
	MOVE_L_09,
	MOVE_L_10,
	MOVE_L_11,
	MOVE_L_12,
	MOVE_L_MAX,

	MOVE_R_01,
	MOVE_R_02,
	MOVE_R_03,
	MOVE_R_04,
	MOVE_R_05,
	MOVE_R_06,
	MOVE_R_07,
	MOVE_R_08,
	MOVE_R_09,
	MOVE_R_10,
	MOVE_R_11,
	MOVE_R_12,
	MOVE_R_MAX,

	Attack1_L_01,
	Attack1_L_02,
	Attack1_L_03,
	Attack1_L_04,
	Attack1_L_MAX,

	Attack1_R_01,
	Attack1_R_02,
	Attack1_R_03,
	Attack1_R_04,
	Attack1_R_MAX,

	Attack2_L_01,
	Attack2_L_02,
	Attack2_L_03,
	Attack2_L_04,
	Attack2_L_MAX,
		  
	Attack2_R_01,
	Attack2_R_02,
	Attack2_R_03,
	Attack2_R_04,
	Attack2_R_MAX,

	Attack3_L_01,
	Attack3_L_02,
	Attack3_L_03,
	Attack3_L_04,
	Attack3_L_05,
	Attack3_L_06,
	Attack3_L_MAX,
		  
	Attack3_R_01,
	Attack3_R_02,
	Attack3_R_03,
	Attack3_R_04,
	Attack3_R_05,
	Attack3_R_06,
	Attack3_R_MAX,

	xSpark_1,
	xSpark_2,
	xSpark_3,
	xSpark_4,
	xSpark_MAX,

	Shadow,
	HPGuage
};