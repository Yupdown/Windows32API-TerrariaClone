#include "pch.h"

#include "TRWorld.h"
#include "TRTileManager.h"
#include "TRTile.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CCamera.h"
#include "CScene.h"
#include "CItemContainerVisualizer.h"

#include "TRItemManager.h"
#include "TRItemStack.h"

#include "Vec2Int.hpp"
#include "CustomMath.hpp"

TRWorld::TRWorld()
{
	tile_map = new TRTileMap(TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT);
	player = new CPlayer(this);
	player->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2, TRWorld::WORLD_HEIGHT - 32)));
	player->SetScale(Vec2{ 40.f, 56.f });

	for (int i = 0; i < 10; ++i)
	{
		quick_bar[i] = new TRItemContainer();
		container_visualizers[i] = new CItemContainerVisualizer(quick_bar[i]);
		container_visualizers[i]->SetPos(Vec2Int(56 * i + 36, 42));
		container_visualizers[i]->SetScale(Vec2Int(52, 52));
	}
	quick_bar[0]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"pickaxe_iron"), 1));
	quick_bar[1]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"hammer_iron"), 1));
	quick_bar[2]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"longsword_iron"), 1));
	quick_bar[3]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"tile_dirt"), 1));
	quick_bar[4]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"tile_bricks_stone"), 1));
	quick_bar[5]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"wall_bricks_stone"), 1));
	quick_bar_index = 5;
}

TRWorld::~TRWorld()
{
	delete tile_map;

	for (int i = 0; i < 10; ++i)
		delete quick_bar[i];
}

void TRWorld::Update()
{
	if (KEY_TAP(KEY::ONE))
		quick_bar_index = 0;
	else if (KEY_TAP(KEY::TWO))
		quick_bar_index = 1;
	else if (KEY_TAP(KEY::THREE))
		quick_bar_index = 2;
	else if (KEY_TAP(KEY::FOUR))
		quick_bar_index = 3;
	else if (KEY_TAP(KEY::FIVE))
		quick_bar_index = 4;
	else if (KEY_TAP(KEY::SIX))
		quick_bar_index = 5;
	else if (KEY_TAP(KEY::SEVEN))
		quick_bar_index = 6;
	else if (KEY_TAP(KEY::EIGHT))
		quick_bar_index = 7;
	else if (KEY_TAP(KEY::NINE))
		quick_bar_index = 8;
	else if (KEY_TAP(KEY::ZERO))
		quick_bar_index = 9;

	float vSize_normal = 52.0f;
	float vSize_select = 64.0f;

	for (int i = 0; i < 10; ++i)
	{
		bool selected = i == quick_bar_index;
		Vec2 position = container_visualizers[i]->GetPos();
		Vec2 scale = container_visualizers[i]->GetScale();

		float x_delta = 36.0f + 56.0f * i;
		if (i > quick_bar_index)
			x_delta += vSize_select - vSize_normal;
		else if (i == quick_bar_index)
			x_delta += (vSize_select - vSize_normal) * 0.5f;

		container_visualizers[i]->SetSelected(selected);
		container_visualizers[i]->SetPos(Vec2(Lerp(position.x, x_delta, DT * 16.0f), position.y));
		container_visualizers[i]->SetScale(Vec2(
			Lerp(scale.x, selected ? vSize_select : vSize_normal, DT * 16.0f),
			Lerp(scale.y, selected ? vSize_select : vSize_normal, DT * 16.0f)
		));
	}

	if (!quick_bar[quick_bar_index]->Blank())
	{
		Vec2 mouse_world_pos = TRWorld::GlobalToWorld(Mgr(CCamera)->GetRealPos(Mgr(CKeyMgr)->GetMousePos()));

		if (KEY_TAP(KEY::LBTN))
			quick_bar[quick_bar_index]->GetItemStack().GetItem()->OnUseItem(player, this, mouse_world_pos);
	}
}

void TRWorld::CreateWorld(int seed)
{
	TRWorldGeneration* generator = new TRWorldGeneration();

	TRWorldGenerationProcess* processes[] =
	{
		new TRWorldGenerationTerrainHeight(),
		new TRWorldGenerationAttachWall(),
		new TRWorldGenerationPinchCaves(),
		new TRWorldGenerationGrowOres(Mgr(TRTileManager)->GetTileByKey(L"copper_ore"), -20),
		new TRWorldGenerationGrowOres(Mgr(TRTileManager)->GetTileByKey(L"iron_ore"), 0),
		new TRWorldGenerationGrowOres(Mgr(TRTileManager)->GetTileByKey(L"silver_ore"), 10),
		new TRWorldGenerationGrowOres(Mgr(TRTileManager)->GetTileByKey(L"gold_ore"), 30),
		new TRWorldGenerationGrowGrass()
	};

	for (int i = 0; i < sizeof(processes) / sizeof(*processes); ++i)
		generator->AddProcess(processes[i]);
	generator->GenerateWorld(tile_map, TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT, seed);
	delete generator;
	for (int i = 0; i < sizeof(processes) / sizeof(*processes); ++i)
		delete processes[i];
}

void TRWorld::OnSceneCreate(CScene* scene)
{
	tile_map->OnSceneCreate(scene);
	scene->AddObject(player, GROUP_TYPE::PLAYER);
	Mgr(CCamera)->SetTarget(player);
	scene->RegisterPlayer(player);

	for (int i = 0; i < 10; ++i)
		scene->AddObject(container_visualizers[i], GROUP_TYPE::UI);
}

Vec2 TRWorld::WorldToGlobal(const Vec2& v)
{
	return Vec2(v.x * PIXELS_PER_TILE, (static_cast<float>(TRWorld::WORLD_HEIGHT) - v.y) * PIXELS_PER_TILE);
}

Vec2 TRWorld::GlobalToWorld(const Vec2& v)
{
	return Vec2(v.x / PIXELS_PER_TILE, static_cast<float>(TRWorld::WORLD_HEIGHT) - v.y / PIXELS_PER_TILE);
}

TRTileMap* TRWorld::GetTileMap() const
{
	return tile_map;
}
