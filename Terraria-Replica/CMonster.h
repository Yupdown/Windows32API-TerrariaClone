#pragma once
#include "CObject.h"

class CMonster :
    public CObject
{
private:
   
public:
    CMonster(TRWorld* const _trWorld, wstring_view _wstrMonName ,wstring_view _wstrMonImgName);
    ~CMonster();
    CMonster* Clone()const override
    {
        auto p = new CMonster{ *this };
        return p;
    }

    void update()override;
    void render(HDC _dc) const override;

    virtual void OnCollision(CCollider* const _pOther) override;
    virtual void OnCollisionEnter(CCollider* const _pOther) override;
    virtual void OnCollisionExit(CCollider* const _pOther) override;

};

