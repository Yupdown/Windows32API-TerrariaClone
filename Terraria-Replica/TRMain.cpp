#include "pch.h"
#include "TRMain.h"
#include "TRTileManager.h"
#include "CSceneMgr.h"
#include "CLayer.h"

TRMain::TRMain()
{
	TRTileManager::GetInst()->LoadTiles();

	active_world = new TRWorld();
	active_world->CreateWorld(0);

	scene_agent = Mgr(CSceneMgr)->GetCurScene();
	active_world->OnSceneCreate(scene_agent);
}

TRMain::~TRMain()
{
	delete active_world;
}

void TRMain::Update()
{

}