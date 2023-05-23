#pragma once
#include "CObject.h"

class CTileLayer
	:public CObject
{
private:
	HDC m_hTileLayerDC = nullptr;
	HBITMAP m_hTileLayerBit = nullptr;

public:
	CTileLayer(Vec2 _vWorldPos,UINT _iWidth,UINT _iHeight);
	~CTileLayer();
	void pre_render(wstring_view _wstrFileName,Vec2 _vLayerLTPos,Vec2 _vBitPos, Vec2 _vSliceSize = TILE_PIXEL_XY);

	CTileLayer* Clone()const
	{
		auto pTileLayer = new CTileLayer{ *this };
		return pTileLayer;
	}
	void render(HDC _dc)const override;
};

