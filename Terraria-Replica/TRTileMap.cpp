#include "pch.h"
#include "TRTileMap.h"
#include "CTileLayer.h"
#include "CAtlasMgr.h"
#include "CAtlasElement.h"

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

void TRTileMap::OnSceneCreate(CScene* scene)
{
	CTileLayer* tilemap_layer = new CTileLayer(Vec2(500.0f, 7300.0f), tile_width * 8, tile_height * 8);

	for (int x = 0; x < tile_width; ++x)
	{
		for (int y = 0; y < tile_height; ++y)
		{
			TRTile* tile = GetTile(x, y);
			if (!tile->Solid())
				continue;

            int dir[][2] = { 0, -1, 0, 1, -1, 0, 1, 0 };
            int bitmask = 0;

            for (int k = 0; k < 4; ++k)
            {
                int xp = x + dir[k][0];
                int yp = y + dir[k][1];

                if (yp < 0 || yp >= tile_height || xp < 0 || xp >= tile_width)
                    bitmask |= 1 << k;
                else
                    bitmask |= (GetTile(xp, yp)->Solid()) << k;
            }

			tile->OnDrawElement(tilemap_layer, x, y, bitmask);
		}
	}
	scene->AddTileLayer(tilemap_layer);
}

TRTile** TRTileMap::GetTileReference(int x, int y) const
{
	if (x < 0 || x >= tile_width)
		return nullptr;
	if (y < 0 || y >= tile_height)
		return nullptr;
	return &tile_map[x % tile_width + y * tile_width];
}
