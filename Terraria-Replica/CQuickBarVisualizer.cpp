#include "pch.h"
#include "TRItem.h"
#include "TRItemContainer.h"
#include "CQuickBarVisualizer.h"
#include "CItemContainerVisualizer.h"
#include "CScene.h"
#include "CResMgr.h"
#include "Vec2Int.hpp"
#include "CustomMath.hpp"

CQuickBarVisualizer::CQuickBarVisualizer(TRItemContainer* containers[10])
{
	for (int index = 0; index < 10; ++index)
	{
		container_visualizers[index] = new CItemContainerVisualizer(containers[index]);
		container_visualizers[index]->SetPos(m_vPos + Vec2(36.0f + 56.0f * index, 26.0f));
		container_visualizers[index]->SetScale(Vec2::one * 52.0f);
	}

    container_background[0] = Mgr(CResMgr)->GetImg(L"Inventory_Back.png");
    container_background[1] = Mgr(CResMgr)->GetImg(L"Inventory_Back_Select.png");

    select_index = 0;
    m_bIsCamAffected = false;
	visible = true;
}

CQuickBarVisualizer::CQuickBarVisualizer(const CQuickBarVisualizer& other) : CObject(other)
{
}

CQuickBarVisualizer::~CQuickBarVisualizer()
{
}

void CQuickBarVisualizer::SetSelectIndex(int value)
{
    select_index = value;
}

void CQuickBarVisualizer::AddContainerVisualizers(CScene* scene)
{
	for (int i = 0; i < 10; ++i)
		scene->AddObject(container_visualizers[i], GROUP_TYPE::UI);
}

void CQuickBarVisualizer::SetVisible(bool value)
{
	visible = value;
	for (int i = 0; i < 10; ++i)
		container_visualizers[i]->SetVisible(value);
}

CQuickBarVisualizer* CQuickBarVisualizer::Clone() const
{
	return new CQuickBarVisualizer(*this);
}

void CQuickBarVisualizer::update()
{
	float vSize_normal = 52.0f;
	float vSize_select = 64.0f;

	for (int i = 0; i < 10; ++i)
	{
		bool selected = i == select_index;
		Vec2 position = container_visualizers[i]->GetPos();
		Vec2 scale = container_visualizers[i]->GetScale();

		float x_delta = m_vPos.x + 36.0f + 56.0f * i;
		if (i > select_index)
			x_delta += vSize_select - vSize_normal;
		else if (i == select_index)
			x_delta += (vSize_select - vSize_normal) * 0.5f;

		container_visualizers[i]->SetPos(Vec2(Lerp(position.x, x_delta, DT * 16.0f), m_vPos.y + 26.0f));
		container_visualizers[i]->SetScale(Vec2(
			Lerp(scale.x, selected ? vSize_select : vSize_normal, DT * 16.0f),
			Lerp(scale.y, selected ? vSize_select : vSize_normal, DT * 16.0f)
		));
	}
}

void CQuickBarVisualizer::render(HDC _dc) const
{
	if (!visible)
		return;

	TRItemContainer* item_container = container_visualizers[select_index]->GetItemContainer();

	if (!item_container->Blank())
	{
		static wchar_t buffer[64];
		wsprintf(buffer, L"%s", item_container->GetItemStack().GetItem()->GetName().c_str());

		SIZE s_out;
		GetTextExtentPoint32(_dc, buffer, lstrlen(buffer), &s_out);

		renderText(_dc, 0x00000000, m_vPos + Vec2(278.0f - s_out.cx / 2, -18.0f), buffer);
		renderText(_dc, 0x00FFFFFF, m_vPos + Vec2(278.0f - s_out.cx / 2, -20.0f), buffer);
	}

	for (int i = 0; i < 10; ++i)
	{
		CImage* image = container_background[i == select_index ? 1 : 0];
		Vec2 pos = container_visualizers[i]->GetPos();
		Vec2 scale = container_visualizers[i]->GetScale();
		Mgr(CResMgr)->renderImg(_dc, image, pos - scale * 0.5f, scale, Vec2Int::zero, Vec2Int::one * 26);
	}
}
