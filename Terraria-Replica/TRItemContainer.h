#pragma once
#include "TRItemStack.h"

class TRItemContainer
{
private:
	TRItemStack item_stack;

public:
	TRItemContainer();
	~TRItemContainer();

	TRItemStack GetItemStack() const;
	bool Blank() const;

	TRItemStack Apply(TRItemStack item_stack);
	void Apply(int amount);
};

