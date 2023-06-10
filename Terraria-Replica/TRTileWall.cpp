#include "pch.h"
#include "TRWorld.h"
#include "TRTileWall.h"
#include "CAtlasMgr.h"
#include "Vec2Int.hpp"
#include "CustomMath.hpp"
#include <random>

TRTileWall::TRTileWall(std::wstring name, std::wstring k_element, std::wstring k_dropitem)
{
	this->name = name;
	this->k_element = k_element;
	this->k_dropitem = k_dropitem;

	if (k_element != L"")
		CreateAtlasElements();
}

std::wstring TRTileWall::DropItem() const
{
    return k_dropitem;
}

void TRTileWall::CreateAtlasElements()
{
    for (int i = 0; i < 13; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                for (int l = 0; l < 4; ++l)
                {
                    for (int m = k + 1; m <= 4; ++m)
                    {
                        for (int n = l + 1; n <= 4; ++n)
                        {
                            int w = m - k;
                            int h = n - l;
                            elements[i][j][k][l][w][h] = Mgr(CAtlasMgr)->GetAtlasElement(k_element, Vec2Int(i, j) * 18 + Vec2Int(k, l) * 4, Vec2Int(w, h) * 4);
                        }
                    }
                }
            }
        }
    }
}

void TRTileWall::OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask)
{
    OnDrawElement(tilemap_layer, x, y, bitmask, 0, 0, 4, 4);
}

void TRTileWall::OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask, int sx, int sy, int sw, int sh)
{
    if (k_element == L"")
        return;

    static std::default_random_engine dre;
    static std::uniform_int_distribution<int> uid;

    int si = 0;
    int sj = 0;
    int sr = uid(dre) % 3;

    int loword = bitmask & 15;

    switch (loword)
    {
    case 0b00000000:
        si = 3;
        sj = 9 + sr;
        break;
    case 0b00000001:
        si = 3;
        sj = 6 + sr;
        break;
    case 0b00000010:
        si = 0;
        sj = 6 + sr;
        break;
    case 0b00000011:
        si = sr;
        sj = 5;
        break;
    case 0b00000100:
        si = sr;
        sj = 12;
        break;
    case 0b00000101:
        si = 4;
        sj = 1 + sr * 2;
        break;
    case 0b00000110:
        si = 3;
        sj = 1 + sr * 2;
        break;
    case 0b00000111:
        si = sr;
        sj = 4;
        break;
    case 0b00001000:
        si = sr;
        sj = 9;
        break;
    case 0b00001001:
        si = 4;
        sj = sr * 2;
        break;
    case 0b00001010:
        si = 3;
        sj = sr * 2;
        break;
    case 0b00001011:
        si = sr;
        sj = 0;
        break;
    case 0b00001100:
        si = 4;
        sj = 6 + sr;
        break;
    case 0b00001101:
        si = 2;
        sj = 1 + sr;
        break;
    case 0b00001110:
        si = 0;
        sj = 1 + sr;
        break;
    default:
        si = 1;
        sj = 1 + sr;
        break;
    }

    Vec2Int offset = Vec2Int(min((sx - 1) * 8, 0), min((sy - 1) * 8, 0));
    tilemap_layer->pre_render(elements[sj][si][sx][sy][sw][sh], TRWorld::WorldToGlobal(Vec2Int(x, y + 1)) + offset, Vec2Int(sw, sh) * 8);
}