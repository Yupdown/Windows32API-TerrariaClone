#pragma once
#include "pch.h"

#pragma optimize("", on)
template <typename T>
class DoubleLockQueue
{
private:
	struct Node {
		T data;
		Node* next = nullptr;
	};
	Node* head;
	Node* tail;
	SpinLock headLock, tailLock;
public:
	DoubleLockQueue() { head = tail = new Node; }
	template <typename... Args>
	void emplace(Args&&... args) noexcept{
		Node* value = new Node{ T{std::forward<Args>(args)...} };
		tail->next = value;
		tail = value;
	}
	const bool try_pop(T& _target)noexcept {
		std::lock_guard<SpinLock> lock{ tailLock };
		Node* oldHead = head;
		Node* newHead = head->next;
		if (newHead)
		{
			_target = std::move(newHead->data);
			head = newHead;
			delete oldHead;
			return true;
		}
		else
		{
			return false;
		}
	}
};
#pragma optimize("", off)