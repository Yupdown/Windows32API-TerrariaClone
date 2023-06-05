#include "pch.h"
#include "CSlime.h"
#include "CAnimator.h"

CSlime::CSlime(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName)
	:CMonster{ _trWorld,_wstrMonName,_wstrMonImgName }
{
	auto pAnim = GetComp<CAnimator>();
	pAnim->CreateAnimation(GetName() + L"Walk", _wstrMonImgName, Vec2{ 0,0 }, Vec2{ 16,13 }, Vec2{ 0,13 }, 0.1f, 2);
	pAnim->Play(GetName() + L"Walk", true);
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
}

void CSlime::OnCollisionEnter(CCollider* const _pOther)
{
}

void CSlime::OnCollisionExit(CCollider* const _pOther)
{
}
