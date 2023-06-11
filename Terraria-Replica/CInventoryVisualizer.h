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

    TRItemContainer* cursor_container;

    CItemContainerVisualizer* container_visualizers[NUMOF_ITEMS];
    CItemContainerVisualizer* container_armor_visualizers[3];
    CItemContainerVisualizer* cursor_visualizer;

    CImage* container_background[2];
    bool visible;

public:
    CInventoryVisualizer(TRItemContainer* containers[NUMOF_ITEMS], TRItemContainer* armor_containers[3]);
    CInventoryVisualizer(const CInventoryVisualizer& other);
    ~CInventoryVisualizer();

    void AddContainerVisualizers(CScene* scene);
    void SetVisible(bool value);
    bool HandleMouseInput();

    CInventoryVisualizer* Clone() const override;

    void update() override;
    void render(HDC _dc) const override;
};
