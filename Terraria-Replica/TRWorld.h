#pragma once

#include "TRTile.h"
#include "TRTileMap.h"
#include "TRWorldGeneration.h"
#include "CPlayer.h"
#include "CScene.h"

class TRWorld
{
public:
	static constexpr int WORLD_WIDTH = 512;
	static constexpr int WORLD_HEIGHT = 256;

private:
	TRTileMap* tile_map;
	CPlayer* player;

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