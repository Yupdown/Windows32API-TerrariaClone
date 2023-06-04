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
public:
    CWeapon(CObject* const _pPlayer);
    ~CWeapon();

    CWeapon* Clone()const override
    {
        auto pWeapon = new CWeapon{*this};
        pWeapon->m_pPlayer = nullptr;
        return pWeapon;
    }

    void SetWeaponImg(wstring_view _wstrFileName,Vec2 _vScale);
    void update()override;
    void render(HDC _dc)const override;
    void ReForm()
    {
        m_fDeg1 = 0.f;
        m_fDeg2 = -75.f;

        m_AccfDeg1 = 0.f;
        m_AccfDeg2 = -75.f;
    }
};

