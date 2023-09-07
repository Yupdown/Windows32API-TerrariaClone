#include "pch.h"
#include "CScene_Intro.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CPlayer.h"
#include "CAnimator.h"
#include "CSceneMgr.h"
#include "CScene_Start.h"
#include "CCore.h"
#include "CSoundMgr.h"

std::atomic<bool> g_bLoadMainStage = false;
jthread g_LoadThread;
static HPEN loadingPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));

CScene_Intro::CScene_Intro()
{
	SetName(L"CScene_Intro");
	CreateDCBITMAP(m_hIntroDC, m_hIntroBit,Vec2{200.f,100.f});
	SetBkMode(m_hIntroDC, TRANSPARENT);
	DeleteObject(SelectObject(m_hIntroDC, loadingPen));
}

CScene_Intro::~CScene_Intro()
{
	DeleteDCBITMAP(m_hIntroDC, m_hIntroBit);
	DeleteObject(loadingPen);
}

void CScene_Intro::Enter()
{
	CScene::Enter();
	m_pIntroImg = Mgr(CResMgr)->GetImg(L"Intro.png");
	m_pIntroLogo = Mgr(CResMgr)->GetImg(L"Intro_Logo.png");
	m_pPlayer = make_unique<CPlayer>(nullptr);
	m_pPlayer->SetScale(Vec2{ 60.0f, 84.0f });
	m_pPlayer->SetPos(Vec2{ 100,600 });
	auto pAnim = m_pPlayer->GetComp<CAnimator>();
	pAnim->Play(L"Player_Torso_WALK", true);
	m_pPlayer->m_pAnimLeg->Play(L"Player_Leg_WALK", true);

	m_fScrollSpeed = 0.f;
	m_iImageLoopPos = 0;

	m_fAccTime = 0.f;
	m_bTurn = true;

	m_fDir = 1.f;
	m_bLoading = false;

	g_bLoadMainStage = false;

	Mgr(CSoundMgr)->PlayBGM("05. Title Classic.mp3", 0.5f);
}

void CScene_Intro::Exit()
{
	CScene::Exit();
	if (g_LoadThread.joinable())
	{
		g_LoadThread.join();
	}
	Reset();
}

void CScene_Intro::update()
{
	m_pPlayer->component_update();
	auto vPos = m_pPlayer->GetPos();

	if (vPos.x >= 1350.f)
	{
		m_fDir *= -1.f;
		m_pPlayer->GetComp<CAnimator>()->SetAnimLeft();
		m_pPlayer->m_pAnimLeg->SetAnimLeft();
	}

	if (vPos.x <= 90.f)
	{
		m_fDir *= -1.f;
		m_pPlayer->GetComp<CAnimator>()->SetAnimRight();
		m_pPlayer->m_pAnimLeg->SetAnimRight();
	}
	m_pPlayer->SetPos(vPos + Vec2{300.f, 0.f} *m_fDir* DT);

	if (KEY_TAP(KEY::ENTER) && !m_bLoading)
	{
		m_bLoading = true;
		g_bLoadMainStage.store(true, std::memory_order_seq_cst);
		g_LoadThread = jthread{ []() {
			CScene_Start* pStart = (CScene_Start*)Mgr(CSceneMgr)->GetScene(SCENE_TYPE::START);
			pStart->LoadWorld();
		} };
		Mgr(CSoundMgr)->PlayEffect("Menu_Open.wav", 0.5f);
	}

	m_fScrollSpeed += 100.f * DT;
	m_iImageLoopPos = (int)m_fScrollSpeed;

	m_fAccTime += DT;
	if (0.5f <= m_fAccTime)
	{
		m_fAccTime = 0.f;
		m_bTurn = !m_bTurn;
	}
}

void CScene_Intro::render(HDC _dc)
{
	const int iCurSize = m_iImageLoopPos + (int)m_vRes.x;
	const int iImageWidth = m_pIntroImg->GetWidth();

	if (m_iImageLoopPos >= iImageWidth)
	{
		m_iImageLoopPos = 0;
		m_fScrollSpeed = 0.f;
	}

	if (iCurSize < iImageWidth)
	{
		BitBlt(_dc
			, 0
			, 0
			, (int)m_vRes.x
			, (int)m_vRes.y
			, m_pIntroImg->GetDC()
			, m_iImageLoopPos
			, 0
			, SRCCOPY);
		m_pIntroImg->ReleaseDC();
	}
	else
	{
		BitBlt(_dc
			, 0
			, 0
			, iImageWidth - m_iImageLoopPos
			, (int)m_vRes.y
			, m_pIntroImg->GetDC()
			, m_iImageLoopPos
			, 0
			, SRCCOPY);

		m_pIntroImg->ReleaseDC();

		BitBlt(_dc
			, iImageWidth - m_iImageLoopPos
			, 0
			, (int)m_vRes.x - iImageWidth + m_iImageLoopPos
			, (int)m_vRes.y
			, m_pIntroImg->GetDC()
			, 0
			, 0
			, SRCCOPY);
		m_pIntroImg->ReleaseDC();
	}
	
	m_pIntroLogo->TransparentBlt(_dc
		, 420
		, 100
		, 550
		, 150
		, 50
		, 10
		, 600 - 50
		, 160 - 10
		, RGB(176, 176, 176));

	if (m_bTurn && !m_bLoading)
	{
		Mgr(CCore)->MazentaBlt(m_hIntroDC, Vec2{ 200.f,100.f });
		renderText(m_hIntroDC, RGB(255, 255, 255), Vec2{ 0,0 }, L"Press Enter Key");

		TransparentBlt(_dc
			, 550
			, 400
			, 500
			, 300
			, m_hIntroDC
			, 0
			, 0
			, 200
			, 100
			, RGB(255, 0, 255));
	}
	if (!m_bLoading)
	{
		m_pPlayer->render(_dc);
	}
	if (m_bTurn && m_bLoading)
	{
		Mgr(CCore)->MazentaBlt(m_hIntroDC, Vec2{ 200.f,100.f });
		renderText(m_hIntroDC, RGB(255, 255, 255), Vec2{ 0,0 }, L"NOW LOADING ...");

		TransparentBlt(_dc
			, 550
			, 400
			, 500
			, 300
			, m_hIntroDC
			, 0
			, 0
			, 200
			, 100
			, RGB(255, 0, 255));
	}
	if (!m_bTurn && m_bLoading)
	{
		Mgr(CCore)->MazentaBlt(m_hIntroDC, Vec2{ 200.f,100.f });
		renderText(m_hIntroDC, RGB(255, 255, 255), Vec2{ 0,0 }, L"NOW LOADING ..");

		TransparentBlt(_dc
			, 550
			, 400
			, 500
			, 300
			, m_hIntroDC
			, 0
			, 0
			, 200
			, 100
			, RGB(255, 0, 255));
	}
}
