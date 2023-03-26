#pragma once

class CTexture;
class CRes;
class CObject;

class CResMgr
	:public Singleton<CResMgr>
{
	friend class Singleton;
	CResMgr();
	~CResMgr();
private:
	unordered_map<wstring, unique_ptr<CTexture>>		m_mapTex;
	HDC	 m_hBackDC;
	HBITMAP m_hBackBit;
private:
	optional<CTexture*> FindTexture(wstring_view _strKey);
	void DoStrechBlt(HDC _dc,wstring_view _wstrFileName, Vec2 _vLT, Vec2 _vScale, Vec2 _vBitPos = {}, Vec2 _vSlice = {});
	CTexture* LoadTexture(wstring_view _strKey, wstring_view _strRelativePath);
public:
	void Clear();
	void init();
	CTexture* GetTexture(wstring_view _strFileName);
	CTexture* CreateTexture(wstring_view _strKey, UINT _iWidth, UINT _iHeight);
	void renderTex(HDC _dc, CTexture* const _pTex, CObject* const _pObj, Vec2 _vBitPos = {}, Vec2 _vSlice = {});
	void renderTex(HDC _dc, CTexture* const _pTex, Vec2 _vLT , Vec2 _vScale, Vec2 _vBitPos = {}, Vec2 _vSlice = {});
};

