#pragma once

class CAtlasElement;

union ATLAS_ID
{
	struct {
		short BitLeft;
		short BitTop;
		short Width;
		short Height;
	};
	ULONGLONG ID;
};

class CAtlas
{
private:
	const CImage* m_pAtlasImg = nullptr;
	unordered_map<ULONGLONG, unique_ptr<CAtlasElement>> m_mapElement;
public:
	CAtlas(CImage* const _pImg);
	~CAtlas();
	CAtlasElement* LoadAtlasElement(Vec2 _vBitPos, Vec2 _vSliceSize);
	
};

