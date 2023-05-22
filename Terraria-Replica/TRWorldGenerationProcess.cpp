#include "pch.h"
#include "TRWorldGenerationProcess.h"
#include "PerlinNoise.hpp"
#include "TRTileManager.h"
#include "TRTile.h"

void TRWorldGenerationTerrainHeight::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            TRTile* air = TRTileManager::GetInst()->TileAir();
            tile_map->SetTile(i, j, air);
        }
    }

    for (int i = 0; i < width; ++i)
    {
        float noise_value = Perlin::Fbm(i * 0.02f, 4);
        int h = height - RoundToInt((noise_value + 1.0f) * 30.0f);
        for (int j = 0; j < h; ++j)
        {
            TRTile* tile = TRTileManager::GetInst()->GetTileByID(j + 1 >= h ? 3 : j + 10 >= h ? 2 : 1);
            tile_map->SetTile(i, height - 1 - j, tile);
        }
    }

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            float noise_value = Perlin::Fbm(i * 0.05f, j * 0.05f, 4) + 0.2f;
            float value = Clamp01(Ceil(noise_value));

            if (FloorToInt(value) == 0)
            {
                TRTile* air = TRTileManager::GetInst()->TileAir();
                tile_map->SetTile(i, j, air);
            }
        }
    }
}
