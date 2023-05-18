#include "pch.h"
#include "CTimeMgr.h"

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
}
