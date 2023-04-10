#pragma once
#include "CScene.h"

class CScene_Tool :
    public CScene
{
private:
	Vec2		m_vMouseDownPos = {};
	Vec2		m_vMouseMovePos = {};
	Vec2		m_vMouseUpPos = {};
	bool		m_bMouseDown = false;
	bool		m_bTileDone = false;
public:
    CScene_Tool();
    ~CScene_Tool();
public:
	virtual void render(HDC _dc)override;
	virtual void update()override;
	virtual void Enter()override;
	virtual void Exit()override;
	Vec2 GetLtPos()const { return m_vMouseDownPos; }
	Vec2 GetRbPos()const { return m_vMouseUpPos; }
	void SetDone() { m_bTileDone = true; }
};

