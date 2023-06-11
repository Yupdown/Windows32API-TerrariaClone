#pragma once

#include <string>
#include <Windows.h>
#include "CAtlasElement.h"

#include "TRWorld.h"
#include "CPlayer.h"

class TRItem
{
protected:
	std::wstring name;
	std::wstring k_element;

	int max_stacksize;

	CImage* element;

public:
	TRItem(std::wstring name, std::wstring k_element);
	virtual ~TRItem();

	void CreateAtlasElements();
	std::wstring GetName() const;
	int GetMaxStacksize() const;
	CImage* GetItemElement() const;
	virtual bool OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos);
};

class TRItemTile : public TRItem
{
protected:
	std::wstring k_tile;

public:
	TRItemTile(std::wstring name, std::wstring k_element, std::wstring k_tile);
	~TRItemTile();

	virtual bool OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos) override;
};

class TRItemTileWall : public TRItem
{
protected:
	std::wstring k_tilewall;

public:
	TRItemTileWall(std::wstring name, std::wstring k_element, std::wstring k_tilewall);
	~TRItemTileWall();

	virtual bool OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos) override;
};

class TRItemTool : public TRItem
{
public:
	TRItemTool(std::wstring name, std::wstring k_element);
	~TRItemTool();
};

class TRItemPickaxe : public TRItemTool
{
public:
	TRItemPickaxe(std::wstring name, std::wstring k_element);
	~TRItemPickaxe();

	virtual bool OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos) override;
};

class TRItemHammer : public TRItemTool
{
public:
	TRItemHammer(std::wstring name, std::wstring k_element);
	~TRItemHammer();

	virtual bool OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos) override;
};

class TRItemSword : public TRItemTool
{
public:
	TRItemSword(std::wstring name, std::wstring k_element);
	~TRItemSword();

	virtual bool OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos) override;
};

class TRItemSummonBoss : public TRItem
{
public:
	TRItemSummonBoss(std::wstring name, std::wstring k_element);
	~TRItemSummonBoss();

	virtual bool OnUseItem(CPlayer* user, TRWorld* world, const Vec2& target_pos) override;
};

class TRItemArmor : public TRItem
{
private:
	int armor_part;
	int armor_point;

public:
	TRItemArmor(std::wstring name, std::wstring k_element, int armor_part, int armor_point);
	~TRItemArmor();

	int GetArmorPart() const;
	int GetArmorPoint() const;
};