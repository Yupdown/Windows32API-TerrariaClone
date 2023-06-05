#include "pch.h"
#include "CItemContainerVisualizer.h"
#include "CResMgr.h"
#include "TRItemContainer.h"
#include "TRItem.h"
#include "Vec2Int.hpp"

CItemContainerVisualizer::CItemContainerVisualizer(TRItemContainer* _item_container)
{
    item_container = _item_container;
    container_selected = false;
    m_bIsCamAffected = false;

    container_background[0] = Mgr(CResMgr)->GetImg(L"Inventory_Back.png");
    container_background[1] = Mgr(CResMgr)->GetImg(L"Inventory_Back_Select.png");
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
    
    Mgr(CResMgr)->renderImg(_dc, container_background[container_selected ? 1 : 0], m_vPos - m_vScale * 0.5f, m_vScale, Vec2Int::zero, Vec2Int::one * 26);

    if (!item_container->Blank())
    {
        CImage* image_item = item_container->GetItemStack().GetItem()->GetItemElement();
        Vec2Int image_size = Vec2Int(image_item->GetWidth(), image_item->GetHeight());
        Mgr(CResMgr)->renderImg(_dc, image_item, m_vPos - image_size, image_size * 2, Vec2Int::zero, image_size);
    }
}

void CItemContainerVisualizer::SetSelected(bool value)
{
    container_selected = value;
}

CItemContainerVisualizer* CItemContainerVisualizer::Clone() const
{
    auto p = new CItemContainerVisualizer{ *this };
    return p;
}