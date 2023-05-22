#include "pch.h"
#include "TRWorldGeneration.h"

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
	for (TRWorldGenerationProcess* process : *processes)
		process->GenerateWorld(tile_map, width, height, seed);
}