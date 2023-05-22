#pragma once

#include "TRWorldGenerationProcess.h"
#include "TRTileMap.h"

class TRWorldGeneration
{
private:
	std::vector<TRWorldGenerationProcess*>* processes;
public:
	TRWorldGeneration();
	~TRWorldGeneration();

	void AddProcess(TRWorldGenerationProcess* process);
	void GenerateWorld(TRTileMap* tile_map, int width, int height, int seed);
};

