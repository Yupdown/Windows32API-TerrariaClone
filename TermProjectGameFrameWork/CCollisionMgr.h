#pragma once


class CCollider;
class CObject;

union COLLIDER_ID
{
	struct {
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};

class CCollisionMgr
	:public Singleton<CCollisionMgr>
{
	friend class Singleton;
	CCollisionMgr();
	~CCollisionMgr();
private:
	unordered_map<ULONGLONG, bool> m_mapColPrev;
	std::bitset<etoi(GROUP_TYPE::END)> m_bitColTable[etoi(GROUP_TYPE::END)];
private:
	bool IsCollision(const CCollider* const _pLeftCol, const CCollider* const _pRightCol)const;
public:
	void update();
	void CollisionUpdateGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void RegisterGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void Reset()
	{
		m_mapColPrev.clear();
		for (auto& bit : m_bitColTable)
		{
			bit.reset();
		}
	}
};

