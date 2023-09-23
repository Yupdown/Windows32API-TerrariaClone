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
extern std::atomic<bool> g_particleWait;
extern HDC g_particleDC;
static vector<std::future<void>> g_vecDrawCall;

CScene::CScene()
{
	g_vecDrawCall.reserve(1000);
	for (int i = 0; i < THREAD::END + 1; ++i)
	{
		CreateDCBITMAP(m_hSceneThreadDC[i], m_hSceneThreadBit[i], Mgr(CCore)->GetResolutionV());
		SetBkMode(m_hSceneThreadDC[i], TRANSPARENT);
	}
	m_vRes = Mgr(CCore)->GetResolutionV();
}

CScene::~CScene()
{
	for (int i = 0; i < THREAD::END + 1; ++i)
	{
		DeleteDCBITMAP(m_hSceneThreadDC[i], m_hSceneThreadBit[i]);
	}
	Reset();
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
			//Mgr(CThreadMgr)->EnqueueUpdate(&CObject::update, vecPtr[i].get());
		}
	}
	//Mgr(CThreadMgr)->JoinUpdate();
}

void CScene::Enter()
{
	if (g_bDoMultiThread)
	{
		Mgr(CThreadMgr)->Join_all();
		Mgr(CThreadMgr)->JoinUpdate();
	}
}

void CScene::Exit()
{
	if (g_bDoMultiThread)
	{
		Mgr(CThreadMgr)->Join_all();
		Mgr(CThreadMgr)->JoinUpdate();
	}
	Reset();
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
			//Mgr(CThreadMgr)->EnqueueUpdate(&CObject::component_update, vecPtr[i].get());
		}
	}
	//Mgr(CThreadMgr)->JoinUpdate();
}

void CScene::render(HDC _dc)
{
	const Vec2 vRes = Mgr(CCore)->GetResolutionV();
	
	if (g_bDoMultiThread)
	{
		Mgr(CThreadMgr)->Join_all();

		std::atomic_thread_fence(std::memory_order_seq_cst);

		static const int size = (int)m_vecLayer.size();

		Mgr(CThreadMgr)->Enqueue(THREAD::T0, [this]() {for (int i = 0; i < 3; ++i)
		{
			m_vecLayer[i]->render(m_hSceneThreadDC[THREAD::T0]);
		}});

		Mgr(CThreadMgr)->Enqueue(THREAD::T1, [this]() {for (int i = 3; i < size; ++i)
		{
			m_vecLayer[i]->render(m_hSceneThreadDC[THREAD::T1]);
		}});

		Mgr(CThreadMgr)->Enqueue(THREAD::T2, [this]() {
		{
			static const auto tileVec = m_vecTileLayer.data();
			(*tileVec)->render(m_hSceneThreadDC[THREAD::T2]);
		}});

		const Vec2 vPlayerOriginPos = m_pPlayer->GetPos();
		const Vec2 vCamShadingPos = Mgr(CCamera)->GetCamShadingPos();
		m_pPlayer->SetPos(vCamShadingPos);

		for (int vecObj=0;vecObj<etoi(GROUP_TYPE::UI);++vecObj)
		{
			const auto vecPtr = m_vecObj[vecObj].data();
			for (size_t i = 0; i < m_vecObj[vecObj].size();)
			{
				if (vecPtr[i]->IsDead())
				{
					Mgr(CEventMgr)->AddDeadObj(vecPtr[i]);
					vecPtr[i] = std::move(m_vecObj[vecObj].back());
					m_vecObj[vecObj].pop_back();
				}
				else
				{
					//vecPtr[i]->render(m_hSceneThreadDC[THREAD::END]);
					g_vecDrawCall.emplace_back(std::async(std::launch::deferred, &CObject::render, vecPtr[i].get(), m_hSceneThreadDC[THREAD::END]));
					++i;
				}
			}
		};

		while (!g_particleWait.load(std::memory_order_seq_cst)) {

		}
	

		const auto cache = g_vecDrawCall.data();
		const unsigned short num = (unsigned short)g_vecDrawCall.size();
		for (unsigned short i = 0; i < num; ++i)cache[i].get();
		g_vecDrawCall.clear();

		m_pPlayer->SetPos(vPlayerOriginPos);

		Mgr(CThreadMgr)->Join(THREAD::T2);

		std::atomic_thread_fence(std::memory_order_seq_cst);

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

		std::atomic_thread_fence(std::memory_order_seq_cst);

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

		std::atomic_thread_fence(std::memory_order_seq_cst);

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

		std::atomic_thread_fence(std::memory_order_seq_cst);


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

		std::atomic_thread_fence(std::memory_order_seq_cst);

		Mgr(CThreadMgr)->Enqueue(THREAD::T0, &CCore::MaznetaClear, Mgr(CCore), m_hSceneThreadDC[THREAD::T1], THREAD::T0);
		Mgr(CThreadMgr)->Enqueue(THREAD::T1, &CCore::MaznetaClear, Mgr(CCore), m_hSceneThreadDC[THREAD::T2], THREAD::T1);
		Mgr(CThreadMgr)->Enqueue(THREAD::T2, &CCore::MaznetaClear, Mgr(CCore), m_hSceneThreadDC[THREAD::END], THREAD::T2);

		Mgr(CCamera)->ResetRenderPos();
		Mgr(CCamera)->SetCamRect(vCamShadingPos);

		if (g_particleDC)
		{
			TransparentBlt(_dc
				, 0
				, 0
				, (int)vRes.x
				, (int)vRes.y
				, g_particleDC
				, 0
				, 0
				, (int)vRes.x
				, (int)vRes.y
				, RGB(255, 0, 255));

			Mgr(CCore)->MazentaBlt(g_particleDC, vRes);
		}

		if (m_vecTileLayer.size() > 1)
		{
			m_vecTileLayer[1]->render(_dc);
			Mgr(CCore)->MazentaBlt(m_vecTileLayer[1]->GetTileLayerDC(), vRes);
		}

		const auto ui_cache = m_vecObj[etoi(GROUP_TYPE::UI)].data();
		const int n = (int)m_vecObj[etoi(GROUP_TYPE::UI)].size();
		for (int i = 0; i < n; ++i)
		{
			ui_cache[i]->render(_dc);
		}
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
	Mgr(CCamera)->Reset();
	for (auto& i : m_vecObj)
	{
		i.clear();
	}
	g_vecDrawCall.clear();
	m_vecLayer.clear();
	m_vecTileLayer.clear();
	m_pPlayer = nullptr;
}

