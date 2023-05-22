#pragma once

#include "TRTile.h"
#include "TRTileMap.h"
#include "TRWorldGeneration.h"
#include "CScene.h"

class TRWorld
{
public:
	static constexpr int WORLD_WIDTH = 256;
	static constexpr int WORLD_HEIGHT = 256;

private:
	TRTileMap* tile_map;

public:
	TRWorld();
	~TRWorld();

	void CreateWorld(int seed);
	void OnSceneCreate(CScene* scene);
};

