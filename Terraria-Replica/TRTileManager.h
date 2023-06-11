#pragma once
#include "Singleton.hpp"
#include "Registry.hpp"
#include "TRTile.h"
#include "TRTileWall.h"

class TRTileManager : public Singleton<TRTileManager>
{
private:
	Registry<TRTile>* registry_tile;
	Registry<TRTileWall>* registry_tile_wall;
	TRTile* tile_air;
	TRTileWall* tile_wall_air;

public:
	friend class Singleton;

	TRTileManager();
	~TRTileManager();

	void LoadTiles();
	void Initialize();

	TRTile* GetTileByID(int id) const;
	TRTile* GetTileByKey(const std::wstring& key) const;
	TRTile* TileAir() const;

	TRTileWall* GetTileWallByID(int id) const;
	TRTileWall* GetTileWallByKey(const std::wstring& key) const;
	TRTileWall* TileWallAir() const;
};

