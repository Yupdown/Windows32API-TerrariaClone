#include "pch.h"
#include "CDebugMgr.h"
#include "CCore.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CTileLayer.h"
#include "CCamera.h"

CDebugMgr::CDebugMgr()
{
}

CDebugMgr::~CDebugMgr()
{
	ReleaseDC(m_hWnd, m_hDC);
	DeleteDCBITMAP(m_hMemDC, m_hMemBit);
	DeleteDCBITMAP(m_hMemDC2, m_hMemBit2);
}

void CDebugMgr::init()
{
	m_hWnd = CreateWindow(L"DebugMgr", NULL, WS_OVERLAPPEDWINDOW,
		1000, 10, 400, 800, Mgr(CCore)->GetMainHwnd(), NULL, Mgr(CCore)->GethInst(), nullptr);

	RECT rt{ 0,0,static_cast<LONG>(400),static_cast<LONG>(800) };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW,false);
	SetWindowPos(m_hWnd, nullptr, 1000, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	ShowWindow(m_hWnd, SW_SHOW);

	m_hDC = GetDC(m_hWnd);
	m_hMemDC = CreateCompatibleDC(m_hDC);
	m_hMemBit = CreateCompatibleBitmap(m_hDC, 400, 800);
	DeleteObject(SelectObject(m_hMemDC, m_hMemBit));


	m_hDC = GetDC(m_hWnd);
	m_hMemDC2 = CreateCompatibleDC(m_hDC);
	m_hMemBit2 = CreateCompatibleBitmap(m_hDC, 400, 800);
	DeleteObject(SelectObject(m_hMemDC2, m_hMemBit2));

	auto pCurScene = Mgr(CSceneMgr)->GetCurScene();

	for (auto& layer : pCurScene->m_vecLayer)
	{
		TransparentBlt(m_hMemDC2
			, 0
			, 0
			, 400
			, 800
			, layer->GetLayerDC()
			, 0
			, 0
			, 1400
			, 8000
			, RGB(255, 0, 255));
	}

	
	for (auto& layer : pCurScene->m_vecTileLayer)
	{
		const Vec2 vLTpos = (layer->GetPos() -layer->GetScale() / 2.f);
		float yScale = min((8000.f - layer->GetScale().y), layer->GetScale().y);
		Vec2 vScale = layer->GetScale();
	
		TransparentBlt(m_hMemDC2
			, vLTpos.x * (400.f / 2800.f) 
			, vLTpos.y * (800.f / 8000.f) - 50.f 
			, 400
			, 800 
			, layer->GetTileLayerDC()
			, 0 
			, 0 
			, (int)layer->GetScale().x 
			, (int)yScale
			, RGB(255, 0, 255));
	}
}

void CDebugMgr::update()
{
}

void CDebugMgr::render()
{
	BitBlt(m_hMemDC
		, 0
		, 0
		, 400
		, 800
		, m_hMemDC2
		, 0
		, 0
		, SRCCOPY);

	auto pCurScene = Mgr(CSceneMgr)->GetCurScene();
	for (auto& vec : pCurScene->m_vecObj)
	{
		for (auto& obj : vec)
		{
			auto vPos = obj->GetPos();
			auto vScale = obj->GetScale();
			Rectangle(m_hMemDC
				,(int)((vPos.x - vScale.x/2.f) * 400.f / 2800.f)
				,(int)((vPos.y - vScale.y/2.f) * 800.f / 8000.f)
				,(int)((vPos.x + vScale.x/2.f) * 400.f / 2800.f)
				,(int)((vPos.y + vScale.y/2.f) * 800.f / 8000.f));
		}
	}
	auto hOld = SelectObject(m_hMemDC, GetStockObject(HOLLOW_BRUSH));
	const auto CamRect = Mgr(CCamera)->GetCamRect();

	Rectangle(m_hMemDC
		,(int)(CamRect.vLT.x * 400.f / 2800.f)
		,(int)(CamRect.vLT.y * 800.f / 8000.f)
		,(int)(CamRect.vRB.x * 400.f / 2800.f)
		,(int)(CamRect.vRB.y * 800.f / 8000.f));

	SelectObject(m_hMemDC, hOld);
	BitBlt(m_hDC
		, 0
		, 0
		, 400
		, 800
		, m_hMemDC
		, 0
		, 0
		, SRCCOPY);
}

void CDebugMgr::progress()
{
	while (true)
	{
		update();
		render();
	}
}
