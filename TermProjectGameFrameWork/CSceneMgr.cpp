#include "pch.h"
#include "CCore.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Start.h"

CSceneMgr::CSceneMgr()
{
}

CSceneMgr::~CSceneMgr()
{
		
}

void CSceneMgr::init()
{
	// Scene »ý¼º
	m_arrScene[(UINT)SCENE_TYPE::START] = make_unique<CScene_Start>();
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");


	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::START].get();


	m_pCurScene->Enter();
}

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
}
