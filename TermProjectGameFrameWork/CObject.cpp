#include "pch.h"
#include "CObject.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CCompFactory.h"
#include "CComponent.h"
#include "CCamera.h"

CObject::CObject()
{
}

CObject::~CObject()
{
}

CObject::CObject(const CObject& _other)
{
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
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	//SelectGDI gdi{ _dc,BRUSH_TYPE::HOLLOW };
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Rectangle(_dc, (int)(vPos.x - vScale.x / 2.),
		(int)(vPos.y - vScale.y / 2.),
		(int)(vPos.x + vScale.x / 2.),
		(int)(vPos.y + vScale.y / 2.));
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
