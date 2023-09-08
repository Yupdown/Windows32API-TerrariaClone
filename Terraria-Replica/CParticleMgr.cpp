#include "pch.h"
#include "CParticleMgr.h"
#include "CCore.h"

CParticleMgr::CParticleMgr()
{
}

CParticleMgr::~CParticleMgr()
{
}

void CParticleMgr::Init()
{
}

void CParticleMgr::SetParticles(const Vec2 vPos_, CAtlasElement* const pImg_)
{
	const unsigned short end_particles = m_iCurParticleNum + 20;
	static const auto cache = m_arrParticles.data();
	for (unsigned short i = m_iCurParticleNum; i < end_particles; ++i)
	{
		cache[i].ActivateParticle(vPos_, pImg_);
	}
	m_iCurParticleNum = (m_iCurParticleNum + 20) % 1000;
}

void CParticleMgr::Update()
{
	static const auto cache = m_arrParticles.data();
	static auto renderer = Mgr(CCore)->GetMainDC();
	for (unsigned short i = 0; i < 1000; ++i)
	{
		if (cache[i].IsActivate())
		{
			cache[i].Update();
		}
	}
	for (unsigned short i = 0; i < 1000; ++i)
	{
		if (cache[i].IsActivate())
		{
			cache[i].Render(renderer);
		}
	}
}
