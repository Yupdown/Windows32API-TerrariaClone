#include "pch.h"
#include "CTexture.h"
#include "CCore.h"
#include "CResMgr.h"

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

void CTexture::Load(wstring_view _strFilePath)
{
	m_hBit = (HBITMAP)LoadImage(nullptr, _strFilePath.data(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());	
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);	
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
	//assert(m_hBit);
}

void CTexture::render(HDC _dc, const CObject* const _pObj, Vec2 _vBitPos , Vec2 _vSlice )const
{
	//Mgr(CResMgr)->renderTex(_dc, this, _pObj,_vBitPos,_vSlice);
}

void CTexture::render(HDC _dc, Vec2 _vLT, Vec2 _vScale, Vec2 _vBitPos , Vec2 _vSlice )const
{
	/*if (_vScale.IsZero())
	{
		_vScale.x = Width();
		_vScale.y = Height();
	}*/
//	Mgr(CResMgr)->renderTex(_dc, this, _vLT,_vScale,_vBitPos,_vSlice);
}



void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	HDC mainDC = CCore::GetInst()->GetMainDC();
	m_hBit = CreateCompatibleBitmap(mainDC, _iWidth, _iHeight);
	m_dc = CreateCompatibleDC(mainDC); 
	HBITMAP tempBit = static_cast<HBITMAP>(SelectObject(m_dc, m_hBit));
	DeleteObject(tempBit);
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}