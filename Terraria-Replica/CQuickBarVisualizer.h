#pragma once
#include "pch.h"
#include "CObject.h"

class TRItemContainer;
class CItemContainerVisualizer;
class CScene;

class CQuickBarVisualizer : public CObject
{
private:
    CItemContainerVisualizer* container_visualizers[10];
    CImage* container_background[2];
    int select_index;

public:
    CQuickBarVisualizer(TRItemContainer* containers[10]);
    CQuickBarVisualizer(const CQuickBarVisualizer& other);

    void SetSelectIndex(int index);
    void AddContainerVisualizers(CScene* scene);

    CQuickBarVisualizer* Clone() const override;

    void update() override;
    void render(HDC _dc) const override;
};

