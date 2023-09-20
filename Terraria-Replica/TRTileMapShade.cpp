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
	light_level_last = new int[tile_width * tile_height];

	for (int i = 0; i < 16; ++i)
		elements[i] = Mgr(CAtlasMgr)->GetAtlasElement(L"tile_shade.png", { i * 8.0f, 0.0f }, { 8.0f, 8.0f });
}

TRTileMapShade::~TRTileMapShade()
{
	delete[] light_level;
	delete[] light_level_last;
	DeleteObject(hTileMapBrush);
}

void TRTileMapShade::BuildLightLevelMap(const TRTileMap& tile_map)
{
	InvalidateLightLevelMap(tile_map, 0, 0, tile_width, tile_height);
}

void TRTileMapShade::InvalidateLightLevelMap(const TRTileMap& tile_map, int x0, int y0, int x1, int y1, bool for_redraw)
{
	std::queue<Vec2Int> bfs;
	TRTileWall* tile_wall_air = Mgr(TRTileManager)->TileWallAir();
	const int dir[][2] = { 0, 1, 0, -1, -1, 0, 1, 0 };

	x0 = max(x0, 0);
	y0 = max(y0, 0);
	x1 = min(x1, tile_width);
	y1 = min(y1, tile_height);

	for (int x = x0; x < x1; ++x)
	{
		if (y0 > 0)
			bfs.push(Vec2Int(x, y0 - 1));
		if (y1 < tile_height)
			bfs.push(Vec2Int(x, y1));
	}

	for (int y = y0; y < y1; ++y)
	{
		if (x0 > 0)
			bfs.push(Vec2Int(x0 - 1, y));
		if (x1 < tile_width)
			bfs.push(Vec2Int(x1, y));
	}

	for (int x = x0; x < x1; ++x)
	{
		for (int y = y0; y < y1; ++y)
		{
			light_level_last[x % tile_width + y * tile_width] = GetLightLevel(x, y);
			SetLightLevel(x, y, 0);
			TRTile* tile = tile_map.GetTile(x >> 1, y >> 1);
			if (tile->Solid())
				continue;
			bfs.push(Vec2Int(x, y));
		}
	}

	while (!bfs.empty())
	{
		int xx = bfs.front().x;
		int yy = bfs.front().y;
		bfs.pop();

		TRTile* tile_from = tile_map.GetTile(xx >> 1, yy >> 1);
		TRTileWall* tile_wall_from = tile_map.GetTileWall(xx >> 1, yy >> 1);

		if (!tile_from->Solid() && tile_wall_from == tile_wall_air)
			SetLightLevel(xx, yy, 32);
		else if (tile_from->LightLevel() > 0)
			SetLightLevel(xx, yy, tile_from->LightLevel());
		int level = GetLightLevel(xx, yy);
		if (level == 0)
			continue;

		for (int k = 0; k < 4; ++k)
		{
			int xp = xx + dir[k][0];
			int yp = yy + dir[k][1];

			if (yp < y0 || yp >= y1 || xp < x0 || xp >= x1)
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

	for (int x = x0; x < x1; ++x)
	{
		for (int y = y0; y < y1; ++y)
		{
			if (light_level_last[x % tile_width + y * tile_width] != GetLightLevel(x, y))
			{
				if (for_redraw)
					redraw_buffer.push(Vec2Int(x, y));
				light_level_last[x % tile_width + y * tile_width] = GetLightLevel(x, y);
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

void TRTileMapShade::RedrawInvalidated(CTileLayer* tilemap_layer)
{
	HDC hdc = tilemap_layer->GetTileLayerDC();
	
	while (!redraw_buffer.empty())
	{
		Vec2Int e = redraw_buffer.front();
		Vec2Int p = TRWorld::WorldToGlobal(Vec2(e.x * 0.5f, (e.y + 1) * 0.5f));
		RECT r = { p.x, p.y, p.x + 8, p.y + 8 };
		redraw_buffer.pop();

		FillRect(hdc, &r, hTileMapBrush);
		int light_level = GetLightLevel(e.x, e.y);
		if (light_level >= 32)
			continue;
		elements[light_level / 2]->render(hdc, p, Vec2Int(8, 8));
	}
}