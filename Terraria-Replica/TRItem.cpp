#include "pch.h"
#include "TRItem.h"
#include "CResMgr.h"
#include "TRTileManager.h"

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

int TRItem::GetMaxStacksize() const
{
	return max_stacksize;
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
	int x = floor(target_pos.x);
	int y = floor(target_pos.y);
	world->GetTileMap()->SetTile(x, y, Mgr(TRTileManager)->GetTileByKey(k_tile), true);
	return true;
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
	int x = floor(target_pos.x);
	int y = floor(target_pos.y);
	world->GetTileMap()->SetTileWall(x, y, Mgr(TRTileManager)->GetTileWallByKey(k_tilewall), true);
	return true;
}

TRItemTool::TRItemTool(std::wstring name, std::wstring k_element) : TRItem(name, k_element)
{

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
	int x = floor(target_pos.x);
	int y = floor(target_pos.y);
	world->GetTileMap()->SetTile(x, y, Mgr(TRTileManager)->TileAir(), true);
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
	int x = floor(target_pos.x);
	int y = floor(target_pos.y);
	world->GetTileMap()->SetTileWall(x, y, Mgr(TRTileManager)->TileWallAir(), true);
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
