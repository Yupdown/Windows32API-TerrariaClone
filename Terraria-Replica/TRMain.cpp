#include "pch.h"
#include "TRMain.h"
#include "TRTileManager.h"
#include "TRItemManager.h"
#include "CSceneMgr.h"
#include "CLayer.h"

#include <random>
#include <time.h>

TRMain::TRMain()
{
	std::default_random_engine dre((unsigned int)time(NULL));
	std::uniform_int_distribution<int> uid;

	Mgr(TRTileManager)->LoadTiles();
	Mgr(TRItemManager)->LoadItems();

	active_world = new TRWorld();
	active_world->CreateWorld(uid(dre));

	scene_agent = Mgr(CSceneMgr)->GetScene(SCENE_TYPE::START);
	active_world->OnSceneCreate(scene_agent);
}

TRMain::~TRMain()
{
	delete active_world;
}

void TRMain::Update()
{
	active_world->Update();
}