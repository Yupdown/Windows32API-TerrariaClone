#include "pch.h"
#include "CCore.h"
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CObject.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"


CCamera::CCamera()
{

}

CCamera::~CCamera()
{
	DeleteDCBITMAP(m_hCamVeilDC, m_hCamVeilBit);
}

void CCamera::init()
{
	m_vResolution = CCore::GetInst()->GetResolutionV();
	m_vOriginMid = m_vLookAt = m_vCurLookAt = m_vPrevLookAt = m_vResolution / 2.f;
	m_CamRect.vLT = Vec2{ 0.f,0.f };
	m_CamRect.vRB = m_vResolution;
	m_CamRect.vLookMid = m_vResolution / 2.f;

	CreateDCBITMAP(m_hCamVeilDC, m_hCamVeilBit, m_vResolution);
	PatBlt(m_hCamVeilDC, 0, 0, (int)m_vResolution.x, (int)m_vResolution.y, BLACKNESS);
}

void CCamera::render(HDC _dc)
{
	auto pPlayer = Mgr(CSceneMgr)->GetCurScene()->GetPlayer();
	if (pPlayer)
	{
		pPlayer->dmg_render(_dc);
	}

	if (m_listCamEffect.empty())
	{
		return; 
	}
	tCamEffect& effect = m_listCamEffect.front();
	effect.fCurTime += DT;
	
	float fRatio = 0.f;
	fRatio = effect.fCurTime / effect.fDuration; 
	

	if (0.f > fRatio)
		fRatio = 0.f;
	if (1.f < fRatio)
		fRatio = 1.f;

	int iAlpha = 0;
	
	switch (effect.eEffect)
	{
	case CAM_EFFECT::FADE_IN:
		iAlpha = static_cast<int>(135.f * (1.f - fRatio));
		break;
	case CAM_EFFECT::FADE_OUT:
		iAlpha = static_cast<int>(135.f * fRatio);
		break;
	case CAM_EFFECT::NONE:
		break;
	default:
		break;
	}

	static BLENDFUNCTION bf = {
	.BlendOp = AC_SRC_OVER,
	.BlendFlags = 0,
	.AlphaFormat = 0,
	};

	bf.SourceConstantAlpha = iAlpha;

	AlphaBlend(_dc
		,0
		,0
		,(int)m_vResolution.x
		,(int)m_vResolution.y
		,m_hCamVeilDC
		,0
		,0
		,(int)m_vResolution.x
		,(int)m_vResolution.y
		,bf);

	
	if (effect.fDuration < effect.fCurTime) 
	{
		m_listCamEffect.pop_front();
	}
}

void CCamera::TransformRenderPos() const
{
	Mgr(CCore)->ScaleTransform(m_fCamZoom);
	Mgr(CCore)->TranslateTransform(m_vOriginMid - m_vCurLookAt * m_fCamZoom );
}

void CCamera::TransformRenderPos(HDC _dc) const
{
	Mgr(CCore)->ScaleTransform(_dc, m_fCamZoom);
	Mgr(CCore)->TranslateTransform(_dc, m_vOriginMid - m_vCurLookAt * m_fCamZoom );
}

void CCamera::ResetRenderPos() const
{
	Mgr(CCore)->ResetTransform();
}

void CCamera::ResetRenderPos(HDC _dc) const
{
	Mgr(CCore)->ResetTransform(_dc);
}

pair<Vec2, Vec2> CCamera::GetRenderPos(const CObject* const _pObj) const
{
	Vec2 vScale = _pObj->GetScale() ;
	Vec2 vLtPos =   (m_vCurLookAt * (1.f - m_fCamZoom) + _pObj->GetPos() * m_fCamZoom) -vScale/2.f - m_vDiff;
	return std::make_pair(vLtPos, vScale);
}



void CCamera::renderBackGround(HDC _hDest,HDC _hSrc,Vec2 _vLayerScale, float _fSpeed) const
{	
	const short iBackWidth = (short)_vLayerScale.x;
	const short iBackHeight = (short)_vLayerScale.y;
	const short iLeft = ((short)(m_vDiff.x * _fSpeed) % iBackWidth + iBackWidth) % iBackWidth;
	const short iTop = (short)(m_vCurLookAt.y * _fSpeed - m_vResolution.y/2);


	/*TransparentBltSafe(_hDest
		, 0
		, 0
		, (int)m_vResolution.x
		, (int)m_vResolution.y 
		, _hSrc
		, iLeft
		, iTop
		, (int)_vLayerScale.x
		, (int)(m_vResolution.y / _fSpeed)
		, (int)_vLayerScale.x * 2
		, (int)_vLayerScale.y
		, RGB(255, 0, 255));

	TransparentBltSafe(_hDest
		, iLeft + (int)m_vResolution.x
		, 0
		, (int)m_vResolution.x
		, (int)m_vResolution.y 
		, _hSrc
		, 0
		, iTop
		, (int)m_vResolution.x
		, (int)(m_vResolution.y / _fSpeed)
		, (int)_vLayerScale.x * 2
		, (int)_vLayerScale.y 
		, RGB(255, 0, 255));*/
	
	TransparentBltSafe(_hDest
		, 0
		, 0
		, static_cast<short>(m_vResolution.x)
		, static_cast<short>(m_vResolution.y)
		, _hSrc
		, static_cast<short>(m_vDiff.x * _fSpeed)
		, static_cast<short>(m_vDiff.y * _fSpeed)
		, static_cast<short>(m_vResolution.x)
		, static_cast<short>((m_vResolution.y))//static_cast<short>((m_vResolution.y / _fSpeed))
		, static_cast<short>(_vLayerScale.x * 10.f)
		, static_cast<short>(_vLayerScale.y)
		, RGB(255, 0, 255)); 

}

void CCamera::update()
{
	if (m_pTargetObj)
	{
		
		if (m_pTargetObj->IsDead())
		{
			m_pTargetObj = nullptr;
		}
		else
		{
			const Vec2 vLookAt = m_pTargetObj->GetPos();
			int xint = static_cast<int>(floorf(vLookAt.x));
			int yint = static_cast<int>(floorf(vLookAt.y));;
			xint = xint - xint % 2;
			yint = yint - yint % 2;
			SetCamRect(Vec2(static_cast<float>(xint), static_cast<float>(yint)));
		}
	}

	/*if (KEY_HOLD(KEY::W))
		m_vLookAt.y -= 50. * DT;
	if (KEY_HOLD(KEY::S))
		m_vLookAt.y += 50. * DT;
	if (KEY_HOLD(KEY::A))
		m_vLookAt.x -= 50. * DT;
	if (KEY_HOLD(KEY::D))
		m_vLookAt.x += 50. * DT;*/

	
	if (ShakeFlag)
	{
		Vec2 vPrev = m_vLookAt;
		ShakeFlag = true;
		if (0. >= m_fShakeAcc)
		{
			Shake = 4;
			m_fShakeAcc = 0.1f;
			ShakeFlag = false;
			if (m_pTargetObj)
			{
				//SetLookAt(m_pTargetObj->GetPos());
				SetCamRect(m_pTargetObj->GetPos());
			}
			else
			{
				//Vec2 vRes = CCore::GetInst()->GetResolution();
				//SetLookAt(vRes / 2.f);
				SetNowLookAt(vPrev);
				//SetCamRect(Mgr(CCore)->GetResolutionV()/2.f);
			}
		}
		else
		{
			m_fShakeAcc -= DT;
			++Shake;
			Shake = wrapAround(Shake, 1, 5);
			switch (Shake % 4)
			{
			case 0:
				m_vLookAt.y -= 5.f;
				break;		   
			case 1:			   
				m_vLookAt.x -= 5.f;
				break;		   
			case 2:			   
				m_vLookAt.y += 5.f;
				break;		   
			case 3:			   
				m_vLookAt.x += 5.f;
				break;
			default:
				break;
			}
		}
	}
	

	CalDiff();
}

CoRoutine CCamera::CamMoveCoRoutine(Vec2 _vDest)
{
	auto pTarget = m_pTargetObj;
	m_pTargetObj = nullptr;
	
	while (m_bMoveFlag)
	{
		SetLookAt(_vDest);
		co_await std::suspend_always{};
	}
	m_pTargetObj = pTarget;
	co_return;
}

CoRoutine CCamera::ZoomInBoss(const Vec2 _vBossPos)
{
	float fAccTime = 0.f;
	m_fDestSpeed = 1000.f;
	const auto vReturnPos = m_vCurLookAt;
	m_bMoveFlag = true;
	StartCoEvent(CamMoveCoRoutine(_vBossPos));
	while (m_bMoveFlag)
	{
		m_fCamZoom += 0.002f;
		co_await std::suspend_always{};
	}
	m_bMoveFlag = true;
	while (1.f >= fAccTime)
	{
		fAccTime += DT;
		m_bMoveFlag = true;
		co_await std::suspend_always{};
	}
	
	StartCoEvent(CamMoveCoRoutine(vReturnPos));
	
	while (m_bMoveFlag)
	{
		m_fCamZoom -= 0.002f;
		co_await std::suspend_always{};
	}
	m_fCamZoom = 1.f;
	m_fDestSpeed = m_fOriginSpeed;
	m_bMoveFlag = true;
	co_return;
}

void CCamera::CalDiff()
{
	if (m_bMoveFlag)
	{
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;

		m_fAccTime += DT;
		m_fSpeed -= m_fAccel * DT;
		if (m_pTargetObj)
		{
			m_vCurLookAt = m_vLookAt;
			m_bMoveFlag = false;
		}
		else
		{
			if (0. >= m_fSpeed)
			{
				m_vCurLookAt = m_vLookAt;
				m_fSpeed = m_fOriginSpeed;
				m_fAccTime = 0.f;
				m_bMoveFlag = false;
			}
			else
			{
				Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;
				if (!vLookDir.IsZero())
				{
					m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * DT;
				}
				else
				{
					m_vCurLookAt = m_vLookAt;
					m_fSpeed = m_fOriginSpeed;
					m_fAccTime = 0.f;
				}
			}
		}
	}
	else
	{
		m_vCurLookAt = m_vLookAt;
	}
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2.f;
	Vec2 vCurLookLT = m_vCurLookAt - vResolution / 2;
	Vec2 vCurLookRB = m_vCurLookAt + vResolution / 2;
	//m_vCurLookAt.x = m_vCurLookAt.x * m_dCamZoom + (FLOAT)(vCurLookLT.x) * 2.f * (1.f - m_dCamZoom);
	//m_vCurLookAt.y = m_vCurLookAt.y * m_dCamZoom + (FLOAT)(vCurLookLT.y) * 2.f * (1.f - m_dCamZoom);
	
	
	m_vDiff = m_vCurLookAt - vCenter; 

	
	m_vPrevLookAt = m_vCurLookAt;
}
