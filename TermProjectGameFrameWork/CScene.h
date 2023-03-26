#pragma once

class CObject;
class CTile;
class CTexture;
	
class CScene
{
public:
	CScene();
	virtual ~CScene();
protected:
	map<Vec2, unique_ptr<CTile>>			m_mapTile;
	static CTexture*							m_pBackGroundTex;
private:
	vector<unique_ptr<CObject>>			m_vecObj[(UINT)GROUP_TYPE::END]; // ������Ʈ�� ������ ���͸� �׷찳����ŭ
	wstring								m_strName;	
	CObject*							m_pPlayer = {};
public: 
	void AddTile(wstring_view _strTileTexName, Vec2 _vPos, Vec2 _vScale, Vec2 _vBitPos, Vec2 _vSlice);
	void RegisterPlayer(CObject* const _pPlayer) { m_pPlayer = _pPlayer; }
	CObject* GetPlayer()const { return m_pPlayer; }
	void AddObject(CObject* const _pObj, GROUP_TYPE _eType) { m_vecObj[(UINT)_eType].emplace_back(_pObj); }
	const auto& GetGroupObject(GROUP_TYPE _eType)const { return m_vecObj[(UINT)_eType]; }
	auto& GetUIGroup() { return m_vecObj[(UINT)GROUP_TYPE::UI]; }
public:
	virtual void update();
	void component_update();
	void render(HDC _dc);
	void DeleteGroup(GROUP_TYPE _eTarget); 
	void Reset();
public:
	void Clear();
	void SetName(wstring_view _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	virtual void Enter() = 0;	
	virtual void Exit() = 0;
};
