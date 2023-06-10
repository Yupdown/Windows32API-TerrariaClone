#pragma once
#include "CObject.h"
#include "pch.h"
class CTileLayer;

class CMiniMap
	:public CObject
{
private:
	HDC m_hMinmapDC = nullptr;
	HBITMAP m_hMimimapBit = nullptr;

	HDC m_hMinmapDC2 = nullptr;
	HBITMAP m_hMimimapBit2 = nullptr;

	unique_ptr<CTileLayer> m_pMiniMapTileLayer = nullptr;
	const CImage* m_pMapFrameImg = nullptr;
	bool m_bMiniMapOn = false;
public:
	CMiniMap();
	~CMiniMap();

	CMiniMap* Clone()const override {
		return nullptr;
	}

	void update()override;
	void render(HDC _dc)const override;

	void SetMiniMap(bool _b) { m_bMiniMapOn = _b; }
	bool IsMiniMapOn()const { return m_bMiniMapOn; }

	CTileLayer* GetMiniMapTileLayer()const { return m_pMiniMapTileLayer.get(); }

	void CreateMiniMap();
};

