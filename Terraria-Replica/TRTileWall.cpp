#include "pch.h"
#include "TRWorld.h"
#include "TRTileWall.h"
#include "CAtlasMgr.h"
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

void TRTileWall::CreateAtlasElements()
{
    for (int i = 0; i < 13; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            for (int k = 0; k < 16; ++k)
            {
                int lo = k & 0b0011;
                int hi = (k >> 2) & 0b0011;
                
                int x = i * 18;
                int y = j * 18;
                int w = 0;
                int h = 0;

                switch (lo)
                {
                case 0:
                    w = 16;
                    break;
                case 1:
                    x += 4;
                    w = 12;
                    break;
                case 2:
                    w = 12;
                    break;
                case 3:
                    x += 4;
                    w = 8;
                    break;
                }
                switch (hi)
                {
                case 0:
                    h = 16;
                    break;
                case 1:
                    y += 4;
                    h = 12;
                    break;
                case 2:
                    h = 12;
                    break;
                case 3:
                    y += 4;
                    h = 8;
                    break;
                }

                elements[i][j][k] = Mgr(CAtlasMgr)->GetAtlasElement(k_element, Vec2(x, y), Vec2(w, h));
            }
        }
    }
}

void TRTileWall::OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask, int clip)
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

    int lo = clip & 0b0011;
    int hi = (clip >> 2) & 0b0011;

    int sx = 0;
    int sy = 0;
    int w = 0;
    int h = 0;

    if (!(lo & 1))
        sx = -8;
    if (!(hi & 1))
        sy = -8;

    switch (lo)
    {
    case 0:
        w = 32;
        break;
    case 1:
    case 2:
        w = 24;
        break;
    case 3:
        w = 16;
        break;
    }

    switch (hi)
    {
    case 0:
        h = 32;
        break;
    case 1:
    case 2:
        h = 24;
        break;
    case 3:
        h = 16;
        break;
    }

    tilemap_layer->pre_render(elements[sj][si][clip], TRWorld::WorldToGlobal(Vec2(x, y + 1)) + Vec2(sx, sy), Vec2(w, h));
}

