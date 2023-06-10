#pragma once
#include "pch.h"
#include "CObject.h"

class CHealthIndicator : public CObject
{
private:
    CImage* image_background;
    CImage* image_cell;

    int health_value;

public:
    CHealthIndicator();
    CHealthIndicator(const CHealthIndicator& other);
    ~CHealthIndicator();
    CHealthIndicator* Clone() const override;

    void SetHealthValue(int value);

    void update() override;
    void render(HDC _dc) const override;
};
