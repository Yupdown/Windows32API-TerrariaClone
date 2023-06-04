#include "pch.h"
#include "TRWorld.h"
#include "TRTileManager.h"
#include "TRTile.h"
#include "Vec2Int.hpp"
#include "CKeyMgr.h"
#include "CCamera.h"
#include "CScene.h"

#include "TRItemManager.h"
#include "TRItemStack.h"

TRWorld::TRWorld()
{
	tile_map = new TRTileMap(TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT);
	player = new CPlayer(this);
	player->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2, TRWorld::WORLD_HEIGHT - 32)));
	player->SetScale(Vec2{ 40.f, 56.f });

	for (int i = 0; i < 10; ++i)
		quick_bar[i] = new TRItemContainer();
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
	if (!quick_bar[quick_bar_index]->Blank())
	{
		Vec2 mouse_world_pos = TRWorld::GlobalToWorld(Mgr(CCamera)->GetRealPos(Mgr(CKeyMgr)->GetMousePos()));

		if (Mgr(CKeyMgr)->GetKeyState(KEY::LBTN) == KEY_STATE::TAP)
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
