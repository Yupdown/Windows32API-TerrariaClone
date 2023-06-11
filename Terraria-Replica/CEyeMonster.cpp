#include "pch.h"
#include "CEyeMonster.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCore.h"
#include "CCamera.h"
#include "CustomMath.hpp"
#include "SimpleMath.hpp"
#include "CCollider.h"

CEyeMonster::CEyeMonster(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName)
	:CMonster{ _trWorld,_wstrMonName,_wstrMonImgName }
{
	auto pAnim = GetComp<CAnimator>();
	pAnim->CreateAnimation(GetName() + L"Walk", _wstrMonImgName, Vec2{ 0,0 }, Vec2{ 19,11 }, Vec2{ 0,12 }, 0.1f, 2);
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

	auto vPlayerPos = Mgr(CSceneMgr)->GetCurScene()->GetPlayer()->GetPos();
	auto pAnim = GetComp<CAnimator>();
	auto vCurPos = GetPos();

	auto vDir = (vPlayerPos - vCurPos).Normalize();
	
	auto pRigid = GetComp<CRigidBody>();

	pRigid->AddVelocity(vDir * 20.f);

	if (vDir.x > 0)
		pAnim->SetAnimLeft();
	else
		pAnim->SetAnimRight();
}

void CEyeMonster::render(HDC _dc) const
{
	auto vPlayerPos = Mgr(CSceneMgr)->GetCurScene()->GetPlayer()->GetPrevPos();
	auto vCurPos = GetPrevPos();
	auto vDir = (GetPos() - GetPrevPos()).Normalize();
	const float fDeg = atan2f(vDir.y, vDir.x) + (GetComp<CAnimator>()->GetAnimDir() ? 0.0f : F_PI);
	const auto [vLT, vScale] = Mgr(CCamera)->GetRenderPos(this);
	Mgr(CCore)->RotateTransform(_dc, fDeg * F_RAD2DEG, vLT + vScale/2.f);
	CMonster::render(_dc);
	Mgr(CCore)->ResetTransform(_dc);
}

void CEyeMonster::OnCollision(CCollider* const _pOther)
{
	CMonster::OnCollision(_pOther);
}

void  CEyeMonster::OnCollisionEnter(CCollider* const _pOther)
{
	CMonster::OnCollisionEnter(_pOther);
}

void  CEyeMonster::OnCollisionExit(CCollider* const _pOther)
{
	CMonster::OnCollisionExit(_pOther);
}
