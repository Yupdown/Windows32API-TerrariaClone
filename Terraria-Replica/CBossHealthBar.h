#pragma once
#include "CObject.h"

class CCthulhuEye;

class CBossHealthBar :
    public CObject
{
private:
    CCthulhuEye* m_pBoss = nullptr;
    const CImage* m_pHealthBarImg = nullptr;
    Vec2 m_vBarPos = {};
    Vec2 m_vHpPos = {};
    Vec2 m_vBarScale = {};
    Vec2 m_vHpScale{};

public:
    CBossHealthBar(CCthulhuEye* const _pBoss, wstring_view _fileName);
    ~CBossHealthBar();
    
    CLONE(CBossHealthBar)

    void update()override;
    void render(HDC _dc)const override;
};

