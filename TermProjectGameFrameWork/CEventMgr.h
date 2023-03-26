#pragma once
#include "CoRoutine.hpp"

class CObject;

class CEventMgr
	:public Singleton<CEventMgr>
{
	friend class Singleton;
	CEventMgr();
	~CEventMgr();
private:
	vector<function<void(void)>> m_vecEvent;
	multimap<CObject*, CoRoutine>      m_mapCoRoutine;
	std::list<CoRoutine>			   m_listCoRoutine;
public:
	void init();
	void update();
	void AddEvent(function<void(void)>&& _fp) { m_vecEvent.emplace_back(std::move(_fp)); }
	void AddCoRoutineWithObj(CObject* const _pObj, CoRoutine&& _coEvn) { m_mapCoRoutine.emplace(make_pair(_pObj, std::move(_coEvn))); }
	void AddCoRoutineWithOutObj(CoRoutine&& _coEvn) { m_listCoRoutine.emplace_back(std::move(_coEvn)); }
	size_t GetCoRoutineSize()const { return m_mapCoRoutine.size(); }
};

