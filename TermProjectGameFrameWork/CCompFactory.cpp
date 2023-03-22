#include "pch.h"
#include "CCompFactory.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"

CComponent* CCompFactory::CreateComponent(COMPONENT_TYPE _eCompType, CObject* const _pOwner, Vec2 _vScale, Vec2 _vPos)
{
	CComponent* pComp = nullptr;

	switch (_eCompType)
	{
	case COMPONENT_TYPE::COLLIDER:
	{
		CCollider* pCollider = new CCollider;
		pCollider->SetOffsetPos(_vPos);
		pCollider->SetScale(_vScale);
		pComp = pCollider;
	}
	break;
	case COMPONENT_TYPE::ANIMATOR:
	{
		CAnimator* pAnim = new CAnimator;
		pComp = pAnim;
	}
	break;
	case COMPONENT_TYPE::RIGIDBODY:
	{
		CRigidBody* pRigid = new CRigidBody;
		pComp = pRigid;
	}
	break;
	case COMPONENT_TYPE::END:
		break;
	default:
		break;
	}
	assert(pComp);
	pComp->SetOwner(_pOwner);
	return pComp;
}
