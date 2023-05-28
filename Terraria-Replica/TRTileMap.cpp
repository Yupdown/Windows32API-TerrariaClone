#include "pch.h"
#include "TRTile.h"
#include "TRTileMap.h"
#include "TRTileManager.h"
#include "CTileLayer.h"
#include "CAtlasMgr.h"
#include "CAtlasElement.h"
#include "CResMgr.h"
#include "Vec2Int.hpp"

TRTileMap::TRTileMap(int width, int height)
{
	tile_width = width;
	tile_height = height;

	tile_map = new TRTile*[width * height];
	tile_wall_map = new TRTileWall*[width * height];
	renderer = nullptr;
}

TRTileMap::~TRTileMap()
{
	delete[] tile_map;
	delete[] tile_wall_map;
}

TRTile* TRTileMap::GetTile(int x, int y) const
{
	TRTile** ref = GetTileReference(x, y);
	if (ref == nullptr)
		return nullptr;

	return *ref;
}

TRTile* TRTileMap::SetTile(int x, int y, TRTile* new_tile)
{
	TRTile** ref = GetTileReference(x, y);
	if (ref == nullptr)
		return nullptr;

	TRTile* old_tile = *ref;
	*ref = new_tile;

	if (new_tile != old_tile)
		UpdateTileRenderer(x, y);

	return old_tile;
}

TRTileWall* TRTileMap::GetTileWall(int x, int y) const
{
	TRTileWall** ref = GetTileWallReference(x, y);
	if (ref == nullptr)
		return nullptr;

	return *ref;
}

TRTileWall* TRTileMap::SetTileWall(int x, int y, TRTileWall* new_tile)
{
	TRTileWall** ref = GetTileWallReference(x, y);
	if (ref == nullptr)
		return nullptr;

	TRTileWall* old_tile = *ref;
	*ref = new_tile;

	if (new_tile != old_tile)
		UpdateTileRenderer(x, y);

	return old_tile;
}

void TRTileMap::OnSceneCreate(CScene* scene)
{
	Vec2Int tile_pixel_size = Vec2Int(tile_width * PIXELS_PER_TILE, tile_height * PIXELS_PER_TILE);
	renderer = new CTileLayer(tile_pixel_size / 2, tile_pixel_size.x, tile_pixel_size.y);

	TRTile* tile_dirt = Mgr(TRTileManager)->GetTileByKey("dirt");
	TRTile* tile_grass = Mgr(TRTileManager)->GetTileByKey("dirtgrass");
	TRTileWall* tile_wall_air = Mgr(TRTileManager)->TileWallAir();

	int dir[][2] = { 0, -1, 0, 1, -1, 0, 1, 0, 1, -1, -1, 1, -1, -1, 1, 1 };

	for (int x = 0; x < tile_width; ++x)
	{
		for (int y = 0; y < tile_height; ++y)
		{
			int bitmask = 0;

			TRTileWall* tile = GetTileWall(x, y);
			if (tile == tile_wall_air)
				continue;

            for (int k = 0; k < 8; ++k)
            {
                int xp = x + dir[k][0];
                int yp = y + dir[k][1];

				if (yp < 0 || yp >= tile_height || xp < 0 || xp >= tile_width)
					bitmask |= 1 << k;
				else
				{
					TRTileWall* tile_p = GetTileWall(xp, yp);

					if (tile_p != tile_wall_air)
						bitmask |= 1 << k;
				}
            }

			tile->OnDrawElement(renderer, x, y, bitmask);
		}
	}

	for (int x = 0; x < tile_width; ++x)
	{
		for (int y = 0; y < tile_height; ++y)
		{
			int bitmask = 0;

			TRTile* tile = GetTile(x, y);
			if (!tile->Solid())
				continue;

			for (int k = 0; k < 8; ++k)
			{
				int xp = x + dir[k][0];
				int yp = y + dir[k][1];

				if (yp < 0 || yp >= tile_height || xp < 0 || xp >= tile_width)
					bitmask |= 1 << k;
				else
				{
					TRTile* tile_p = GetTile(xp, yp);

					bool is_from_dirt = tile == tile_dirt || tile == tile_grass;
					bool is_to_dirt = tile_p == tile_dirt || tile_p == tile_grass;
					bool flag_stick = tile_p->StickGroup() & tile->StickGroup();
					if (!tile_p->StickEach() && tile_p->StickGroup() == tile->StickGroup())
						flag_stick &= tile_p == tile;

					if (flag_stick)
						bitmask |= 1 << k;
					if (!is_from_dirt)
						bitmask |= is_to_dirt << (k + 8);
				}
			}

			tile->OnDrawElement(renderer, x, y, bitmask);
		}
	}

	Mgr(CResMgr)->Clear();
	scene->AddTileLayer(renderer);
}

void TRTileMap::UpdateTileRenderer(int x, int y)
{

}

TRTile** TRTileMap::GetTileReference(int x, int y) const
{
	if (x < 0 || x >= tile_width)
		return nullptr;
	if (y < 0 || y >= tile_height)
		return nullptr;
	return &tile_map[x % tile_width + y * tile_width];
}

TRTileWall** TRTileMap::GetTileWallReference(int x, int y) const
{
	if (x < 0 || x >= tile_width)
		return nullptr;
	if (y < 0 || y >= tile_height)
		return nullptr;
	return &tile_wall_map[x % tile_width + y * tile_width];
}