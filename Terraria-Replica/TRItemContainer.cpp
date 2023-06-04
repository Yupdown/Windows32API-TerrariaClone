#include "pch.h"
#include "TRItemContainer.h"

TRItemContainer::TRItemContainer()
{
	item_stack = TRItemStack();
}

TRItemContainer::~TRItemContainer()
{

}

TRItemStack TRItemContainer::GetItemStack() const
{
	return item_stack;
}

bool TRItemContainer::Blank() const
{
	return item_stack.Null();
}

TRItemStack TRItemContainer::Apply(TRItemStack item_stack)
{
	TRItemStack::Apply(this->item_stack, item_stack);

	return item_stack;
}

void TRItemContainer::Apply(int amount)
{
	TRItemStack::AddStack(this->item_stack, amount);
}