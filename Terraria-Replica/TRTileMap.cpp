#include "pch.h"
#include "TRTileMap.h"
#include "CLayer.h"
#include "CAtlasMgr.h"
#include "CAtlasElement.h"
#include <random>

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
	CLayer* tilemap_layer = CLayer::CreateTileLayer(L"Tilemap_Layer", Vec2{ tile_width * 8.0f, tile_height * 8.0f });
	HDC tilemap_dc = tilemap_layer->GetLayerDC();// tilemap_layer->GetLayerImg()->GetDC();
	CAtlasElement* elements[16][15];
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 15; ++j)
			elements[i][j] = Mgr(CAtlasMgr)->GetAtlasElement(L"Tiles_0008.png", { i * 9.0f, j * 9.0f }, { 8, 8 });
	}

    std::default_random_engine dre;
    std::uniform_int_distribution<int> uid;

	for (int x = 0; x < tile_width; ++x)
	{
		for (int y = 0; y < tile_height; ++y)
		{
			if (!GetTile(x, y)->Solid())
				continue;

            int si = 0;
            int sj = 0;
            int sr = uid(dre) % 3;

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

            switch (bitmask)
            {
            case 0:
                si = 3;
                sj = 9 + sr;
                break;
            case 1:
                si = 3;
                sj = 6 + sr;
                break;
            case 2:
                si = 0;
                sj = 6 + sr;
                break;
            case 3:
                si = sr;
                sj = 5;
                break;
            case 4:
                si = sr;
                sj = 12;
                break;
            case 5:
                si = 4;
                sj = 1 + sr * 2;
                break;
            case 6:
                si = 3;
                sj = 1 + sr * 2;
                break;
            case 7:
                si = sr;
                sj = 4;
                break;
            case 8:
                si = sr;
                sj = 9;
                break;
            case 9:
                si = 4;
                sj = sr * 2;
                break;
            case 10:
                si = 3;
                sj = sr * 2;
                break;
            case 11:
                si = sr;
                sj = 0;
                break;
            case 12:
                si = 4;
                sj = 6 + sr;
                break;
            case 13:
                si = 2;
                sj = 1 + sr;
                break;
            case 14:
                si = 0;
                sj = 1 + sr;
                break;
            default:
                si = 1;
                sj = 1 + sr;
                break;
            }

			elements[sj][si]->render(tilemap_dc, Vec2{x * 8.0f, y * 8.0f});
		}
	}
	//tilemap_layer->GetLayerImg()->ReleaseDC();
	scene->AddLayer(tilemap_layer);
}

TRTile** TRTileMap::GetTileReference(int x, int y) const
{
	if (x < 0 || x >= tile_width)
		return nullptr;
	if (y < 0 || y >= tile_height)
		return nullptr;
	return &tile_map[x % tile_width + y * tile_width];
}
