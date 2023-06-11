#pragma once
#include "CScene.h"

class CAtlasElement;
class TRMain;

class CScene_Start
	:public CScene
{
private:
	TRMain* m_pTRMain = nullptr;
	bool m_bChangeScene = false;
public:
	CScene_Start();
	~CScene_Start();
public:
	void Enter()override;
	void Exit()override;
	void update()override;
	void render(HDC _dc)override;

	void CreateStage();

	void LoadWorld();
};

