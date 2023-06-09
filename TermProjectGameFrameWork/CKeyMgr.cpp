#include "pch.h"
#include "CKeyMgr.h"
#include "CCore.h"	


extern HHOOK hHook;

int g_arrVK[(int)KEY::LAST]		
{
	VK_LEFT,	
	VK_RIGHT,			
	VK_UP,		
	VK_DOWN,		
	'W',		
	'A'	,
	'S'	,		
	'D'	,		
	VK_SPACE,			
	VK_RETURN ,		
	VK_TAB,		
	VK_LBUTTON,
	VK_RBUTTON ,
	VK_LSHIFT ,
	VK_CONTROL ,
	VK_RSHIFT ,
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',
	'M',
	VK_ESCAPE
};

CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr() 
{

}

void CKeyMgr::init()
{
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.emplace_back(tKeyInfo{ KEY_STATE::NONE,false });
	}
}

void CKeyMgr::update()
{
	HWND hWnd{ GetFocus() };	

	if (hWnd)		
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPush)	
				{
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true; 
			}
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}

				m_vecKey[i].bPrevPush = false;
			}
		}
		GetCursorPos(&m_ptCurMousePos);	
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_ptCurMousePos);
		m_vCurMousePos = m_ptCurMousePos;
	}
	else
	{
		for (auto& key : m_vecKey)
		{
			key.bPrevPush = false;

			if (KEY_STATE::TAP == key.eState || KEY_STATE::HOLD == key.eState)
			{
				key.eState = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == key.eState)
			{
				key.eState = KEY_STATE::NONE;
			}
		}
	}

}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_MOUSEWHEEL)
		{
			UnhookWindowsHookEx(hHook);
			MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;
			Mgr(CKeyMgr)->m_wheelDelta = GET_WHEEL_DELTA_WPARAM(pMouseStruct->mouseData);
			hHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}