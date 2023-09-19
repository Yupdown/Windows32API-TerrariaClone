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

	bool rocky;

	int light_level;

	std::wstring k_element;
	std::wstring k_dropitem;

	CAtlasElement* elements[16][22];

public:
	TRTile(std::wstring name, bool solid, float hardness, bool rocky, std::wstring k_element, std::wstring k_dropitem);
	virtual ~TRTile() = default;

    virtual void CreateAtlasElements();
	virtual void OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask);

	int StickGroup() const;
	bool StickEach() const;
	bool Solid() const;
	float Hardness() const;
	bool Rocky() const;
	int LightLevel() const;
	std::wstring DropItem() const;

	CAtlasElement* GetTileImg() const
	{
		static std::mt19937 rng{ std::random_device{}() };
		static std::uniform_int_distribution uid;
		int sr = uid(rng) % 3;
		return elements[1 + sr][1];
	}
};

class TRTileAir : public TRTile
{
public:
	TRTileAir() : TRTile(L"Air", false, 1.0f, false, L"", L"") {}
	virtual void OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask) override;
};

class TRTileSolid : public TRTile
{
public:
	TRTileSolid(std::wstring name, float hardness, bool rocky, std::wstring k_element, std::wstring k_dropitem);
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

class TRTileTorch : public TRTile
{
public:
	TRTileTorch(std::wstring name, std::wstring k_element, std::wstring k_dropitem);
	virtual void CreateAtlasElements() override;
	virtual void OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask) override;
};