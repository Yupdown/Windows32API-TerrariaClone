#include "pch.h"
#include "func.h"
#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CEventMgr.h"
#include "CTimeMgr.h"

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

	Mgr(CEventMgr)->AddEvent([&_pDeadObj]() {_pDeadObj->SetDead(); });
}

void ChangeAIState(AI* const _pAI, MON_STATE _eNextState) 
{

}