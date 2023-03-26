#include "pch.h"
#include "CCollisionMgr.h"
#include "CCollider.h"
#include "CObject.h"

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

bool CCollisionMgr::IsCollision(CCollider* const _pLeftCol, CCollider* const _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vLeftPos.x - vRightPos.x) <= (vLeftScale.x + vRightScale.x) / 2.
		&& abs(vLeftPos.y - vRightPos.y) <= (vLeftScale.y + vRightScale.y) / 2.)
	{
		return true;
	}

	return false;
}

void CCollisionMgr::RegisterCollision(CObject* const _pLeft, CObject* const _pRight)
{
	auto LeftCol = _pLeft->GetComp<CCollider>();
	auto RightCol = _pRight->GetComp<CCollider>();
	COLLIDER_ID ID = {};
	ID.Left_id = LeftCol->GetID();
	ID.Right_id = RightCol->GetID();
	m_mapColPrev.insert(make_pair(ID.ID, false));
	m_mapCollison.insert(make_pair(LeftCol, RightCol));
}

void CCollisionMgr::update()
{
	for (auto& [left, right] : m_mapCollison)
	{
		const bool nowCollison = IsCollision(left, right);
		COLLIDER_ID ID = {};
		ID.Left_id = left->GetID();
		ID.Right_id = right->GetID();
		const bool prevCol = m_mapColPrev[ID.ID];
		if (nowCollison)
		{
			if (prevCol)
			{
				if (left->GetOwner()->IsDead() || right->GetOwner()->IsDead())
				{
					left->OnCollisionExit(right);
					right->OnCollisionExit(left);
				}
				else
				{
					left->OnCollision(right);
					right->OnCollision(left);
				}
			}
			else
			{
				if (left->GetOwner()->IsDead() || right->GetOwner()->IsDead())
				{
					continue;
				}
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);
			}
		}
		else
		{
			if (prevCol)
			{
				left->OnCollisionExit(right);
				right->OnCollisionExit(left);
			}
		}
		m_mapColPrev[ID.ID] = nowCollison;
	}
	std::erase_if(m_mapCollison, [](const auto& col) {
		return col.first->GetOwner()->IsDead() || col.second->GetOwner()->IsDead();
		});
}



