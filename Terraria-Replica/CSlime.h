#pragma once
#include "CMonster.h"

class CSlime :
    public CMonster
{
private:
    float m_charge_time;

public:

   CSlime(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName);
   ~CSlime();

    void update()override;
    void render(HDC _dc)const override;

    virtual void OnCollision(CCollider* const _pOther) override;
    virtual void OnCollisionEnter(CCollider* const _pOther) override;
    virtual void OnCollisionExit(CCollider* const _pOther) override;

    void Charge();
};

