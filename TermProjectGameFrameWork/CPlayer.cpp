#include "pch.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CAnimator.h"

CPlayer::CPlayer()
{
	m_bIsCamAffected = true;
	CreateComponent(COMPONENT_TYPE::COLLIDER);
	CreateComponent(COMPONENT_TYPE::ANIMATOR);
//	m_pTex = Mgr(CResMgr)->GetTexture(L"link_0.bmp");
	auto pAnim = GetComp<CAnimator>();
	pAnim->CreateAnimation(L"Player_Walk", L"link_0.bmp", Vec2{ 0,455 }, Vec2{ 60,65 }, 0.1, 10);	
	pAnim->CreateAnimation(L"Player_Walk_Down", L"link_0.bmp", Vec2{ 0,260 }, Vec2{ 60,65 }, 0.1, 10);
	pAnim->CreateAnimation(L"Player_Walk_Up", L"link_0.bmp", Vec2{ 0,390 }, Vec2{ 60,65 }, 0.1, 10);
	pAnim->CreateAnimation(L"Player_IDle", L"link_0.bmp", Vec2{ 0,0 }, Vec2{ 60,65 }, 0.1, 3);
	pAnim->Play(L"Player_Walk", true);
	m_pPrevAnim = pAnim->GetCurAnim();
	//SetScale(Vec2{ 150., 150. });
}

void CPlayer::update()
{
	Vec2 vPos = GetPos();	
	auto pAnim = GetComp<CAnimator>();
	if (m_pPrevAnim == pAnim->GetCurAnim())
	{
		pAnim->Play(L"Player_IDle", true);
	}

	if (KEY_HOLD(KEY::W))
	{
		vPos.y -= 500. * DT;
		pAnim->Play(L"Player_Walk_Up", true);
	}

	if (KEY_HOLD(KEY::S))
	{
		vPos.y += 500. * DT;
		pAnim->Play(L"Player_Walk_Down", true);
	}

	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= 500. * DT;
		//auto pANim = GetComp<CAnimator>();
		pAnim->Play(L"Player_Walk", true);
		pAnim->SetAnimLeft();
	}

	if (KEY_HOLD(KEY::D))
	{
		vPos.x += 500. * DT;
		//auto pANim = GetComp<CAnimator>();
		pAnim->Play(L"Player_Walk", true);
		pAnim->SetAnimRight();
	}
	SetPos(vPos);
	//CObject::component_update();
	m_pPrevAnim = pAnim->GetCurAnim();
	//StartDelayCoRoutine(this,&CPlayer::update, 1);
}


void CPlayer::render(HDC _dc)
{
	//CObject::render(_dc);
	//m_pTex->render(_dc, this, Vec2{0,260},Vec2{60.,65.});
	CObject::component_render(_dc);
}

void CPlayer::OnCollision(CCollider* const _pOther)
{

}

void CPlayer::OnCollisionEnter(CCollider* const _pOther)
{
	
}

void CPlayer::OnCollisionExit(CCollider* const _pOther)
{
}
