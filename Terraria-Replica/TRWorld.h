#pragma once

#include "TRTile.h"
#include "TRTileMap.h"
#include "TRWorldGeneration.h"
#include "CPlayer.h"
#include "CScene.h"

#include "TRItemContainer.h"
#include "TRItemStack.h"

class TRItemContainer;
class CQuickBarVisualizer;
class CHealthIndicator;
class CInventoryVisualizer;

class TRWorld
{
public:
	static constexpr int WORLD_WIDTH = 512;
	static constexpr int WORLD_HEIGHT = 256;

private:
	TRTileMap* tile_map;
	CPlayer* player;
	CScene* m_pScene;

	TRItemContainer* player_inventory[50];
	TRItemContainer* player_armor[3];
	TRItemContainer* quick_bar[10];

	CQuickBarVisualizer* quick_bar_visualizer;
	CInventoryVisualizer* inventory_visualizer;

	CHealthIndicator* health_indicator;
	int quick_bar_index;
	bool toggle_inventory;

public:
	TRWorld();
	~TRWorld();

	void Update();

	void CreateWorld(int seed);
	void OnSceneCreate(CScene* scene);

	static Vec2 WorldToGlobal(const Vec2& v);
	static Vec2 GlobalToWorld(const Vec2& v);

	TRTileMap* GetTileMap() const;
	CPlayer* GetPlayer() const;

	bool PlaceTile(int x, int y, TRTile* new_tile);
	void BreakTile(int x, int y);
	bool PlaceTileWall(int x, int y, TRTileWall* new_tile);
	void BreakTileWall(int x, int y);

	void DropItem(Vec2 world_pos, TRItemStack item);

	void SetToggleInventory(bool value);
	void AddItemToInventory(TRItemStack item);
	void SwitchQuickBarIndex(int value);

	auto& GetQuickBarList() { return quick_bar; }
	int GetQuickBarIdx()const { return quick_bar_index; }
	int GetArmorPoint() const;

	void FloatDamageText(int value, Vec2 vPos, COLORREF color);
	void SpawnBoss();
};