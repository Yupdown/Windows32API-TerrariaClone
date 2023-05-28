#pragma once

class CAtlasElement
{
private:
	const CImage* m_pAtlasImg = nullptr;
	Vec2 m_vBitPos = {};
	Vec2 m_vSliceSize = {};
public:
	CAtlasElement(const CImage* const _pImg,Vec2 _vBit,Vec2 _vSlice);
	~CAtlasElement();
	void render(HDC _dc, Vec2 _vLT, Vec2 _vScale)const;
};

