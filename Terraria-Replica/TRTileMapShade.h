#pragma once

class CTileLayer;
class CAtlasElement;
class TRTileMap;

class TRTileMapShade
{
private:
	int tile_width;
	int tile_height;

	int* light_level;

	CAtlasElement* elements[16];

public:
	TRTileMapShade(int width, int height);
	~TRTileMapShade();

	void BuildLightLevelMap(const TRTileMap& tile_map);

	int GetLightLevel(int x, int y) const;
	void SetLightLevel(int x, int y, int v);
	void OnDrawElement(CTileLayer* tilemap_layer, int x, int y) const;
};