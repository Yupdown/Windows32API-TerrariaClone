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
