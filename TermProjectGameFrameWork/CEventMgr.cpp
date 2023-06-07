#include "pch.h"
#include "CEventMgr.h"
#include "CObject.h"

CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::init()
{
	m_vecEvent.reserve(100);
}

void CEventMgr::update()
{
	for (auto iter = m_mapCoRoutine.begin(); iter != m_mapCoRoutine.end();)
	{
		if (iter->second.done() || iter->first->IsDead())
		{
			iter = m_mapCoRoutine.erase(iter);
		}
		else
		{
			iter->second.resume();
			++iter;
		}
	}

	for (auto iter = m_listCoRoutine.begin(); iter != m_listCoRoutine.end();)
	{
		if (iter->done())
		{
			iter = m_listCoRoutine.erase(iter);
		}
		else
		{
			iter->resume();
			++iter;
		}
	}

	for (const auto& eve : m_vecEvent)
	{
		eve();
	}

	m_vecEvent.clear();
	m_vecDeadObj.clear();

	if (m_fpTRupdate)
	{
		m_fpTRupdate();
	}
}

void CEventMgr::AddDeadObj(unique_ptr<CObject>& _pDeadObj)
{
	m_vecDeadObj.emplace_back(std::move(_pDeadObj));
}

void CEventMgr::Reset()
{
	m_vecEvent.clear();
	m_fpTRupdate = nullptr;
	m_listCoRoutine.clear();
	m_mapCoRoutine.clear();
}
