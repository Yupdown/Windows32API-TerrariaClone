#include "pch.h"
#include "TRItemStack.h"
#include "TRItem.h"

TRItemStack::TRItemStack()
{
	this->item = nullptr;
	this->stack_size = 0;
}

TRItemStack::TRItemStack(TRItem* item, int stack_size)
{
	this->item = item;
	this->stack_size = min(stack_size, item->GetMaxStacksize());
}

TRItemStack::~TRItemStack()
{

}

TRItem* TRItemStack::GetItem() const
{
	return item;
}

int TRItemStack::GetStackSize() const
{
	return stack_size;
}

bool TRItemStack::Null() const
{
	return item == nullptr;
}

int TRItemStack::StackUp(int amount)
{
	stack_size += amount;
	int max_size = item->GetMaxStacksize();

	if (stack_size > max_size)
	{
		int delta = stack_size - max_size;
		stack_size = max_size;
		return delta;
	}
	return 0;
}

void TRItemStack::Apply(TRItemStack& dest, TRItemStack& src)
{
	bool flag = false;

	if (!dest.Null() && !src.Null())
		flag = dest.GetItem() == src.GetItem();

	if (flag)
	{
		int size = dest.StackUp(src.GetStackSize());

		if (size > 0)
			src.stack_size = size;
		else
			src = TRItemStack();
	}
	else
		std::swap(dest, src);
}

void TRItemStack::AddStack(TRItemStack& dest, int amount)
{
	dest.StackUp(amount);

	if (dest.stack_size <= 0)
		dest = TRItemStack();
}
