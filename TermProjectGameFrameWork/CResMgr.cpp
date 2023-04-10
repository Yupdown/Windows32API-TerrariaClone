#include "pch.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "CCore.h"
#include "CObject.h"
#include "CCamera.h"

CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
	DeleteDC(m_hBackDC);
	DeleteObject(m_hBackBit);
}

void CResMgr::init()
{
	std::filesystem::recursive_directory_iterator TexIter{ Mgr(CPathMgr)->GetTexPath() };
	for (const auto& Tex : TexIter)
	{
		if (Tex.is_regular_file())
		{
			LoadTexture(Tex.path().filename().wstring(), Tex.path().wstring());
		}
	}
	m_hBackDC = CreateCompatibleDC(Mgr(CCore)->GetMainDC());
	m_hBackBit = CreateCompatibleBitmap(Mgr(CCore)->GetMainDC(), Mgr(CCore)->GetResolution().x*2, Mgr(CCore)->GetResolution().y*2);
	DeleteObject(SelectObject(m_hBackDC, m_hBackBit));
	Clear();
	SetStretchBltMode(m_hBackDC, HALFTONE);
}

CTexture* CResMgr::LoadTexture(wstring_view _strKey, wstring_view _strFilePath)
{
	CTexture* pTex = FindTexture(_strKey).value_or(nullptr);
	if (pTex)	
	{
		return pTex;
	}
	pTex = new CTexture;
	pTex->Load(_strFilePath);
	pTex->SetKey(_strKey);	
	pTex->SetPath(_strFilePath);
	m_mapTex.insert(make_pair(_strKey, pTex));
	return pTex;
}

optional<CTexture*> CResMgr::FindTexture(wstring_view _strKey)const
{
	auto iter = m_mapTex.find(_strKey.data());
	if (iter == m_mapTex.end())
	{
		return std::nullopt;
	}
	return static_cast<CTexture*>(iter->second.get());
}

void CResMgr::DoStrechBlt(HDC _dc,wstring_view _wstrFileName, Vec2 _vLT, Vec2 _vScale, Vec2 _vBitPos, Vec2 _vSlice)const
{
	if (_vSlice.IsZero())
	{
		_vSlice.x = m_mapTex.find(_wstrFileName.data())->second->Width();
		_vSlice.y = m_mapTex.find(_wstrFileName.data())->second->Height();
	}

	POINT ptRes = Mgr(CCore)->GetResolution();

	_vScale.x = _vScale.x < ptRes.x ? _vScale.x : ptRes.x;
	_vScale.y = _vScale.y < ptRes.y ? _vScale.y : ptRes.y;

	StretchBlt(m_hBackDC
		,  static_cast<int>(_vLT.x)
		,  static_cast<int>(_vLT.y)
		,  static_cast<int>(_vScale.x)
		,  static_cast<int>(_vScale.y)
		, m_mapTex.find(_wstrFileName.data())->second->GetDC()
		,  static_cast<int>(_vBitPos.x)
		,  static_cast<int>(_vBitPos.y)
		,  static_cast<int>(_vSlice.x)
		,  static_cast<int>(_vSlice.y)
		, SRCCOPY);

	_vScale.x = min(_vScale.x, _vScale.x + _vLT.x);
	_vScale.y = min(_vScale.y, _vScale.y + _vLT.y);
	_vLT.x = max(_vLT.x, 0);
	_vLT.y = max(_vLT.y, 0);

	TransparentBlt(_dc
		, static_cast<int>(_vLT.x)
		, static_cast<int>(_vLT.y)
		, static_cast<int>(_vScale.x)
		, static_cast<int>(_vScale.y)
		, m_hBackDC
		, static_cast<int>(_vLT.x)
		, static_cast<int>(_vLT.y)
		, static_cast<int>(_vScale.x)
		, static_cast<int>(_vScale.y)
		, RGB(255, 0, 255));
}

void CResMgr::Clear()
{
	PatBlt(m_hBackDC, -Mgr(CCore)->GetResolution().x, -Mgr(CCore)->GetResolution().y, Mgr(CCore)->GetResolution().x*2 , Mgr(CCore)->GetResolution().y*2, WHITENESS);
}

CTexture* CResMgr::CreateTexture(wstring_view _strKey, UINT _iWidth, UINT _iHeight)
{
	CTexture* pTex = FindTexture(_strKey).value_or(nullptr);
	if (pTex)
	{
		return pTex;
	}
	pTex = new CTexture;
	pTex->Create(_iWidth, _iHeight); 
	m_mapTex.insert(make_pair(_strKey.data(), pTex));
	return pTex;
}

CTexture* CResMgr::GetTexture(wstring_view _strFileName)const
{
	return FindTexture(_strFileName).value_or(nullptr);
}

void CResMgr::renderTex(HDC _dc,const CTexture* const _pTex,const CObject* const _pObj, Vec2 _vBitPos, Vec2 _vSlice)const
{
	Vec2 vLtPos = {};
	vLtPos = _pObj->GetPos() - _pObj->GetScale() / 2.;
	if (_pObj->IsCamAffect())
	{
		vLtPos = Mgr(CCamera)->GetRenderPos(vLtPos);
	}
	DoStrechBlt(_dc, _pTex->GetKey(), vLtPos, _pObj->GetScale(),_vBitPos,_vSlice);
}

void CResMgr::renderTex(HDC _dc,const CTexture* const _pTex, Vec2 _vLT, Vec2 _vScale, Vec2 _vBitPos, Vec2 _vSlice)const
{
	DoStrechBlt(_dc, _pTex->GetKey(), _vLT, _vScale,_vBitPos,_vSlice);
}
