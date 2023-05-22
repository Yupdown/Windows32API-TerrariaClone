#pragma once

#include "TRTile.h"
#include "CScene.h"

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
	void OnSceneCreate(CScene* scene);

private:
	TRTile** GetTileReference(int x, int y) const;
};