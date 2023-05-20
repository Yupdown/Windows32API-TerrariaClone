#pragma once

class CObject;
class CTile;
class CTexture;
class CLayer;

class CScene
{
	static int g_iSceneID;
public:
	CScene();
	virtual ~CScene();
protected:
	map<Vec2, unique_ptr<CTile>>			m_mapTile;
	const CImage* m_pBackGroundImg = nullptr;
	const CImage* m_pBackGroundImg2 = nullptr;
	vector<CImage*> m_vecSceneLayer;
	vector<unique_ptr<CLayer>> m_vecLayer;
private:
	vector<unique_ptr<CObject>>			m_vecObj[(UINT)GROUP_TYPE::END]; // 오브젝트를 관리할 벡터를 그룹개수만큼
	vector<unique_ptr<CObject>>			m_vecDeadObj;
	wstring								m_strName;	
	CObject*							m_pPlayer = {};
public: 
	void AddTile(wstring_view _strTileTexName, Vec2 _vPos, Vec2 _vScale, Vec2 _vBitPos, Vec2 _vSlice);
	void AddChunkTile(wstring_view _strTileTexName,Vec2 _vLtPos,Vec2 _vScale ,UINT _iRow, UINT _iCol,UINT _iBitMapIdx);
	void RegisterPlayer(CObject* const _pPlayer) { m_pPlayer = _pPlayer; }
	CObject* GetPlayer()const { return m_pPlayer; }
	void AddObject(CObject* const _pObj, GROUP_TYPE _eType) { m_vecObj[(UINT)_eType].emplace_back(_pObj); }
	const auto& GetGroupObject(GROUP_TYPE _eType)const { return m_vecObj[(UINT)_eType]; }
	auto& GetUIGroup() { return m_vecObj[(UINT)GROUP_TYPE::UI]; }
public:
	
	void component_update()const;
	
	void DeleteGroup(GROUP_TYPE _eTarget); 
	void Reset();

	virtual void render(HDC _dc);
	virtual void update();
	virtual void Enter() = 0;
	virtual void Exit() = 0;

public:
	void SetName(wstring_view _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }


	
};
