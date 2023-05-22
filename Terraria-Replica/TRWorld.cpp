#include "pch.h"
#include "TRWorld.h"

TRWorld::TRWorld()
{
	tile_map = new TRTileMap(TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT);
}

TRWorld::~TRWorld()
{
	delete tile_map;
}

void TRWorld::CreateWorld(int seed)
{
	TRWorldGeneration* generator = new TRWorldGeneration();
	generator->GenerateWorld(tile_map, TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT, seed);
}
