#include "pch.h"
#include "TRWorldGenerationProcess.h"
#include "PerlinNoise.hpp"
#include "TRTileManager.h"
#include "TRTile.h"

void TRWorldGenerationTerrainHeight::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    for (int i = 0; i < width; ++i)
    {
        float noise_value = Perlin::Fbm(i * 0.02f, 4);
        int h = height - RoundToInt((noise_value + 1.0f) * 30.0f);
        for (int j = 0; j < h; ++j)
        {
            TRTile* tile = TRTileManager::GetInst()->GetTileByID(j + 10 >= h ? 2 : 1);
            tile_map->SetTile(i, height - 1 - j, tile);
        }
    }
}

void TRWorldGenerationPinchCaves::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            float noise_value = Perlin::Fbm(i * 0.05f, j * 0.05f, 4) + 0.2f;
            float value = Clamp01(Ceil(noise_value));

            if (FloorToInt(value) == 0)
                tile_map->SetTile(i, j, TRTileManager::GetInst()->TileAir());
        }
    }
}

void TRWorldGenerationGrowGrass::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    TRTile* dirt = TRTileManager::GetInst()->GetTileByKey("dirt");
    TRTile* grass = TRTileManager::GetInst()->GetTileByKey("dirtgrass");

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (tile_map->GetTile(i, j) != dirt)
                continue;

            bool exposed = false;
            exposed |= i > 0 && !tile_map->GetTile(i - 1, j)->Solid();
            exposed |= i < width - 1 && !tile_map->GetTile(i + 1, j)->Solid();
            exposed |= j > 0 && !tile_map->GetTile(i, j - 1)->Solid();
            exposed |= j < height - 1 && !tile_map->GetTile(i, j + 1)->Solid();

            if (exposed)
                tile_map->SetTile(i, j, grass);
        }
    }
}

TRWorldGenerationGrowOres::TRWorldGenerationGrowOres(TRTile* _tile, int _seed)
{
    ore_tile = _tile;
    seed = _seed;
}

void TRWorldGenerationGrowOres::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    TRTile* cobblestone = TRTileManager::GetInst()->GetTileByKey("cobblestone");

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (tile_map->GetTile(i, j) != cobblestone)
                continue;

            float noise_value = Perlin::Noise(i * 0.1f + this->seed * 100, j * 0.1f + this->seed * 100) + 0.5f;
            float value = Clamp01(Ceil(noise_value));

            if (value == 0)
                tile_map->SetTile(i, j, ore_tile);
        }
    }
}