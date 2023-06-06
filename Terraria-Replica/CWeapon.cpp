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

CWeapon::CWeapon(CObject* const _pPlayer)
	:m_pPlayer{_pPlayer}
{
	CreateComponent(COMPONENT_TYPE::COLLIDER);
	Mgr(CSceneMgr)->GetCurScene()->AddObject(this, GROUP_TYPE::PLAYER_WEAPON);
	SetScale({ 32,32 });
}

CWeapon::~CWeapon()
{
}

void CWeapon::SetWeaponImg(wstring_view _wstrFileName,wstring_view _wstrWeaponName ,Vec2 _vScale)
{
	m_pWeaponImg = Mgr(CResMgr)->GetImg(_wstrFileName);
	SetName(_wstrWeaponName);
	GetComp<CCollider>()->SetScale(_vScale);
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
	const auto [vLT, vScale] = Mgr(CCamera)->GetRenderPos(this);
	Mgr(CCore)->RotateTransform(_dc, m_fDeg2, vLT + vScale / 2.f);
	if (!m_iFlip)
	{
		Mgr(CResMgr)->renderImg(_dc, m_pWeaponImg, this, Vec2{ 0,0 }, Vec2{ 16,16 }, false);
	}
	else
	{
		Mgr(CResMgr)->renderImg(_dc, m_pWeaponImg, this, Vec2{ 0,0 }, Vec2{ 16,16 }, true);
	}
	Mgr(CCore)->ResetTransform(_dc);
	CObject::component_render(_dc);
}

void CWeapon::OnCollision(CCollider* const _pOther)
{
}

void CWeapon::OnCollisionEnter(CCollider* const _pOther)
{
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
		if (pMon->GetHP() <= 0)
		{
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
