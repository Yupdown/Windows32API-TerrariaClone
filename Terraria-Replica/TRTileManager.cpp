#include "pch.h"
#include "TRTileManager.h"

TRTileManager::TRTileManager()
{
	registry_tile = new Registry<TRTile>();
	registry_tile_wall = new Registry<TRTileWall>();
}

TRTileManager::~TRTileManager()
{
	delete registry_tile;
	delete registry_tile_wall;
}

void TRTileManager::LoadTiles()
{
	tile_air = new TRTileAir();
	registry_tile->Insert(L"air", tile_air);
	registry_tile->Insert(L"cobblestone", new TRTileSolid(L"Cobblestone", 1.0f, true, L"tile_cobblestone.png", L"tile_cobblestone"));
	registry_tile->Insert(L"dirt", new TRTileDirt(L"Dirt", 1.0f, L"tile_dirt.png", L"tile_dirt"));
	registry_tile->Insert(L"dirtgrass", new TRTileGrass(L"Grass Soil", 1.0f, L"tile_dirtgrass.png", L"tile_dirt"));
	registry_tile->Insert(L"clay", new TRTileSolid(L"Clay", 1.0f, false, L"tile_clay.png", L""));
	registry_tile->Insert(L"copper_ore", new TRTileOre(L"Copper Ore", 1.0f, L"tile_copperore.png", L"tile_copper_ore"));
	registry_tile->Insert(L"iron_ore", new TRTileOre(L"Iron Ore", 1.0f, L"tile_ironore.png", L"tile_iron_ore"));
	registry_tile->Insert(L"silver_ore", new TRTileOre(L"Silver Ore", 1.0f, L"tile_silverore.png", L"tile_silver_ore"));
	registry_tile->Insert(L"gold_ore", new TRTileOre(L"Gold Ore", 1.0f, L"tile_goldore.png", L"tile_gold_ore"));
	registry_tile->Insert(L"planks_wood", new TRTileSolid(L"Wooden Planks", 1.0f, false, L"tile_planks.png", L"tile_planks_wood"));
	registry_tile->Insert(L"bricks_clay", new TRTileSolid(L"Bricks", 1.0f, true, L"tile_bricks.png", L"tile_bricks_clay"));
	registry_tile->Insert(L"bricks_stone", new TRTileSolid(L"Stone Bricks", 1.0f, true, L"tile_stonebricks.png", L"tile_bricks_stone"));
	registry_tile->Insert(L"torch", new TRTileTorch(L"Torch", L"tile_torch.png", L"tile_torch"));

	tile_wall_air = new TRTileWallAir();
	registry_tile_wall->Insert(L"air", tile_wall_air);
	registry_tile_wall->Insert(L"cobblestone", new TRTileWall(L"Cobblestone Wall", L"wall_cobblestone.png", L"wall_cobblestone"));
	registry_tile_wall->Insert(L"dirt", new TRTileWall(L"Dirt Wall", L"wall_dirt.png", L"wall_dirt"));
	registry_tile_wall->Insert(L"planks_wood", new TRTileWall(L"Wooden Planks Wall", L"wall_planks.png", L"wall_planks_wood"));
	registry_tile_wall->Insert(L"bricks_clay", new TRTileWall(L"Bricks Wall", L"wall_bricks.png", L"wall_bricks_clay"));
	registry_tile_wall->Insert(L"bricks_stone", new TRTileWall(L"Stone Bricks Wall", L"wall_stonebricks.png", L"wall_bricks_stone"));
}

void TRTileManager::Initialize()
{
}

TRTile* TRTileManager::GetTileByID(int id) const
{
	return (*registry_tile)[id];
}

TRTile* TRTileManager::GetTileByKey(const std::wstring& key) const
{
	return (*registry_tile)[key];
}

TRTile* TRTileManager::TileAir() const
{
	return tile_air;
}

TRTileWall* TRTileManager::GetTileWallByID(int id) const
{
	return (*registry_tile_wall)[id];
}

TRTileWall* TRTileManager::GetTileWallByKey(const std::wstring& key) const
{
	return (*registry_tile_wall)[key];
}

TRTileWall* TRTileManager::TileWallAir() const
{
	return tile_wall_air;
}

