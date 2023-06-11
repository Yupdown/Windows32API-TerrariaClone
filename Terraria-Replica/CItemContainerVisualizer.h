#pragma once
#include "pch.h"
#include "CObject.h"

class TRItemContainer;

class CItemContainerVisualizer : public CObject
{
private:
    TRItemContainer* item_container;
    bool visible;

public:
    CItemContainerVisualizer(TRItemContainer* _item_container);
    CItemContainerVisualizer(const CItemContainerVisualizer& other);
    ~CItemContainerVisualizer();
    CItemContainerVisualizer* Clone() const override;
    TRItemContainer* GetItemContainer() const;
    void SetVisible(bool value);

    void update() override;
    void render(HDC _dc) const override;
};