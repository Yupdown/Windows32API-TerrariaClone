#pragma once
#include "Singleton.hpp"
#include "Registry.hpp"
#include "TRTile.h"

class TRTileManager : public Singleton<TRTileManager>
{
private:
	Registry<TRTile*>* registry;
	TRTile* tile_air;

public:
	friend class Singleton;

	TRTileManager();
	~TRTileManager();

	void LoadTiles();
	void Initialize();

	TRTile* GetTileByID(int id) const;
	TRTile* GetTileByKey(const std::string& key) const;
	TRTile* TileAir() const;
};

