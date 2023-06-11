#include "pch.h"
#include "TRItem.h"
#include "TRItemStack.h"
#include "TRItemContainer.h"
#include "CItemContainerVisualizer.h"
#include "CInventoryVisualizer.h"
#include "Vec2Int.hpp"
#include "CScene.h"
#include "CResMgr.h"
#include "CKeyMgr.h"

CInventoryVisualizer::CInventoryVisualizer(TRItemContainer* containers[NUMOF_ITEMS], TRItemContainer* armor_containers[3])
{
	for (int index = 0; index < NUMOF_ITEMS; ++index)
	{
		container_visualizers[index] = new CItemContainerVisualizer(containers[index]);
		container_visualizers[index]->SetPos(m_vPos + Vec2(36.0f + 56.0f * index, 26.0f));
		container_visualizers[index]->SetScale(Vec2::one * 52.0f);
	}

	for (int index = 0; index < 3; ++index)
	{
		container_armor_visualizers[index] = new CItemContainerVisualizer(armor_containers[index]);
		container_armor_visualizers[index]->SetPos(m_vPos + Vec2(640.0f, 26.0f + 56.0f * index));
		container_armor_visualizers[index]->SetScale(Vec2::one * 52.0f);
	}

	cursor_container = new TRItemContainer();
	cursor_visualizer = new CItemContainerVisualizer(cursor_container);

	container_background[0] = Mgr(CResMgr)->GetImg(L"Inventory_Back.png");
	container_background[1] = Mgr(CResMgr)->GetImg(L"Inventory_Back_Armor.png");
	m_bIsCamAffected = false;
	visible = true;
}

CInventoryVisualizer::CInventoryVisualizer(const CInventoryVisualizer& other) : CObject(other)
{
}

CInventoryVisualizer::~CInventoryVisualizer()
{
	delete cursor_container;
}

void CInventoryVisualizer::AddContainerVisualizers(CScene* scene)
{
	for (int i = 0; i < NUMOF_ITEMS; ++i)
		scene->AddObject(container_visualizers[i], GROUP_TYPE::UI);
	for (int i = 0; i < 3; ++i)
		scene->AddObject(container_armor_visualizers[i], GROUP_TYPE::UI);
	scene->AddObject(cursor_visualizer, GROUP_TYPE::UI);
}

void CInventoryVisualizer::SetVisible(bool value)
{
	visible = value;
	for (int i = 0; i < NUMOF_ITEMS; ++i)
		container_visualizers[i]->SetVisible(value);
	for (int i = 0; i < 3; ++i)
		container_armor_visualizers[i]->SetVisible(value);
	cursor_visualizer->SetVisible(value);
}

bool CInventoryVisualizer::HandleMouseInput()
{
	if (!visible)
		return false;

	Vec2 mouse_pos = Mgr(CKeyMgr)->GetMousePos();

	auto callback = [](CItemContainerVisualizer* obj, const Vec2& mouse_pos) -> bool
	{
		Vec2 pos = obj->GetPos();
		Vec2 size = obj->GetScale();

		float left = pos.x - size.x * 0.5f;
		float top = pos.y - size.y * 0.5f;
		float right = pos.x + size.x * 0.5f;
		float bottom = pos.y + size.y * 0.5f;

		return mouse_pos.x >= left && mouse_pos.x <= right && mouse_pos.y >= top && mouse_pos.y <= bottom;
	};

	for (int i = 0; i < NUMOF_ITEMS; ++i)
	{
		CItemContainerVisualizer* ptr = container_visualizers[i];

		if (callback(ptr, mouse_pos))
		{
			TRItemStack return_stack = ptr->GetItemContainer()->Apply(cursor_container->GetItemStack());
			cursor_container->Apply(return_stack);
			return true;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		CItemContainerVisualizer* ptr = container_armor_visualizers[i];

		if (callback(ptr, mouse_pos))
		{
			TRItemStack item = cursor_container->GetItemStack();
			TRItemArmor* item_armor = dynamic_cast<TRItemArmor*>(item.GetItem());

			bool condition = false;
			if (item_armor != nullptr)
				condition = item_armor->GetArmorPart() == i;

			if (condition || item.Null())
			{
				TRItemStack return_stack = ptr->GetItemContainer()->Apply(item);
				cursor_container->Apply(return_stack);
			}

			return true;
		}
	}

	return false;
}

CInventoryVisualizer* CInventoryVisualizer::Clone() const
{
	return new CInventoryVisualizer(*this);
}

void CInventoryVisualizer::update()
{
	if (!visible)
		return;

	for (int index = 0; index < NUMOF_ITEMS; ++index)
	{
		int r = index / NUMOF_COLUMNS;
		int c = index % NUMOF_COLUMNS;
		container_visualizers[index]->SetPos(m_vPos + Vec2(36.0f + 56.0f * c, 26.0f + 56.0f * r));
	}

	for (int index = 0; index < 3; ++index)
		container_armor_visualizers[index]->SetPos(m_vPos + Vec2(640.0f, 26.0f + 56.0f * index));

	Vec2 mouse_pos = Mgr(CKeyMgr)->GetMousePos();
	cursor_visualizer->SetPos(mouse_pos);
}

void CInventoryVisualizer::render(HDC _dc) const
{
	if (!visible)
		return;

	for (int i = 0; i < NUMOF_ITEMS; ++i)
	{
		CImage* image = container_background[0];
		Vec2 pos = container_visualizers[i]->GetPos();
		Vec2 scale = container_visualizers[i]->GetScale();
		Mgr(CResMgr)->renderImg(_dc, image, pos - scale * 0.5f, scale, Vec2Int::zero, Vec2Int::one * 26);
	}

	for (int i = 0; i < 3; ++i)
	{
		CImage* image = container_background[1];
		Vec2 pos = container_armor_visualizers[i]->GetPos();
		Vec2 scale = container_armor_visualizers[i]->GetScale();
		Mgr(CResMgr)->renderImg(_dc, image, pos - scale * 0.5f, scale, Vec2Int::zero, Vec2Int::one * 26);
	}
}
