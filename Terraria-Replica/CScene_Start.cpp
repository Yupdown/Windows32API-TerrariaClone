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
#include "TRMain.h"
#include "CEventMgr.h"
#include "TRWorld.h"
#include "TRTileManager.h"
#include "TRItemManager.h"


extern std::atomic<bool> g_bLoadMainStage;


CScene_Start::CScene_Start()
{
	SetName(L"Scene_Start");
}

CScene_Start::~CScene_Start()
{	
}

void CScene_Start::Enter()
{
	CScene::Enter();
	Mgr(CEventMgr)->SetTRupdate(&TRMain::Update, m_pTRMain);
	g_bLoadMainStage = false;
	Mgr(CSoundMgr)->PlayEffect("Menu_Close.wav", 1.f);
}

void CScene_Start::Exit()
{
	delete m_pTRMain;
	CScene::Exit();
	m_pTRMain = nullptr;
	m_bChangeScene = false;
}

void CScene_Start::update()
{
	CScene::update();
	if (KEY_TAP(KEY::RSHIFT) && !m_bChangeScene)
	{
		m_bChangeScene = true;
		g_bLoadMainStage.store(true, std::memory_order_seq_cst);
		ChangeScene(SCENE_TYPE::INTRO);
	}
}

void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);
}

void CScene_Start::CreateStage()
{
	const Vec2 vRes = Mgr(CCore)->GetResolutionV();
	float ground_level = 64 * PIXELS_PER_TILE;

	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_0.png", Vec2{ 0, -100 }, Vec2{ 1400, 1000 }, Vec2{ 1400, vRes.y * 10.f }, 10, 0.1f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_1.png", Vec2{ 0, vRes.y / 2.f - 200 }, Vec2{ 1048,435 }, Vec2{ 1048,vRes.y * 10.f }, 10, 0.3f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_2.png", Vec2{ 0, vRes.y / 2.f - 100 }, Vec2{ 1048,435 }, Vec2{ 1048,vRes.y * 10.f }, 10, 0.4f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_3.png", Vec2{ 0, vRes.y / 2.f }, Vec2{ 1048,435 }, Vec2{ 1048,vRes.y * 10.f }, 10, 0.5f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_4.png", Vec2{ 0, vRes.y / 2.f + 100 }, Vec2{ 1048,600 }, Vec2{ 1048,vRes.y * 10.f }, 10, 0.6f));
	m_vecLayer.emplace_back(CLayer::CreateLayer(L"Background_5.png", Vec2{ 0,vRes.y / 2.f + 100 + 600 }, Vec2{ 1024,600 }, Vec2{ vRes.x,vRes.y * 10 }, 10, 1.f));
}

void CScene_Start::LoadWorld()
{
	CScene::Enter();
	std::atomic_thread_fence(std::memory_order_seq_cst);
	// 纠 积己
	CreateStage();




	//  TR岿靛 积己 
	TRMain* terraria_main = new TRMain();
	m_pTRMain = terraria_main;

	std::atomic_thread_fence(std::memory_order_seq_cst);
	//std::this_thread::sleep_for(std::chrono::seconds(3));
	g_bLoadMainStage = false;
	Mgr(CSoundMgr)->PlayBGM("03. Overworld Day.mp3", 0.1f);
	ChangeScene(SCENE_TYPE::START);
}
