#include "pch.h"
#include "CDebugMgr.h"
#include "CCore.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CTileLayer.h"
#include "CCamera.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CThreadMgr.h"

extern bool g_bStopToken;

extern LRESULT CALLBACK MiniMapProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void miniMapWin();

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
	/*m_hWnd = CreateWindow(L"DebugMgr", NULL, WS_POPUP,
		1050, 50, 264-15, 264-17, Mgr(CCore)->GetMainHwnd(), NULL, Mgr(CCore)->GethInst(), nullptr);*/
	m_hWnd = CreateWindow(L"DebugMgr", NULL, WS_POPUP,
		1120, 120, 264 - 10 , 264 - 10  , Mgr(CCore)->GetMainHwnd(), NULL, Mgr(CCore)->GethInst(), nullptr);
	//m_hWnd = CreateWindow(L"DebugMgr", L"ChildClass", WS_CHILD | WS_VISIBLE, 100, 0, 720, 720, Mgr(CCore)->GetMainHwnd(), NULL, Mgr(CCore)->GethInst(), MiniMapProc);

	//RECT rt{ 0,0,static_cast<LONG>(300),static_cast<LONG>(300) };
	//AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW,false);
	//SetWindowPos(m_hWnd, nullptr, 1000, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	//ShowWindow(m_hWnd, SW_SHOW);

	m_hDC = GetDC(m_hWnd);
	m_hMemDC = CreateCompatibleDC(m_hDC);
	m_hMemBit = CreateCompatibleBitmap(m_hDC, 264 , 264 );
	DeleteObject(SelectObject(m_hMemDC, m_hMemBit));


	m_hMemDC2 = CreateCompatibleDC(m_hDC);
	m_hMemBit2 = CreateCompatibleBitmap(m_hDC, 264, 264 );
	DeleteObject(SelectObject(m_hMemDC2, m_hMemBit2));


	SetStretchBltMode(m_hMemDC, HALFTONE);
	SetStretchBltMode(m_hMemDC2, HALFTONE);

	auto pCurScene = Mgr(CSceneMgr)->GetCurScene();
	
	//Vec2{ 0, vRes.y / 2.f - 200 };
	//Vec2{ 0, vRes.y / 2.f - 100 };
	//Vec2{ 0, vRes.y / 2.f };
	//Vec2{ 0, vRes.y / 2.f + 100 };
	//int cnt = -3;
	//for (auto& layer : pCurScene->m_vecLayer)
	//{
	//	TransparentBlt(m_hMemDC2
	//		, 0
	//		, (400.f + cnt * 100) * 200.f / 4096.f
	//		, 400
	//		, 200
	//		, layer->GetLayerDC()
	//		, 0
	//		, 0
	//		, 8192
	//		, 4096//4096
	//		, RGB(255, 0, 255));
	//	++cnt;
	//}

	int cnt = -2;
	for (int i = 0; i < pCurScene->m_vecLayer.size(); ++i)
	{
		if (i == 0)
		{
			TransparentBlt(m_hMemDC2
				, 0 
				, 0
				, 264 
				, 264
				, pCurScene->m_vecLayer[i]->GetLayerDC()
				, 0
				, 0
				, 8192
				, 8000
				, RGB(255, 0, 255));
		}
		else
		{
			TransparentBlt(m_hMemDC2
				, 0 
				, (int)((400.f + float(cnt) * 100.f) * 264.f / 4096.f) 
				, 264
				, 264
				, pCurScene->m_vecLayer[i]->GetLayerDC()
				, 0
				, 0
				, 8192
				, 4096
				, RGB(255, 0, 255));
			++cnt;
		}
	}
	
	for (auto& layer : pCurScene->m_vecTileLayer)
	{
		const Vec2 vLTpos = (layer->GetPos() - layer->GetScale() / 2.f);
		float scaleX = layer->GetScale().x * (264.f / 8192.f);
		float scaleY = layer->GetScale().y * (264.f / 4096.f);

		TransparentBlt(m_hMemDC2
			, 0 
			, 0 
			, 264 
			, 264
			, layer->GetTileLayerDC()
			, 0
			, 0
			, static_cast<int>(layer->GetScale().x)
			, static_cast<int>(layer->GetScale().y) 
			, RGB(255, 0, 255));
	}

	
	StretchBlt(m_hMemDC
		, 0 
		, 0 
		, 264
		, 264
		, m_hMemDC2
		, 10
		, 10
		, 264
		, 264
		, SRCCOPY);

	m_pMinimapImg = Mgr(CResMgr)->GetImg(L"Minimap_Frame.png");

	m_pTileLayer = std::make_unique<CTileLayer>(Vec2{ 0,0 }, 8192, 4096);

	for (auto& layer : pCurScene->m_vecTileLayer)
	{
		TransparentBlt(m_pTileLayer->GetTileLayerDC()
			, 0
			, 0
			, 8192
			, 4096
			, layer->GetTileLayerDC()
			, 0
			, 0
			, 8192
			, 4096
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
		, 264 - 10
		, 264 - 10
		, m_hMemDC2
		, 0
		, 0
		, SRCCOPY);

	auto pCurScene = Mgr(CSceneMgr)->GetCurScene();

	TransparentBlt(m_hMemDC
		, 0
		, 0
		, 264
		, 264
		, m_pTileLayer->GetTileLayerDC()
		, 0
		, 0
		, static_cast<int>(m_pTileLayer->GetScale().x)
		, static_cast<int>(m_pTileLayer->GetScale().y)
		, RGB(255, 0, 255));

	m_pMinimapImg->TransparentBlt(m_hMemDC
		, 0
		, 0
		, 264
		, 264
		, 10
		, 10
		, 264 - 10
		, 264 - 10
		, RGB(255, 0, 255));

//	auto pCurScene = Mgr(CSceneMgr)->GetCurScene();

	for (auto& vec : pCurScene->m_vecObj)
	{
		for (auto& obj : vec)
		{
			auto vPos = obj->GetPos();
			auto vScale = obj->GetScale();
			Rectangle(m_hMemDC
				,(int)((vPos.x - vScale.x/2.f) * 264.f / 8192.f)-2
				,(int)((vPos.y - vScale.y/2.f) * 264.f / 4098.f)-2
				,(int)((vPos.x + vScale.x/2.f) * 264.f / 8192.f)+2
				,(int)((vPos.y + vScale.y/2.f) * 264.f / 4098.f)+2);
		}
	}
	auto hOld = SelectObject(m_hMemDC, GetStockObject(HOLLOW_BRUSH));
	const auto CamRect = Mgr(CCamera)->GetCamRect();

	Rectangle(m_hMemDC
		,(int)(CamRect.vLT.x * 264.f / 8192.f)
		,(int)(CamRect.vLT.y * 264.f / 4098.f )
		,(int)(CamRect.vRB.x * 264.f / 8192.f)
		,(int)(CamRect.vRB.y * 264.f / 4098.f ));

	SelectObject(m_hMemDC, hOld);


	BitBlt(m_hDC
		, 0
		, 0
		, 264 - 10
		, 264 - 10
		, m_hMemDC
		, 0
		, 0
		, SRCCOPY);
}

void CDebugMgr::progress()
{
	if (!g_bStopToken)
	{
		update();
		render();
	}
}

void miniMapWin()
{
	MSG msg;

	while (true)
	{
		if (g_bStopToken)
		{
			break;
		}

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
			{
				break;
			}
			
		TranslateMessage(&msg);
		DispatchMessage(&msg);
			
		}
		else
		{
			Mgr(CDebugMgr)->progress();
		}
	}
}