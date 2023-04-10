#pragma once

class CObject;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	NONE, 
};

struct tCamEffect
{
	CAM_EFFECT eEffect;	 
	float	fDuration; 
	float	fCurTime; 
};

class CTexture;

class CCamera
	:public Singleton<CCamera>
{
	friend class Singleton;
	CCamera();
	~CCamera();
private:
	Vec2		m_vLookAt = {}; 
	Vec2		m_vCurLookAt = {}; 
	Vec2		m_vPrevLookAt = {};

	CObject* m_pTargetObj = {}; 

	Vec2		m_vDiff = {};		

	double		m_dTime = { 1. };
	double		m_dSpeed = { 3000. };
	double		m_dAccTime = {};
	double		m_dAccel = {};
	CTexture* m_pVeilTex = {}; 

	double		m_Speed = 1000.; 
	double		m_dShakeAcc = 1.;
	UINT        Shake = 4;
	bool		ShakeFlag = false;
	bool        m_bMoveFlag = false;
	list<tCamEffect>	m_listCamEffect; 
public:
	void init();
	//void render(HDC _dc);
	constexpr Vec2	GetLookAt()const { return m_vCurLookAt; }
	void	FadeIn(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_IN;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;
		m_listCamEffect.emplace_back(ef);
		assert(_fDuration); // 0ÃÊ¸é ÄÆ
	}
	void	FadeOut(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_OUT;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;
		m_listCamEffect.emplace_back(ef);
		assert(_fDuration); 
	}
	void SetLookAt(Vec2 _vLook)
	{
		m_dSpeed = 3000.;
		m_dAccTime = 0.;
		m_vLookAt = _vLook;
		double dMoveDist = (m_vLookAt - m_vPrevLookAt).length();
		m_dTime = 2. * dMoveDist / m_dSpeed;	
		m_dAccTime = 0.;	
		m_dAccel = m_dSpeed / m_dTime;
		SetMoveFlag(true);
	}
	constexpr void SetTarget(CObject* const _pTarget) { m_pTargetObj = _pTarget;}
	constexpr Vec2 GetRenderPos(Vec2 _vObjPos)const { return _vObjPos - m_vDiff; }
	constexpr Vec2 GetRealPos(Vec2 _vRenderPos)const { return _vRenderPos + m_vDiff; }
	constexpr void SetMoveFlag(bool _b = true) { m_bMoveFlag = _b; }
public:
	void update();

private:
	void CalDiff();
};

