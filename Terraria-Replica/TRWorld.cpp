#include "pch.h"

#include "TRWorld.h"
#include "TRTileManager.h"
#include "TRTile.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CCamera.h"
#include "CScene.h"
#include "CQuickBarVisualizer.h"
#include "CHealthIndicator.h"

#include "CMonster.h"
#include "TRItemManager.h"
#include "TRItemStack.h"
#include "CCollisionMgr.h"
#include "CZombie.h"
#include "CSlime.h"
#include "CEyeMonster.h"

#include "Vec2Int.hpp"
#include "CustomMath.hpp"

TRWorld::TRWorld()
{
	tile_map = new TRTileMap(TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT);
	player = new CPlayer(this);
	player->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2, TRWorld::WORLD_HEIGHT - 32)));
	player->SetScale(Vec2{ 40.f, 56.f });

	for (int i = 0; i < 10; ++i)
		quick_bar[i] = new TRItemContainer();
	quick_bar_visualizer = new CQuickBarVisualizer(quick_bar);
	quick_bar_visualizer->SetPos(Vec2Int(10, 24));
	health_indicator = new CHealthIndicator();
	health_indicator->SetPos(Vec2Int(1120, 10));

	quick_bar[0]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"pickaxe_iron"), 1));
	quick_bar[1]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"hammer_iron"), 1));
	quick_bar[2]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"longsword_iron"), 1));
	quick_bar[3]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"tile_dirt"), 1));
	quick_bar[4]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"tile_bricks_stone"), 1));
	quick_bar[5]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"wall_bricks_stone"), 1));
	quick_bar_index = 0;
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
	quick_bar_visualizer->SetSelectIndex(quick_bar_index);

	health_indicator->SetHealthValue(player->GetHP());

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

	{
		auto pMon = new CZombie{ this,L"Zombie",L"NPC_3.png" };
		pMon->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2, TRWorld::WORLD_HEIGHT)));
		pMon->SetScale(Vec2{ 38.0f, 46.0f });
		scene->AddObject(pMon, GROUP_TYPE::MONSTER);
	}

	{
		auto pMon = new CSlime{ this,L"Slime",L"NPC_1.png" };
		pMon->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2 - 100, TRWorld::WORLD_HEIGHT)));
		pMon->SetScale(Vec2{ 32.0f, 24.0f });
		scene->AddObject(pMon, GROUP_TYPE::MONSTER);
	}

	{
		auto pMon = new CEyeMonster{ this,L"EyeMonster",L"NPC_2.png" };
		pMon->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2 - 10, TRWorld::WORLD_HEIGHT)));
		pMon->SetScale(Vec2{ 38.0f, 22.0f });
		scene->AddObject(pMon, GROUP_TYPE::MONSTER);
	}

	Mgr(CCollisionMgr)->RegisterGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER); 

	scene->AddObject(quick_bar_visualizer, GROUP_TYPE::UI);
	quick_bar_visualizer->AddContainerVisualizers(scene);
	scene->AddObject(health_indicator, GROUP_TYPE::UI);
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
