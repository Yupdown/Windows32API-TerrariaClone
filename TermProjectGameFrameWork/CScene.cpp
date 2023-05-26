#include "pch.h"
#include "CScene.h"
#include "CObject.h"
#include "CTile.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CCore.h"
#include "CCamera.h"
#include "CEventMgr.h"
#include "CLayer.h"
#include "CTileLayer.h"


jthread CScene::g_renderThread[3];

CScene::CScene()
{
	
	for (int i = 0; i < THREAD::END + 1; ++i)
	{
		CreateDCBITMAP(m_hSceneThreadDC[i], m_hSceneThreadBit[i], Mgr(CCore)->GetResolutionV());
	}

}

CScene::~CScene()
{
	for (int i = 0; i < THREAD::END + 1; ++i)
	{
		DeleteDCBITMAP(m_hSceneThreadDC[i], m_hSceneThreadBit[i]);
	}
}

void CScene::update()
{
	for (const auto& i : m_vecObj)
	{
		for (auto& j : i)
		{
			if (j->IsDead())
			{
				continue;
			}
			j->update();
		}
	}
}

void CScene::Enter()
{
	g_renderThread[THREAD::T0] = jthread{ &CCore::MaznetaClear,Mgr(CCore),m_hSceneThreadDC[THREAD::T1],0 };
	g_renderThread[THREAD::T1] = jthread{ &CCore::MaznetaClear,Mgr(CCore),m_hSceneThreadDC[THREAD::T2],1 };
	g_renderThread[THREAD::T2] = jthread{ &CCore::MaznetaClear,Mgr(CCore),m_hSceneThreadDC[THREAD::END],2 };
}

void CScene::Exit()
{
	for (auto& t : g_renderThread)
	{
		t.join();
	}
}

void CScene::AddTileLayer(CTileLayer* const _pTileLayer)
{
	m_vecTileLayer.emplace_back(_pTileLayer);
}

void CScene::AddObject(CObject* const _pObj, GROUP_TYPE _eType)
{
	m_vecObj[(UINT)_eType].emplace_back(_pObj); 
}

const vector<unique_ptr<CObject>>& CScene::GetGroupObject(GROUP_TYPE _eType) const
{
	return m_vecObj[(UINT)_eType];
}

vector<unique_ptr<CObject>>& CScene::GetUIGroup()
{
	return m_vecObj[(UINT)GROUP_TYPE::UI];
}

void CScene::component_update()const
{
	for (const auto& i : m_vecObj)
	{
		for (auto& j : i)
		{
			j->component_update();
		}
	}
}

void CScene::render(HDC _dc)
{
	const Vec2 vRes = Mgr(CCore)->GetResolutionV();
	
	
	static const auto render_layer1 = [this]() {for (int i = 0; i < 3; ++i)
	{
		m_vecLayer[i]->render(m_hSceneThreadDC[THREAD::T0]);
	}};

	
	static const int size = (int)m_vecLayer.size();
	static const auto render_layer2 = [this]() {for (int i = 3; i < size; ++i)
	{
		m_vecLayer[i]->render(m_hSceneThreadDC[THREAD::T1]);
	}};

	static const auto render_Tile = [this]() {for (const auto& tileVec : m_vecTileLayer)
	{
		tileVec->render(m_hSceneThreadDC[THREAD::T2]);
	}};

	static const auto mazentaClear1 = [this]() {Mgr(CCore)->MaznetaClear(m_hSceneThreadDC[THREAD::T1], THREAD::T0); };
	static const auto mazentaClear2 = [this]() {Mgr(CCore)->MaznetaClear(m_hSceneThreadDC[THREAD::T2], THREAD::T1); };
	static const auto mazentaClear3 = [this]() {Mgr(CCore)->MaznetaClear(m_hSceneThreadDC[THREAD::END], THREAD::T2); };

	g_renderThread[THREAD::T0] = jthread{ render_layer1 };
	g_renderThread[THREAD::T1] = jthread{ render_layer2 };
	g_renderThread[THREAD::T2] = jthread{ render_Tile };
	
	for (auto& vecObj : m_vecObj)
	{
		const auto vecPtr = vecObj.data();
		for (size_t i = 0; i < vecObj.size();)
		{
			if (vecPtr[i]->IsDead())
			{
				Mgr(CEventMgr)->AddDeadObj(vecPtr[i]);
				vecPtr[i] = std::move(vecObj.back());
				vecObj.pop_back();
			}
			else
			{
				vecPtr[i]->render(m_hSceneThreadDC[THREAD::END]);
				++i;
			}
		}
	};
	
	g_renderThread[THREAD::T2].join();
	
	TransparentBlt(m_hSceneThreadDC[THREAD::T2]
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, m_hSceneThreadDC[THREAD::END]
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, RGB(255, 0, 255));

	g_renderThread[THREAD::T0].join();
	g_renderThread[THREAD::T1].join();

	TransparentBlt(m_hSceneThreadDC[THREAD::T0]
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, m_hSceneThreadDC[THREAD::T1]
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, RGB(255, 0, 255));

	TransparentBlt(m_hSceneThreadDC[THREAD::T0]
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, m_hSceneThreadDC[THREAD::T2]
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, RGB(255, 0, 255));

	Mgr(CCamera)->SetNowLookAt(vRes/2);

	Mgr(CCamera)->TransformRenderPos();

	BitBlt(_dc
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, m_hSceneThreadDC[THREAD::T0]
		, 0
		, 0
		,SRCCOPY);


	g_renderThread[THREAD::T0] = jthread{ mazentaClear1 };
	g_renderThread[THREAD::T1] = jthread{ mazentaClear2 };
	g_renderThread[THREAD::T2] = jthread{ mazentaClear3 };

	
	Mgr(CCamera)->ResetRenderPos();
	Mgr(CCamera)->SetNowLookAt(GetPlayer()->GetPos());
}

void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	m_vecObj[etoi(_eTarget)].clear();
}

void CScene::Reset()
{
	for (auto& i : m_vecObj)
	{
		i.clear();
	}
}

