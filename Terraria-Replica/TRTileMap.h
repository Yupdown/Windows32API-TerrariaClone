#pragma once

#include "TRTile.h"
#include "CScene.h"
#include "CTileLayer.h"

class TRTileMap
{
private:
	int tile_width;
	int tile_height;

	TRTile** tile_map;

	CTileLayer* renderer;

public:
	TRTileMap(int width, int height);
	~TRTileMap();

	TRTile* GetTile(int x, int y) const;
	TRTile* SetTile(int x, int y, TRTile* new_tile);

	void OnSceneCreate(CScene* scene);
	void UpdateTileRenderer(int x, int y);

private:
	TRTile** GetTileReference(int x, int y) const;
};