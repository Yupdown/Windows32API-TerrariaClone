#pragma once
#include "Singleton.hpp"
#include "Registry.hpp"
#include "TRItem.h"

class TRItemManager : public Singleton<TRItemManager>
{
private:
	Registry<TRItem>* registry;

public:
	friend class Singleton;

	TRItemManager();
	~TRItemManager();

	void LoadItems();
	void Initialize();

	TRItem* GetItemByID(int id) const;
	TRItem* GetItemByKey(const std::wstring& key) const;
};

