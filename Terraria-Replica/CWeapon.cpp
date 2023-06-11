#include "pch.h"
#include "CPlayer.h"
#include "CWeapon.h"
#include "CResMgr.h"
#include "CCore.h"
#include "CCamera.h"
#include "CustomMath.hpp"
#include "CAnimator.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CMonster.h"
#include "CSoundMgr.h"
#include "TRWorld.h"

static std::mt19937 randHitSound{std::random_device{}()};
static std::uniform_int_distribution<> uidHit{0, 2};
static std::uniform_int_distribution<> uidDir{0, 1};

CWeapon::CWeapon(TRWorld* _pTRWorld, CObject* const _pPlayer)
	:m_pPlayer{ _pPlayer }
{
	CreateComponent(COMPONENT_TYPE::COLLIDER);
	Mgr(CSceneMgr)->GetScene(SCENE_TYPE::START)->AddObject(this, GROUP_TYPE::PLAYER_WEAPON);
	m_pTRWolrd = _pTRWorld;
}

CWeapon::~CWeapon()
{
}

void CWeapon::SetWeaponState(const CImage* const _pImg ,wstring_view _wstrWeaponName)
{
	const auto vScale = Vec2{ (float)(_pImg->GetWidth()*2),(float)(_pImg->GetHeight()*2) };
	m_pWeaponImg = _pImg;
	SetName(_wstrWeaponName);
	GetComp<CCollider>()->SetScale(vScale);
	SetScale(vScale);
	const auto wstrName =_wstrWeaponName.substr(_wstrWeaponName.find(L'_')+1);

	if (L"Pickaxe" == wstrName)
	{
		m_iWeaponDmg = 10;
	}
	else if (L"Hammer" == wstrName)
	{
		m_iWeaponDmg = 20;
	}
	else if (L"Longsword" == wstrName)
	{
		m_iWeaponDmg = 50;
	}
	else
	{
		m_iWeaponDmg = 1;
	}
}

//void CWeapon::SetWeaponImg(CImage* _cImage)
//{
//	m_pWeaponImg = _cImage;
//	SetName(L"Weapon");
//	//SetName(L"Weapon_Image");
//	SetScale(Vec2((float)_cImage->GetWidth(), (float)_cImage->GetHeight()) * 2.0f);
//}

void CWeapon::update()
{
	///*const auto vPos = m_pPlayer->GetPos();
	//m_iFlip = m_pPlayer->GetComp<CAnimator>()->GetAnimDir();
	//static Vec2 vLTpos;

	//if (m_iFlip)
	//{
	//	vLTpos.x = vPos.x + m_pPlayer->GetScale().x / 2.f;
	//	vLTpos.y = vPos.y - m_pPlayer->GetScale().y / 2.f;
	//}
	//else
	//{
	//	vLTpos.x = vPos.x - m_pPlayer->GetScale().x / 2.f;
	//	vLTpos.y = vPos.y - m_pPlayer->GetScale().y / 2.f;
	//}

	//vLTpos.y += 2.f;
	//
	//const auto vtempLT = vLTpos - vPos;
	//
	//float x = vtempLT.x * cosf((m_fDeg1) * F_DEG2RAD) - vtempLT.y * sinf((m_fDeg1) * F_DEG2RAD);
	//float y = vtempLT.y * cosf((m_fDeg1) * F_DEG2RAD) + vtempLT.x * sinf((m_fDeg1) * F_DEG2RAD);


	//x += vPos.x;
	//y += vPos.y;

	//m_AccfDeg1 += 180.f / 0.25f * DT;
	//m_AccfDeg2 += 180.f / 0.28f * DT;
	//

	//if (m_iFlip)
	//{
	//	m_fDeg1 = -1.f * m_AccfDeg1;
	//	m_fDeg2 = -1.f * m_AccfDeg2;
	//}
	//else
	//{
	//	m_fDeg1 = 1.f * m_AccfDeg1;
	//	m_fDeg2 = 1.f * m_AccfDeg2;
	//}
	//
	//SetPos(Vec2{ x,y });*/
}

void CWeapon::render(HDC _dc) const
{
	/*const auto [vLT, vScale] = Mgr(CCamera)->GetRenderPos(this);
	Mgr(CCore)->RotateTransform(_dc, m_fDeg2, vLT + vScale / 2.f);
	if (!m_iFlip)
	{
		Mgr(CResMgr)->renderImg(_dc, m_pWeaponImg, this, Vec2{ 0,0 }, Vec2{ 16,16 }, false);
	}
	else
	{
		Mgr(CResMgr)->renderImg(_dc, m_pWeaponImg, this, Vec2{ 0,0 }, Vec2{ 16,16 }, true);
	}
	Mgr(CCore)->ResetTransform(_dc);*/
}

void CWeapon::update_weapon()
{
	if (!m_bActivate)
	{
		return;
	}

	const auto vPos = m_pPlayer->GetPos();
	m_iFlip = m_pPlayer->GetComp<CAnimator>()->GetAnimDir();
	static Vec2 vLTpos;

	if (m_iFlip)
	{
		vLTpos.x = vPos.x + m_pPlayer->GetScale().x / 2.f;
		vLTpos.y = vPos.y - m_pPlayer->GetScale().y / 2.f;
	}
	else
	{
		vLTpos.x = vPos.x - m_pPlayer->GetScale().x / 2.f;
		vLTpos.y = vPos.y - m_pPlayer->GetScale().y / 2.f;
	}

	vLTpos.y += 2.f;

	const auto vtempLT = vLTpos - vPos;

	float x = vtempLT.x * cosf((m_fDeg1)*F_DEG2RAD) - vtempLT.y * sinf((m_fDeg1)*F_DEG2RAD);
	float y = vtempLT.y * cosf((m_fDeg1)*F_DEG2RAD) + vtempLT.x * sinf((m_fDeg1)*F_DEG2RAD);


	x += vPos.x;
	y += vPos.y;

	m_AccfDeg1 += 180.f / 0.25f * DT;
	m_AccfDeg2 += 180.f / 0.28f * DT;


	if (m_iFlip)
	{
		m_fDeg1 = -1.f * m_AccfDeg1;
		m_fDeg2 = -1.f * m_AccfDeg2;
	}
	else
	{
		m_fDeg1 = 1.f * m_AccfDeg1;
		m_fDeg2 = 1.f * m_AccfDeg2;
	}

	SetPos(Vec2{ x,y }); 
}

void CWeapon::render_weapon(HDC _dc) const
{
	if (!m_bActivate)
	{
		return;
	}

	const auto [vLT, vScale] = Mgr(CCamera)->GetRenderPos(this);
	Mgr(CCore)->RotateTransform(_dc, m_fDeg2, vLT + vScale / 2.f);
	const auto vBitSlice = GetScale() / 2.f;
	if (!m_iFlip)
	{
		Mgr(CResMgr)->renderImg(_dc, m_pWeaponImg, this, Vec2{ 0,0 }, vBitSlice, false);
	}
	else
	{
		Mgr(CResMgr)->renderImg(_dc, m_pWeaponImg, this, Vec2{ 0,0 }, vBitSlice, true);
	}
	Mgr(CCore)->ResetTransform(_dc);
	CObject::component_render(_dc);
}

void CWeapon::OnCollision(CCollider* const _pOther)
{
	
}

void CWeapon::OnCollisionEnter(CCollider* const _pOther)
{
	if (!m_bActivate)
	{
		return;
	}

	auto pObj = _pOther->GetOwner();
	const wstring wstrObjName = pObj->GetName().substr(0,pObj->GetName().find(L'_'));

	if (L"Monster" == wstrObjName)
	{
		auto pMon = (CMonster*)pObj;
		auto pMonRigid = pMon->GetComp<CRigidBody>();
		auto vDir = pMon->GetPos() - m_pPlayer->GetPos();
		vDir.y = 0;
		pMonRigid->SetLimitBreak();
		//pPlayer->GetComp<CRigidBody>()->SetLimitBreak();
		//pPlayer->GetComp<CRigidBody>()->SetVelocity({});
		Vec2 vForce = {};
		if (IsFloatZero(vDir.x))
		{
			vForce.x = uidDir(randHitSound) ? 1.f : -1.f;
		}
		else if (vDir.x < 0.f)
		{
			vForce.x = -1.f;
		}
		else
		{
			vForce.x = 1.f;
		}
		
		pMon->GetComp<CRigidBody>()->AddVelocity(vForce * 500.f * 2.f);
		pMon->GetComp<CRigidBody>()->AddForce(vForce * 500.f * 2.f);
		pMonRigid->component_update();

		if (m_bDmgOnce)
		{
			return;
		}
		m_bDmgOnce = true;
		Mgr(CSoundMgr)->PlayEffect("NPC_Hit_1.wav", 0.5f);
		m_pTRWolrd->FloatDamageText(m_iWeaponDmg, pMon->GetPos() - Vec2::up * 24.0f, (COLORREF)0x0000B0FF);

		pMon->SetHP(pMon->GetHP() - m_iWeaponDmg);
		if (pMon->GetHP() <= 0)
		{
			if (pObj->GetName() == L"Monster_Zombie")
			{
				Mgr(CSoundMgr)->PlayEffect("NPC_Killed_2.wav", 0.5f);
			}
			else
			{
				Mgr(CSoundMgr)->PlayEffect("NPC_Killed_1.wav", 0.5f);
			}
			DeleteObj(pObj);

			if (L"Monster_CthulhuEye" == pMon->GetName())
			{
				Mgr(CSoundMgr)->PlayBGM("03. Overworld Day.mp3", 0.1f);
				Mgr(CCamera)->FadeOut(1.f);
				Mgr(CCamera)->FadeIn(1.f);
			}
		}
	}
}

void CWeapon::OnCollisionExit(CCollider* const _pOther)
{
	auto pObj = _pOther->GetOwner();
	const wstring wstrObjName = pObj->GetName().substr(0, pObj->GetName().find(L'_'));

	if (L"Monster" == wstrObjName)
	{
		pObj->GetComp<CRigidBody>()->SetLimitOrigin();
	}
}
