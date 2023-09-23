#include "pch.h"
#include "CBossHealthBar.h"
#include "CCthulhuEye.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCamera.h"
#include "CCore.h"

CBossHealthBar::CBossHealthBar(CCthulhuEye* const _pBoss, wstring_view _fileName)
{
	SetName(L"UI_BossHpBar");

	m_bIsCamAffected = false;
	m_pBoss = _pBoss;
	m_pHealthBarImg = Mgr(CResMgr)->GetImg(_fileName);

	m_vBarPos = Vec2{};
	m_vBarScale = Vec2{ 516.f, 58.0f };

	m_vHpPos = Vec2{ 30.f,80.f };
	
	SetPos(Vec2{ 450.f,700.f });
	
	Mgr(CSceneMgr)->GetScene(SCENE_TYPE::START)->AddObject(this, GROUP_TYPE::UI);

}

CBossHealthBar::~CBossHealthBar()
{
}

void CBossHealthBar::update()
{
	
}

void CBossHealthBar::render(HDC _dc) const
{
	auto vPos = GetPos();

	float progress = (float)m_pBoss->GetHP() / 200.0f;
	float width = progress * 456.0f;

	Mgr(CResMgr)->renderImg(_dc, m_pHealthBarImg, vPos + Vec2::right * 32.0f, Vec2(width, m_vBarScale.y), Vec2(32.0f, 58.0f), Vec2(2.0f, m_vBarScale.y));
	Mgr(CResMgr)->renderImg(_dc, m_pHealthBarImg, vPos, m_vBarScale, Vec2::zero, m_vBarScale);
}

