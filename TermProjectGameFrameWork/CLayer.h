#pragma once

class CLayer
{
private:
	const CImage* m_pLayerImg = nullptr;
	
	HDC m_hLayerDC = nullptr;
	HBITMAP m_hLayerBit = nullptr;
	Vec2 m_vLayerScale = {};
	float m_fLayerSpeed = 1.f;

public:
	CLayer();
	~CLayer();
	static CLayer* CreateLayer(wstring_view _wstrFileName,Vec2 _vLayerStartPos,Vec2 _vLayerSlice ,Vec2 _vLayerScale,UINT _iLayerCount,float _fSpeed = 1.f);
	void render(HDC _dc);
	const CImage* GetLayerImg()const { return m_pLayerImg; }
	HDC GetLayerDC()const { return m_hLayerDC; }
};

