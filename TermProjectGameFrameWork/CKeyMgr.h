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
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	ZERO,
	M,
	LAST,		

};

struct tKeyInfo
{
	KEY_STATE		eState;		// Ű�� ���¸� �˷��ִ� enum ��
	bool			bPrevPush;			// ������ ���ȴ��� �ȴ��ȴ���
};

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

class CKeyMgr
	:public Singleton<CKeyMgr>
{
	friend LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	friend class Singleton;
	CKeyMgr();
	~CKeyMgr();
private:
	vector<tKeyInfo>m_vecKey;			
	Vec2			m_vCurMousePos = {};
	POINT			m_ptCurMousePos = {};
	short			m_wheelDelta=0;
public:
	void init();
	void update();
public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	Vec2	GetMousePos()const { return m_vCurMousePos; }
	POINT   GetMousePosPt()const { return m_ptCurMousePos; }
	void ReSetWheel() { m_wheelDelta = 0; }
	bool GetMouseWheelUp()const { return m_wheelDelta > 0; }
	bool GetMouseWheelDown()const { return m_wheelDelta < 0; }
};
