#include "pch.h"
#include "TRWorld.h"
#include "TRTile.h"
#include "define.h"
#include "CAtlasMgr.h"
#include "Vec2Int.hpp"
#include <random>

TRTile::TRTile(std::wstring name, bool solid, float hardness, bool rocky, std::wstring k_element, std::wstring k_dropitem)
{
	this->name = name;
	this->solid = solid;
	this->hardness = hardness;
    this->rocky = rocky;
	this->k_element = k_element;
	this->k_dropitem = k_dropitem;
    this->stick_group = 0;
    this->stick_each = true;

	if (k_element != L"")
		CreateAtlasElements();
}

void TRTile::CreateAtlasElements()
{
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 22; ++j)
			elements[i][j] = Mgr(CAtlasMgr)->GetAtlasElement(k_element, { i * 9.0f, j * 9.0f });
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

    int loword = bitmask & 15;
    int hiword = bitmask >> 8 & 15;

    switch (loword)
    {
    case 0b00000000:
        si = 3;
        sj = 9 + sr;
        break;
    case 0b00000001:
        switch (hiword)
        {
        case 0b00000001:
            si = 8 + sr;
            sj = 6;
            break;
        default:
            si = 3;
            sj = 6 + sr;
            break;
        }
        break;
    case 0b00000010:
        switch (hiword)
        {
        case 0b00000010:
            si = 5 + sr;
            sj = 6;
            break;
        default:
            si = 0;
            sj = 6 + sr;
            break;
        }
        break;
    case 0b00000011:
        switch (hiword)
        {
        case 0b00000001:
            si = 8 + sr;
            sj = 7;
            break;
        case 0b00000010:
            si = 5 + sr;
            sj = 7;
            break;
        case 0b00000011:
            si = 12 + sr;
            sj = 6;
            break;
        default:
            si = sr;
            sj = 5;
            break;
        }
        break;
    case 0b00000100:
        switch (hiword)
        {
        case 0b00000100:
            si = 13;
            sj = sr;
            break;
        default:
            si = sr;
            sj = 12;
            break;
        }
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
        switch (hiword)
        {
        case 0b00000001:
            si = 8 + sr;
            sj = 5;
            break;
        case 0b00000010:
            si = 5 + sr;
            sj = 5;
            break;
        case 0b00000100:
            si = 3;
            sj = 13 + sr;
            break;
        default:
            si = sr;
            sj = 4;
            break;
        }
        break;
    case 0b00001000:
        switch (hiword)
        {
        case 0b00000100:
            si = 13;
            sj = 3 + sr;
            break;
        default:
            si = sr;
            sj = 9;
            break;
        }
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
        switch (hiword)
        {
        case 0b00000001:
            si = 8 + sr;
            sj = 4;
            break;
        case 0b00000010:
            si = 5 + sr;
            sj = 4;
            break;
        case 0b00001000:
            si = 2;
            sj = 13 + sr;
            break;
        default:
            si = sr;
            sj = 0;
            break;
        }
        break;
    case 0b00001100:
        switch (hiword)
        {
        case 0b00000100:
            si = 14;
            sj = sr;
            break;
        case 0b00001000:
            si = 14;
            sj = 3 + sr;
            break;
        case 0b00001100:
            si = 11;
            sj = 9 + sr;
            break;
        default:
            si = 4;
            sj = 6 + sr;
            break;
        }
        break;
    case 0b00001101:
        switch (hiword)
        {
        case 0b00000001:
            si = 1;
            sj = 13 + sr;
            break;
        case 0b00000100:
            si = 12;
            sj = sr;
            break;
        case 0b00001000:
            si = 12;
            sj = 3 + sr;
            break;
        default:
            si = 2;
            sj = 1 + sr;
            break;
        }
        break;
    case 0b00001110:
        switch (hiword)
        {
        case 0b00000010:
            si = 0;
            sj = 13 + sr;
            break;
        case 0b00000100:
            si = 11;
            sj = sr;
            break;
        case 0b00001000:
            si = 11;
            sj = 3 + sr;
            break;
        default:
            si = 0;
            sj = 1 + sr;
            break;
        }
        break;
    default:
        switch (hiword)
        {
        case 0b00000000:
            si = 1;
            sj = 1 + sr;
            break;
        case 0b00000001:
            si = 6;
            sj = 8 + sr;
            break;
        case 0b00000010:
            si = 5;
            sj = 8 + sr;
            break;
        case 0b00000011:
            si = 10;
            sj = 8 + sr;
            break;
        case 0b00000100:
            si = 7 + sr;
            sj = 9;
            break;
        case 0b00000101:
            si = 5 + sr * 2;
            sj = 2;
            break;
        case 0b00000110:
            si = 6 + sr * 2;
            sj = 2;
            break;
        case 0b00000111:
            si = 5 + sr;
            sj = 12;
            break;
        case 0b00001000:
            si = 7 + sr;
            sj = 8;
            break;
        case 0b00001001:
            si = 5 + sr * 2;
            sj = 3;
            break;
        case 0b00001010:
            si = 6 + sr * 2;
            sj = 3;
            break;
        case 0b00001011:
            si = 8 + sr;
            sj = 12;
            break;
        case 0b00001100:
            si = 7 + sr;
            sj = 10;
            break;
        case 0b00001101:
            si = 5 + sr;
            sj = 11;
            break;
        case 0b00001110:
            si = 8 + sr;
            sj = 11;
            break;
        case 0b00001111:
            si = 11;
            sj = 6 + sr;
            break;
        }
        break;
    }

    // tilemap_layer->pre_render(elements[sj][si], TRWorld::WorldToGlobal(Vec2(x, y)), TILE_PIXEL_XY);

    HDC hdc = tilemap_layer->GetTileLayerDC();
    Vec2Int p = TRWorld::WorldToGlobal(Vec2Int(x, y + 1));
    elements[sj][si]->render(hdc, p, TILE_PIXEL_XY);
}

int TRTile::StickGroup() const
{
    return stick_group;
}

bool TRTile::StickEach() const
{
    return stick_each;
}

bool TRTile::Solid() const
{
    return solid;
}

float TRTile::Hardness() const
{
    return hardness;
}

bool TRTile::Rocky() const
{
    return rocky;
}

int TRTile::LightLevel() const
{
    return light_level;
}

std::wstring TRTile::DropItem() const
{
    return k_dropitem;
}

TRTileSolid::TRTileSolid(std::wstring name, float hardness, bool rocky, std::wstring k_element, std::wstring k_dropitem) : TRTile(name, true, hardness, rocky, k_element, k_dropitem)
{
    this->stick_group = 1;
}

TRTileOre::TRTileOre(std::wstring name, float hardness, std::wstring k_element, std::wstring k_dropitem) : TRTileSolid(name, hardness, true, k_element, k_dropitem)
{
    this->stick_group = 2;
    this->stick_each = false;
}

TRTileDirt::TRTileDirt(std::wstring name, float hardness, std::wstring k_element, std::wstring k_dropitem) : TRTileSolid(name, hardness, false, k_element, k_dropitem)
{
    this->stick_group = 3;
}

TRTileGrass::TRTileGrass(std::wstring name, float hardness, std::wstring k_element, std::wstring k_dropitem) : TRTileSolid(name, hardness, false, k_element, k_dropitem)
{
    this->stick_group = 1;
}

void TRTileGrass::OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask)
{
    if (k_element == L"")
        return;

    static std::default_random_engine dre;
    static std::uniform_int_distribution<int> uid;

    int si = 0;
    int sj = 0;
    int sr = uid(dre) % 3;

    int loword = bitmask & 15;
    int hiword = bitmask >> 4 & 15;

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
        if (hiword & 0b00000100)
        {
            si = 4;
            sj = 1 + sr * 2;
        }
        else
        {
            si = 14;
            sj = 9 + sr * 3;
        }
        break;
    case 0b00000110:
        if (hiword & 0b00000010)
        {
            si = 3;
            sj = 1 + sr * 2;
        }
        else
        {
            si = 12;
            sj = 9 + sr * 3;
        }
        break;
    case 0b00000111:
        switch (hiword & 0b00000110)
        {
        case 0b00000000:
            si = 13;
            sj = 9 + sr * 3;
            break;
        case 0b00000010:
            si = 21;
            sj = 3 + sr;
            break;
        case 0b00000100:
            si = 21;
            sj = sr;
            break;
        default:
            si = sr;
            sj = 4;
            break;
        }
        break;
    case 0b00001000:
        si = sr;
        sj = 9;
        break;
    case 0b00001001:
        if (hiword & 0b00000001)
        {
            si = 4;
            sj = sr * 2;
        }
        else
        {
            si = 14;
            sj = 7 + sr * 3;
        }
        break;
    case 0b00001010:
        if (hiword & 0b00001000)
        {
            si = 3;
            sj = sr * 2;
        }
        else
        {
            si = 12;
            sj = 7 + sr * 3;
        }
        break;
    case 0b00001011:
        switch (hiword & 0b00001001)
        {
        case 0b00000000:
            si = 13;
            sj = 7 + sr * 3;
            break;
        case 0b00000001:
            si = 20;
            sj = 3 + sr;
            break;
        case 0b00001000:
            si = 20;
            sj = sr;
            break;
        default:
            si = sr;
            sj = 0;
            break;
        }
        break;
    case 0b00001100:
        si = 4;
        sj = 6 + sr;
        break;
    case 0b00001101:
        switch (hiword & 0b00000101)
        {
        case 0b00000000:
            si = 14;
            sj = 8 + sr * 3;
            break;
        case 0b00000001:
            si = 19;
            sj = 3 + sr;
            break;
        case 0b00000100:
            si = 19;
            sj = sr;
            break;
        default:
            si = 2;
            sj = 1 + sr;
            break;
        }
        break;
    case 0b00001110:
        switch (hiword & 0b00001010)
        {
        case 0b00000000:
            si = 12;
            sj = 8 + sr * 3;
            break;
        case 0b00000010:
            si = 18;
            sj = sr;
            break;
        case 0b00001000:
            si = 18;
            sj = 3 + sr;
            break;
        default:
            si = 0;
            sj = 1 + sr;
            break;
        }
        break;
    default:
        switch (hiword)
        {
        case 0b00000000:
            si = 13;
            sj = 8 + sr * 3;
            break;
        case 0b00000001:
            si = 19;
            sj = 6 + sr;
            break;
        case 0b00000010:
            si = 20;
            sj = 6 + sr;
            break;
        case 0b00000100:
            si = 21;
            sj = 6 + sr;
            break;
        case 0b00001000:
            si = 18;
            sj = 6 + sr;
            break;
        case 0b00001110:
            si = 6 + sr * 2;
            sj = 2;
            break;
        case 0b00001101:
            si = 5 + sr * 2;
            sj = 3;
            break;
        case 0b00001011:
            si = 6 + sr * 2;
            sj = 3;
            break;
        case 0b00000111:
            si = 5 + sr * 2;
            sj = 2;
            break;
        case 0b00001010:
            si = 1;
            sj = 6 + sr;
            break;
        case 0b00000101:
            si = 2;
            sj = 6 + sr;
            break;
        case 0b00001001:
            si = sr;
            sj = 10;
            break;
        case 0b00000110:
            si = sr;
            sj = 11;
            break;
        case 0b00000011:
            si = 17;
            sj = 2 + sr;
            break;
        case 0b00001100:
            si = 17;
            sj = 5 + sr;
            break;
        default:
            si = 1;
            sj = 1 + sr;
            break;
        }
        break;
    }

    // tilemap_layer->pre_render(elements[sj][si], TRWorld::WorldToGlobal(Vec2(x, y)), TILE_PIXEL_XY);

    HDC hdc = tilemap_layer->GetTileLayerDC();
    Vec2Int p = TRWorld::WorldToGlobal(Vec2Int(x, y + 1));
    elements[sj][si]->render(hdc, p, TILE_PIXEL_XY);
}

void TRTileAir::OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask)
{

}

TRTileTorch::TRTileTorch(std::wstring name, std::wstring k_element, std::wstring k_dropitem) : TRTile(name, false, 1.0f, false, k_element, k_dropitem)
{
    light_level = 32;
}

void TRTileTorch::CreateAtlasElements()
{
    for (int i = 0; i < 3; ++i)
        elements[i][0] = Mgr(CAtlasMgr)->GetAtlasElement(k_element, { i * 9.0f, 0.0f });
}

void TRTileTorch::OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask)
{
    int si = 0;
    int sj = 0;

    int loword = bitmask & 15;
    if (loword & 0b00000100)
        sj = 1;
    else if (loword & 0b00001000)
        sj = 2;

    HDC hdc = tilemap_layer->GetTileLayerDC();
    Vec2Int p = TRWorld::WorldToGlobal(Vec2Int(x, y + 1));
    elements[sj][si]->render(hdc, p, TILE_PIXEL_XY);
}
