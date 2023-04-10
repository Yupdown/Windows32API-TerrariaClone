#pragma once
#include "CComponent.h"

class CRigidBody :
    public CComponent
{
private:
    CRigidBody(const CRigidBody&) = default;
private:
	Vec2            m_vForce;   
	Vec2            m_vAccel;  
	Vec2            m_vVelocity;   
	Vec2            m_vMaxVelocity = {200.,600.};
	double          m_dMass = 1.;
	double          m_dFriction = 100.;
	bool            m_bGravity = true;  
	bool            m_bIsGround = false;
private:
	void Move();
public:
	CRigidBody();
	~CRigidBody();
	CRigidBody* Clone(CObject* const _pOwner)const override
	{
		CRigidBody* pNewComp = new CRigidBody{ *this };
		pNewComp->SetOwner(_pOwner);
		return pNewComp;
	}
public:
	bool  IsGround()const { return m_bIsGround; }
	void SetIsGround(bool _b)
	{
		m_bIsGround = _b;
		if (m_bIsGround)
		{
			m_vForce.y = 0.f;
			m_vVelocity.y = 0.f;
		}
	}
	bool  IsGravity()const { return m_bGravity; }
	void  SetGravity(bool _b) { m_bGravity = _b; }
public:
	Vec2 GetVeloicity()const { return m_vVelocity; }
	void SetVelocity(Vec2 _v) { m_vVelocity = _v; }
	void SetMaxVelocity(Vec2 _vMaxVel) { m_vMaxVelocity = _vMaxVel; }
	void SetMass(double _fMass) { m_dMass = _fMass; }
	double GetMass()const { return m_dMass; }
	void AddForce(Vec2 _vF) { m_vForce += _vF; }
	void AddVelocity(Vec2 _v) { m_vVelocity += _v; }
	Vec2 GetForce()const { return m_vForce; }
	void SetForce(Vec2 _v) { m_vForce = _v; }
public:
	void component_update()override;
	void component_render(HDC _dc)const override;
	void update_gravity();
};

