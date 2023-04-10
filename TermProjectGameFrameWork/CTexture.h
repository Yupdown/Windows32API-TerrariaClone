#pragma once
#include "CRes.h"

class CObject;

class CTexture
	:public CRes
{
private:
	friend class CResMgr;
	CTexture();
	void Create(UINT _iWidth, UINT _iHeight);
private:
	HDC			m_dc = {}; 
	HBITMAP		m_hBit = {};	
	BITMAP		m_bitInfo = {};
public:
	~CTexture();
public:
	void Load(wstring_view _strFilePath); 
	UINT	Width()const { return m_bitInfo.bmWidth; }	
	UINT	Height()const { return m_bitInfo.bmHeight; } 
	HDC		GetDC()const { return m_dc; }
	void render(HDC _dc,const CObject* const _pObj, Vec2 _vBitPos = {}, Vec2 _vSlice = {})const;
	void render(HDC _dc, Vec2 _vLT = {}, Vec2 _vScale = {}, Vec2 _vBitPos = {}, Vec2 _vSlice = {})const;
};

