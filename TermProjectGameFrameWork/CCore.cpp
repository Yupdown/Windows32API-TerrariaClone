#include "pch.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CCamera.h"
#include "resource.h"
#include "CEventMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CAtlasMgr.h"
#include "CThreadMgr.h"
#include "CDebugMgr.h"

CCore::CCore()
{
	m_xform.eM11 = (FLOAT)1.0;
	m_xform.eM12 = (FLOAT)0.0;
	m_xform.eM21 = (FLOAT)0.0;
	m_xform.eM22 = (FLOAT)1.0;
	m_xform.eDx = (FLOAT)0.0;
	m_xform.eDy = (FLOAT)0.0;

	m_xform2.eM11 = (FLOAT)1.0;
	m_xform2.eM12 = (FLOAT)0.0;
	m_xform2.eM21 = (FLOAT)0.0;
	m_xform2.eM22 = (FLOAT)1.0;
	m_xform2.eDx = (FLOAT)0.0;
	m_xform2.eDy = (FLOAT)0.0;

	m_xform3.eM11 = (FLOAT)1.0;
	m_xform3.eM12 = (FLOAT)0.0;
	m_xform3.eM21 = (FLOAT)0.0;
	m_xform3.eM22 = (FLOAT)1.0;
	m_xform3.eDx = (FLOAT)0.0;
	m_xform3.eDy = (FLOAT)0.0;
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);	
	for (auto& pen : m_arrPen)
	{
		DeleteObject(pen);
	}
	for (int i = 3; i < etoi(BRUSH_TYPE::END); ++i)
	{
		DeleteObject(m_arrBrush[i]);
	}
	DestroyMenu(m_hMenu); 

	for (int i = 0; i < THREAD::END; ++i)
	{
		DeleteDCBITMAP(m_hThreadMazentaDC[i], m_hThreadMazentaBit[i]);
	}
}

void CCore::CreateBrushPen()
{
	
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::WHITE] = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::RED] = CreateSolidBrush(RGB(255, 0, 0));
	m_arrBrush[(UINT)BRUSH_TYPE::BLUE] = CreateSolidBrush(RGB(0, 0, 255));
	m_arrBrush[(UINT)BRUSH_TYPE::GREEN] = CreateSolidBrush(RGB(0, 255, 0));
	m_arrBrush[(UINT)BRUSH_TYPE::MAZENTA] = CreateSolidBrush(RGB(255, 0, 255));

	m_arrPen[(UINT)PEN_TYPE::BLACK] = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPen[(UINT)PEN_TYPE::MAZENTA] = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
}

void CCore::DockMenu()
{
	SetMenu(m_hWnd, m_hMenu); 
	ChangeWindowSize(m_ptResolution, true); 
}

void CCore::DividMenu()
{
	SetMenu(m_hWnd, nullptr); 
	ChangeWindowSize(m_ptResolution, false); 
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt{ 0,0,static_cast<LONG>(_vResolution.x),static_cast<LONG>(_vResolution.y) };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu); 
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}

int CCore::init(HWND _hwnd, POINT _ptResolution, HINSTANCE _hInst)
{

	m_hWnd = _hwnd;						
	m_ptResolution = _ptResolution;		
	m_hInst = _hInst;

	ChangeWindowSize(m_ptResolution, false);

	// ��� ������ ����� �޴����� �ڵ��� �������ִ´�.
	// �ε�޴� �Լ��� ���ҽ� ����� �˾ƾ��Ѵ�
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_TERMPROJECTGAMEFRAMEWORK)); // �����Ҷ� ���� �޴����̵� ���� // �Ҹ��ڿ��� ���� �ʼ���
	// ��� ���� ���� enter �Ҷ� �޴� ������۾��� �ʿ��ϴ�

	m_hDC = GetDC(m_hWnd);	
	
	m_hMemDC = CreateCompatibleDC(m_hDC);
	m_hBackBuffer = CreateCompatibleBitmap(m_hDC,m_ptResolution.x ,m_ptResolution.y );
	DeleteObject(SelectObject(m_hMemDC, m_hBackBuffer));
	
	SetGraphicsMode(m_hMemDC, GM_ADVANCED);
	CPathMgr::GetInst()->init();
	//Mgr(CSoundMgr)->init();
	Mgr(CResMgr)->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSceneMgr::GetInst()->init();
	Mgr(CEventMgr)->init();	 
	Mgr(CAtlasMgr)->init();
	Mgr(CThreadMgr)->init();
	CreateBrushPen();
	
	Clear();

	for (int i = 0; i < THREAD::END; ++i)
	{
		CreateDCBITMAP(m_hThreadMazentaDC[i], m_hThreadMazentaBit[i], GetResolutionV());
	}

	return S_OK;
}


static bool bDebugInit;

void CCore::progress()
{
	// �Ŵ����� ������Ʈ
	if (!bDebugInit)
	{
		Mgr(CDebugMgr)->init();
		jthread{ []() {Mgr(CDebugMgr)->progress(); } }.detach();
		bDebugInit = true;
	}
	CTimeMgr::GetInst()->update();

	CKeyMgr::GetInst()->update();
	//CCamera::GetInst()->update();
	// =========
	// �� ������Ʈ
	// =========
	CSceneMgr::GetInst()->update();
	// �浹üũ
	CCollisionMgr::GetInst()->update();

	//=============
	// UI �̺�Ʈ üũ
	// ============
	//CUIMgr::GetInst()->update();

	//==============
	// ������ ����
	// =============
	CCamera::GetInst()->update();
	CSceneMgr::GetInst()->render(m_hMemDC);
	
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,		
		m_hMemDC, 0, 0, SRCCOPY);
	//Clear();
	Mgr(CEventMgr)->update();

}

void CCore::Clear()
{
	//PatBlt(m_hMemDC, 0, 0, m_ptResolution.x, m_ptResolution.y, WHITENESS);
	Mgr(CResMgr)->Clear();
}

HBRUSH CCore::ChangeBrush(BRUSH_TYPE _eType)
{
	return (HBRUSH)SelectObject(m_hMemDC, m_arrBrush[etoi(_eType)]);
}
HPEN CCore::ChangePen(PEN_TYPE _eType) 
{
	return (HPEN)SelectObject(m_hMemDC, m_arrPen[etoi(_eType)]);
}


void CCore::RotateTransform(int _iDegree, Vec2 centerPt)
{
	if (centerPt.x == 0 && centerPt.y == 0)
	{
		RECT rect = {};
		GetClientRect(m_hWnd, &rect);
		centerPt.x = (float)(rect.right - rect.left) / 2.f;
		centerPt.y = (float)(rect.bottom - rect.top) / 2.f;
	}
	XFORM xform;
	float fangle = (float)_iDegree / 180.f * 3.1415926f;
	xform.eM11 = (float)cosf(fangle);
	xform.eM12 = (float)sinf(fangle);
	xform.eM21 = (float)-sinf(fangle);
	xform.eM22 = (float)cosf(fangle);
	xform.eDx = (float)(centerPt.x - cosf(fangle) * centerPt.x + sinf(fangle) * centerPt.y);
	xform.eDy = (float)(centerPt.y - cosf(fangle) * centerPt.y - sinf(fangle) * centerPt.x);
	m_xform = xform * m_xform;
	SetWorldTransform(m_hMemDC, &m_xform);
}

void CCore::ResetTransform()
{
	XFORM xform;
	xform.eM11 = (FLOAT)1.0;
	xform.eM12 = (FLOAT)0.0;
	xform.eM21 = (FLOAT)0.0;
	xform.eM22 = (FLOAT)1.0;
	xform.eDx = (FLOAT)0.0;
	xform.eDy = (FLOAT)0.0;
	m_xform = xform;
	SetWorldTransform(m_hMemDC, &m_xform);
}

void CCore::ScaleTransform(float _fScale)
{
	XFORM xform;
	xform.eM11 = _fScale;
	xform.eM12 = (FLOAT)0;
	xform.eM21 = (FLOAT)0;
	xform.eM22 = _fScale;
	xform.eDx = 0;
	xform.eDy = 0;
	m_xform = xform * m_xform;
	SetWorldTransform(m_hMemDC, &m_xform);
}

void CCore::TranslateTransform(Vec2 vDist)
{
	XFORM xform;
	xform.eM11 = (FLOAT)1;
	xform.eM12 = (FLOAT)0;
	xform.eM21 = (FLOAT)0;
	xform.eM22 = (FLOAT)1;
	xform.eDx = (FLOAT)(vDist.x);
	xform.eDy = (FLOAT)(vDist.y);
	m_xform = xform * m_xform;
	SetWorldTransform(m_hMemDC, &m_xform);
}

void CCore::YFlipTransform(Vec2 vCenter)
{
	if (vCenter.x == 0 && vCenter.y == 0)
	{
		RECT rect = {};
		GetClientRect(m_hWnd, &rect);
		vCenter.x = (float)(rect.right - rect.left) / 2.f;
		vCenter.y = (float)(rect.bottom - rect.top) / 2.f;
	}
	XFORM xform;
	xform.eM11 = (FLOAT)-1;
	xform.eM12 = (FLOAT)0;
	xform.eM21 = (FLOAT)0;
	xform.eM22 = (FLOAT)1;
	xform.eDx =  (FLOAT)(vCenter.x*2.);
	xform.eDy =  (FLOAT)0;
	m_xform = xform * m_xform;
	SetWorldTransform(m_hMemDC, &m_xform);
}

void CCore::XFlipTransform(Vec2 vCenter)
{
	if (vCenter.x == 0 && vCenter.y == 0)
	{
		RECT rect = {};
		GetClientRect(m_hWnd, &rect);
		vCenter.x = (float)(rect.right - rect.left) / 2;
		vCenter.y = (float)(rect.bottom - rect.top) / 2;
	}
	XFORM xform;
	xform.eM11 = (FLOAT)1;
	xform.eM12 = (FLOAT)0;
	xform.eM21 = (FLOAT)0;
	xform.eM22 = (FLOAT)-1;
	xform.eDx = (FLOAT)0;
	xform.eDy = (FLOAT)vCenter.y * 2;
	m_xform = xform * m_xform;
	SetWorldTransform(m_hMemDC, &m_xform);
}

void CCore::RotateTransform(HDC dc, int _iDegree, Vec2 centerPt)
{
	if (centerPt.x == 0 && centerPt.y == 0)
	{
		RECT rect = {};
		GetClientRect(m_hWnd, &rect);
		centerPt.x = (float)(rect.right - rect.left) / 2;
		centerPt.y = (float)(rect.bottom - rect.top) / 2;
	}
	XFORM xform;
	float fangle = (float)_iDegree / 180.f * 3.1415926f;
	xform.eM11 = (float)cosf(fangle);
	xform.eM12 = (float)sinf(fangle);
	xform.eM21 = (float)-sinf(fangle);
	xform.eM22 = (float)cosf(fangle);
	xform.eDx = (float)(centerPt.x - cosf(fangle) * centerPt.x + sinf(fangle) * centerPt.y);
	xform.eDy = (float)(centerPt.y - cosf(fangle) * centerPt.y - sinf(fangle) * centerPt.x);
	m_xform2 = xform * m_xform2;
	SetWorldTransform(dc, &m_xform2);
}

void CCore::ResetTransform(HDC dc)
{
	XFORM xform;
	xform.eM11 = (FLOAT)1.0;
	xform.eM12 = (FLOAT)0.0;
	xform.eM21 = (FLOAT)0.0;
	xform.eM22 = (FLOAT)1.0;
	xform.eDx = (FLOAT)0.0;
	xform.eDy = (FLOAT)0.0;
	m_xform2 = xform;
	SetWorldTransform(dc, &m_xform2);
}

void CCore::YFlipTransform(HDC dc, Vec2 vCenter)
{
	if (vCenter.x == 0 && vCenter.y == 0)
	{
		RECT rect = {};
		GetClientRect(m_hWnd, &rect);
		vCenter.x = (float)(rect.right - rect.left) / 2;
		vCenter.y = (float)(rect.bottom - rect.top) / 2;
	}
	XFORM xform;
	xform.eM11 = (FLOAT)-1;
	xform.eM12 = (FLOAT)0;
	xform.eM21 = (FLOAT)0;
	xform.eM22 = (FLOAT)1;
	xform.eDx = (FLOAT)(vCenter.x * 2.);
	xform.eDy = (FLOAT)0;
	m_xform2 = xform * m_xform2;
	SetWorldTransform(dc, &m_xform2);
}

void CCore::XFlipTransform(HDC dc, Vec2 vCenter)
{
	if (vCenter.x == 0 && vCenter.y == 0)
	{
		RECT rect = {};
		GetClientRect(m_hWnd, &rect);
		vCenter.x = (float)(rect.right - rect.left) / 2;
		vCenter.y = (float)(rect.bottom - rect.top) / 2;
	}
	XFORM xform;
	xform.eM11 = (FLOAT)1;
	xform.eM12 = (FLOAT)0;
	xform.eM21 = (FLOAT)0;
	xform.eM22 = (FLOAT)-1;
	xform.eDx = (FLOAT)0;
	xform.eDy = (FLOAT)vCenter.y * 2;
	m_xform2 = xform * m_xform2;
	SetWorldTransform(dc, &m_xform2);
}

void CCore::ScaleTransform(HDC dc, float _fScale)
{
	XFORM xform;
	xform.eM11 = _fScale;
	xform.eM12 = (FLOAT)0;
	xform.eM21 = (FLOAT)0;
	xform.eM22 = _fScale;
	xform.eDx = 0;
	xform.eDy = 0;
	m_xform2 = xform * m_xform2;
	SetWorldTransform(dc, &m_xform2);
}

void CCore::TranslateTransform(HDC dc, Vec2 vDist)
{
	XFORM xform;
	xform.eM11 = (FLOAT)1;
	xform.eM12 = (FLOAT)0;
	xform.eM21 = (FLOAT)0;
	xform.eM22 = (FLOAT)1;
	xform.eDx = (FLOAT)(vDist.x);
	xform.eDy = (FLOAT)(vDist.y);
	m_xform2 = xform * m_xform2;
	SetWorldTransform(dc, &m_xform2);
}

void CCore::ScaleTransform(HDC dc, float _fScaleX, float _fScaleY)
{
	XFORM xform;
	xform.eM11 = _fScaleX;
	xform.eM12 = (FLOAT)0;
	xform.eM21 = (FLOAT)0;
	xform.eM22 = _fScaleY;
	xform.eDx = 0;
	xform.eDy = 0;
	m_xform3 = xform * m_xform3;
	SetWorldTransform(dc, &m_xform3);
}

void CCore::ResetTransformDebug(HDC dc)
{
	XFORM xform;
	xform.eM11 = (FLOAT)1.0;
	xform.eM12 = (FLOAT)0.0;
	xform.eM21 = (FLOAT)0.0;
	xform.eM22 = (FLOAT)1.0;
	xform.eDx = (FLOAT)0.0;
	xform.eDy = (FLOAT)0.0;
	m_xform3 = xform;
	SetWorldTransform(dc, &m_xform3);
}
