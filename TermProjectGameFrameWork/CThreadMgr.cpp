#include "pch.h"
#include "CThreadMgr.h"


CThreadMgr::CThreadMgr()
    :m_mutexMain{}
    ,m_cvQ{}
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
        m_arrDone[i].store(true, std::memory_order_relaxed);
        m_vecThread[i] = std::jthread([this] {
            while (true) {
                std::function<void(void)> task;
                size_t idx;
                {
                    std::unique_lock<std::mutex> lock{ m_mutexQ };
                    m_cvQ.wait(lock, [this] {
                        return !m_jobQueue.empty() || m_bStopRequest;
                        });

                    if (m_bStopRequest && m_jobQueue.empty()) {
                        break;
                    }

                    std::tie(task,idx) = std::move(m_jobQueue.front());
                    m_jobQueue.pop();
                }

                task();

               m_arrDone[idx].store(true, std::memory_order_release);

               m_cvMain.notify_all();
            }
            });
    }
}

bool CThreadMgr::isDone(const size_t _idx) const
{
    return m_arrDone[_idx].load(std::memory_order_acquire);
}

void CThreadMgr::Join(const size_t _idx)
{
    std::unique_lock<std::mutex> lock{ m_mutexMain };
    m_cvMain.wait_for(lock, std::chrono::milliseconds(16),[this, _idx] {
        return isDone(_idx);
    });
    m_cvMain.notify_all();
}

void CThreadMgr::join_all()
{
    while (!std::all_of(m_arrDone.begin(), m_arrDone.end(), [](const auto& b) { return b.load(std::memory_order_relaxed); }))
    {
        std::this_thread::yield();
    }
}

