#pragma once

class CAtlasElement;

class CParticle
{
	static constexpr Vec2 g_particleSize = SRC_TILE_SIZE_XY;
	static constexpr float g_particleSpeed = 30.f;
	static std::mt19937 g_rng;
	static std::uniform_real_distribution<float> g_urd;
private:
	Vec2 m_vMidPos;
	Vec2 m_vRandDir;
	bool m_bIsActivate = false;
	float m_fLife = 0.f;
	CAtlasElement* m_pAtlasForRender = nullptr;
public:
	CParticle();
	~CParticle();
	CParticle(const CParticle&) = delete;
	CParticle operator = (const CParticle&) = delete;
	void ActivateParticle(Vec2 vMidPos_,CAtlasElement* const _pImg_);
	bool IsActivate()const { return m_bIsActivate; }
	void Update();
	void Render(HDC dc_);
};

