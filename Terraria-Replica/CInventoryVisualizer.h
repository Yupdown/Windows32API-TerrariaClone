#pragma once
#include "pch.h"
#include "CObject.h"

class TRItemContainer;
class CItemContainerVisualizer;

class CInventoryVisualizer : public CObject
{
private:
    static constexpr int NUMOF_ITEMS = 50;
    static constexpr int NUMOF_COLUMNS = 10;

    CItemContainerVisualizer* container_visualizers[NUMOF_ITEMS];
    TRItemContainer* cursor_container;
    CItemContainerVisualizer* cursor_visualizer;
    CImage* container_background;
    bool visible;

public:
    CInventoryVisualizer(TRItemContainer* containers[NUMOF_ITEMS]);
    CInventoryVisualizer(const CInventoryVisualizer& other);
    ~CInventoryVisualizer();

    void AddContainerVisualizers(CScene* scene);
    void SetVisible(bool value);
    bool HandleMouseInput();

    CInventoryVisualizer* Clone() const override;

    void update() override;
    void render(HDC _dc) const override;
};
