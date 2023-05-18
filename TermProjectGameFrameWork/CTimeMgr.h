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
public:
	void init();
	void update();
	float GetDT()const { return m_DeltaTime.count(); }
};

