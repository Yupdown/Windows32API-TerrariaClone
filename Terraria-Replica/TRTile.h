#pragma once

#include <string>
#include <Windows.h>
#include "CAtlasElement.h"
#include "CTileLayer.h"

class TRTile
{
protected:
	std::wstring name;

	bool solid;
	float hardness;

	int stick_group;
	bool stick_each;

	std::wstring k_element;
	std::wstring k_dropitem;

	CAtlasElement* elements[16][22];

public:
    TRTile(std::wstring name, bool solid, float hardness, std::wstring k_element, std::wstring k_dropitem);

    void CreateAtlasElements();
	virtual void OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask);

	int StickGroup() const;
	bool StickEach() const;
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
	TRTileSolid(std::wstring name, float hardness, std::wstring k_element, std::wstring k_dropitem);
};

class TRTileDirt : public TRTileSolid
{
public:
	TRTileDirt(std::wstring name, float hardness, std::wstring k_element, std::wstring k_dropitem);
};

class TRTileGrass : public TRTileSolid
{
public:
	TRTileGrass(std::wstring name, float hardness, std::wstring k_element, std::wstring k_dropitem);
	virtual void OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask) override;
};

class TRTileOre : public TRTileSolid
{
public:
	TRTileOre(std::wstring name, float hardness, std::wstring k_element, std::wstring k_dropitem);
};