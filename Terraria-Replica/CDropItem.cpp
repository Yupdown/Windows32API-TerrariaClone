#include "pch.h"
#include "CCamera.h"
#include "CDropItem.h"
#include "CCollider.h"
#include "CResMgr.h"
#include "CRigidBody.h"
#include "TRItem.h"
#include "Vec2Int.hpp"
#include "CSoundMgr.h"

CDropItem::CDropItem(TRWorld* const _trWorld, TRItemStack _item)
{
	m_pTRWolrd = _trWorld;
	m_bIsCamAffected = true;
	m_item = _item;

	CreateComponent(COMPONENT_TYPE::COLLIDER, Vec2::one * 16.0f);
	CreateComponent(COMPONENT_TYPE::RIGIDBODY);
	GetComp<CRigidBody>()->AddVelocity(Vec2::down * 240.0f);

	SetName(L"DropItem_" + _item.GetItem()->GetName());
}

CDropItem::~CDropItem()
{

}

void CDropItem::update()
{
	CObject::update();

	CRigidBody* rBody = GetComp<CRigidBody>();
	CObject* target = m_pTRWolrd->GetPlayer();
	Vec2 t_vPos = target->GetPos();

	float dist = (t_vPos - m_vPos).length();
	if (dist < 80.0f)
		rBody->AddForce((t_vPos - m_vPos).Normalize() * 5000.0f);
}

void CDropItem::render(HDC _dc) const
{
	CObject::component_render(_dc);

	if (m_item.Null())
		return;

	CImage* image = m_item.GetItem()->GetItemElement();
	Vec2 pos = Mgr(CCamera)->GetRenderPos(m_vPos);
	Vec2Int size = Vec2Int(image->GetWidth(), image->GetHeight());
	Mgr(CResMgr)->renderImg(_dc, image, pos - Vec2Int(size.x, size.y * 2 - 6), size * 2, Vec2Int::zero, size);
}

void CDropItem::OnCollision(CCollider* const _pOther)
{

}

void CDropItem::OnCollisionEnter(CCollider* const _pOther)
{
	auto pObj = _pOther->GetOwner();
	if (pObj == m_pTRWolrd->GetPlayer())
	{
		m_pTRWolrd->AddItemToInventory(m_item);
		Mgr(CSoundMgr)->PlayEffect("Grab.wav", 0.5f);
		DeleteObj(this);
	}
}

void CDropItem::OnCollisionExit(CCollider* const _pOther)
{

}