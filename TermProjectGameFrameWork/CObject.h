#pragma once

class CCollider;
class CAnimator;
class CComponent;
class CTexture;
class CCollider;
class CRigidBody;

class CObject
{
	friend void DeleteObj(CObject* const _pDeadObj);
protected:
	CObject(const CObject& _other);
public:
	virtual CObject* Clone()const = 0;
	explicit CObject();
	virtual ~CObject();
	virtual void initObj() {};
protected:
	CTexture* m_pRTex = nullptr;
	CTexture* m_pLTex = nullptr;
	int					m_iDir = 1;	
	int					m_iPrevDir = {};
	Vec2				m_vPrevPos = {};
	bool				m_bIsCamAffected = true;
private:
	wstring		m_strName = {}; 
	Vec2		m_vPos{};	
	Vec2		m_vScale{};	
	array<unique_ptr<CComponent>, (UINT)COMPONENT_TYPE::END>		m_arrComp; 
	bool					m_bAlive = true;
private:
	void	SetDead() { m_bAlive = false; } 
public:
	bool IsDead()const { return !m_bAlive; } 
	bool IsCamAffect()const { return m_bIsCamAffected; }
	Vec2 GetPrevPos()const { return m_vPrevPos; }
	void SetName(wstring_view _strName) { m_strName = _strName; }
	const wstring& GetName()const { return m_strName; }
public:
	template <typename Comp>
	constexpr auto GetComp()const
	{
		int idx = 0;
		if constexpr (std::is_same<Comp*, CCollider*> {})
			idx = etoi(COMPONENT_TYPE::COLLIDER);
		else if constexpr (std::is_same<Comp*, CAnimator*> {})
			idx = etoi(COMPONENT_TYPE::ANIMATOR);
		else if constexpr (std::is_same<Comp*, CRigidBody*> {})
			idx = etoi(COMPONENT_TYPE::RIGIDBODY);
		else
			assert(false);
		return static_cast<Comp*>(m_arrComp[idx].get());
	}
	//CComponent* GetComponent(COMPONENT_TYPE _eCompType)const { return m_arrComp[(UINT)_eCompType].get(); }
	void AddComponent(COMPONENT_TYPE _eCompType, CComponent* const _pComp);
	void CreateComponent(COMPONENT_TYPE _eCompType, Vec2 _vScale = {}, Vec2 _vPos = {});
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetPos()const { return m_vPos; }
	Vec2 GetScale()const { return m_vScale; }
public:
	virtual void OnCollision(CCollider* const _pOther) {}	
	virtual void OnCollisionEnter(CCollider* const _pOther) {}
	virtual void OnCollisionExit(CCollider* const _pOther) {}
public:
	void component_render(HDC _dc);
	virtual void update();
	virtual void render(HDC _dc);	
	virtual void component_update();
};

