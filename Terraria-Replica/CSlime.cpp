#include "pch.h"
#include "TRWorld.h"
#include "CPlayer.h"
#include "CSlime.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CRigidBody.h"

CSlime::CSlime(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName)
	:CMonster{ _trWorld,_wstrMonName,_wstrMonImgName }
{
	auto pAnim = GetComp<CAnimator>();
	pAnim->CreateAnimation(GetName() + L"Idle", _wstrMonImgName, Vec2{ 0,0 }, Vec2{ 17,12 }, Vec2{ 0,13 }, 0.1f, 2);
	pAnim->CreateAnimation(GetName() + L"Charge", _wstrMonImgName, Vec2{ 0,0 }, Vec2{ 17,12 }, Vec2{ 0,13 }, 0.03f, 2);
	pAnim->Play(GetName() + L"Idle", true);

	GetComp<CCollider>()->SetScale(Vec2(12.0f, 12.0f));
	m_charge_time = 0.0f;
}

CSlime::~CSlime()
{

}

void CSlime::update()
{
	CMonster::update();

	float t = m_charge_time;
	m_charge_time -= DT;

	if (m_charge_time <= 0.0f)
		Charge();

	if (t > 1.0f && m_charge_time <= 1.0f)
	{
		auto pAnim = GetComp<CAnimator>();
		pAnim->Play(GetName() + L"Charge", true);
	}
}

void CSlime::render(HDC _dc) const
{
	CMonster::render(_dc);
}

void CSlime::OnCollision(CCollider* const _pOther)
{
	CMonster::OnCollision(_pOther);
}

void  CSlime::OnCollisionEnter(CCollider* const _pOther)
{
	CMonster::OnCollisionEnter(_pOther);
}

void  CSlime::OnCollisionExit(CCollider* const _pOther)
{
	CMonster::OnCollisionExit(_pOther);
}

void CSlime::Charge()
{
	auto pRigid = GetComp<CRigidBody>();
	CPlayer* player = m_pTRWolrd->GetPlayer();
	float dx = (player->GetPos() - m_vPos).x;

	if (dx > 0.0f)
		pRigid->AddVelocity(Vec2(800.0f, -800.0f));
	else
		pRigid->AddVelocity(Vec2(-800.0f, -800.0f));

	auto pAnim = GetComp<CAnimator>();
	pAnim->Play(GetName() + L"Idle", true);

	m_charge_time = 3.0f;
}
