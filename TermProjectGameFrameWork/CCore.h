#pragma once
#include "Singleton.hpp"

class CTexture; 

class CCore
	:public Singleton<CCore>
{
	friend class Singleton;
	CCore();
public:
	~CCore();
private:
	HWND m_hWnd{};			
	POINT m_ptResolution{};
	HDC m_hDC{};			

	CTexture* m_pMemTex = {}; 
	
	HBRUSH			m_arrBrush[(UINT)BRUSH_TYPE::END] = {};
	HPEN			m_arrPen[(UINT)PEN_TYPE::END] = {};
	
	HMENU			m_hMenu; 
public:
	void CreateBrushPen(); 
	HBRUSH	GetBrush(BRUSH_TYPE _eType)const { return m_arrBrush[(UINT)_eType]; }
	HPEN	GetPen(PEN_TYPE _eType)const { return m_arrPen[(UINT)_eType]; }
public:
	void DockMenu();
	void DividMenu(); 
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu); 
public:
	int init(HWND _hwnd, POINT _ptResolution);
	void progress();
	void Clear();
public:
	HWND GetMainHwnd() { return m_hWnd; }	
	POINT GetResolution()const { return m_ptResolution; }	
	HDC GetMainDC()const { return m_hDC; }	
	HMENU GetMenu()const { return m_hMenu; } 
	HBRUSH ChangeBrush(BRUSH_TYPE _eType);
	HPEN ChangePen(PEN_TYPE _eType);
};
