#pragma once
#include "CObject.h"
#include "TRItemStack.h"

class CAcquireItemText : public CObject
{
private:
    TRItemStack m_item;
    float m_time;

public:
    CAcquireItemText(TRItemStack item);
    CAcquireItemText(const CAcquireItemText& other);

    CAcquireItemText* Clone() const override;

    void update() override;
    void render(HDC _dc) const override;
};