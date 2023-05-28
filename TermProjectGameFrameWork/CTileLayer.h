#pragma once
#include "CObject.h"

class CAtlasElement;

class CTileLayer
	:public CObject
{
private:

	Vec2 m_vTileLayerScale = {};
	HDC m_hTileLayerDC = nullptr;
	HBITMAP m_hTileLayerBit = nullptr;
	
	

public:
	CTileLayer(Vec2 _vWorldPos,UINT _iWidth,UINT _iHeight);
	~CTileLayer();
	void pre_render(wstring_view _wstrFileName,Vec2 _vLayerLTPos, Vec2 _vScale, Vec2 _vBitPos, Vec2 _vSliceSize = TILE_PIXEL_XY);
	void pre_render(CAtlasElement* const _pElement, Vec2 _vLayerLTPos, Vec2 _vScale);
	
	CTileLayer* Clone()const
	{
		auto pTileLayer = new CTileLayer{ *this };
		return nullptr;
	}

	void render(HDC _dc)const override;
	HDC GetTileLayerDC()const { return m_hTileLayerDC; }
};