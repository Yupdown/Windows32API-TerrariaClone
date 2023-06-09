#include "pch.h"
#include "CObject.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CCompFactory.h"
#include "CComponent.h"
#include "CCamera.h"

extern void updateTileCollision(CObject* const _pObj, TRWorld* const _pTRWorld);

CObject::CObject()
{
}

CObject::~CObject()
{
}

CObject::CObject(const CObject& _other)
	:m_strName{ _other.m_strName }
	, m_vPos{ _other.m_vPos }
	, m_vScale{ _other.m_vScale }
{
	for (int i = 0; i < etoi(COMPONENT_TYPE::END); ++i)
	{
		if (_other.m_arrComp[i])
		{
			CComponent* pComp = _other.m_arrComp[i]->Clone(this);
			AddComponent(static_cast<COMPONENT_TYPE>(i), pComp);
		}
	}
}

void CObject::AddComponent(COMPONENT_TYPE _eCompType, CComponent* const _pComp)
{
	m_arrComp[etoi(_eCompType)] = unique_ptr<CComponent>{ _pComp };
}

void CObject::CreateComponent(COMPONENT_TYPE _eCompType, Vec2 _vScale, Vec2 _vPos)
{
	auto pComp = CCompFactory::CreateComponent(_eCompType, this, _vScale, _vPos);
	AddComponent(_eCompType, pComp);
}

void CObject::update()
{
	m_vPrevPos = m_vPos;
}

void CObject::render(HDC _dc)const
{
	/*auto [vPos, vScale] = Mgr(CCamera)->GetRenderPos(this);
	
	Rectangle(_dc, 
		(int)(vPos.x),
		(int)(vPos.y),
		(int)(vPos.x + vScale.x),
		(int)(vPos.y + vScale.y));*/
}

void CObject::component_render(HDC _dc)const
{
	for (const auto& component : m_arrComp)
	{
		if (component)
		{
			component->component_render(_dc);
		}
	}
}

void CObject::component_update()
{
	for (auto& component : m_arrComp)
	{
		if (component)
		{
			component->component_update();
		}
	}
}

void CObject::SetColliderScale(Vec2 _vScale)
{
	auto pCollider = GetComp<CCollider>();
	assert(pCollider);
	pCollider->SetScale(_vScale);
}

void CObject::updateTileCollision()
{
	if (m_pTRWolrd && m_arrComp[etoi(COMPONENT_TYPE::RIGIDBODY)])
	{
		::updateTileCollision(this, m_pTRWolrd);
	}
}
