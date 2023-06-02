#include "pch.h"
#include "TRWorld.h"
#include "TRTileManager.h"
#include "TRTile.h"
#include "Vec2Int.hpp"
#include "CKeyMgr.h"
#include "CCamera.h"
#include "CScene.h"

TRWorld::TRWorld()
{
	tile_map = new TRTileMap(TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT);
	player = new CPlayer(this);
	player->SetPos(Vec2{ 100.0f, 100.0f });
	player->SetScale(Vec2{ 40.f, 56.f });
}

TRWorld::~TRWorld()
{
	delete tile_map;
}

void TRWorld::Update()
{
	if (Mgr(CKeyMgr)->GetKeyState(KEY::LBTN) == KEY_STATE::TAP)
	{
		Vec2Int tile_pos = TRWorld::GlobalToWorld(Mgr(CCamera)->GetRealPos(Mgr(CKeyMgr)->GetMousePos()));
		tile_map->SetTile(tile_pos.x, tile_pos.y, Mgr(TRTileManager)->GetTileByKey("planks_wood"), true);

		/*wchar_t tbuffer[64];
		wsprintf(tbuffer, L"Clicked Position : {%d, %d}", tile_pos.x, tile_pos.y);
		MessageBox(NULL, tbuffer, L"Result", MB_OK);*/
	}
}

void TRWorld::CreateWorld(int seed)
{
	TRWorldGeneration* generator = new TRWorldGeneration();

	TRWorldGenerationProcess* processes[] =
	{
		new TRWorldGenerationTerrainHeight(),
		new TRWorldGenerationAttachWall(),
		new TRWorldGenerationPinchCaves(),
		new TRWorldGenerationGrowOres(Mgr(TRTileManager)->GetTileByKey("copper_ore"), -20),
		new TRWorldGenerationGrowOres(Mgr(TRTileManager)->GetTileByKey("iron_ore"), 0),
		new TRWorldGenerationGrowOres(Mgr(TRTileManager)->GetTileByKey("silver_ore"), 10),
		new TRWorldGenerationGrowOres(Mgr(TRTileManager)->GetTileByKey("gold_ore"), 30),
		new TRWorldGenerationGrowGrass()
	};

	for (int i = 0; i < sizeof(processes) / sizeof(*processes); ++i)
		generator->AddProcess(processes[i]);
	generator->GenerateWorld(tile_map, TRWorld::WORLD_WIDTH, TRWorld::WORLD_HEIGHT, seed);
	delete generator;
	for (int i = 0; i < sizeof(processes) / sizeof(*processes); ++i)
		delete processes[i];
}

void TRWorld::OnSceneCreate(CScene* scene)
{
	tile_map->OnSceneCreate(scene);
	scene->AddObject(player, GROUP_TYPE::PLAYER);
	Mgr(CCamera)->SetTarget(player);
	scene->RegisterPlayer(player);
}

Vec2 TRWorld::WorldToGlobal(const Vec2& v)
{
	return Vec2(v.x * PIXELS_PER_TILE, (static_cast<float>(TRWorld::WORLD_HEIGHT) - v.y) * PIXELS_PER_TILE);
}

Vec2 TRWorld::GlobalToWorld(const Vec2& v)
{
	return Vec2(v.x / PIXELS_PER_TILE, static_cast<float>(TRWorld::WORLD_HEIGHT) - v.y / PIXELS_PER_TILE);
}

TRTileMap* TRWorld::GetTileMap() const
{
	return tile_map;
}
