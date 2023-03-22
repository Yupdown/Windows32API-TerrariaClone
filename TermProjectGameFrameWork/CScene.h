#pragma once

class CObject;

class CScene
{
public:
	CScene();
	virtual ~CScene();
private:
	vector<unique_ptr<CObject>>			m_arrObj[(UINT)GROUP_TYPE::END]; // 오브젝트를 관리할 벡터를 그룹개수만큼
	wstring								m_strName;	
	CObject* m_pPlayer = {};
public: 
	void RegisterPlayer(CObject* const _pPlayer) { m_pPlayer = _pPlayer; }
	CObject* GetPlayer()const { return m_pPlayer; }
	void AddObject(CObject* const _pObj, GROUP_TYPE _eType) { m_arrObj[(UINT)_eType].emplace_back(_pObj); }
	const auto& GetGroupObject(GROUP_TYPE _eType)const { return m_arrObj[(UINT)_eType]; }
	auto& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }
public:
	virtual void update();
	void component_update();
	void render(HDC _dc);
	void DeleteGroup(GROUP_TYPE _eTarget); 
	void Reset();
public:
	void SetName(wstring_view _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	virtual void Enter() = 0;	
	virtual void Exit() = 0;
};
