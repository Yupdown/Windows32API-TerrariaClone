#include "pch.h"
#include "TRTileManager.h"

TRTileManager::TRTileManager()
{
	registry = new Registry<TRTile*>();
}

TRTileManager::~TRTileManager()
{
	delete registry;
}

void TRTileManager::LoadTiles()
{
	tile_air = new TRTileAir();
	registry->Insert("air", tile_air);
	registry->Insert("cobblestone", new TRTileSolid(L"Cobblestone", 1.0f, L"tile_cobblestone.png", L""));
	registry->Insert("dirt", new TRTileSolid(L"Dirt", 1.0f, L"tile_dirt.png", L""));
	registry->Insert("dirtgrass", new TRTileSolid(L"Grass Soil", 1.0f, L"tile_dirtgrass.png", L""));
	registry->Insert("clay", new TRTileSolid(L"Clay", 1.0f, L"tile_clay.png", L""));
	registry->Insert("copper_ore", new TRTileSolid(L"Copper Ore", 1.0f, L"tile_copperore.png", L""));
	registry->Insert("iron_ore", new TRTileSolid(L"Iron Ore", 1.0f, L"tile_ironore.png", L""));
	registry->Insert("silver_ore", new TRTileSolid(L"Silver Ore", 1.0f, L"tile_silverore.png", L""));
	registry->Insert("gold_ore", new TRTileSolid(L"Gold Ore", 1.0f, L"tile_goldore.png", L""));
	registry->Insert("planks_wood", new TRTileSolid(L"Wooden Planks", 1.0f, L"tile_planks.png", L""));
	registry->Insert("bricks_clay", new TRTileSolid(L"Bricks", 1.0f, L"tile_bricks.png", L""));
	registry->Insert("bricks_stone", new TRTileSolid(L"Stone Bricks", 1.0f, L"tile_stonebricks.png", L""));
}

void TRTileManager::Initialize()
{
	
}

TRTile* TRTileManager::GetTileByID(int id) const
{
	return (*registry)[id];
}

TRTile* TRTileManager::GetTileByKey(const std::string& key) const
{
	return (*registry)[key];
}

TRTile* TRTileManager::TileAir() const
{
	return tile_air;
}
