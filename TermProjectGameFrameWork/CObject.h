#pragma once

class CCollider;
class CAnimator;
class CComponent;
class CTexture;
class CCollider;
class CRigidBody;
class CCollisionMgr;
class TRWorld;

class CObject
{
	friend void DeleteObj(CObject* const _pDeadObj);
	friend class CCollisionMgr;
protected:
	CObject(const CObject& _other);
public:
	virtual CObject* Clone()const = 0;
	explicit CObject();
	virtual ~CObject();
	virtual void initObj() {};
protected:
	Vec2				m_vPrevPos = {};
	bool				m_bIsCamAffected = true;
	Vec2		m_vPos{};
	Vec2		m_vScale{};
	TRWorld* m_pTRWolrd = nullptr;
	Vec2 m_vWillPos = {};
	int m_iHP = 200;
private:
	wstring		m_strName = {}; 
	array<unique_ptr<CComponent>, (UINT)COMPONENT_TYPE::END>		m_arrComp; 
	bool					m_bAlive = true;
private:
	void	SetDead() { m_bAlive = false; } 
	void updateTileCollision();
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
	void AddComponent(COMPONENT_TYPE _eCompType, CComponent* const _pComp);
	void CreateComponent(COMPONENT_TYPE _eCompType, Vec2 _vScale = {}, Vec2 _vPos = {});
	constexpr inline void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	constexpr inline void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	constexpr inline Vec2 GetPos()const { return m_vPos; }
	constexpr inline Vec2 GetScale()const { return m_vScale; }
public:
	virtual void OnCollision(CCollider* const _pOther) {}	
	virtual void OnCollisionEnter(CCollider* const _pOther) {}
	virtual void OnCollisionExit(CCollider* const _pOther) {}
public:
	void component_render(HDC _dc)const;
	virtual void update();
	virtual void render(HDC _dc)const;
	virtual void component_update();
	
	constexpr inline void SetWillPos(Vec2 _vPos) { m_vWillPos = _vPos; }
	constexpr inline Vec2 GetWillPos() const { return m_vWillPos; }

	void SetHP(int _iHP) { m_iHP = _iHP; }
	int GetHP()const { return m_iHP; }

	void SetColliderScale(Vec2 _vScale);

	virtual void dmg_render(HDC _dc){}
};

