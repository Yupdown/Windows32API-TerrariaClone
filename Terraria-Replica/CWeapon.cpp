#include "pch.h"
#include "CWeapon.h"
#include "CResMgr.h"
#include "CCore.h"
#include "CCamera.h"
#include "CustomMath.hpp"
#include "CAnimator.h"

CWeapon::CWeapon(CObject* const _pPlayer)
	:m_pPlayer{_pPlayer}
{
}

CWeapon::~CWeapon()
{
}

void CWeapon::SetWeaponImg(wstring_view _wstrFileName, Vec2 _vScale)
{
	m_pWeaponImg = Mgr(CResMgr)->GetImg(_wstrFileName);
	SetName(_wstrFileName);
	SetScale(_vScale);
}

void CWeapon::update()
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
	
	float x = vtempLT.x * cosf((m_fDeg1) * F_DEG2RAD) - vtempLT.y * sinf((m_fDeg1)*F_DEG2RAD);
	float y = vtempLT.y * cosf((m_fDeg1)*F_DEG2RAD) + vtempLT.x * sinf((m_fDeg1)*F_DEG2RAD);


	x += vPos.x;
	y += vPos.y;

	m_AccfDeg1 += 90.f / 0.18f * DT;
	m_AccfDeg2 += 90.f / 0.2f * DT;
	

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

void CWeapon::render(HDC _dc) const
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
}
