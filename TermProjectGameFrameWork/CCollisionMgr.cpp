#include "pch.h"
#include "CCollisionMgr.h"
#include "CCollider.h"
#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SimpleMath.hpp"

CCollisionMgr::CCollisionMgr()
{
	for (auto& bit : m_bitColTable)
	{
		bit.reset();
	}
}

CCollisionMgr::~CCollisionMgr()
{
}

bool CCollisionMgr::IsCollision(const CCollider* const _pLeftCol, const CCollider* const _pRightCol)const
{
	const Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	const Vec2 vLeftScale = _pLeftCol->GetScale();
	 
	const Vec2 vRightPos = _pRightCol->GetFinalPos();
	const Vec2 vRightScale = _pRightCol->GetScale();

	if (bitwise_absf(vLeftPos.x - vRightPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f
		&& bitwise_absf(vLeftPos.y - vRightPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}

	return false;
}


void CCollisionMgr::update()
{
	for (int iRow = 0; iRow < etoi(GROUP_TYPE::END); ++iRow)
	{
		for (int iCol = iRow; iCol < etoi(GROUP_TYPE::END); ++iCol)
		{
			if (m_bitColTable[iRow][iCol])
			{
				CollisionUpdateGroup(static_cast<GROUP_TYPE>(iRow), static_cast<GROUP_TYPE>(iCol));
			}
		}
	}

	const auto& arrCurSceneObj = Mgr(CSceneMgr)->GetCurScene()->GetSceneObj();

	/*for (const auto& vecObj : arrCurSceneObj)
	{
		const auto vecPtr = vecObj.data();
		for (size_t i = 0, size = vecObj.size(); i < size; ++i)
		{
			vecPtr[i]->updateTileCollision();
		}
	}*/

	std::for_each(std::execution::par, std::begin(arrCurSceneObj), std::end(arrCurSceneObj), [](const auto& _vecObj) {
		std::for_each(std::execution::par_unseq, _vecObj.begin(), _vecObj.end(), [](auto& _pObj) {
			_pObj->updateTileCollision();
			});
		});
}

void CCollisionMgr::RegisterGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	int iRow = static_cast<int>(_eLeft);
	int iCol = static_cast<int>(_eRight);

	if (iRow > iCol)
	{
		std::swap(iRow, iCol);
	}

	m_bitColTable[iRow][iCol].flip();
}

void CCollisionMgr::CollisionUpdateGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	auto pCurScene = Mgr(CSceneMgr)->GetCurScene();

	auto& vecLeft = pCurScene->GetGroupObject(_eLeft);
	auto& vecRight = pCurScene->GetGroupObject(_eRight);

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->GetComp<CCollider>())
		{
			continue;
		}

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			if (nullptr == vecRight[j]->GetComp<CCollider>()
				|| vecLeft[i] == vecRight[j])
			{
				continue;
			}

			auto pLeftCol = vecLeft[i]->GetComp<CCollider>();
			auto pRightCol = vecRight[j]->GetComp<CCollider>();

			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			auto iter = m_mapColPrev.find(ID.ID);

			if (m_mapColPrev.end() == iter)
			{
				iter = m_mapColPrev.insert(make_pair(ID.ID, false)).first;
			}

			if (IsCollision(pLeftCol, pRightCol))
			{
				if (iter->second)
				{
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}

				else
				{
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())	
					{												
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}
			else
			{
				if (iter->second)
				{
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}



