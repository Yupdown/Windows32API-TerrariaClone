#pragma once

class CScene;

class CSceneMgr
	:public Singleton<CSceneMgr>
{
	friend class Singleton;
	//friend CoRoutine ChangeScene(SCENE_TYPE _eNext);
	CSceneMgr();
public:
	~CSceneMgr();
private:	
	array<unique_ptr<CScene>, (UINT)SCENE_TYPE::END>			m_arrScene ;
	CScene* m_pCurScene{};						

public:
	void init();
	void update();
	void render(HDC _dc);
	CScene* GetCurScene()const { return m_pCurScene; }	
private:
	void ChangeScene(SCENE_TYPE _eNext);
};


