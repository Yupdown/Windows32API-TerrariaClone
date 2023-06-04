#include "pch.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "CCore.h"
#include "CObject.h"
#include "CCamera.h"
#include "CAtlasMgr.h"


CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
	DeleteDC(m_hBackDC);
	DeleteObject(m_hBackBit);

	DeleteDC(m_hBackEraseDC);
	DeleteObject(m_hBackEraseBit);
	
}

void CResMgr::init()
{
	std::filesystem::recursive_directory_iterator TexIter{ Mgr(CPathMgr)->GetTexPath() };
	
	for (const auto& Tex : TexIter)
	{
		if (Tex.is_regular_file())
		{
			CImage* pImg = new CImage;
			pImg->Load(Tex.path().wstring().data());
			pImg->SetTransparentColor(RGB(255, 0, 255));
			m_mapCImage.emplace(Tex.path().filename().wstring(), pImg);
			if (L"Atlas" == Tex.path().parent_path().filename())
			{
				Mgr(CAtlasMgr)->CreateAtlas(Tex.path().filename().wstring(), pImg);
			}
		}
	}
	m_hBackDC = CreateCompatibleDC(Mgr(CCore)->GetMainDC());
	m_hBackBit = CreateCompatibleBitmap(Mgr(CCore)->GetMainDC(), Mgr(CCore)->GetResolution().x*10, Mgr(CCore)->GetResolution().y*10);
	DeleteObject(SelectObject(m_hBackDC, m_hBackBit));
	Clear();
	SetStretchBltMode(m_hBackDC, HALFTONE);
	SetGraphicsMode(m_hBackDC, GM_ADVANCED);

	m_hBackEraseDC = CreateCompatibleDC(Mgr(CCore)->GetMainDC());
	m_hBackEraseBit = CreateCompatibleBitmap(Mgr(CCore)->GetMainDC(), Mgr(CCore)->GetResolution().x * 10, Mgr(CCore)->GetResolution().y * 10);
	DeleteObject(SelectObject(m_hBackEraseDC, m_hBackEraseBit));
	PatBlt(m_hBackEraseDC, 0, 0, Mgr(CCore)->GetResolution().x * 10, Mgr(CCore)->GetResolution().y * 10,WHITENESS);

	SetStretchBltMode(m_hBackEraseDC, HALFTONE);
	SetGraphicsMode(m_hBackEraseDC, GM_ADVANCED);

}

void CResMgr::Clear()
{
	//PatBlt(m_hBackDC, 0, 0, Mgr(CCore)->GetResolution().x*2 , Mgr(CCore)->GetResolution().y*10, WHITENESS);
	BitBlt(m_hBackDC
		, 0
		, 0
		, (int)Mgr(CCore)->GetResolution().x * 10
		, (int)Mgr(CCore)->GetResolution().y * 10
		, m_hBackEraseDC
		, 0
		, 0
		, SRCCOPY);
}

CImage* CResMgr::CreateImg(wstring_view _strKey, UINT _iWidth, UINT _iHeight)
{
	if (m_mapCImage.contains(_strKey.data()))
	{
		assert(0);
	}
	CImage* pImg = new CImage;
	pImg->Create(_iWidth, _iHeight, 24);
	m_mapCImage.emplace(_strKey, pImg);
	return pImg;
}

void CResMgr::renderImg(HDC _dc, const CImage* const _pImg, Vec2 _vLT, Vec2 _vScale, Vec2 _vBitPos, Vec2 _vSliceSize) const
{
	 Vec2 vLtPos = _vLT;
	 Vec2 vScale = _vScale;

	 // vLtPos = Mgr(CCamera)->GetRenderPos(vLtPos);

	 _pImg->StretchBlt(m_hBackDC
		 , (int)vLtPos.x
		 , (int)vLtPos.y
		 , (int)vScale.x
		 , (int)vScale.y
		 , (int)_vBitPos.x
		 , (int)_vBitPos.y
		 , (int)_vSliceSize.x
		 , (int)_vSliceSize.y
		 , SRCCOPY);

	vScale.x = min(vScale.x, vScale.x + vLtPos.x);
	vScale.y = min(vScale.y, vScale.y + vLtPos.y);
	vLtPos.x = max(vLtPos.x, 0);
	vLtPos.y = max(vLtPos.y, 0);

	TransparentBlt(_dc
		, (int)vLtPos.x
		, (int)vLtPos.y
		, (int)vScale.x
		, (int)vScale.y
		, m_hBackDC
		, (int)vLtPos.x
		, (int)vLtPos.y
		, (int)vScale.x
		, (int)vScale.y
		, RGB(255, 0, 255));

}

void CResMgr::renderImg(HDC _dc,const CImage* const _pImg, const CObject* const _pObj, Vec2 _vBitPos, Vec2 _vSlice, bool _bIsFlip)const
{
	static Vec2 vLtPos;
	static Vec2 vScale;

	if (_pObj->IsCamAffect())
	{
		std::tie(vLtPos, vScale) = Mgr(CCamera)->GetRenderPos(_pObj);
	}
	else
	{
		vLtPos = _pObj->GetPos() - _pObj->GetScale() / 2.;
		vScale = _pObj->GetScale();
	}

	const float i = _bIsFlip ? 1.f : 0.f;

	_pImg->StretchBlt(m_hBackDC
		, (int)vLtPos.x 
		, (int)vLtPos.y
		, (int)vScale.x
		, (int)vScale.y
		, (int)_vBitPos.x + (int)(i * (_vSlice.x - 1.f) )
		, (int)_vBitPos.y 
		, (int)_vSlice.x - (int)(i * _vSlice.x * 2.f)
		, (int)_vSlice.y
		, SRCCOPY);

	vScale.x = min(vScale.x, vScale.x + vLtPos.x);
	vScale.y = min(vScale.y, vScale.y + vLtPos.y);
	vLtPos.x = max(vLtPos.x, 0);
	vLtPos.y = max(vLtPos.y, 0);

	TransparentBlt(_dc
		, (int)vLtPos.x
		, (int)vLtPos.y
		, (int)vScale.x
		, (int)vScale.y
		, m_hBackDC
		, (int)vLtPos.x
		, (int)vLtPos.y
		, (int)vScale.x
		, (int)vScale.y
		, RGB(255, 0, 255));

	/*BitBlt(m_hBackDC
		, (int)vLtPos.x
		, (int)vLtPos.y
		, (int)vScale.x
		, (int)vScale.y
		, m_hBackEraseDC
		, (int)vLtPos.x
		, (int)vLtPos.y
		, SRCCOPY);*/

	//PatBlt(m_hBackDC,0,0, (int)vScale.x, (int)vScale.y, WHITENESS);
	//auto f = std::async(std::launch::async,PatBlt,m_hBackDC, (int)vLtPos.x, (int)vLtPos.y, (int)vScale.x, (int)vScale.y, WHITENESS);
	
}

void CResMgr::renderDC(HDC _dest, HDC _src, const CObject* const _pObj, Vec2 _vBitPos, Vec2 _vSlice)const
{
	static Vec2 vLtPos;
	static Vec2 vScale;

	if (_pObj->IsCamAffect())
	{
		std::tie(vLtPos, vScale) = Mgr(CCamera)->GetRenderPos(_pObj);
	}
	else
	{
		vLtPos = _pObj->GetPos() - _pObj->GetScale() / 2.;
		vScale = _pObj->GetScale();
	}

	/*StretchBlt(m_hBackDC
		, (int)vLtPos.x
		, (int)vLtPos.y
		, (int)vScale.x
		, (int)vScale.y
		,  _src
		, (int)_vBitPos.x
		, (int)_vBitPos.y
		, (int)_vSlice.x
		, (int)_vSlice.y
		, SRCCOPY);*/

	vScale.x = min(vScale.x, vScale.x + vLtPos.x);
	vScale.y = min(vScale.y, vScale.y + vLtPos.y);
	vLtPos.x = max(vLtPos.x, 0);
	vLtPos.y = max(vLtPos.y, 0);
	
	TransparentBlt(_dest
		, (int)vLtPos.x
		, (int)vLtPos.y
		, (int)vScale.x
		, (int)vScale.y
		, _src
		, (int)vLtPos.x
		, (int)vLtPos.y
		, (int)vScale.x
		, (int)vScale.y
		, RGB(255, 0, 255));

	
}

void CResMgr::renderPNG(HDC _dc, const CImage* const _pImg, Vec2 _vGlobalLTpos, bool _bIsFlip) const
{
	const float i = _bIsFlip ? 1.f : 0.f;
	Vec2 vScale = Vec2{(float)_pImg->GetWidth(),(float)_pImg->GetHeight() };

	_pImg->StretchBlt(m_hBackDC
		, (int)_vGlobalLTpos.x
		, (int)_vGlobalLTpos.y
		, (int)vScale.x
		, (int)vScale.y
		, (int)(i * (vScale.x - 1.f))
		,  0
		, (int)vScale.x - (int)(i * vScale.x * 2.f)
		, (int)vScale.y
		, SRCCOPY);

	vScale.x = min(vScale.x, vScale.x + _vGlobalLTpos.x);
	vScale.y = min(vScale.y, vScale.y + _vGlobalLTpos.y);
	_vGlobalLTpos.x = max(_vGlobalLTpos.x, 0);
	_vGlobalLTpos.y = max(_vGlobalLTpos.y, 0);

	TransparentBlt(_dc
		, (int)_vGlobalLTpos.x
		, (int)_vGlobalLTpos.y
		, (int)vScale.x
		, (int)vScale.y
		, m_hBackDC
		, (int)_vGlobalLTpos.x
		, (int)_vGlobalLTpos.y
		, (int)vScale.x
		, (int)vScale.y
		, RGB(255, 0, 255));
}
