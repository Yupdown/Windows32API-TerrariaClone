#include "pch.h"
#include "TRItem.h"
#include "CResMgr.h"
#include "TRTileManager.h"
#include "CustomMath.hpp"
#include "CSoundMgr.h"

TRItem::TRItem(std::wstring name, std::wstring k_element)
{
	this->name = name;
	this->k_element = k_element;
	this->max_stacksize = 99;

	if (k_element != L"")
		CreateAtlasElements();
}

TRItem::~TRItem()
{
}

void TRItem::CreateAtlasElements()
{
	element = Mgr(CResMgr)->GetImg(k_element);
}

std::wstring TRItem::GetName() const
{
	return name;
}

int TRItem::GetMaxStacksize() const
{
	return max_stacksize;
}

CImage* TRItem::GetItemElement() const
{
	return element;
}

bool TRItem::OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos)
{
	return false;
}

TRItemTile::TRItemTile(std::wstring name, std::wstring k_element, std::wstring k_tile) : TRItem(name, k_element)
{
	this->k_tile = k_tile;
}

TRItemTile::~TRItemTile()
{

}

bool TRItemTile::OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos)
{
	Vec2 vPos = TRWorld::GlobalToWorld(user->GetPos());
	if ((target_pos - vPos).length() > 10.0f)
		return false;

	int x = FloorToInt(target_pos.x);
	int y = FloorToInt(target_pos.y);
	bool result = world->PlaceTile(x, y, Mgr(TRTileManager)->GetTileByKey(k_tile));
	return result;
}

TRItemTileWall::TRItemTileWall(std::wstring name, std::wstring k_element, std::wstring k_tilewall) : TRItem(name, k_element)
{
	this->k_tilewall = k_tilewall;
}

TRItemTileWall::~TRItemTileWall()
{

}

bool TRItemTileWall::OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos)
{
	Vec2 vPos = TRWorld::GlobalToWorld(user->GetPos());
	if ((target_pos - vPos).length() > 10.0f)
		return false;

	int x = FloorToInt(target_pos.x);
	int y = FloorToInt(target_pos.y);
	bool result = world->PlaceTileWall(x, y, Mgr(TRTileManager)->GetTileWallByKey(k_tilewall));
	return result;
}

TRItemTool::TRItemTool(std::wstring name, std::wstring k_element) : TRItem(name, k_element)
{
	this->max_stacksize = 1;
}

TRItemTool::~TRItemTool()
{
}

TRItemPickaxe::TRItemPickaxe(std::wstring name, std::wstring k_element) : TRItemTool(name, k_element)
{
}

TRItemPickaxe::~TRItemPickaxe()
{
}

bool TRItemPickaxe::OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos)
{
	Vec2 vPos = TRWorld::GlobalToWorld(user->GetPos());
	if ((target_pos - vPos).length() > 10.0f)
		return false;

	int x = FloorToInt(target_pos.x);
	int y = FloorToInt(target_pos.y);
	world->BreakTile(x, y);
	
	return false;
}

TRItemHammer::TRItemHammer(std::wstring name, std::wstring k_element) : TRItemTool(name, k_element)
{
}

TRItemHammer::~TRItemHammer()
{
}

bool TRItemHammer::OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos)
{
	Vec2 vPos = TRWorld::GlobalToWorld(user->GetPos());
	if ((target_pos - vPos).length() > 10.0f)
		return false;

	int x = FloorToInt(target_pos.x);
	int y = FloorToInt(target_pos.y);
	world->BreakTileWall(x, y);
	return false;
}

TRItemSword::TRItemSword(std::wstring name, std::wstring k_element) : TRItemTool(name, k_element)
{
}

TRItemSword::~TRItemSword()
{
}

bool TRItemSword::OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos)
{
	return false;
}

TRItemSummonBoss::TRItemSummonBoss(std::wstring name, std::wstring k_element) : TRItem(name, k_element)
{
	this->max_stacksize = 1;
}

TRItemSummonBoss::~TRItemSummonBoss()
{
}

bool TRItemSummonBoss::OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos)
{
	Mgr(CSoundMgr)->PlayEffect("Roar_0.wav", 0.5f);
	world->SpawnBoss();
	return true;
}

TRItemArmor::TRItemArmor(std::wstring name, std::wstring k_element, int armor_part, int armor_point) : TRItem(name, k_element)
{
	this->armor_part = armor_part;
	this->armor_point = armor_point;
	this->max_stacksize = 1;
}

TRItemArmor::~TRItemArmor()
{
}

int TRItemArmor::GetArmorPart() const
{
	return armor_part;
}

int TRItemArmor::GetArmorPoint() const
{
	return armor_point;
}
