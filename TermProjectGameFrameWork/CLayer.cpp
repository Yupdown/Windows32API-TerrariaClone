#include "pch.h"
#include "CLayer.h"
#include "CResMgr.h"
#include "CCamera.h"
#include "CCore.h"
#include "CObject.h"
CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CLayer* CLayer::CreateLayer(wstring_view _wstrFileName,Vec2 _vLayerStartPos, Vec2 _vLayerSlice,Vec2 _vLayerScale, UINT _iLayerCount, float _fSpeed)
{
	const Vec2 vRes = Mgr(CCore)->GetResolutionV();
	auto pLayer = new CLayer;
	pLayer->m_pLayerImg = Mgr(CResMgr)->GetImg(_wstrFileName);
	pLayer->m_vLayerScale = _vLayerScale;
	pLayer->m_fLayerSpeed = _fSpeed;
	CreateDCBITMAP(pLayer->m_hLayerDC, pLayer->m_hLayerBit, Vec2{ _vLayerScale.x * (float)_iLayerCount,_vLayerScale.y });

	for (int i = 0; i < (int)_iLayerCount; ++i)
	{
		pLayer->m_pLayerImg->StretchBlt(pLayer->m_hLayerDC
			, (int)_vLayerStartPos.x + (int)((float)i * vRes.x)
			, (int)_vLayerStartPos.y
			, (int)_vLayerSlice.x
			, (int)_vLayerSlice.y);
	}
	if (1 != _iLayerCount)
	{
		pLayer->m_bIsLoop = true;
	}
	else
	{
		pLayer->m_bIsLoop = false;
	}
	return pLayer;
}

CLayer* CLayer::CreateTileLayer(wstring_view _wstrFileName, Vec2 _vLayerScale)
{
	auto pTileLayer = new CLayer;
	pTileLayer->m_vLayerScale = _vLayerScale;
	pTileLayer->m_pLayerImg = Mgr(CResMgr)->CreateImg(_wstrFileName, (int)_vLayerScale.x, (int)_vLayerScale.y);
	CreateDCBITMAP(pTileLayer->m_hLayerDC, pTileLayer->m_hLayerBit, Vec2{ _vLayerScale.x ,_vLayerScale.y });
	auto hDC = pTileLayer->m_pLayerImg->GetDC();

	BitBlt(hDC
		, 0
		, 0
		, (int)_vLayerScale.x
		, (int)_vLayerScale.y
		, pTileLayer->m_hLayerDC
		, 0
		, 0
		, SRCCOPY);
	pTileLayer->m_pLayerImg->ReleaseDC();

	return pTileLayer;
}


void CLayer::render(HDC _dc)
{
	if (m_bIsLoop)
	{
		Mgr(CCamera)->renderBackGround(_dc, m_hLayerDC, m_vLayerScale, m_fLayerSpeed);
	}
	else
	{
		//Mgr(CResMgr)->renderImg(_dc, m_pLayerImg, Vec2{ 0,Mgr(CCore)->GetResolutionV().y * 9 },m_vLayerScale ,Vec2{ 0,0 }, m_vLayerScale);
		Vec2 vLtPos = Vec2{ 0,Mgr(CCore)->GetResolutionV().y * 9 };
		Vec2 vScale = m_vLayerScale;
		//Vec2 vLtPos = Mgr(CCamera)->GetRenderPos(Vec2{});
		vLtPos = Mgr(CCamera)->GetRenderPos(vLtPos);
		//vScale.x = min(vScale.x, vScale.x + vLtPos.x);
		//vScale.y = min(vScale.y, vScale.y + vLtPos.y);
		//vLtPos.x = max(vLtPos.x, 0);
		vLtPos.y = max(vLtPos.y, 0);
	//	auto hDC = m_pLayerImg->GetDC();
		Vec2 vRes = Mgr(CCore)->GetResolutionV();
		TransparentBlt(_dc
			, vLtPos.x
			, vLtPos.y
			, (int)vScale.x
			, (int)vScale.y
			, m_hLayerDC
			, 0
			, 0
			, (int)vScale.x
			, (int)vScale.y
			, RGB(255, 0, 255));

		//m_pLayerImg->ReleaseDC();
	}
}
