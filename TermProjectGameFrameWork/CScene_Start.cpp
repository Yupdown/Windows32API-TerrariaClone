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
#include "CAtlasMgr.h"
#include "CAtlasElement.h"
#include "CLayer.h"

CScene_Start::CScene_Start()
{
	
	Vec2 vRes = Mgr(CCore)->GetResolutionV();
	m_pBackGroundImg = Mgr(CResMgr)->CreateImg(L"Start_Scene_BackGround", (int)vRes.x * 2, (int)vRes.y*10);
	m_pBackGroundImg2 = Mgr(CResMgr)->CreateImg(L"Start_Scene_BackGround2", (int)vRes.x * 2, (int)vRes.y * 10);
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
	auto hDC = m_pBackGroundImg->GetDC();
	m_vecTile.emplace_back(Mgr(CAtlasMgr)->GetAtlasElement(L"Tiles_0000.png",{0,0},{8,8}));
	m_vecTile.emplace_back(Mgr(CAtlasMgr)->GetAtlasElement(L"Tiles_0000.png", { 16,0 }, { 8,8 }));

	for (int i = 0; i < 100; ++i) {
		m_vecTile[0]->render(hDC, Vec2{ 100 + (float)i * 20 ,7000  });
	}
	for (int i = 0; i < 100; ++i) {
		m_vecTile[0]->render(hDC, Vec2{ vRes.x +(100 + (float)i * 20),7000  });
	}
	for (int i = 0; i < 100; ++i) {
		m_vecTile[1]->render(hDC, Vec2{ 100 ,7000 + (float)i * 20 });
	}
	for (int i = 0; i < 100; ++i) {
		m_vecTile[1]->render(hDC, Vec2{ vRes.x + 100 ,7000 + (float)i * 20 });
	}
	m_pBackGroundImg->ReleaseDC();

	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_0.png",Vec2{0,0}, Vec2{ vRes.x,vRes.y * 10 }, Vec2{vRes.x,vRes.y * 10}, 2,1.f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_1.png",Vec2{0,vRes.y*9},vRes ,Vec2{ vRes.x,vRes.y*10}, 2, 0.4f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_2.png",Vec2{0,vRes.y*9},vRes ,Vec2{ vRes.x,vRes.y * 10}, 2,0.6f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_3.png",Vec2{0,vRes.y*9},vRes ,Vec2{ vRes.x,vRes.y * 10}, 2,0.8f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_4.png",Vec2{0,vRes.y*9},vRes ,Vec2{ vRes.x,vRes.y * 10}, 2,1.f));
	m_vecLayer.emplace_back(CLayer::CreateTileLayer(L"Tile_Layer", Vec2{ 2048,2048 }));

	auto hDC2 = m_vecLayer.back()->GetLayerImg()->GetDC();
	for (int i = 0; i < 100; ++i) {
		m_vecTile[0]->render(hDC2, Vec2{ 100 + (float)i * 20 ,600 });
	}
	m_vecLayer.back()->GetLayerImg()->ReleaseDC();
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{

	auto p = new CPlayer;
	p->SetPos(Vec2{ 500., 7500. });
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
