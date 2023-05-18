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

	HDC m_hMemDC = {};
	HBITMAP m_hBackBuffer = {};
	
	HBRUSH			m_arrBrush[(UINT)BRUSH_TYPE::END] = {};
	HPEN			m_arrPen[(UINT)PEN_TYPE::END] = {};
	
	HMENU			m_hMenu; 
	XFORM m_xform = {};
	XFORM m_xform2 = {};
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
	HDC GetMemDC()const { return m_hMemDC; }
	void RotateTransform(int _iDegree, Vec2 centerPt = {});
	void ResetTransform();
	void ScaleTransform(float _fScale);
	void TranslateTransform(Vec2 vDist);
	void YFlipTransform(Vec2 vCenter = {});
	void XFlipTransform(Vec2 vCenter = {});

	void RotateTransform(HDC dc, int _iDegree, Vec2 centerPt = {});
	void ResetTransform(HDC dc);
	void YFlipTransform(HDC dc, Vec2 vCenter = {});
	void XFlipTransform(HDC dc, Vec2 vCenter = {});
	void ScaleTransform(HDC dc, float _fScale);
	void TranslateTransform(HDC dc, Vec2 vDist);
};
