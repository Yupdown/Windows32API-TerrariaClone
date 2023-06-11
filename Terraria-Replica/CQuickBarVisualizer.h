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
    bool visible;

public:
    CQuickBarVisualizer(TRItemContainer* containers[10]);
    CQuickBarVisualizer(const CQuickBarVisualizer& other);
    ~CQuickBarVisualizer();
    void SetSelectIndex(int index);
    void AddContainerVisualizers(CScene* scene);
    void SetVisible(bool value);

    CQuickBarVisualizer* Clone() const override;

    void update() override;
    void render(HDC _dc) const override;
};

