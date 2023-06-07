#include "pch.h"
#include "CMonster.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CRigidBody.h"
#include "CPlayer.h"
#include "CWeapon.h"
#include "CEventMgr.h"

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
	if (Mgr(CSceneMgr)->GetCurScene()->GetPlayerCast()->IsPlayerSlane())
	{
		return;
	}

	CObject::update();
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
	//GetComp<CAnimator>()->component_render(_dc);
	CObject::component_render(_dc);
}

void CMonster::OnCollision(CCollider* const _pOther)
{
	auto pObj = _pOther->GetOwner();
	if (L"Player" == pObj->GetName())
	{
		auto pPlayer = (CPlayer*)pObj;
		if (pPlayer->IsPlayerSlane())
		{
			return;
		}
		auto vDir = GetComp<CRigidBody>()->GetVelocity().Normalize();
		vDir.y = 0;
		
		pPlayer->GetComp<CRigidBody>()->AddForce(vDir * 500);
		pPlayer->GetComp<CRigidBody>()->component_update();
	}
}

void CMonster::OnCollisionEnter(CCollider* const _pOther)
{
	auto pObj = _pOther->GetOwner();
	const wstring wstrObjName = pObj->GetName().substr(0, pObj->GetName().find(L'_'));
	if (L"Player" == wstrObjName)
	{
		auto pPlayer = (CPlayer*)pObj;

		if (pPlayer->IsPlayerSlane())
		{
			return;
		}

		auto iCurHP = pPlayer->GetHP();
		pPlayer->SetHP(iCurHP - 30);
		auto vDir = GetComp<CRigidBody>()->GetVelocity().Normalize();
		vDir.y = 0;
		pPlayer->GetComp<CRigidBody>()->SetLimitBreak();
		pPlayer->GetComp<CRigidBody>()->SetVelocity({});
		pPlayer->GetComp<CRigidBody>()->AddVelocity(vDir * 500 );
		pPlayer->GetComp<CRigidBody>()->AddForce(vDir*500 );
		pPlayer->GetComp<CRigidBody>()->component_update();
		if (pPlayer->GetHP() <= 0)
		{
			pPlayer->SetSlane(true);
			pPlayer->GetComp<CRigidBody>()->SetVelocity({});
			StartDelayCoRoutine(pPlayer,pPlayer->PlayerRebirthProcess(), 1.f);
		}
	}
}

void CMonster::OnCollisionExit(CCollider* const _pOther)
{
	
}