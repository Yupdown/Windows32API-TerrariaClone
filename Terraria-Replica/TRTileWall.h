#pragma once

#include <string>
#include <Windows.h>
#include "CAtlasElement.h"
#include "CTileLayer.h"

class TRTileWall
{
protected:
	std::wstring name;

	std::wstring k_element;
	std::wstring k_dropitem;

	CAtlasElement* elements[13][5][4][4][5][5];

public:
	TRTileWall(std::wstring name, std::wstring k_element, std::wstring k_dropitem);

	std::wstring DropItem() const;

	void CreateAtlasElements();
	virtual void OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask);
	void OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask, int sx, int sy, int sw, int sh);
};

class TRTileWallAir : public TRTileWall
{
public:
	TRTileWallAir() : TRTileWall(L"Air Wall", L"", L"") {}
};
