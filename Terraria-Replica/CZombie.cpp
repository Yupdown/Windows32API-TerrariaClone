#include "pch.h"
#include "CZombie.h"
#include "CAnimator.h"

CZombie::CZombie(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName)
	:CMonster{_trWorld,_wstrMonName,_wstrMonImgName}
{
	auto pAnim = GetComp<CAnimator>();
	pAnim->CreateAnimation(GetName() + L"Walk", _wstrMonImgName, Vec2{0,0}, Vec2{19,23}, Vec2{0,24}, 0.2f, 3);
	pAnim->Play(GetName() + L"Walk", true);
}

CZombie::~CZombie()
{
}

void CZombie::update()
{
	CMonster::update();
}

void CZombie::render(HDC _dc) const
{
	CMonster::render(_dc);
}

void CZombie::OnCollision(CCollider* const _pOther)
{
}

void CZombie::OnCollisionEnter(CCollider* const _pOther)
{
}

void CZombie::OnCollisionExit(CCollider* const _pOther)
{
}
