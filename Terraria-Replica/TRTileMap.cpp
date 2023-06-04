#include "pch.h"
#include "TRWorld.h"
#include "TRTile.h"
#include "TRTileMap.h"
#include "TRTileManager.h"
#include "CTileLayer.h"
#include "CAtlasMgr.h"
#include "CAtlasElement.h"
#include "CResMgr.h"
#include "Vec2Int.hpp"
#include "CCore.h"

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

TRTile* TRTileMap::SetTile(int x, int y, TRTile* new_tile, bool render)
{
	TRTile** ref = GetTileReference(x, y);
	if (ref == nullptr)
		return nullptr;

	TRTile* old_tile = *ref;
	*ref = new_tile;

	if (new_tile != old_tile && render)
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

TRTileWall* TRTileMap::SetTileWall(int x, int y, TRTileWall* new_tile, bool render)
{
	TRTileWall** ref = GetTileWallReference(x, y);
	if (ref == nullptr)
		return nullptr;

	TRTileWall* old_tile = *ref;
	*ref = new_tile;

	if (new_tile != old_tile && render)
		UpdateTileRenderer(x, y);

	return old_tile;
}

void TRTileMap::OnSceneCreate(CScene* scene)
{
	Vec2Int tile_pixel_size = Vec2Int(tile_width * PIXELS_PER_TILE, tile_height * PIXELS_PER_TILE);
	renderer = new CTileLayer(tile_pixel_size / 2, tile_pixel_size.x, tile_pixel_size.y);

	TRTileWall* tile_wall_air = Mgr(TRTileManager)->TileWallAir();

	for (int x = 0; x < tile_width; ++x)
	{
		for (int y = 0; y < tile_height; ++y)
		{
			TRTileWall* tile = GetTileWall(x, y);
			if (tile == tile_wall_air)
				continue;

			int bitmask = GetTileWallNeighborMask(x, y);
			tile->OnDrawElement(renderer, x, y, bitmask);
		}
	}

	for (int x = 0; x < tile_width; ++x)
	{
		for (int y = 0; y < tile_height; ++y)
		{
			TRTile* tile = GetTile(x, y);
			if (!tile->Solid())
				continue;

			int bitmask = GetTileNeighborMask(x, y);
			tile->OnDrawElement(renderer, x, y, bitmask);
		}
	}

	Mgr(CResMgr)->Clear();
	scene->AddTileLayer(renderer);
}

void TRTileMap::UpdateTileRenderer(int x, int y)
{
	static HBRUSH brush = CreateSolidBrush(0x00FF00FF);
	HDC hdc = renderer->GetTileLayerDC();
	
	Vec2Int p = TRWorld::WorldToGlobal(Vec2(static_cast<float>(x), static_cast<float>(y)));
	RECT r = { p.x - PIXELS_PER_TILE, p.y - PIXELS_PER_TILE * 2, p.x + PIXELS_PER_TILE * 2, p.y + PIXELS_PER_TILE };
	FillRect(hdc, &r, brush);

	for (int dx = -2; dx <= 2; ++dx)
	{
		for (int dy = -2; dy <= 2; ++dy)
		{
			int xp = x + dx;
			int yp = y + dy;

			TRTileWall* tile = GetTileWall(xp, yp);
			if (tile == nullptr)
				return;

			int bitmask = GetTileWallNeighborMask(xp, yp);

			int sx = max(dx * -2 - 1, 0);
			int sy = max(dy * 2 - 1, 0);
			int sw = min(abs(dx) * -2 + 5, 4);
			int sh = min(abs(dy) * -2 + 5, 4);

			tile->OnDrawElement(renderer, x + max(dx, -1), y + min(dy, 1), bitmask, sx, sy, sw, sh);
		}
	}

	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			int xp = x + dx;
			int yp = y + dy;

			TRTile* tile = GetTile(xp, yp);
			if (tile == nullptr)
				return;

			int bitmask = GetTileNeighborMask(xp, yp);
			tile->OnDrawElement(renderer, xp, yp, bitmask);
		}
	}
}

int TRTileMap::GetTileNeighborMask(int x, int y) const
{
	TRTile* tile = GetTile(x, y);
	const int dir[][2] = { 0, 1, 0, -1, -1, 0, 1, 0, 1, 1, -1, -1, -1, 1, 1, -1 };
	TRTile* tile_dirt = Mgr(TRTileManager)->GetTileByID(2);
	TRTile* tile_grass = Mgr(TRTileManager)->GetTileByID(3);

	int bitmask = 0;

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

	return bitmask;
}

int TRTileMap::GetTileWallNeighborMask(int x, int y) const
{
	const int dir[][2] = { 0, 1, 0, -1, -1, 0, 1, 0, 1, 1, -1, -1, -1, 1, 1, -1 };
	TRTileWall* tile_wall_air = Mgr(TRTileManager)->TileWallAir();

	int bitmask = 0;

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

	return bitmask;
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