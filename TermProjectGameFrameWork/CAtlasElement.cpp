#include "pch.h"
#include "CAtlasElement.h"
#include "CResMgr.h"
#include "CCamera.h"

CAtlasElement::CAtlasElement(const CImage* const _pImg, Vec2 _vBit, Vec2 _vSlice)
	:m_pAtlasImg{_pImg}
	,m_vBitPos{_vBit}
	,m_vSliceSize{_vSlice}
{
}

CAtlasElement::~CAtlasElement()
{
}

void CAtlasElement::render(HDC _dc, Vec2 _vLT, Vec2 _vScale) const
{
	Mgr(CResMgr)->renderImg(_dc,m_pAtlasImg, _vLT, _vScale, m_vBitPos, m_vSliceSize);
}
