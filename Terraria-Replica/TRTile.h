#pragma once

#include <string>

class TRTile
{
private:
	bool solid;
	float hardness;
	
	std::string k_element;
	std::string k_dropitem;

public:
	TRTile(bool solid, float hardness, std::string k_element, std::string k_dropitem)
	{
		this->solid = solid;
		this->hardness = hardness;
		this->k_element = k_element;
		this->k_dropitem = k_dropitem;
	}
};

class TRTileAir : public TRTile
{
public:
	TRTileAir() : TRTile(false, 0.0f, "", "") {}
};

class TRTileSolid : public TRTile
{
public:
	TRTileSolid(float hardness, std::string k_element, std::string k_dropitem) : TRTile(true, hardness, k_element, k_dropitem) {}
};