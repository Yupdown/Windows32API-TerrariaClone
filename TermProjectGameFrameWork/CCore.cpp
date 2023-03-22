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

	// 대신 툴씬이 사용할 메뉴바의 핸들을 가지고있는다.
	// 로드메뉴 함수는 리소스 헤더를 알아야한다
	//m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_CLIENT)); // 시작할때 썻던 메뉴아이디 복붙 // 소멸자에서 해제 필수임
	// 대신 이제 툴씬 enter 할때 메뉴 만드는작업이 필요하다

	m_hDC = GetDC(m_hWnd);		// 그림을 그려줄 DC를 얻는다 .
	// 리소스매니저에게 더블버퍼링용 화면을 받는다
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer", m_ptResolution.x, m_ptResolution.y);
	// 매니저들 초기화

	CPathMgr::GetInst()->init();	// 경로 찾기
	//Mgr(CSoundMgr)->init();
	Mgr(CResMgr)->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();		// 여기에서 static CSceneMgr mgr 객체가 기본생성되는데, 이때 인라인이고 그래서 여기서 기본생성자가 호출되는데
	CCamera::GetInst()->init();									// 기본생성자가 헤더파일 자체에 선언과 동시에 디폴트로 정의되어있다면 그걸  쓰게되고, 그걸 쓰는데 걔는 헤더에 CScene이 없어서 
	// CScene들의 정보를 몰라서 만들 수가 없다
	// 생성자가 호출될 곳에 필요한 잡 헤더들을 넣자
	// 소멸자가 호출 될 곳도 잘 생각하고,
	// 가급적 Pimple을 사용할 때 기본생성자와 기본소멸자를 cpp에 넣어두자


// 이중 버퍼링 용도의 비트맵과 DC를 만든다.
// 58화: 결국 이런 더블버퍼링용 비트맵을 만들었다는 것 자체가 자체적으로 텍스쳐를 만들어 쓰고있었다는 뜻임
//m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
//m_memDC = CreateCompatibleDC(m_hDC);
//HBITMAP hOldBit{ (HBITMAP)SelectObject(m_memDC,m_hBit) };	// memDC를 m_hBit에 그리도록 설정한다
//DeleteObject(hOldBit);	// 반환값인 기존 더미 비트맵을 삭제한다.
// ============ 58화부터 리소스매니저 에게 위임

	CreateBrushPen();

	return S_OK;
}



void CCore::progress()
{
	// 매니저들 업데이트
	CTimeMgr::GetInst()->update();

	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();
	// =========
	// 씬 업데이트
	// =========
	CSceneMgr::GetInst()->update();

	// 씬매니저의 업데이트가 완료된후 콜리전매니저의 업데이트
	// 충돌체크
	CCollisionMgr::GetInst()->update();

	//=============
	// UI 이벤트 체크
	// ============
	//CUIMgr::GetInst()->update();

	//==============
	// 렌더링 시작
	// =============


	// 화면 clear
	// 58화 알파블렌딩에서 더블버퍼링용 텍스쳐를 따로 만들었으니 걔한테 이제 DC받는다
	// 62화 코어에서 화면을 지우는기능을 따로 때서 분리한다. 이때 화면색깔 브러쉬도 고르게한다. 여기서 브러쉬 세팅하면 렌더링하는 내내 모든 브러쉬가 검은색기 되기 때문
	//Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x, m_ptResolution.y);
	Clear();


	// 렌더를 Scene 매니저에게 위임
	CSceneMgr::GetInst()->render(m_pMemTex->GetDC());	// 씬매니저에게 그림을 그려줄 DC를 전달해야한다.
	// 카메라의 렌더시점은 모든물체가 렌더링 되고 나서
	//CCamera::GetInst()->render(m_pMemTex->GetDC());

	// 더블버퍼링된 다른 DC에서 복사한다.
	// 즉 우리가 보아야할 곳으로 이중버퍼링 비트맵 내용을 그대로 복사한다
	BitBlt(m_hDC, -m_ptResolution.x, -m_ptResolution.y, m_ptResolution.x*2, m_ptResolution.y*2,		// hDC에 복사.
		m_pMemTex->GetDC(), -m_ptResolution.x, -m_ptResolution.y, SRCCOPY);

	//CTimeMgr::GetInst()->render();
	// 렌더링까지 끝난 다음에야 이벤트매니저가 일을한다.
	// 왜냐하면 렌더 전에 이벤트매니저가 이벤트를해결하면 업데이트 된애랑 안된애가 뒤섞여서 렌더가 될것이기때문
	//CEventMgr::GetInst()->update();
	//CO_EventMgr::GetInst()->update();
	//Mgr(CSoundMgr)->update();
}

void CCore::Clear()
{
	PatBlt(m_pMemTex->GetDC(), -m_ptResolution.x, -m_ptResolution.y, m_ptResolution.x*2, m_ptResolution.y*2, WHITENESS);
}

HBRUSH CCore::ChangeBrush(BRUSH_TYPE _eType)
{
	return (HBRUSH)SelectObject(m_pMemTex->GetDC(), m_arrBrush[etoi(_eType)]);
}
HPEN CCore::ChangePen(PEN_TYPE _eType) 
{
	return (HPEN)SelectObject(m_pMemTex->GetDC(), m_arrPen[etoi(_eType)]); 
}

