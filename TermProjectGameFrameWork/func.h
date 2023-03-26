#pragma once
#include "CoRoutine.hpp"
#include "CTimeMgr.h"

template <typename T>
constexpr int etoi(T _eType) { return static_cast<int>(_eType); }

int wrapAround(int x, int low, int high);

class CObject;
class AI;

void StartCoRoutine(CObject* const _pObj,CoRoutine&& _co);

void StartCoEvent(CoRoutine&& _co);

template <typename T>
CoRoutine DelayCoRoutine(T* const _pObj, void(T::* _objFp)(void), double _dDelayTime)
{
	double dAccTime = 0.;
	while (dAccTime < _dDelayTime)
	{
		dAccTime += DT;
		co_await std::suspend_always{};
	}
	(_pObj->*_objFp)();
	co_return;
}

template<typename T>
void StartDelayCoRoutine(T* const _pObj, void(T::* _objFp)(void), double _dDelayTime)
{
	StartCoRoutine(_pObj, DelayCoRoutine(_pObj, _objFp,_dDelayTime));
}

CoRoutine DelayCoRoutineLambda(CObject* const _pObj, function<void(void)>&& _fp, double _dDelayTime)
{
	double dAccTime = 0.;
	while (dAccTime < _dDelayTime)
	{
		dAccTime += DT;
		co_await std::suspend_always{};
	}
	_fp();
	co_return;
}

void StartDelayCoRoutine(CObject* const _pObj, function<void(void)>&& _fp, double _dDelayTime)
{
	StartCoRoutine(_pObj, DelayCoRoutineLambda(_pObj, std::move(_fp), _dDelayTime));
}

void CreateObj(CObject* const _pObj, GROUP_TYPE _eGroup);

void DeleteObj(CObject* const _pDeadObj);

CoRoutine ChangeScene(SCENE_TYPE _eNext);

void ChangeAIState(AI* const _pAI, MON_STATE _eNextState);