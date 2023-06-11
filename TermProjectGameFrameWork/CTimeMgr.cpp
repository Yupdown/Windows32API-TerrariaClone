#include "pch.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CCamera.h"

CTimeMgr::CTimeMgr()
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::init()
{
	m_DeltaTime = std::chrono::duration<float>(0.f);
	m_PrevTime = std::chrono::steady_clock::now();
}

void CTimeMgr::update()
{
	auto CurTime = std::chrono::steady_clock::now();
	m_DeltaTime = CurTime - m_PrevTime;
	m_PrevTime = CurTime;
	m_fAccTime += m_DeltaTime.count();
	
	wstring wstr = L"Terraria-Replica  FPS: " + std::to_wstring(m_fCheckDT);
	SetWindowText(Mgr(CCore)->GetMainHwnd(), wstr.data());
	if (m_fAccTime >= 1.f)
	{
		m_fAccTime = 0.f;
		m_fCheckDT = 1.f / m_DeltaTime.count();
	}
}
