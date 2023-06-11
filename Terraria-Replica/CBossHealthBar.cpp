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
	m_vBarScale = Vec2{ 516.f,60.f };

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

	m_pHealthBarImg->TransparentBlt(_dc
		, (int)vPos.x
		, (int)vPos.y
		, (int)m_vBarScale.x
		, (int)m_vBarScale.y
		, (int)m_vBarPos.x
		, (int)m_vBarPos.y
		, (int)m_vBarScale.x
		, (int)m_vBarScale.y
		, RGB(255, 0, 255));
}

