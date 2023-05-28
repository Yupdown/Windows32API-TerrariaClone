#include "pch.h"
#include "TRItemManager.h"

TRItemManager::TRItemManager()
{
	registry = new Registry<TRItem*>();
}

TRItemManager::~TRItemManager()
{
	delete registry;
}

void TRItemManager::LoadItems()
{

}

void TRItemManager::Initialize()
{

}

TRItem* TRItemManager::GetItemByID(int id) const
{
	return (*registry)[id];
}

TRItem* TRItemManager::GetItemByKey(const std::string& key) const
{
	return (*registry)[key];
}
