#pragma once
#include "CMonster.h"

class CZombie :
    public CMonster
{
private:
    float m_charge_time;

public:
    CZombie(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName);
    ~CZombie();

    void update()override;
    void render(HDC _dc)const override;

    virtual void OnCollision(CCollider* const _pOther) override;
    virtual void OnCollisionEnter(CCollider* const _pOther) override;
    virtual void OnCollisionExit(CCollider* const _pOther) override;
};

