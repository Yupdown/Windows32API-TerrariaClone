#pragma once

#include <vector>
#include "TRTileMap.h"

class TRWorldGenerationProcess
{
public:
	virtual void GenerateWorld(TRTileMap* tile_map, int width, int height, int seed) = 0;
};

class TRWorldGenerationTerrainHeight : public TRWorldGenerationProcess
{
public:
	virtual void GenerateWorld(TRTileMap* tile_map, int width, int height, int seed) override;
};