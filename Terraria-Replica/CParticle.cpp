#include "pch.h"
#include "CParticle.h"
#include "CTimeMgr.h"
#include "CustomMath.hpp"
#include "CAtlasElement.h"
#include "CCamera.h"

std::mt19937 CParticle::g_rng = std::mt19937{ std::random_device{}()};
std::uniform_real_distribution<float> CParticle::g_urd = std::uniform_real_distribution<float>{ -1.0f, 1.0f };

CParticle::CParticle()
{
}

CParticle::~CParticle()
{
}

void CParticle::Update()noexcept
{
	m_fLife -= DT;
	m_vVelocity += Vec2::up * 480.0f * DT;
	m_vMidPos += m_vVelocity * DT;
}

void CParticle::Render(HDC dc_)noexcept
{
	const Vec2 vLT = m_vMidPos - g_particleSize / 2.f;
	m_pAtlasForRender->render(dc_, Mgr(CCamera)->GetRenderPos(vLT), g_particleSize * m_fLife * 2.0f);
	if (0.f >= m_fLife)
	{
		m_bIsActivate = false;
	}
}

void CParticle::ActivateParticle(Vec2 vMidPos_, CAtlasElement* const pImg_)noexcept
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
	Vec2 vRandForce = Vec2(g_urd(g_rng), g_urd(g_rng)) * g_particleSpeed;
	m_vVelocity = vRandForce;
}