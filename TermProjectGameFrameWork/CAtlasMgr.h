#pragma once
#include "Singleton.hpp"

class CAtlas;
class CAtlasElement;

class CAtlasMgr
	:public Singleton<CAtlasMgr>
{
	friend class Singleton;
	CAtlasMgr();
	~CAtlasMgr();
private:
	unordered_map<wstring,unique_ptr<CAtlas>> m_mapAtlas;
public:
	void init();
	void CreateAtlas(wstring_view _strName, CImage* const _pImg);
	CAtlasElement* GetAtlasElement(wstring_view _wstrAtlasName, Vec2 _vBitPos, Vec2 _vSlice = SRC_TILE_SIZE_XY)const;
};

