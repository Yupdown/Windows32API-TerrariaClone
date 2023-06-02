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
#include "CTileLayer.h"

CScene_Start::CScene_Start()
{
	SetName(L"Scene_Start");
	const Vec2 vRes = Mgr(CCore)->GetResolutionV();
	
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_0.png",Vec2{0,0}, Vec2{ vRes.x,vRes.y * 10  }, Vec2{vRes.x,vRes.y * 10}, 10,1.f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_1.png",Vec2{0,vRes.y * 8.2f},vRes ,Vec2{ vRes.x,vRes.y * 10.f }, 10, 0.4f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_2.png",Vec2{0,vRes.y * 8.2f},vRes ,Vec2{ vRes.x,vRes.y * 10.f }, 10,0.6f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_3.png",Vec2{0,vRes.y * 8.2f},vRes ,Vec2{ vRes.x,vRes.y * 10.f }, 10,0.8f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_4.png",Vec2{0,vRes.y * 8.2f},vRes ,Vec2{ vRes.x,vRes.y * 10.f }, 10,1.f));
	const  Vec2 vGround = Vec2{ vRes.x,vRes.y * 1.5f };
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_5.png", Vec2{ 0,vRes.y * (8.5f)}, vGround, Vec2{vRes.x,vRes.y * 10}, 10, 1.f));


	//m_vecTileLayer.emplace_back(new CTileLayer{ Vec2{500,7300},2048,1024 });
	//for (int i = 0; i < 100; ++i) {
	//	m_vecTileLayer[0]->pre_render(L"Tiles_0000.png", Vec2{ 100 + (float)i * 20 ,500 }, Vec2{ 0,0 });
	//}
	//

	//m_vecTileLayer.emplace_back(new CTileLayer{ Vec2{500,7400},2048,1024 });
	//for (int i = 0; i < 100; ++i) {
	//	m_vecTileLayer[1]->pre_render(L"Tiles_0001.png", Vec2{ 100 + (float)i * 20 ,500 }, Vec2{ 0,0 });
	//}
	//

	//m_vecTileLayer.emplace_back(new CTileLayer{ Vec2{500,7500},2048,1024 });
	//for (int i = 0; i < 100; ++i) {
	//	m_vecTileLayer[2]->pre_render(L"Tiles_0002.png", Vec2{ 100 + (float)i * 20 ,500 }, Vec2{ 0,0 });
	//}
	
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	CScene::Enter();
	auto p = new CPlayer;
	p->SetPos(Vec2{ 100.0f, 100.0f });
	p->SetScale(Vec2{ 40.f, 56.f });
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
