#include "pch.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CAnimator.h"
#include "CCamera.h"
#include "CRigidBody.h"
#include "CAnimation.h"
#include "TRWorld.h"
#include "CustomMath.hpp"
#include "Vec2Int.hpp"
#include "CWeapon.h"
#include "SimpleMath.hpp"
#include "CCollider.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCamera.h"
#include "CEventMgr.h"
#include "CDropItem.h"

CPlayer::CPlayer(TRWorld* const _trWorld)
{
	m_pTRWolrd = _trWorld;
	m_bIsCamAffected = true;
	CreateComponent(COMPONENT_TYPE::COLLIDER, Vec2{ 24.0f, 48.0f });
	CreateComponent(COMPONENT_TYPE::ANIMATOR);
	CreateComponent(COMPONENT_TYPE::RIGIDBODY);
	SetName(L"Player_");
	auto pAnim = GetComp<CAnimator>();
	m_pAnimLeg = make_unique<CAnimator>();
	m_pAnimLeg->SetOwner(this);
	pAnim->CreateAnimation(L"Player_Torso_WALK", L"Player_Torso.png", Vec2{ 20 * 2, 28 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.04f, 13);
	pAnim->CreateAnimation(L"Player_Torso_IDLE", L"Player_Torso.png", Vec2{ 0,0 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.2f, 1);
	pAnim->CreateAnimation(L"Player_Torso_JUMP", L"Player_Torso.png", Vec2{ 0,28 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.5f, 1);
	pAnim->CreateAnimation(L"Player_Torso_ATTACK", L"Player_Torso.png", Vec2{ 0,0 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.05f, 5);
	m_pAnimLeg->CreateAnimation(L"Player_Leg_WALK", L"Player_Leg.png", Vec2{ 0,28 * 7 }, Vec2{ 20,28 }, Vec2{ 0,28 },0.04f, 13);
	m_pAnimLeg->CreateAnimation(L"Player_Leg_IDLE", L"Player_Leg.png", Vec2{ 0,0}, Vec2{ 20,28 }, Vec2{ 0,28 }, 0.1f, 4);
	m_pAnimLeg->CreateAnimation(L"Player_Leg_JUMP", L"Player_Leg.png", Vec2{ 0,28 * 5 }, Vec2{ 20,28 }, Vec2{ 0,28 }, 0.5f, 1);
	pAnim->Play(L"Player_Torso_IDLE",true);
	m_pAnimLeg->Play(L"Player_Leg_IDLE", true);
	m_pPrevAnim = pAnim->GetCurAnim();

	auto pRigid = GetComp<CRigidBody>();
	pRigid->SetIsGround(false);
	
}

CPlayer::CPlayer(const CPlayer& other)
	:CObject{other}
{
}

void CPlayer::update()
{
	if (m_bSlain)
	{
		return;
	}

	CObject::update();
	auto pAnim = GetComp<CAnimator>();
	
	m_ePrevState = m_eCurState;

	updateMove();

	updateState();
	
	updateAnimation();
	
	if (Mgr(CKeyMgr)->GetMouseWheelUp())
	{
		m_iCurWeapon = (m_iCurWeapon + 1) % 3;
	}
	if (Mgr(CKeyMgr)->GetMouseWheelDown())
	{
		m_iCurWeapon = max(0, m_iCurWeapon - 1);
	}

	if (PLAYER_STATE::ATTACK == m_eCurState)
	{
		m_vecWeapon[m_iCurWeapon]->update_weapon();
	}
	else
	{
		m_vecWeapon[m_iCurWeapon]->ReForm();
	}
	
}


void CPlayer::render(HDC _dc)const
{
	if (m_bSlain)
	{
		return;
	}
	Vec2 vPos = GetPos();
	vPos = Mgr(CCamera)->GetRenderPos(vPos);
	auto [vLT, vScale] = Mgr(CCamera)->GetRenderPos(this);
	//Mgr(CCore)->RotateTransform(_dc,m_iDegree,vLT + vScale/2);
	//Mgr(CResMgr)->renderImg(_dc, m_pWeapon, this, Vec2{ 0,0 }, Vec2{ 16,16 });
	//Mgr(CCore)->ResetTransform(_dc);
	//m_iDegree = (m_iDegree + 5)%360;
	//Mgr(CCore)->RotateTransform(m_iDegree,vLT + vScale/2);
	//m_pWeapon->render(_dc);
	CObject::component_render(_dc);
	m_pAnimLeg->component_render(_dc);
	if (PLAYER_STATE::ATTACK == m_eCurState)
	{
		m_vecWeapon[m_iCurWeapon]->render_weapon(_dc);
	}
	
	//m_pWeapon->render(_dc);
	//Mgr(CCore)->ResetTransform();
	//m_iDegree = (m_iDegree + 1);
	
}

void CPlayer::updateState()
{
	m_bIsIDLE = true;
	auto pAnim = GetComp<CAnimator>();
	auto pRigid = GetComp<CRigidBody>();
	
	if (KEY_TAP(KEY::SPACE) && bitwise_absf(pRigid->GetVelocity().y) == 0)
	{
		pRigid->SetIsGround(false);

		pRigid->AddVelocity(Vec2::down * 720.0f);
		//pRigid->SetForce(Vec2{ 0, -1000.0f });
		m_eCurState = PLAYER_STATE::JUMP;
		m_bIsIDLE = false;

	}

	if (m_ePrevState == PLAYER_STATE::ATTACK && pAnim->IsFinish())
	{
		m_eCurState = PLAYER_STATE::IDLE;
		m_bIsAtk = false;
	}

	if (KEY_HOLD(KEY::A))
	{
		pAnim->SetAnimLeft();
		m_pAnimLeg->SetAnimLeft();
		m_eCurState = PLAYER_STATE::WALK;
		m_bIsIDLE = false;
	}

	if (KEY_HOLD(KEY::D))
	{
		pAnim->SetAnimRight();
		m_pAnimLeg->SetAnimRight();
		m_eCurState = PLAYER_STATE::WALK;
		m_bIsIDLE = false;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		if (!m_bIsAtk)
		{
			m_eCurState = PLAYER_STATE::ATTACK;
			m_bIsIDLE = false;
		}
	}

	if (m_bIsAtk)
	{
		m_eCurState = PLAYER_STATE::ATTACK;
		m_bIsIDLE = false;
	}

	if (m_bIsIDLE)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}

	if (abs(pRigid->GetVelocity().y) > 0)
	{
		m_eCurState = PLAYER_STATE::JUMP;

		m_bIsIDLE = false;
	}
}

void CPlayer::updateMove()
{
	auto pRigid = GetComp<CRigidBody>();
	auto vPos = GetPos();
	
	if (KEY_HOLD(KEY::A))
	{
		pRigid->AddVelocity(Vec2{ -20.0f, 0.0f });
	}

	if (KEY_HOLD(KEY::D))
	{
		pRigid->AddVelocity(Vec2{ 20.0f, 0.0f });
	}

	if (KEY_HOLD(KEY::W))
	{
		SetPos({ GetPos().x,GetPos().y - 5.f });
		pRigid->AddVelocity(Vec2{ 0,-300 });
		pRigid->AddForce(Vec2{ 0,-300 });
	}
	if (KEY_HOLD(KEY::S))
	{
		SetPos({ GetPos().x,GetPos().y + 5.f });
		pRigid->AddVelocity(Vec2{ 0,300 });
		pRigid->AddForce(Vec2{ 0,300 });
	}
}

void CPlayer::updateAnimation()
{
	if (m_ePrevState == m_eCurState)
	{
		return;
	}

	auto pAnim = GetComp<CAnimator>();

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		if (!m_bIsAtk)
		{
			pAnim->Play(L"Player_Torso_IDLE", true);
		}
		m_pAnimLeg->Play(L"Player_Leg_IDLE", true);
	}
	break;
	case PLAYER_STATE::WALK:
	{
		if (!m_bIsAtk)
		{
			pAnim->Play(L"Player_Torso_WALK", true);
		}
		m_pAnimLeg->Play(L"Player_Leg_WALK", true);
	}
	break;
	case PLAYER_STATE::JUMP:
	{
		
		
			if (!m_bIsAtk)
			{
				pAnim->Play(L"Player_Torso_JUMP", true);
			}
			m_pAnimLeg->Play(L"Player_Leg_JUMP", true);
		
	}
	break;
	case PLAYER_STATE::ATTACK:
	{
		if (!m_bIsAtk)
		{
			pAnim->Play(L"Player_Torso_ATTACK", false);
			m_bIsAtk = true;
		}
	}
	break;
	default:
		break;
	}
	m_pPrevAnim = pAnim->GetCurAnim();
}

void CPlayer::component_update()
{
	if (m_bSlain)
	{
		return;
	}
	CObject::component_update();
	m_pAnimLeg->component_update();
}

void CPlayer::OnCollision(CCollider* const _pOther)
{

}

void CPlayer::OnCollisionEnter(CCollider* const _pOther)
{
	auto pObj = _pOther->GetOwner();
	const wstring wstrObjName = pObj->GetName().substr(0, pObj->GetName().find(L'_'));

	if (L"Monster" == wstrObjName)
	{
		if (!Mgr(CCamera)->IsCamMove() && !Mgr(CCamera)->IsCamShake())
		{
			Mgr(CCamera)->SetShakeFlag(true);
		}
	}
}

void CPlayer::OnCollisionExit(CCollider* const _pOther)
{
	
	auto pCollider = GetComp<CCollider>();
	auto pRigid = GetComp<CRigidBody>();
	
}

void CPlayer::AddPlayerWeapon()
{
	auto& vecWeapon = Mgr(CSceneMgr)->GetCurScene()->GetPlayerWeapon();
	for (auto& pWeapon : vecWeapon)
	{
		m_vecWeapon.emplace_back(static_cast<CWeapon*>(pWeapon.get()));
	}
}

CoRoutine CPlayer::PlayerRebirthProcess()
{
	Mgr(CCamera)->SetMoveFlag(true);
	int x = TRWorld::WORLD_WIDTH / 2;
	const Vec2 vPlayerDeadPos = TRWorld::WorldToGlobal(Vec2Int(x, m_pTRWolrd->GetTileMap()->GetTopYpos(x))) - Vec2(20.0f, 28.0f);
	StartCoEvent(Mgr(CCamera)->CamMoveCoRoutine(vPlayerDeadPos));
	co_await std::suspend_always{};
	while (Mgr(CCamera)->IsCamMove())
	{
		co_await std::suspend_always{};
	}
	Mgr(CCamera)->FadeIn(1.f);
	SetHP(200);
	SetWillPos(vPlayerDeadPos);
	SetPos(vPlayerDeadPos);
	m_eCurState = PLAYER_STATE::IDLE;
	m_bSlain = false;
	Mgr(CCamera)->SetMoveFlag(false);
	Mgr(CCamera)->SetTarget(this);
	Mgr(CCamera)->update();
	co_return;
}
