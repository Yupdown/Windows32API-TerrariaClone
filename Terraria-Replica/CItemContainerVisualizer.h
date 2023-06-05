#pragma once
#include "pch.h"
#include "CObject.h"

class TRItemContainer;

class CItemContainerVisualizer : public CObject
{
private:
    TRItemContainer* item_container;
    CImage* container_background[2];
    bool container_selected;

public:
    CItemContainerVisualizer(TRItemContainer* _item_container);
    CItemContainerVisualizer(const CItemContainerVisualizer& other);

    CItemContainerVisualizer* Clone() const override;

    void SetSelected(bool value);

    void update() override;
    void render(HDC _dc) const override;
};