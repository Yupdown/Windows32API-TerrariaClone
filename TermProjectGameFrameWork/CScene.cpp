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
#include "CThreadMgr.h"

extern bool g_bDoMultiThread;

CScene::CScene()
{
	
	for (int i = 0; i < THREAD::END + 1; ++i)
	{
		CreateDCBITMAP(m_hSceneThreadDC[i], m_hSceneThreadBit[i], Mgr(CCore)->GetResolutionV());
		SetBkMode(m_hSceneThreadDC[i], TRANSPARENT);
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
	for (const auto& vecObj : m_vecObj)
	{
		const auto vecPtr = vecObj.data();
		for (size_t i = 0, size = vecObj.size(); i < size; ++i)
		{
			if (vecPtr[i]->IsDead())
			{
				continue;
			}
			vecPtr[i]->update();
		}
	}
}

void CScene::Enter()
{
	
}

void CScene::Exit()
{
	if (g_bDoMultiThread)
	{
		Mgr(CThreadMgr)->join_all();
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
	for (const auto& vecObj : m_vecObj)
	{
		const auto vecPtr = vecObj.data();
		for (size_t i = 0, size = vecObj.size(); i < size; ++i)
		{
			vecPtr[i]->component_update();
		}
	}
}

void CScene::render(HDC _dc)
{
	const Vec2 vRes = Mgr(CCore)->GetResolutionV();
	
	if (g_bDoMultiThread)
	{
		Mgr(CThreadMgr)->join_all();

		static const int size = (int)m_vecLayer.size();

		Mgr(CThreadMgr)->Enqueue(THREAD::T0, [this]() {for (int i = 0; i < 3; ++i)
		{
			m_vecLayer[i]->render(m_hSceneThreadDC[THREAD::T0]);
		}});

		Mgr(CThreadMgr)->Enqueue(THREAD::T1, [this]() {for (int i = 3; i < size; ++i)
		{
			m_vecLayer[i]->render(m_hSceneThreadDC[THREAD::T1]);
		}});

		Mgr(CThreadMgr)->Enqueue(THREAD::T2, [this]() {for (const auto& tileVec : m_vecTileLayer)
		{
			tileVec->render(m_hSceneThreadDC[THREAD::T2]);
		}});

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

		Mgr(CThreadMgr)->Join(THREAD::T2);

		Mgr(CThreadMgr)->Enqueue(THREAD::T2, TransparentBlt, m_hSceneThreadDC[THREAD::T2]
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

		Mgr(CThreadMgr)->Join(THREAD::T0);
		Mgr(CThreadMgr)->Join(THREAD::T1);

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

		Mgr(CThreadMgr)->Join(THREAD::T2);

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

		Mgr(CCamera)->SetNowLookAt(vRes / 2);

		Mgr(CCamera)->TransformRenderPos();

		BitBlt(_dc
			, 0
			, 0
			, (int)vRes.x
			, (int)vRes.y
			, m_hSceneThreadDC[THREAD::T0]
			, 0
			, 0
			, SRCCOPY);


		Mgr(CThreadMgr)->Enqueue(THREAD::T0, &CCore::MaznetaClear, Mgr(CCore), m_hSceneThreadDC[THREAD::T1], THREAD::T0);
		Mgr(CThreadMgr)->Enqueue(THREAD::T1, &CCore::MaznetaClear, Mgr(CCore), m_hSceneThreadDC[THREAD::T2], THREAD::T1);
		Mgr(CThreadMgr)->Enqueue(THREAD::T2, &CCore::MaznetaClear, Mgr(CCore), m_hSceneThreadDC[THREAD::END], THREAD::T2);

		Mgr(CCamera)->ResetRenderPos();
		Mgr(CCamera)->SetCamRect(GetPlayer()->GetPos());
	}
	else
	{
	
		for (auto& layer : m_vecLayer)
		{
			layer->render(m_hSceneThreadDC[1]);
		}

		for (auto& tilelayer : m_vecTileLayer)
		{
			tilelayer->render(m_hSceneThreadDC[1]);
		}

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
					vecPtr[i]->render(m_hSceneThreadDC[1]);
					++i;
				}
			}
		};

	
		TransparentBlt(m_hSceneThreadDC[0]
			, 0
			, 0
			, (int)vRes.x
			, (int)vRes.y
			, m_hSceneThreadDC[1]
			, 0
			, 0
			, (int)vRes.x
			, (int)vRes.y
			, RGB(255, 0, 255));

		Mgr(CCamera)->SetNowLookAt(vRes / 2.f );

		Mgr(CCamera)->TransformRenderPos();
		

		BitBlt(_dc
			, 0
			, 0
			, (int)vRes.x
			, (int)vRes.y
			, m_hSceneThreadDC[0]
			, 0
			, 0
			, SRCCOPY);
		
		Mgr(CCamera)->ResetRenderPos();
		Mgr(CCamera)->SetCamRect(GetPlayer()->GetPos());

	}
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

