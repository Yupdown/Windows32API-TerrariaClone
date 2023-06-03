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
#include "Vec2Int.hpp"

short bitwise_abs(const short x) 
{
	const short mask = x >> (sizeof(short) * 8 - 1);
	return (x + mask) ^ mask;
}

CPlayer::CPlayer(TRWorld* const _trWorld)
{
	m_pTRWolrd = _trWorld;
	m_bIsCamAffected = true;
	CreateComponent(COMPONENT_TYPE::COLLIDER);
	CreateComponent(COMPONENT_TYPE::ANIMATOR);
	CreateComponent(COMPONENT_TYPE::RIGIDBODY);

	auto pAnim = GetComp<CAnimator>();
	m_pAnimLeg = make_unique<CAnimator>();
	m_pAnimLeg->SetOwner(this);
	pAnim->CreateAnimation(L"Player_Torso_WALK", L"Player_Torso.png", Vec2{ 20,28 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.2f, 4);
	pAnim->CreateAnimation(L"Player_Torso_IDLE", L"Player_Torso.png", Vec2{ 0,0 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.2f, 1);
	pAnim->CreateAnimation(L"Player_Torso_JUMP", L"Player_Torso.png", Vec2{ 0,28 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.5f, 1);
	pAnim->CreateAnimation(L"Player_Torso_ATTACK", L"Player_Torso.png", Vec2{ 0,0 }, Vec2{ 20,28 }, Vec2{ 20,0 }, 0.07f, 5);
	m_pAnimLeg->CreateAnimation(L"Player_Leg_WALK", L"Player_Leg.png", Vec2{ 0,28 * 5 }, Vec2{ 20,28 }, Vec2{ 0,28 },0.1f, 15);
	m_pAnimLeg->CreateAnimation(L"Player_Leg_IDLE", L"Player_Leg.png", Vec2{ 0,0}, Vec2{ 20,28 }, Vec2{ 0,28 }, 0.1f, 4);
	m_pAnimLeg->CreateAnimation(L"Player_Leg_JUMP", L"Player_Leg.png", Vec2{ 0,28 * 5 }, Vec2{ 20,28 }, Vec2{ 0,28 }, 0.5f, 1);
	pAnim->Play(L"Player_Torso_IDLE",true);
	m_pAnimLeg->Play(L"Player_Leg_IDLE", true);
	m_pPrevAnim = pAnim->GetCurAnim();

	auto pRigid = GetComp<CRigidBody>();
	pRigid->SetIsGround(false);
	//SetScale(Vec2{ 150., 150. });
}

CPlayer::CPlayer(const CPlayer& other)
{
}

void CPlayer::update()
{
	CObject::update();
	auto pAnim = GetComp<CAnimator>();
	
	updateMove();

	updateState();
	
	updateAnimation();
	
	m_ePrevState = m_eCurState;

	//updateTileCollision();
}


void CPlayer::render(HDC _dc)const
{
	Vec2 vPos = GetPos();
	vPos = Mgr(CCamera)->GetRenderPos(vPos);
	auto [vLT, vScale] = Mgr(CCamera)->GetRenderPos(this);
	//Mgr(CCore)->RotateTransform(m_iDegree,vLT + vScale/2);
	
	CObject::component_render(_dc);
	m_pAnimLeg->component_render(_dc);
	//Mgr(CCore)->ResetTransform();
	//m_iDegree = (m_iDegree + 1);
}

void CPlayer::updateState()
{
	m_bIsIDLE = true;
	auto pAnim = GetComp<CAnimator>();
	auto pRigid = GetComp<CRigidBody>();
	

	
		if (KEY_TAP(KEY::SPACE) && abs(pRigid->GetVelocity().y) == 0)
		{
			pRigid->SetIsGround(false);
			
			pRigid->AddVelocity(Vec2{ 0,-500 });
			pRigid->SetForce(Vec2{ 0,-500 });
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
	if (KEY_TAP(KEY::A))
	{
		pRigid->AddVelocity(Vec2{ -300,0 });
	}
	
	if (KEY_HOLD(KEY::A))
	{
		pRigid->AddForce(Vec2{ -300,0 });
	}

	if (KEY_TAP(KEY::D))
	{
		pRigid->AddVelocity(Vec2{ 300,0 });
	}

	if (KEY_HOLD(KEY::D))
	{
		pRigid->AddForce(Vec2{ 300,0 });
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
	CObject::component_update();
	m_pAnimLeg->component_update();
	updateTileCollision();
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

void CPlayer::updateTileCollision()
{
	Vec2 vColScale = Vec2{ 24.f,48.f };
	Vec2Int vStart = TRWorld::GlobalToWorld(m_vWillPos + vColScale /2.f);

	Vec2Int vEnd = TRWorld::GlobalToWorld(m_vWillPos - vColScale / 2.f);
	auto vPos = GetPos();
	const short playerLeft = static_cast<short>(vPos.x - vColScale.x / 2.f);
	const short playerTop = static_cast<short>(vPos.y - vColScale.y / 2.f);
	const short playerRight = static_cast<short>(vPos.x + vColScale.x / 2.f);
	const short playerBottom = static_cast<short>(vPos.y + vColScale.y / 2.f);



	auto pRigid = GetComp<CRigidBody>();
	auto vOriginVelocity = pRigid->GetVelocity();
	bool flag = false;
		for (int y = vStart.y; y <= vEnd.y; ++y)
		{
			for (int x = vEnd.x; x <= vStart.x; ++x)
			{
				if (m_pTRWolrd->GetTileMap()->GetTile(x, y)->Solid())
				{
					flag = true;
					auto tilepos = TRWorld::WorldToGlobal(Vec2{ (float)x,(float)y });
					const short tileLeft = static_cast<short>(tilepos.x);
					const short tileTop = static_cast<short>(tilepos.y - 16.f);
					const short tileRight = static_cast<short>(tilepos.x + 16.f);
					const short tileBottom = static_cast<short>(tilepos.y);


					const short dx = min(bitwise_abs(playerRight - tileLeft), bitwise_abs(playerLeft - tileRight));
					const short dy = min(bitwise_abs(playerBottom - tileTop), bitwise_abs(playerTop - tileBottom));

					if (dx < dy)
					{
						if (playerRight - tileLeft < tileRight - playerLeft)
						{
							if (m_pTRWolrd->GetTileMap()->GetTile(x, y + 1)->Solid())
							{
								m_vWillPos.x = tileLeft - 24.f / 2.f;

							}
						}
						else
						{
							if (m_pTRWolrd->GetTileMap()->GetTile(x, y + 1)->Solid())
							{
								m_vWillPos.x = tileRight + 24.f / 2.f;
							}

						}
					}
					else
					{

						if (playerBottom - tileTop < tileBottom - playerTop)
						{
							m_vWillPos.y = tileTop - 48.f / 2.f;
						}
						else
						{
							m_vWillPos.y = tileBottom + 48.f / 2.f;
						}
						pRigid->SetVelocity({ vOriginVelocity.x,0 });
					}
				}
			}
		}
		SetPos(m_vWillPos);
		if (!flag)
		{
			pRigid->SetIsGround(false);
		}
}
