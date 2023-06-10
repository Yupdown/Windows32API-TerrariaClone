#include "pch.h"
#include "TRWorld.h"
#include "CZombie.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CPlayer.h"

CZombie::CZombie(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName)
	:CMonster{_trWorld,_wstrMonName,_wstrMonImgName}
{
	auto pAnim = GetComp<CAnimator>();
	auto pRigid = GetComp<CRigidBody>();
	auto pCol = GetComp<CCollider>();

	pAnim->CreateAnimation(GetName() + L"Walk", _wstrMonImgName, Vec2{0,0}, Vec2{19,23}, Vec2{0,24}, 0.2f, 3);
	pAnim->Play(GetName() + L"Walk", true);

	pRigid->SetMaxVelocity(Vec2(120.0f, 720.0f));
	pCol->SetScale(Vec2(24.0f, 48.0f));

	m_charge_time = 0.0f;
}

CZombie::~CZombie()
{
}

void CZombie::update()
{
	CMonster::update();

	auto pRigid = GetComp<CRigidBody>();
	auto pAnim = GetComp<CAnimator>();

	m_charge_time -= DT;
	if (IsFloatZero(pRigid->GetVelocity().x) && pRigid->IsGround())
	{
		pRigid->AddVelocity(Vec2::down * 640.0f);
		pRigid->AddForce(Vec2::down * 640.0f);
		m_charge_time = 3.0f;
	}

	CPlayer* player = m_pTRWolrd->GetPlayer();

	auto vDir = (player->GetPos() - m_vPos).Normalize();
	if (vDir.x > 0.0f)
		pRigid->AddVelocity(Vec2::right * 10.0f);
	else
		pRigid->AddVelocity(Vec2::left * 10.0f);

	if (pRigid->GetVelocity().x > 0.0f)
		pAnim->SetAnimLeft();
	else
		pAnim->SetAnimRight();
}

void CZombie::render(HDC _dc) const
{
	CMonster::render(_dc);
}

void CZombie::OnCollision(CCollider* const _pOther)
{
	CMonster::OnCollision(_pOther);
}

void CZombie::OnCollisionEnter(CCollider* const _pOther)
{
	CMonster::OnCollisionEnter(_pOther);
}

void CZombie::OnCollisionExit(CCollider* const _pOther)
{
	CMonster::OnCollisionExit(_pOther);
}
