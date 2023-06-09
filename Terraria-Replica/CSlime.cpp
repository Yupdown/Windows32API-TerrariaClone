#include "pch.h"
#include "CSlime.h"
#include "CAnimator.h"
#include "CCollider.h"

CSlime::CSlime(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName)
	:CMonster{ _trWorld,_wstrMonName,_wstrMonImgName }
{
	auto pAnim = GetComp<CAnimator>();
	pAnim->CreateAnimation(GetName() + L"Idle", _wstrMonImgName, Vec2{ 0,0 }, Vec2{ 16,12 }, Vec2{ 0,13 }, 0.1f, 2);
	pAnim->CreateAnimation(GetName() + L"Charge", _wstrMonImgName, Vec2{ 0,0 }, Vec2{ 16,12 }, Vec2{ 0,13 }, 0.02f, 2);
	pAnim->Play(GetName() + L"Idle", true);

	GetComp<CCollider>()->SetScale(Vec2(16.0f, 12.0f));
	m_charge_time = 0.0f;
}

CSlime::~CSlime()
{

}

void CSlime::update()
{
	CMonster::update();
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
