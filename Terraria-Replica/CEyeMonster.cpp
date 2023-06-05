#include "pch.h"
#include "CEyeMonster.h"
#include "CAnimator.h"
#include "CRigidBody.h"

CEyeMonster::CEyeMonster(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName)
	:CMonster{ _trWorld,_wstrMonName,_wstrMonImgName }
{
	auto pAnim = GetComp<CAnimator>();
	pAnim->CreateAnimation(GetName() + L"Walk", _wstrMonImgName, Vec2{ 0,0 }, Vec2{ 18,12 }, Vec2{ 0,12 }, 0.1f, 2);
	pAnim->Play(GetName() + L"Walk", true);
	auto pRigid = GetComp<CRigidBody>();

	pRigid->SetGravity(false);
}

CEyeMonster::~CEyeMonster()
{
}

void CEyeMonster::update()
{
	CMonster::update();
}

void CEyeMonster::render(HDC _dc) const
{
	CMonster::render(_dc);
}

void CEyeMonster::OnCollision(CCollider* const _pOther)
{
}

void CEyeMonster::OnCollisionEnter(CCollider* const _pOther)
{
}

void CEyeMonster::OnCollisionExit(CCollider* const _pOther)
{
}
