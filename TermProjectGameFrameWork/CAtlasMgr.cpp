#include "pch.h"
#include "CAtlasMgr.h"
#include "CResMgr.h"
#include "CAtlas.h"
#include "CAtlasElement.h"

CAtlasMgr::CAtlasMgr()
{
}

CAtlasMgr::~CAtlasMgr()
{
}

void CAtlasMgr::init()
{

}

void CAtlasMgr::CreateAtlas(wstring_view _strName, CImage* const _pImg)
{
	m_mapAtlas.emplace(_strName, std::make_unique<CAtlas>(_pImg));
}

CAtlasElement* CAtlasMgr::GetAtlasElement(wstring_view _wstrAtlasName, Vec2 _vBitPos, Vec2 _vSlice) const
{
	return m_mapAtlas.find(_wstrAtlasName.data())->second->LoadAtlasElement(_vBitPos, _vSlice);
}
