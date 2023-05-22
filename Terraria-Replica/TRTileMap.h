#pragma once

#include "TRTile.h"

class TRTileMap
{
private:
	int tile_width;
	int tile_height;

	TRTile** tile_map;

public:
	TRTileMap(int width, int height);
	~TRTileMap();

	TRTile* GetTile(int x, int y) const;
	void SetTile(int x, int y, TRTile* new_tile);

private:
	TRTile** GetTileReference(int x, int y) const;
};