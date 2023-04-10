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
	Vec2 vPos = CCore::GetInst()->GetResolution();
	m_vLookAt = m_vCurLookAt = m_vPrevLookAt = vPos / 2.;
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
		if (0. >= m_dShakeAcc)
		{
			m_dShakeAcc = 1.;
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
			m_dShakeAcc -= DT;
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
	CalDiff();
}

void CCamera::CalDiff()
{
	if (m_bMoveFlag)
	{
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;

		m_dAccTime += DT;
		m_dSpeed -= m_dAccel * DT;
		if (m_pTargetObj)
		{
			m_vCurLookAt = m_vLookAt;
			m_bMoveFlag = false;
		}
		else
		{
			if (0. >= m_dSpeed)
			{
				m_vCurLookAt = m_vLookAt;
				m_dSpeed = 3000.;
				m_dAccTime = 0.;
				m_bMoveFlag = false;
			}
			else
			{
				Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;
				if (!vLookDir.IsZero())
				{
					m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_dSpeed * DT;
				}
				else
				{
					m_vCurLookAt = m_vLookAt;
					m_dSpeed = 3000.;
					m_dAccTime = 0.;
				}
			}
		}
	}
	else
	{
		m_vCurLookAt = m_vLookAt;
	}
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2.;
	m_vDiff = m_vCurLookAt - vCenter; 
	m_vPrevLookAt = m_vCurLookAt;
}