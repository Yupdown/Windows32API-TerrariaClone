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
	registry->Insert("cobblestone", new TRTileSolid(1.0f, "", ""));
	registry->Insert("dirt", new TRTileSolid(1.0f, "", ""));
	registry->Insert("sand", new TRTileSolid(1.0f, "", ""));
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
