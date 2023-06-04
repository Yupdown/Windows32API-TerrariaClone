#pragma once

class TRItem;

struct TRItemStack
{
private:
	TRItem* item;
	int stack_size;

public:
	TRItemStack();
	TRItemStack(TRItem* item, int stack_size);
	~TRItemStack();

	TRItem* GetItem() const;
	int GetStackSize() const;
	bool Null() const;

	int StackUp(int amount);
	static void Apply(TRItemStack& dest, TRItemStack& src);
	static void AddStack(TRItemStack& dest, int amount);
};