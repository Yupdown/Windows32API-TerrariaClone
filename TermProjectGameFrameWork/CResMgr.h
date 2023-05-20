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
	unordered_map<wstring, unique_ptr<CImage>>		m_mapCImage;
	HDC	 m_hBackDC;
	HBITMAP m_hBackBit;

public:
	void Clear();
	void init();
	CImage* GetImg(wstring_view _strFileName)const
	{
		auto iter = m_mapCImage.find(_strFileName.data());
		if (m_mapCImage.end() == iter)
		{
			assert(0);
		}
		return iter->second.get();
	}
	CImage* CreateImg(wstring_view _strKey, UINT _iWidth, UINT _iHeight);
	void renderImg(const CImage* const _pImg, const CObject* const _pObj, Vec2 _vBitPos, Vec2 _vSlice)const;
	void renderImg(HDC _dc, const CImage* const _pImg, Vec2 _vLT, Vec2 _vScale, Vec2 _vBitPos, Vec2 _vSliceSize)const;
	HDC GetResMgrBackDC()const { return m_hBackDC; }
};

