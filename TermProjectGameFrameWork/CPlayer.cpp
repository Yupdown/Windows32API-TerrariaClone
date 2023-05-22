#include "pch.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CAnimator.h"
#include "CCamera.h"

CPlayer::CPlayer()
{
	m_bIsCamAffected = true;
	CreateComponent(COMPONENT_TYPE::COLLIDER);
	CreateComponent(COMPONENT_TYPE::ANIMATOR);
	CreateComponent(COMPONENT_TYPE::RIGIDBODY);
//	m_pTex = Mgr(CResMgr)->GetTexture(L"link_0.bmp");
	/*m_playerImg = Mgr(CResMgr)->GetImg(L"link_0.bmp").value();
	auto pAnim = GetComp<CAnimator>();
	pAnim->CreateAnimation(L"Player_Walk", L"link_0.bmp", Vec2{ 0,455 }, Vec2{ 60,65 }, 0.1, 10);	
	pAnim->CreateAnimation(L"Player_Walk_Down", L"link_0.bmp", Vec2{ 0,260 }, Vec2{ 60,65 }, 0.1, 10);
	pAnim->CreateAnimation(L"Player_Walk_Up", L"link_0.bmp", Vec2{ 0,390 }, Vec2{ 60,65 }, 0.1, 10);
	pAnim->CreateAnimation(L"Player_IDle", L"link_0.bmp", Vec2{ 0,0 }, Vec2{ 60,65 }, 0.1, 3);
	pAnim->Play(L"Player_Walk", true);*/
	auto pAnim = GetComp<CAnimator>();
	m_pAnimLeg = make_unique<CAnimator>();
	m_pAnimLeg->SetOwner(this);
	pAnim->CreateAnimation(L"Player_Torso_WALK", L"Player_Torso.png", Vec2{ 20,28 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.2f, 4);
	pAnim->CreateAnimation(L"Player_Torso_IDLE", L"Player_Torso.png", Vec2{ 0,0 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.2f, 1);
//	pAnim->CreateAnimation(L"Player_Torse_JUMP",L"Player_Torso.png",Vec2{})
	m_pAnimLeg->CreateAnimation(L"Player_Leg_WALK", L"Player_Leg.png", Vec2{ 0,28 * 5 }, Vec2{ 20,28 }, Vec2{ 0,28 },0.1f, 15);
	m_pAnimLeg->CreateAnimation(L"Player_Leg_IDLE", L"Player_Leg.png", Vec2{ 0,0}, Vec2{ 20,28 }, Vec2{ 0,28 }, 0.1f, 4);
	pAnim->Play(L"Player_Torso_IDLE",true);
	m_pAnimLeg->Play(L"Player_Leg_IDLE", true);
	m_pPrevAnim = pAnim->GetCurAnim();
	//SetScale(Vec2{ 150., 150. });
}

CPlayer::CPlayer(const CPlayer& other)
{
}

void CPlayer::update()
{
	Vec2 vPos = GetPos();	
	auto pAnim = GetComp<CAnimator>();
	
	if (m_pPrevAnim == pAnim->GetCurAnim())
	{
		//pAnim->Play(L"Player_IDle", true);
		pAnim->Play(L"Player_Torso_IDLE", true);
		m_pAnimLeg->Play(L"Player_Leg_IDLE", true);
	}

	if (KEY_HOLD(KEY::W))
	{
		vPos.y -= 500.f * DT;
		//pAnim->Play(L"Player_Walk_Up", true);

	}

	if (KEY_HOLD(KEY::S))
	{
		vPos.y += 500.f * DT;
		//pAnim->Play(L"Player_Walk_Down", true);
	}

	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= 500.f * DT;
		//auto pANim = GetComp<CAnimator>();
		//pAnim->Play(L"Player_Walk", true);
		//pAnim->SetAnimLeft();
		
		pAnim->Play(L"Player_Torso_WALK", true);
		m_pAnimLeg->Play(L"Player_Leg_WALK", true);
		
		pAnim->SetAnimLeft();
		m_pAnimLeg->SetAnimLeft();
	}

	if (KEY_HOLD(KEY::D))
	{
		vPos.x += 500.f * DT;
		//auto pANim = GetComp<CAnimator>();
		//pAnim->Play(L"Player_Walk", true);
	//	pAnim->SetAnimRight();
		
		pAnim->Play(L"Player_Torso_WALK", true);
		m_pAnimLeg->Play(L"Player_Leg_WALK", true);
		pAnim->SetAnimRight();
		m_pAnimLeg->SetAnimRight();
	}
	SetPos(vPos);
	//CObject::component_update();
	//m_pPrevAnim = pAnim->GetCurAnim();
	//StartDelayCoRoutine(this,&CPlayer::update, 1);
	//StartDelayCoRoutine(this, &CPlayer::update, 1);
	//StartDelayCoRoutine(1,this,&CPlayer::update);
	//m_pPrevAnim = pAnim->GetCurAnim();
	//StartDelayCoRoutine(this, [&]() {this->update(); }, 1);
	//m_pAnimLeg->component_update();

}


void CPlayer::render(HDC _dc)const
{
	//CObject::render(_dc);
	//m_pTex->render(_dc, this, Vec2{0,260},Vec2{60.,65.});
	Vec2 vPos = GetPos();
	vPos = Mgr(CCamera)->GetRenderPos(vPos);
	auto [vLT, vScale] = Mgr(CCamera)->GetRenderPos(this);
	//Mgr(CCore)->RotateTransform(m_iDegree,vLT + vScale/2);
	
	CObject::component_render(_dc);
	m_pAnimLeg->component_render(_dc);
	//Mgr(CCore)->ResetTransform();
	m_iDegree = (m_iDegree + 1);
}

void CPlayer::component_update()
{
	CObject::component_update();
	m_pAnimLeg->component_update();
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
