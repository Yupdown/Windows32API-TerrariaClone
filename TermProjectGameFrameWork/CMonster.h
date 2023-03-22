#pragma once
#include "CObject.h"

class CMonster
	:public CObject
{
public:
	CMonster();
	~CMonster();
	CMonster* Clone()const override
	{
		CMonster* NewMon = new CMonster{ *this };
		return NewMon;
	}
	virtual void OnCollision(CCollider* const _pOther);
	virtual void OnCollisionEnter(CCollider* const _pOther);
	virtual void OnCollisionExit(CCollider* const _pOther);
};

