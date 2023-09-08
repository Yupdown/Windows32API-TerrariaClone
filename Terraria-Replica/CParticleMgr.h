#pragma once
#include "CParticle.h"

class CParticleMgr
	:public Singleton<CParticleMgr>
{
	friend class Singleton;
	CParticleMgr();
	~CParticleMgr();
private:
	array<CParticle,1000> m_arrParticles;
	unsigned short m_iCurParticleNum = 0;
public:
	void Init();
	void SetParticles(const Vec2 vPos_,CAtlasElement* const pImg_);
	void Update();
};

