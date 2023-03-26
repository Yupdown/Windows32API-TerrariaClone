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

CCore::CCore()
{

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
}

void CCore::CreateBrushPen()
{
	
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::WHITE] = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::RED] = CreateSolidBrush(RGB(255, 0, 0));
	m_arrBrush[(UINT)BRUSH_TYPE::BLUE] = CreateSolidBrush(RGB(0, 0, 255));
	m_arrBrush[(UINT)BRUSH_TYPE::GREEN] = CreateSolidBrush(RGB(0, 255, 0));

	m_arrPen[(UINT)PEN_TYPE::BLACK] = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
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
	SetWindowPos(m_hWnd, nullptr, 75, 75, rt.right - rt.left, rt.bottom - rt.top, 0);
}

int CCore::init(HWND _hwnd, POINT _ptResolution)
{

	m_hWnd = _hwnd;						
	m_ptResolution = _ptResolution;		
	
	ChangeWindowSize(m_ptResolution, false);

	// ��� ������ ����� �޴����� �ڵ��� �������ִ´�.
	// �ε�޴� �Լ��� ���ҽ� ����� �˾ƾ��Ѵ�
	//m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_CLIENT)); // �����Ҷ� ���� �޴����̵� ���� // �Ҹ��ڿ��� ���� �ʼ���
	// ��� ���� ���� enter �Ҷ� �޴� ������۾��� �ʿ��ϴ�

	m_hDC = GetDC(m_hWnd);	
	
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer", m_ptResolution.x, m_ptResolution.y);
	

	CPathMgr::GetInst()->init();
	//Mgr(CSoundMgr)->init();
	Mgr(CResMgr)->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSceneMgr::GetInst()->init();
	Mgr(CEventMgr)->init();	 
	


	CreateBrushPen();

	return S_OK;
}



void CCore::progress()
{
	// �Ŵ����� ������Ʈ
	CTimeMgr::GetInst()->update();

	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();
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
	CSceneMgr::GetInst()->render(m_pMemTex->GetDC());
	
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,		
		m_pMemTex->GetDC(), 0, 0, SRCCOPY);
	Clear();
	Mgr(CEventMgr)->update();
}

void CCore::Clear()
{
	PatBlt(m_pMemTex->GetDC(), -m_ptResolution.x, -m_ptResolution.y, m_ptResolution.x*2, m_ptResolution.y*2, WHITENESS);
	Mgr(CResMgr)->Clear();
}

HBRUSH CCore::ChangeBrush(BRUSH_TYPE _eType)
{
	return (HBRUSH)SelectObject(m_pMemTex->GetDC(), m_arrBrush[etoi(_eType)]);
}
HPEN CCore::ChangePen(PEN_TYPE _eType) 
{
	return (HPEN)SelectObject(m_pMemTex->GetDC(), m_arrPen[etoi(_eType)]); 
}

HDC CCore::GetMemDC() const
{
	return m_pMemTex->GetDC();
}

