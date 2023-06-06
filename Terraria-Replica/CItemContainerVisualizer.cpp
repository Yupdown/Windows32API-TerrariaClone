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
}

CItemContainerVisualizer::CItemContainerVisualizer(const CItemContainerVisualizer& other) : CObject(other)
{

}

void CItemContainerVisualizer::update()
{

}

void CItemContainerVisualizer::render(HDC _dc) const
{
    CObject::render(_dc);

    if (item_container->Blank())
        return;

    CImage* image_item = item_container->GetItemStack().GetItem()->GetItemElement();
    Vec2Int image_size = Vec2Int(image_item->GetWidth(), image_item->GetHeight());
    Mgr(CResMgr)->renderImg(_dc, image_item, m_vPos - image_size, image_size * 2, Vec2Int::zero, image_size);

    static wchar_t buffer[64];
    wsprintf(buffer, L"%d", item_container->GetItemStack().GetStackSize());
    renderText(_dc, m_vPos + Vec2::one * 8.0f, buffer);
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
