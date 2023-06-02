#pragma once

class CTimeMgr
	:public Singleton<CTimeMgr>
{
	friend class Singleton;
private:
	CTimeMgr();
	~CTimeMgr();
private:
	std::chrono::steady_clock::time_point m_PrevTime;
	std::chrono::duration<float> m_DeltaTime;
	float m_fAccTime = 0.;
	float m_fCheckDT = 1.;
public:
	void init();
	void update();
	float GetDT()const { return min(0.016f,m_DeltaTime.count()); }
};

