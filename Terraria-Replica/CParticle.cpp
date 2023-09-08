#include "pch.h"
#include "CParticle.h"
#include "CTimeMgr.h"
#include "CustomMath.hpp"
#include "CAtlasElement.h"
#include "CCamera.h"

std::mt19937 CParticle::g_rng = std::mt19937{ std::random_device{}()};
std::uniform_real_distribution<float> CParticle::g_urd = std::uniform_real_distribution<float>{ 0.f,360.f };

CParticle::CParticle()
{
}

CParticle::~CParticle()
{
}

void CParticle::Update()
{
	m_fLife -= DT;
	m_vMidPos += m_vRandDir * DT * g_particleSpeed;
}

void CParticle::Render(HDC dc_)
{
	const Vec2 vLT = m_vMidPos - g_particleSize / 2.f;
	m_pAtlasForRender->render(dc_, Mgr(CCamera)->GetRenderPos(vLT), g_particleSize);
	if (0.f >= m_fLife)
	{
		m_bIsActivate = false;
	}
}

void CParticle::ActivateParticle(Vec2 vMidPos_, CAtlasElement* const pImg_)
{
	if (!pImg_)
	{
		m_bIsActivate = false;
		return;
	}
	m_bIsActivate = true;
	m_fLife = 0.5f;
	m_vMidPos = vMidPos_;
	m_pAtlasForRender = pImg_;
	const float angle = g_urd(g_rng) * F_DEG2RAD;
	m_vRandDir = Vec2{ std::cosf(angle),std::sinf(angle) };
}
