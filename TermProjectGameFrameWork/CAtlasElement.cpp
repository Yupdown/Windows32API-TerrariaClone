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

void CAtlasElement::render(HDC _dc,Vec2 _v) const
{
	auto vPos = Mgr(CCamera)->GetRenderPos(_v);
	Mgr(CResMgr)->renderImg(_dc, m_pAtlasImg, vPos, Vec2{ 16.0f, 16.0f }, m_vBitPos, m_vSliceSize);
}
