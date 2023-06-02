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

	CAtlasElement* elements[13][5][16];

public:
	TRTileWall(std::wstring name, std::wstring k_element, std::wstring k_dropitem);

	void CreateAtlasElements();
	virtual void OnDrawElement(CTileLayer* tilemap_layer, int x, int y, int bitmask, int clip = 0);
};

class TRTileWallAir : public TRTileWall
{
public:
	TRTileWallAir() : TRTileWall(L"Air Wall", L"", L"") {}
};
