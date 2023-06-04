#pragma once

class CObject;
class CTexture;
class CLayer;
class CTileLayer;

class CScene
{
	//static jthread m_rednerThread;
	friend class CDebugMgr;
public:
	CScene();
	virtual ~CScene();
protected:
	vector<unique_ptr<CLayer>> m_vecLayer;
	vector<unique_ptr<CTileLayer>> m_vecTileLayer;
	bool m_bDoThreadPool = false;
private:
	vector<unique_ptr<CObject>>			m_vecObj[(UINT)GROUP_TYPE::END]; 
	wstring								m_strName;	
	CObject*							m_pPlayer = {};
	
	HDC	m_hSceneThreadDC[THREAD::END + 1];
	HBITMAP	m_hSceneThreadBit[THREAD::END + 1];

public: 
	void RegisterPlayer(CObject* const _pPlayer) { m_pPlayer = _pPlayer; }
	CObject* GetPlayer()const { return m_pPlayer; }
	void AddObject(CObject* const _pObj, GROUP_TYPE _eType);
	const vector<unique_ptr<CObject>>& GetGroupObject(GROUP_TYPE _eType)const;
	vector<unique_ptr<CObject>>& GetUIGroup();
public:
	
	void component_update()const;
	
	void DeleteGroup(GROUP_TYPE _eTarget); 
	void Reset();


	virtual void render(HDC _dc);
	virtual void update();
	virtual void Enter();
	virtual void Exit();

public:
	void SetName(wstring_view _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	void AddTileLayer(CTileLayer* const _pTileLayer);

};
