#pragma once
#include "Singleton.hpp"

class CTexture; 

class CCore
	:public Singleton<CCore>
{
	friend class Singleton;
	CCore();
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
	constexpr HBRUSH	GetBrush(BRUSH_TYPE _eType)const { return m_arrBrush[(UINT)_eType]; }
	constexpr HPEN	GetPen(PEN_TYPE _eType)const { return m_arrPen[(UINT)_eType]; }
public:
	void DockMenu();
	void DividMenu(); 
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu); 
public:
	int init(HWND _hwnd, POINT _ptResolution);
	void progress();
	void Clear();
public:
	constexpr HWND GetMainHwnd()const { return m_hWnd; }
	constexpr POINT GetResolution()const { return m_ptResolution; }
	constexpr Vec2 GetResolutionV()const { return Vec2{ m_ptResolution }; }
	constexpr HDC GetMainDC()const { return m_hDC; }
	constexpr HMENU GetMenu()const { return m_hMenu; }
	HBRUSH ChangeBrush(BRUSH_TYPE _eType);
	HPEN ChangePen(PEN_TYPE _eType);
	HDC GetMemDC()const;
};
