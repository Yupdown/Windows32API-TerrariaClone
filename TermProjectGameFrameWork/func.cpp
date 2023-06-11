#include "pch.h"
#include "func.h"
#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CEventMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"

int wrapAround(const int x,const int low,const int high)
{
	assert(low < high);
	const int n = (x - low) % (high - low);
	return (n >= 0) ? (n + low) : (n + high);
}

void StartCoRoutine(CObject* const _pObj, CoRoutine&& _co)
{
	Mgr(CEventMgr)->AddCoRoutineWithObj(_pObj, std::move(_co));
}

void StartCoEvent(CoRoutine&& _co)
{
	Mgr(CEventMgr)->AddCoRoutineWithOutObj(std::move(_co));
}


void ChangeScene(SCENE_TYPE _eNext)
{
	Mgr(CEventMgr)->AddEvent(&CSceneMgr::ChangeScene, Mgr(CSceneMgr),_eNext);
}


void CreateObj(CObject* const _pObj, GROUP_TYPE _eGroup)
{
	Mgr(CEventMgr)->AddEvent([_pObj, _eGroup]() {
		Mgr(CSceneMgr)->GetCurScene()->AddObject(_pObj, _eGroup);
		});
}

void DeleteObj(CObject* const _pDeadObj)
{
	if (_pDeadObj->IsDead())
	{
		return;
	}
	//Mgr(CEventMgr)->AddEvent(&CObject::SetDead, _pDeadObj);
	Mgr(CEventMgr)->AddEvent([_pDeadObj]() {_pDeadObj->SetDead(); });
}

void ChangeAIState(AI* const _pAI, MON_STATE _eNextState) 
{

}

XFORM operator*(const XFORM& lhs, const XFORM& rhs)
{
	XFORM xform = {};
	xform.eM11 = lhs.eM11 * rhs.eM11 + lhs.eM12 * rhs.eM21;
	xform.eM12 = lhs.eM11 * rhs.eM12 + lhs.eM12 * rhs.eM22;
	xform.eDx = lhs.eM11 * rhs.eDx + lhs.eM12 * rhs.eDy + lhs.eDx;
	xform.eM21 = lhs.eM21 * rhs.eM11 + lhs.eM22 * rhs.eM21;
	xform.eM22 = lhs.eM21 * rhs.eM12 + lhs.eM22 * rhs.eM22;
	xform.eDy = lhs.eM21 * rhs.eDx + lhs.eM22 * rhs.eDy + lhs.eDy;
	return xform;
}

HDC CreateDCBITMAP(HDC& _dc, HBITMAP& _hBit, Vec2 _vBitMapScale)
{
	_dc = CreateCompatibleDC(Mgr(CCore)->GetMainDC());
	_hBit = CreateCompatibleBitmap(Mgr(CCore)->GetMainDC(), (int)_vBitMapScale.x, (int)_vBitMapScale.y);
	DeleteObject(SelectObject(_dc, _hBit));
	auto hBrush = CreateSolidBrush(RGB(255, 0, 255));
	auto hOld = SelectObject(_dc, hBrush);
	auto hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	auto hOldPen = SelectObject(_dc, hPen);
	Rectangle(_dc, 0, 0, (int)_vBitMapScale.x, (int)_vBitMapScale.y);
	DeleteObject(SelectObject(_dc, hOld));
	DeleteObject(SelectObject(_dc, hOldPen));
	SetGraphicsMode(_dc, GM_ADVANCED);
	return _dc;
}

void DeleteDCBITMAP(HDC& _dc, HBITMAP& _hBit)
{
	DeleteDC(_dc);
	DeleteObject(_hBit);
}

CoRoutine DelayCoRoutine(function<void(void)> _fp, float _fDelayTime)
{
	float fAccTime = 0.f;
	while (fAccTime < _fDelayTime)
	{
		fAccTime += DT;
		co_await std::suspend_always{};
	}
	_fp();
	co_return;
}

//BOOL TransparentBltSafe(HDC hdcDest, int xoriginDest, int yoriginDest, int wDest, int hDest, HDC hdcSrc, int xoriginSrc, int yoriginSrc, int wSrc, int hSrc, int wBit, int hBit, UINT crTransparent)
//{
//	const int os_left = max(-xoriginSrc, 0);
//	const int os_top = max(-yoriginSrc, 0);
//	const int os_right = max(xoriginSrc + wSrc - wBit, 0);
//	const int os_bottom = max(yoriginSrc + hSrc - hBit, 0);
//	const int od_left = os_left * wDest / wSrc;
//	const int od_top = os_top * hDest / hSrc;
//	const int od_right = os_right * wDest / wSrc;
//	const int od_bottom = os_bottom * hDest / hSrc;
//
//	return TransparentBlt(hdcDest, xoriginDest + od_left, yoriginDest + od_top, wDest - od_left - od_right, hDest - od_top - od_bottom, hdcSrc, xoriginSrc + os_left, yoriginSrc + os_top, wSrc - os_left - os_right, hSrc - os_top - os_bottom, crTransparent);
//}

BOOL TransparentBltSafe(HDC hdcDest, const short xoriginDest, const short yoriginDest, const short wDest, const short hDest,
	HDC hdcSrc, const short xoriginSrc, const short yoriginSrc, const short wSrc, const short hSrc,
	const short wBit, const short hBit, UINT crTransparent)
{
	const short os_left = max(-xoriginSrc, 0);
	const short os_top = max(-yoriginSrc, 0);
	const short os_right = max(xoriginSrc + wSrc - wBit, 0);
	const short os_bottom = max(yoriginSrc + hSrc - hBit, 0);
	const short od_left = os_left * wDest / wSrc;
	const short od_top = os_top * hDest / hSrc;
	const short od_right = os_right * wDest / wSrc;
	const short od_bottom = os_bottom * hDest / hSrc;
		 
	const short destX = xoriginDest + od_left;
	const short destY = yoriginDest + od_top;
	const short destWidth = wDest - od_left - od_right;
	const short destHeight = hDest - od_top - od_bottom;
	const short srcX = xoriginSrc + os_left;
	const short srcY = yoriginSrc + os_top;
	const short srcWidth = wSrc - os_left - os_right;
	const short srcHeight = hSrc - os_top - os_bottom;

	return TransparentBlt(hdcDest
		, destX
		, destY
		, destWidth
		, destHeight
		, hdcSrc
		, srcX
		, srcY
		, srcWidth
		, srcHeight
		, crTransparent);
}

void renderText(HDC _dc,COLORREF _rgb,Vec2 _vGlobalLT, wstring_view _wstrText)
{
	SetTextColor(_dc, _rgb);
	TextOutW(_dc, (int)_vGlobalLT.x, (int)_vGlobalLT.y, _wstrText.data(), (int)_wstrText.size());
}

CoRoutine DelayCoRoutine(CObject* const _pObj, CoRoutine _delayCoEvn, float _fDelayTime)
{
	float fAccTime = 0.f;
	while (fAccTime < _fDelayTime)
	{
		fAccTime += DT;
		co_await std::suspend_always{};
	}
	StartCoRoutine(_pObj, CoRoutine{ std::move(_delayCoEvn) });
	co_return;
}

void StartDelayCoRoutine(CObject* const _pObj, CoRoutine&& _delayCoEvn, float _fDelayTime)
{
	StartCoEvent(DelayCoRoutine(_pObj, CoRoutine{ std::move(_delayCoEvn) }, _fDelayTime));
}