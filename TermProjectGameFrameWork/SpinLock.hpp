#pragma once
#include "pch.h"

class SpinLock
{
private:
	std::atomic<bool> m_bIsLocked = false;
public:
	SpinLock() = default;
	~SpinLock() { unlock(); }
	void lock() {
		bool expected = false;
		bool desired = true;
		while (!m_bIsLocked.compare_exchange_weak(expected, desired)) {
			expected = false;
		}
	}
	void unlock() { m_bIsLocked.store(false); }
	bool try_lock() {
		bool expected = false;
		bool desired = true;
		return m_bIsLocked.compare_exchange_strong(expected, desired);
	}
};