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
#include "CInventoryVisualizer.h"

#include "CMonster.h"
#include "TRItemManager.h"
#include "TRItemStack.h"
#include "CCollisionMgr.h"
#include "CZombie.h"
#include "CSlime.h"
#include "CEyeMonster.h"

#include "Vec2Int.hpp"
#include "CustomMath.hpp"
#include "CWeapon.h"
#include "TRMonGenerator.h"
#include "CMiniMap.h"
#include "CDropItem.h"

TRWorld* g_TRWorld = nullptr;

TRWorld::TRWorld()
{
	g_TRWorld = this;
	tile_map = new TRTileMap(TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT);
	
	for (int i = 0; i < 50; ++i)
		player_inventory[i] = new TRItemContainer();
	for (int i = 0; i < 10; ++i)
		quick_bar[i] = player_inventory[i];

	quick_bar_visualizer = new CQuickBarVisualizer(quick_bar);
	quick_bar_visualizer->SetPos(Vec2Int(10, 24));

	inventory_visualizer = new CInventoryVisualizer(player_inventory);
	inventory_visualizer->SetPos(Vec2Int(10, 24));

	health_indicator = new CHealthIndicator();
	health_indicator->SetPos(Vec2Int(1120, 10));

	for (int id = 0; id < 17; ++id)
		player_inventory[id]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByID(id), 10));

	quick_bar_index = 0;
	SetToggleInventory(false);
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
	else if (KEY_TAP(KEY::ENTER))
		SetToggleInventory(!toggle_inventory);
	quick_bar_visualizer->SetSelectIndex(quick_bar_index);

	health_indicator->SetHealthValue(player->GetHP());

	if (KEY_TAP(KEY::LBTN))
	{
		bool result = inventory_visualizer->HandleMouseInput();
		if (!result && !quick_bar[quick_bar_index]->Blank())
		{
			Vec2 mouse_world_pos = TRWorld::GlobalToWorld(Mgr(CCamera)->GetRealPos(Mgr(CKeyMgr)->GetMousePos()));
			quick_bar[quick_bar_index]->GetItemStack().GetItem()->OnUseItem(player, this, mouse_world_pos);
		}
	}

	//TRMonGenerator::GenerateMonster();
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
	m_pScene = scene;

	player = new CPlayer(this);
	int x = TRWorld::WORLD_WIDTH / 2;
	player->SetPos(TRWorld::WorldToGlobal(Vec2Int(x, tile_map->GetTopYpos(x))) - Vec2(20.0f, 28.0f));
	player->SetScale(Vec2{ 40.f, 56.f });
	scene->AddObject(player, GROUP_TYPE::PLAYER);
	Mgr(CCamera)->SetTarget(player);
	scene->RegisterPlayer(player);

	for (int i = 0; i < 17; ++i)
		DropItem(Vec2Int(x + i * 4, 254), TRItemStack(Mgr(TRItemManager)->GetItemByID(i), 100));

	/*{
		CWeapon* pWeapon;
		pWeapon = new CWeapon{ player };
		pWeapon->SetWeaponImg(L"Item_Pickaxe.png",L"Item_Pickaxe", Vec2{32,32});



		pWeapon = new CWeapon{ player };
		pWeapon->SetWeaponImg(L"Item_Hammer.png", L"Item_Hammer", Vec2{ 32,32 });


		pWeapon = new CWeapon{ player };
		pWeapon->SetWeaponImg(L"Item_Sword.png", L"Item_Sword", Vec2{ 32,32 });

	}*/
	
	tile_map->OnSceneCreate(scene);

	//{
	//	auto pMon = new CZombie{ this,L"Monster_Zombie",L"NPC_3.png" };
	//	pMon->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2, TRWorld::WORLD_HEIGHT)));
	//	pMon->SetScale(Vec2{ 38.0f, 46.0f });
	//	scene->AddObject(pMon, GROUP_TYPE::MONSTER);
	//	pMon->SetColliderScale(Vec2{ 38.0f, 46.0f });
	//}

	//{
	//	auto pMon = new CSlime{ this,L"Monster_Slime",L"NPC_1.png" };
	//	pMon->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2 - 100, TRWorld::WORLD_HEIGHT)));
	//	pMon->SetScale(Vec2{ 32.0f, 24.0f });
	//	scene->AddObject(pMon, GROUP_TYPE::MONSTER);
	//	pMon->SetColliderScale(Vec2{ 32.0f, 24.0f });
	//}

	//{
	//	auto pMon = new CEyeMonster{ this,L"Monster_EyeMonster",L"NPC_2.png" };
	//	pMon->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2 - 10, TRWorld::WORLD_HEIGHT)));
	//	pMon->SetScale(Vec2{ 38.0f, 22.0f });
	//	scene->AddObject(pMon, GROUP_TYPE::MONSTER);
	//	pMon->SetColliderScale(Vec2{ 38.0f, 22.0f });
	//}

	{
		scene->AddObject(new CMiniMap, GROUP_TYPE::UI);
	}

	Mgr(CCollisionMgr)->RegisterGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER); 
	Mgr(CCollisionMgr)->RegisterGroup( GROUP_TYPE::MONSTER,GROUP_TYPE::PLAYER_WEAPON);
	Mgr(CCollisionMgr)->RegisterGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DROP_ITEM);
	scene->AddObject(quick_bar_visualizer, GROUP_TYPE::UI);
	quick_bar_visualizer->AddContainerVisualizers(scene);
	scene->AddObject(inventory_visualizer, GROUP_TYPE::UI);
	inventory_visualizer->AddContainerVisualizers(scene);
	scene->AddObject(health_indicator, GROUP_TYPE::UI);

	scene->Enter();
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

CPlayer* TRWorld::GetPlayer() const
{
	return player;
}

bool TRWorld::PlaceTile(int x, int y, TRTile* new_tile)
{
	TRTile* tile = tile_map->GetTile(x, y);

	if (tile == nullptr)
		return false;
	if (tile->Solid())
		return false;

	tile_map->SetTile(x, y, new_tile, true);
	return true;
}

void TRWorld::BreakTile(int x, int y)
{
	TRTile* tile = tile_map->GetTile(x, y);

	if (tile == nullptr)
		return;

	tile_map->SetTile(x, y, Mgr(TRTileManager)->TileAir(), true);

	std::wstring k_dropitem = tile->DropItem();
	if (k_dropitem == L"")
		return;

	TRItem* p_dropitem = Mgr(TRItemManager)->GetItemByKey(k_dropitem);
	DropItem(Vec2(x + 0.5f, y + 0.5f), TRItemStack(p_dropitem, 1));
}

void TRWorld::DropItem(Vec2 world_pos, TRItemStack item)
{
	if (item.Null())
		return;

	CDropItem* drop_item = new CDropItem(this, item);
	drop_item->SetPos(TRWorld::WorldToGlobal(world_pos));

	m_pScene->AddObject(drop_item, GROUP_TYPE::DROP_ITEM);
}

void TRWorld::SetToggleInventory(bool value)
{
	toggle_inventory = value;
	quick_bar_visualizer->SetVisible(!value);
	inventory_visualizer->SetVisible(value);
}

void TRWorld::AddItemToInventory(TRItemStack item)
{
	TRItemStack return_item = item;

	for (auto& container : player_inventory)
	{
		if (container->Blank() || container->GetItemStack().GetItem() == item.GetItem())
			return_item = container->Apply(return_item);

		if (return_item.Null())
			break;
	}
}