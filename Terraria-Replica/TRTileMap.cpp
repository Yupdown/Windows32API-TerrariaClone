#include "pch.h"
#include "TRTile.h"
#include "TRTileMap.h"
#include "TRTileManager.h"
#include "CTileLayer.h"
#include "CAtlasMgr.h"
#include "CAtlasElement.h"

TRTileMap::TRTileMap(int width, int height)
{
	tile_width = width;
	tile_height = height;

	tile_map = new TRTile*[width * height];
	renderer = nullptr;
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

TRTile* TRTileMap::SetTile(int x, int y, TRTile* new_tile)
{
	TRTile** ref = GetTileReference(x, y);
	if (ref == nullptr)
		return nullptr;

	TRTile* old_tile = *ref;
	*ref = new_tile;

	if (new_tile != old_tile)
		;

	return old_tile;
}

void TRTileMap::OnSceneCreate(CScene* scene)
{
	renderer = new CTileLayer(Vec2(500.0f, 7300.0f), tile_width * 8, tile_height * 8);

	TRTile* tile_dirt = Mgr(TRTileManager)->GetTileByKey("dirt");
	TRTile* tile_grass = Mgr(TRTileManager)->GetTileByKey("dirtgrass");

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

			tile->OnDrawElement(renderer, x, y, bitmask);
		}
	}
	scene->AddTileLayer(renderer);
}

void TRTileMap::UpdateTileRenderer(int x, int y)
{

}

TRTile** TRTileMap::GetTileReference(int x, int y) const
{
	if (x < 0 || x >= tile_width)
		return nullptr;
	if (y < 0 || y >= tile_height)
		return nullptr;
	return &tile_map[x % tile_width + y * tile_width];
}
