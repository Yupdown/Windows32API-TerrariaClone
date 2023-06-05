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
	if (m_bIsGround)
	{
		m_vVelocity.y = 0.f;
	}

	Vec2 vPos = m_pOwner->GetPos();
	Vec2 vScale = m_pOwner->GetScale()/2;
	

	vPos += m_vVelocity * DT;

	//if (vPos.x < vScale.x)
	//{
	//	vPos.x = vScale.x;
	//}
	//if (vPos.y > 8000.f - vScale.y)
	//{
	//	vPos.y = 8000.f - vScale.y;
	//}
	//if (vPos.x + vScale.x > 8192.f)
	//{
	//	vPos.x = 8192.f - vScale.x;
	//}

	//m_pOwner->SetPos(vPos);

	m_pOwner->SetWillPos(vPos);
}

void CRigidBody::component_update()
{
	update_gravity();
	m_vAccel = m_vForce / m_fMass;
	m_vVelocity += m_vAccel * DT;
	if (!m_vVelocity.IsZero())
	{
		Vec2 vFriction = m_vVelocity.Normalize();
		vFriction *= m_fFriction * DT;
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

void CRigidBody::component_render(HDC _dc)const
{
	
}

void CRigidBody::update_gravity()
{
	auto vLow = m_pOwner->GetPos().y + m_pOwner->GetScale().y/2.f;
	if (vLow >= 7040)
	{
	//	m_bIsGround = true;
	}
	if (m_bGravity && !m_bIsGround)
	{
		AddForce(Vec2{ 0.0f,2000.0f });
	}
}



