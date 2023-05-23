#include "pch.h"
#include "CScene_Tool.h"
#include "CCore.h"
#include "CCamera.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "resource.h"

CScene_Tool::CScene_Tool()
{
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::update()
{
	const Vec2 vCurCamLook = Mgr(CCamera)->GetLookAt();
	const Vec2 vCurMousePos = MOUSE_POS;
	wstring curLookPosData = L"카메라: (" + std::to_wstring(vCurCamLook.x) + L" , " + std::to_wstring(vCurCamLook.y) + L") ";
	curLookPosData += L"마우스: (" + std::to_wstring(vCurMousePos.x) + L" , " + std::to_wstring(vCurMousePos.y) + L")";
	SetWindowText(CCore::GetInst()->GetMainHwnd(), curLookPosData.data());

	if (KEY_TAP(KEY::RSHIFT)) {
		Mgr(CCamera)->SetLookAt(Mgr(CCore)->GetResolutionV() / 2.);
	}

	if (KEY_TAP(KEY::LBTN)) {
		m_vMouseDownPos = MOUSE_POS;
		m_bMouseDown = true;
		m_bTileDone = false;
	}
	if (KEY_AWAY(KEY::LBTN)) {
		m_vMouseUpPos = MOUSE_POS;
		m_bMouseDown = false;
		//m_bTileDone = true;
	}
	m_vMouseMovePos = MOUSE_POS;
}

void CScene_Tool::render(HDC _dc)
{
	CScene::render(_dc);
	if (m_bMouseDown)
	{
		Rectangle(_dc
			, std::lround(m_vMouseDownPos.x)
			, std::lround(m_vMouseDownPos.y)
			, std::lround(m_vMouseMovePos.x)
			, std::lround(m_vMouseMovePos.y));
	}

	if (!m_bMouseDown && !m_bTileDone)
	{
		Rectangle(_dc
			, std::lround(m_vMouseDownPos.x)
			, std::lround(m_vMouseDownPos.y)
			, std::lround(m_vMouseUpPos.x)
			, std::lround(m_vMouseUpPos.y));
	}
}

void CScene_Tool::Enter()
{
	Mgr(CCore)->DockMenu();
}

void CScene_Tool::Exit()
{
	Mgr(CCore)->DividMenu();
}


//================================== TileProc
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iRow = GetDlgItemInt(hDlg, IDC_ROW, nullptr, false); 
			UINT iCol = GetDlgItemInt(hDlg, IDC_COL, nullptr, false);
			UINT iBitIdx = GetDlgItemInt(hDlg, IDC_IDX, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);
			const Vec2 vScale =   pToolScene->GetRbPos()- pToolScene->GetLtPos();
		//	pCurScene->AddChunkTile(L"Tile.bmp", pToolScene->GetLtPos(), vScale, iRow, iCol, iBitIdx);
			pToolScene->SetDone();
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}
	return (INT_PTR)FALSE;
}