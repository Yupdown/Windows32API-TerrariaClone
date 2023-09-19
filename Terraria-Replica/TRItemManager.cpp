#include "pch.h"
#include "TRItemManager.h"

TRItemManager::TRItemManager()
{
	registry = new Registry<TRItem>();
}

TRItemManager::~TRItemManager()
{
	delete registry;
}

void TRItemManager::LoadItems()
{
	registry->Insert(L"tile_dirt", new TRItemTile(L"Dirt", L"Item_2.png", L"dirt"));
	registry->Insert(L"tile_cobblestone", new TRItemTile(L"Cobblestone", L"Item_3.png", L"cobblestone"));
	registry->Insert(L"tile_planks_wood", new TRItemTile(L"Wooden Planks", L"Item_9.png", L"planks_wood"));
	registry->Insert(L"tile_iron_ore", new TRItemTile(L"Iron Ore", L"Item_11.png", L"iron_ore"));
	registry->Insert(L"tile_copper_ore", new TRItemTile(L"Copper Ore", L"Item_12.png", L"copper_ore"));
	registry->Insert(L"tile_gold_ore", new TRItemTile(L"Gold Ore", L"Item_13.png", L"gold_ore"));
	registry->Insert(L"tile_silver_ore", new TRItemTile(L"Silver Ore", L"Item_14.png", L"silver_ore"));
	registry->Insert(L"tile_bricks_stone", new TRItemTile(L"Stone Bricks", L"Item_129.png", L"bricks_stone"));
	registry->Insert(L"tile_bricks_clay", new TRItemTile(L"Bricks", L"Item_131.png", L"bricks_clay"));
	registry->Insert(L"tile_torch", new TRItemTile(L"Torch", L"Item_Torch.png", L"torch"));
	registry->Insert(L"wall_cobblestone", new TRItemTileWall(L"Cobblestone Wall", L"Item_26.png", L"cobblestone"));
	registry->Insert(L"wall_dirt", new TRItemTileWall(L"Dirt Wall", L"Item_30.png", L"dirt"));
	registry->Insert(L"wall_planks_wood", new TRItemTileWall(L"Wooden Planks Wall", L"Item_93.png", L"planks_wood"));
	registry->Insert(L"wall_bricks_stone", new TRItemTileWall(L"Stone Bricks Wall", L"Item_130.png", L"bricks_stone"));
	registry->Insert(L"wall_bricks_clay", new TRItemTileWall(L"Bricks Wall", L"Item_132.png", L"bricks_clay"));
	registry->Insert(L"pickaxe_iron", new TRItemPickaxe(L"Iron Pickaxe", L"Item_Pickaxe.png"));
	registry->Insert(L"hammer_iron", new TRItemHammer(L"Iron Hammer", L"Item_Hammer.png"));
	registry->Insert(L"longsword_iron", new TRItemSword(L"Iron Longsword", L"Item_Sword.png"));
	registry->Insert(L"summon_cthulhueye", new TRItemSummonBoss(L"Suspicious Eye", L"Item_43.png"));
	registry->Insert(L"armor_iron_head", new TRItemArmor(L"Iron Helmet", L"Item_90.png", 0, 10));
	registry->Insert(L"armor_iron_chestplate", new TRItemArmor(L"Iron Chestplate", L"Item_81.png", 1, 15));
	registry->Insert(L"armor_iron_leggings", new TRItemArmor(L"Iron Leggings", L"Item_77.png", 2, 10));
}

void TRItemManager::Initialize()
{
	
}

TRItem* TRItemManager::GetItemByID(int id) const
{
	return (*registry)[id];
}

TRItem* TRItemManager::GetItemByKey(const std::wstring& key) const
{
	return (*registry)[key];
}
