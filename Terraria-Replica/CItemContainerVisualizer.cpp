#include "pch.h"
#include "CItemContainerVisualizer.h"
#include "CResMgr.h"
#include "TRItemContainer.h"
#include "TRItem.h"
#include "Vec2Int.hpp"

CItemContainerVisualizer::CItemContainerVisualizer(TRItemContainer* _item_container)
{
    item_container = _item_container;
    m_bIsCamAffected = false;
    visible = true;
}

CItemContainerVisualizer::CItemContainerVisualizer(const CItemContainerVisualizer& other) : CObject(other)
{

}

CItemContainerVisualizer::~CItemContainerVisualizer()
{
}

void CItemContainerVisualizer::update()
{

}

void CItemContainerVisualizer::render(HDC _dc) const
{
    if (!visible || item_container->Blank())
        return;

    CImage* image_item = item_container->GetItemStack().GetItem()->GetItemElement();
    Vec2Int image_size = Vec2Int(image_item->GetWidth(), image_item->GetHeight());
    Mgr(CResMgr)->renderImg(_dc, image_item, m_vPos - image_size, image_size * 2, Vec2Int::zero, image_size);

    static wchar_t buffer[64];
    int stack_size = item_container->GetItemStack().GetStackSize();

    if (stack_size > 1)
    {
        wsprintf(buffer, L"%d", stack_size);
        renderText(_dc, 0x00000000, m_vPos + Vec2::one * 8.0f + Vec2::up * 2.0f, buffer);
        renderText(_dc, 0x00FFFFFF, m_vPos + Vec2::one * 8.0f, buffer);
    }
}

CItemContainerVisualizer* CItemContainerVisualizer::Clone() const
{
    auto p = new CItemContainerVisualizer{ *this };
    return p;
}

TRItemContainer* CItemContainerVisualizer::GetItemContainer() const
{
    return item_container;
}

void CItemContainerVisualizer::SetVisible(bool value)
{
    visible = value;
}
