#pragma once

#include "TRWorld.h"
#include "CScene.h"

class TRMain
{
private:
	TRWorld* active_world;
	CScene* scene_agent;

public:
	TRMain();
	~TRMain();

	void Update();
};