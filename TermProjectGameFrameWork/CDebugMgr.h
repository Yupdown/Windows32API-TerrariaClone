#pragma once
#include "Singleton.hpp"

class CTileLayer;

class CDebugMgr
	:public Singleton<CDebugMgr>
{
	friend class CCore;
	friend void miniMapWin();
	friend class Singleton;
	CDebugMgr();
	~CDebugMgr();
private:
	HWND m_hWnd;

	HDC m_hDC;
	

	HDC m_hMemDC;
	HBITMAP m_hMemBit;

	HDC m_hMemDC2;
	HBITMAP m_hMemBit2;

	const CImage* m_pMinimapImg = nullptr;

	unique_ptr<CTileLayer> m_pTileLayer = nullptr;

public:
	void init();
	void update();
	void render();
	void progress();

	CTileLayer* GetMiniMapTileLayer()const { return m_pTileLayer.get(); }
};

