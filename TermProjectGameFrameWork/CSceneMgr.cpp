#include "pch.h"
#include "CCore.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Tool.h"
#include "CEventMgr.h"

CSceneMgr::CSceneMgr()
{
}

CSceneMgr::~CSceneMgr()
{
	if (m_pCurScene)
	{
		m_pCurScene->Exit();
	}
}

//void CSceneMgr::init()
//{
//	// Scene »ý¼º
//	m_arrScene[etoi(SCENE_TYPE::START)] = make_unique<CScene_Start>();
//	m_arrScene[etoi(SCENE_TYPE::START)]->SetName(L"Start Scene");
//
//	m_arrScene[etoi(SCENE_TYPE::TOOL)] = make_unique<CScene_Tool>();
//	m_arrScene[etoi(SCENE_TYPE::TOOL)]->SetName(L"Tool Scene");
//
//	//m_pCurScene = m_arrScene[etoi(SCENE_TYPE::TOOL)].get();
//	m_pCurScene = m_arrScene[etoi(SCENE_TYPE::START)].get();
//
//	m_pCurScene->Enter();
//}

void CSceneMgr::update()
{
	m_pCurScene->update();
	m_pCurScene->component_update();
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	Mgr(CEventMgr)->Reset();

	m_pCurScene->Exit();

	m_pCurScene = m_arrScene[etoi(_eNext)].get();

	m_pCurScene->Enter();

}

void CSceneMgr::AddScene(SCENE_TYPE _eType, CScene* const _pScene)
{
	m_arrScene[etoi(_eType)] = std::unique_ptr<CScene>{ _pScene };
}

void CSceneMgr::init(SCENE_TYPE _eType)
{
	m_pCurScene = m_arrScene[etoi(_eType)].get();

	m_pCurScene->Enter();
}

CScene* CSceneMgr::GetScene(SCENE_TYPE _eType) const
{
	return m_arrScene[etoi(_eType)].get();
}
