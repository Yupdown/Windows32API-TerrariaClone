#pragma once

#include <string>
#include <Windows.h>
#include "CAtlasElement.h"
#include "CTileLayer.h"

class TRTile
{
private:
	std::wstring name;

	bool solid;
	float hardness;
	
	std::wstring k_element;
	std::wstring k_dropitem;

	CAtlasElement* elements[16][16];

public:
    TRTile(std::wstring name, bool solid, float hardness, std::wstring k_element, std::wstring k_dropitem);

    void CreateAtlasElements();
	void OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask);

	bool Solid() const;
	float Hardness() const;
};

class TRTileAir : public TRTile
{
public:
	TRTileAir() : TRTile(L"Air", false, 0.0f, L"", L"") {}
};

class TRTileSolid : public TRTile
{
public:
	TRTileSolid(std::wstring name, float hardness, std::wstring k_element, std::wstring k_dropitem) : TRTile(name, true, hardness, k_element, k_dropitem) {}
};