#include "pch.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CAnimation.h"

CAnimator::CAnimator()
{
}

CAnimator::~CAnimator()
{
}

void CAnimator::CreateAnimation(wstring_view _strName, wstring_view _strFileName, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	CAnimation* pAnim = FindAnimation(_strName).value_or(nullptr);
	assert(!pAnim);
	pAnim = new CAnimation;
	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	_vSliceSize.x -= 1;
	pAnim->Create(_strFileName, _vLT, _vSliceSize, _vStep, _fDuration, _iFrameCount);
	m_mapAnim.insert(make_pair(_strName, pAnim));
}

optional<CAnimation*> CAnimator::FindAnimation(wstring_view _strName)
{
	auto iter = m_mapAnim.find(_strName.data());

	if (iter == m_mapAnim.end())
	{
		return std::nullopt;
	}

	return iter->second.get();
}
	
void CAnimator::Play(wstring_view _strName, bool _bRepeat)
{
	if (m_pCurAnim)
	{
		m_pCurAnim->SetFrame(0);
	}
	m_pCurAnim = FindAnimation(_strName).value_or(nullptr);
	m_pCurAnim->m_bFinish = false;
	m_bRepeat = _bRepeat;
}

void CAnimator::component_update()
{
	if (m_pCurAnim)
	{
		m_pCurAnim->update();

		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::component_render(HDC _dc)const
{
	if (m_pCurAnim)
	{
		m_pCurAnim->render(_dc,m_iAnimDir);
	}
}

bool CAnimator::IsFinish() const
{
	return !m_bRepeat && m_pCurAnim->IsFinish();
}

void CAnimator::LoadAnimation(wstring_view _strRelativePath)
{
}

void CAnimator::SaveAnimation(wstring_view _strRelativePath)
{
}

CAnimator::CAnimator(const CAnimator& _other)
	:m_pCurAnim{ nullptr }
	, m_bRepeat{ _other.m_bRepeat }
{
	for (const auto& [name, anim] : _other.m_mapAnim)
	{
		auto iter = m_mapAnim.insert(make_pair(name, new CAnimation{ *anim }));
		iter.first->second->m_pAnimator = this;
	}
	//m_pCurAnim = _other.m_pCurAnim;
}