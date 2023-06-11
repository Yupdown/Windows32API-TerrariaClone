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

struct CamRect
{
	Vec2 vLT = {};
	Vec2 vRB = {};

	Vec2 vLookMid = {};

	static constexpr Vec2 vCamSize = Vec2{1400.f,800.f};

	operator RECT()const
	{
		return RECT{ (int)vLT.x,(int)vLT.y,(int)vRB.x,(int)vRB.y };
	}
};

class CTexture;

class CCamera
	:public Singleton<CCamera>
{
	friend class Singleton;
	CCamera();
	~CCamera();
	static constexpr Vec2 vCamLowLimit{ 0.f,4096.f };
	static constexpr Vec2 vCamUpperLimit{ (float)8192.f,0.f };
private:
	CamRect	m_CamRect = {};

	Vec2		m_vLookAt = {}; 
	Vec2		m_vCurLookAt = {}; 
	Vec2		m_vPrevLookAt = {};
	Vec2		m_vResolution = {};
	Vec2		m_vOriginMid = {};
	
	CObject* m_pTargetObj = {}; 

	Vec2		m_vDiff = {};		
	float m_fOriginSpeed = 2000.f;
	float		m_fTime = { 1.0f };
	float		m_fSpeed = { 2000.f };
	float		m_fAccTime = {};
	float		m_fAccel = {};
	CTexture* m_pVeilTex = {}; 

	//float		m_fSpeed = 1000.; 
	float m_fDestSpeed = 2000.f;
	float		m_fShakeAcc = 0.1f;
	UINT        Shake = 4;
	bool		ShakeFlag = false;
	bool        m_bMoveFlag = false;
	std::deque<tCamEffect>	m_listCamEffect; 
	float m_fCamZoom = 1.f;

	HDC m_hCamVeilDC = nullptr;
	HBITMAP m_hCamVeilBit = nullptr;

public:
	void init();
	void render(HDC _dc);
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
		m_fSpeed = m_fDestSpeed;
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
	constexpr bool IsCamMove()const { return m_bMoveFlag; }
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
	CamRect GetCamRect()const { return m_CamRect; }
	void SetCamRect(Vec2 _vCamLookMid) 
	{ 
		_vCamLookMid.x = max(m_vOriginMid.x, _vCamLookMid.x);
		_vCamLookMid.y = max(m_vOriginMid.y , _vCamLookMid.y);
		_vCamLookMid.x = min(((float)(8192.f) - 700.f), _vCamLookMid.x);
		_vCamLookMid.y = min(4096.f - m_vOriginMid.y , _vCamLookMid.y);
		m_CamRect.vLookMid = _vCamLookMid;
		m_CamRect.vLT = _vCamLookMid - m_CamRect.vCamSize / 2.f;
		m_CamRect.vRB = _vCamLookMid + m_CamRect.vCamSize / 2.f;
		SetNowLookAt(_vCamLookMid);
	}

	void SetCamRect(Vec2 _vGlobalLT, Vec2 _vGlobalRB)
	{
		SetCamRect((_vGlobalLT + _vGlobalRB) / 2.f);
	}

	void SetShakeFlag(bool _b) { ShakeFlag = _b; }
	constexpr bool IsCamShake()const { return ShakeFlag; }
	void Reset()
	{
		m_CamRect = {};

		m_vLookAt = {};
		m_vCurLookAt = {};
		m_vPrevLookAt = {};
		m_vResolution = {};
		m_vOriginMid = {};

		m_pTargetObj = {};

		m_vDiff = {};

		m_fTime = { 1.0f };
		m_fSpeed = { 2000.f };
		m_fAccTime = {};
		m_fAccel = {};
		
		m_fShakeAcc = 0.1f;
		Shake = 4;
		ShakeFlag = false;
		m_bMoveFlag = false;
		m_listCamEffect.clear();
		m_fCamZoom = 1.f;
		init();
	}
	CoRoutine CamMoveCoRoutine(const Vec2 _vDest);

	CoRoutine ZoomInBoss(const Vec2 _vBossPos);
private:
	void CalDiff();
	
};

