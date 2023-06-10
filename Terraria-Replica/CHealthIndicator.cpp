#include "pch.h"
#include "CHealthIndicator.h"
#include "CResMgr.h"
#include "Vec2Int.hpp"

CHealthIndicator::CHealthIndicator()
{
    image_background = Mgr(CResMgr)->GetImg(L"Health_Background.png");
    image_cell = Mgr(CResMgr)->GetImg(L"Health_Full.png");

    m_bIsCamAffected = false;
    health_value = 200;
}

CHealthIndicator::CHealthIndicator(const CHealthIndicator& other) : CObject(other)
{
}

CHealthIndicator::~CHealthIndicator()
{
}

CHealthIndicator* CHealthIndicator::Clone() const
{
    return new CHealthIndicator(*this);
}

void CHealthIndicator::SetHealthValue(int value)
{
    health_value = value;
}

void CHealthIndicator::update()
{
}

void CHealthIndicator::render(HDC _dc) const
{
    Vec2Int back_size = Vec2Int(248, 38);
    Mgr(CResMgr)->renderImg(_dc, image_background, m_vPos, back_size, Vec2Int::zero, back_size);

    for (int i = 0; i < 10; ++i)
    {
        if (i * 20 >= health_value)
            continue;

        Vec2 pos = m_vPos + Vec2(15.5f + 24.0f * i, 19.5f);
        Vec2 size = Vec2(22.0f, 22.0f);
        Vec2 d_size = size;

        if ((i + 1) * 20 > health_value)
        {
            float fraction = (float)(health_value - i * 20) / 20.0f + sinf(0.004f * clock()) * 0.15f;
            d_size *= fraction;
        }

        Mgr(CResMgr)->renderImg(_dc, image_cell, pos - d_size * 0.5f, d_size, Vec2Int::zero, size);
    }
}
