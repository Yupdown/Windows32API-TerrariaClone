#pragma once

#include "TRTile.h"
#include "TRTileMap.h"
#include "TRWorldGeneration.h"

class TRWorld
{
public:
	static constexpr int WORLD_WIDTH = 256;
	static constexpr int WORLD_HEIGHT = 128;

private:
	TRTileMap* tile_map;

public:
	TRWorld();
	~TRWorld();

	void CreateWorld(int seed);
};

