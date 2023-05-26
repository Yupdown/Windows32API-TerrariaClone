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


jthread CScene::m_renderThread[2];

CScene::CScene()
{
	CreateDCBITMAP(m_hSceneDC, m_hSceneBit, Mgr(CCore)->GetResolutionV());
	CreateDCBITMAP(m_hSceneDC2, m_hSceneBit2, Mgr(CCore)->GetResolutionV());
	CreateDCBITMAP(m_hSceneDC3, m_hSceneBit3, Mgr(CCore)->GetResolutionV());
}

CScene::~CScene()
{
	DeleteDCBITMAP(m_hSceneDC, m_hSceneBit);
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
	
	static const auto render_layer = [this]() {for (const auto& layer : m_vecLayer)
	{
		layer->render(m_hSceneDC);
	}};

	static const auto render_Tile = [this]() {for (const auto& tileVec : m_vecTileLayer)
	{
		tileVec->render(m_hSceneDC2);
	}};

	static const auto mazentaBlt1 = [this]() {Mgr(CCore)->MazentaBlt(m_hSceneDC2, Mgr(CCore)->GetResolutionV()); };
	static const auto mazentaBlt2 = [this]() {Mgr(CCore)->MazentaBlt(m_hSceneDC3, Mgr(CCore)->GetResolutionV()); };

	m_renderThread[0] = jthread{ render_layer };
	m_renderThread[1] = jthread{ render_Tile };
	
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
				vecPtr[i]->render(m_hSceneDC3);
				++i;
			}
		}
	};
	
	m_renderThread[1].join();
	
	TransparentBlt(m_hSceneDC2
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, m_hSceneDC3
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, RGB(255, 0, 255));

	m_renderThread[0].join();

	TransparentBlt(m_hSceneDC
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, m_hSceneDC2
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
			, m_hSceneDC
			, 0
			, 0
			,SRCCOPY);

	m_renderThread[0] = jthread{ mazentaBlt1 };
	m_renderThread[1] = jthread{ mazentaBlt2 };

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

