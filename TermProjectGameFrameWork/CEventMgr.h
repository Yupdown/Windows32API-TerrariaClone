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
	vector<unique_ptr<CObject>>		   m_vecDeadObj;

	function<void(void)> m_fpTRupdate = nullptr;

	SpinLock m_spinLock = {};
public:
	void init();
	void update();
	void AddCoRoutineWithObj(CObject* const _pObj, CoRoutine&& _coEvn) {
		SPIN_LOCK;
		m_mapCoRoutine.emplace(_pObj, std::move(_coEvn)); 
	}
	void AddCoRoutineWithOutObj(CoRoutine&& _coEvn) { 
		SPIN_LOCK;
		m_listCoRoutine.emplace_back(std::move(_coEvn)); 
	}
	size_t GetCoRoutineSize()const { return m_mapCoRoutine.size(); }
	template<typename Func, typename... Args> requires std::invocable<Func,Args...>
	void AddEvent(Func&& fp,Args&&... args) 
	{ 
		SPIN_LOCK;
		m_vecEvent.emplace_back([fp = std::forward<Func>(fp), ...args = std::forward<Args>(args)]()mutable{std::invoke(std::forward<decltype(fp)>(fp), std::forward<Args>(args)...); });
	}
	void AddDeadObj(unique_ptr<CObject>& _pDeadObj);

	template<typename Func, typename... Args> requires std::invocable<Func, Args...>
	void SetTRupdate(Func&& fp, Args&&... args)
	{
		m_fpTRupdate = std::bind(std::forward<Func>(fp), std::forward<Args>(args)...);
	}
	void ResetTRupdate() { m_fpTRupdate = nullptr; }

	void Reset();
};

