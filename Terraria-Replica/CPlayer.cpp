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

	//SetScale(Vec2{ 150., 150. });
}

CPlayer::CPlayer(const CPlayer& other)
{
}

void CPlayer::update()
{
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
	//pRigid->SetIsGround(true);
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

	

	if (pRigid->IsGround())
	{
		if (KEY_TAP(KEY::SPACE))
		{
			pRigid->SetIsGround(false);
			SetPos(GetPos() + Vec2{ 0,-10 });
			pRigid->AddVelocity(Vec2{ 0,-500 });
			pRigid->AddForce(Vec2{ 0,-500 });
			m_eCurState = PLAYER_STATE::JUMP;
			m_bIsIDLE = false;
		}
	}
	else
	{
		m_eCurState = PLAYER_STATE::JUMP;
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
		pRigid->SetVelocity({ 0,0 });
	}
}

void CPlayer::updateMove()
{
	auto pRigid = GetComp<CRigidBody>();

	if (KEY_HOLD(KEY::A))
	{
		pRigid->AddVelocity(Vec2{ -1000,0 });
		pRigid->AddForce(Vec2{ -1000,0 });
	}

	if (KEY_HOLD(KEY::D))
	{
		pRigid->AddVelocity(Vec2{ 1000,0 });
		pRigid->AddForce(Vec2{ 1000,0 });
	}
	if (KEY_HOLD(KEY::W))
	{
		SetPos({ GetPos().x,GetPos().y - 5.f });
		pRigid->AddVelocity(Vec2{ 0,-1000 });
		pRigid->AddForce(Vec2{ 0,-1000 });
	}
	if (KEY_HOLD(KEY::S))
	{
		SetPos({ GetPos().x,GetPos().y + 5.f });
		pRigid->AddVelocity(Vec2{ 0,1000 });
		pRigid->AddForce(Vec2{ 0,1000 });
	}
}

void CPlayer::updateAnimation()
{
	if (m_ePrevState == m_eCurState)
	{
		return;
	}

	auto pAnim = GetComp<CAnimator>();
	auto pRigid = GetComp<CRigidBody>();

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
	Vec2Int vStart = TRWorld::GlobalToWorld(GetPos() + GetScale()/2.f);

	Vec2Int vEnd = TRWorld::GlobalToWorld(GetPos() - GetScale() / 2.f);

	auto playerLeft = GetPos().x - GetScale().x / 2;
	auto playerTop = GetPos().y - GetScale().y / 2;
	auto playerRight = GetPos().x + GetScale().x / 2;
	auto playerBottom = GetPos().y + GetScale().y / 2;

	auto vPos = GetPos();

	auto pRigid = GetComp<CRigidBody>();

	bool flag = false;
		for (int y = vStart.y; y <= vEnd.y; ++y)
		{
			for (int x = vEnd.x; x <= vStart.x; ++x)
			{
				if (m_pTRWolrd->GetTileMap()->GetTile(x, y)->Solid())
				{
					flag = true;
					auto tilepos = TRWorld::WorldToGlobal(Vec2{ (float)x,(float)y });
					float tileLeft = tilepos.x;
					float tileTop = tilepos.y - 16;
					float tileRight = tilepos.x + 16;
					float tileBottom = tilepos.y;
					
					if (playerRight > tileLeft && playerLeft < tileRight &&
						playerBottom > tileTop && playerTop < tileBottom) {

						

						float dx = min(std::abs(playerRight - tileLeft), std::abs(playerLeft - tileRight));
						float dy = min(std::abs(playerBottom - tileTop), std::abs(playerTop - tileBottom));
						
						if (dx < dy)
						{
							//if (!pRigid->IsGround())
							{
								if (playerRight - tileLeft < tileRight - playerLeft)
								{

									vPos.x = tileLeft - GetScale().x/2 ;
								}
								else
								{

									vPos.x = tileRight + GetScale().x / 2;
								}
							}
						}
						else
						{

							if (playerBottom - tileTop < tileBottom - playerTop)
							{
								if (!pRigid->IsGround())
								{
									//if (!m_bPrevCol)
									{
										vPos.y = tileTop - GetScale().y/2 ;
										pRigid->SetIsGround(true);
										pRigid->SetVelocity({});
									}
								}
								//pRigid->SetIsGround(false);							
							}
							else
							{
								//if (!pRigid->IsGround())
								vPos.y = tileBottom + GetScale().y / 2;
							}

						}
						
					}
					m_bPrevCol = true;
					//flag = false;
				}
				else
				{
					pRigid->SetIsGround(false);
				}
			}
	}
		if (flag)
		{
			SetPos(vPos);
		}
		else
		{
			pRigid->SetIsGround(false);
			m_bPrevCol = false;
		}
}
