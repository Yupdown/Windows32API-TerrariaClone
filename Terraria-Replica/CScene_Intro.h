#pragma once
#include "CScene.h"

class CScene_Intro :
    public CScene
{
private:
	const CImage* m_pIntroImg = nullptr;
	const CImage* m_pIntroLogo = nullptr;

	float m_fScrollSpeed = 0.f;
	int m_iImageLoopPos = 0;

	float m_fAccTime = 0.f;
	bool m_bTurn = true;

	HDC m_hIntroDC;
	HBITMAP m_hIntroBit;

	unique_ptr<class CPlayer> m_pPlayer;
	float m_fDir = 1.f;

	bool m_bLoading = false;
public:
	CScene_Intro();
	~CScene_Intro();
public:
	void Enter()override;
	void Exit()override;
	void update()override;
	void render(HDC _dc)override;
};

