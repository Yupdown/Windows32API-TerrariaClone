#pragma once
#include "CObject.h"

class CAcquireItemText : public CObject
{
private:
    std::wstring m_text;
    COLORREF m_color;
    float m_time;

public:
    CAcquireItemText(std::wstring text, COLORREF color = (COLORREF)0x00FFFFFF);
    CAcquireItemText(const CAcquireItemText& other);

    CAcquireItemText* Clone() const override;

    void update() override;
    void render(HDC _dc) const override;
};