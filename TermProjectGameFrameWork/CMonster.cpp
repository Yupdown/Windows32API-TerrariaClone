#include "pch.h"
#include "CMonster.h"
#include "CCore.h"
#include "CCompFactory.h"

CMonster::CMonster()
{
	SetPos(Vec2{ 50,50 });
	SetScale(Vec2{ 100,100 });
	CreateComponent(COMPONENT_TYPE::COLLIDER);
}

CMonster::~CMonster()
{
}

void CMonster::OnCollision(CCollider* const _pOther)
{
}

void CMonster::OnCollisionEnter(CCollider* const _pOther)
{
	Mgr(CCore)->ChangeBrush(BRUSH_TYPE::BLACK);
}

void CMonster::OnCollisionExit(CCollider* const _pOther)
{
	Mgr(CCore)->ChangeBrush(BRUSH_TYPE::WHITE);
}
