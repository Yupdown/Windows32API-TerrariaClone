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
#include "CSoundMgr.h"

CScene_Start::CScene_Start()
{
	//Mgr(CSoundMgr)->PlayBGM("Test.mp3", 0.1f);
	SetName(L"Scene_Start");
	const Vec2 vRes = Mgr(CCore)->GetResolutionV();
	float ground_level = 64 * PIXELS_PER_TILE;
	
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_0.png",Vec2{ 0, 0 }, Vec2{ vRes.x,vRes.y * 10.f },Vec2{ vRes.x,vRes.y * 10.f }, 10,0.1f));
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_1.png",Vec2{ 0, ground_level },vRes,Vec2{ vRes.x,vRes.y * 10.f }, 10,0.3f));
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_2.png",Vec2{ 0, ground_level },vRes,Vec2{ vRes.x,vRes.y * 10.f }, 10,0.4f));
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_3.png",Vec2{ 0, ground_level },vRes,Vec2{ vRes.x,vRes.y * 10.f }, 10,0.5f));
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_4.png",Vec2{ 0, ground_level },vRes,Vec2{ vRes.x,vRes.y * 10.f }, 10,0.6f));
	//const  Vec2 vGround = Vec2{ vRes.x,vRes.y * 1.5f };
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_5.png", Vec2{ 0,vRes.y * (8.5f)}, vGround, Vec2{vRes.x,vRes.y * 10}, 10, 1.f));
	
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_0.png", Vec2{ 0, 0 }, Vec2{ vRes.x,vRes.y * 10.f }, Vec2{ vRes.x,vRes.y * 10.f }, 10, 0.1f));
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_1.png", Vec2{ 0, vRes.y / 2.f -200 }, Vec2{1048,435}, Vec2{ 1048,vRes.y * 10.f  }, 10, 0.3f));
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_2.png", Vec2{ 0, vRes.y / 2.f - 100}, Vec2{1048,435}, Vec2{ 1048,vRes.y * 10.f  }, 10, 0.4f));
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_3.png", Vec2{ 0, vRes.y / 2.f }, Vec2{ 1048,435 }, Vec2{ 1048,vRes.y * 10.f }, 10, 0.5f));
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_4.png", Vec2{ 0, vRes.y / 2.f + 100 }, Vec2{1048,600}, Vec2{ 1048,vRes.y * 10.f  }, 10, 0.6f));
	////const  Vec2 vGround = Vec2{ vRes.x,vRes.y * 1.5f };
	//m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_5.png", Vec2{ 0,vRes.y / 2.f + 100 + 600 }, Vec2{1024,600}, Vec2{ vRes.x,vRes.y * 10 }, 10, 1.f));

	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_0.png", Vec2{ 0, 0 }, Vec2{ vRes.x,vRes.y * 10.f }, Vec2{ vRes.x,vRes.y * 10.f }, 10, 0.1f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_1.png", Vec2{ 0, vRes.y / 2.f - 200 }, Vec2{ 1048,435 }, Vec2{ 1048,vRes.y * 10.f }, 10, 0.3f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_2.png", Vec2{ 0, vRes.y / 2.f - 100 }, Vec2{ 1048,435 }, Vec2{ 1048,vRes.y * 10.f }, 10, 0.4f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_3.png", Vec2{ 0, vRes.y / 2.f }, Vec2{ 1048,435 }, Vec2{ 1048,vRes.y * 10.f }, 10, 0.5f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_4.png", Vec2{ 0, vRes.y / 2.f + 100 }, Vec2{ 1048,600 }, Vec2{ 1048,vRes.y * 10.f }, 10, 0.6f));
	//const  Vec2 vGround = Vec2{ vRes.x,vRes.y * 1.5f };
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_5.png", Vec2{ 0,vRes.y / 2.f + 100 + 600 }, Vec2{ 1024,600 }, Vec2{ vRes.x,vRes.y * 10 }, 10, 1.f));

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
}

void CScene_Start::Exit()
{
}

void CScene_Start::update()
{
	CScene::update();
}

void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);
}
