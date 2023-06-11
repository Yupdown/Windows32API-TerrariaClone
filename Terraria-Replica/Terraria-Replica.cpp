// TermProjectGameFrameWork.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "CEventMgr.h"
#include "CCore.h"
#include "TRMain.h"
#include "Terraria-Replica.h"
#include "CSceneMgr.h"
#include "CScene_Start.h"
#include "CKeyMgr.h"
#include "CObject.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CustomMath.hpp"
#include "CScene_Intro.h"
#include "TRItemManager.h"
#include "TRTileManager.h"
#include "CCthulhuEye.h"

void updateTileCollision(CObject* const _pObj, TRWorld* const _pTRWorld);
extern bool g_bStopToken;
extern jthread g_LoadThread;
extern std::atomic<bool> g_bLoadMainStage;

HHOOK hHook;
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND g_hWnd;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MiniMapProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TERMPROJECTGAMEFRAMEWORK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    if (FAILED(CCore::GetInst()->init(g_hWnd, POINT{ 1400,800 },hInst)))        // 해상도
    {
        MessageBox(nullptr, L"게임 실행 실패", L"ERROR", MB_OK);
    }
    

    // 씬 생성

    Mgr(TRTileManager)->LoadTiles();
    Mgr(TRItemManager)->LoadItems();

    auto pSceneStart = new CScene_Start;
    Mgr(CSceneMgr)->AddScene(SCENE_TYPE::START, pSceneStart);

    auto pSceneIntro = new CScene_Intro;
    Mgr(CSceneMgr)->AddScene(SCENE_TYPE::INTRO, pSceneIntro);

    // 시작 씬 설정
    Mgr(CSceneMgr)->init(SCENE_TYPE::INTRO);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TERMPROJECTGAMEFRAMEWORK));

    MSG msg;
    //hHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
            {
                g_bStopToken = true;
               
                break;
            }
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            CCore::GetInst()->progress();
        }
    }
    Mgr(CEventMgr)->ResetTRupdate();
   // UnhookWindowsHookEx(hHook);
   
    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TERMPROJECTGAMEFRAMEWORK));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_TERMPROJECTGAMEFRAMEWORK);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    wcex.hCursor = LoadCursor(NULL, IDC_HELP);
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszClassName = L"DebugMgr"; 
    wcex.lpfnWndProc = MiniMapProc; 
    return  RegisterClassEx(&wcex); 
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd)
    {
        return FALSE;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MiniMapProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case ID_MAKE_TILE:
        {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_TILE), hWnd, TileCountProc);
        }
        break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void updateTileCollision(CObject* const _pObj,TRWorld* const _pTRWorld)
{
	auto pRigid = _pObj->GetComp<CRigidBody>();
    auto pCol = _pObj->GetComp<CCollider>();

    CCthulhuEye* casting = dynamic_cast<CCthulhuEye*>(_pObj);

    if (pCol == nullptr || casting != nullptr)
    {
        _pObj->SetPos(_pObj->GetWillPos());
        return;
    }

	auto pTileMap = _pTRWorld->GetTileMap();
	Vec2 world_pos = TRWorld::GlobalToWorld(_pObj->GetWillPos());
	Vec2 world_vel = pRigid->GetVelocity();

	float w = pCol->GetScale().x / (float)PIXELS_PER_TILE;
	float h = pCol->GetScale().y / (float)PIXELS_PER_TILE;

	Vec2 pre_pos = TRWorld::GlobalToWorld(_pObj->GetPos());
	Vec2 post_pos = world_pos;
	Vec2 post_vel = world_vel;

	bool landed = false;
	bool collided = false;

	int x_min = FloorToInt(pre_pos.x - w * 0.5f);
	int x_max = CeilToInt(pre_pos.x + w * 0.5f) - 1;
	int y_min = FloorToInt(world_pos.y - h * 0.5f);
	int y_max = CeilToInt(world_pos.y + h * 0.5f) - 1;

	if (x_min >= 0 && x_max < TRWorld::WORLD_WIDTH && y_min >= 0 && y_max < TRWorld::WORLD_HEIGHT)
	{
		for (int x = x_min; x <= x_max; ++x)
		{
			if (world_vel.y > 0.0f && pTileMap->GetTile(x, y_min)->Solid())
			{
				post_pos.y = y_min + 1.0f + h * 0.5f;
				post_vel.y = 0.0f;

				landed = true;
				collided = true;
				break;
			}
			if (world_vel.y < 0.0f && pTileMap->GetTile(x, y_max)->Solid())
			{
				post_pos.y = y_max - h * 0.5f;
				post_vel.y = 0.0f;

				collided = true;
				break;
			}
		}
	}

	if (world_pos.x - w * 0.5f < 0.0f)
	{
		post_pos.x = w * 0.5f;
		post_vel.x = 0.0f;
	}
	if (world_pos.x + w * 0.5f > TRWorld::WORLD_WIDTH)
	{
		post_pos.x = TRWorld::WORLD_WIDTH - w * 0.5f;
		post_vel.x = 0.0f;
	}

	x_min = FloorToInt(post_pos.x - w * 0.5f);
	x_max = CeilToInt(post_pos.x + w * 0.5f) - 1;
	y_min = FloorToInt(post_pos.y - h * 0.5f);
	y_max = CeilToInt(post_pos.y + h * 0.5f) - 1;

	if (x_min >= 0 && x_max < TRWorld::WORLD_WIDTH && y_min >= 0 && y_max < TRWorld::WORLD_HEIGHT)
	{
		bool collision_x = false;
		float reform_x = 0.0f;

		for (int y = y_min; y <= y_max; ++y)
		{
			if (world_vel.x < 0.0f && pTileMap->GetTile(x_min, y)->Solid())
			{
				reform_x = x_min + 1.0f + w * 0.5f;

				collided = true;
				collision_x = true;
				break;
			}
			if (world_vel.x > 0.0f && pTileMap->GetTile(x_max, y)->Solid())
			{
				reform_x = x_max - w * 0.5f;

				collided = true;
				collision_x = true;
				break;
			}
		}

		if (collision_x)
		{
			y_min = y_min + 1;
			y_max = CeilToInt(y_min + h) - 1;

			bool flag = false;
			for (int x = x_min; x <= x_max; ++x)
			{
				for (int y = y_min; y <= y_max; ++y)
					flag |= pTileMap->GetTile(x, y)->Solid();
			}
			if (flag)
			{
 				post_pos.x = reform_x;
				post_vel.x = 0.0f;
			}
			else if (post_vel.y >= 0.0f)
				post_pos.y = y_min + h * 0.5f;
		}
	}

    _pObj->SetPos(TRWorld::WorldToGlobal(post_pos));
	pRigid->SetVelocity(post_vel);
    if (!landed)
    {
        pRigid->SetIsGround(false);
    }
}