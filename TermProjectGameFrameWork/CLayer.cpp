#include "pch.h"
#include "CLayer.h"
#include "CResMgr.h"
#include "CCamera.h"
#include "CCore.h"

extern bool g_bStopToken;

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
	DeleteDCBITMAP(m_hLayerDC, m_hLayerBit);
}

CLayer* CLayer::CreateLayer(wstring_view _wstrFileName,Vec2 _vLayerStartPos, Vec2 _vLayerSlice,Vec2 _vLayerScale, UINT _iLayerCount, float _fSpeed)
{
	const Vec2 vRes = Mgr(CCore)->GetResolutionV();
	auto pLayer = new CLayer;
	pLayer->m_pLayerImg = Mgr(CResMgr)->GetImg(_wstrFileName);
	pLayer->m_vLayerScale = _vLayerScale;
	pLayer->m_fLayerSpeed = _fSpeed;

	while (!CreateDCBITMAP(pLayer->m_hLayerDC, pLayer->m_hLayerBit, Vec2{ _vLayerScale.x * (float)_iLayerCount,_vLayerScale.y }) && !g_bStopToken) {
		DeleteDCBITMAP(pLayer->m_hLayerDC, pLayer->m_hLayerBit);
	}

	if (g_bStopToken)
	{
		return nullptr;
	}

	for (int i = 0; i < (int)_iLayerCount; ++i)
	{
		pLayer->m_pLayerImg->StretchBlt(pLayer->m_hLayerDC
			, (int)_vLayerStartPos.x + (int)((float)i * _vLayerSlice.x)
			, (int)_vLayerStartPos.y
			, (int)_vLayerSlice.x
			, (int)_vLayerSlice.y);
	}
	return pLayer;
}

void CLayer::render(HDC _dc)
{
	Mgr(CCamera)->renderBackGround(_dc, m_hLayerDC, m_vLayerScale, m_fLayerSpeed);
}
