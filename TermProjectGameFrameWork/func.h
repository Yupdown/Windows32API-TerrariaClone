#pragma once
#include "CoRoutine.hpp"
#include "CTimeMgr.h"

template <typename T> requires std::is_enum<T>::value
constexpr int etoi(T _eType) { return static_cast<int>(_eType); }

int wrapAround(const int x,const int low,const int high);

class CObject;
class AI;
class CScene;

void StartCoRoutine(CObject* const _pObj,CoRoutine&& _co);

void StartCoEvent(CoRoutine&& _co);

//template <typename T> requires std::is_base_of<CObject, T>::value
//CoRoutine DelayCoRoutine(T* const _pObj, void(T::* _objFp)(void), double _dDelayTime)
//{
//	double dAccTime = 0.;
//	while (dAccTime < _dDelayTime)
//	{
//		dAccTime += DT;
//		co_await std::suspend_always{};
//	}
//	(_pObj->*_objFp)();
//	co_return;
//}
//
//template<typename T> requires std::is_base_of<CObject, T>::value
//void StartDelayCoRoutine(T* const _pObj, void(T::* _objFp)(void), double _dDelayTime)
//{
//	StartCoRoutine(_pObj, DelayCoRoutine(_pObj, _objFp,_dDelayTime));
//}

CoRoutine DelayCoRoutine( function<void(void)> _fp, float _fDelayTime);

//template<typename Obj,typename Func,typename... Args>
//requires std::is_base_of<CObject,Obj>::value || std::invocable<Func,Args...>
//void StartDelayCoRoutine(float _fElapsedTime , Obj* const _pObj ,Func&& fp, Args&&... args)
//{
//	StartCoRoutine(_pObj, DelayCoRoutine(std::bind(std::forward<Func>(fp), _pObj, std::forward<Args>(args)...), _fElapsedTime));
//}

CoRoutine DelayCoRoutine(CObject* const _pObj, CoRoutine _delayCoEvn, float _fDelayTime);


void StartDelayCoRoutine(CObject* const _pObj, CoRoutine&& _delayCoEvn, float _fDelayTime);


template <typename T> requires std::derived_from<T, CObject> || std::derived_from<T, CScene>
CoRoutine DelayCoRoutine(T* const _pObj, void(T::* _objFp)(void), float _fDelayTime)
{
	float fAccTime = 0.;
	while (fAccTime < _fDelayTime)
	{
		fAccTime += DT;
		co_await std::suspend_always{};
	}
	(_pObj->*_objFp)();
	co_return;
}

template<typename T>requires std::derived_from<T, CObject>
void StartDelayCoRoutine(T* const _pObj, void(T::* _objFp)(void), float _fDelayTime)
{
	StartCoRoutine(_pObj, DelayCoRoutine(_pObj, _objFp, _fDelayTime));
}

template<typename T>requires std::derived_from<T, CScene>
void StartDelayCoRoutine(T* const _pObj, void(T::* _objFp)(void), float _fDelayTime)
{
	StartCoEvent(DelayCoRoutine(_pObj, _objFp, _fDelayTime));
}


void CreateObj(CObject* const _pObj, GROUP_TYPE _eGroup);

void DeleteObj(CObject* const _pDeadObj);

void ChangeScene(SCENE_TYPE _eNext);

void ChangeAIState(AI* const _pAI, MON_STATE _eNextState);

XFORM operator*(const XFORM& lhs, const XFORM& rhs);

HDC CreateDCBITMAP(HDC& _dc, HBITMAP& _hBit,Vec2 _vBitMapScale);

void DeleteDCBITMAP(HDC& _dc, HBITMAP& _hBit);

//BOOL TransparentBltSafe(HDC hdcDest
//	, int xoriginDest
//	, int yoriginDest
//	, int wDest
//	, int hDest
//	, HDC hdcSrc
//	, int xoriginSrc
//	, int yoriginSrc
//	, int wSrc
//	, int hSrc
//	, int wBit
//	, int hBit
//	, UINT crTransparent);

BOOL TransparentBltSafe(HDC hdcDest
	, const short xoriginDest
	, const short yoriginDest
	, const short wDest
	, const short hDest
	, HDC hdcSrc
	, const short xoriginSrc
	, const short yoriginSrc
	, const short wSrc
	, const short hSrc
	, const short wBit
	, const short hBit
	, UINT crTransparent);

void renderText(HDC _dc,COLORREF _rgb ,Vec2 _vGlobalLT, wstring_view _wstrText);

