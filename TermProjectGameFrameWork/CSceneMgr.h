#pragma once

class CScene;

class CSceneMgr
	:public Singleton<CSceneMgr>
{
	friend void ChangeScene(SCENE_TYPE _eNext);
	friend class Singleton;
	CSceneMgr();
	~CSceneMgr();
private:	
	array<unique_ptr<CScene>, (UINT)SCENE_TYPE::END>			m_arrScene ;
	CScene* m_pCurScene{};						

public:
	void update();
	void render(HDC _dc);
	CScene* GetCurScene()const { return m_pCurScene; }	
private:
	void ChangeScene(SCENE_TYPE _eNext);

public:
	void AddScene(SCENE_TYPE _eType, CScene* const _pScene);
	void init(SCENE_TYPE _eType);
	CScene* GetScene(SCENE_TYPE _eType)const;
};


