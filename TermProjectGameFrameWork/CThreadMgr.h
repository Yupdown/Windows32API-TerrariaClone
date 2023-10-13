#pragma once
#include "Singleton.hpp"
#include "CircleQueue.hpp"

class CThreadMgr
	: public Singleton<CThreadMgr>
{
	friend class Singleton;
	CThreadMgr();
	~CThreadMgr();
	static constexpr int NUM_OF_WORKER = 6;
private:
	std::vector<std::jthread> m_vecThread;
	std::array<std::atomic<bool>, THREAD::END> m_arrDone;
	//std::queue<std::pair<std::function<void()>, size_t>> m_jobQueue;
	CircleQueue<std::pair<std::function<void()>, size_t>> m_jobQueue;

	SpinLock m_spinLockRender = {};
	std::condition_variable_any m_cvQRender;
	bool m_bStopRequest = false;

	std::vector<std::jthread> m_vecUpdateWorker;
	SpinLock m_spinLockUpdate = {};
	std::condition_variable_any m_cvQUpdate;
	std::atomic<int> m_jobCount = 0;
	//std::queue<function<void(void)>> m_jobUpdateQ;
	CircleQueue<function<void(void)>> m_jobUpdateQ;
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
	void Enqueue(const size_t _idx, Func&& fp, Args&&... args)noexcept
	{
		{
			std::lock_guard<SpinLock> lock{ m_spinLockRender };
			m_jobQueue.emplace([fp = std::forward<Func>(fp), ...args = std::forward<Args>(args)]()mutable noexcept{std::invoke(std::forward<decltype(fp)>(fp), std::forward<Args>(args)...); }, _idx);
			m_arrDone[_idx].store(false, std::memory_order_relaxed);
		}
		m_cvQRender.notify_one();
	}
	template<typename Func, typename... Args>
		requires std::invocable<Func, Args...>
	void EnqueueUpdate(Func&& fp, Args&&... args)noexcept
	{
		m_jobCount.fetch_add(1);
		auto task = ([fp = std::forward<Func>(fp), ...args = std::forward<Args>(args)]()mutable noexcept{std::invoke(std::forward<Func>(fp), std::forward<Args>(args)...); });
		{
			std::lock_guard<SpinLock> lock{ m_spinLockUpdate };
			m_jobUpdateQ.emplace(std::move(task));
		}
		m_cvQUpdate.notify_one();
	}
	void JoinUpdate()const noexcept{
		while (m_jobCount.load()) {

		}
	}
};

