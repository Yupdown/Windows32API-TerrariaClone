#include "pch.h"
#include "CScene_Start.h"
#include "CPlayer.h"
#include "CCollisionMgr.h"
#include "CCamera.h"
#include "CMonster.h"
#include "CKeyMgr.h"
#include "CTile.h"
#include "CTexture.h"
#include "CCore.h"
#include "CResMgr.h"

CScene_Start::CScene_Start()
{
	
	Vec2 vRes = Mgr(CCore)->GetResolutionV();
	m_pBackGroundImg = Mgr(CResMgr)->CreateImg(L"Start_Scene_BackGround", (int)vRes.x*10, (int)vRes.y*10);
	m_pBackGroundImg2 = Mgr(CResMgr)->CreateImg(L"Start_Scene_BackGround2", (int)vRes.x * 10, (int)vRes.y * 10);
	m_vecSceneLayer.resize(5);
	m_vecSceneLayer[0] = Mgr(CResMgr)->GetImg(L"Background_0.png");
	m_vecSceneLayer[1] = Mgr(CResMgr)->GetImg(L"Background_1.png");
	m_vecSceneLayer[2] = Mgr(CResMgr)->GetImg(L"Background_2.png");
	m_vecSceneLayer[3] = Mgr(CResMgr)->GetImg(L"Background_3.png");
	m_vecSceneLayer[4] = Mgr(CResMgr)->GetImg(L"Background_4.png");


	for (int i = 0; i < 2; ++i)
	{
		m_vecSceneLayer[0]->TransparentBlt(m_pBackGroundImg->GetDC()
			, (int)vRes.x * i
			, 0
			, (int)vRes.x
			, (int)vRes.y * 10
		);
		m_pBackGroundImg->ReleaseDC();
	}

	for (int i = 1; i < m_vecSceneLayer.size(); ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			m_vecSceneLayer[i]->TransparentBlt(m_pBackGroundImg->GetDC()
				, (int)vRes.x * j
				, (int)vRes.y * 9
				, (int)vRes.x
				, (int)vRes.y
			);
			m_pBackGroundImg->ReleaseDC();
		}
	}
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{

	auto p = new CPlayer;
	p->SetPos(Vec2{ 1000., 7500. });
	p->SetScale(Vec2{ 30., 30. });
	AddObject(p, GROUP_TYPE::PLAYER);
	Mgr(CCamera)->SetTarget(p);
	RegisterPlayer(p);
	/*for (int i = 0; i < 100 ;++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			AddTile(L"Bullet.bmp", Vec2{ 0 + (double)j * 10,0 + (double)i * 10 }, Vec2{ 10,10 }, Vec2{ 0,0 }, Vec2{ 64,64 });
		}
	}
	
	for (int i = 0; i < 1000; ++i) {
		auto pMon = new CMonster;
		AddObject(pMon, GROUP_TYPE::MONSTER);
	}
	Mgr(CCollisionMgr)->RegisterGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);*/
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
}
