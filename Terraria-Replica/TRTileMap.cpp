#include "pch.h"
#include "TRTileMap.h"

TRTileMap::TRTileMap(int width, int height)
{
	tile_width = width;
	tile_height = height;

	tile_map = new TRTile*[width * height];
}

TRTileMap::~TRTileMap()
{
	delete[] tile_map;
}

TRTile* TRTileMap::GetTile(int x, int y) const
{
	TRTile** ref = GetTileReference(x, y);
	if (ref == nullptr)
		return nullptr;
	return *ref;
}

void TRTileMap::SetTile(int x, int y, TRTile* new_tile)
{
	TRTile** ref = GetTileReference(x, y);
	if (ref == nullptr)
		return;
	*ref = new_tile;
}

TRTile** TRTileMap::GetTileReference(int x, int y) const
{
	if (x < 0 || x >= tile_width)
		return nullptr;
	if (y < 0 || y >= tile_height)
		return nullptr;
	return &tile_map[x % tile_width + y * tile_width];
}
