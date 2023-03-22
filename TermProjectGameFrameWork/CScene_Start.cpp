#include "pch.h"
#include "CScene_Start.h"
#include "CPlayer.h"
#include "CCollisionMgr.h"
#include "CCamera.h"
#include "CMonster.h"
#include "CKeyMgr.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	auto p = new CPlayer;
	p->SetPos(Vec2{ 100., 100. });
	p->SetScale(Vec2{ 150., 150. });
	AddObject(p, GROUP_TYPE::PLAYER);

	auto pMon = new CMonster;
	AddObject(pMon, GROUP_TYPE::MONSTER);
	/*auto a = new CPlayer;
	a->SetPos(Vec2{ 150., 150. });
	a->SetScale(Vec2{ 100., 100. });
	AddObject(a, GROUP_TYPE::PLAYER);

	Mgr(CCollisionMgr)->RegisterCollision(p, a);*/
	//Mgr(CCamera)->SetTarget(p);
	Mgr(CCollisionMgr)->RegisterCollision(p, pMon);
}

void CScene_Start::Exit()
{
}

void CScene_Start::update()
{
	CScene::update();

	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vLook = Mgr(CCamera)->GetRealPos(MOUSE_POS);
		Mgr(CCamera)->SetLookAt(vLook);
	}
}
