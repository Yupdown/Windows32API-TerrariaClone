#include "pch.h"
#include "CMonster.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CRigidBody.h"

CMonster::CMonster(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName)
{
	m_pTRWolrd = _trWorld;
	m_bIsCamAffected = true;

	CreateComponent(COMPONENT_TYPE::COLLIDER);
	CreateComponent(COMPONENT_TYPE::ANIMATOR);
	CreateComponent(COMPONENT_TYPE::RIGIDBODY);

	SetName(_wstrMonName);

}

CMonster::~CMonster()
{

}

void CMonster::update()
{
	auto vPlayerPos = Mgr(CSceneMgr)->GetCurScene()->GetPlayer()->GetPos();
	auto pAnim = GetComp<CAnimator>();
	auto vCurPos = GetPos();

	auto vDir = (vPlayerPos - vCurPos).Normalize();
	
	auto pRigid = GetComp<CRigidBody>();

	if (pRigid->IsGravity())
	{
		vDir.y = 0.f;
	}

	pRigid->AddVelocity(vDir * 20.f);

	if (vDir.x > 0)
	{
		pAnim->SetAnimLeft();
	}
	else
	{
		pAnim->SetAnimRight();
	}
}

void CMonster::render(HDC _dc) const
{
	GetComp<CAnimator>()->component_render(_dc);
}

void CMonster::OnCollision(CCollider* const _pOther)
{

}

void CMonster::OnCollisionEnter(CCollider* const _pOther)
{
	auto pObj = _pOther->GetOwner();
	if (L"Player" == pObj->GetName())
	{
		DeleteObj(this);
	}
}

void CMonster::OnCollisionExit(CCollider* const _pOther)
{

}