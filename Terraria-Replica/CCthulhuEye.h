#pragma once
#include "CMonster.h"

class CBossHealthBar;

class CCthulhuEye 
    : public CMonster
{
private:
    int m_phase;
    int m_pattern;
    int m_pattern_parameter;
    float m_pattern_time;
    Vec2 m_charge_direction;
    CBossHealthBar* m_pHpBar;
public:
    CCthulhuEye(TRWorld* const _trWorld, wstring_view _wstrMonName, wstring_view _wstrMonImgName);
    ~CCthulhuEye();

    void update()override;
    void render(HDC _dc)const override;

    void UpdatePatternState();
    void Charge();

};