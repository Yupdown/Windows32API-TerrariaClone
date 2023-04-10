#include "pch.h"
#include "CScene_Start.h"
#include "CPlayer.h"
#include "CCollisionMgr.h"
#include "CCamera.h"
#include "CMonster.h"
#include "CKeyMgr.h"
#include "CTile.h"
#include "CTexture.h"

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
	Mgr(CCamera)->SetTarget(p);
	for (int i = 0; i < 100 ;++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			AddTile(L"Bullet.bmp", Vec2{ 0 + (double)j * 10,0 + (double)i * 10 }, Vec2{ 10,10 }, Vec2{ 0,0 }, Vec2{ 64,64 });
		}
	}
	
	for (int i = 0; i < 100; ++i) {
		auto pMon = new CMonster;
		AddObject(pMon, GROUP_TYPE::MONSTER);
	}
	Mgr(CCollisionMgr)->RegisterGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	//auto a = new CPlayer;
//	pMon->SetPos(Vec2{ 150., 150. });
//	pMon->SetScale(Vec2{ 100., 100. });
	//AddObject(pMon, GROUP_TYPE::MONSTER);

	//Mgr(CCollisionMgr)->RegisterCollision(p, a);
	//Mgr(CCamera)->SetTarget(p);
	//Mgr(CCollisionMgr)->RegisterCollision(p, pMon);
}

void CScene_Start::Exit()
{
}

void CScene_Start::update()
{
	CScene::update();
	Vec2 vmouse = MOUSE_POS;
	POINT mouse = vmouse;
	if (KEY_TAP(KEY::LBTN))
	{
		for (auto& [key, val] : m_mapTile)
		{
			//val->DeleteTile(m_pBackGroundTex->GetDC());
			RECT rt{};
			Vec2 vPos = val->GetPos();
			Vec2 vScale = val->GetScale();
			Vec2 vLt = vPos - vScale / 2;
			Vec2 vRb = vPos + vScale / 2;
			rt.left = vLt.x;
			rt.top = vLt.y;
			rt.right = vRb.x;
			rt.bottom = vRb.y;
			if (PtInRect(&rt, mouse))
			{
				val->DeleteTile(m_pBackGroundTex->GetDC());
			}
		}

	}
}
