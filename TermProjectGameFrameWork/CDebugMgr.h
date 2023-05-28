#pragma once
#include "Singleton.hpp"

class CDebugMgr
	:public Singleton<CDebugMgr>
{
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

public:
	void init();
	void update();
	void render();
	void progress();
};

