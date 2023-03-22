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
public:
	~CCollisionMgr();
private:
	unordered_map<CCollider*, CCollider*> m_mapCollison;
	unordered_map<ULONGLONG, bool> m_mapColPrev;
private:
	bool IsCollision(CCollider* const _pLeftCol, CCollider* const _pRightCol);
public:
	void RegisterCollision(CObject* const _pLeft, CObject* const _pRight);
	void update();
	void Reset()
	{
		m_mapCollison.clear();
		m_mapColPrev.clear();
	}
};

