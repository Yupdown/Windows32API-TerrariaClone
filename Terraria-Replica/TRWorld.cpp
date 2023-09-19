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
#include "CAcquireItemText.h"

#include "CMonster.h"
#include "TRItemManager.h"
#include "TRItemStack.h"
#include "CCollisionMgr.h"
#include "CZombie.h"
#include "CSlime.h"
#include "CEyeMonster.h"
#include "CCthulhuEye.h"

#include "Vec2Int.hpp"
#include "CustomMath.hpp"
#include "CWeapon.h"
#include "TRMonGenerator.h"
#include "CMiniMap.h"
#include "CDropItem.h"
#include "CSoundMgr.h"

#include "CParticleMgr.h"

TRWorld* g_TRWorld = nullptr;
extern bool g_bStopToken;

static std::mt19937 randDigSound{std::random_device{}()};
static std::uniform_int_distribution<> uidDig{0, 2};

TRWorld::TRWorld()
{
	g_TRWorld = this;
	tile_map = new TRTileMap(TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT);
	
	for (int i = 0; i < 50; ++i)
		player_inventory[i] = new TRItemContainer();
	for (int i = 0; i < 3; ++i)
		player_armor[i] = new TRItemContainer();
	for (int i = 0; i < 10; ++i)
		quick_bar[i] = player_inventory[i];

	quick_bar_visualizer = new CQuickBarVisualizer(quick_bar);
	quick_bar_visualizer->SetPos(Vec2Int(10, 24));

	inventory_visualizer = new CInventoryVisualizer(player_inventory, player_armor);
	inventory_visualizer->SetPos(Vec2Int(10, 24));

	health_indicator = new CHealthIndicator();
	health_indicator->SetPos(Vec2Int(1120, 10));

	//for (int id = 0; id < 17; ++id)
	//	player_inventory[id]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByID(id), 10));

	quick_bar_index = 0;
	SetToggleInventory(false);

	player_inventory[0]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"pickaxe_iron"), 1));
	player_inventory[1]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"hammer_iron"), 1));
	player_inventory[2]->Apply(TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"longsword_iron"), 1));


	Mgr(CParticleMgr)->Init();
}

TRWorld::~TRWorld()
{
	delete tile_map;

	for (int i = 0; i < 50; ++i)
		delete player_inventory[i];
	for (int i = 0; i < 3; ++i)
		delete player_armor[i];
	
	g_TRWorld = nullptr;
}

void TRWorld::Update()
{
	if (KEY_TAP(KEY::ONE))
		SwitchQuickBarIndex(0);
	else if (KEY_TAP(KEY::TWO))
		SwitchQuickBarIndex(1);
	else if (KEY_TAP(KEY::THREE))
		SwitchQuickBarIndex(2);
	else if (KEY_TAP(KEY::FOUR))
		SwitchQuickBarIndex(3);
	else if (KEY_TAP(KEY::FIVE))
		SwitchQuickBarIndex(4);
	else if (KEY_TAP(KEY::SIX))
		SwitchQuickBarIndex(5);
	else if (KEY_TAP(KEY::SEVEN))
		SwitchQuickBarIndex(6);
	else if (KEY_TAP(KEY::EIGHT))
		SwitchQuickBarIndex(7);
	else if (KEY_TAP(KEY::NINE))
		SwitchQuickBarIndex(8);
	else if (KEY_TAP(KEY::ZERO))
		SwitchQuickBarIndex(9);
	else if (KEY_TAP(KEY::ESC))
		SetToggleInventory(!toggle_inventory);
	
	
	if (Mgr(CKeyMgr)->GetMouseWheelUp())
	{
		SwitchQuickBarIndex(wrapAround(quick_bar_index + 1, 0, 10));
	}

	if (Mgr(CKeyMgr)->GetMouseWheelDown())
	{
		SwitchQuickBarIndex(wrapAround(quick_bar_index - 1, 0, 10));
	}

	health_indicator->SetHealthValue(player->GetHP());

	if (KEY_TAP(KEY::LBTN))
	{
		bool result = inventory_visualizer->HandleMouseInput();
		if (result)
		{
			Mgr(CSoundMgr)->PlayEffect("Grab.wav", 0.5f);
		}
		else if (!quick_bar[quick_bar_index]->Blank())
		{
			Vec2 mouse_world_pos = TRWorld::GlobalToWorld(Mgr(CCamera)->GetRealPos(Mgr(CKeyMgr)->GetMousePos()));
			player->UseItem();
			bool use_item = quick_bar[quick_bar_index]->GetItemStack().GetItem()->OnUseItem(player, this, mouse_world_pos);
			if (use_item)
				quick_bar[quick_bar_index]->Apply(-1);
		}
	}

	Mgr(CParticleMgr)->Update();

	TRMonGenerator::GenerateMonster();
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
	if (g_bStopToken)
	{
		return;
	}

	m_pScene = scene;
	Mgr(CCollisionMgr)->Reset();
	player = new CPlayer(this);
	int x = TRWorld::WORLD_WIDTH / 2;
	player->SetPos(TRWorld::WorldToGlobal(Vec2Int(x, tile_map->GetTopYpos(x))) - Vec2(20.0f, 28.0f));
	player->SetScale(Vec2{ 40.f, 56.f });
	scene->AddObject(player, GROUP_TYPE::PLAYER);
	Mgr(CCamera)->SetTarget(player);
	scene->RegisterPlayer(player);

	TRItemStack dropitem_list[] = {
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"tile_planks_wood"), 100),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"tile_bricks_stone"), 100),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"tile_bricks_clay"), 100),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"tile_torch"), 100),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"wall_planks_wood"), 100),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"wall_bricks_stone"), 100),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"wall_bricks_clay"), 100),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"armor_iron_head"), 1),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"armor_iron_chestplate"), 1),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"armor_iron_leggings"), 1),
		TRItemStack(Mgr(TRItemManager)->GetItemByKey(L"summon_cthulhueye"), 1)
	};

	for (int i = 0; i < sizeof(dropitem_list) / sizeof(*dropitem_list); ++i)
		DropItem(Vec2Int(x - 10 + i * -4, 255), dropitem_list[i]);
	
	tile_map->OnSceneCreate(scene);

	Mgr(CCollisionMgr)->RegisterGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER); 
	Mgr(CCollisionMgr)->RegisterGroup( GROUP_TYPE::MONSTER,GROUP_TYPE::PLAYER_WEAPON);
	Mgr(CCollisionMgr)->RegisterGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DROP_ITEM);
	scene->AddObject(quick_bar_visualizer, GROUP_TYPE::UI);
	quick_bar_visualizer->AddContainerVisualizers(scene);
	scene->AddObject(inventory_visualizer, GROUP_TYPE::UI);
	inventory_visualizer->AddContainerVisualizers(scene);
	scene->AddObject(health_indicator, GROUP_TYPE::UI);

	{
		auto pMap = new CMiniMap;
		pMap->CreateMiniMap();
		scene->AddObject(pMap, GROUP_TYPE::UI);
	}

	Mgr(CCamera)->update();
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

	const int dir[][2] = { 0, 1, 0, -1, -1, 0, 1, 0 };
	int bitmask = 0;

	for (int k = 0; k < 4; ++k)
	{
		int xp = x + dir[k][0];
		int yp = y + dir[k][1];

		TRTile* tile_p = tile_map->GetTile(xp, yp);
		if (tile_p == nullptr)
			continue;

		if (tile_p->Solid())
			bitmask |= 1 << k;
	}

	TRTileWall* tile_wall_p = tile_map->GetTileWall(x, y);

	if (bitmask == 0 && tile_wall_p == Mgr(TRTileManager)->TileWallAir())
		return false;

	switch (uidDig(randDigSound))
	{
	case 0:Mgr(CSoundMgr)->PlayEffect("Dig_0.wav", 0.5f); break;
	case 1:Mgr(CSoundMgr)->PlayEffect("Dig_1.wav", 0.5f); break;
	case 2:Mgr(CSoundMgr)->PlayEffect("Dig_2.wav", 0.5f); break;
	default:
		break;
	}

	tile_map->SetTile(x, y, new_tile, true);
	return true;
}

void TRWorld::BreakTile(int x, int y)
{
	TRTile* tile = tile_map->GetTile(x, y);

	if (tile == nullptr)
		return;

	TRTile* air_tile = Mgr(TRTileManager)->TileAir();

	if (tile == air_tile)
		return;

	char buffer[16];
	sprintf_s(buffer, "%s_%d.wav", tile->Rocky() ? "Tink" : "Dig", uidDig(randDigSound));
	Mgr(CSoundMgr)->PlayEffect(buffer, 0.5f);

	tile_map->SetTile(x, y, air_tile, true);

	const Vec2 vParticlePos = TRWorld::WorldToGlobal(Vec2{ (float)x,(float)y });
	CAtlasElement* pImg = tile->GetTileImg();
	Mgr(CParticleMgr)->SetParticles(vParticlePos, pImg);

	std::wstring k_dropitem = tile->DropItem();
	if (k_dropitem == L"")
		return;

	TRItem* p_dropitem = Mgr(TRItemManager)->GetItemByKey(k_dropitem);
	DropItem(Vec2(x + 0.5f, y + 0.5f), TRItemStack(p_dropitem, 1));
}

bool TRWorld::PlaceTileWall(int x, int y, TRTileWall* new_tile)
{
	TRTileWall* tile = tile_map->GetTileWall(x, y);
	TRTileWall* air_tile = Mgr(TRTileManager)->TileWallAir();

	if (tile == nullptr)
		return false;
	if (tile != air_tile)
		return false;

	const int dir[][2] = { 0, 1, 0, -1, -1, 0, 1, 0 };
	int bitmask = 0;

	for (int k = 0; k < 4; ++k)
	{
		int xp = x + dir[k][0];
		int yp = y + dir[k][1];

		TRTile* tile_p = tile_map->GetTile(xp, yp);
		TRTileWall* tile_wall_p = tile_map->GetTileWall(xp, yp);

		if (tile_p != nullptr)
		{
			if (tile_p->Solid())
				bitmask |= 1 << k;
		}
		if (tile_wall_p != nullptr)
		{
			if (tile_wall_p != air_tile)
				bitmask |= 1 << k;
		}
	}

	TRTile* tile_p = tile_map->GetTile(x, y);

	if (bitmask == 0 && !tile_p->Solid())
		return false;
	
	char buffer[16];
	sprintf_s(buffer, "%s_%d.wav", "Dig", uidDig(randDigSound));
	Mgr(CSoundMgr)->PlayEffect(buffer, 0.5f);

	tile_map->SetTileWall(x, y, new_tile, true);
	return true;
}

void TRWorld::BreakTileWall(int x, int y)
{
	TRTileWall* tile = tile_map->GetTileWall(x, y);

	if (tile == nullptr)
		return;

	TRTileWall* air_tile = Mgr(TRTileManager)->TileWallAir();

	if (tile == air_tile)
		return;

	char buffer[16];
	sprintf_s(buffer, "%s_%d.wav", "Dig", uidDig(randDigSound));
	Mgr(CSoundMgr)->PlayEffect(buffer, 0.5f);

	tile_map->SetTileWall(x, y, air_tile, true);

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
	if (toggle_inventory != value)
	{
		if (value)
		{
			Mgr(CSoundMgr)->PlayEffect("Menu_Open.wav", 0.5f);
		}
		else
		{
			Mgr(CSoundMgr)->PlayEffect("Menu_Close.wav", 0.5f);
		}
	}
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

	static wchar_t buffer[64];
	if (item.GetStackSize() > 1)
		wsprintf(buffer, L"%s (%d)", item.GetItem()->GetName().c_str(), item.GetStackSize());
	else
		wsprintf(buffer, L"%s", item.GetItem()->GetName().c_str());

	CAcquireItemText* text = new CAcquireItemText(buffer);
	text->SetPos(player->GetPos() - Vec2::up * 24.0f);
	m_pScene->AddObject(text, GROUP_TYPE::DROP_ITEM);
}

void TRWorld::SwitchQuickBarIndex(int value)
{
	if (quick_bar_index == value)
		return;
	quick_bar_index = value;

	quick_bar_visualizer->SetSelectIndex(quick_bar_index);
	Mgr(CSoundMgr)->PlayEffect("Menu_Tick.wav", 0.5f);
}

int TRWorld::GetArmorPoint() const
{
	int armor_point = 0;

	for (TRItemContainer* container : player_armor)
	{
		if (container->Blank())
			continue;

		TRItemArmor* armor = dynamic_cast<TRItemArmor*>(container->GetItemStack().GetItem());
		if (armor != nullptr)
			armor_point += armor->GetArmorPoint();
	}

	return armor_point;
}

void TRWorld::FloatDamageText(int value, Vec2 vPos, COLORREF color)
{
	static wchar_t buffer[64];
	wsprintf(buffer, L"%d", value);

	CAcquireItemText* text = new CAcquireItemText(buffer, color);
	text->SetPos(vPos);
	m_pScene->AddObject(text, GROUP_TYPE::DROP_ITEM);
}

void TRWorld::SpawnBoss()
{
	auto pMon = new CCthulhuEye{ this,L"Monster_CthulhuEye", L"NPC_4.png" };
	pMon->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2, TRWorld::WORLD_HEIGHT)));
	pMon->SetScale(Vec2{ 110.0f, 166.0f });
	pMon->SetColliderScale(Vec2{ 110.0f, 110.0f });

	m_pScene->AddObject(pMon, GROUP_TYPE::MONSTER);

	Mgr(CSoundMgr)->PlayBGM("04. Boss 1.mp3", 0.5f);

	Mgr(CCamera)->FadeOut(1.f);
	Mgr(CCamera)->FadeIn(1.f);

	StartCoEvent(Mgr(CCamera)->ZoomInBoss(pMon->GetPos()));
}