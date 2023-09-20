#pragma once

#include <queue>
#include "Vec2Int.hpp"

class CTileLayer;
class CAtlasElement;
class TRTileMap;

class TRTileMapShade
{
private:
	int tile_width;
	int tile_height;

	int* light_level;
	int* light_level_last;

	CAtlasElement* elements[16];

	std::queue<Vec2Int> redraw_buffer;
	HBRUSH hTileMapBrush = CreateSolidBrush(0x00FF00FF);

public:
	TRTileMapShade(int width, int height);
	~TRTileMapShade();

	void BuildLightLevelMap(const TRTileMap& tile_map);
	void InvalidateLightLevelMap(const TRTileMap& tile_map, int x0, int y0, int x1, int y1, bool for_redraw = false);

	int GetLightLevel(int x, int y) const;
	void SetLightLevel(int x, int y, int v);
	void OnDrawElement(CTileLayer* tilemap_layer, int x, int y) const;
	void RedrawInvalidated(CTileLayer* tilemap_layer);
};