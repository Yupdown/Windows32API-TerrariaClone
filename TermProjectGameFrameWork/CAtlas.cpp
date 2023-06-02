#include "pch.h"
#include "CAtlas.h"
#include "CAtlasElement.h"

CAtlas::CAtlas(CImage* const _pImg)
	:m_pAtlasImg{_pImg}
{
}

CAtlas::~CAtlas()
{
}

CAtlasElement* CAtlas::LoadAtlasElement(Vec2 _vBitPos,Vec2 _vSliceSize)
{
	ATLAS_ID ID = {};
	//ID.Left_ID = _vBitPos.x;
	//ID.Right_ID = _vBitPos.y;
	ID.BitLeft = (short)_vBitPos.x;
	ID.BitTop = (short)_vBitPos.y;
	ID.Width = (short)_vSliceSize.x;
	ID.Height = (short)_vSliceSize.y;

	auto iter = m_mapElement.find(ID.ID);
	if (m_mapElement.end() == iter)
	{
		auto pAtlasElement = new CAtlasElement{ m_pAtlasImg,_vBitPos,_vSliceSize };
		return m_mapElement.emplace(std::make_pair(ID.ID,pAtlasElement)).first->second.get();
	}
	else
	{
		return iter->second.get();
	}
}
