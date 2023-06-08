#include "pch.h"
#include "TRItemStack.h"
#include "TRItemContainer.h"
#include "CItemContainerVisualizer.h"
#include "CInventoryVisualizer.h"
#include "Vec2Int.hpp"
#include "CScene.h"
#include "CResMgr.h"
#include "CKeyMgr.h"

CInventoryVisualizer::CInventoryVisualizer(TRItemContainer* containers[NUMOF_ITEMS])
{
	for (int index = 0; index < NUMOF_ITEMS; ++index)
	{
		container_visualizers[index] = new CItemContainerVisualizer(containers[index]);
		container_visualizers[index]->SetPos(m_vPos + Vec2(36.0f + 56.0f * index, 26.0f));
		container_visualizers[index]->SetScale(Vec2::one * 52.0f);
	}

	cursor_container = new TRItemContainer();
	cursor_visualizer = new CItemContainerVisualizer(cursor_container);

	container_background = Mgr(CResMgr)->GetImg(L"Inventory_Back.png");
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
	scene->AddObject(cursor_visualizer, GROUP_TYPE::UI);
}

void CInventoryVisualizer::SetVisible(bool value)
{
	visible = value;
	for (int i = 0; i < NUMOF_ITEMS; ++i)
		container_visualizers[i]->SetVisible(value);
	cursor_visualizer->SetVisible(value);
}

bool CInventoryVisualizer::HandleMouseInput()
{
	if (!visible)
		return false;

	Vec2 mouse_pos = Mgr(CKeyMgr)->GetMousePos();

	for (int i = 0; i < NUMOF_ITEMS; ++i)
	{
		CItemContainerVisualizer* ptr = container_visualizers[i];

		Vec2 pos = ptr->GetPos();
		Vec2 size = ptr->GetScale();

		float left = pos.x - size.x * 0.5f;
		float top = pos.y - size.y * 0.5f;
		float right = pos.x + size.x * 0.5f;
		float bottom = pos.y + size.y * 0.5f;

		if (mouse_pos.x >= left && mouse_pos.x <= right && mouse_pos.y >= top && mouse_pos.y <= bottom)
		{
			TRItemStack return_stack = ptr->GetItemContainer()->Apply(cursor_container->GetItemStack());
			cursor_container->Apply(return_stack);
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

	Vec2 mouse_pos = Mgr(CKeyMgr)->GetMousePos();
	cursor_visualizer->SetPos(mouse_pos);
}

void CInventoryVisualizer::render(HDC _dc) const
{
	if (!visible)
		return;

	for (int i = 0; i < NUMOF_ITEMS; ++i)
	{
		CImage* image = container_background;
		Vec2 pos = container_visualizers[i]->GetPos();
		Vec2 scale = container_visualizers[i]->GetScale();
		Mgr(CResMgr)->renderImg(_dc, image, pos - scale * 0.5f, scale, Vec2Int::zero, Vec2Int::one * 26);
	}
}
