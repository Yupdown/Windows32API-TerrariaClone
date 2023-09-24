#include "pch.h"
#include "CParticleMgr.h"
#include "CCore.h"

std::jthread g_ParticleRenderer;
bool g_bParticleStop = false;
HDC g_particleDC;
vector<std::future<void>> g_renderVec;
std::atomic<bool> g_particleWait;


CParticleMgr::CParticleMgr()
{
	CreateDCBITMAP(m_particeDC, m_particleBit, Mgr(CCore)->GetResolutionV());
	g_particleDC = m_particeDC;
	g_renderVec.reserve(1000);
	g_particleWait.store(true);
	g_ParticleRenderer = std::jthread{ []()noexcept {
		while (!g_bParticleStop)
		{
			g_particleWait.wait(true);

			static const auto c = g_renderVec.data();
			const unsigned short n = (unsigned short)(g_renderVec.size());
			for (unsigned short i = 0; i < n; ++i)c[i].get();
			
			g_particleWait.store(true);

			g_renderVec.clear();
		}
		} };
}

CParticleMgr::~CParticleMgr()
{
	DeleteDCBITMAP(m_particeDC, m_particleBit);
	g_particleWait.store(false);
	g_bParticleStop = true;
	
	g_particleWait.notify_one();

	g_ParticleRenderer.join();
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
	for (unsigned short i = 0; i < 1000; ++i)
	{
		if (cache[i].IsActivate())
		{
			cache[i].Update();
		}
	}

	/*g_ParticleRenderer = std::async(std::launch::async, [] {
		for (unsigned short i = 0; i < 1000; ++i)
		{
			if (cache[i].IsActivate())
			{
				cache[i].Render(g_particleDC);
			}
		}
		});*/
	for (unsigned short i = 0; i < 1000; ++i)
	{
		if (cache[i].IsActivate())
		{
			g_renderVec.emplace_back(std::async(std::launch::deferred,[=]()noexcept {
				cache[i].Render(g_particleDC);
				}));
		}
	}

	g_particleWait.store(false);
	g_particleWait.notify_one();

	/*g_ParticleRenderer = std::async(std::launch::async, []()noexcept {
		static const auto c = g_renderVec.data();
		const unsigned short n = (unsigned short)(g_renderVec.size());
		for (unsigned short i = 0; i < n; ++i)c[i]();
		g_renderVec.clear();
		});*/
}
