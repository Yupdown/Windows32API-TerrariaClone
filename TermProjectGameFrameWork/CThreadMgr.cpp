#include "pch.h"
#include "CThreadMgr.h"


CThreadMgr::CThreadMgr()
{
}

CThreadMgr::~CThreadMgr()
{
    stop_all();
}

void CThreadMgr::stop_all()
{
    m_bStopRequest = true;

    m_cvQ.notify_all();

    for (auto& t : m_vecThread)
    {
        t.join();
    }
}

void CThreadMgr::init()
{
    m_vecThread.resize(THREAD::END);
    for (size_t i = 0; i < THREAD::END; ++i) {
        m_arrDone[i].store(true, std::memory_order_seq_cst);
        m_vecThread[i] = std::jthread([this] {
            while (true) {
                std::function<void(void)> task;
                size_t idx;
                {
                    std::unique_lock<std::mutex> lock{ m_mutexQ };
                    m_cvQ.wait(lock, [this] {
                        return !m_jobQueue.empty() || m_bStopRequest;
                        });

                    if (m_bStopRequest) {
                        break;
                    }

                    std::tie(task,idx) = std::move(m_jobQueue.front());
                    m_jobQueue.pop();
                }

                task();

                std::atomic_thread_fence(std::memory_order_seq_cst);

                m_arrDone[idx].store(true,std::memory_order_relaxed);
            }
            });
    }
}

void CThreadMgr::Join(const size_t _idx)const
{
    while (!m_arrDone[_idx].load(std::memory_order_relaxed))
    {
    }
}

void CThreadMgr::Join_all()const
{
    while (!std::all_of(m_arrDone.begin(), m_arrDone.end(), [](const auto& b) { return b.load(std::memory_order_relaxed); }))
    {
    }
}

