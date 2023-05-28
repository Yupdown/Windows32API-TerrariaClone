#pragma once

#include <vector>
#include "TRTileMap.h"
#include "TRTile.h"

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

class TRWorldGenerationPinchCaves : public TRWorldGenerationProcess
{
public:
	virtual void GenerateWorld(TRTileMap* tile_map, int width, int height, int seed) override;
};

class TRWorldGenerationGrowGrass : public TRWorldGenerationProcess
{
public:
	virtual void GenerateWorld(TRTileMap* tile_map, int width, int height, int seed) override;
};

class TRWorldGenerationGrowOres : public TRWorldGenerationProcess
{
private:
	TRTile* ore_tile;
	int seed;

public:
	TRWorldGenerationGrowOres(TRTile* _tile, int _seed);
	virtual void GenerateWorld(TRTileMap* tile_map, int width, int height, int seed) override;
};

class TRWorldGenerationAttachWall : public TRWorldGenerationProcess
{
public:
	virtual void GenerateWorld(TRTileMap* tile_map, int width, int height, int seed) override;
};