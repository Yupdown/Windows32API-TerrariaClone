#include "pch.h"
#include "CRigidBody.h"
#include "CObject.h"
#include "CTimeMgr.h"

CRigidBody::CRigidBody()
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::Move()
{
	if (m_vVelocity.IsZero())
	{
		return;
	}
	Vec2 vPos = m_pOwner->GetPos();
	vPos += m_vVelocity * DT;
	m_pOwner->SetPos(vPos);
}

void CRigidBody::component_update()
{
	update_gravity();
	m_vAccel = m_vForce / m_dMass;
	m_vVelocity += m_vAccel * DT;
	if (!m_vVelocity.IsZero())
	{
		Vec2 vFriction = m_vVelocity.Normalize();
		vFriction *= m_dFriction * DT;
		if (m_vVelocity.length() <= vFriction.length())
		{
			m_vVelocity = Vec2{ 0.,0. };
		}
		else
		{
			m_vVelocity -= vFriction;
		}
	}
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		m_vVelocity.x = m_vVelocity.x / abs(m_vVelocity.x) * abs(m_vMaxVelocity.x);
	}
	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	{
		m_vVelocity.y = m_vVelocity.y / abs(m_vVelocity.y) * abs(m_vMaxVelocity.y);
	}
	Move();
	m_vForce = Vec2{ 0.,0. };
}

void CRigidBody::component_render(HDC _dc)
{
	
}

void CRigidBody::update_gravity()
{
	if (m_bGravity && !m_bIsGround)
	{
		AddForce(Vec2{ 0.,1000. });
	}
}



