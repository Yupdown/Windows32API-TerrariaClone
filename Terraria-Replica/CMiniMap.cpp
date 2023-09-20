#include "pch.h"
#include "CMiniMap.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CTileLayer.h"
#include "CLayer.h"
#include "CResMgr.h"
#include "CCamera.h"
#include "CKeyMgr.h"
#include "CSoundMgr.h"

CMiniMap* g_MiniMap = nullptr;
extern bool g_bStopToken;

CMiniMap::CMiniMap()
{
	g_MiniMap = this;
	//SetPos({ 1200,100 });
	//SetScale({ 264,264 });
	m_pMapFrameImg = Mgr(CResMgr)->GetImg(L"Minimap_Frame.png");
	while (!CreateDCBITMAP(m_hMinmapDC, m_hMimimapBit, Vec2{ 264,264 }) && !g_bStopToken) {
		DeleteDCBITMAP(m_hMinmapDC, m_hMimimapBit);
	}
	while (!CreateDCBITMAP(m_hMinmapDC2, m_hMimimapBit2, Vec2{ 264,264 }) && !g_bStopToken){
		DeleteDCBITMAP(m_hMinmapDC2, m_hMimimapBit2);
	}
	m_pMiniMapTileLayer = std::make_unique<CTileLayer>(Vec2{ 0,0 }, 8192, 4096);
}

CMiniMap::~CMiniMap()
{
	DeleteDCBITMAP(m_hMinmapDC, m_hMimimapBit);
	DeleteDCBITMAP(m_hMinmapDC2, m_hMimimapBit2);
}

void CMiniMap::update()
{
	if (KEY_TAP(KEY::M))
	{
		m_bMiniMapOn = !m_bMiniMapOn;
		if (m_bMiniMapOn)
		{
			Mgr(CSoundMgr)->PlayEffect("Menu_Open.wav", 0.5f);
		}
		else
		{
			Mgr(CSoundMgr)->PlayEffect("Menu_Close.wav", 0.5f);
		}
	}
}

void CMiniMap::render(HDC _dc) const
{
	if (!m_bMiniMapOn)
		return;

	auto pCurScene = Mgr(CSceneMgr)->GetCurScene();

	BitBlt(m_hMinmapDC
		, 5
		, 6
		, 264 - 10
		, 264 - 12
		, m_hMinmapDC2
		, 0
		, 0
		, SRCCOPY);

	TransparentBlt(m_hMinmapDC
		, 10
		, 10
		, 264 -20
		, 264 -20
		, m_pMiniMapTileLayer->GetTileLayerDC()
		, 0
		, 0
		, static_cast<int>(pCurScene->m_vecTileLayer.back()->GetScale().x)
		, static_cast<int>(pCurScene->m_vecTileLayer.back()->GetScale().y)
		, RGB(255, 0, 255));

	m_pMapFrameImg->TransparentBlt(m_hMinmapDC
		, 0
		, -1
		, 264
		, 264
		, 5
		, 5
		, 264 - 10
		, 264 - 10
		, RGB(255, 0, 255));

	auto& vec = pCurScene->m_vecObj;

	for (int i=0;i<etoi(GROUP_TYPE::END);++i)
	{
		if (GROUP_TYPE::UI == (GROUP_TYPE)(i) || GROUP_TYPE::PLAYER_WEAPON == (GROUP_TYPE)(i))
		{
			continue;
		}

		for (const auto& obj : vec[i])
		{
			const auto vPos = obj->GetPos();
			const auto vScale = obj->GetScale();
			Rectangle(m_hMinmapDC
				, (int)((vPos.x - vScale.x / 2.f) * 264.f / 8192.f) -1
				, (int)((vPos.y - vScale.y / 2.f) * 264.f / 4098.f) -1
				, (int)((vPos.x + vScale.x / 2.f) * 264.f / 8192.f) +1
				, (int)((vPos.y + vScale.y / 2.f) * 264.f / 4098.f) +1);
		}
	}

	auto hOld = SelectObject(m_hMinmapDC, GetStockObject(HOLLOW_BRUSH));
	const auto CamRect = Mgr(CCamera)->GetCamRect();

	//Rectangle(m_hMinmapDC
	//	, (int)(CamRect.vLT.x * 264.f / 8192.f)
	//	, (int)(CamRect.vLT.y * 264.f / 4098.f)
	//	, (int)(CamRect.vRB.x * 264.f / 8192.f)
	//	, (int)(CamRect.vRB.y * 264.f / 4098.f));

	SelectObject(m_hMinmapDC, hOld);

	TransparentBlt(_dc
		, 1125
		, 100
		, 264
		, 264
		, m_hMinmapDC
		, 0
		, 0
		, 264
		, 264
		, RGB(255, 0, 255));

	/*BitBlt(_dc
		, 1125
		, 100
		, 264
		, 264
		, m_hMinmapDC
		, 0
		, 0
		, SRCCOPY);*/
}

void CMiniMap::CreateMiniMap()
{
	if (g_bStopToken)
	{
		return;
	}

	auto pCurScene = Mgr(CSceneMgr)->GetScene(SCENE_TYPE::START);


	int cnt = -2;
	for (int i = 0; i < pCurScene->m_vecLayer.size(); ++i)
	{
		if (i == 0)
		{
			TransparentBlt(m_hMinmapDC2
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
			TransparentBlt(m_hMinmapDC2
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
		TransparentBlt(m_pMiniMapTileLayer->GetTileLayerDC()
			, 0
			, 0
			, static_cast<int>(layer->GetScale().x)
			, static_cast<int>(layer->GetScale().y)
			, layer->GetTileLayerDC()
			, 0
			, 0
			, static_cast<int>(layer->GetScale().x)
			, static_cast<int>(layer->GetScale().y)
			, RGB(255, 0, 255));
	}
}
