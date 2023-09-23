#include "pch.h"
#include "CCthulhuEye.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCore.h"
#include "CCamera.h"
#include "CustomMath.hpp"
#include "SimpleMath.hpp"
#include "CCollider.h"
#include "CSoundMgr.h"
#include "CBossHealthBar.h"

extern HDC g_particleDC;

CCthulhuEye::CCthulhuEye(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName) : CMonster(_trWorld, _wstrMonName, _wstrMonImgName)
{
	auto pAnim = GetComp<CAnimator>();
	auto pRigid = GetComp<CRigidBody>();
	auto pCol = GetComp<CCollider>();

	pAnim->CreateAnimation(GetName() + L"Phase_1", _wstrMonImgName, Vec2{ 0,0 }, Vec2{ 55,83 }, Vec2{ 0,83 }, 0.2f, 3);
	pAnim->CreateAnimation(GetName() + L"Phase_2", _wstrMonImgName, Vec2{ 0,249 }, Vec2{ 55,83 }, Vec2{ 0,83 }, 0.2f, 3);
	pAnim->Play(GetName() + L"Phase_2", true);

	pRigid->SetGravity(false);
	pRigid->SetFriction(160.0f);
	pRigid->SetMaxVelocity(Vec2(640.0f, 640.0f));

	m_phase = 0;
	m_pattern = 0;
	m_pattern_parameter = 3;
	m_pattern_time = 5.0f;

	m_pHpBar = new CBossHealthBar{ this, L"UI_BossBar.png" };
}

CCthulhuEye::~CCthulhuEye()
{
	auto& uiVec = Mgr(CSceneMgr)->GetCurScene()->GetUIGroup();
	std::erase_if(uiVec, [this](const auto& p) {return p.get() == m_pHpBar; });
	m_pHpBar = nullptr;
}

void CCthulhuEye::update()
{
	if (Mgr(CCamera)->IsCamMove())
	{
		return;
	}

	CMonster::update();

	UpdatePatternState();

	auto vPlayerPos = Mgr(CSceneMgr)->GetCurScene()->GetPlayer()->GetPos();
	auto pAnim = GetComp<CAnimator>();
	auto vCurPos = GetPos();

	auto pRigid = GetComp<CRigidBody>();

	Vec2 target_vPos = vPlayerPos + Vec2::down * 160.0f;
	Vec2 vVel = pRigid->GetVelocity();
	Vec2 vDelta = (target_vPos - vCurPos).Normalize();

	if (m_pattern == 0)
		pRigid->AddForce(vDelta * 800.0f);
	else if (m_pattern_time < 1.0f)
		pRigid->AddForce(vDelta * 400.0f);
	else
		pRigid->SetVelocity(m_charge_direction * 640.0f);

	if (vDelta.x > 0)
		pAnim->SetAnimLeft();
	else
		pAnim->SetAnimRight();
}

void CCthulhuEye::render(HDC _dc) const
{
	auto vPlayerPos = Mgr(CSceneMgr)->GetCurScene()->GetPlayer()->GetPrevPos();
	auto vCurPos = GetPrevPos();
	auto vDir = Vec2::zero;
	if (m_pattern == 0 || m_pattern_time < 1.0f)
		vDir = (vPlayerPos - GetPos()).Normalize();
	else
		vDir = (GetPos() - vCurPos).Normalize();

	const float fDeg = atan2f(vDir.y, vDir.x) - F_PI * 0.5f;
	const auto [vLT, vScale] = Mgr(CCamera)->GetRenderPos(this);

	Mgr(CCore)->RotateTransform(_dc, fDeg * F_RAD2DEG, vLT + vScale / 2.f);
	CMonster::render(_dc);
	Mgr(CCore)->ResetTransform(_dc);
}

void CCthulhuEye::UpdatePatternState()
{
	m_pattern_time -= DT;

	if (m_pattern_time <= 0.0f)
	{
		if (m_pattern == 0)
		{
			m_pattern = 1;
			m_pattern_parameter = 3;
		}

		if (m_pattern == 1)
		{
			m_pattern_parameter -= 1;
			if (m_pattern_parameter < 0)
			{
				m_pattern = 0;
				m_pattern_time = 5.0f;
			}
			else
			{
				Charge();
				m_pattern_time = 2.0f;
			}
		}
	}
}

void CCthulhuEye::Charge()
{
	auto vPlayerPos = Mgr(CSceneMgr)->GetCurScene()->GetPlayer()->GetPos();
	m_charge_direction = (vPlayerPos - GetPos()).Normalize();

	Mgr(CSoundMgr)->PlayEffect("Roar_0.wav", 0.5f);
}

