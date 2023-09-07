#include "pch.h"

#include "TRTileMapShade.h"
#include "TRTileManager.h"
#include "TRWorld.h"
#include "TRTile.h"
#include "TRTileWall.h"

#include "CAtlasMgr.h"
#include "CTileLayer.h"
#include "CAtlasElement.h"

#include "Vec2Int.hpp"

TRTileMapShade::TRTileMapShade(int width, int height)
{
	tile_width = width * 2;
	tile_height = height * 2;

	light_level = new int[tile_width * tile_height];

	for (int i = 0; i < 16; ++i)
		elements[i] = Mgr(CAtlasMgr)->GetAtlasElement(L"tile_shade.png", { i * 8.0f, 0.0f }, { 8.0f, 8.0f });
}

TRTileMapShade::~TRTileMapShade()
{
	delete[] light_level;
}

void TRTileMapShade::BuildLightLevelMap(const TRTileMap& tile_map)
{
	std::queue<Vec2Int> bfs;
	memset(light_level, 0, tile_width * tile_height * sizeof(int));
	TRTileWall* tile_wall_air = Mgr(TRTileManager)->TileWallAir();
	const int dir[][2] = { 0, 1, 0, -1, -1, 0, 1, 0 };

	for (int x = 0; x < tile_width; ++x)
	{
		for (int y = 0; y < tile_height; ++y)
		{
			TRTile* tile = tile_map.GetTile(x >> 1, y >> 1);
			if (tile->Solid())
				continue;

			bfs.push(Vec2Int(x, y));
			while (!bfs.empty())
			{
				int xx = bfs.front().x;
				int yy = bfs.front().y;
				bfs.pop();

				TRTile* tile_from = tile_map.GetTile(xx >> 1, yy >> 1);
				TRTileWall* tile_wall_from = tile_map.GetTileWall(xx >> 1, yy >> 1);

				if (!tile_from->Solid() && tile_wall_from == tile_wall_air)
					SetLightLevel(xx, yy, 32);
				int level = GetLightLevel(xx, yy);
				if (level == 0)
					continue;

				for (int k = 0; k < 4; ++k)
				{
					int xp = xx + dir[k][0];
					int yp = yy + dir[k][1];

					if (yp < 0 || yp >= tile_height || xp < 0 || xp >= tile_width)
						continue;

					TRTile* tile_to = tile_map.GetTile(xp >> 1, yp >> 1);
					TRTileWall* tile_wall_to = tile_map.GetTileWall(xp >> 1, yp >> 1);

					if (!tile_to->Solid() && tile_wall_to == tile_wall_air)
						continue;

					int sub_level = level;
					if (tile_from->Solid())
						sub_level = max(level - 4, 0);
					else if (tile_wall_from != tile_wall_air)
						sub_level = max(level - 1, 0);

					if (GetLightLevel(xp, yp) >= sub_level)
						continue;

					SetLightLevel(xp, yp, sub_level);
					bfs.push(Vec2Int(xp, yp));
				}
			}
		}
	}
}

int TRTileMapShade::GetLightLevel(int x, int y) const
{
	if (x < 0 || x >= tile_width)
		return 32;
	if (y < 0 || y >= tile_height)
		return 32;
	return light_level[x % tile_width + y * tile_width];
}

void TRTileMapShade::SetLightLevel(int x, int y, int v)
{
	if (x < 0 || x >= tile_width)
		return;
	if (y < 0 || y >= tile_height)
		return;
	light_level[x % tile_width + y * tile_width] = v;
}

void TRTileMapShade::OnDrawElement(CTileLayer* tilemap_layer, int x, int y) const
{
	HDC hdc = tilemap_layer->GetTileLayerDC();
	Vec2Int p = TRWorld::WorldToGlobal(Vec2Int(x, y + 1));
	const int offset[][2] = { 0, 0, 0, 1, 1, 0, 1, 1 };

	for (int i = 0; i < 4; ++i)
	{
		int light_level = GetLightLevel(x * 2 + offset[i][0], y * 2 + 1 - offset[i][1]);
		if (light_level >= 32)
			return;

		elements[light_level / 2]->render(hdc, p + Vec2Int(offset[i][0] * 8, offset[i][1] * 8), Vec2Int(8, 8));
	}
}
