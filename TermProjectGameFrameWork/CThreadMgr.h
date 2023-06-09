#pragma once
#include "Singleton.hpp"

class CThreadMgr
	: public Singleton<CThreadMgr>
{
	friend class Singleton;
	CThreadMgr();
	~CThreadMgr();
private:
	std::vector<std::jthread> m_vecThread;
	std::array<std::atomic<bool>, THREAD::END> m_arrDone;
	std::queue<std::pair<std::function<void()>, size_t>> m_jobQueue;


	std::mutex m_mutexQ;
	std::condition_variable m_cvQ;
	bool m_bStopRequest = false;


	//std::mutex m_mutexMain;
	//std::condition_variable m_cvMain;

private:
	void stop_all();
public:
	void init();

	void Join(const size_t _idx)const;
	void Join_all()const;

	template<typename Func, typename... Args>
		requires std::invocable<Func, Args...>
	void Enqueue(const size_t _idx, Func&& fp, Args&&... args)
	{
		std::lock_guard<std::mutex> lock{ m_mutexQ };
		m_jobQueue.emplace([_idx,fp,args...]()mutable {std::invoke(std::forward<Func>(fp), std::forward<Args>(args)...); }, _idx);
		m_arrDone[_idx].store(false, std::memory_order_relaxed);
		m_cvQ.notify_one();
	}
};

