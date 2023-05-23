#include "pch.h"
#include "CCore.h"
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CObject.h"
#include "CKeyMgr.h"




CCamera::CCamera()
{

}

CCamera::~CCamera()
{

}

void CCamera::init()
{
	m_vResolution = CCore::GetInst()->GetResolutionV();
	m_vOriginMid = m_vLookAt = m_vCurLookAt = m_vPrevLookAt = m_vResolution / 2.;
}

void CCamera::TransformRenderPos() const
{
	Mgr(CCore)->ScaleTransform(m_fCamZoom);
	Mgr(CCore)->TranslateTransform(m_vOriginMid- m_vCurLookAt * m_fCamZoom );
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
	Vec2 vScale = _pObj->GetScale() * m_fCamZoom;
	Vec2 vLtPos =   (m_vCurLookAt * (1.f - m_fCamZoom) + _pObj->GetPos() * m_fCamZoom) -vScale/2.f - m_vDiff;
	return std::make_pair(vLtPos, vScale);
}



void CCamera::renderBackGround(HDC _hDest,HDC _hSrc,Vec2 _vLayerScale, float _fSpeed) const
{	
	const int iBackWidth = (int)_vLayerScale.x;
	const int iBackHeight = (int)_vLayerScale.y;
	const int iLeft = ((int)(m_vDiff.x * _fSpeed) % iBackWidth + iBackWidth) % iBackWidth;
	const int iTop = (int)(m_vCurLookAt.y - m_vResolution.y/2);

	TransparentBltSafe(_hDest
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
			m_vLookAt = m_pTargetObj->GetPos();
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

	
	if (KEY_TAP(KEY::TAB) || ShakeFlag)
	{
		Vec2 vPrev = m_vLookAt;
		ShakeFlag = true;
		if (0. >= m_fShakeAcc)
		{
			m_fShakeAcc = 1.f;
			ShakeFlag = false;
			if (m_pTargetObj)
			{
				SetLookAt(m_pTargetObj->GetPos());
			}
			else
			{
				//Vec2 vRes = CCore::GetInst()->GetResolution();
				//SetLookAt(vRes / 2.);
				SetLookAt(vPrev);
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
				m_vLookAt.y -= 20.;
				break;		   
			case 1:			   
				m_vLookAt.x -= 20.;
				break;		   
			case 2:			   
				m_vLookAt.y += 20.;
				break;		   
			case 3:			   
				m_vLookAt.x += 20.;
				break;
			default:
				break;
			}
		}
	}
	if (KEY_HOLD(KEY::UP))
		m_fCamZoom += 0.01f;
	if (KEY_HOLD(KEY::DOWN))
		m_fCamZoom -= 0.01f;
	CalDiff();
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
				m_fSpeed = 3000.f;
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
					m_fSpeed = 3000.f;
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
	//m_vCurLookAt.x = m_vCurLookAt.x * m_dCamZoom + (FLOAT)(vCurLookLT.x) * 2.f * (1.f - m_dCamZoom);
	//m_vCurLookAt.y = m_vCurLookAt.y * m_dCamZoom + (FLOAT)(vCurLookLT.y) * 2.f * (1.f - m_dCamZoom);

	m_vDiff = m_vCurLookAt - vCenter; 
	
	m_vPrevLookAt = m_vCurLookAt;
}