#include "pch.h"
#include "func.h"
#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CEventMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"

int wrapAround(int x, int low, int high)
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

CoRoutine ChangeScene(SCENE_TYPE _eNext)
{
	while (Mgr(CEventMgr)->GetCoRoutineSize() > 1)
	{
		co_await std::suspend_always{};
	}
	CSceneMgr::GetInst()->ChangeScene(_eNext);
	co_return;
}

void CreateObj(CObject* const _pObj, GROUP_TYPE _eGroup)
{
	Mgr(CEventMgr)->AddEvent([&_pObj, &_eGroup]() {
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
	Mgr(CEventMgr)->AddEvent([&_pDeadObj]() {_pDeadObj->SetDead(); });
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

void CreateDCBITMAP(HDC& _dc, HBITMAP& _hBit, Vec2 _vBitMapScale)
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
