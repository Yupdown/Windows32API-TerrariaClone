#include "pch.h"
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

static std::mt19937 randHitSound{std::random_device{}()};
static std::uniform_int_distribution<> uidHit{0, 1};

CWeapon::CWeapon(CObject* const _pPlayer)
	:m_pPlayer{_pPlayer}
{
	CreateComponent(COMPONENT_TYPE::COLLIDER);
	Mgr(CSceneMgr)->GetCurScene()->AddObject(this, GROUP_TYPE::PLAYER_WEAPON);
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
		pMon->SetHP(pMon->GetHP() - 50);
		auto pMonRigid = pMon->GetComp<CRigidBody>();
		auto vMonDir = pMonRigid->GetVelocity().Normalize();
		pMonRigid->SetLimitBreak();
		pMonRigid->AddVelocity(vMonDir * -1 * 1000);
		pMonRigid->AddForce(vMonDir * -1 * 1000);
		pMonRigid->component_update();

		Mgr(CSoundMgr)->PlayEffect("NPC_Hit_1.wav", 0.5f);
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
		}
	}
}

void CWeapon::OnCollisionExit(CCollider* const _pOther)
{
	auto pObj = _pOther->GetOwner();
	const wstring wstrObjName = pObj->GetName().substr(0, pObj->GetName().find(L'_'));

	if (L"Monster" == wstrObjName)
	{
		
	}
}
