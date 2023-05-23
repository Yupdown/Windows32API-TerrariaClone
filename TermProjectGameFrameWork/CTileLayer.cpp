#include "pch.h"
#include "CTileLayer.h"
#include "CResMgr.h"
#include "CAtlasMgr.h"
#include "CAtlasElement.h"
#include "CCamera.h"

CTileLayer::CTileLayer(Vec2 _vWorldPos ,UINT _iWidth, UINT _iHeight)
{
	SetScale(Vec2{ (float)_iWidth,(float)_iHeight});
	SetPos(_vWorldPos);
	CreateDCBITMAP(m_hTileLayerDC, m_hTileLayerBit, GetScale());
}

CTileLayer::~CTileLayer()
{
}

void CTileLayer::pre_render(wstring_view _wstrFileName, Vec2 _vLayerLTPos, Vec2 _vBitPos, Vec2 _vSliceSize)
{
	auto pAtlasEle = Mgr(CAtlasMgr)->GetAtlasElement(_wstrFileName, _vBitPos, _vSliceSize);
	pAtlasEle->render(m_hTileLayerDC, _vLayerLTPos);
}

void CTileLayer::render(HDC _dc)const
{
	const auto vOriginScale = GetScale();
	const Vec2 vLTpos = Mgr(CCamera)->GetRenderPos(GetPos() - vOriginScale / 2);
	TransparentBltSafe(_dc
		, (int)vLTpos.x
		, (int)vLTpos.y
		, (int)vOriginScale.x
		, (int)vOriginScale.y
		, m_hTileLayerDC
		, 0
		, 0
		, (int)vOriginScale.x
		, (int)vOriginScale.y
		, (int)vOriginScale.x
		, (int)vOriginScale.y
		, RGB(255, 0, 255));
}


