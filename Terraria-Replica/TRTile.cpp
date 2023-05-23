#include "pch.h"
#include "TRTile.h"
#include "define.h"
#include "CAtlasMgr.h"
#include <random>

TRTile::TRTile(std::wstring name, bool solid, float hardness, std::wstring k_element, std::wstring k_dropitem)
{
	this->name = name;
	this->solid = solid;
	this->hardness = hardness;
	this->k_element = k_element;
	this->k_dropitem = k_dropitem;

	if (k_element != L"")
		CreateAtlasElements();
}

void TRTile::CreateAtlasElements()
{
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 15; ++j)
			elements[i][j] = Mgr(CAtlasMgr)->GetAtlasElement(k_element, { i * 9.0f, j * 9.0f }, { 8, 8 });
	}
}

void TRTile::OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask)
{
    if (k_element == L"")
        return;

    static std::default_random_engine dre;
    static std::uniform_int_distribution<int> uid;

    int si = 0;
    int sj = 0;
    int sr = uid(dre) % 3;

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

    tilemap_layer->pre_render(elements[sj][si], Vec2{ x * 8.0f, y * 8.0f });
}

bool TRTile::Solid() const
{
    return solid;
}

float TRTile::Hardness() const
{
    return hardness;
}