#pragma once
#include "CMonster.h"

class CEyeMonster :
    public CMonster
{
public:
    CEyeMonster(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName);
    ~CEyeMonster();

    void update()override;
    void render(HDC _dc)const override;

    virtual void OnCollision(CCollider* const _pOther) override;
    virtual void OnCollisionEnter(CCollider* const _pOther) override;
    virtual void OnCollisionExit(CCollider* const _pOther) override;
};

