#pragma once

class CAtlasElement;

union ATLAS_ID
{
	struct {
		float Left_ID;
		float Right_ID;
	};
	double ID;
};

class CAtlas
{
private:
	const CImage* m_pAtlasImg = nullptr;
	unordered_map<double, unique_ptr<CAtlasElement>> m_mapElement;
public:
	CAtlas(CImage* const _pImg);
	~CAtlas();
	CAtlasElement* LoadAtlasElement(Vec2 _vBitPos, Vec2 _vSliceSize);
};

