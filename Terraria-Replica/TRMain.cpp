#include "pch.h"
#include "TRMain.h"
#include "TRTileManager.h"
#include "TRItemManager.h"
#include "CSceneMgr.h"
#include "CLayer.h"

TRMain::TRMain()
{
	active_world = new TRWorld();
	active_world->CreateWorld(0);

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