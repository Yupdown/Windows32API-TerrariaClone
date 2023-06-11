#pragma once

#define bit_abs_nonZ(x)  ((((x>>31) ^ x) - (x>>31))|1)
#define fDT CTimeMgr::GetInst()->GetfDT()			
#define DT	CTimeMgr::GetInst()->GetDT()			

#define CLONE(type)  type* Clone()const override{return new type{*this};} 

#define KEY_CHECK(key,state)	CKeyMgr::GetInst()->GetKeyState(key)==state
#define KEY_HOLD(key)			KEY_CHECK(key,KEY_STATE::HOLD)		
#define KEY_TAP(key)			KEY_CHECK(key,KEY_STATE::TAP)
#define KEY_AWAY(key)			KEY_CHECK(key,KEY_STATE::AWAY)
#define KEY_NONE(key)			KEY_CHECK(key,KEY_STATE::NONE)

#define MOUSE_POS (CCamera::GetInst()->GetRealPos(CKeyMgr::GetInst()->GetMousePos()))

#define PI	3.1415926535f

#define TILE_SIZE 64 // 타일 사이즈 픽셀 매크로

#define COMP_CLONE(type,owner)	type* Clone(owner* const _pOwner)const override {type* pNewComp = new type {*this}; pNewComp->SetOwner(_pOwner);return pNewComp;}

enum class GROUP_TYPE	
{
	DEFAULT,	
	TILE,		
	GROUND, 
	MONSTER,	
	PLAYER,		
	PROJ_PLAYER, 
	PROJ_MONSTER, 

	PLAYER_WEAPON,
	DROP_ITEM,

	UI = 31,		
	
	END = 32, 
};

enum class SCENE_TYPE	
{
	TOOL,
	START,
	STAGE_01,
	STAGE_02,
	INTRO,


	END,
};

enum class BRUSH_TYPE
{
	HOLLOW,
	BLACK, 
	WHITE,
	RED,
	GREEN,
	BLUE,
	MAZENTA,

	END,
};

enum class PEN_TYPE
{
	BLACK,
	RED,
	GREEN,
	BLUE,
	MAZENTA,

	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT, 
	DELETE_OBJECT, 
	SCENE_CHANGE, 
	CHANGE_AI_STATE,
	END, 
};

enum class MON_STATE		
{
	IDLE,
	PATROL,
	TRACE,
	ATK,
	RUN,
	DEAD,
	END,
};

enum class COMPONENT_TYPE
{
	COLLIDER,
	ANIMATOR,
	RIGIDBODY,

	END,
};
	
#define Mgr(type)	(type::GetInst())

enum class SOUND_TYPE
{
	WALK,
	ATTACK,
	JUMP,
	HIT,
	DIE,

	END
};

enum THREAD
{
	T0,
	T1,
	T2,
	
	END
};

#define SRC_TILE_SIZE 8

#define SRC_TILE_SIZE_XY (Vec2{SRC_TILE_SIZE,SRC_TILE_SIZE})

#define PIXELS_PER_TILE 16

#define TILE_PIXEL_XY (Vec2{PIXELS_PER_TILE,PIXELS_PER_TILE})

