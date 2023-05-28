#include "pch.h"
#include "TRTileManager.h"

TRTileManager::TRTileManager()
{
	registry_tile = new Registry<TRTile*>();
	registry_tile_wall = new Registry<TRTileWall*>();
}

TRTileManager::~TRTileManager()
{
	delete registry_tile;
	delete registry_tile_wall;
}

void TRTileManager::LoadTiles()
{
	tile_air = new TRTileAir();
	registry_tile->Insert("air", tile_air);
	registry_tile->Insert("cobblestone", new TRTileSolid(L"Cobblestone", 1.0f, L"tile_cobblestone.png", L""));
	registry_tile->Insert("dirt", new TRTileDirt(L"Dirt", 1.0f, L"tile_dirt.png", L""));
	registry_tile->Insert("dirtgrass", new TRTileGrass(L"Grass Soil", 1.0f, L"tile_dirtgrass.png", L""));
	registry_tile->Insert("clay", new TRTileSolid(L"Clay", 1.0f, L"tile_clay.png", L""));
	registry_tile->Insert("copper_ore", new TRTileOre(L"Copper Ore", 1.0f, L"tile_copperore.png", L""));
	registry_tile->Insert("iron_ore", new TRTileOre(L"Iron Ore", 1.0f, L"tile_ironore.png", L""));
	registry_tile->Insert("silver_ore", new TRTileOre(L"Silver Ore", 1.0f, L"tile_silverore.png", L""));
	registry_tile->Insert("gold_ore", new TRTileOre(L"Gold Ore", 1.0f, L"tile_goldore.png", L""));
	registry_tile->Insert("planks_wood", new TRTileSolid(L"Wooden Planks", 1.0f, L"tile_planks.png", L""));
	registry_tile->Insert("bricks_clay", new TRTileSolid(L"Bricks", 1.0f, L"tile_bricks.png", L""));
	registry_tile->Insert("bricks_stone", new TRTileSolid(L"Stone Bricks", 1.0f, L"tile_stonebricks.png", L""));

	tile_wall_air = new TRTileWallAir();
	registry_tile_wall->Insert("air", tile_wall_air);
	registry_tile_wall->Insert("cobblestone", new TRTileWall(L"Cobblestone Wall", L"wall_cobblestone.png", L""));
	registry_tile_wall->Insert("dirt", new TRTileWall(L"Dirt Wall", L"wall_dirt.png", L""));
	registry_tile_wall->Insert("planks_wood", new TRTileWall(L"Wooden Planks Wall", L"wall_planks.png", L""));
	registry_tile_wall->Insert("bricks_clay", new TRTileWall(L"Bricks Wall", L"wall_bricks.png", L""));
	registry_tile_wall->Insert("bricks_stone", new TRTileWall(L"Stone Bricks Wall", L"wall_stonebricks.png", L""));
}

void TRTileManager::Initialize()
{
	
}

TRTile* TRTileManager::GetTileByID(int id) const
{
	return (*registry_tile)[id];
}

TRTile* TRTileManager::GetTileByKey(const std::string& key) const
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

TRTileWall* TRTileManager::GetTileWallByKey(const std::string& key) const
{
	return (*registry_tile_wall)[key];
}

TRTileWall* TRTileManager::TileWallAir() const
{
	return tile_wall_air;
}
