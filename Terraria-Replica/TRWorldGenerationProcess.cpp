#include "pch.h"
#include "TRWorldGenerationProcess.h"
#include "PerlinNoise.hpp"
#include "TRTileManager.h"
#include "TRTile.h"
#include "TRTileWall.h"

void TRWorldGenerationTerrainHeight::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    int ox = seed & 1023;

    for (int i = 0; i < width; ++i)
    {
        float noise_value = Perlin::Fbm(ox + i * 0.02f, 4);
        int h = height - 64 + RoundToInt(noise_value * 16.0f);
        for (int j = 0; j < h; ++j)
        {
            TRTile* tile = TRTileManager::GetInst()->GetTileByID(j + 10 >= h ? 2 : 1);
            tile_map->SetTile(i, j, tile);
        }
    }
}

void TRWorldGenerationPinchCaves::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    int ox = seed & 1023;
    int oy = (seed >> 10) & 1023;

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            float noise_value = Perlin::Fbm(ox + i * 0.05f, oy + j * 0.05f, 4) + 0.2f;
            float value = Clamp01(Ceil(noise_value));

            if (FloorToInt(value) == 0)
                tile_map->SetTile(i, j, TRTileManager::GetInst()->TileAir());
        }
    }
}

void TRWorldGenerationGrowGrass::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    TRTile* dirt = TRTileManager::GetInst()->GetTileByKey(L"dirt");
    TRTile* grass = TRTileManager::GetInst()->GetTileByKey(L"dirtgrass");

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (tile_map->GetTile(i, j) != dirt)
                continue;

            int dir[][2] = { 0, -1, 0, 1, -1, 0, 1, 0, -1, -1, -1, 1, 1, -1, 1, 1 };
            bool exposed = false;

            for (int k = 0; k < 8 && !exposed; ++k)
            {
                int xp = i + dir[k][0];
                int yp = j + dir[k][1];

                if (yp < 0 || yp >= height || xp < 0 || xp >= width)
                    continue;
                exposed |= !tile_map->GetTile(xp, yp)->Solid();
            }

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
    int ox = seed & 1023;
    int oy = (seed >> 10) & 1023;

    TRTile* cobblestone = TRTileManager::GetInst()->GetTileByKey(L"cobblestone");

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (tile_map->GetTile(i, j) != cobblestone)
                continue;

            float noise_value = Perlin::Noise(ox + i * 0.1f + this->seed * 100, oy + j * 0.1f + this->seed * 100) + 0.5f;
            float value = Clamp01(Ceil(noise_value));

            if (value == 0)
                tile_map->SetTile(i, j, ore_tile);
        }
    }
}

void TRWorldGenerationAttachWall::GenerateWorld(TRTileMap* tile_map, int width, int height, int seed)
{
    int dir[][2] = { 0, -1, 0, 1, -1, 0, 1, 0, 1, -1, -1, 1, -1, -1, 1, 1 };

    TRTile* tile_dirt = Mgr(TRTileManager)->GetTileByKey(L"dirt");
    TRTile* tile_stone = Mgr(TRTileManager)->GetTileByKey(L"cobblestone");

    TRTileWall* tile_wall_dirt = Mgr(TRTileManager)->GetTileWallByKey(L"dirt");
    TRTileWall* tile_wall_stone = Mgr(TRTileManager)->GetTileWallByKey(L"cobblestone");

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            TRTile* tile = tile_map->GetTile(i, j);
            int bitmask = 255;

            for (int k = 0; k < 8; ++k)
            {
                int xp = i + dir[k][0];
                int yp = j + dir[k][1];

                if (yp < 0 || yp >= height || xp < 0 || xp >= width)
                    continue;

                TRTile* tile_p = tile_map->GetTile(xp, yp);

                if (!tile_p->Solid())
                    bitmask &= ~(1 << k);
            }

            if (bitmask == 255)
            {
                if (tile == tile_dirt)
                    tile_map->SetTileWall(i, j, tile_wall_dirt);
                else if (tile == tile_stone)
                    tile_map->SetTileWall(i, j, tile_wall_stone);
            }
        }
    }
}
