#pragma once
#include "CObject.h"

class CWeapon :
    public CObject
{
private:
    CObject* m_pPlayer = nullptr;
    const CImage* m_pWeaponImg = nullptr;

    float m_fDeg1 = 0;
    float m_fDeg2 = -75.f;

    float m_AccfDeg1 = 0;
    float m_AccfDeg2 = -75.f;

    int m_iFlip = 0;

    bool m_bActivate = true;

    int m_iWeaponDmg = 1;

    bool m_bDmgOnce = false;
public:
    CWeapon(TRWorld* _pTRWorld, CObject* const _pPlayer);
    ~CWeapon();

    CWeapon* Clone()const override
    {
        auto pWeapon = new CWeapon{*this};
        pWeapon->m_pPlayer = nullptr;
        return pWeapon;
    }

    void SetWeaponState(const CImage* const _pImg, wstring_view _wstrWeaponName);
   //void SetWeaponImg(CImage* _cImage);

    void update()override;
    void render(HDC _dc)const override;
    void ReForm()
    {
        m_fDeg1 = 0.f;
        m_fDeg2 = -75.f;

        m_AccfDeg1 = 0.f;
        m_AccfDeg2 = -75.f;

        m_bDmgOnce = false;

        SetPos({ 0,0 });
    }

    void update_weapon();
    void render_weapon(HDC _dc)const;

    void SetActivate(bool _b) { m_bActivate = _b; }
    bool IsActivate()const { return m_bActivate; }

    virtual void OnCollision(CCollider* const _pOther)override;
    virtual void OnCollisionEnter(CCollider* const _pOther)override;
    virtual void OnCollisionExit(CCollider* const _pOther)override;
    void SetDmg(const int _iDmg) { m_iWeaponDmg = _iDmg; }
};

