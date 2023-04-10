#pragma once

enum class KEY_STATE
{
	NONE,	
	TAP, 
	HOLD, 
	AWAY, 
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	W,
	A,
	S,
	D,
	SPACE,
	ENTER, 
	TAB,
	LBTN,
	RBTN,	
	LSHIFT, 
	CTRL, 
	RSHIFT,
	LAST,		

};

struct tKeyInfo
{
	KEY_STATE		eState;		// Ű�� ���¸� �˷��ִ� enum ��
	bool			bPrevPush;			// ������ ���ȴ��� �ȴ��ȴ���
};

class CKeyMgr
	:public Singleton<CKeyMgr>
{
	friend class Singleton;
	CKeyMgr();
	~CKeyMgr();
private:
	vector<tKeyInfo>m_vecKey;			
	Vec2			m_vCurMousePos = {};
	POINT			m_ptCurMousePos = {};
public:
	void init();
	void update();
public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	Vec2	GetMousePos()const { return m_vCurMousePos; }
	POINT   GetMousePosPt()const { return m_ptCurMousePos; }
};
