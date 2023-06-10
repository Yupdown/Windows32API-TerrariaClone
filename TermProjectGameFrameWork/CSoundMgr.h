#pragma once
#include "Singleton.hpp"
#include "Fmod.hpp"

class CSoundMgr
	: public Singleton<CSoundMgr>
{
	friend class Singleton;
	CSoundMgr();
	~CSoundMgr();
private:
	int m_iCurChannel = 0;
	FMOD_SYSTEM* m_fmSystem;
	FMOD_CHANNEL* m_fmBGMChannel;
	unordered_map<string, FMOD_SOUND*> m_mapSound;
	unordered_map<string, FMOD_SOUND*>			m_mapBGM;
	array<FMOD_CHANNEL*, 31>						m_arrChannel;
public:
	void init();
	void PlayEffect(string_view _strName, float _fVol = 1.f);
	void PlayBGM(string_view _strName, float _fVol = 1.f);
	void BGMVolUp();
	void BGMVolDown();
	void update();
};

