#include "pch.h"
#include "CTimeMgr.h"
#include "CCore.h"

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
	if (m_fAccTime >= 1.f)
	{
		SetWindowText(Mgr(CCore)->GetMainHwnd(), std::to_wstring(1.f / m_DeltaTime.count()).data());
		m_fAccTime = 0.f;
	}
}
