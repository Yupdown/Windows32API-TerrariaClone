#include "pch.h"
#include "TRWorldGeneration.h"
#include "TRTileManager.h"
#include "TRTile.h"

TRWorldGeneration::TRWorldGeneration()
{
	processes = new vector<TRWorldGenerationProcess*>();
}

TRWorldGeneration::~TRWorldGeneration()
{
	delete processes;
}

void TRWorldGeneration::AddProcess(TRWorldGenerationProcess* process)
{
	processes->push_back(process);
}

void TRWorldGeneration::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    for (int i = 0; i < width; ++i)
    {
		for (int j = 0; j < height; ++j)
		{
			tile_map->SetTile(i, j, TRTileManager::GetInst()->TileAir());
			tile_map->SetTileWall(i, j, TRTileManager::GetInst()->TileWallAir());
		}
    }

	for (TRWorldGenerationProcess* process : *processes)
		process->GenerateWorld(tile_map, width, height, seed);
}