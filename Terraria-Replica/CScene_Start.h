#pragma once
#include "CScene.h"

class CAtlasElement;

class CScene_Start
	:public CScene
{
private:

public:
	CScene_Start();
	~CScene_Start();
public:
	void Enter()override;
	void Exit()override;
	void update()override;
};

