#pragma once

#include "TRTile.h"
#include "TRTileMap.h"
#include "TRWorldGeneration.h"
#include "CPlayer.h"
#include "CScene.h"

#include "TRItemContainer.h"

class TRItemContainer;

class TRWorld
{
public:
	static constexpr int WORLD_WIDTH = 512;
	static constexpr int WORLD_HEIGHT = 256;

private:
	TRTileMap* tile_map;
	CPlayer* player;

	TRItemContainer* quick_bar[10];
	int quick_bar_index;

public:
	TRWorld();
	~TRWorld();

	void Update();

	void CreateWorld(int seed);
	void OnSceneCreate(CScene* scene);

	static Vec2 WorldToGlobal(const Vec2& v);
	static Vec2 GlobalToWorld(const Vec2& v);

	TRTileMap* GetTileMap() const;
};