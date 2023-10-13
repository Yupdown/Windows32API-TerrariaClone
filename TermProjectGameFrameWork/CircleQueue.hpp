#pragma once

template<typename T>
class CircleQueue
{
	static constexpr unsigned short MAX_SIZE = 256;
private:
	T arr[MAX_SIZE] = {};
	unsigned short m_front = 0;
	unsigned short m_rear = 0;
	unsigned short m_count = 0;
public:
	template <typename... Args> 
	void emplace(Args&&... args)noexcept {
		arr[m_rear] = T{ std::forward<Args>(args)... };
		++m_count;
		m_rear = ((m_rear + 1) % MAX_SIZE);
	}
	T& front() noexcept { return arr[m_front]; }
	void pop()noexcept {
		--m_count;
		m_front = ((m_front + 1) % MAX_SIZE);
	}
	const bool empty()const noexcept { return 0 == m_count; }
};