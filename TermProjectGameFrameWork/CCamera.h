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
	Vec2		m_vResolution = {};
	Vec2		m_vOriginMid = {};
	
	CObject* m_pTargetObj = {}; 

	Vec2		m_vDiff = {};		

	float		m_fTime = { 1.f };
	float		m_fSpeed = { 3000.f };
	float		m_fAccTime = {};
	float		m_fAccel = {};
	CTexture* m_pVeilTex = {}; 

	//float		m_fSpeed = 1000.; 
	float		m_fShakeAcc = 1.;
	UINT        Shake = 4;
	bool		ShakeFlag = false;
	bool        m_bMoveFlag = false;
	list<tCamEffect>	m_listCamEffect; 
	float m_fCamZoom = 1.f;
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
		m_fSpeed = 3000.f;
		m_fAccTime = 0.f;
		m_vLookAt = _vLook;
		float fMoveDist = (m_vLookAt - m_vPrevLookAt).length();
		m_fTime = 2.f * fMoveDist / m_fSpeed;	
		m_fAccTime = 0.;	
		m_fAccel = m_fSpeed / m_fTime;
		SetMoveFlag(true);
	}
	constexpr void SetTarget(CObject* const _pTarget) { m_pTargetObj = _pTarget;}
	constexpr Vec2 GetRenderPos(Vec2 _vObjPos)const { return _vObjPos - m_vDiff; }
	constexpr Vec2 GetRealPos(Vec2 _vRenderPos)const { return _vRenderPos + m_vDiff; }
	constexpr Vec2 GetCurDiff()const { return m_vDiff; }
	constexpr void SetMoveFlag(bool _b = true) { m_bMoveFlag = _b; }
	constexpr float GetCamZoom()const { return m_fCamZoom; }
	void TransformRenderPos()const;
	void TransformRenderPos(HDC _dc)const;
	void ResetRenderPos()const;
	void ResetRenderPos(HDC _dc)const;
	pair<Vec2,Vec2> GetRenderPos(const CObject* const _pObj)const;
	void SetNowLookAt(Vec2 _vLook) { m_vCurLookAt =m_vLookAt = _vLook; }
	void renderBackGround(HDC _hDest, HDC _hSrc, Vec2 _vLayerScale ,float _fSpeed)const;

public:
	void update();

private:
	void CalDiff();
};

